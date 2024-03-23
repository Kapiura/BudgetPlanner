#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QPushButton>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    // constructor
    explicit DatabaseManager(QObject* parent = nullptr);
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager(DatabaseManager &&) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;
    DatabaseManager &operator=(DatabaseManager &&) = delete;
    DatabaseManager(const QString &dbName, const QString &userName,
                    const QString &password, const QString &host,
                    const int &port);
    ~DatabaseManager();

    QSqlQuery returnQuery(const QString& query);
    QSqlDatabase returnDataBase(){ return db;};

    // adding expenses
    void addExpenses(int& id, double& amount, QString& currency, QString& category, QString& desc);
    void addIncomes(int& id, double& amount, QString& currency, QString& category, QString& desc);

    // current user variables
    static QString currentUsername;
    static int userId;

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

signals:
};

#endif // DATABASEMANAGER_H
