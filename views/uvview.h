#ifndef UVVIEW_H
#define UVVIEW_H

#include <glview.h>

class UVView : public GLView
{
    Q_OBJECT
public:
    explicit UVView(QWidget *parent = 0);

    QString getViewLabel();
    MeshPropType meshVertexSpace() { return MeshPropType::UV; }
signals:

public slots:
private:
    bool                      _validShaders;
};

#endif // UVVIEW_H
