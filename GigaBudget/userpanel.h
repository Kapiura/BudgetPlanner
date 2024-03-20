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
    ~UserPanel();

    void creatingLoginPanel(QSqlQuery& query);
    //void creatingLoginPanel(QStringList& userList);
    QWidget* creatingLoginPanel(QStringList& userList);
    QWidget* returnGridLaylout() {return _loginPanel;};

signals:
private:
    QWidget* _loginPanel;
};

#endif // USERPANEL_H
