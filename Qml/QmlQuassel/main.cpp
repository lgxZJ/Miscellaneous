#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

#include "ThemeHolder.h"
#include "LanguageHolder.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<ThemeHolder>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<ListView>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<ColorBlock>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<TabView>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<TextEdit>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<AboutQuassel>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<ConfigureQuasselWindow>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<SimpleTreeView>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");
    qmlRegisterType<ToolBar>("lgxZJ.CppThemeHolder", 1, 0, "CppThemeHolder");

    qmlRegisterType<LanguageHolder>("lgxZJ.CppLanguageHolder", 1, 0, "CppLanguageHolder");

    //  WARNING:    The icon resource here must be referenced by `:/` prefix not `qrc:///`
    //  or it cannot be discovered! Following are their differenced:
    //      `:/`        ----    a file path
    //      `qrc:///`   ----    an url ( maybe a lcoal file url )
    app.setWindowIcon(QIcon(":/resources/quassel.ico"));

    QQmlApplicationEngine engine;

    //
    engine.rootContext()->setContextProperty("cppThemeHolderObject", new ThemeHolder);

    //
    LanguageHolder  languageHolder;
    QObject::connect(&languageHolder,   &LanguageHolder::languageChanged,
                     &engine,           &QQmlApplicationEngine::retranslate);
    engine.rootContext()->setContextProperty("cppLanguageHolderObject", &languageHolder);

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
