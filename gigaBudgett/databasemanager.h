#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H


#include <QObject>
#include <QtSql/QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);

    bool connectToDatabase(const QString& dbName, const QString& userName, const QString& password, const QString& host = "localhost", int port = 3306);
    bool executeQuery(const QString& queryString);
    QSqlQuery returnQuery(const QString& queryString);

    QSqlDatabase return_db(){ return m_database;};

signals:
private:
    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
