#include "glcache.h"

#include <QOpenGLContext>
#include <iostream>

// maps Mesh instances to OpenGL texture handles
static QHash<Mesh*,GLuint> meshTextures;

static QHash<Mesh*,QOpenGLBuffer*> meshVertexBuffers;
static QHash<Mesh*,QOpenGLBuffer*> meshUVBuffers;
static QHash<Mesh*,QOpenGLBuffer*> meshIndexBuffers;
static QHash<Mesh*,QOpenGLVertexArrayObject*> meshVertexArrayObjects;

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


bool GLCache::hasMeshTexture(Mesh *mesh)
{
    checkContext();
    return meshTextures.contains(mesh);
}

GLuint GLCache::meshTextureId(Mesh *mesh)
{
    checkContext();
    return meshTextures[mesh];
}

void GLCache::setMeshTexture(Mesh *mesh, GLuint id)
{
    checkContext();
    meshTextures[mesh] = id;
}

GLuint GLCache::removeMeshTexture(Mesh* mesh)
{
    checkContext();
    return meshTextures[mesh];
}

QOpenGLBuffer *GLCache::meshVertexBuffer(Mesh *mesh)
{
    checkContext();
    if (!meshVertexBuffers.contains(mesh)) {
        QOpenGLBuffer *vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        vbo->create();
        vbo->bind();
        vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo->allocate(mesh->_vertices.data(), mesh->_vertices.count()  * sizeof(QVector3D));
        vbo->release();

        meshVertexBuffers[mesh] = vbo;
    }
    return meshVertexBuffers[mesh];
}

QOpenGLBuffer *GLCache::meshUVBuffer(Mesh *mesh)
{
    checkContext();
    if (!meshUVBuffers.contains(mesh)) {
        QOpenGLBuffer *uvbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        uvbo->create();
        uvbo->bind();
        uvbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        uvbo->allocate(mesh->_uvs.data(), mesh->_uvs.count() * sizeof(QVector3D));
        uvbo->release();

        meshUVBuffers[mesh] = uvbo;
    }
    return meshUVBuffers[mesh];
}

QOpenGLBuffer *GLCache::meshIndexBuffer(Mesh *mesh)
{
    checkContext();
    if (!meshIndexBuffers.contains(mesh)) {
        QOpenGLBuffer *ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        ibo->create();
        ibo->bind();
        ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        ibo->allocate(mesh->_triangleIndices.data(), mesh->_triangleIndices.count() * sizeof(GLuint));
        ibo->release();

        meshIndexBuffers[mesh] = ibo;
    }
    return meshIndexBuffers[mesh];
}

QOpenGLVertexArrayObject *GLCache::meshVertexArray(Mesh *mesh)
{
    checkContext();
    if (!meshVertexArrayObjects.contains(mesh)) {
        QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();

        meshVertexArrayObjects[mesh] = vao;
    }
    return meshVertexArrayObjects[mesh];
}
