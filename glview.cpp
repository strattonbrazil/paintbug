#include "glview.h"

#include <iostream>
#include "camera.h"
#include "QMatrix4x4"

#include <GL/freeglut.h>

namespace MouseMode {
    enum { FREE, CAMERA, TOOL };
}

int mouseMode = MouseMode::FREE;
int activeMouseButton = -1;

GLView::GLView(QWidget *parent) :
    QGLWidget(parent)
{
    _camera = new Camera();

}

void GLView::resizeGL(int w, int h)
{
    std::cout << "resizing: " << w << " " << h << std::endl;
    glViewport(0, 0, w, h);
}

void GLView::paintGL()
{
    glClearColor(.2,.2,.2,0);
    glClear(GL_COLOR_BUFFER_BIT);

    QMatrix4x4 cameraProjM = Camera::getProjMatrix(_camera, width(), height());
    QMatrix4x4 cameraViewM = Camera::getViewMatrix(_camera, width(), height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(cameraProjM.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(cameraViewM.data());

    glutSolidSphere(1, 8, 8);


}

void GLView::mousePressEvent(QMouseEvent* event)
{
    bool altDown = event->modifiers() & Qt::AltModifier;

    if (mouseMode == MouseMode::FREE && altDown) {
        mouseMode = MouseMode::CAMERA;
        activeMouseButton = event->button();
        Camera::mousePressed(_camera, _cameraScratch, event);
    }
    /*
    else if (mouseMode == MouseMode::FREE && event->button() & Qt::LeftButton) {
        if (_workTool != 0) {
            mouseMode = MouseMode::TOOL;
            activeMouseButton = event->button();
            _workTool->mousePressed(this, event);
        }
        else {
            mouseMode = MouseMode::TOOL;
            activeMouseButton = event->button();
            CursorTool* cursorTool = SunshineUi::cursorTool();
            cursorTool->mousePressed(this, event);
        }
    }
    */
}

void GLView::mouseDoubleClickEvent(QMouseEvent *event)
{
#if 0
    if (mouseMode == MouseMode::FREE && event->button() & Qt::LeftButton) {

        CursorTool* cursorTool = SunshineUi::cursorTool();
        cursorTool->mouseDoubleClicked(this, event);
        /*
        mouseMode = MouseMode::TOOL;
        activeMouseButton = event->button();
        //basicSelect = BasicSelect.instance
        //basic_select.mousePressed(self,event)
        //_basicSelect->mousePressed(this, event);
        CursorTool* cursorTool = SunshineUi::cursorTool();
        cursorTool->mousePressed(this, event);
        */
    }
#endif
}

void GLView::mouseReleaseEvent(QMouseEvent* event)
{
    //CursorTool* cursorTool = SunshineUi::cursorTool();

    if (mouseMode == MouseMode::CAMERA && event->button() == activeMouseButton) {
        mouseMode = MouseMode::FREE;
        activeMouseButton = -1;
        Camera::mouseReleased(_camera, _cameraScratch, event);
    }
    /*
    else if (mouseMode == MouseMode::TOOL && _workTool == 0 && event->button() == activeMouseButton) {
        mouseMode = MouseMode::FREE;
        activeMouseButton = -1;

        cursorTool->mouseReleased(this, event);
    }
    else if (_workTool != 0) {
        if (_workTool->ravageMouse()) {
            if (_workTool != 0 && event->button() & Qt::LeftButton) {
                _workTool->finish(event);
                _workTool = 0;
            }
            else if (_workTool != 0 && event->button() & Qt::RightButton) {
                _workTool->cancel(event);
                _workTool = 0;
            }
        } else {
            _workTool->mouseReleased(this, event);
        }
        mouseMode = MouseMode::FREE;
    }
    else if (mouseMode == MouseMode::FREE && event->button() == Qt::RightButton && cursorTool->canMouseCancel(this)) {
        cursorTool->cancel(this);
    }
    else if (mouseMode == MouseMode::FREE && event->button() == Qt::RightButton) { // popup menu
        showContextMenu(event);
    }
    */

    update();
}

void GLView::mouseMoveEvent(QMouseEvent* event)
{
/*
    if (_workTool != 0 || mouseMode == MouseMode::TOOL) {
        mouseDragEvent(event);

        //_workTool->mouseMoved(event);


        update();
    }
    */
    if (mouseMode == MouseMode::FREE) {
        /*
        // calculate preselection
        Point3 rayOrig = camera()->eye();
        Vector3 rayDir = computeRayDirection(event->pos());
        //FaceUtil::FaceHit faceHit = FaceUtil::closestFace(_scene, rayOrig, rayDir, false);

        QList<Triangle> triangles = _meshGrid.trianglesByPoint(QPoint(event->pos().x(), height()-event->pos().y()));
        FaceUtil::FaceHit faceHit = FaceUtil::closestFace(triangles, rayOrig, rayDir, false);
        VertexUtil::VertexHit vertexHit = VertexUtil::closestVertex(this, event, false);

        _hoverMesh = faceHit.nearMesh ? faceHit.nearMesh : 0;
        if (faceHit.nearFace.is_valid()) _hoverFace = faceHit.nearFace;
        else _hoverFace.invalidate();

        if (vertexHit.vertex.is_valid()) _hoverVert = vertexHit.vertex;
        else _hoverVert.invalidate();

        CursorTool* cursorTool = SunshineUi::cursorTool();
        cursorTool->mouseMoved(this, event);
        */

        update();
    }
    else {
        mouseDragEvent(event);
    }
}

void GLView::mouseDragEvent(QMouseEvent* event)
{
    if (mouseMode == MouseMode::CAMERA) {
        Camera::mouseDragged(_camera, _cameraScratch, event);

    }
    /*
    else if (mouseMode == MouseMode::TOOL || _workTool != 0) {
        if (_workTool != 0) {
            // move mouse back to center
                QPoint mouseDiff = QCursor::pos() - centerMouse(TRUE);
                if (mouseDiff != QPoint(0,0)) {
                    _workTool->mouseMoved(this, event, mouseDiff.x(), mouseDiff.y());
                    if (_workTool->ravageMouse())
                        centerMouse(FALSE);
                }

         //   _workTool->mouseMoved(event, 2, 0);
        }
        else {
            CursorTool* cursorTool = SunshineUi::cursorTool();
            cursorTool->mouseDragged(this, event);
        }
    }
    */

    update();

}

