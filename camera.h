#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include "transformable.h"
#include <QMouseEvent>

typedef struct myRotatePair {
    QVector4D rot1;
    QVector4D rot2;
} RotatePair;

namespace MoveType {
    enum { ROTATING, PANNING, TRUCKING, NOT_MOVING };
}

class CameraScratch
{
public:
    CameraScratch() { moveType = MoveType::NOT_MOVING; }
    int pickX;
    int pickY;
    int moveType;
    Point3 origEye;
    Vector3 origUp;
    Vector3 origLeft;
};

class Camera : public Transformable
{
public:
    virtual QMatrix4x4 getViewMatrix(int width, int height) = 0;
    virtual QMatrix4x4 getProjMatrix(int width, int height) = 0;

    virtual void mousePressed(CameraScratch &scratch, QMouseEvent* event) = 0;
    virtual void mouseReleased(CameraScratch &scratch, QMouseEvent* event) = 0;
    virtual void mouseDragged(CameraScratch &scratch, QMouseEvent* event) = 0;
    //static QMatrix4x4            getOrthoMatrix(float left, float right, float bottom, float top, float near=-1, float far=1);
};

class PerspectiveCamera : public Camera
{
    Q_OBJECT
public:
    PerspectiveCamera();

    QMatrix4x4 getViewMatrix(int width, int height);
    QMatrix4x4 getProjMatrix(int width, int height);

    //void                         lookTransform(RtMatrix &t);
    //void                         flipYZ(RtMatrix m);
    RotatePair                   aim(Vector3 dir);

    void                  mousePressed(CameraScratch &scratch, QMouseEvent* event);
    void                  mouseReleased(CameraScratch &scratch, QMouseEvent* event);
    void                  mouseDragged(CameraScratch &scratch, QMouseEvent* event);

    Point3                       tmpEye;
};

class OrthographicCamera : public Camera
{
    Q_OBJECT
public:
    OrthographicCamera();

    QMatrix4x4 getViewMatrix(int width, int height);
    QMatrix4x4 getProjMatrix(int width, int height);

    void                  mousePressed(CameraScratch &scratch, QMouseEvent* event);
    void                  mouseReleased(CameraScratch &scratch, QMouseEvent* event);
    void                  mouseDragged(CameraScratch &scratch, QMouseEvent* event);
};

#endif // CAMERA_H
