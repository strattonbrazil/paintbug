#include "camera.h"

//#include <iostream>
//using namespace std;



// used by panning


PerspectiveCamera::PerspectiveCamera() : Camera()
{
    resetLook();
    _center = Point3(4.0,2.0,4.0);
    _yRot = 225;
    _upRot = 45;
    _distance = _center.length();
    updateLook();
    _center = -lookDir();
    //updateLook();
    //moveType = MoveType::NOT_MOVING;
}

/*
Point3 PerspectiveCamera::eye() { return _center; }
Vector3 PerspectiveCamera::upDir() {
    return _rotate.rotatedVector(Vector3(0,1,0));
}
Point3 PerspectiveCamera::lookat() { return _center + lookDir(); }
Vector3 PerspectiveCamera::lookDir() { return _rotate.rotatedVector(Vector3(0,0,_distance)); }
Vector3 PerspectiveCamera::leftDir() { return Vector3::crossProduct(upDir(), lookDir()).normalized(); }
*/

QMatrix4x4 PerspectiveCamera::getViewMatrix(int width, int height)
{
    QMatrix4x4 m;
    m.lookAt(eye(), lookat(), upDir());
    return m;
}

/*
QMatrix4x4 PerspectiveCamera::getProjMatrix(PerspectiveCamera* PerspectiveCamera, int width, int height, float dx, float dy)
{
    // taken from gluPerspective docs
    float aspect = (float)width / (float)height;
    float zNear = 0.1f;
    float zFar = 1000.0f;

    float top = tan(PerspectiveCamera->fov()*3.14159/360.0) * zNear;
    //float top = tan(fov*0.5) * zNear;
    float bottom = -top;

    float left = aspect * bottom;
    float right = aspect * top;

    QMatrix4x4 m;
    m.frustum(left+dx, right+dx, bottom+dy, top+dy, zNear, zFar);
    //m.perspective(PerspectiveCamera->fov(), aspect, zNear, zFar);
    return m;
}
*/

QMatrix4x4 PerspectiveCamera::getProjMatrix(int width, int height)
{
    // can probably remove this now
    float pixdx = 0;
    float pixdy = 0;

    // taken from gluPerspective docs
    float aspect = (float)width / (float)height;
    float zNear = .1f;
    float zFar = 100.0f;

    float top = tan(fov()*3.14159/360.0) * zNear;
    //float top = tan(fov*0.5) * zNear;
    float bottom = -top;

    float left = aspect * bottom;
    float right = aspect * top;

    //int viewport[4];
    //glGetIntegerv(GL_VIEWPORT, viewport);
    float xwsize = right - left;
    float ywsize = top - bottom;

    // MAINT: width/height should be pulled from viewport if it doesn't match
    // size of render
    float dx = -(pixdx * xwsize / (float)width);
    float dy = -(pixdy * ywsize / (float)height);

    QMatrix4x4 m;
    m.frustum(left+dx, right+dx, bottom+dy, top+dy, zNear, zFar);
    //m.perspective(PerspectiveCamera->fov(), aspect, zNear, zFar);
    return m;
}


/*
QMatrix4x4 PerspectiveCamera::getProjMatrix(PerspectiveCamera* PerspectiveCamera, int width, int height, float offsetX, float offsetY)
{
    // taken from gluPerspective docs
    float aspect = (float)width / (float)height;
    float zNear = 0.1f;
    float zFar = 1000.0f;


    double fov2, left, right, bottom, top;

        fov2 = ((fovy * Math.PI) / 180.0) / 2.0;

        top = near / (Math.cos(fov2) / Math.sin(fov2));
        bottom = -top;

        right = top * aspect;
        left = -right;

        accFrustum(gl, left, right, bottom, top, near, far, pixdx, pixdy, eyedx,
            eyedy, focus);

    QMatrix4x4 m;
    m.perspective(PerspectiveCamera->fov(), aspect, zNear, zFar);
    return m;
}
*/

