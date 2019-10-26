#include <QMatrix4x4>
#include <QOpenGLFramebufferObjectFormat>

#include <GL/glext.h>

#include "perspectiveview.h"
#include "project.h"
#include "sessionsettings.h"
#include "glcache.h"

PerspectiveView::PerspectiveView(QWidget *parent) :
    GLView(parent)
{
    _camera = new PerspectiveCamera();
}

QString PerspectiveView::getViewLabel()
{
    return "Perspective";
}



