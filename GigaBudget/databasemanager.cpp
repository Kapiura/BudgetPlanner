#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}, dbName("gigaBudget"),userName("root"),password("!@#QWE123qwe"),host("localhost"), port(3306)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setUserName(userName);
    db.setPassword(password);

    if(!db.open())
    {
        qDebug() << "Database connection error: " << db.lastError().text();
    }
    else
    {
        qDebug() << "Connecting to database went well, database name: " << dbName;
    }
}

DatabaseManager::~DatabaseManager()
{
    db.close();
}
