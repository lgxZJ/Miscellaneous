#include <QApplication>
#include <QQmlApplicationEngine>
#include <QJSValue>

#include "SqlDb.h"

#include <QDebug>

static QJSValue singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)

    QJSValue value = scriptEngine->newObject();
    value.setProperty("sqlInstance", (const char*)&SqlDb::instance());
    return value;
}

int main(int argc, char *argv[])
{
    qmlRegisterType<SqlDb>("lgxZJ", 1, 0, "SqlDb");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
