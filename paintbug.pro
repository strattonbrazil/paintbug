#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T06:32:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paintbug
TEMPLATE = app
LIBS += -lglut -lassimp -lGLEW
CONFIG += c++11

SOURCES += main.cpp\
           mainwindow.cpp \
           glview.cpp \
    camera.cpp \
    transformable.cpp \
    util.cpp \
    shader.cpp \
    mesh.cpp \
    scene.cpp \
    views/perspectiveview.cpp \
    views/uvview.cpp \
    gl_util.cpp \
    sessionsettings.cpp \
    buttons/brushsizebutton.cpp \
    buttons/brushcolorbutton.cpp \
    buttons/labeledtoolbutton.cpp \
    exporttexturesdialog.cpp \
    exporttexturestablemodel.cpp \
    texturepathitemdelegate.cpp

HEADERS  += mainwindow.h \
            glview.h \
    camera.h \
    transformable.h \
    util.h \
    shader.h \
    mesh.h \
    scene.h \
    views/perspectiveview.h \
    views/uvview.h \
    gl_util.h \
    constants.h \
    sessionsettings.h \
    buttons/brushsizebutton.h \
    buttons/brushcolorbutton.h \
    buttons/labeledtoolbutton.h \
    exporttexturesdialog.h \
    exporttexturestablemodel.h \
    texturepathitemdelegate.h

FORMS    += mainwindow.ui \
    exporttexturesdialog.ui

OTHER_FILES += \
    README.md

RESOURCES += \
    brushes.qrc
