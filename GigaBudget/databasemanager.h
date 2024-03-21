#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    // constructor
    explicit DatabaseManager(QObject* parent = nullptr);
    DatabaseManager(const QString& dbName,const QString& userName,const QString& password,const QString&host,const int& port);
    ~DatabaseManager();


    QSqlQuery returnQuery(const QString& query);
    QSqlDatabase returnDataBase(){ return db;};

    // current user
    static QString currentUsername;
    static int userId;

signals:

private:
    QSqlDatabase db;
    // variables required to connect to database
    QString _dbName;
    QString _userName;
    QString _password;
    QString _host;
    int _port;

    // query
    QSqlQuery query;




};

#endif // DATABASEMANAGER_H
