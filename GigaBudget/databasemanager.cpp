#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QDebug>
#include <QtSql/QSqlDatabase>

QString DatabaseManager::currentUsername = "Dupa";
int DatabaseManager::userId = 0;

DatabaseManager::DatabaseManager(QObject *parent)
   : QObject{parent}
{
}

DatabaseManager::DatabaseManager(const QString &dbName, const QString &userName, const QString &password, const QString &host, const int &port)
    : _dbName(dbName),_userName(userName),_password(password),_host(host),_port(port)
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(_host);
    db.setPort(port);
    db.setDatabaseName(_dbName);
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

void DatabaseManager::addExpenses(int& id, double& amount, QString& currency, QString& category, QString& desc)
{
    QString queryString = QString("INSERT INTO expenses (u_id, amount, currency, date, description, category) "
                                  "VALUES (%1, %2, '%3', NOW(), '%4', '%5')")
                              .arg(id)
                              .arg(amount)
                              .arg(currency)
                              .arg(desc)
                              .arg(category);
    QSqlQuery query(queryString,db);
}

void DatabaseManager::addIncomes(int& id, double& amount, QString& currency, QString& category, QString& desc)
{
    QString queryString = QString("INSERT INTO incomes (u_id, amount, currency, date, description, category) "
                                  "VALUES (%1, %2, '%3', NOW(), '%4', '%5')")
                              .arg(id)
                              .arg(amount)
                              .arg(currency)
                              .arg(desc)
                              .arg(category);
    QSqlQuery query(queryString,db);
}

