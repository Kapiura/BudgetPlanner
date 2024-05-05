#ifndef USERPANEL_H
#define USERPANEL_H

#include "databasemanager.h"
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QScrollArea>
#include <QSqlQuery>
#include <QTableView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPlainTextEdit>
#include <array>
#include <QStandardItemModel>

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

    void creatingLoginPanel(QGridLayout *lay, DatabaseManager *db);
    void deleteDynamicWidgets(QGridLayout *lay);
    void setUserSettings(DatabaseManager *db, QLineEdit *username, QTextEdit *desc);
    void listExIn(QString &queryString, QTableView *table, DatabaseManager *dbHandler, Flag flaga);
    bool deleteRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName);
    // bool updateRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName);
    std::array<QString, 5> getRowData(QStandardItemModel* model, const int& in);
    bool updateRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName, std::array<QString,5> rowData);
    void currentBudget(QGridLayout *lay, DatabaseManager *db);

    bool checkIfEmpty(QDoubleSpinBox* b1, QComboBox* b2, QComboBox* b3, QTextEdit* b4);
    bool checkIfEmpty(QLineEdit* b1, QDoubleSpinBox* b2, QComboBox* b3, QPlainTextEdit* b4);
    void setDefaultBox(QDoubleSpinBox* b1, QComboBox* b2, QComboBox* b3, QTextEdit* b4);
    void setDefaultBox(QLineEdit* b1, QDoubleSpinBox* b2, QComboBox* b3, QPlainTextEdit* b4);


    // void creatingGoals();
    void creatingGoals(QGridLayout *area, DatabaseManager *db);
    QWidget *returnGridLaylout()
    {
        return _loginPanel;
    };

    QStringList loadingCategories(QString &cat);

  private:
    QWidget *_loginPanel;
    QWidget *_goals;

  signals:
    void login();
    void reloadInExSavGo();
};

#endif // USERPANEL_H
