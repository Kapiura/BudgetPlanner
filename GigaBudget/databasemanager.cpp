#include "databasemanager.h"
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProcess>
#include <QSqlRecord>
#include <QString>
#include <QUrl>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

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

DatabaseManager::DatabaseManager(QObject *parent) : QObject{parent}
{
}

DatabaseManager::DatabaseManager(const QString &dbName, const QString &userName, const QString &password,
                                 const QString &host, const int &port)
    : _dbName(dbName), _userName(userName), _password(password), _host(host), _port(port)
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(_host);
    db.setPort(port);
    db.setDatabaseName(_dbName);
    db.setUserName(_userName);
    db.setPassword(_password);

    if (!db.open())
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

QSqlQuery DatabaseManager::returnQuery(const QString &query)
{
    QSqlQuery tempQuery(query, db);
    return tempQuery;
}

void DatabaseManager::addExpenses(int &id, double &amount, QString &currency, QString &category, QString &desc)
{
    QString queryString = QString("INSERT INTO expenses (u_id, amount, currency, "
                                  "date, description, category) "
                                  "VALUES (%1, %2, '%3', NOW(), '%4', '%5')")
                              .arg(id)
                              .arg(amount)
                              .arg(currency)
                              .arg(desc)
                              .arg(category);
    QSqlQuery query(queryString, db);
}

void DatabaseManager::addIncomes(int &id, double &amount, QString &currency, QString &category, QString &desc)
{
    QString queryString = QString("INSERT INTO incomes (u_id, amount, currency, "
                                  "date, description, category) "
                                  "VALUES (%1, %2, '%3', NOW(), '%4', '%5')")
                              .arg(id)
                              .arg(amount)
                              .arg(currency)
                              .arg(desc)
                              .arg(category);
    QSqlQuery query(queryString, db);
}

bool DatabaseManager::addUser(QString &name)
{
    QString queryString = QString("INSERT INTO users (name) "
                                  "VALUES ('%1')")
                              .arg(name);
    QSqlQuery query(db);
    query.prepare(queryString);
    if (query.exec())
    {
        qDebug() << "User has been created";
        return true;
    }
    else
    {
        qDebug() << "Something gone wrong during adding new user";
        qDebug() << query.lastError().text();
        return false;
    }
}

void DatabaseManager::addGoal(QString &title, double &amount, QString &currency, QString &desc)
{
    QString queryString = QString("insert into goal (title, goal_amount, current_amount, currency, "
                                  "description, u_id) values('%1', %2, 0, '%3', '%4', %5);")
                              .arg(title)
                              .arg(amount)
                              .arg(currency)
                              .arg(desc)
                              .arg(DatabaseManager::userId);
    QSqlQuery query(queryString, db);
}

