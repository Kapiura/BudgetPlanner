#include "mainwindow.h"
#include "databasemanager.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QMap>
#include <QStandardItem>
#include <QUiLoader>
#include <QtSql/QSqlError>
#include <qfile.h>

// -----------------------------------------
// CONSTRUCORS & DESCRUCTORS
// -----------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbHandler = new DatabaseManager("gigaBudget", "root", "!@#QWE123qwe", "localhost", 3306);
    up = new UserPanel();
    messDialog = new QMessageBox();

    dialogWindow = new QDialog();
    userDelete = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialogWindow);
    connect(userDelete, &QDialogButtonBox::accepted, [=]() { dialogWindow->accept(); });
    connect(userDelete, &QDialogButtonBox::rejected, [=]() { dialogWindow->reject(); });
    connect(up, &UserPanel::login, this, &MainWindow::login);
    connect(up, &UserPanel::reloadInExSavGo, this, &MainWindow::reloadInExSavGo);

    up->creatingLoginPanel(ui->users, dbHandler);
    this->setDefaultPageIndex();
}

MainWindow::~MainWindow()
{
    delete userDelete;
    delete dialogWindow;
    delete messDialog;
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

// loggin into account - loading all the things
void MainWindow::login()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->welcomeText->setText("Hello " + DatabaseManager::currentUsername);
    this->reloadInExSavGo();
    this->dailyQuote();
}

void MainWindow::reloadInExSavGo()
{
    QString query;

    up->deleteDynamicWidgets(ui->sumgoal);
    up->deleteDynamicWidgets(ui->summaryCards);
    query = QString("SELECT amount, currency, category, date, description, idi "
                    "FROM incomes where u_id = %1")
                .arg(DatabaseManager::getUserId());
    up->listExIn(query, ui->tableIncomes, dbHandler, UserPanel::Incomes);
    query = QString("SELECT amount, currency, category, date, description, ide "
                    "FROM expenses where u_id = %1")
                .arg(DatabaseManager::getUserId());
    up->listExIn(query, ui->tableExpenses, dbHandler, UserPanel::Expenses);
    query = QString("SELECT amount, savings.currency, title, date, savings.description, "
                    "ids FROM savings left join goal on g_id=idg where savings.u_id =%1;")
                .arg(DatabaseManager::getUserId());
    up->listExIn(query, ui->tableSav, dbHandler, UserPanel::Savings);
    up->creatingGoals(ui->sumgoal, dbHandler);
    up->currentBudget(ui->summaryCards, dbHandler);
    this->addingCategoriesItems();
}

// adding items to comboboxes in expenses and incomes
void MainWindow::addingCategoriesItems()
{
    // Query - loading and adding to combo box categories in expenses
    QString queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE "
                          "TABLE_NAME='expenses' AND COLUMN_NAME='category'";
    // exec the query
    QSqlQuery query(queryString, dbHandler->returnDataBase());
    // adding items to combo box
    ui->categoryExpenses->clear();
    while (query.next())
    {
        QString temp = query.value(0).toString();
        ui->categoryExpenses->addItems(up->loadingCategories(temp));
    }

    // Query - loading and adding to combo box categories in incomes
    queryString = "SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE "
                  "TABLE_NAME='incomes' AND COLUMN_NAME='category'";
    // preparing new query
    query.prepare(queryString);
    // executing new query
    query.exec();
    // adding items to combo box
    ui->categoryIncomes->clear();
    while (query.next())
    {
        QString temp = query.value(0).toString();
        ui->categoryIncomes->addItems(up->loadingCategories(temp));
    }

    queryString = QString("select title from goal where u_id=%1;").arg(DatabaseManager::userId);
    query.prepare(queryString);
    query.exec();
    QStringList temp;
    ui->savingsGoal->clear();
    while (query.next())
    {
        QString temp = query.value(0).toString();
        qDebug() << temp;
        ui->savingsGoal->addItem(temp);
    }
}

void MainWindow::listSav(QString &queryString, QTableView *table)
{
    // headers for table
    QStringList headers = {"Amount", "Currency", "Goal", "Date", "Description"};
    // making a query
    QSqlQuery query(queryString, dbHandler->returnDataBase());
    QList<QStringList> list;
    // adding records to table
    while (query.next())
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
    table->setModel(model);
}

void MainWindow::dailyQuote()
{
    int currentDay = QDate::currentDate().day();
    QString queryString = QString("SELECT * from quotes where id=%1").arg(currentDay);

    QSqlQuery query(queryString, dbHandler->returnDataBase());

    QMessageBox msgBox;
    if (query.next())
        msgBox.setText(QString("Quote of the day:\n%1").arg(query.value(1).toString()));
    msgBox.exec();
}

void MainWindow::messPopUp(QString &text, QString &title)
{
    QMessageBox mess(this);
    mess.setText(text);
    mess.setWindowTitle(title);
    mess.exec();
}

void MainWindow::setDefaultPageIndex()
{
    ui->tabWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
    ui->paneluserTabs->setCurrentIndex(0);
};

// -----------------------------------------
// TRIGGER BUTTONS
// -----------------------------------------

