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

    QString meshName() { return _meshName; }
    void setMeshName(QString name) { _meshName = name; }

    QString texturePath() { return _texturePath; }
    void setTexturePath(QString path) { _texturePath = path; }

    // TODO: make these private
    QVector<Point3> _vertices;
    QVector<Point2> _uvs;
    QVector<int> _triangleIndices;
private:
    QString _texturePath;
    QString _meshName;
};

#endif // MESH_H
