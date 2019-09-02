#include "texturecache.h"

#include <QOpenGLContext>
#include <iostream>

// maps Mesh instances to OpenGL texture handles
static QHash<Mesh*,QOpenGLTexture*> meshTextures;

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

QOpenGLTexture *TextureCache::meshTextureId(Mesh *mesh)
{
    checkContext();
    return meshTextures[mesh];
}

void TextureCache::setMeshTexture(Mesh *mesh, QOpenGLTexture *texture)
{
    checkContext();
    if (meshTextures.contains(mesh)) { // clean up previous texture
        delete meshTextures[mesh];
    }
    meshTextures[mesh] = texture;
}
