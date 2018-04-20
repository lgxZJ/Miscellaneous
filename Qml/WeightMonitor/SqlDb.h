#ifndef SQLDB_H
#define SQLDB_H

#include <QSqlDatabase>
#include <QObject>
#include <QVariantMap>

#include <vector>

class SqlDb : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE static SqlDb&   instance();

    SqlDb();

    Q_INVOKABLE void        addWeight       (unsigned year, unsigned month, unsigned day, float weight);
    Q_INVOKABLE QVariantMap getMonthWeight  (unsigned year, unsigned month);
    Q_INVOKABLE bool        containsDataOfDay(unsigned year, unsigned month, unsigned day);

private:


    bool isStatusValid();

    const char* m_connectionName = "defaultConnections";
    const char* m_tableName = "weightTable";

    bool            m_statusIsValid;
    QSqlDatabase    m_database;
};

#endif // SQLDB_H
