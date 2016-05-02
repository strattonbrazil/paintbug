#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QOpenGLFramebufferObject>
#include <QTimer>
#include <QTime>

#include "transformable.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"

#define PAINT_FBO_WIDTH 2048

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLView(QWidget *parent = 0);

    void initializeGL();
    QGLFormat defaultFormat();
    void resizeGL(int w, int h);
    void paintGL();

    virtual void glPass() = 0;
    virtual void painterPass(QPainter* painter) = 0;

    virtual void mousePressEvent(QMouseEvent* event) = 0;
    virtual void mouseDoubleClickEvent(QMouseEvent *) = 0;
    virtual void mouseReleaseEvent(QMouseEvent* event) = 0;
    virtual void mouseMoveEvent(QMouseEvent* event) = 0;
    virtual void mouseDragEvent(QMouseEvent* event) = 0;
    virtual void keyPressEvent(QKeyEvent* event) = 0;


    bool hasMeshTexture(Mesh* mesh);
    GLuint meshTexture(Mesh* mesh);
    void setMeshTexture(Mesh* mesh, GLuint id);
signals:

public slots:
    void messageTimerUpdate();
protected:
    // shared resources
    QOpenGLFramebufferObject* transferFbo();
    QOpenGLFramebufferObject* paintFbo();

    void setBusyMessage(QString message, int duration);

private:
    QTimer _messageTimer;
    QString _busyMessage = "";
    QTime _messageFinished;
};

#endif // GLVIEW_H
