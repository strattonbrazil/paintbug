#include "glview.h"

#include <iostream>


// store the first GL widget and use it as the shared widget
QGLWidget* firstWidget = 0;
QGLWidget* sharedWidget(QGLWidget* view)
{
    if (firstWidget)
        return firstWidget;
    else {
        firstWidget = view;
        return 0;
    }
}

GLView::GLView(QWidget *parent) :
    QGLWidget(parent, sharedWidget(this))
{
}

void GLView::initializeGL()
{
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

void GLView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

QHash<Mesh*,GLuint> meshTextures;


bool GLView::hasMeshTexture(Mesh *mesh)
{
    return meshTextures.contains(mesh);
}

GLuint GLView::meshTexture(Mesh *mesh)
{
    return meshTextures[mesh];
}

void GLView::setMeshTexture(Mesh *mesh, GLuint id)
{
    std::cout << "addnig mesh" << std::endl;
    meshTextures[mesh] = id;
}
