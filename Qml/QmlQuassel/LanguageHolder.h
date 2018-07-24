#ifndef LANGUAGECHANGER_H
#define LANGUAGECHANGER_H

#include <QObject>
#include <QTranslator>
#include <QApplication>
#include <QDir>

#include <QDebug>

class LanguageHolder : public QObject
{
    Q_OBJECT
public:
    explicit LanguageHolder() {
        if (m_englishTranslator.load("qmlquassel_english.qm") == false) {
            qDebug() << "load english error"<< QDir::currentPath() ;
        }
        if (m_chineseTranslator.load("qmlquassel_chinese.qm") == false) {
            qDebug() << "load chinese error"<< QDir::currentPath() ;
        }
    }

    enum Languages { Chinese, English };
    Q_ENUM(Languages)

    Q_INVOKABLE void changeTo(Languages language) {
        qDebug() << "changing to " << language;

        switch (language) {
        case Chinese:
            qApp->removeTranslator(&m_englishTranslator);

            if (qApp->installTranslator(&m_chineseTranslator) == false) {
                qDebug() << "chinese installTranslator error"<< QDir::currentPath() << language;
            }
            break;
        case English:
            qApp->removeTranslator(&m_chineseTranslator);
            if (qApp->installTranslator(&m_englishTranslator) == false) {
                qDebug() << "english installTranslator error" << QDir::currentPath() << language;
            }
            break;

        default:    throw std::logic_error("Unexpected case, check your logic!");
        }

        emit languageChanged();
    }

signals:
    void languageChanged();

private:
    QTranslator m_englishTranslator;
    QTranslator m_chineseTranslator;
};

#endif // LANGUAGECHANGER_H
