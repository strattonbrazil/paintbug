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

    int textureSize() { return _textureSize; }
    void setTextureSize(int size) { _textureSize = size; }

    QString texturePath() { return _texturePath; }
    void setTexturePath(QString path) { _texturePath = path; }

    QString geometryPath() { return _geometryPath; }
    void setGeometryPath(QString path) { _geometryPath = path; }

    QJsonObject serialize();

    // TODO: make these private
    QVector<Point3> _vertices;
    QVector<Point2> _uvs;
    QVector<int> _triangleIndices;
private:
    int     _textureSize = 0;
    QString _texturePath = "";
    QString _geometryPath = "";
    QString _meshName = "";

    QJsonArray serializeVertices();
    QJsonArray serializeUvs();
    QJsonArray serializeTriangleIndices();
};

#endif // MESH_H
