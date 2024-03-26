#include "mainwindow.h"
#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <qfile.h>
#include <QDir>
#include <QUiLoader>
#include <QMap>
#include <QStandardItem>

// -----------------------------------------
// CONSTRUCORS & DESCRUCTORS
// -----------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbHandler = new DatabaseManager("gigaBudget","root","!@#QWE123qwe","localhost",3306);
    up = new UserPanel();
    connect(up, &UserPanel::login, this, &MainWindow::login);

    this->setDefaultPageIndex();
}

MainWindow::~MainWindow()
{
    delete ui->tableExpenses->model();
    delete up;
    delete dbHandler;
    delete ui->users;
    delete ui;
}
// -----------------------------------------
// -----------------------------------------


// -----------------------------------------
// LOADING USERS, EXPENSES, INCOMES  etc.
// -----------------------------------------
void MainWindow::userPanelLoginLoad()
{
    // Query -  loading users and displaying them
    //          on user panel
    QString queryString = "SELECT name, idu FROM users;";

    // Executing a query
    QSqlQuery query(queryString,dbHandler->returnDataBase());
    // Map with username and their id
    QMap<QString, int> users;
    while(query.next())
    {
        QString username = query.value(0).toString();
        users.insert(query.value(0).toString(), query.value(1).toInt());
    }
    // creating widget with users
    QWidget* gl = up->creatingLoginPanel(users,dbHandler);
    // adding widget to ui
    ui->users->addWidget(gl);
}

// loggin into account - loading all the things
void MainWindow::login()
{
    // changing to page number 2
    ui->stackedWidget->setCurrentIndex(1);
    // change value of welcoming text
    ui->welcomeText->setText("Hello " + DatabaseManager::currentUsername);

    // userLoggedInSuccessfully = true;
    // list incomes, expenses, goals, saving for chosen user
    QString queryEx = QString("SELECT amount, currency, category, date, description FROM expenses where u_id = %1").arg(DatabaseManager::getUserId());
    QString queryIn = QString("SELECT amount, currency, category, date, description FROM incomes where u_id = %1").arg(DatabaseManager::getUserId());
    QString querySav = QString("SELECT amount, savings.currency, title, date, savings.description FROM savings left join goal on g_id=idg where savings.u_id =%1;").arg(DatabaseManager::getUserId());
    this->listExIn(queryEx, ui->tableExpenses);
    this->listExIn(queryIn, ui->tableIncomes);
    this->listSav(querySav, ui->tableSav);
    // list goals
    this->addingCategoriesItems();
}

// adding items to comboboxes in expenses and incomes
void MainWindow::addingCategoriesItems()
{
    // Query - loading and adding to combo box categories in expenses
    QString queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='expenses' AND COLUMN_NAME='category'";
    // exec the query
    QSqlQuery query(queryString, dbHandler->returnDataBase());
    // adding items to combo box
    ui->categoryExpenses->clear();
    while(query.next())
    {
        QString temp = query.value(0).toString();
        ui->categoryExpenses->addItems(up->loadingCategories(temp));
    }

    // Query - loading and adding to combo box categories in incomes
    queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='incomes' AND COLUMN_NAME='category'";
    // preparing new query
    query.prepare(queryString);
    // executing new query
    query.exec();
    // adding items to combo box
    ui->categoryIncomes->clear();
    while(query.next())
    {
        QString temp = query.value(0).toString();
        ui->categoryIncomes->addItems(up->loadingCategories(temp));
    }

    queryString = QString("select title from goal where u_id=%1;").arg(DatabaseManager::userId);
    query.prepare(queryString);
    query.exec();
    QStringList temp;
    ui->savingsGoal->clear();
    while(query.next())
    {
        QString temp = query.value(0).toString();
        qDebug() << temp;
        ui->savingsGoal->addItem(temp);
    }

}

// listing Expenses, Incomes, Savings
void MainWindow::listExIn(QString &queryString, QTableView* table)
{
    // headers for table
    QStringList headers = {"Amount", "Currency", "Category", "Date", "Description"};
    // making a query
    QSqlQuery query(queryString, dbHandler->returnDataBase());
    QList<QStringList> list;
    // adding records to table
    while(query.next())
    {
        QStringList tempList;
        tempList.append(QString::number(query.value(0).toDouble()));
        tempList.append(query.value(1).toString());
        tempList.append(query.value(2).toString());
        tempList.append(query.value(3).toString());
        tempList.append(query.value(4).toString());
        list.append(tempList);
    }
    // creating model of table
    QStandardItemModel *model = new QStandardItemModel(list.size(), headers.size());
    model->setHorizontalHeaderLabels(headers);
    // adding items to model
    for (int row = 0; row < list.size(); ++row)
    {
        for (int column = 0; column < list[row].size(); ++column)
        {
            QModelIndex index = model->index(row, column, QModelIndex());
            model->setData(index, list[row][column]);
        }
    }
    // displaying model
    table->setModel(model);
}

