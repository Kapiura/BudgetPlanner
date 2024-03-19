#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    // constructor
    //explicit DatabaseManager(QObject* parent = nullptr);
    DatabaseManager(const QString& dbName,const QString& userName,const QString& password,const QString&host,const int& port);
    ~DatabaseManager();

signals:

private:
    QSqlDatabase db;
    // variables required to connect to database
    QString _dbName;
    QString _userName;
    QString _password;
    QString _host;
    int _port;

};

#endif // DATABASEMANAGER_H
