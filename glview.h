#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFramebufferObject>
#include <QColorDialog>
#include <QTimer>
#include <QTime>
#include <QOpenGLDebugLogger>

#include "transformable.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "constants.h"

#define PAINT_FBO_WIDTH 2048

class GLView : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GLView(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void drawOutlinedText(QPainter* painter, int x, int y, QString text, QColor bgColor, QColor fgColor);
    void drawBrush();

    virtual void glPass() = 0;
    virtual void painterPass(QPainter* painter) = 0;
    virtual QString getViewLabel() = 0;
    virtual MeshPropType meshBakingSpace() = 0; // returns the space the geometry is rendered in

    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseDragEvent(QMouseEvent* event);
    void leaveEvent(QEvent* event);
    void keyPressEvent(QKeyEvent* event);


    bool hasMeshTexture(Mesh* mesh);
    GLuint meshTextureId(Mesh* mesh);
    GLuint meshTextureSize(Mesh* mesh);
    void setMeshTexture(Mesh* mesh, GLuint id, GLuint size);
signals:

public slots:
    void messageTimerUpdate();
protected:
    // shared GL resources
    QOpenGLFramebufferObject* transferFbo();
    QOpenGLFramebufferObject* paintFbo();

    QOpenGLFramebufferObject* _transferFbo = 0;
    QOpenGLFramebufferObject* _paintFbo = 0;

    // non-shared GL resources
    QGLShaderProgram*         _meshShader;
    QGLShaderProgram*         _bakeShader;
    QGLShaderProgram*         _paintDebugShader;

    Camera* _camera;
    QList<Point2>             _strokePoints;
    bool                      _bakePaintLayer;
    CameraScratch             _cameraScratch;

    static QList<GLView*> _glViews;

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
