#ifndef UVVIEW_H
#define UVVIEW_H

#include <glview.h>

class UVView : public GLView
{
public:
    explicit UVView();

    void glPass(GLResourceContext &ctx);
    void painterPass(QPainter *painter);
    QString viewLabel() { return "UV"; }
signals:

public slots:
private:
    QColor                    _brushColor;
    bool                      _validShaders;
};

#endif // UVVIEW_H
