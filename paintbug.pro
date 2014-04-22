#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T06:32:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paintbug
TEMPLATE = app
LIBS += -lglut

SOURCES += main.cpp\
           mainwindow.cpp \
           glview.cpp \
    camera.cpp \
    transformable.cpp \
    util.cpp

HEADERS  += mainwindow.h \
            glview.h \
    camera.h \
    transformable.h \
    util.h
FORMS    += mainwindow.ui
