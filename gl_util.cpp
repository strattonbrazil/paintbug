#include "gl_util.h"

// just to get OpenGL calls
#include <QGLWidget>

void renderMesh(Mesh* mesh, MeshPropType texCoordType, MeshPropType vertCoordType)
{
    glBegin(GL_TRIANGLES);
    {
        const int NUM_TRIANGLES = mesh->numTriangles();
        for (int i = 0; i < NUM_TRIANGLES; i++) {
            for (int j = 0; j < 3; j++) {
                const unsigned int vertIndex = mesh->_triangleIndices[i*3+j];
                Point3 vert = mesh->_vertices[vertIndex];
                Point3 uv = mesh->_uvs[vertIndex];

                if (texCoordType == MeshPropType::UV)
                    glTexCoord2f(uv.x(), uv.y());
                else
                    glTexCoord3f(vert.x(), vert.y(), vert.z());

                if (vertCoordType == MeshPropType::UV)
                    glVertex2f(uv.x(), uv.y());
                else
                    glVertex3f(vert.x(), vert.y(), vert.z());
            }
        }
    }
    glEnd();
}

void drawTexturedRect(int x, int y, int width, int height)
{
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0,0);
        glVertex2f(x,y);
        glTexCoord2f(1,0);
        glVertex2f(x+width,y);
        glTexCoord2f(1,1);
        glVertex2f(x+width,y+height);
        glTexCoord2f(0,1);
        glVertex2f(x,y+height);
    }
    glEnd();
}
