#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QOpenGLFramebufferObject>
#include "transformable.h"
#include "camera.h"
#include "shader.h"

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLView(QWidget *parent = 0);
    void resizeGL(int w, int h);
    void paintGL();

    void                     mousePressEvent(QMouseEvent* event);
    void                     mouseDoubleClickEvent(QMouseEvent *);
    void                     mouseReleaseEvent(QMouseEvent* event);
    void                     mouseMoveEvent(QMouseEvent* event);
    void                     mouseDragEvent(QMouseEvent* event);
    QGLFormat                defaultFormat();
signals:

public slots:

private:
    void                     _drawPaintLayer();

    Transformable*     _camera;
    CameraScratch      _cameraScratch;
    bool               _validShaders;
    bool               _validFbos;
    QGLShaderProgram*  _meshShader;
    QGLShaderProgram*  _paintDebugShader;
    QOpenGLFramebufferObject* _paintFbo;
    QList<Point2>      _strokePoints;
};

#endif // GLVIEW_H
