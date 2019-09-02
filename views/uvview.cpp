#include "uvview.h"
#include "project.h"
#include "gl_util.h"
#include "sessionsettings.h"
#include "texturecache.h"

UVView::UVView(QWidget *parent) :
    GLView(parent)
{
    _camera = new OrthographicCamera();
    _validShaders = false;
}

QString UVView::getViewLabel() {
    return "UV View";
}


