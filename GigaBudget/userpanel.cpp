#include "userpanel.h"
#include "databasemanager.h"
#include <QMap>
#include <QMessageBox>
#include <QProgressBar>
#include <QStandardItemModel>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

UserPanel::UserPanel(QObject *parent) : QObject{parent}
{
    _loginPanel = new QWidget;
}

UserPanel::~UserPanel()
{
    delete _loginPanel;
}

class MainWindow;
void UserPanel::creatingLoginPanel(QGridLayout *lay, DatabaseManager *db)
{
    QString queryString = "SELECT name, idu FROM users;";
    QSqlQuery query(db->returnDataBase());
    query.prepare(queryString);
    query.exec();
    QMap<QString, int> users;
    while (query.next())
    {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        users.insert(username, id);
    }
    QVector<QWidget *> widgets;
    for (auto [username, id] : users.asKeyValueRange())
    {
        QWidget *container = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;

        QLabel *label = new QLabel(username);

        QPushButton *button = new QPushButton("Login");

        layout->addWidget(label);
        layout->addWidget(button);

        container->setLayout(layout);
        connect(button, &QPushButton::clicked, [=]() {
            DatabaseManager::userId = id;
            DatabaseManager::currentUsername = username;
            emit login();
        });
        widgets.append(container);
    }

    for (auto &el : widgets)
    {
        lay->addWidget(el);
    }
}

void UserPanel::creatingGoals(QGridLayout *area, DatabaseManager *db)
{
    QString queryString = QString("select title, goal_amount, current_amount, currency, "
                                  "description, idg from goal where u_id LIKE %1;")
                              .arg(DatabaseManager::getUserId());
    QSqlQuery query(db->returnDataBase());
    query.prepare(queryString);
    query.exec();
    while (query.next())
    {
        QString title = query.value(0).toString();
        double goal_amount = query.value(1).toDouble();
        // double current_amount = query.value(2).toDouble();
        QString currency = query.value(3).toString();
        QString desc = query.value(4).toString();
        int id = query.value(5).toInt();

        QString tempStr = QString("select sum(amount) from savings where g_id=%1;").arg(id);
        QSqlQuery tempquery(tempStr, db->returnDataBase());
        double tempdouble;
        QProgressBar *amountBar = new QProgressBar;
        if (tempquery.next())
        {
            tempdouble = tempquery.value(0).toDouble();
            qDebug() << tempdouble;
        }
        else
            tempdouble = 0;
        tempStr = QString("update goal set current_value=%1 where idg=%2;").arg(tempdouble).arg(id);

        QString amount = QString("%1 / %2").arg(tempdouble).arg(goal_amount);

        QWidget *container = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;

        QLabel *labelTitle = new QLabel(title);
        QLabel *labelDesc = new QLabel(desc);
        QLabel *labelAmount = new QLabel(amount);

        amountBar->setOrientation(Qt::Horizontal);
        amountBar->setRange(0, goal_amount);
        amountBar->setValue(tempdouble);

        QPushButton *deleteButton = new QPushButton("Delete");

        layout->addWidget(labelTitle);
        layout->addWidget(labelDesc);
        layout->addWidget(labelAmount);
        layout->addWidget(amountBar);
        layout->addWidget(deleteButton);

        connect(deleteButton, &QPushButton::clicked, [=]() {
            QString tableName = "goal";
            QString table = "savings";
            QString ide = "idg";
            QString idg = "g_id";
            this->deleteRecord(table, id, db, idg);
            this->deleteRecord(tableName, id, db, ide);
            emit reloadInExSavGo();
        });

        container->setLayout(layout);
        area->addWidget(container);

        if (tempdouble >= goal_amount)
        {
            QMessageBox msgBox;
            msgBox.setText(
                QString("Congratulations! You have successfully saved %1 towards %2.").arg(goal_amount).arg(title));
            msgBox.exec();
            QString tableName = "goal";
            QString table = "savings";
            QString ide = "idg";
            QString idg = "g_id";
            this->deleteRecord(table, id, db, idg);
            this->deleteRecord(tableName, id, db, ide);
            emit reloadInExSavGo();
        }
    }
}