void DatabaseManager::addSav(QString &title, double &amount, QString &currency, QString &desc)
{
    QString queryString = QString("SELECT idg FROM goal LEFT JOIN users ON idu=u_id where title='%2' and u_id=%1;")
                              .arg(DatabaseManager::userId)
                              .arg(title);
    QSqlQuery query(queryString, db);
    if (query.next())
    {
        int g_id = query.value(0).toInt();
        queryString = QString("INSERT INTO savings (u_id, g_id, amount, currency, "
                              "date, description) "
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
    if (query.exec())
    {
        return true;
    }
    else
    {
        QString tables[4] = {"savings", "incomes", "expenses", "goal"};
        for (auto &tab : tables)
        {
            this->deleteData(tab);
        }
        if (query.exec())
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
    if (query.exec())
    {
        return true;
    }
    else
    {
        qDebug() << "Something gone wrong";
        qDebug() << query.lastError().text();
        return false;
    }
}

bool DatabaseManager::exportData(const QString &username, const int &user_id)
{
    QSqlQuery query(db);
    QString fileName = QString(QDir::homePath() + "/BudgetPlanner-data-export-%1.json").arg(username);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Error: cannot open a file";
        return false;
    }
    QTextStream out(&file);
    QString queryString;
    std::array<QString, 2> tables = {"expenses", "incomes"};
    // goals
    queryString = QString("SELECT JSON_OBJECT"
                          "('goal_amount',goal_amount,'current_amount',current_amount,"
                          "'currency',currency,'description',description,"
                          "'title',title,'table','goal') "
                          "AS json FROM goal WHERE u_id=%1")
                      .arg(user_id);
    query.prepare(queryString);
    if (!query.exec())
    {
        qDebug() << "Error: query executing - goals\n";
        return false;
    }
    this->addJsonObjectToFile(query, out);
    // savings
    queryString = QString("SELECT JSON_OBJECT('amount',savings.amount,'currency',savings.currency,"
                          "'date',savings.date,'description',savings.description,'table','savings','title',title) "
                          "AS json FROM savings LEFT JOIN goal on idg=g_id WHERE savings.u_id=%1;")
                      .arg(user_id);
    query.prepare(queryString);
    if (!query.exec())
    {
        qDebug() << "Error: query executing - savings\n";
        return false;
    }
    this->addJsonObjectToFile(query, out);
    for (auto &el : tables)
    {
        queryString = QString("SELECT JSON_OBJECT"
                              "('amount',amount,'currency',currency,'date',date,'description',description,'table','%1')"
                              " AS json FROM %1 WHERE u_id=%2;")
                          .arg(el)
                          .arg(user_id);
        query.prepare(queryString);
        if (!query.exec())
        {
            qDebug() << "Error: query executing - " << el << "\n";
            return false;
        }
        this->addJsonObjectToFile(query, out);
    }
    file.close();
    return true;
}

bool DatabaseManager::importData(const QString &username, const int &user_id)
{

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setWindowTitle("Open File");
    QString filePath;

    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        filePath = fileNames.first();
    }
    if (filePath.endsWith(".json", Qt::CaseInsensitive))
    {
        qDebug() << "It is a json file\n";
    }
    else
    {
        qDebug() << "It isnt a json file\n";
        return false;
    }

    QSqlQuery query(db);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error: cannot open file\n";
    }

    while (!file.atEnd())
    {
        QByteArray line = file.readLine().trimmed();

        if (line.isEmpty())
        {
            continue;
        }

        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(line, &jsonError);
        if (jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << "Error: failed to parse JSON:" << jsonError.errorString();
            return 1;
        }

        if (!jsonDoc.isObject())
        {
            qDebug() << "JSON document is not an object";
            return 1;
        }

        QJsonObject jsonObj = jsonDoc.object();

        QString table = jsonObj["table"].toString();
        QString date = jsonObj["date"].toString();
        double amount = jsonObj["amount"].toDouble();
        QString currency = jsonObj["currency"].toString();
        QString description = jsonObj["description"].toString();
        QString title = jsonObj["title"].toString();
        double goal_amount = jsonObj["goal_amount"].toDouble();

        if (table == "goal")
        {
            QString queryStr = QString("INSERT INTO %1 (u_id, goal_amount, current_amount, currency, "
                                       "title,description) VALUES (%2,%3,0,'%4','%5','%6');")
                                   .arg(table)
                                   .arg(getUserId())
                                   .arg(goal_amount)
                                   .arg(currency)
                                   .arg(title)
                                   .arg(description);
            query.prepare(queryStr);
            if (!query.exec())
            {
                qDebug() << "Error: unable to exec query - goal";
                return 1;
            }
        }
        else if (table == "expenses" || table == "incomes")
        {
            QString queryStr =
                QString("INSERT INTO %1 (u_id, amount, currency, date, description) VALUES (%2,%3,'%4','%5','%6');")
                    .arg(table)
                    .arg(getUserId())
                    .arg(amount)
                    .arg(currency)
                    .arg(date)
                    .arg(description);
            query.prepare(queryStr);
            if (!query.exec())
            {
                qDebug() << "Error: unable to exec query - expenses or incomes";
                return 1;
            }
        }
        else
        {
            QString queryStr =
                QString("SELECT idg FROM goal WHERE u_id = %1 AND title LIKE '%2';").arg(getUserId()).arg(title);
            query.prepare(queryStr);
            if (!query.exec())
            {
                qDebug() << "Error: unable to exec query - geting gid";
                return 1;
            }
            int gid = 0;
            if (query.next())
            {
                gid = query.value(0).toInt();
            }
            queryStr = QString("INSERT INTO %1 (u_id, g_id, amount, currency, date, description) VALUES "
                               "(%2,%3,%4,'%5','%6','%7');")
                           .arg(table)
                           .arg(getUserId())
                           .arg(gid)
                           .arg(amount)
                           .arg(currency)
                           .arg(date)
                           .arg(description);
            query.prepare(queryStr);
            if (!query.exec())
            {
                qDebug() << "Error: unable to exec query - savings";
                return 1;
            }
        }
    }

    file.close();

    return true;
}

