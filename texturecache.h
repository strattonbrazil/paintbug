#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "mesh.h"

#include <QOpenGLTexture>

// global access to shared texture resources
class TextureCache {
public:
    static bool hasMeshTexture(Mesh* mesh);
    static QOpenGLTexture *meshTextureId(Mesh* mesh);
    static void setMeshTexture(Mesh* mesh, QOpenGLTexture *texture);
};

#endif // TEXTURECACHE_H
