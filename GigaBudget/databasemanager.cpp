#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QDebug>
#include <QtSql/QSqlDatabase>

DatabaseManager::DatabaseManager(QObject *parent)
   : QObject{parent}
{
    qDebug() << "Default constructor of database manager class";
}
// connectiong to mysql client
DatabaseManager::DatabaseManager(const QString &dbName, const QString &userName, const QString &password, const QString &host, const int &port)
    : _dbName(dbName),_userName(userName),_password(password),_host(host),_port(port)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(_host);
    db.setPort(_port);
    db.setUserName(_userName);
    db.setPassword(_password);

    if(!db.open())
    {
        qDebug() << "Database connection error: " << db.lastError().text();
    }
    else
    {
        qDebug() << "Connecting to database went well, database name: " << _dbName;
    }
}

DatabaseManager::~DatabaseManager()
{
    db.close();
}

QSqlQuery DatabaseManager::returnQuery(const QString& query)
{
    QSqlQuery tempQuery(query,db);
    return tempQuery;
}