void DatabaseManager::addJsonObjectToFile(QSqlQuery &query, QTextStream &out)
{
    while (query.next())
    {
        QString data;
        for (int i = 0; i < query.record().count(); i++)
        {
            data += query.value(i).toString();
        }
        // out << data.trimmed() << "\n";
    }
}

int DatabaseManager::expensesAmount30days()
{
    QString queryString =
        // QString("SELECT SUM(amount) FROM expenses WHERE u_id = %1 AND date >= CURRENT_DATE - INTERVAL 30 DAY;")
            QString("SELECT SUM(amount) FROM expenses WHERE u_id = %1;")
            .arg(DatabaseManager::userId);
    QSqlQuery query(db);
    query.prepare(queryString);
    if (!query.exec())
    {
        qDebug() << "Error: unable to exec query";
        return 0;
    }

    if (query.next())
    {
        return query.value(0).toInt();
    }
    else
    {
        qDebug() << "No valid record found";
        return 0;
    }
}

int DatabaseManager::incomesAmount30days()
{
    QString queryString =
        // QString("SELECT SUM(amount) FROM incomes WHERE u_id = %1 AND date >= CURRENT_DATE - INTERVAL 30 DAY;")
            QString("SELECT SUM(amount) FROM incomes WHERE u_id = %1;")
            .arg(DatabaseManager::userId);
    QSqlQuery query(db);
    query.prepare(queryString);
    if (!query.exec())
    {
        qDebug() << "Error: unable to exec query";
        return 0;
    }

    if (query.next())
    {
        return query.value(0).toInt();
    }
    else
    {
        qDebug() << "No valid record found";
        return 0;
    }
}

QMap<QString, double> DatabaseManager::inExCatAmount(QString &el)
{
    QMap<QString, double> result;
    QSqlQuery query(db);
    QString queryString;
    QVector<QString> categories;
    if (el == "expenses")
    {
        categories = {"rent", "groceries",  "utilities",       "dining",    "travel",
                      "work", "toiletries", "household items", "medicines", "other"};
    }
    else
    {
        categories = {"salary", "bonus", "investment", "savings", "other"};
    }
    for (auto &cat : categories)
    {
        // QString queryString = QString("select sum(amount) from %1 where u_id=%2 and category='%3' AND date >= "
        //                               "CURRENT_DATE - INTERVAL 30 DAY;")
        QString queryString = QString("select sum(amount) from %1 where u_id=%2 and category='%3';")
                                  .arg(el)
                                  .arg(DatabaseManager::userId)
                                  .arg(cat);
        query.prepare(queryString);
        int temp = 0;
        if (!query.exec())
        {
            qDebug() << "Error executing query:" << query.lastError().text();
        }
        else
        {
            if (query.next())
                temp = query.value(0).toInt();
        }
        result.insert(cat, temp);
    }
    return result;
}

QMap<QString, double> DatabaseManager::ExIn()
{
    QMap<QString, double> result;
    result.insert("Incomes", incomesAmount30days());
    result.insert("Expenses", expensesAmount30days());
    return result;
}
