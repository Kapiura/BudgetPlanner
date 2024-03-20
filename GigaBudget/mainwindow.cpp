#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbHandler = new DatabaseManager("gigaBudget","root","!@#QWE123qwe","localhost",3306);
    up = new UserPanel();
}

MainWindow::~MainWindow()
{
    delete up;
    delete dbHandler;
    delete ui;
}

void MainWindow::userPanelLoad()
{
    QString queryString = "SELECT name FROM users;";
    qDebug() << queryString;
    QSqlQuery query(queryString,dbHandler->returnDataBase());
    QStringList userList;
    while(query.next())
    {
        userList << query.value(0).toString();
    }
    QWidget* gl = up->creatingLoginPanel(userList);
    ui->gridLayout->addWidget(gl);
}

