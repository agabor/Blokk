#-------------------------------------------------
#
# Project created by QtCreator 2012-07-02T18:57:48
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = OpenCVdemo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += C:\\OpenCV2.4\\include\\

LIBS += -LC:\\OpenCV2.4\\lib \
-lopencv_core241 \
-lopencv_highgui241 \
-lopencv_imgproc241 \
-lopencv_features2d241 \
-lopencv_calib3d241

