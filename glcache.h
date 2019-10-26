#ifndef GLCACHE_H
#define GLCACHE_H

#include "mesh.h"

#include <GL/gl.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

// global access to shared texture resources
class GLCache {
public:
    static bool hasMeshTexture(Mesh *mesh);
    static GLuint meshTextureId(Mesh *mesh);
    static void setMeshTexture(Mesh *mesh, GLuint id);
    static GLuint removeMeshTexture(Mesh *mesh);

    static QOpenGLBuffer *meshVertexBuffer(Mesh *mesh);
    static QOpenGLBuffer *meshUVBuffer(Mesh *mesh);
    static QOpenGLBuffer *meshIndexBuffer(Mesh *mesh);
    static QOpenGLVertexArrayObject *meshVertexArray(Mesh *mesh);
};

#endif // GLCACHE_H
