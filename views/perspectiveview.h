#ifndef PERSPECTIVEVIEW_H
#define PERSPECTIVEVIEW_H

#include <glview.h>

class PerspectiveView : public GLView
{
    Q_OBJECT
public:
    explicit PerspectiveView(QWidget *parent = 0);

    QString getViewLabel();
    MeshPropType meshVertexSpace() { return MeshPropType::GEOMETRY; }
signals:

public slots:
private:
    void                     initializeView();

};

#endif // PERSPECTIVEVIEW_H
