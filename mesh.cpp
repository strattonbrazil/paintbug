#include "mesh.h"

Mesh::Mesh()
{
}

void Mesh::addVertex(float x, float y, float z)
{
    _vertices.append(Point3(x, y, z));
}

void Mesh::addUV(float u, float v)
{
    _uvs.append(Point2(u, v));
}

void Mesh::addTriangle(int i, int j, int k)
{
    _triangleIndices.append(i);
    _triangleIndices.append(j);
    _triangleIndices.append(k);
}

int Mesh::numTriangles()
{
    return _triangleIndices.size() / 3;
}
