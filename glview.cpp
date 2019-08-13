#include "glview.h"

#include <iostream>

#include "scene.h"
#include "gl_util.h"

#define DEBUG_PAINT_LAYER 0

namespace MouseMode {
    enum { FREE, CAMERA, TOOL, HUD };
}

int mouseMode = MouseMode::FREE;
int activeMouseButton = -1;
QList<GLView*> GLView::_glViews;

QOpenGLFramebufferObject* GLView::transferFbo() {
    if (!_transferFbo) {
        _transferFbo = new QOpenGLFramebufferObject(PAINT_FBO_WIDTH, PAINT_FBO_WIDTH);
    }
    return _transferFbo;
}

QOpenGLFramebufferObject* GLView::paintFbo() {
    if (!_paintFbo) {
        QOpenGLFramebufferObjectFormat format;
        format.setInternalTextureFormat(GL_RED);
        _paintFbo = new QOpenGLFramebufferObject(PAINT_FBO_WIDTH, PAINT_FBO_WIDTH, format);

        _paintFbo->bind();
        glClearColor(0,0,0,0); // only red is used
        glClear(GL_COLOR_BUFFER_BIT);
        _paintFbo->release();
    }
    return _paintFbo;
}

GLView::GLView(QWidget *parent) :
    QOpenGLWidget(parent)
{
    connect(&_messageTimer, SIGNAL(timeout()), this, SLOT(messageTimerUpdate()));
    _messageTimer.setInterval(100);

    _glViews.append(this); // keep track of all views

    _bakePaintLayer = false;
}

void GLView::initializeGL()
{
    _meshShader = ShaderFactory::buildMeshShader(this);
    _bakeShader = ShaderFactory::buildBakeShader(this);    
#if DEBUG_PAINT_LAYER
        _paintDebugShader = ShaderFactory::buildPaintDebugShader(this);
#endif
    _logger = new QOpenGLDebugLogger(this);
    _logger->initialize();
}

void GLView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLView::paintGL()
{
    QList<QOpenGLDebugMessage> messages = _logger->loggedMessages();
    foreach (const QOpenGLDebugMessage &message, messages)
        qDebug() << message;

    QPainter painter;
    painter.begin(this);
    painter.beginNativePainting();

    glPass();

    // draw strokes onto paint FBO
    drawPaintStrokes();

#if DEBUG_PAINT_LAYER
    drawPaintLayer();
#endif

    if (_bakePaintLayer) {
        bakePaintLayer();
        setBusyMessage("baking", 400);
    }

    painter.endNativePainting();

    painterPass(&painter);

    drawOutlinedText(&painter, 20, 20, getViewLabel(), QColor(0,0,0), QColor(255,255,255));

    if (_messageTimer.isActive()) {
        QFont prevFont = painter.font();
        QFont bakingFont(prevFont.family(), 20);
        QFontMetrics fm(bakingFont);
        painter.setFont(bakingFont);

        const int bottomOffset = 50;
        QRect bgRect(0, height() - bottomOffset, width(), fm.height());

        painter.fillRect(bgRect, QColor(0,0,0, 200));
        painter.drawText((width()-fm.width(_busyMessage)) / 2, bgRect.y() + bgRect.height() - fm.descent() - 4, _busyMessage);

        painter.setFont(prevFont);
    }

    painter.end();
}

void GLView::drawOutlinedText(QPainter* painter, int x, int y, QString text, QColor bgColor, QColor fgColor)
{
    painter->setPen(bgColor);
    painter->drawText(x, y, text);
    painter->setPen(fgColor);
    painter->drawText(x-2, y-2, text);
}

static QHash<Mesh*,GLuint> meshTextures;


bool GLView::hasMeshTexture(Mesh *mesh)
{
    return meshTextures.contains(mesh);
}

GLuint GLView::meshTexture(Mesh *mesh)
{
    return meshTextures[mesh];
}

void GLView::setMeshTexture(Mesh *mesh, GLuint id)
{
    meshTextures[mesh] = id;
}


void GLView::messageTimerUpdate()
{
    update();

    if (_messageFinished < QTime::currentTime())
        _messageTimer.stop();
}

