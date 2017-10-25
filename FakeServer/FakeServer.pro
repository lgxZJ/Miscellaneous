QT += core
QT -= gui

CONFIG += c++11

TARGET = FakeServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/fakeserver.cpp \
    src/Logger.cpp \
    src/main.cpp

HEADERS += \
    src/microhttpd.h \
    src/Logger.h \
    src/fakeserver.h

LIBS += -L$$PWD/lib -llibmicrohttpd
DESTDIR = $$PWD/bin
