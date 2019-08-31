#ifndef MESH_H
#define MESH_H

#include <QVector>

#include "util.h"

class Mesh
{
public:
    Mesh();
    void addVertex(float x, float y, float z);
    void addUV(float u, float v);
    void addTriangle(int i, int j, int k);
    int numTriangles();

    QString _texturePath;
    QVector<Point3> _vertices;
    QVector<Point2> _uvs;
    QVector<int> _triangleIndices;
};

#endif // MESH_H
