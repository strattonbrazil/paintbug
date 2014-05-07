#include "glview.h"

#include <iostream>
#include <QMatrix4x4>
#include <QOpenGLFramebufferObjectFormat>

#include "camera.h"
#include "scene.h"

#define PAINT_FBO_WIDTH 2048

#define DEBUG_PAINT_LAYER 0

namespace MouseMode {
    enum { FREE, CAMERA, TOOL };
}

int mouseMode = MouseMode::FREE;
int activeMouseButton = -1;

GLView::GLView(QWidget *parent) :
    QGLWidget(parent)
{
    _camera = new Camera();
    _validShaders = false;
    _validFbos = false;
}

void GLView::resizeGL(int w, int h)
{

    glViewport(0, 0, w, h);
}

void GLView::paintGL()
{
    if (!_validShaders) {
        _meshShader = ShaderFactory::buildMeshShader(this);
#if DEBUG_PAINT_LAYER
        _paintDebugShader = ShaderFactory::buildPaintDebugShader(this);
#endif
        _validShaders = true;
    }

    if (!_validFbos) {
        // TODO: change this to a smaller format since we're only using alpha
        QOpenGLFramebufferObjectFormat format;
        format.setInternalTextureFormat(GL_RED);
        _paintFbo = new QOpenGLFramebufferObject(PAINT_FBO_WIDTH, PAINT_FBO_WIDTH, format);
        _paintFbo->bind();
        glClearColor(0,0,0,0); // only red is used
        glClear(GL_COLOR_BUFFER_BIT);
        _paintFbo->release();
        _validFbos = true;
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(.2,.2,.2,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 cameraProjM = Camera::getProjMatrix(_camera, width(), height());
    QMatrix4x4 cameraViewM = Camera::getViewMatrix(_camera, width(), height());
    QMatrix4x4 cameraProjViewM = cameraProjM * cameraViewM;
    QMatrix4x4 objToWorld;

    glBindTexture(GL_TEXTURE_2D, _paintFbo->texture());

    _meshShader->bind();
    _meshShader->setUniformValue("objToWorld", objToWorld);
    _meshShader->setUniformValue("cameraPV", cameraProjViewM);
    _meshShader->setUniformValue("paintTexture", 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(cameraProjM.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(cameraViewM.data());

    Scene* scene = Scene::activeScene();

    // render each mesh
    QHashIterator<QString,Mesh*> meshes = scene->meshes();
    while (meshes.hasNext()) {
        meshes.next();
        Mesh* mesh = meshes.value();

        glBegin(GL_TRIANGLES);
        {
            const int NUM_TRIANGLES = mesh->numTriangles();
            for (int i = 0; i < NUM_TRIANGLES; i++) {
                for (int j = 0; j < 3; j++) {
                    const unsigned int vertIndex = mesh->_triangleIndices[i*3+j];
                    Point3 vert = mesh->_vertices[vertIndex];
                    Point2 uv = mesh->_uvs[vertIndex];
                    glTexCoord2f(uv.x(), uv.y());
                    glVertex3f(vert.x(), vert.y(), vert.z());
                }
            }
        }
        glEnd();
    }

    _meshShader->release();

    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        glVertex3f(-10, 0, i);
        glVertex3f(10, 0, i);
    }
    glEnd();

    glDisable(GL_DEPTH_TEST);

    // draw strokes onto paint FBO
    _paintFbo->bind();
    glViewport(0,0,PAINT_FBO_WIDTH,PAINT_FBO_WIDTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, PAINT_FBO_WIDTH, 0, PAINT_FBO_WIDTH, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPointSize(20.0f);
    glBegin(GL_POINTS);
    foreach (Point2 p, _strokePoints) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();
    glViewport(0,0,width(),height());
    _paintFbo->release();

#if DEBUG_PAINT_LAYER
    _drawPaintLayer();
#endif
}

void GLView::_drawPaintLayer()
{

    QMatrix4x4 cameraProjViewM;
    cameraProjViewM.ortho(0, width(), 0, height(), -1, 1);

    glBindTexture(GL_TEXTURE_2D, _paintFbo->texture());

    _paintDebugShader->bind();
    _paintDebugShader->setUniformValue("cameraPV", cameraProjViewM);
    _paintDebugShader->setUniformValue("paintTexture", 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, _paintFbo->texture());

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0,0);
        glVertex2f(0,0);
        glTexCoord2f(1,0);
        glVertex2f(PAINT_FBO_WIDTH,0);
        glTexCoord2f(1,1);
        glVertex2f(PAINT_FBO_WIDTH,PAINT_FBO_WIDTH);
        glTexCoord2f(0,1);
        glVertex2f(0,PAINT_FBO_WIDTH);
    }
    glEnd();

    glDisable(GL_BLEND);
    _paintDebugShader->release();
}


QGLFormat GLView::defaultFormat()
{
    QGLFormat format;
       //format.setVersion(3,2);
    format.setAlpha(true);
    format.setStencil(true);
    format.setVersion(3,1);
    format.setProfile(QGLFormat::CoreProfile);
    return format;
}

void GLView::mousePressEvent(QMouseEvent* event)
{
    //bool altDown = event->modifiers() & Qt::AltModifier;
    bool camDown = event->modifiers() & Qt::MetaModifier;

    if (mouseMode == MouseMode::FREE && camDown) {
        mouseMode = MouseMode::CAMERA;
        activeMouseButton = event->button();
        Camera::mousePressed(_camera, _cameraScratch, event);
    }
    else if (mouseMode == MouseMode::FREE && event->button() & Qt::LeftButton) {
        _strokePoints.append(Point2(event->pos().x(), height()-event->pos().y()));
        mouseMode = MouseMode::TOOL;
        /*
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
        */
    }
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
    if (mouseMode == MouseMode::TOOL) {
        mouseDragEvent(event);

        //_workTool->mouseMoved(event);


        update();
    }
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
    else if (mouseMode == MouseMode::TOOL) {
        _strokePoints.append(Point2(event->pos().x(), height()-event->pos().y()));
        /*
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
        */
    }

    update();

}

