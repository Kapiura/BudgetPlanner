#include "include/userpanel.h"
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

QWidget* UserPanel::creatingLoginPanel(QStringList& userList)
{
    qDebug() << "Creating layout";

    QVBoxLayout* layout = new QVBoxLayout;

    foreach (QString username, userList)
    {
        QWidget* userPanel = new QWidget;
        QVBoxLayout* userLayout = new QVBoxLayout;

        QPushButton* loginButton = new QPushButton("Login");

        // connect(loginButton, &QPushButton::clicked, [=] ()
        //         {
        //             qDebug() << "Logging user: " << username;
        //         });

        QLabel* usernameLabel = new QLabel(username);
        userLayout->addWidget(usernameLabel);
        userLayout->addWidget(loginButton);

        userPanel->setLayout(userLayout);
        layout->addWidget(userPanel);

        connect(loginButton, &QPushButton::clicked, [=] ()
                {
            qDebug() << "Logging user: " << username;
            emit deleteLoginPanel();
        });
    }

    _loginPanel->setLayout(layout);
    return _loginPanel;
}



