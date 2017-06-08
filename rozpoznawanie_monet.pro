#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T13:04:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rozpoznawanie_monet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\opencv\opencv\release\install\include
LIBS += -L"C:/opencv/opencv/release/install/x86/mingw/bin"

LIBS += -lopencv_imgproc310 -lopencv_highgui310 -lopencv_core310 -lopencv_imgcodecs310 -lopencv_videoio310 -lopencv_objdetect310 -lopencv_ml310
