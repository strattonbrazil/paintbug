#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T06:32:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paintbug
TEMPLATE = app
LIBS += -lassimp
CONFIG += c++11

SOURCES += main.cpp\
           mainwindow.cpp \
           glview.cpp \
    camera.cpp \
    transformable.cpp \
    util.cpp \
    shader.cpp \
    mesh.cpp \
    views/perspectiveview.cpp \
    views/uvview.cpp \
    sessionsettings.cpp \
    buttons/brushsizebutton.cpp \
    buttons/brushcolorbutton.cpp \
    buttons/labeledtoolbutton.cpp \
    exporttexturesdialog.cpp \
    exporttexturestablemodel.cpp \
    texturepathitemdelegate.cpp \
    texturebaker.cpp \
    project.cpp \
    glcache.cpp

HEADERS  += mainwindow.h \
            glview.h \
    camera.h \
    transformable.h \
    util.h \
    shader.h \
    mesh.h \
    views/perspectiveview.h \
    views/uvview.h \
    constants.h \
    sessionsettings.h \
    buttons/brushsizebutton.h \
    buttons/brushcolorbutton.h \
    buttons/labeledtoolbutton.h \
    exporttexturesdialog.h \
    exporttexturestablemodel.h \
    texturepathitemdelegate.h \
    texturebaker.h \
    project.h \
    glcache.h

FORMS    += mainwindow.ui \
    exporttexturesdialog.ui

OTHER_FILES += \
    README.md

RESOURCES += \
    main.qrc

DISTFILES +=
