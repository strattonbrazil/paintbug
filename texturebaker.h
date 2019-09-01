#ifndef TEXTUREBAKER_H
#define TEXTUREBAKER_H

#include <QOpenGLWidget>

#include "mesh.h"

class TextureBaker : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit TextureBaker(QWidget *parent = nullptr);
    bool writeTextureToFile(Mesh *mesh);
signals:

public slots:
};

#endif // TEXTUREBAKER_H
