#include "SqlDb.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QVariant>

#include <QDebug>

/////////////////////////////////////////////////////////////////////////////

SqlDb& SqlDb::instance()
{
    static SqlDb s_instance;

    return s_instance;
}

SqlDb::SqlDb()
    : m_statusIsValid(false)
{
    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        qDebug() <<"MySQL database is not available, available drivers:" << QSqlDatabase::drivers();
        return;
    }

    m_database = QSqlDatabase::addDatabase("QMYSQL", m_connectionName);
    if (!m_database.isValid()) {
        qDebug() << "MySQL cannot be loaded!";
        return;
    }

    m_database.setHostName("47.98.116.187");
    m_database.setPort(3306);
    m_database.setDatabaseName("WeightMonitor");
    m_database.setUserName("uuu");
    m_database.setPassword("password");
    if (!m_database.open()) {
        qDebug() << "dtabase open error, code:" << m_database.lastError();
        return;
    }

    if (!m_database.tables(QSql::Tables).contains("weightTable")) {
        qDebug() << "table named weightTable not found, trying to create it...";

        auto sqlCreateTable = QString("CREATE TABLE IF NOT EXISTS %1 (year INTEGER, month INTEGER, day INTEGER, weight REAL)")
                                .arg(m_tableName);
        QSqlQuery query(sqlCreateTable, m_database);
        if (!query.isActive()) {
            qDebug() << "create table error, code:" << query.lastError();
            return;
        }

        query.finish();
        qDebug() << "create table succeed!";
    }

    m_statusIsValid = true;
}

//////////////////////////////////////////////////////////////////////////////////

void SqlDb::addWeight(unsigned year, unsigned month, unsigned day, float weight)
{
    if (isStatusValid()) {
        auto sqlInsert = QString("INSERT INTO %1 VALUES(%2, %3, %4, %5)")
                            .arg(m_tableName)
                            .arg(year)
                            .arg(month)
                            .arg(day)
                            .arg(weight);
        QSqlQuery query(sqlInsert, m_database);

        if (!query.isActive()) {
            qDebug() << "insert data into db failed, code:" << query.lastError();
            return;
        }
        qDebug() << "insert one record into table succeed, rows affected:" << query.numRowsAffected();
    }
}

bool SqlDb::isStatusValid()
{
    if (!m_statusIsValid) {
        qDebug() << "database status is invalid";
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////

QVariantMap SqlDb::getMonthWeight(unsigned year, unsigned month)
{
    if (isStatusValid()) {
        auto selectStr = QString("SELECT ALL day, weight FROM %1 WHERE year = %2 AND month = %3")
                            .arg(m_tableName)
                            .arg(year)
                            .arg(month);
        QSqlQuery query(m_database);
        query.setForwardOnly(true);

        if (!query.exec(selectStr)) {
            qDebug() << "select error, string:" << selectStr << "error code:" << query.lastError();
            return {};
        }

        QVariantMap weights;
        while (query.next())
            weights[QString::number(query.value(0).toUInt())] = query.value(1).toFloat();

        query.finish();
        return weights;
    }

    return {};
}

////////////////////////////////////////////////////////////////////////////////////////////

bool SqlDb::containsDataOfDay(unsigned year, unsigned month, unsigned day)
{
    return getMonthWeight(year, month).contains(QString::number(day));
}
