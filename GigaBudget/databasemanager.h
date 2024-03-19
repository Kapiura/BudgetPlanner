#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    // constructor
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

signals:

private:
    QSqlDatabase db;
    // variables required to connect to database
    QString dbName;
    QString userName;
    QString password;
    QString host;
    int port;

};

#endif // DATABASEMANAGER_H
