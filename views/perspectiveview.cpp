#include <QMatrix4x4>
#include <QOpenGLFramebufferObjectFormat>
#include <QColorDialog>
#include <QMetaEnum>
#include <GL/glext.h>

#include "perspectiveview.h"
#include "scene.h"
#include "gl_util.h"

#define DEBUG_PAINT_LAYER 0
#define DRAW_MESH_TEXTURE 1

namespace MouseMode {
    enum { FREE, CAMERA, TOOL, HUD };
}

int mouseMode = MouseMode::FREE;
int activeMouseButton = -1;

PerspectiveView::PerspectiveView(QWidget *parent) :
    GLView(parent)
{
    _camera = new PerspectiveCamera();
    _validShaders = false;
    _bakePaintLayer = false;
    _brushColor = QColor(255,0,0);
}

void PerspectiveView::glPass()
{
    if (!_validShaders) {
        _meshShader = ShaderFactory::buildMeshShader(this);
        _bakeShader = ShaderFactory::buildBakeShader(this);
#if DEBUG_PAINT_LAYER
        _paintDebugShader = ShaderFactory::buildPaintDebugShader(this);
#endif
        _validShaders = true;
    }

    //if (_bakePaintLayer)
        //bakePaintLayer();

    glEnable(GL_DEPTH_TEST);

    glClearColor(.2,.2,.2,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 cameraProjM = _camera->getProjMatrix(width(), height());
    QMatrix4x4 cameraViewM = _camera->getViewMatrix(width(), height());
    QMatrix4x4 cameraProjViewM = cameraProjM * cameraViewM;
    QMatrix4x4 objToWorld;

    // TODO: is this line needed still?
    glBindTexture(GL_TEXTURE_2D, paintFbo()->texture());


    // used for non-shader drawing later
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(cameraProjM.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(cameraViewM.data());

    Scene* scene = Scene::activeScene();

    // render each mesh
    QHashIterator<QString,Mesh*> meshes = scene->meshes();
    while (meshes.hasNext()) {
        meshes.next();
        Mesh* mesh = meshes.value();

        // make sure a texture exists for this mesh
        if (!hasMeshTexture(mesh)) {
            std::cout << "creating mesh texture" << std::endl;

            transferFbo()->bind();

            GLuint textureId;
            glGenTextures(1, &textureId);



            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glClearColor(.5,.5,.5,1);
            glClear(GL_COLOR_BUFFER_BIT);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0,1,0,1,-1,1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glViewport(0, 0, 256, 256);

            glColor3f(.8,.8,.8);
            glBegin(GL_QUADS);
            {
                glVertex2f(.25,.25);
                glVertex2f(.75,.25);
                glVertex2f(.75,.75);
                glVertex2f(.25,.75);
            }
            glEnd();

            glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 256, 256, 0);


            transferFbo()->release();

            //glActiveTexture(GL_TEXTURE0);
            //QImage img("/tmp/lena.jpg");
            //QImage image = QGLWidget::convertToGLFormat(img);
            //glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , image.width(), image.height(),  GL_RGB, GL_UNSIGNED_BYTE, image.bits() );


            glViewport(0, 0, width(), height());

            setMeshTexture(mesh, textureId);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, meshTexture(mesh));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, paintFbo()->texture());
        glActiveTexture(GL_TEXTURE0);

        _meshShader->bind();
        _meshShader->setUniformValue("objToWorld", objToWorld);
        _meshShader->setUniformValue("cameraPV", cameraProjViewM);
        _meshShader->setUniformValue("paintFboWidth", PAINT_FBO_WIDTH);
        _meshShader->setUniformValue("brushColor", _brushColor.redF(), _brushColor.greenF(), _brushColor.blueF(), 1);
        _meshShader->setUniformValue("meshTexture", 0);
        _meshShader->setUniformValue("paintTexture", 1);

        renderMesh(mesh);

        _meshShader->release();

        if (_bakePaintLayer) {
            bakePaintLayer();
            setBusyMessage("baking", 400);
        }
    }


#if DRAW_MESH_TEXTURE
    //drawMeshTexture();
#endif

    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        glVertex3f(-10, 0, i);
        glVertex3f(10, 0, i);
    }
    glEnd();

    glDisable(GL_DEPTH_TEST);

    // draw strokes onto paint FBO
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

#if DEBUG_PAINT_LAYER
    drawPaintLayer();
#endif

}

void drawOutlinedText(QPainter* painter, int x, int y, const char* text, QColor bgColor, QColor fgColor)
{
    painter->setPen(bgColor);
    painter->drawText(x, y, text);
    painter->setPen(fgColor);
    painter->drawText(x-2, y-2, text);
}

void PerspectiveView::painterPass(QPainter* painter)
{
    // render HUD
    //
    drawOutlinedText(painter, 20, height()-20, "Brush Color", QColor(0,0,0), QColor(255,255,255));

    QFontMetrics fm(painter->font());
    const int textHeight = fm.height();
    _brushColorRect = QRect(20, height()-120-textHeight, 100, 100);
    painter->fillRect(_brushColorRect, _brushColor);


}


void PerspectiveView::bakePaintLayer()
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

        _bakeShader->bind();
        _bakeShader->setUniformValue("objToWorld", objToWorld);
        _bakeShader->setUniformValue("orthoPV", orthoProjViewM);
        _bakeShader->setUniformValue("cameraPV", cameraProjViewM);
        _bakeShader->setUniformValue("meshTexture", 0);
        _bakeShader->setUniformValue("paintTexture", 1);
        _bakeShader->setUniformValue("targetScale", targetScale);
        _bakeShader->setUniformValue("brushColor", _brushColor.redF(), _brushColor.greenF(), _brushColor.blueF(), 1);

        renderMesh(mesh, MeshPropType::GEOMETRY, MeshPropType::UV);

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

    _bakePaintLayer = false;
}

