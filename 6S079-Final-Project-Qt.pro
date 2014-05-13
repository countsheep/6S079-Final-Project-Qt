#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T15:21:56
#
#-------------------------------------------------

QT       += core gui
QT       += core gui opengl
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

LIBS += -L/usr/local/lib -lCGAL -lCGAL_Core -lmpfr -lgmp -lboost_system

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -DBOOST_NO_CXX11_NUMERIC_LIMITS
#QMAKE_CXXFLAGS += -arch x86_64
INCLUDEPATH +=   /usr/local/include

TARGET = 6S079-Final-Project-Qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    glwidget.cpp \
    vecmath/src/Matrix2f.cpp \
    vecmath/src/Matrix3f.cpp \
    vecmath/src/Matrix4f.cpp \
    vecmath/src/Quat4f.cpp \
    vecmath/src/Vector2f.cpp \
    vecmath/src/Vector3f.cpp \
    vecmath/src/Vector4f.cpp

HEADERS  += mainwindow.h \
    camera.h \
    glwidget.h \
    GL/freeglut.h \
    GL/freeglut_ext.h \
    GL/freeglut_std.h \
    GL/glut.h \
    vecmath/include/Matrix2f.h \
    vecmath/include/Matrix3f.h \
    vecmath/include/Matrix4f.h \
    vecmath/include/Quat4f.h \
    vecmath/include/vecmath.h \
    vecmath/include/Vector2f.h \
    vecmath/include/Vector3f.h \
    vecmath/include/Vector4f.h \
    Extra.h

FORMS    += mainwindow.ui

