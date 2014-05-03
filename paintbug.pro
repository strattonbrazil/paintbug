#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T06:32:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paintbug
TEMPLATE = app
LIBS += -lglut -lassimp

SOURCES += main.cpp\
           mainwindow.cpp \
           glview.cpp \
    camera.cpp \
    transformable.cpp \
    util.cpp \
    shader.cpp \
    mesh.cpp \
    scene.cpp

HEADERS  += mainwindow.h \
            glview.h \
    camera.h \
    transformable.h \
    util.h \
    shader.h \
    mesh.h \
    scene.h
FORMS    += mainwindow.ui
