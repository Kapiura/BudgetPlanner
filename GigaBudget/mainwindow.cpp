#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbHandler = new DatabaseManager("gigaBudget","root","!@#QWE123qwe","localhost",3306);
    up = new UserPanel(this);
}

MainWindow::~MainWindow()
{
    delete up;
    delete dbHandler;
    delete ui;
}

void MainWindow::userPanelLoad()
{
    QString queryString = "SELECT username from users;";
    QSqlQuery query = dbHandler->returnQuery(queryString);

    up->creatingLoginPanel(query);
    QWidget* gl = up->returnGridLaylout();
    ui->gridLayout->addWidget(gl); // Assuming ui->gridLayout is a layout, not a widget
}

