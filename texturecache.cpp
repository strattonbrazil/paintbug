#include "texturecache.h"

#include <QOpenGLContext>
#include <iostream>

// maps Mesh instances to OpenGL texture handles
static QHash<Mesh*,GLuint> meshTextures;

// prints a warning if the previous GL context and the current context
// aren't shared
static QOpenGLContext *lastContext = 0;
void checkContext()
{
    QOpenGLContext *current = QOpenGLContext::currentContext();
    if (lastContext != 0 && !QOpenGLContext::areSharing(lastContext, current)) {
        std::cerr << "ERROR: switching between OpenGL contexts, which are not shared" << std::endl;
    }
    lastContext = current;
}


bool TextureCache::hasMeshTexture(Mesh *mesh)
{
    checkContext();
    return meshTextures.contains(mesh);
}

GLuint TextureCache::meshTextureId(Mesh *mesh)
{
    checkContext();
    return meshTextures[mesh];
}

void TextureCache::setMeshTexture(Mesh *mesh, GLuint id)
{
    checkContext();
    meshTextures[mesh] = id;
}
