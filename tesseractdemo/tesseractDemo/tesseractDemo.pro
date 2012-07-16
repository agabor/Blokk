#-------------------------------------------------
#
# Project created by QtCreator 2012-07-10T18:59:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = tesseractDemo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
..\\tesseract-3.01\\api\\*.cpp \
..\\tesseract-3.01\\ccmain\\*.cpp \
..\\tesseract-3.01\\ccutil\\*.cpp \
..\\tesseract-3.01\\ccstruct\\*.cpp \
..\\tesseract-3.01\\classify\\*.cpp \
..\\tesseract-3.01\\cube\*.cpp \
..\\tesseract-3.01\\cutil\\*.cpp \
..\\tesseract-3.01\\dict\\*.cpp \
..\\tesseract-3.01\\image\\*.cpp \
..\\tesseract-3.01\\textord\\*.cpp \
..\\tesseract-3.01\\viewer\\*.cpp \
..\\tesseract-3.01\\wordrec\\*.cpp \
..\\tesseract-3.01\\port\\*.cpp \
..\\tesseract-3.01\\neural_networks\\runtime\\*.cpp


INCLUDEPATH += ..\\tesseract-3.01\\api \
..\\tesseract-3.01\\ccmain \
..\\tesseract-3.01\\ccutil \
..\\tesseract-3.01\\ccstruct \
..\\tesseract-3.01\\classify \
..\\tesseract-3.01\\cube \
..\\tesseract-3.01\\cutil \
..\\tesseract-3.01\\dict \
..\\tesseract-3.01\\display \
..\\tesseract-3.01\\image \
..\\tesseract-3.01\\textord \
..\\tesseract-3.01\\viewer \
..\\tesseract-3.01\\wordrec \
..\\tesseract-3.01\\pageseg \
..\\tesseract-3.01\\include \
..\\tesseract-3.01\\include\\leptonica \
..\\tesseract-3.01\\port \
..\\tesseract-3.01\\neural_networks\\runtime


LIBS += ..\\tesseract-3.01\\lib\\*.lib
