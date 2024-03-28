#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QDebug>
#include <QtSql/QSqlDatabase>

 int DatabaseManager::userId = 0;
 QString DatabaseManager::currentUsername = "";

 int DatabaseManager::getUserId()
 {
     return userId;
 }

 QString DatabaseManager::getCurrentUsername()
 {
     return currentUsername;
 }

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

void DatabaseManager::addUser(QString &name, QString &desc)
{
    QString queryString = QString("INSERT INTO users (name,description) "
                                  "VALUES ('%1', '%2')")
                              .arg(name)
                              .arg(desc);
    QSqlQuery query(queryString,db);
}

void DatabaseManager::addGoal(QString &title, double &amount, QString &currency, QString &desc)
{


    QString queryString = QString("insert into goal (title, goal_amount, current_amount, currency, description, u_id) values('%1', %2, 0, '%3', '%4', %5);")
                              .arg(title)
                              .arg(amount)
                              .arg(currency)
                              .arg(desc)
                              .arg(DatabaseManager::userId);
    QSqlQuery query(queryString,db);
}

void DatabaseManager::addSav(QString &title, double &amount, QString &currency, QString &desc)
{
    QString queryString = QString("SELECT idg FROM goal WHERE title = '%1'").arg(title);
    QSqlQuery query(queryString,db);
    if (query.next()) {
        int g_id = query.value(0).toInt();
        queryString = QString("INSERT INTO savings (u_id, g_id, amount, currency, date, description) "
                              "VALUES (%1, %2, %3, '%4', NOW(),'%5')")
                          .arg(DatabaseManager::userId)
                          .arg(g_id)
                          .arg(amount)
                          .arg(currency)
                          .arg(desc);
        query.prepare(queryString);
        query.exec();
    }
}

bool DatabaseManager::deleteUser()
{
    QString queryString = QString("DELETE FROM users WHERE idu = '%1'").arg(DatabaseManager::userId);
    QSqlQuery query(db);
    query.prepare(queryString);
    if(query.exec())
    {
        return true;
    }
    else
    {
        QString tables[4] = {"goal","incomes","expenses","savings"};
        for(auto& tab: tables)
        {
            this->deleteData(tab);
        }
        if(query.exec())
        {
            return true;
        }
        else
        {
            qDebug() << "Something gone wrong";
            qDebug() << db.lastError().text();
            return false;
        }
    }

}

bool DatabaseManager::deleteData(QString &table)
{
    QString queryString = QString("delete from %1 where u_id = %2").arg(table).arg(DatabaseManager::getUserId());
    QSqlQuery query(db);
    query.prepare(queryString);
    if(query.exec())
    {
        qDebug() << "Table " << table << " has been deleted";
        return true;
    }
    else
    {
        qDebug() << "Something gone wrong";
        qDebug() << query.lastError().text();
        return false;
    }

}
