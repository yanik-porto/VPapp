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
    imgprocfunctions.cpp \
    ipmethod.cpp \
    sandp.cpp \
    filters.cpp \
    derivatives.cpp \
    featuresPts.cpp \
    colorbasics.cpp \
    flipimg.cpp \
    resizeimg.cpp \
    houghtrans.cpp \
    addlogo.cpp

HEADERS  += mainwindow.h \
    processengine.h \
    imgprocfunctions.h \
    ipmethod.h \
    sandp.h \
    filters.h \
    derivatives.h \
    featuresPts.h \
    colorbasics.h \
    flipimg.h \
    resizeimg.h \
    houghtrans.h \
    addlogo.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include
LIBS += -L"/usr/local/lib"
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lopencv_features2d -lopencv_xfeatures2d -lopencv_stitching -lopencv_calib3d

