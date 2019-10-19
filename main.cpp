#include "mainwindow.h"
#include <QApplication>

#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setSamples(0);
    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
