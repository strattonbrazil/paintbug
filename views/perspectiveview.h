#ifndef PERSPECTIVEVIEW_H
#define PERSPECTIVEVIEW_H

#include <glview.h>

class PerspectiveView : public GLView
{
    Q_OBJECT
public:
    explicit PerspectiveView(QWidget *parent = 0);

    void glPass();
    void painterPass(QPainter *painter);

    void                     mousePressEvent(QMouseEvent* event);
    void                     mouseDoubleClickEvent(QMouseEvent *);
    void                     mouseReleaseEvent(QMouseEvent* event);
    void                     mouseMoveEvent(QMouseEvent* event);
    void                     mouseDragEvent(QMouseEvent* event);
    void                     keyPressEvent(QKeyEvent* event);
signals:

public slots:
private:
    void                     bakePaintLayer();
    void                     drawPaintLayer();
    void                     drawMeshTexture(GLuint meshTexture);
    void                     initializeView();

    PerspectiveCamera*        _camera;
    CameraScratch             _cameraScratch;
    bool                      _validShaders;
    QGLShaderProgram*         _meshShader;
    QGLShaderProgram*         _bakeShader;
    QGLShaderProgram*         _paintDebugShader;
    QList<Point2>             _strokePoints;
    bool                      _bakePaintLayer;
    QColor                    _brushColor;
    QRect                     _brushColorRect;
};

#endif // PERSPECTIVEVIEW_H
