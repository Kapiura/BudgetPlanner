#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <databasemanager.h>
#include <userpanel.h>
#include <QtSql/QSqlQuery>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    // MainWindow(DatabaseManager* db = , QWidget *parent = nullptr);
    ~MainWindow();

    void userPanelLoginLoad();
    void addingCategoriesItems();
    void addingExpensesTables();

    int returnPageIndex() const {return ui->stackedWidget->currentIndex();};
    void setDefaultPageIndex();

    void listExpenses();
    void listIncomes();

    void loadDatabase(DatabaseManager* db);

    DatabaseManager* returnDb() {return dbHandler;};

    bool returnSuccess(){return userLoggedInSuccessfully;};


private:
    Ui::MainWindow *ui;
    DatabaseManager* dbHandler;
    UserPanel* up;
    QStackedWidget *stackedWidget;

    bool userLoggedInSuccessfully = false;

    int userId;

private slots:
    void login();
    void on_buttonExpenses_clicked();
    void on_buttonIncomes_clicked();
    void on_btnCreateUser_clicked();
    void on_buttonLogout_clicked();
};
#endif // MAINWINDOW_H
