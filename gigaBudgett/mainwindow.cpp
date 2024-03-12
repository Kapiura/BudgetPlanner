#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_users()
{
    // Checking if database is loaded and wokring
    if(!db.connectToDatabase("gigaBudget", "root", "!@#QWE123qwe"))
    {
        QMessageBox::warning(this, "Login", "Error during loggin into database - exiting program");
        exit(EXIT_FAILURE);
    }

    QString queryString = "SELECT username FROM users;";
    QSqlQuery query(queryString, db.return_db());

    paneluser.creatingLoginPanel(query);
    QWidget *gl = paneluser.returnGridLayout();
    ui->gridLayout->addWidget(gl);

}
