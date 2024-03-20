#include "userpanel.h"

UserPanel::UserPanel(QObject *parent)
    : QObject{parent}
{
    loginPanel.gridLaylout = new QWidget;
}

void UserPanel::creatingLoginPanel(QSqlQuery& query)
{
    //LoginPanel loginPanel;
    while(query.next())
    {
        loginPanel.username = query.value(0).toString();
        loginPanel.usersList << loginPanel.username;
        qDebug() << loginPanel.username;
    }

    loginPanel.layout = new QVBoxLayout;

    foreach (QString username, loginPanel.usersList)
    {
        loginPanel.userPanel = new QWidget;
        loginPanel.userLayout = new QVBoxLayout(loginPanel.userPanel);

        loginPanel.loginButton = new QPushButton("Login");

        connect(loginPanel.loginButton, &QPushButton::clicked, [=]()
        {
            qDebug() << "Logging user: " << username;
        });

        loginPanel.usernameLabel = new QLabel(loginPanel.username);

        loginPanel.userLayout->addWidget(loginPanel.usernameLabel);
        loginPanel.userLayout->addWidget(loginPanel.loginButton);

        loginPanel.layout->addWidget(loginPanel.userPanel);
    }
    loginPanel.gridLaylout->setLayout(loginPanel.layout);
}


