#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include "transformable.h"
#include "camera.h"

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

signals:

public slots:

private:
    Transformable* _camera;
    CameraScratch            _cameraScratch;
};

#endif // GLVIEW_H
