#ifndef GL_UTIL_H
#define GL_UTIL_H

#include "mesh.h"
#include "constants.h"

void renderMesh(Mesh* mesh,
                MeshPropType texCoordType=MeshPropType::UV,
                MeshPropType vertCoordType=MeshPropType::GEOMETRY);

void drawTexturedRect(int x, int y, int width, int height);

#endif // GL_UTIL_H