void PerspectiveView::drawPaintLayer()
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

void PerspectiveView::drawMeshTexture(GLuint meshTexture)
{
    QMatrix4x4 cameraProjViewM;
    cameraProjViewM.ortho(0, width(), 0, height(), -1, 1);

    _paintDebugShader->bind();
    _paintDebugShader->setUniformValue("cameraPV", cameraProjViewM);
    _paintDebugShader->setUniformValue("paintTexture", 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, meshTexture);

    drawTexturedRect(20, 20, 256, 256);

    glDisable(GL_BLEND);
    _paintDebugShader->release();
}

void PerspectiveView::mousePressEvent(QMouseEvent* event)
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
        /*
        if (_workTool != 0) {
            mouseMode = MouseMode::TOOL;
            activeMouseButton = event->button();
            _workTool->mousePressed(this, event);
        }
        else {
            mouseMode = MouseMode::TOOL;
            activeMouseButton = event->button();
            CursorTool* cursorTool = SunshineUi::cursorTool();
            cursorTool->mousePressed(this, event);
        }
        */
    }
}

void PerspectiveView::mouseDoubleClickEvent(QMouseEvent *event)
{
#if 0
    if (mouseMode == MouseMode::FREE && event->button() & Qt::LeftButton) {

        CursorTool* cursorTool = SunshineUi::cursorTool();
        cursorTool->mouseDoubleClicked(this, event);
        /*
        mouseMode = MouseMode::TOOL;
        activeMouseButton = event->button();
        //basicSelect = BasicSelect.instance
        //basic_select.mousePressed(self,event)
        //_basicSelect->mousePressed(this, event);
        CursorTool* cursorTool = SunshineUi::cursorTool();
        cursorTool->mousePressed(this, event);
        */
    }
#endif
}

void PerspectiveView::mouseReleaseEvent(QMouseEvent* event)
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

        //cursorTool->mouseReleased(this, event);
    }
        /*
    else if (_workTool != 0) {
        if (_workTool->ravageMouse()) {
            if (_workTool != 0 && event->button() & Qt::LeftButton) {
                _workTool->finish(event);
                _workTool = 0;
            }
            else if (_workTool != 0 && event->button() & Qt::RightButton) {
                _workTool->cancel(event);
                _workTool = 0;
            }
        } else {
            _workTool->mouseReleased(this, event);
        }
        mouseMode = MouseMode::FREE;
    }
    else if (mouseMode == MouseMode::FREE && event->button() == Qt::RightButton && cursorTool->canMouseCancel(this)) {
        cursorTool->cancel(this);
    }
    else if (mouseMode == MouseMode::FREE && event->button() == Qt::RightButton) { // popup menu
        showContextMenu(event);
    }
    */

    update();
}

void PerspectiveView::mouseMoveEvent(QMouseEvent* event)
{
    if (mouseMode == MouseMode::TOOL) {
        mouseDragEvent(event);

        //_workTool->mouseMoved(event);


        update();
    }
    if (mouseMode == MouseMode::FREE) {
        /*
        // calculate preselection
        Point3 rayOrig = camera()->eye();
        Vector3 rayDir = computeRayDirection(event->pos());
        //FaceUtil::FaceHit faceHit = FaceUtil::closestFace(_scene, rayOrig, rayDir, false);

        QList<Triangle> triangles = _meshGrid.trianglesByPoint(QPoint(event->pos().x(), height()-event->pos().y()));
        FaceUtil::FaceHit faceHit = FaceUtil::closestFace(triangles, rayOrig, rayDir, false);
        VertexUtil::VertexHit vertexHit = VertexUtil::closestVertex(this, event, false);

        _hoverMesh = faceHit.nearMesh ? faceHit.nearMesh : 0;
        if (faceHit.nearFace.is_valid()) _hoverFace = faceHit.nearFace;
        else _hoverFace.invalidate();

        if (vertexHit.vertex.is_valid()) _hoverVert = vertexHit.vertex;
        else _hoverVert.invalidate();

        CursorTool* cursorTool = SunshineUi::cursorTool();
        cursorTool->mouseMoved(this, event);
        */

        update();
    }
    else {
        mouseDragEvent(event);
    }
}

void PerspectiveView::mouseDragEvent(QMouseEvent* event)
{
    if (mouseMode == MouseMode::CAMERA) {
        _camera->mouseDragged(_cameraScratch, event);

    }
    else if (mouseMode == MouseMode::TOOL) {
        _strokePoints.append(Point2(event->pos().x(), height()-event->pos().y()));
        /*
        if (_workTool != 0) {
            // move mouse back to center
                QPoint mouseDiff = QCursor::pos() - centerMouse(TRUE);
                if (mouseDiff != QPoint(0,0)) {
                    _workTool->mouseMoved(this, event, mouseDiff.x(), mouseDiff.y());
                    if (_workTool->ravageMouse())
                        centerMouse(FALSE);
                }

         //   _workTool->mouseMoved(event, 2, 0);
        }
        else {
            CursorTool* cursorTool = SunshineUi::cursorTool();
            cursorTool->mouseDragged(this, event);
        }
        */
    }

    update();

}

void PerspectiveView::keyPressEvent(QKeyEvent *event)
{
    if (mouseMode == MouseMode::FREE) {
        if (event->key() == Qt::Key_Space) {
            _bakePaintLayer = true;
            update();
        }
    }

    // TODO: call base class if not using this event
}
