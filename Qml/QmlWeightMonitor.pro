TEMPLATE = app

QT += qml quick widgets sql
CONFIG += c++11

SOURCES += main.cpp \
    SqlDb.cpp

RESOURCES += qml.qrc
DESTDIR = $$PWD/build

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    SqlDb.h
