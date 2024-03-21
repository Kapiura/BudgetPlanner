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
    qDebug() << queryString;
    QSqlQuery query(queryString,dbHandler->returnDataBase());
    QMap<QString, int> users;
    while(query.next())
    {
        QString username = query.value(0).toString();
        qDebug() << username;
        users.insert(query.value(0).toString(), query.value(1).toInt());

    }
    QWidget* gl = up->creatingLoginPanel(users);
    ui->stackedWidget->setCurrentIndex(0);
    ui->verticalLayout->addWidget(gl);
}

void MainWindow::login()
{
    qDebug()<< "login";
    qDebug() << "Username = " << DatabaseManager::currentUsername << "\nid: " << DatabaseManager::userId;
    ui->stackedWidget->setCurrentIndex(1);
    ui->label->setText(DatabaseManager::currentUsername);
}

void MainWindow::userPanelLoad()
{
    qDebug() << "lol";
}






