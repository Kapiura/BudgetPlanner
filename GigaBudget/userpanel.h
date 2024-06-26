#ifndef USERPANEL_H
#define USERPANEL_H

#include "databasemanager.h"
#include "graph.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QTableView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <array>

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
    void creatingGraph(int l, const QMap<QString, double> &map, QFrame *frame, QString &title);
    void clearGraph(int l);
    void updateGraph(int l, const QMap<QString, double> &map, QString &title);
    void setUserSettings(DatabaseManager *db, QLineEdit *username, QTextEdit *desc);
    void listExIn(QString &queryString, QTableView *table, DatabaseManager *dbHandler, Flag flaga);
    bool deleteRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName);
    std::array<QString, 4> getRowData(QStandardItemModel *model, const int &in, const int &b);
    std::array<QString, 5> getRowData(QStandardItemModel *model, const int &in);
    bool updateRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName,
                      std::array<QString, 5> rowData);
    bool updateRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName,
                      std::array<QString, 4> rowData);
    void currentBudget(QGridLayout *lay, DatabaseManager *db);

    bool checkIfEmpty(QDoubleSpinBox *b1, QComboBox *b2, QComboBox *b3, QTextEdit *b4);
    bool checkIfEmpty(QLineEdit *b1, QDoubleSpinBox *b2, QComboBox *b3, QPlainTextEdit *b4);
    void setDefaultBox(QDoubleSpinBox *b1, QComboBox *b2, QComboBox *b3, QTextEdit *b4);
    void setDefaultBox(QLineEdit *b1, QDoubleSpinBox *b2, QComboBox *b3, QPlainTextEdit *b4);

    void creatingGoals(QGridLayout *area, DatabaseManager *db);
    QWidget *returnGridLaylout()
    {
        return _loginPanel;
    };

    QStringList loadingCategories(QString &cat);

  private:
    QWidget *_loginPanel;
    QWidget *_goals;
    Graph **graph;

  signals:
    void login();
    void reloadInExSavGo();
};

#endif // USERPANEL_H
