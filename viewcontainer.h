#ifndef VIEWCONTAINER_H
#define VIEWCONTAINER_H

#include <QGLWidget>
#include <QOpenGLDebugLogger>
#include <QHash>

#include "glview.h"
#include "shader.h"
#include "glresourcecontext.h"

enum ViewMode {
    PERSPECTIVE_AND_UV,
    PERSPECTIVE,
    UV,
};

class ViewContainer : public QGLWidget
{
    Q_OBJECT

public:
    explicit ViewContainer(QWidget *parent = 0);

    void initializeGL();
    QGLFormat defaultFormat();
    void resizeGL(int w, int h);
    void paintGL();

    QRect getFullRegion();
    QRect getLeftRegion();
    QRect getRightRegion();

    void drawViewGLPass(QString viewName, QRect region);
    void drawViewPainterPass(QString viewName, QRect region, QPainter* painter);

    QMouseEvent convertToRegion(QMouseEvent* event, QRect region);

    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseDragEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

signals:

public slots:
private:
    ViewMode _viewMode = ViewMode::PERSPECTIVE_AND_UV;

    QOpenGLDebugLogger* _logger;

    GLResourceContext _glResourceContext;
    QHash<QString,QSharedPointer<GLView>> _views;

    QString _viewOwningMouse; // name of view, which has mouse
    QString _lastViewFocused;
    Qt::MouseButton _mousePressButton;
    QRect _owningRegion; // region where mouse was clicked
};

#endif // VIEWCONTAINER_H
