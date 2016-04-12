#-------------------------------------------------
#
# Project created by QtCreator 2016-03-27T23:35:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VPapp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    processengine.cpp \
    imgprocfunctions.cpp

HEADERS  += mainwindow.h \
    processengine.h \
    imgprocfunctions.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include
LIBS += -L"/usr/local/lib"
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lopencv_features2d
