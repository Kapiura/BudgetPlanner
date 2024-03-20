#ifndef USERPANEL_H
#define USERPANEL_H

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>

class UserPanel : public QObject
{
    Q_OBJECT
public:
    explicit UserPanel(QObject *parent = nullptr);

    void creatingLoginPanel(QSqlQuery& query);
    QWidget* returnGridLaylout() {return loginPanel.gridLaylout;};

signals:
private:
    struct LoginPanel
    {
        QWidget* gridLaylout;
        QStringList usersList;
        QString username;
        QVBoxLayout* layout;
        QWidget* userPanel;
        QVBoxLayout* userLayout;
        QPushButton* loginButton;
        QLabel* usernameLabel;
    };
    LoginPanel loginPanel;
};

#endif // USERPANEL_H