void MainWindow::listSav(QString &queryString, QTableView* table)
{
    // headers for table
    QStringList headers = {"Amount", "Currency", "Goal", "Date", "Description"};
    // making a query
    QSqlQuery query(queryString, dbHandler->returnDataBase());
    QList<QStringList> list;
    // adding records to table
    while(query.next())
    {
        QStringList tempList;
        tempList.append(QString::number(query.value(0).toDouble()));
        tempList.append(query.value(1).toString());
        tempList.append(query.value(2).toString());
        tempList.append(query.value(3).toString());
        tempList.append(query.value(4).toString());
        list.append(tempList);
    }
    // creating model of table
    QStandardItemModel *model = new QStandardItemModel(list.size(), headers.size());
    model->setHorizontalHeaderLabels(headers);
    // adding items to model
    for (int row = 0; row < list.size(); ++row)
    {
        for (int column = 0; column < list[row].size(); ++column)
        {
            QModelIndex index = model->index(row, column, QModelIndex());
            model->setData(index, list[row][column]);
        }
    }
    // displaying model
    table->setModel(model);
}
// -----------------------------------------
// -----------------------------------------


void MainWindow::setDefaultPageIndex()
{
    // setting defualts indexes
    ui->tabWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
    ui->paneluserTabs->setCurrentIndex(0);
};


// -----------------------------------------
// TRIGGER BUTTONS
// -----------------------------------------

void MainWindow::on_goalAdd_clicked()
{
    // getting things
    QString title = ui->goalTitle->text();
    double amount = ui->goalAmount->value();
    QString currency = ui->goalCurrency->currentText();
    QString desc = ui->goalDesc->toPlainText();
    qDebug() << title << amount << currency << desc;
    dbHandler->addGoal(title, amount, currency, desc);

    this->addingCategoriesItems();
}


void MainWindow::on_savingsButton_clicked()
{
    // getting things
    QString title = ui->savingsGoal->currentText();
    double amount = ui->savingsAmount->value();
    QString currency = ui->savingsCurrency->currentText();
    QString desc = ui->savingsDesc->toPlainText();
    dbHandler->addSav(title, amount, currency, desc);
    // qDebug() << title;

    QString querySav = QString("SELECT amount, savings.currency, title, date, savings.description FROM savings inner join goal on g_id=idg where savings.u_id =%1;").arg(DatabaseManager::getUserId());
    this->listSav(querySav, ui->tableSav);
}

void MainWindow::on_buttonLogout_clicked()
{
    // setting chosen user id and name
    DatabaseManager::userId = 0;
    DatabaseManager::currentUsername = "";
    // returning to selecting user
    ui->savingsGoal->clear();
    ui->categoryIncomes->clear();
    ui->categoryExpenses->clear();
    this->setDefaultPageIndex();
}

void MainWindow::on_btnCreateUser_clicked()
{
    // getting username and description
    QString username = ui->newUsername->text();
    QString desc = ui->newDesc->toPlainText();
    // adding user to database
    dbHandler->addUser(username, desc);

    // while( QLayoutItem* item = ui->users->layout()->takeAt(0) )
    // {
    //     item->widget()->setVisible(false);
    //     ui->users->layout()->removeItem(item);
    // }

    this->userPanelLoginLoad();
    this->setDefaultPageIndex();
}

void MainWindow::on_buttonIncomes_clicked()
{
    // variables of incomes
    int id = dbHandler->getUserId();
    double amount = ui->amountIncomes->value();
    QString currency = ui->currencyIncomes->currentText();
    QString desc = ui->descIncomes->toPlainText();
    QString category = ui->categoryIncomes->currentText();
    // making a query -  adding record to table incomes
    dbHandler->addIncomes(id, amount, currency, category, desc);

    QString queryIn = QString("SELECT amount, currency, category, date, description FROM incomes where u_id = %1").arg(DatabaseManager::getUserId());
    this->listExIn(queryIn, ui->tableIncomes);
}

void MainWindow::on_buttonExpenses_clicked()
{
    // variables of expenses
    int id = dbHandler->getUserId();
    double amount = ui->amountExpenses->value();
    QString currency = ui->currencyExpenses->currentText();
    QString desc = ui->descExpenses->toPlainText();
    QString category = ui->categoryExpenses->currentText();
    // making a query - adding record to table expenses
    dbHandler->addExpenses(id, amount, currency, category, desc);

    QString queryEx = QString("SELECT amount, currency, category, date, description FROM expenses where u_id = %1").arg(DatabaseManager::getUserId());
    this->listExIn(queryEx, ui->tableExpenses);

}
// -----------------------------------------
// -----------------------------------------
