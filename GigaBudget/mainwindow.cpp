#include "mainwindow.h"
#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <qfile.h>
#include <QDir>
#include <QUiLoader>
#include <QMap>
#include <QStandardItem>


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
    delete ui->tableExpenses->model();
    delete up;
    delete dbHandler;
    delete ui->users;
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
    QWidget* gl = up->creatingLoginPanel(users,dbHandler);
    qDebug() << dbHandler->getUserId() << " " << dbHandler->getCurrentUsername();
    ui->tabWidget_2->setCurrentIndex(0);
    ui->users->addWidget(gl);
}

void MainWindow::login()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->welcomeText->setText("Hello " + DatabaseManager::currentUsername);
    userId = DatabaseManager::userId;
    userLoggedInSuccessfully = true;
}

void MainWindow::addingCategoriesItems()
{
    QString queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='expenses' AND COLUMN_NAME='category'";
    QSqlQuery query(queryString, dbHandler->returnDataBase());
    query.exec();
    // loading categories in expenses
    while(query.next())
    {
        QString temp = query.value(0).toString();
        qDebug() << temp;
        ui->categoryExpenses->addItems(up->loadingCategories(temp));
    }
    // loading categories in incomes
    queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='incomes' AND COLUMN_NAME='category'";
    query.prepare(queryString);
    query.exec();
    while(query.next())
    {
        QString temp = query.value(0).toString();
        qDebug() << temp;
        ui->categoryIncomes->addItems(up->loadingCategories(temp));
    }
}

void MainWindow::setDefaultPageIndex()
{
    ui->paneluserTabs->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
};


void MainWindow::on_buttonExpenses_clicked()
{
    int id = dbHandler->getUserId();
        //DatabaseManager::userId;
    double amount = ui->amountExpenses->value();
    QString currency = ui->currencyExpenses->currentText();
    QString desc = ui->descExpenses->toPlainText();
    QString category = ui->categoryExpenses->currentText();
    dbHandler->addExpenses(id, amount, currency, category, desc);
}

void MainWindow::on_buttonIncomes_clicked()
{
    int id = dbHandler->getUserId();
        //DatabaseManager::userId;
    double amount = ui->amountIncomes->value();
    QString currency = ui->currencyIncomes->currentText();
    QString desc = ui->descIncomes->toPlainText();
    QString category = ui->categoryIncomes->currentText();
    dbHandler->addIncomes(id, amount, currency, category, desc);
}

void MainWindow::listExpenses()
{
    QString queryString = QString("SELECT amount, currency, category, date, description FROM expenses;");
    qDebug() << "Id: " << dbHandler->getUserId();


    QStringList headers = {"Amount", "Currency", "Category", "Date", "Description"};

    QSqlQuery query(queryString, dbHandler->returnDataBase());
    QList<QStringList> list;

    while(query.next())
    {
        QStringList tempList;
            qDebug() << "yes " << DatabaseManager::userId;
            tempList.append(QString::number(query.value(0).toDouble()));
            tempList.append(query.value(1).toString());
            tempList.append(query.value(2).toString());
            tempList.append(query.value(3).toString());
            tempList.append(query.value(4).toString());
            list.append(tempList);
    }

    QStandardItemModel *model = new QStandardItemModel(list.size(), headers.size()); // Allocate on heap
    model->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < list.size(); ++row)
    {
        for (int column = 0; column < list[row].size(); ++column)
        {
            QModelIndex index = model->index(row, column, QModelIndex());
            model->setData(index, list[row][column]);
        }
    }

    ui->tableExpenses->setModel(model);
}


void MainWindow::listIncomes()
{

}

void MainWindow::loadDatabase(DatabaseManager* db)
{
    this->dbHandler = db;
}

void MainWindow::on_buttonLogout_clicked()
{
    DatabaseManager::userId = 0;
    DatabaseManager::currentUsername = "";
    this->setDefaultPageIndex();
}


void MainWindow::on_btnCreateUser_clicked()
{
    userLoggedInSuccessfully = false;
    QString username = ui->newUsername->text();
    QString desc = ui->newDesc->toPlainText();
    dbHandler->addUser(username, desc);
    if (ui->users) {
        QLayoutItem *item;
        while ((item = ui->users->takeAt(0)) != nullptr) {
            if (QWidget *widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
    }

    this->userPanelLoginLoad();
}