QStringList UserPanel::loadingCategories(QString &cat)
{
    // deleting enum word and ()
    std::string word = "enum";
    std::string temp;
    temp = cat.toStdString().erase(cat.toStdString().find(word), word.length());
    temp.erase(temp.find("("), 1);
    temp.erase(temp.find(")"), 1);

    // deleting ' from string
    size_t n;
    do
    {
        n = temp.find("'");
        if (n != std::string::npos)
            temp = temp.erase(n, 1);
    } while (n != std::string::npos);

    // append every word into list of categories
    QStringList list;
    std::string goal = ",";
    int commaPos;
    int currPos = 0;
    int prevPos = 0;

    do
    {
        commaPos = temp.find(goal, currPos);

        if (commaPos >= 0)
        {
            currPos = commaPos;
            list.append(temp.substr(prevPos, currPos - prevPos).c_str());
            currPos++;
            prevPos = currPos;
        }
    } while (commaPos >= 0);

    list.append(temp.substr(prevPos, temp.length()).c_str());

    return list;
}

void UserPanel::deleteDynamicWidgets(QGridLayout *lay)
{
    QLayoutItem *item;
    while ((item = lay->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
}

void UserPanel::setUserSettings(DatabaseManager *db, QLineEdit *username, QTextEdit *desc)
{
    QString queryString =
        QString("SELECT name, description FROM users WHERE idu = %1").arg(DatabaseManager::getUserId());
    QSqlQuery query(db->returnDataBase());
    query.prepare(queryString);
    if (query.exec())
    {
        if (query.next())
        {
            QString name = query.value(0).toString();
            QString description = query.value(1).toString();
            username->setText(name);
            desc->setText(description);
        }
        else
        {
            qDebug() << "No rows returned from the query.";
        }
    }
    else
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }
}

void UserPanel::listExIn(QString &queryString, QTableView *table, DatabaseManager *dbHandler, Flag flaga)
{
    switch (flaga)
    {
    case Expenses:
    case Incomes: {
        QStringList headers = {"Amount", "Currency", "Category", "Date", "Description", "Modify", "Delete"};
        QSqlQuery query(queryString, dbHandler->returnDataBase());
        QList<QStringList> list;
        QVector<int> ides;
        while (query.next())
        {
            QStringList tempList;
            tempList.append(QString::number(query.value(0).toDouble()));
            tempList.append(query.value(1).toString());
            tempList.append(query.value(2).toString());
            tempList.append(query.value(3).toString());
            tempList.append(query.value(4).toString());
            ides.append(query.value(5).toInt());
            list.append(tempList);
        }
        QStandardItemModel *model = new QStandardItemModel(list.size(), headers.size());
        model->setHorizontalHeaderLabels(headers);
        for (int row = 0; row < list.size(); ++row)
        {
            for (int column = 0; column < list[row].size(); ++column)
            {
                QModelIndex index = model->index(row, column, QModelIndex());
                model->setData(index, list[row][column]);
            }
        }
        table->setModel(model);
        for (int i = 0; i < model->rowCount(); i++)
        {
            QPushButton *mod = new QPushButton();
            mod->setText("Modify");
            table->setIndexWidget(model->index(i, 5), mod);
            connect(mod, &QPushButton::clicked, [=]()
            {
                QString tableName;
                QString ide;
                if (flaga == Expenses)
                {
                    tableName = "expenses";
                    ide = "ide";
                }
                else
                {
                    tableName = "incomes";
                    ide = "idi";
                }
                std::array<QString, 5> data = this->getRowData(model,i);

                this->updateRecord(tableName, ides[i], dbHandler, ide, data);
                emit reloadInExSavGo();
            });

            QPushButton *del = new QPushButton();
            del->setText("Delete");
            table->setIndexWidget(model->index(i, 6), del);
            connect(del, &QPushButton::clicked, [=]() {
                qDebug() << "del -> " << i << " ide: " << ides[i];
                QString tableName;
                QString ide;
                if (flaga == Expenses)
                {
                    tableName = "expenses";
                    ide = "ide";
                }
                else
                {
                    tableName = "incomes";
                    ide = "idi";
                }

                this->deleteRecord(tableName, ides[i], dbHandler, ide);
                emit reloadInExSavGo();
            });
        }
    }
    break;
    case Savings:
        QString tempQuery = "SELECT * FROM GOAL;";
        QSqlQuery query(dbHandler->returnDataBase());
        query.prepare(tempQuery);
        if(!query.exec())
        {
            QStringList headers = {"Amount", "Currency", "Goal", "Date", "Description", "Modify", "Delete"};
            query.prepare(queryString);
            query.exec();
            QList<QStringList> list;
            QVector<int> ides;
            while (query.next())
            {
                QStringList tempList;
                tempList.append(QString::number(query.value(0).toDouble()));
                tempList.append(query.value(1).toString());
                tempList.append(query.value(2).toString());
                tempList.append(query.value(3).toString());
                tempList.append(query.value(4).toString());
                ides.append(query.value(5).toInt());
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
            for (int i = 0; i < model->rowCount(); i++)
            {
                QPushButton *mod = new QPushButton();
                mod->setText("Modify");
                table->setIndexWidget(model->index(i, 5), mod);
                connect(mod, &QPushButton::clicked, [=]() {
                    QString tableName = "savings";
                    QString ide = "ids";
                    std::array<QString, 5> data = this->getRowData(model, i);
                    this->updateRecord(tableName, ides[i], dbHandler, ide, data);
                    emit reloadInExSavGo();
                });

                QPushButton *del = new QPushButton();
                del->setText("Delete");
                table->setIndexWidget(model->index(i, 6), del);
                connect(del, &QPushButton::clicked, [=]() {
                    qDebug() << "del -> " << i << " ide: " << ides[i];
                    QString tableName = "savings";
                    QString ide = "ids";
                    this->deleteRecord(tableName, ides[i], dbHandler, ide);
                    emit reloadInExSavGo();
                });
            }
        }


        break;
    }
}

bool UserPanel::deleteRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName)
{
    QString queryString = QString("DELETE FROM %1 WHERE %2 = %3;").arg(table).arg(idName).arg(id);
    QSqlQuery query(dbHandler->returnDataBase());
    if (!query.exec(queryString))
    {
        qDebug() << "Error executing query: " << query.lastError().text();
        return false;
    }
    return true;
}

bool UserPanel::updateRecord(QString &table, int id, DatabaseManager *dbHandler, QString &idName, std::array<QString,5> rowData)
{
    QString queryString = QString("UPDATE %1 SET amount=%4, currency='%5', description='%6' WHERE %2 = %3;")
            .arg(table).arg(idName).arg(id)
            .arg(rowData[0]).arg(rowData[1]).arg(rowData[4]);
    /*
    "amount"
    "curr"
    "rent"
    "date"
    "desc"
    */
    QSqlQuery query(dbHandler->returnDataBase());
    if(!query.exec(queryString))
    {
        qDebug() << "Error executing query: " << query.lastError().text();
        return false;
    }
    return true;
}

std::array<QString, 5> UserPanel::getRowData(QStandardItemModel* model, const int& in)
{
    std::array<QString, 5> result;
    for(int j = 0; j < result.size(); j++)
    {
        QModelIndex index = model->index(in,j);

        if(index.isValid())
        {
            QVariant dataV = index.data(Qt::DisplayRole);

            if(dataV.isValid())
            {
                result[j] = dataV.toString();
            }
        }
    }
    return result;
}

void UserPanel::currentBudget(QGridLayout *lay, DatabaseManager *db)
{
    QString queryString = QString("SELECT ((SELECT SUM(amount) FROM incomes WHERE u_id = %1) - "
                                  "(SELECT SUM(amount) FROM expenses WHERE u_id = %1));")
                              .arg(DatabaseManager::getUserId());
    QSqlQuery query(db->returnDataBase());
    query.prepare(queryString);
    query.exec();
    double id;
    while (query.next())
    {
        id = query.value(0).toDouble();
    }
    QWidget *container = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    QLabel *label = new QLabel(DatabaseManager::getCurrentUsername());
    QLabel *balance = new QLabel(QString("Current balance: %1").arg(id));

    layout->addWidget(label);
    layout->addWidget(balance);

    container->setLayout(layout);
    lay->addWidget(container);
}

// expenses, incomes, savings
bool UserPanel::checkIfEmpty(QDoubleSpinBox *b1, QComboBox *b2, QComboBox *b3, QTextEdit *b4)
{
   return (b1->value()>0 && !b2->currentText().isEmpty() && !b3->currentText().isEmpty() && !b4->toPlainText().isEmpty())?true:false;
}

// goal
bool UserPanel::checkIfEmpty(QLineEdit *b1, QDoubleSpinBox *b2, QComboBox *b3, QPlainTextEdit *b4)
{
    return (!b1->text().isEmpty() && !b2->text().isEmpty() && !b3->currentText().isEmpty() && !b4->toPlainText().isEmpty()) ? true:false;
}

void UserPanel::setDefaultBox(QDoubleSpinBox *b1, QComboBox *b2, QComboBox *b3, QTextEdit *b4)
{
    qDebug() << "Set default values";
    b1->setValue(0.0);
    b2->setCurrentIndex(0);
    b3->setCurrentIndex(0);
    b4->setText("");
}

void UserPanel::setDefaultBox(QLineEdit *b1, QDoubleSpinBox *b2, QComboBox *b3, QPlainTextEdit *b4)
{
    qDebug() << "Set default values";
    b1->setText("");
    b2->setValue(0.0);
    b3->setCurrentIndex(0);
    b4->setPlainText("");
}
