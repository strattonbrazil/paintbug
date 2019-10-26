#include "uvview.h"
#include "project.h"
#include "sessionsettings.h"
#include "glcache.h"

UVView::UVView(QWidget *parent) :
    GLView(parent)
{
    _camera = new OrthographicCamera();
    _validShaders = false;
}

QString UVView::getViewLabel() {
    return "UV View";
}


