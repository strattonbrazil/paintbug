#include "transformable.h"

#include <QStringList>

Transformable::Transformable()
{
    _startRotate = QQuaternion();
    _rotateOrder = RotateOrder::YXZ;
    _yRot = 0.0f;
    _upRot = 0.0f;
    _fov = 60.0f;
    _distance = 1.0f;

    _scale = Vector3(1,1,1);

    /*
    QString position("{ 'var' : 'position', 'name' : 'Position', 'type' : 'point3', 'getter' : 'center', 'setter' : 'setCenter' }");
    QString scale("{ 'var' : 'scale', 'name' : 'Scale', 'type' : 'point3', 'getter' : 'scale', 'setter' : 'setScale' }");
    QStringList atts;
    atts << position << scale;

    addAttributes(atts);
    */
}

void Transformable::resetLook()
{
    _startRotate = _rotate;
    _yRot = 0.0f;
    _upRot = 0.0f;
}

void Transformable::updateLook()
{
    // setups the rotation based on the two-camera controls
    //Quat4 ySpin = Quat4(_yRot, _startRotate.rotatedVector(Vector3(0,1,0)));
    Quat4 ySpin = Quat4::fromAxisAndAngle(_startRotate.rotatedVector(Vector3(0,1,0)), _yRot);
    Vector3 tmpVec = ySpin.rotatedVector(_startRotate.rotatedVector(Vector3(1,0,0)));
    //Quat4 upSpin = Quat4(_upRot, tmpVec);
    Quat4 upSpin = Quat4::fromAxisAndAngle(tmpVec, _upRot);
    const Quat4 mv = upSpin * ySpin;
    _rotate = mv * _startRotate;
    _rotate.normalize();
}

void Transformable::orient(Point3 eye, Point3 reference, Vector3 up)
{
    // similar to gluLookAt
    _center = eye;

    Vector3 lookDir = (reference - eye).normalized();
    Vector3 leftDir = Vector3::crossProduct(up, lookDir).normalized();
    Vector3 modifiedUp = Vector3::crossProduct(lookDir, leftDir).normalized();

    _rotate = Quat4::fromMatrix(leftDir.x(), modifiedUp.x(), lookDir.x(),
                                leftDir.y(), modifiedUp.y(), lookDir.y(),
                                leftDir.z(), modifiedUp.z(), lookDir.z());

}

QMatrix4x4 Transformable::objectToWorld()
{
    QMatrix4x4 t(1.0f, 0.0f, 0.0f, _center.x(),
                 0.0f, 1.0f, 0.0f, _center.y(),
                 0.0f, 0.0f, 1.0f, _center.z(),
                 0.0f, 0.0f, 0.0f, 1.0f);
    QMatrix4x4 r = _rotate.matrix();
    QMatrix4x4 s(_scale.x(), 0.0f, 0.0f, 0.0f,
                 0.0f, _scale.y(), 0.0f, 0.0f,
                 0.0f, 0.0f, _scale.z(), 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
    return t*r*s;
}
