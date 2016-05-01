#ifndef UVVIEW_H
#define UVVIEW_H

#include <glview.h>

class UVView : public GLView
{
    Q_OBJECT
public:
    explicit UVView(QWidget *parent = 0);

    void paintGL();

    void                     mousePressEvent(QMouseEvent* event);
    void                     mouseDoubleClickEvent(QMouseEvent *);
    void                     mouseReleaseEvent(QMouseEvent* event);
    void                     mouseMoveEvent(QMouseEvent* event);
    void                     mouseDragEvent(QMouseEvent* event);
    void                     keyPressEvent(QKeyEvent* event);
signals:

public slots:
private:
    OrthographicCamera*       _camera;
    QGLShaderProgram*         _meshShader;
    QColor                    _brushColor;
    bool                      _validShaders;
};

#endif // UVVIEW_H
