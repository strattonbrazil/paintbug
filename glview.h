#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QOpenGLFramebufferObject>
#include <QColorDialog>
#include <QTimer>
#include <QTime>
#include <QOpenGLDebugLogger>

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

    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseDragEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);


    bool hasMeshTexture(Mesh* mesh);
    GLuint meshTexture(Mesh* mesh);
    void setMeshTexture(Mesh* mesh, GLuint id);
signals:

public slots:
    void messageTimerUpdate();
protected:
    QOpenGLFramebufferObject* transferFbo();
    QOpenGLFramebufferObject* paintFbo();

    QGLShaderProgram*         _meshShader;
    QGLShaderProgram*         _bakeShader;
    QGLShaderProgram*         _paintDebugShader;

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
    QOpenGLDebugLogger* _logger;

};

#endif // GLVIEW_H
