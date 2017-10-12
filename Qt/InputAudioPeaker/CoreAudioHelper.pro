QT += core
QT -= gui

CONFIG += c++11

TARGET = CoreAudioHelper
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    CoreAudioHelper.cpp

HEADERS += \
    CoreAudioHelper.h \
    PeakReceiver.h

LIBS += -lwinmm
