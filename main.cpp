#include "mainwindow.h"
#include <QApplication>

#include <QSurfaceFormat>

#include <GL/freeglut.h>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setSamples(0);
    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    glutInit(&argc, argv);

    return a.exec();
}
