#include "mainwindow.h"
#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <qfile.h>
#include <QDir>
#include <QUiLoader>
#include <QMap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbHandler = new DatabaseManager("gigaBudget","root","!@#QWE123qwe","localhost",3306);
    up = new UserPanel();
    connect(up, &UserPanel::login, this, &MainWindow::login);
}

MainWindow::~MainWindow()
{
    delete up;
    delete dbHandler;
    delete ui;
}

void MainWindow::userPanelLoginLoad()
{
    QString queryString = "SELECT name, idu FROM users;";
    QSqlQuery query(queryString,dbHandler->returnDataBase());
    QMap<QString, int> users;
    while(query.next())
    {
        QString username = query.value(0).toString();
        users.insert(query.value(0).toString(), query.value(1).toInt());

    }
    QWidget* gl = up->creatingLoginPanel(users);




    ui->tabWidget_2->setCurrentIndex(0);
    ui->gridLayout_4->addWidget(gl);
}

void MainWindow::login()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::userPanelLoad()
{
    QString queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='expenses' AND COLUMN_NAME='category'";
    QSqlQuery query(queryString, dbHandler->returnDataBase());
    query.exec();
    // loading categories in expenses
    while(query.next())
    {
        QString temp = query.value(0).toString();
        ui->category_expenses->addItems(up->loadingCategories(temp));
    }
    // loading categories in incomes
    queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='incomes' AND COLUMN_NAME='category'";
    query.prepare(queryString);
    query.exec();
    while(query.next())
    {
        QString temp = query.value(0).toString();
        ui->category_incomes->addItems(up->loadingCategories(temp));
    }


}






