#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QOpenGLFramebufferObject>

#include "transformable.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLView(QWidget *parent = 0);

    void initializeGL();
    QGLFormat defaultFormat();
    void resizeGL(int w, int h);
    virtual void paintGL() = 0;

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

private:

};

#endif // GLVIEW_H
