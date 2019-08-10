#include <QMatrix4x4>
#include <QOpenGLFramebufferObjectFormat>

#include <GL/glext.h>

#include "perspectiveview.h"
#include "scene.h"
#include "gl_util.h"

PerspectiveView::PerspectiveView(QWidget *parent) :
    GLView(parent)
{
    _camera = new PerspectiveCamera();
    _brushColor = QColor(255,0,0);
}

QString PerspectiveView::getViewLabel()
{
    return "Perspective";
}

void PerspectiveView::glPass()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(.2,.2,.2,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 cameraProjM = _camera->getProjMatrix(width(), height());
    QMatrix4x4 cameraViewM = _camera->getViewMatrix(width(), height());
    QMatrix4x4 cameraProjViewM = cameraProjM * cameraViewM;
    QMatrix4x4 objToWorld;

    Scene* scene = Scene::activeScene();

    // render each mesh
    QHashIterator<QString,Mesh*> meshes = scene->meshes();
    while (meshes.hasNext()) {
        meshes.next();
        Mesh* mesh = meshes.value();

        // make sure a texture exists for this mesh
        if (!hasMeshTexture(mesh)) {
            std::cout << "creating mesh texture" << std::endl;

            transferFbo()->bind();

            GLuint textureId;
            glGenTextures(1, &textureId);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glClearColor(.5,.5,.5,1);
            glClear(GL_COLOR_BUFFER_BIT);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0,1,0,1,-1,1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glViewport(0, 0, 256, 256);

            glColor3f(.8,.8,.8);
            glBegin(GL_QUADS);
            {
                glVertex2f(.25,.25);
                glVertex2f(.75,.25);
                glVertex2f(.75,.75);
                glVertex2f(.25,.75);
            }
            glEnd();

            glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 256, 256, 0);

            transferFbo()->release();

            glViewport(0, 0, width(), height());

            setMeshTexture(mesh, textureId);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, meshTexture(mesh));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, paintFbo()->texture());
        glActiveTexture(GL_TEXTURE0);

        _meshShader->bind();
        _meshShader->setUniformValue("objToWorld", objToWorld);
        _meshShader->setUniformValue("cameraPV", cameraProjViewM);
        _meshShader->setUniformValue("paintFboWidth", PAINT_FBO_WIDTH);
        _meshShader->setUniformValue("brushColor", _brushColor.redF(), _brushColor.greenF(), _brushColor.blueF(), 1);
        _meshShader->setUniformValue("meshTexture", 0);
        _meshShader->setUniformValue("paintTexture", 1);

        renderMesh(mesh);

        _meshShader->release();


    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(cameraProjM.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(cameraViewM.data());

    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++) {
        glVertex3f(-10, 0, i);
        glVertex3f(10, 0, i);
    }
    glEnd();

    glDisable(GL_DEPTH_TEST);
}

void PerspectiveView::painterPass(QPainter* painter)
{
    // render HUD
    //
    drawOutlinedText(painter, 20, height()-20, "Brush Color", QColor(0,0,0), QColor(255,255,255));

    QFontMetrics fm(painter->font());
    const int textHeight = fm.height();
    _brushColorRect = QRect(20, height()-120-textHeight, 100, 100);
    painter->fillRect(_brushColorRect, _brushColor);
}



