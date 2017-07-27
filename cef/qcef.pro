#-------------------------------------------------
#
# Project created by QtCreator 2017-07-11T18:22:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qcef
TEMPLATE = app


#INCLUDEPATH += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32
#INCLUDEPATH += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32/libcef_dll

INCLUDEPATH += D:/cef/cef_binary_3.2526.1366.g8617e7c_windows32

QMAKE_CXXFLAGS_DEBUG += -MT
QMAKE_CXXFLAGS_RELEASE += -MT

QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:msvcrtd.lib
QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:msvcrt.lib

CONFIG( debug, debug|release ) {
    # debug
#    LIBS += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32/Debug/libcef.lib
#    LIBS += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32/Debug/cef_sandbox.lib
#    LIBS += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32/libcef_dll_wrapper/Debug/libcef_dll_wrapper.lib

    LIBS += D:/cef/cef_binary_3.2526.1366.g8617e7c_windows32/Debug/libcef.lib
    LIBS += D:/cef/cef_binary_3.2526.1366.g8617e7c_windows32/Debug/cef_sandbox.lib
    LIBS += D:/cef/cef_binary_3.2526.1366.g8617e7c_windows32/libcef_dll/Debug/libcef_dll_wrapper.lib

    LIBS += -L"C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86/" \
                    -ladvapi32 -luser32 -ldbghelp \
                    -lversion -lwinmm -lpsapi -lOle32\
                    -lws2_32
} else {
    # release
    LIBS += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32/Release/libcef.lib
    LIBS += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32/Release/cef_sandbox.lib
    LIBS += D:/cef/cef_binary_3.3029.1619.geeeb5d7_windows32/libcef_dll_wrapper/Release/libcef_dll_wrapper.lib

    LIBS += -L"C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86/" \
                    -ladvapi32 -luser32 -ldbghelp \
                    -lversion -lwinmm -lpsapi -lOle32\
                    -lws2_32
}

SOURCES += main.cpp \
    embededbrowser.cpp \
    mainwindow.cpp\
	XsLog.cpp

HEADERS  += \
    embededbrowser.h \
    mainwindow.h\
	XsLog.hpp

FORMS += \
    mainwindow.ui

#QMAKE_CFLAGS += /D_USING_V140_SDK71_
#QMAKE_CXXFLAGS += /D_USING_V140_SDK71_
#QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:WINDOWS,5.01
