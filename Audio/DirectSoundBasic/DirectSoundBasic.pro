#-------------------------------------------------
#
# Project created by QtCreator 2018-01-05T13:42:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DirectSoundBasic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DirectSoundBasic.cpp

HEADERS  += mainwindow.h \
    DirectSoundBasic.h

FORMS    += mainwindow.ui

LIBS += -ldsound -lole32 -luser32
