#include "uvview.h"
#include "scene.h"
#include "gl_util.h"

UVView::UVView(QWidget *parent) :
    GLView(parent)
{
    _camera = new OrthographicCamera();
    _validShaders = false;
    _brushColor = QColor(255,0,0);
}

void UVView::glPass()
{
    if (!_validShaders) {
        _meshShader = ShaderFactory::buildMeshShader(this);
        _validShaders = true;
    }

    glClearColor(.2,.2,.2,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 cameraProjM = _camera->getProjMatrix(width(), height());
    QMatrix4x4 cameraViewM = _camera->getViewMatrix(width(), height());
    QMatrix4x4 cameraProjViewM = cameraProjM * cameraViewM;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(cameraProjM.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(cameraViewM.data());

    glColor3f(0,0,0);
    glBegin(GL_LINES);
    {
        glVertex2f(0, 0);
        glVertex2f(10, 0);
        glVertex2f(0, 0);
        glVertex2f(0, 10);

        glVertex2f(1, 0);
        glVertex2f(1, 1);
        glVertex2f(0, 1);
        glVertex2f(1, 1);
    }
    glEnd();

    QMatrix4x4 objToWorld;

    Scene* scene = Scene::activeScene();

    // render each mesh
    QHashIterator<QString,Mesh*> meshes = scene->meshes();
    while (meshes.hasNext()) {
        meshes.next();
        Mesh* mesh = meshes.value();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, meshTexture(mesh));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, paintFbo()->texture());
        glActiveTexture(GL_TEXTURE0);

        _brushColor = QColor(255,255,0);

        _meshShader->bind();
        _meshShader->setUniformValue("objToWorld", objToWorld);
        _meshShader->setUniformValue("cameraPV", cameraProjViewM);
        _meshShader->setUniformValue("paintFboWidth", PAINT_FBO_WIDTH);
        _meshShader->setUniformValue("brushColor", _brushColor.redF(), _brushColor.greenF(), _brushColor.blueF(), 1);
        _meshShader->setUniformValue("meshTexture", 0);
        _meshShader->setUniformValue("paintTexture", 1);

        // draw mesh in UV space
        renderMesh(mesh, MeshPropType::UV, MeshPropType::UV);

        _meshShader->release();
    }

}

void UVView::painterPass(QPainter *painter)
{

}

