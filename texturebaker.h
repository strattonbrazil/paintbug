#ifndef TEXTUREBAKER_H
#define TEXTUREBAKER_H

#include <QOpenGLWidget>

#include "mesh.h"

// helper widget with OpenGL context for fetching and writing
// textures to disk
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
