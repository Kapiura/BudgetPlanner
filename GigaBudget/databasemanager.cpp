#include "databasemanager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSqlRecord>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QUrl>
#include <QDesktopServices>
#include <QFileDialog>
#include <QJsonParseError>
#include <QJsonObject>

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

bool DatabaseManager::addUser(QString &name, QString &desc)
{
    QString queryString = QString("INSERT INTO users (name,description) "
                                  "VALUES ('%1', '%2')")
                              .arg(name)
                              .arg(desc);
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
    QString queryString = QString("SELECT idg FROM goal WHERE title = '%1'").arg(title);
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
        qDebug() << "Deleting tables";
        QString tables[4] = {"savings", "incomes", "expenses", "goal"};
        for (auto &tab : tables)
        {
            qDebug() << "Deleting " << tab;
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
    std::array<QString, 3> tables = {"expenses", "incomes", "savings"};
    for (auto &el : tables)
    {
        queryString =
            QString(
                "SELECT JSON_OBJECT"
                "('amount',amount,'currency',currency,'date',date,'description',description,'table','%1')"
                " AS json FROM %1 WHERE u_id=%2;")
                .arg(el)
                .arg(user_id);
        query.prepare(queryString);
        qDebug() << el;
        if (!query.exec())
        {
            qDebug() << "Error: query executing\n";
            return false;
        }
     this->addJsonObjectToFile(query,out);
    }
    //goals
    queryString = QString("SELECT JSON_OBJECT"
                          "('goal_amount',goal_amount,'current_amount',current_amount,"
                          "'currency',currency,'description',description,"
                          "'title',title,'table','goal') "
                          "AS json FROM goal WHERE u_id=%1").arg(user_id);
    query.prepare(queryString);
    if(!query.exec())
    {
        qDebug() << "Error: query executing\n";
        return false;
    }
    this->addJsonObjectToFile(query,out);

    file.close();
    return true;
}

bool DatabaseManager::importData(const QString& username, const int& user_id)
{

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setWindowTitle("Open File");
    QString filePath;

    if (dialog.exec()) {
        QStringList fileNames = dialog.selectedFiles();
        filePath = fileNames.first();
        qDebug() << "Selected file path:" << filePath;
    }
    if(filePath.endsWith(".json", Qt::CaseInsensitive))
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
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error: cannot open file\n";
    }

    while (!file.atEnd()) {
            QByteArray line = file.readLine().trimmed(); // Read and remove any trailing whitespace, including the newline character

            if (line.isEmpty()) {
                continue;
            }

            QJsonParseError jsonError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(line, &jsonError);
            if (jsonError.error != QJsonParseError::NoError) {
                qDebug() << "Error: failed to parse JSON:" << jsonError.errorString();
                return 1;
            }

            if (!jsonDoc.isObject()) {
                qDebug() << "JSON document is not an object";
                return 1;
            }

            QJsonObject jsonObj = jsonDoc.object();

            QString table = jsonObj["table"].toString();
            QString date = jsonObj["date"].toString();
            double amount = jsonObj["amount"].toDouble();
            QString currency = jsonObj["currency"].toString();
            QString description = jsonObj["description"].toString();

            qDebug() << "Table:" << table;
            qDebug() << "Date:" << date;
            qDebug() << "Amount:" << amount;
            qDebug() << "Currency:" << currency;
            qDebug() << "Description:" << description;
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
        out << data.trimmed() << "\n";
    }
}
