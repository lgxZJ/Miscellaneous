#-------------------------------------------------
#
# Project created by QtCreator 2018-03-02T17:47:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DirectSound3D
TEMPLATE = app
DESTDIR = $$PWD/build

SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview.cpp \
    DragableGraphicsPixmapItem.cpp

HEADERS  += mainwindow.h \
    graphicsview.h \
    DragableGraphicsPixmapItem.h

FORMS    += mainwindow.ui
