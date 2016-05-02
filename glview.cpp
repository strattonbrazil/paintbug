#include "glview.h"

#include <iostream>


// store the first GL widget and use it as the shared widget
QGLWidget* firstWidget = 0;
QGLWidget* sharedWidget(QGLWidget* view)
{
    if (firstWidget)
        return firstWidget;
    else {
        firstWidget = view;
        return 0;
    }
}

QOpenGLFramebufferObject* sharedTransferFbo = 0;
QOpenGLFramebufferObject* GLView::transferFbo() {
    std::cout << "getting transfer fbo" << std::endl;
    if (!sharedTransferFbo) {
        std::cout << "creating transfer fbo" << std::endl;
        sharedTransferFbo = new QOpenGLFramebufferObject(PAINT_FBO_WIDTH, PAINT_FBO_WIDTH);
    }
    return sharedTransferFbo;
}

QOpenGLFramebufferObject* sharedPaintFbo = 0;
QOpenGLFramebufferObject* GLView::paintFbo() {
    if (!sharedPaintFbo) {
        QOpenGLFramebufferObjectFormat format;
        format.setInternalTextureFormat(GL_RED);
        sharedPaintFbo = new QOpenGLFramebufferObject(PAINT_FBO_WIDTH, PAINT_FBO_WIDTH, format);

        sharedPaintFbo->bind();
        glClearColor(0,0,0,0); // only red is used
        glClear(GL_COLOR_BUFFER_BIT);
        sharedPaintFbo->release();
    }
    return sharedPaintFbo;
}

GLView::GLView(QWidget *parent) :
    QGLWidget(parent, sharedWidget(this))
{
    connect(&_messageTimer, SIGNAL(timeout()), this, SLOT(messageTimerUpdate()));
    _messageTimer.setInterval(100);
}

void GLView::initializeGL()
{
}

QGLFormat GLView::defaultFormat()
{
    QGLFormat format;
    //format.setVersion(3,2);
    format.setAlpha(true);
    format.setStencil(true);
    format.setVersion(3,1);
    format.setProfile(QGLFormat::CoreProfile);
    return format;
}

void GLView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLView::paintGL()
{
    QPainter painter;
    painter.begin(this);
    painter.beginNativePainting();

    glPass();

    painter.endNativePainting();

    painterPass(&painter);

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

QHash<Mesh*,GLuint> meshTextures;


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

// duration in milliseconds
void GLView::setBusyMessage(QString message, int duration)
{
    _messageFinished = QTime::currentTime().addMSecs(duration);
    _busyMessage = message;

    _messageTimer.start();
}
