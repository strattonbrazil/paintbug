#include "mesh.h"

#include <QJsonObject>
#include <QJsonArray>

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

QJsonObject Mesh::serialize()
{
    QJsonObject self;
    self.insert("name", _meshName);
    self.insert("texture_size", _textureSize);
    self.insert("texture_path", _texturePath);

    if (_geometryPath.isEmpty()) { // store raw geometry
        self.insert("vertices", serializeVertices());
        self.insert("uvs", serializeUvs());
        self.insert("triangle_indices", serializeTriangleIndices());
    } else {
        self.insert("geometry_path", _geometryPath);
    }

    return self;
}

QJsonArray vec2ToJson(Point2 p) {
    QJsonArray a;
    a.append(p.x());
    a.append(p.y());
    return a;
}

QJsonArray vec3ToJson(Point3 p) {
    QJsonArray a;
    a.append(p.x());
    a.append(p.y());
    a.append(p.z());
    return a;
}

QJsonArray Mesh::serializeVertices()
{
    QJsonArray vertices;
    foreach (Point3 p, _vertices) {
        vertices.append(vec3ToJson(p));
    }
    return vertices;
}

QJsonArray Mesh::serializeUvs()
{
    QJsonArray uvs;
    foreach (Point2 uv, _uvs) {
        uvs.append(vec2ToJson(uv));
    }
    return uvs;
}

QJsonArray Mesh::serializeTriangleIndices()
{
    QJsonArray indices;
    foreach (int index, _triangleIndices) {
        indices.append(index);
    }
    return indices;
}
