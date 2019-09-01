#include "texturecache.h"

#include <QOpenGLContext>
#include <iostream>

static QHash<Mesh*,QPair<GLuint,GLuint>> meshTextures;

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
    return meshTextures[mesh].first;
}

GLuint TextureCache::meshTextureSize(Mesh *mesh)
{
    checkContext();
    return meshTextures[mesh].second;
}

void TextureCache::setMeshTexture(Mesh *mesh, GLuint id, GLuint size)
{
    checkContext();
    meshTextures[mesh] = QPair<GLuint,GLuint>(id, size);
}
