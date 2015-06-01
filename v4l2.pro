#-------------------------------------------------
#
# Project created by QtCreator 2015-05-30T15:55:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = v4l2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        camera_thread.cpp\
	v4l2.cpp \
    utils.cpp \
    haar_cascade.cpp

HEADERS  += mainwindow.h \
        camera_thread.h\
	v4l2.h \
    utils.h \
    haar_cascade.h

FORMS    += mainwindow.ui

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_contrib\
        -lopencv_objdetect

RESOURCES +=
