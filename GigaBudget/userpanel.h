#ifndef USERPANEL_H
#define USERPANEL_H

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>
#include "databasemanager.h"

class UserPanel : public QObject
{
    Q_OBJECT
public:
    explicit UserPanel(QObject *parent = nullptr);
    ~UserPanel();

    void creatingLoginPanel(QGridLayout* lay, DatabaseManager* db);
    void deleteUsersFromLoginPanel(QGridLayout* lay);

    QWidget *creatingGoals();
    QWidget* returnGridLaylout() {return _loginPanel;};

    QStringList loadingCategories(QString& cat);

private:
    QWidget* _loginPanel;
    QWidget* _goals;

signals:
    void login();
};

#endif // USERPANEL_H
