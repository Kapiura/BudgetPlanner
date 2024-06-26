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

    QString title1 = "Incomes and Expenses";
    QString title2 = "Expenses";
    QString title3 = "Incomes";
    QString ex = "expenses";
    QString in = "incomes";
    QMap<QString, double> ExInMap = dbHandler->ExIn();

    up->creatingGraph(0, ExInMap, ui->frameExIn, title1);
    up->creatingGraph(1, ExInMap, ui->frameEx, title2);
    up->creatingGraph(2, ExInMap, ui->frameIn, title3);

    QString styleSheet = R"(
           QPlainTextEdit, QTextEdit, QLineEdit, QComboBox, QDoubleSpinBox {
               background-color: #ffffff;
               color: #333;
               border: 1px solid #aaa;
               padding: 5px;
           }

           QComboBox QAbstractItemView {
               background-color: #ffffff;
               color: #333;
               selection-background-color: #4CAF50;
               selection-color: white;
           }

           QPushButton {
               background-color: #4CAF50;
               color: white;
               border: none;
               padding: 10px;
               border-radius: 5px;
           }

           QPushButton:hover {
               background-color: #45a049;
           }

           QTabWidget::pane {
               border: 1px solid #ccc;
               padding: 10px;
           }

           QTabBar::tab {
               background: #e0e0e0;
               color: #333;
               border: 1px solid #ccc;
               padding: 10px;
           }

           QTabBar::tab:selected {
               background: #d0d0d0;
           }

           QTableView {
               background-color: #ffffff;
               color: #333;
               border: 1px solid #ccc;
               gridline-color: #ccc;
           }

           QTableView::item {
               padding: 10px;
           }

           QProgressBar {
               border: 2px solid #4CAF50;
               border-radius: 5px;
               text-align: center;
               background-color: #f0f0f0;
               color: #333; /* Ustawienie ciemnego koloru tekstu */
           }

           QProgressBar::chunk {
               background-color: #4CAF50;
               width: 20px;
           }
           )";

    setStyleSheet(styleSheet);
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
    this->reloadInExSavGo();
    this->dailyQuote();

    QString title1 = "Incomes and Expenses";
    QString title2 = "Expenses";
    QString title3 = "Incomes";
    QString ex = "expenses";
    QString in = "incomes";
    QMap<QString, double> ExInMap = dbHandler->ExIn();
    QMap<QString, double> ExMap = dbHandler->inExCatAmount(ex);
    QMap<QString, double> InMap = dbHandler->inExCatAmount(in);

    up->updateGraph(0, ExInMap, title1);
    up->updateGraph(1, ExMap, title2);
    up->updateGraph(2, InMap, title3);
}

void MainWindow::reloadInExSavGo()
{
    QString query;

    up->deleteDynamicWidgets(ui->sumgoal);
    up->deleteDynamicWidgets(ui->summaryCards);
    up->currentBudget(ui->summaryCards, dbHandler);
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
    this->addingCategoriesItems();

    QString title1 = "Incomes and Expenses";
    QString title2 = "Expenses";
    QString title3 = "Incomes";
    QString ex = "expenses";
    QString in = "incomes";

    QMap<QString, double> ExInMap = dbHandler->ExIn();
    QMap<QString, double> ExMap = dbHandler->inExCatAmount(ex);
    QMap<QString, double> InMap = dbHandler->inExCatAmount(in);

    up->updateGraph(0, ExInMap, title1);
    up->updateGraph(1, ExMap, title2);
    up->updateGraph(2, InMap, title3);

    // up->currentBudget(ui->summaryCards, dbHandler);
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

void MainWindow::messPopUp(const QString &text, const QString &title)
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
    if (up->checkIfEmpty(ui->goalTitle, ui->goalAmount, ui->goalCurrency, ui->goalDesc))
    {
        QString title = ui->goalTitle->text();
        double amount = ui->goalAmount->value();
        QString currency = ui->goalCurrency->currentText();
        QString desc = ui->goalDesc->toPlainText();
        qDebug() << title << amount << currency << desc;
        dbHandler->addGoal(title, amount, currency, desc);
        this->addingCategoriesItems();
        up->setDefaultBox(ui->goalTitle, ui->goalAmount, ui->goalCurrency, ui->goalDesc);
        this->reloadInExSavGo();
    }
    else
    {
        QString str = "Not enough data";
        QString title = "Error";
        this->messPopUp(str, title);
    }
}

void MainWindow::on_savingsButton_clicked()
{
    if (up->checkIfEmpty(ui->savingsAmount, ui->savingsCurrency, ui->savingsGoal, ui->savingsDesc))
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
        this->messPopUp(str, title);
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
    QString username = ui->newUsername->text();
    // adding user to database
    if (username.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Your username isn't correct!");
    }
    else
    {
        if (dbHandler->addUser(username))
        {
            qDebug() << "User has been added succuessful";
            QVBoxLayout *layout = new QVBoxLayout;
            QLabel *usernameLabel = new QLabel("k");
            layout->addWidget(usernameLabel);
            this->setDefaultPageIndex();
            up->deleteDynamicWidgets(ui->users);
            up->creatingLoginPanel(ui->users, dbHandler);
            ui->newUsername->setText("");
        }
        else
        {
            qDebug() << "Something gone wrong dayum";
        }
    }
}

void MainWindow::on_buttonIncomes_clicked()
{
    if (up->checkIfEmpty(ui->amountIncomes, ui->currencyIncomes, ui->categoryIncomes, ui->descIncomes))
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
        this->messPopUp(str, title);
    }
}

void MainWindow::on_buttonExpenses_clicked()
{
    if (up->checkIfEmpty(ui->amountExpenses, ui->currencyExpenses, ui->categoryExpenses, ui->descExpenses))
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
        this->messPopUp(str, title);
    }
}

void MainWindow::on_deleteButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Potwierdzenie");
    msgBox.setText("Czy na pewno chcesz usunąć użytkownika?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret)
    {
    case QMessageBox::Yes:
        qDebug() << "deleting";
        if (dbHandler->deleteUser())
        {
            qDebug() << QString("User %1 has been deleted").arg(DatabaseManager::userId);
            this->setDefaultPageIndex();
            up->deleteDynamicWidgets(ui->users);
            up->creatingLoginPanel(ui->users, dbHandler);

            QMessageBox::information(
                this, "Usunięto użytkownika",
                QString("Użytkownik %1 został pomyślnie usunięty.").arg(DatabaseManager::getCurrentUsername()));
        }
        else
        {
            qDebug() << "something went wrong during deleting user";
            QMessageBox::critical(this, "Błąd", "Coś poszło nie tak podczas usuwania użytkownika.");
        }
        break;
    case QMessageBox::No:
        qDebug() << "Deleting stopped";
        break;
    default:
        // should never be reached
        break;
    }
}

// -----------------------------------------
// -----------------------------------------

void MainWindow::on_btnExportData_clicked()
{
    // pobieranie danych z bazdy danych essa
    if (dbHandler->exportData(DatabaseManager::getCurrentUsername(), DatabaseManager::getUserId()))
    {
        this->messPopUp("Export has been done successfully", "Export");
    }
    else
    {
        this->messPopUp("Something went wrong!", "Export");
    }
}

void MainWindow::on_btnImportData_clicked()
{
    if (dbHandler->importData(DatabaseManager::getCurrentUsername(), DatabaseManager::getUserId()))
    {
        this->messPopUp("Import has been done successfulluy", "Import");
        this->reloadInExSavGo();
    }
    else
    {
        this->messPopUp("Something went wrong!", "Import");
    }
}
