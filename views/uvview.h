#ifndef UVVIEW_H
#define UVVIEW_H

#include <glview.h>

class UVView : public GLView
{
    Q_OBJECT
public:
    explicit UVView(QWidget *parent = 0);

    QString getViewLabel();
    void glPass();
    void painterPass(QPainter *painter);
    MeshPropType meshBakingSpace() { return MeshPropType::UV; }
signals:

public slots:
private:
    QColor                    _brushColor;
    bool                      _validShaders;
};

#endif // UVVIEW_H
