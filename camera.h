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
    Q_OBJECT
public:
    int                          assetType() { return AssetType::CAMERA_ASSET; }
    Camera();
    static QMatrix4x4            getViewMatrix(Transformable* camera, int width, int height);
    //static QMatrix4x4            getProjMatrix(Camera* camera, int width, int height, float dx=0, float dy=0);
    static QMatrix4x4            getProjMatrix(Transformable* camera, int width, int height, float pixdx=0, float pixdy=0);
    static QMatrix4x4            getOrthoMatrix(float left, float right, float bottom, float top, float near=-1, float far=1);

    //void                         lookTransform(RtMatrix &t);
    //void                         flipYZ(RtMatrix m);
    RotatePair                   aim(Vector3 dir);

    static void                  mousePressed(Transformable* camera, CameraScratch &scratch, QMouseEvent* event);
    static void                  mouseReleased(Transformable* camera, CameraScratch &scratch, QMouseEvent* event);
    static void                  mouseDragged(Transformable* camera, CameraScratch &scratch, QMouseEvent* event);


    Point3                       tmpEye;
};

#endif // CAMERA_H
