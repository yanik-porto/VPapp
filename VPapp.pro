#-------------------------------------------------
#
# Project created by QtCreator 2016-03-27T23:35:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VPapp
TEMPLATE = app


SOURCES += apps/main.cpp\
        common/mainwindow.cpp \
    common/methods/channelsfilter.cpp \
    common/processengine.cpp \
    common/imgprocfunctions.cpp \
    common/methods/ipmethod.cpp \
    common/methods/sandp.cpp \
    common/methods/filters.cpp \
    common/methods/derivatives.cpp \
    common/methods/featuresPts.cpp \
    common/methods/colorbasics.cpp \
    common/methods/flipimg.cpp \
    common/methods/resizeimg.cpp \
    common/methods/houghtrans.cpp \
    common/methods/addlogo.cpp

HEADERS  += common/mainwindow.h \
    common/methods/channelsfilter.h \
    common/processengine.h \
    common/imgprocfunctions.h \
    common/methods/ipmethod.h \
    common/methods/sandp.h \
    common/methods/filters.h \
    common/methods/derivatives.h \
    common/methods/featuresPts.h \
    common/methods/colorbasics.h \
    common/methods/flipimg.h \
    common/methods/resizeimg.h \
    common/methods/houghtrans.h \
    common/methods/addlogo.h

FORMS    += common/mainwindow.ui

#Change for your own opencv
INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L"/usr/local/lib"
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lopencv_features2d -lopencv_xfeatures2d -lopencv_stitching -lopencv_calib3d

