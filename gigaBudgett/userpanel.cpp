#include "userpanel.h"

UserPanel::UserPanel(QWidget *parent)
    : QWidget{parent}
{}

void UserPanel::creatingLoginPanel(QSqlQuery& query)
{
    QStringList usersList;
    while(query.next())
    {
        QString username = query.value(0).toString();
        usersList << username;
    }

    QVBoxLayout *layout = new QVBoxLayout;
    //layout = new QVBoxLayout;
    foreach(QString username, usersList)
    {
        QWidget *userPanel = new QWidget;
        QVBoxLayout *userLayout = new QVBoxLayout(userPanel);
        qDebug() << 4;

        QPushButton *loginButton = new QPushButton("Login");
        qDebug() << 5;

        connect(loginButton, &QPushButton::clicked, [=]() // Użyj [=]() -> to będzie kopia wartości
                {
                    qDebug() << "Logowanie użytkownika: " << username;
                });

        qDebug() << 6;

        QLabel *usernameLabel = new QLabel(username);

        userLayout->addWidget(usernameLabel);
        userLayout->addWidget(loginButton);
        qDebug() << 7;

        layout->addWidget(userPanel);
        qDebug() << 8;

        // Niezbędne jest usunięcie obiektu userPanel w odpowiednim czasie, aby uniknąć wycieków pamięci
        // Możesz to zrobić ręcznie, np. połączyć to z obiektem rodzica, lub użyć mechanizmu zarządzania pamięcią Qt (np. smart pointers)
    }

    qDebug() << 9;
    gridLayout->setLayout(layout);
}