#define PI 3.14159265359
RotatePair PerspectiveCamera::aim(Vector3 dir)
{ // based on VTK aim
    double xzlen, yzlen, yrot, xrot;

    /*
     * The initial rotation about the y axis is given by the projection of
     * the direction vector onto the x,z plane: the x and z components
     * of the direction.
     */
    xzlen = sqrt(dir.x()*dir.x()+dir.z()*dir.z());
    if (xzlen == 0)
        yrot = (dir.y() < 0) ? 180 : 0;
    else
        yrot = 180*acos(dir.z()/xzlen)/PI;
    /*
     * The second rotation, about the x axis, is given by the projection on
     * the y,z plane of the y-rotated direction vector: the original y
     * component, and the rotated x,z vector from above.
    */
    yzlen = sqrt(dir.y()*dir.y()+xzlen*xzlen);
    xrot = 180*acos(xzlen/yzlen)/PI;       /* yzlen should never be 0 */

    RotatePair pair;

    if (dir.y() > 0) {
        pair.rot1 = QVector4D(xrot, 1.0, 0.0, 0.0);
        //fprintf (filePtr, "Rotate %f %f %f %f\n", xrot, 1.0, 0.0, 0.0);
    } else {
        pair.rot1 = QVector4D(-xrot, 1.0, 0.0, 0.0);
        //fprintf (filePtr, "Rotate %f %f %f %f\n", -xrot, 1.0, 0.0, 0.0);
    }
    /* The last rotation declared gets performed first */
    if (dir.x() > 0) {
        pair.rot2 = QVector4D(-yrot, 0.0, 1.0, 0.0);
        //fprintf (filePtr, "Rotate %f %f %f %f\n", -yrot, 0.0, 1.0, 0.0);
    } else {
        pair.rot2 = QVector4D(yrot, 0.0, 1.0, 0.0);
        //fprintf (filePtr, "Rotate %f %f %f %f\n", yrot, 0.0, 1.0, 0.0);
    }

    return pair;
}

/*
void PerspectiveCamera::flipYZ(RtMatrix m)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                if (i == 0)
                    m[i][j] = 1;
                else
                    m[i][j] = -1;
            } else
                m[i][j] = 0;
        }
    }
}
*/

void PerspectiveCamera::mousePressed(CameraScratch &scratch, QMouseEvent *event)
{
    scratch.pickX = event->pos().x();
    scratch.pickY = event->pos().y();

    if (event->button() == Qt::LeftButton) {
        scratch.moveType = MoveType::ROTATING;
    }
    else if (event->button() == Qt::MidButton) {
        scratch.moveType = MoveType::PANNING;

        scratch.origEye = eye();
        //Vector3 f = (lookat() - eye()).normalized();
        //Vector3 s = Vector3::crossProduct(f, upDir());
        scratch.origUp = upDir();
        //origUp = Vector3::crossProduct(s, f);
        scratch.origLeft = leftDir();
    } else if (event->button() == Qt::RightButton) {
        scratch.moveType = MoveType::TRUCKING;
    } else {
        scratch.moveType = MoveType::NOT_MOVING;
    }
}

void PerspectiveCamera::mouseReleased(CameraScratch &scratch, QMouseEvent *event)
{
    scratch.moveType = MoveType::NOT_MOVING;
}

void PerspectiveCamera::mouseDragged(CameraScratch &scratch, QMouseEvent *event)
{
    int xDiff = scratch.pickX - event->pos().x();
    int yDiff = scratch.pickY - event->pos().y();

    if (scratch.moveType == MoveType::ROTATING) {
        Vector3 origLook = eye() + lookDir();
        setYRot(yRot() + xDiff * 0.5f);
        setUpRot(upRot() + yDiff * -0.5f);

        // move eye to look at original focal point (Maya style)
        Vector3 lookAway = lookDir() * -1;
        setCenter(origLook + lookAway);
    }
    else if (scratch.moveType == MoveType::PANNING) {
        float panScale = 0.05f;

        Vector3 mUp = scratch.origUp * -1.0f * yDiff * panScale;
        Vector3 mLeft = scratch.origLeft * -1.0f * xDiff * panScale;

        setCenter(eye() + mUp + mLeft);
    } else if (scratch.moveType == MoveType::TRUCKING) {
        Point3 at = lookat();
        Vector3 l = lookDir() * -0.01f * yDiff;

        setCenter(l + center());
    }

    scratch.pickX = event->pos().x();
    scratch.pickY = event->pos().y();
}

OrthographicCamera::OrthographicCamera() : Camera()
{
}

// handles zoom and translation
QMatrix4x4 OrthographicCamera::getViewMatrix(int width, int height)
{
    QMatrix4x4 m;
    m.setToIdentity();
    return m;
}

QMatrix4x4 OrthographicCamera::getProjMatrix(int width, int height)
{
    float aspect = (float)width / (float)height;

    float left = 0.5f + -0.6f*aspect;
    float right = 0.5f + 0.6f*aspect;
    float bottom = -0.1f;
    float top = 1.1f;

    QMatrix4x4 m;
    m.setToIdentity();
    m.ortho(left, right, bottom, top, -1.f, 1.f);
    return m;
}

void OrthographicCamera::mousePressed(CameraScratch &scratch, QMouseEvent *event)
{

}

void OrthographicCamera::mouseReleased(CameraScratch &scratch, QMouseEvent *event)
{

}

void OrthographicCamera::mouseDragged(CameraScratch &scratch, QMouseEvent *event)
{

}
