#include "util.h"

//#include <iostream>
//using namespace std;

std::ostream& operator<< (std::ostream& o, Vector3 const& v)
{
    return o << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
}

std::ostream& operator<< (std::ostream& o, QVector4D const& v)
{
    return o << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")";
}

std::ostream& operator<< (std::ostream& o, QPoint const& p)
{
    return o << "(" << p.x() << ", " << p.y() << ")";
}

std::ostream& operator<< (std::ostream& o, QString const& s)
{
    return o << s.toStdString();
}

Quat4::Quat4() : QQuaternion()
{

}

Quat4::Quat4(float s, Vector3 v) : QQuaternion(s, v)
{

}

Quat4::Quat4(float w, float x, float y, float z) : QQuaternion(w,x,y,z)
{

}

Quat4::Quat4(QQuaternion q) : QQuaternion(q)
{

}

Quat4 Quat4::fromMatrix(float m00, float m01, float m02,
                        float m10, float m11, float m12,
                        float m20, float m21, float m22) {
    // taken from http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
    float tr = m00 + m11 + m22;

    float w,x,y,z;

    if (tr > 0) {
        float S = (float)sqrt(tr + 1.0) * 2; // S=4*qw
        w = 0.25f * S;
        x = (m21 - m12) / S;
        y = (m02 - m20) / S;
        z = (m10 - m01) / S;
    } else if ((m00 > m11) & (m00 > m22)) {
        float S = (float)sqrt(1.0 + m00 - m11 - m22) * 2; // S=4*qx
        w = (m21 - m12) / S;
        x = 0.25f * S;
        y = (m01 + m10) / S;
        z = (m02 + m20) / S;
    } else if (m11 > m22) {
        float S = (float)sqrt(1.0 + m11 - m00 - m22) * 2; // S=4*qy
        w = (m02 - m20) / S;
        x = (m01 + m10) / S;
        y = 0.25f * S;
        z = (m12 + m21) / S;
    } else {
        float S = (float)sqrt(1.0 + m22 - m00 - m11) * 2; // S=4*qz
        w = (m10 - m01) / S;
        x = (m02 + m20) / S;
        y = (m12 + m21) / S;
        z = 0.25f * S;
    }

    return Quat4(w,x,y,z);
}

QMatrix4x4 Quat4::matrix() {
    // I'm being lazy here.  Eventually transpose it by hand...
    float w = scalar();
    return QMatrix4x4(1.0f - 2.0f * ( y() * y() + z() * z() ),
                      2.0f * (x() * y() + z() * w),
                      2.0f * (x() * z() - y() * w),
                      0.0f,
                      2.0f * ( x() * y() - z() * w ),
                      1.0f - 2.0f * ( x() * x() + z() * z() ),
                      2.0f * (z() * y() + x() * w ),
                      0.0f,
                      2.0f * ( x() * z() + y() * w ),
                      2.0f * ( y() * z() - x() * w ),
                      1.0f - 2.0f * ( x() * x() + y() * y() ),
                      0.0f,
                      0,
                      0,
                      0,
                      1.0f).transposed();
}

Box3D::Box3D()
{
    _valid = false;
}

void Box3D::includePoint(QVector3D p)
{
    const float EPS = 0.00001;
    if (_valid) {
        _min = Point3(std::min(_min.x(), p.x() - EPS),
                      std::min(_min.y(), p.y() - EPS),
                      std::min(_min.z(), p.z() - EPS));
        _max = Point3(std::max(_max.x(), p.x() + EPS),
                      std::max(_max.y(), p.y() + EPS),
                      std::max(_max.z(), p.z() + EPS));
    } else {
        _min = Point3(p.x() - EPS,
                      p.y() - EPS,
                      p.z() - EPS);
        _max = Point3(p.x() + EPS,
                      p.y() + EPS,
                      p.z() + EPS);
        _valid = true;
    }
}

void Box3D::includeBox(Box3D b)
{
    if (_valid) {
        _min = Point3(std::min(_min.x(), b.min().x()),
                      std::min(_min.y(), b.min().y()),
                      std::min(_min.z(), b.min().z()));
        _max = Point3(std::max(_max.x(), b.max().x()),
                      std::max(_max.y(), b.max().y()),
                      std::max(_max.z(), b.max().z()));
    } else {
        _min = b.min();
        _max = b.max();
        _valid = true;
    }

}

float Box3D::maxRadius()
{
    return (this->max() - midpoint()).length();
}

void printMatrix(QMatrix4x4 m)
{
    for (int i = 0; i < 4; i++) {
        Vector4 row = m.row(i);
        printf("%f %f %f %f\n", row.x(), row.y(), row.z(), row.w());
    }
    printf("\n");
    fflush(stdout);
}

void printVector3(Vector3 v)
{
    printf("<%f,%f,%f>\n", v.x(), v.y(), v.z());
    fflush(stdout);
}

void printQuat(QQuaternion q)
{
    printf("<%f,%f,%f,%f>\n", q.x(), q.y(), q.z(), q.scalar());
    fflush(stdout);
}

void printBox3D(Box3D box)
{
    Point3 min = box.min();
    Point3 max = box.max();
    printf("<%f,%f,%f> to <%f,%f,%f>\n",
           min.x(), min.y(), min.z(),
           max.x(), max.y(), max.z());
    fflush(stdout);
}

/*
void loadLuaFile(lua_State* state, QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QString lines = file.readAll();
    //std://:cout << lines.toStdString() << std::endl;
    try {
        std::cout << "executing " << fileName.toStdString() << "...\n-----------" << std::endl;
        std::cout << lines.toStdString().c_str() << std::endl;
        luaL_dostring(state, lines.toStdString().c_str());
    } catch (const std::exception &TheError) {
        std::cout << "*" << std::endl;
        std::cout << TheError.what() << std::endl;
    }

    file.close();
}
*/

namespace PlaneUtil {
    float intersect(Vector3 planeNormal, Point3 planeOrig, Vector3 rayDir, Point3 rayOrig) {
        float t = Vector3::dotProduct(planeNormal, planeOrig - rayOrig) /
                Vector3::dotProduct(planeNormal, rayDir);
        return t;
    }
}
