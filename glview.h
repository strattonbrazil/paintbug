#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QOpenGLFramebufferObject>
#include <QColorDialog>
#include <QTimer>
#include <QTime>

#include "transformable.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "paintutils.h"
#include "glresourcecontext.h"

#define PAINT_FBO_WIDTH 2048

class GLView
{
public:
    explicit GLView();

    virtual void glPass(GLResourceContext &ctx) = 0;
    virtual void painterPass(QPainter* painter) = 0;
    virtual QString viewLabel() = 0;

    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseDragEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

    void setSize(int width, int height) { _width = width; _height = height; }
    void setWidth(int width) { _width = width; }
    void setHeight(int height) { _height = height; }
    int width() { return _width; }
    int height() { return _height; }

    bool hasMeshTexture(Mesh* mesh);
    GLuint meshTexture(Mesh* mesh);
    void setMeshTexture(Mesh* mesh, GLuint id);
signals:

public slots:
    void messageTimerUpdate();
protected:
    QOpenGLFramebufferObject* transferFbo();
    QOpenGLFramebufferObject* paintFbo();

    Camera* _camera;
    QList<Point2>             _strokePoints;
    bool                      _bakePaintLayer;
    CameraScratch             _cameraScratch;
    QRect                     _brushColorRect;
    QColor                    _brushColor;

    QOpenGLFramebufferObject* _transferFbo;
    QOpenGLFramebufferObject* _paintFbo;

    void drawPaintStrokes();
    void drawPaintLayer();

    void setBusyMessage(QString message, int duration);

private:
    void                     bakePaintLayer();

    QTimer _messageTimer;
    QString _busyMessage = "";
    QTime _messageFinished;

    int _width;
    int _height;

};

#endif // GLVIEW_H
