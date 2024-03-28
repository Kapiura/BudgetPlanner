#ifndef USERPANEL_H
#define USERPANEL_H

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>
#include "databasemanager.h"

class Mainwindow;

class UserPanel : public QObject
{
    Q_OBJECT
public:
    explicit UserPanel(QObject *parent = nullptr);
    ~UserPanel();

    void creatingLoginPanel(QSqlQuery& query);
    //void creatingLoginPanel(QStringList& userList);
    QWidget* creatingLoginPanel(QStringList& userList);
    QWidget* creatingLoginPanel(QMap<QString, int>& users);
    QWidget *creatingGoals();
    QWidget* returnGridLaylout() {return _loginPanel;};

    QStringList loadingCategories(QString& cat);
    // QList<QStringList> loadingTableExpenses();



signals:
private:
    QWidget* _loginPanel;
    QWidget* _goals;
signals:
    void login(); // Declare the signal here
};

#endif // USERPANEL_H
