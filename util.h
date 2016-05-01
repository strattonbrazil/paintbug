    #ifndef UTIL_H
#define UTIL_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QPoint>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QTextStream>
#include <QFile>
#include <QSharedPointer>
#include <math.h>
#include <iostream>
#include <QVariant>

typedef QVector2D Vector2;
typedef QVector3D Vector3;
typedef QVector4D Vector4;

// bad idea?
typedef QVector2D Point2;
typedef QVector3D Point3;

//qRegisterMetaType<Point3>();
//#define Point3 QVector3D


//qRegisterMetaType<Point3>("Point3");

std::ostream& operator<< (std::ostream& o, Vector3 const& v);
std::ostream& operator<< (std::ostream& o, QVector4D const& v);
std::ostream& operator<< (std::ostream& o, QPoint const& p);
std::ostream& operator<< (std::ostream& o, QString const& s);

#define PI 3.14159

class Quat4 : public QQuaternion
{
public:
                        Quat4();
                        Quat4(float w, float x, float y, float z);
                        Quat4(float s, Vector3 v);
                        Quat4(QQuaternion q);

    static Quat4        fromMatrix(float m00, float m01, float m02,
                                   float m10, float m11, float m12,
                                   float m20, float m21, float m22);
    QMatrix4x4          matrix();
};

class Box3D
{
public:
    Box3D();
    void includePoint(QVector3D p);
    void includeBox(Box3D b);
    Point3 min() { return _min; }
    Point3 max() { return _max; }
    bool isValid() { return _valid; }
    Point3 midpoint() { return Point3(0.5 * (_min.x() + _max.x()),
                                      0.5 * (_min.y() + _max.y()),
                                      0.5 * (_min.z() + _max.z())); }
    float maxRadius();
private:
    Point3 _min;
    Point3 _max;
    bool _valid;
};

namespace RotateOrder {
    enum { XYZ, YZX, ZXY, XZY, YXZ, ZYX };
}

/*
float fourQuadrantInverseTangent(Vector3 a, Vector3 b)
{
    Vector3 c = Vector3::crossProduct(a, b);
    return std::atan2(c.length(), a*b);
}
*/

//class Entity;
//typedef QSharedPointer<Entity> EntityP;



#if 0
class Entity
{
public:
    void addAttributes(QStringList attributes);

    //int numAttributes() const { return this->size(); }
    //Attribute attributeByIndex(int index) const { return this->at(index); }
    Attribute attributeByName(QString name) const {
        foreach(Attribute attribute, _attributes) {
            if (attribute->property("name").toString() == name)
                return attribute;
        }
        std::cerr << "Cannot find attribute: " << name.toStdString() << std::endl;
        throw KeyErrorException();
    }
    int size() { return _attributes.size(); }
    Attribute at(int i) { return _attributes[i]; }


    // called when an attribute changes in the editor
    void toInstance(QString attribute, QVariant value) {}

    // called when an attribute changes on the instance
    void toEditor(QString attribute, QVariant value) {}

    /*
    QVariant operator[](QString name) {
        foreach(Attribute attribute, (*this)) {
            if (attribute->property("var") == name)
                return attribute->property("value");
        }
        std::cerr << "Cannot find attribute: " << name.toStdString() << std::endl;
        throw KeyErrorException();
    }
    */

protected:
    QList<Attribute> _attributes;
};
typedef QSharedPointer<Entity> EntityP;
#endif

void printMatrix(QMatrix4x4 m);
void printVector3(Vector3 v);
void printQuat(QQuaternion q);
void printBox3D(Box3D box);

const QVector4D SELECTED_COLOR(1,0,1,1);
const QVector4D SELECTED_HOVER_COLOR(1,1,0,1);
const QVector4D UNSELECTED_COLOR(1,1,1,1);
const QVector4D UNSELECTED_HOVER_COLOR(0,1,0,1);

namespace Axis { enum { NoAxis, GlobalX, GlobalY, GlobalZ, LocalX, LocalY, LocalZ, Normal, Screen }; };

namespace WorkMode {
    enum { OBJECT, VERTEX, EDGE, FACE };
}
namespace SelectMode {
    enum { NONE, LINE, BOX };
}
namespace ModelMode {
    enum { NONE, VERTEX, EDGE, FACE };
}

namespace DrawSettings {
    enum {
        DRAW_VERTICES = 1,
        DRAW_EDGES = 2,
        DRAW_FACES = 4,
        USE_OBJECT_COLOR = 8,
        CULL_BORING_VERTICES = 16, // "boring" vertices not-selected or highlighted
        HIGHLIGHT_VERTICES = 32,
        HIGHLIGHT_FACES = 64,
        HIGHLIGHT_OBJECTS = 128,
        STIPPLE_FACES = 256
    };
};

namespace PlaneUtil {
    float intersect(Vector3 planeNormal, Point3 planeOrig, Vector3 rayDir, Point3 rayOrig);
}


#endif // UTIL_H