void GLView::drawPaintStrokes()
{
    paintFbo()->bind();
    glViewport(0,0,PAINT_FBO_WIDTH,PAINT_FBO_WIDTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, PAINT_FBO_WIDTH, 0, PAINT_FBO_WIDTH, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPointSize(20.0f);
    glBegin(GL_POINTS);
    glColor4f(1,1,1,1);
    foreach (Point2 p, _strokePoints) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();
    glViewport(0,0,width(),height());
    paintFbo()->release();
}

void GLView::drawPaintLayer()
{
    QMatrix4x4 cameraProjViewM;
    cameraProjViewM.ortho(0, width(), 0, height(), -1, 1);

    _paintDebugShader->bind();
    _paintDebugShader->setUniformValue("cameraPV", cameraProjViewM);
    _paintDebugShader->setUniformValue("paintTexture", 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, paintFbo()->texture());

    drawTexturedRect(0, 0, PAINT_FBO_WIDTH, PAINT_FBO_WIDTH);

    glDisable(GL_BLEND);
    _paintDebugShader->release();
}

// duration in milliseconds
void GLView::setBusyMessage(QString message, int duration)
{
    _messageFinished = QTime::currentTime().addMSecs(duration);
    _busyMessage = message;

    _messageTimer.start();
}

void GLView::bakePaintLayer()
{
    Scene* scene = Scene::activeScene();

    transferFbo()->bind();

    glViewport(0, 0, 256, 256);

    QMatrix4x4 cameraProjM = _camera->getProjMatrix(width(), height());
    QMatrix4x4 cameraViewM = _camera->getViewMatrix(width(), height());
    QMatrix4x4 cameraProjViewM = cameraProjM * cameraViewM;

    QMatrix4x4 orthoProjViewM;
    orthoProjViewM.ortho(0,1,0,1,-1,1);

    // render the meshes in UV space onto their texture using the paintFBO
    // render each mesh
    QHashIterator<QString,Mesh*> meshes = scene->meshes();
    while (meshes.hasNext()) {
        meshes.next();
        Mesh* mesh = meshes.value();

        QMatrix4x4 objToWorld;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, meshTexture(mesh));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, paintFbo()->texture());
        glActiveTexture(GL_TEXTURE0);

        QVector2D targetScale = QVector2D(width() / (float)PAINT_FBO_WIDTH, height() / (float)PAINT_FBO_WIDTH);

        //QColor _brushColor(255,0,0);

        _bakeShader->bind();
        _bakeShader->setUniformValue("objToWorld", objToWorld);
        _bakeShader->setUniformValue("orthoPV", orthoProjViewM);
        _bakeShader->setUniformValue("cameraPV", cameraProjViewM);
        _bakeShader->setUniformValue("meshTexture", 0);
        _bakeShader->setUniformValue("paintTexture", 1);
        _bakeShader->setUniformValue("targetScale", targetScale);
        _bakeShader->setUniformValue("brushColor", _brushColor.redF(), _brushColor.greenF(), _brushColor.blueF(), 1);

        renderMesh(mesh, meshBakingSpace(), MeshPropType::UV);

        _bakeShader->release();

        // copy bake back into mesh texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, meshTexture(mesh));
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 256, 256, 0);
    }

    transferFbo()->release();

    // clear paint buffer
    paintFbo()->bind();
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    paintFbo()->release();

    _strokePoints.clear();

    glViewport(0, 0, width(), height());

    // redraw other views that may be using texture
    foreach (GLView* view, _glViews) {
        if (view != this) {
            view->update();
        }
    }

    _bakePaintLayer = false;
}

void GLView::mousePressEvent(QMouseEvent* event)
{
    // handle future keyboard widgets with this
    this->setFocus();

    //bool altDown = event->modifiers() & Qt::AltModifier;
    bool camDown = event->modifiers() & Qt::AltModifier;

    if (mouseMode == MouseMode::FREE && camDown) {
        mouseMode = MouseMode::CAMERA;
        activeMouseButton = event->button();
        _camera->mousePressed(_cameraScratch, event);
    }
    else if (mouseMode == MouseMode::FREE && event->button() & Qt::LeftButton) {
        if (_brushColorRect.contains(event->pos())) { // HUD?
            mouseMode = MouseMode::HUD;
            activeMouseButton = event->button();
        } else {
            _strokePoints.append(Point2(event->pos().x(), height()-event->pos().y()));
            mouseMode = MouseMode::TOOL;
            activeMouseButton = event->button();
        }
    }
}

void GLView::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void GLView::mouseReleaseEvent(QMouseEvent* event)
{
    //CursorTool* cursorTool = SunshineUi::cursorTool();

    if (mouseMode == MouseMode::CAMERA && event->button() == activeMouseButton) {
        mouseMode = MouseMode::FREE;
        activeMouseButton = -1;
        _camera->mouseReleased(_cameraScratch, event);
    }
    else if (mouseMode == MouseMode::HUD && event->button() == activeMouseButton) {
        if (_brushColorRect.contains(event->pos())) { // HUD?
            _brushColor = QColorDialog::getColor(_brushColor, this, "Brush Color");
        }

        mouseMode = MouseMode::FREE;
        activeMouseButton = -1;
    }
    else if (mouseMode == MouseMode::TOOL && event->button() == activeMouseButton) {
        mouseMode = MouseMode::FREE;
        activeMouseButton = -1;
    }

    update();
}

void GLView::mouseMoveEvent(QMouseEvent* event)
{
    if (mouseMode == MouseMode::TOOL) {
        mouseDragEvent(event);

        //_workTool->mouseMoved(event);

        update();
    }
    else if (mouseMode != MouseMode::FREE) {
        mouseDragEvent(event);
    }
}

void GLView::mouseDragEvent(QMouseEvent* event)
{
    if (mouseMode == MouseMode::CAMERA) {
        _camera->mouseDragged(_cameraScratch, event);
    }
    else if (mouseMode == MouseMode::TOOL) {
        _strokePoints.append(Point2(event->pos().x(), height()-event->pos().y()));
    }

    update();
}

void GLView::keyPressEvent(QKeyEvent *event)
{
    if (mouseMode == MouseMode::FREE) {
        if (event->key() == Qt::Key_Space) {
            _bakePaintLayer = true;
            update();
        }
    }

    // TODO: call base class if not using this event
}