void MainWindow::on_goalAdd_clicked()
{
if(up->checkIfEmpty(ui->goalTitle,ui->goalAmount,ui->goalCurrency,ui->goalDesc))
{
        QString title = ui->goalTitle->text();
        double amount = ui->goalAmount->value();
        QString currency = ui->goalCurrency->currentText();
        QString desc = ui->goalDesc->toPlainText();
        qDebug() << title << amount << currency << desc;
        dbHandler->addGoal(title, amount, currency, desc);
        this->addingCategoriesItems();
        up->setDefaultBox(ui->goalTitle,ui->goalAmount,ui->goalCurrency,ui->goalDesc);
        this->reloadInExSavGo();
    }
    else
    {
        QString str = "Not enough data";
        QString title = "Error";
        this->messPopUp(str,title);
    }

}

void MainWindow::on_savingsButton_clicked()
{
    if(up->checkIfEmpty(ui->savingsAmount, ui->savingsCurrency, ui->savingsGoal, ui->savingsDesc))
  {
    QString title = ui->savingsGoal->currentText();
    double amount = ui->savingsAmount->value();
    QString currency = ui->savingsCurrency->currentText();
    QString desc = ui->savingsDesc->toPlainText();
    dbHandler->addSav(title, amount, currency, desc);
    up->setDefaultBox(ui->savingsAmount, ui->savingsCurrency, ui->savingsGoal, ui->savingsDesc);
    this->reloadInExSavGo();
    }
    else
    {
      QString str = "Not enough data";
      QString title = "Error";
      this->messPopUp(str,title);
  }
}

void MainWindow::on_buttonLogout_clicked()
{
    // set chosen user id and name
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
    if (dbHandler->addUser(username, desc))
    {
        qDebug() << "User has been added succuessful";
        QVBoxLayout *layout = new QVBoxLayout;
        QLabel *usernameLabel = new QLabel("k");
        layout->addWidget(usernameLabel);
        this->setDefaultPageIndex();
        up->deleteDynamicWidgets(ui->users);
        up->creatingLoginPanel(ui->users, dbHandler);
    }
    else
    {
        qDebug() << "Something gone wrong dayum";
    }
}

void MainWindow::on_buttonIncomes_clicked()
{
      if(up->checkIfEmpty(ui->amountIncomes, ui->currencyIncomes, ui->categoryIncomes, ui->descIncomes))
    {
          // variables of incomes
          int id = dbHandler->getUserId();
          double amount = ui->amountIncomes->value();
          QString currency = ui->currencyIncomes->currentText();
          QString desc = ui->descIncomes->toPlainText();
          QString category = ui->categoryIncomes->currentText();
          // making a query -  adding record to table incomes
          dbHandler->addIncomes(id, amount, currency, category, desc);

          QString queryIn = QString("SELECT amount, currency, category, date, "
                                    "description, idi FROM incomes where u_id = %1")
                                .arg(DatabaseManager::getUserId());
          up->listExIn(queryIn, ui->tableIncomes, dbHandler, UserPanel::Incomes);
          up->setDefaultBox(ui->amountIncomes, ui->currencyIncomes, ui->categoryIncomes, ui->descIncomes);
          this->reloadInExSavGo();
      }
    else
      {
        QString str = "Not enough data";
        QString title = "Error";
        this->messPopUp(str,title);
    }

}

void MainWindow::on_buttonExpenses_clicked()
{
    if(up->checkIfEmpty(ui->amountExpenses, ui->currencyExpenses, ui->categoryExpenses, ui->descExpenses))
    {
        int id = dbHandler->getUserId();
        double amount = ui->amountExpenses->value();
        QString currency = ui->currencyExpenses->currentText();
        QString desc = ui->descExpenses->toPlainText();
        QString category = ui->categoryExpenses->currentText();
        // making a query - adding record to table expenses
        dbHandler->addExpenses(id, amount, currency, category, desc);

        QString queryEx = QString("SELECT amount, currency, category, date, "
                                  "description, ide FROM expenses where u_id = %1")
                              .arg(DatabaseManager::getUserId());
        up->listExIn(queryEx, ui->tableExpenses, dbHandler, UserPanel::Expenses);
        up->setDefaultBox(ui->amountExpenses, ui->currencyExpenses, ui->categoryExpenses, ui->descExpenses);
        this->reloadInExSavGo();
    }
    else
    {
        QString str = "Not enough data";
        QString title = "Error";
        this->messPopUp(str,title);
    }

}

void MainWindow::on_deleteButton_clicked()
{

    // dialogWindow->show();
    if (dialogWindow->exec())
    {
        qDebug() << "deleting";
        if (dbHandler->deleteUser())
        {
            qDebug() << QString("User %1 has been deleted").arg(DatabaseManager::userId);
            this->setDefaultPageIndex();
            up->deleteDynamicWidgets(ui->users);
            up->creatingLoginPanel(ui->users, dbHandler);
        }
        else
            qDebug() << "something gone wrong during deleting user";
    }
    else
        qDebug() << "Deleting stopped";
    }
// -----------------------------------------
// -----------------------------------------

void MainWindow::on_userSettingsChange_clicked()
{
    QString newName = ui->userSettingsUsername->text();
    QString newDesc = ui->userSettingsDesc->toPlainText();
    QString queryString = QString("Update users SET name='%1', description='%2' where idu = %3")
                              .arg(newName)
                              .arg(newDesc)
                              .arg(DatabaseManager::getUserId());
    QSqlQuery query(dbHandler->returnDataBase());
    query.prepare(queryString);
    if (query.exec())
    {
        DatabaseManager::currentUsername = newName;
        qDebug() << query.lastError().text();
        up->deleteDynamicWidgets(ui->users);
        up->creatingLoginPanel(ui->users, dbHandler);
        ui->welcomeText->setText("Hello " + DatabaseManager::currentUsername);
        this->reloadInExSavGo();
    }
}
