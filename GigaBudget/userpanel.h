#ifndef USERPANEL_H
#define USERPANEL_H

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>
#include "databasemanager.h"
#include <QScrollArea>
#include <QLineEdit>
#include <QTableView>
#include <QTextEdit>

class UserPanel : public QObject
{
    Q_OBJECT
public:
    explicit UserPanel(QObject *parent = nullptr);
    ~UserPanel();

    enum Flag
    {
        Expenses,
        Incomes,
        Savings
    };

    void creatingLoginPanel(QGridLayout* lay, DatabaseManager* db);
    void deleteDynamicWidgets(QGridLayout* lay);
    void setUserSettings(DatabaseManager* db, QLineEdit* username, QTextEdit* desc);
    void listExIn(QString &queryString, QTableView* table,DatabaseManager* dbHandler,Flag flaga);
    bool deleteRecord(QString& table, int id, DatabaseManager* dbHandler,QString& idName);
    void currentBudget(QGridLayout* lay, DatabaseManager* db);

    // void creatingGoals();
    void creatingGoals(QGridLayout* area, DatabaseManager* db);
    QWidget* returnGridLaylout() {return _loginPanel;};

    QStringList loadingCategories(QString& cat);



private:
    QWidget* _loginPanel;
    QWidget* _goals;

signals:
    void login();
    void reloadInExSavGo();
};

#endif // USERPANEL_H
