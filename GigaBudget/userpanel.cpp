#include "userpanel.h"
#include "databasemanager.h"
#include <QtSql/QSqlError>

UserPanel::UserPanel(QObject *parent)
    : QObject{parent}
{
    _loginPanel = new QWidget;
}

UserPanel::~UserPanel()
{
    delete _loginPanel;
}


QWidget* UserPanel::creatingLoginPanel(QMap<QString, int>& users)
{
    QVBoxLayout* layout = new QVBoxLayout;
    for (auto [key, value]: users.asKeyValueRange())
    {
        QString username = key;
        unsigned int id = value;
        QWidget* userPanel = new QWidget;
        QVBoxLayout* userLayout = new QVBoxLayout;
        QPushButton* loginButton = new QPushButton("Login");
        QLabel* usernameLabel = new QLabel(username);
        userLayout->addWidget(usernameLabel);
        userLayout->addWidget(loginButton);

        userPanel->setLayout(userLayout);
        layout->addWidget(userPanel);

        connect(loginButton, &QPushButton::clicked,  [=] ()
                {
                    DatabaseManager::currentUsername = username;
                    DatabaseManager::userId = id;
                    emit login();

                });
    }
    _loginPanel->setLayout(layout);
    return _loginPanel;
}

QStringList UserPanel::loadingCategories(QString& cat)
{
    std::string word = "enum";
    std::string temp;
    temp = cat.toStdString().erase(cat.toStdString().find(word),word.length());
    temp.erase(temp.find("("),1);
    temp.erase(temp.find(")"),1);
    size_t n;
    do
    {
        n = temp.find("'");
        if(n != std::string::npos)
        temp = temp.erase(n,1);
    }
    while(n != std::string::npos);

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
    qDebug() << list;
    return list;
}

