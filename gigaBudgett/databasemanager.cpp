#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{

}

bool DatabaseManager::connectToDatabase(const QString& dbName, const QString& userName, const QString& password, const QString& host, int port)
{
    m_database = QSqlDatabase::addDatabase("QMYSQL");
    m_database.setHostName(host);
    m_database.setPort(port);
    m_database.setDatabaseName(dbName);
    m_database.setUserName(userName);
    m_database.setPassword(password);

    if (!m_database.open()) {
        qDebug() << "Database connection error:" << m_database.lastError().text();
        return false;
    }

    qDebug() << "Connected to database:" << dbName;
    return true;
}

bool DatabaseManager::executeQuery(const QString& queryString)
{
    if (!m_database.isOpen()) {
        qDebug() << "Database connection is not open.";
        return false;
    }

    QSqlQuery query(queryString, m_database);

    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
        return false;
    }

    qDebug() << "Query executed successfully.";
    return true;
}
