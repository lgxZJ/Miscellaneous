#-------------------------------------------------
#
# Project created by QtCreator 2018-01-17T22:57:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DirectSoundPlay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    WavFile.cpp \
    WavPlayer.cpp

HEADERS  += mainwindow.h \
    WavFile.h \
    WavPlayer.h

FORMS    += mainwindow.ui

LIBS += -lwinmm -ldsound -ldxguid
