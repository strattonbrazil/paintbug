#include "uvview.h"
#include "scene.h"

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
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, _paintFbo->texture());
        //glActiveTexture(GL_TEXTURE0);

        _meshShader->bind();
        _meshShader->setUniformValue("objToWorld", objToWorld);
        _meshShader->setUniformValue("cameraPV", cameraProjViewM);
        _meshShader->setUniformValue("paintFboWidth", 200);
        _meshShader->setUniformValue("brushColor", _brushColor.redF(), _brushColor.greenF(), _brushColor.blueF(), 1);
        _meshShader->setUniformValue("meshTexture", 0);
        _meshShader->setUniformValue("paintTexture", 1);

        // draw mesh in UV space
        glBegin(GL_TRIANGLES);
        {
            const int NUM_TRIANGLES = mesh->numTriangles();
            for (int i = 0; i < NUM_TRIANGLES; i++) {
                for (int j = 0; j < 3; j++) {
                    const unsigned int vertIndex = mesh->_triangleIndices[i*3+j];
                    Point3 vert = mesh->_vertices[vertIndex];
                    Point2 uv = mesh->_uvs[vertIndex];
                    glTexCoord2f(uv.x(), uv.y());
                    glVertex2f(uv.x(), uv.y());
                }
            }
        }
        glEnd();

        _meshShader->release();
    }

}

void UVView::painterPass(QPainter *painter)
{

}

void UVView::mousePressEvent(QMouseEvent *event)
{

}

void UVView::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void UVView::mouseReleaseEvent(QMouseEvent *event)
{

}

void UVView::mouseMoveEvent(QMouseEvent *event)
{

}

void UVView::mouseDragEvent(QMouseEvent *event)
{

}

void UVView::keyPressEvent(QKeyEvent *event)
{

}
