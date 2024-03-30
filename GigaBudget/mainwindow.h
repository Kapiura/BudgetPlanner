#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include "databasemanager.h"
#include "userpanel.h"
#include <QtSql/QSqlQuery>
#include <QStackedWidget>
#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>

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
    ~MainWindow();

    void addingCategoriesItems();
    void addingExpensesTables();
    void setDefaultPageIndex();

    int returnPageIndex() const {return ui->stackedWidget->currentIndex();};

    void listExIn(QString& queryString, QTableView* table);
    void listSav(QString &queryString, QTableView *table);

    DatabaseManager* returnDb() {return dbHandler;};

private:
    Ui::MainWindow *ui;
    DatabaseManager* dbHandler;
    UserPanel* up;
    QStackedWidget *stackedWidget;
    QDialog* dialogWindow;
    QDialogButtonBox* userDelete;
    QMessageBox* messDialog;

private slots:
    void login();
    void reloadInExSavGo();
    void on_buttonExpenses_clicked();
    void on_buttonIncomes_clicked();
    void on_btnCreateUser_clicked();
    void on_buttonLogout_clicked();
    void on_goalAdd_clicked();
    void on_savingsButton_clicked();
    void on_deleteButton_clicked();
    void on_userSettingsChange_clicked();
};
#endif // MAINWINDOW_H
