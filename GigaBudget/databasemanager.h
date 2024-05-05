#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QPushButton>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class DatabaseManager : public QObject {
  Q_OBJECT
public:
  // constructor
  explicit DatabaseManager(QObject *parent = nullptr);
  DatabaseManager(const QString &dbName, const QString &userName,
                  const QString &password, const QString &host,
                  const int &port);

  static int getUserId();
  static QString getCurrentUsername();

  ~DatabaseManager();

  QSqlQuery returnQuery(const QString &query);
  QSqlDatabase returnDataBase() { return db; };

  // adding expenses
  void addExpenses(int &id, double &amount, QString &currency,
                   QString &category, QString &desc);
  void addIncomes(int &id, double &amount, QString &currency, QString &category,
                  QString &desc);

  // user
  bool addUser(QString &name, QString &desc);
  void addGoal(QString &title, double &amount, QString &currency,
               QString &desc);
  void addSav(QString &title, double &amount, QString &currency, QString &desc);
  bool deleteUser();
  bool deleteData(QString &table);

  // current user variables
  static int userId;
  static QString currentUsername;

  bool exportData(const QString& username, const int& user_id);
  bool importData(const QString& username, const int& user_id);
  void addJsonObjectToFile(QSqlQuery& query,QTextStream& out);

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
