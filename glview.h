#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QOpenGLFramebufferObject>
#include "transformable.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"

#define PERSPECTIVE_VIEW_TYPE "PERSPECTIVE"
#define UV_VIEW_TYPE "UV"

enum ViewType {
    PERSPECTIVE,
    UV
};

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLView(QWidget *parent = 0);

    void initializeGL();
    QGLFormat defaultFormat();
    void resizeGL(int w, int h);
    void paintGL();

    void                     mousePressEvent(QMouseEvent* event);
    void                     mouseDoubleClickEvent(QMouseEvent *);
    void                     mouseReleaseEvent(QMouseEvent* event);
    void                     mouseMoveEvent(QMouseEvent* event);
    void                     mouseDragEvent(QMouseEvent* event);
    void                     keyPressEvent(QKeyEvent* event);

protected:
    bool event(QEvent *e) override;
signals:

public slots:

private:
    void                     bakePaintLayer();
    void                     drawPaintLayer();
    void                     drawMeshTexture(GLuint meshTexture);
    void                     renderHUD(QPainter &painter);
    void                     initializeView();

    Transformable*            _camera;
    CameraScratch             _cameraScratch;
    bool                      _validShaders;
    bool                      _validFbos;
    QGLShaderProgram*         _meshShader;
    QGLShaderProgram*         _bakeShader;
    QGLShaderProgram*         _paintDebugShader;
    QOpenGLFramebufferObject* _paintFbo;
    QOpenGLFramebufferObject* _transferFbo;
    QList<Point2>             _strokePoints;
    bool                      _bakePaintLayer;
    QHash<Mesh*,GLuint>       _meshTextures; // need to find a better place for this
    QColor                    _brushColor;
    QRect                     _brushColorRect;
};

#endif // GLVIEW_H
