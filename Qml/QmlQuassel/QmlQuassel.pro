TEMPLATE = app

QT += qml quick widgets
CONFIG += c++11

SOURCES += main.cpp \
    ThemeHolder.cpp

RESOURCES += qml.qrc

DESTDIR = $$PWD/build

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS +=          \
    ThemeHolder.h   \
    LanguageHolder.h

lupdate_only {
    SOURCES =   main.qml                                \
                JoinChannel.qml                         \
                components/*.qml                        \
                components/Common/*.qml                 \
                components/FileMenu/*.qml               \
                components/HelpMenu/*.qml               \
                components/HelpMenu/DebugMenu\*.qml     \
                components/MenuStyles/*.qml             \
                components/SettingsMenu/*.qml           \
                components/SettingsMenu/RemoteCore/*.qml\
                components/SettingsMenu/Ui/*.qml        \
                components/ThemeMenu/*.qml              \
                components/LanguagesMenu/*.qml
}

DISTFILES += \
    components/LanguagesMenu/LanguagesMenu.qml
