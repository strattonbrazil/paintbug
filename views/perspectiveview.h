#ifndef PERSPECTIVEVIEW_H
#define PERSPECTIVEVIEW_H

#include <glview.h>

class PerspectiveView : public GLView
{
public:
    explicit PerspectiveView();

    void glPass();
    void painterPass(QPainter *painter);
    QString viewLabel() { return "Perspective"; }
signals:

public slots:
private:
    void                     initializeView();

};

#endif // PERSPECTIVEVIEW_H
