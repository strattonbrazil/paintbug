#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "mesh.h"

#include <GL/gl.h>

// global access to shared texture resources
class TextureCache {
public:
    static bool hasMeshTexture(Mesh* mesh);
    static GLuint meshTextureId(Mesh* mesh);
    static void setMeshTexture(Mesh* mesh, GLuint id);
};

#endif // TEXTURECACHE_H
