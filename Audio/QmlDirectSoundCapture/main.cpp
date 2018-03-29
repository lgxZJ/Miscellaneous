#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "CppDsCapture.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<CppDsCapture>("lgxZJ", 1, 0, "CppDsCapture");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
