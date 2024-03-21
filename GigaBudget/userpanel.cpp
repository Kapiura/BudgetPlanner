#include "userpanel.h"
#include "databasemanager.h"
#include <QtSql/QSqlError>

UserPanel::UserPanel(QObject *parent)
    : QObject{parent}
{
    _loginPanel = new QWidget;
    qDebug() << "Creating userpanel object";
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
        qDebug() << username << " " << id;
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


