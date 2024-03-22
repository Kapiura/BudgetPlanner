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

void UserPanel::loadingCategories(QString& cat)
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
    // QStringList list;
    qDebug() << temp; // 'rent','groceries','utilities','dining','travel','work','toiletries','household items','medicines','other'
    QStringList list;




    // if (query.exec()) {
    //     while (query.next()) {
    //         QString columnType = query.value(0).toString();
    //     }
    // } else {
    //     qDebug() << "Query failed: " << query.lastError().text();
    // }
}

