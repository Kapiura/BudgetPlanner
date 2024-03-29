#include "userpanel.h"
#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QMap>
#include <QProgressBar>

UserPanel::UserPanel(QObject *parent)
    : QObject{parent}
{
    _loginPanel = new QWidget;
}

UserPanel::~UserPanel()
{
    delete _loginPanel;
}

class MainWindow;
void UserPanel::creatingLoginPanel(QGridLayout* lay, DatabaseManager* db)
{
    QString queryString = "SELECT name, idu FROM users;";
    QSqlQuery query(db->returnDataBase());
    query.prepare(queryString);
    query.exec();
    QMap<QString, int> users;
    while(query.next())
    {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        users.insert(username,id);
    }
    QVector<QWidget*> widgets;
    for(auto [username,id]: users.asKeyValueRange())
    {
        QWidget* container = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout;

        QLabel* label = new QLabel(username);

        QPushButton* button = new QPushButton("Login");

        layout->addWidget(label);
        layout->addWidget(button);

        container->setLayout(layout);
        connect(button, &QPushButton::clicked,  [=] ()
                {
                    DatabaseManager::userId = id;
                    DatabaseManager::currentUsername = username;
                    // qDebug() << db->getUserId() << " " << db->getCurrentUsername();
                    emit login();
                });
        widgets.append(container);
    }

    for(auto& el: widgets)
    {
        lay->addWidget(el);
    }
}

void UserPanel::creatingGoals(QGridLayout* area, DatabaseManager* db)
{
    QString queryString = "select title, goal_amount, current_amount, currency, description from goal;";
    QSqlQuery query(db->returnDataBase());
    query.prepare(queryString);
    query.exec();
    while(query.next())
    {
        QString title = query.value(0).toString();
        double goal_amount = query.value(1).toDouble();
        double current_amount = query.value(2).toDouble();
        QString currency = query.value(3).toString();
        QString desc = query.value(4).toString();
        QString amount = QString("1% / 2%").arg(current_amount).arg(goal_amount);


        QWidget* container = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout;

        QLabel* labelTitle = new QLabel(title);
        QLabel* labelDesc = new QLabel(desc);
        QLabel* labelAmount = new QLabel(amount);

        QProgressBar* amountBar = new QProgressBar;
        amountBar->setOrientation(Qt::Horizontal);
        amountBar->setRange(0,goal_amount);
        amountBar->setValue(current_amount);


        layout->addWidget(labelTitle);
        layout->addWidget(labelDesc);
        layout->addWidget(labelAmount);
        layout->addWidget(amountBar);


        container->setLayout(layout);
        area->addWidget(container);
        // connect(button, &QPushButton::clicked,  [=] ()
        //         {
        //             DatabaseManager::userId = id;
        //             DatabaseManager::currentUsername = username;
        //             // qDebug() << db->getUserId() << " " << db->getCurrentUsername();
        //             emit login();
        //         });
    }
}

QStringList UserPanel::loadingCategories(QString& cat)
{
    // deleting enum word and ()
    std::string word = "enum";
    std::string temp;
    temp = cat.toStdString().erase(cat.toStdString().find(word),word.length());
    temp.erase(temp.find("("),1);
    temp.erase(temp.find(")"),1);

    // deleting ' from string
    size_t n;
    do
    {
        n = temp.find("'");
        if(n != std::string::npos)
        temp = temp.erase(n,1);
    }
    while(n != std::string::npos);

    // append every word into list of categories
    QStringList list;
    std::string goal = ",";
    int commaPos;
    int currPos = 0;
    int prevPos = 0;

    do
    {
        commaPos = temp.find(goal,currPos);

        if(commaPos >= 0)
        {
            currPos = commaPos;
            list.append(temp.substr(prevPos,currPos-prevPos).c_str());
            currPos++;
            prevPos = currPos;
        }
    }
    while(commaPos >= 0);

    list.append(temp.substr(prevPos,temp.length()).c_str());

    return list;
}

void UserPanel::deleteUsersFromLoginPanel(QGridLayout* lay)
{
    QLayoutItem *item;
    while ((item = lay->takeAt(0)) != nullptr) {
        delete item->widget(); // Usuń widget z komórki (jeśli istnieje)
        delete item; // Usuń element layoutu
    }
}


