#include "mainwindow.h"
#include <QApplication>

#include <GL/freeglut.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    glutInit(&argc, argv);

    return a.exec();
}
