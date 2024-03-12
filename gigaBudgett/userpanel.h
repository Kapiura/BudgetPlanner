#ifndef USERPANEL_H
#define USERPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>

class UserPanel : public QWidget
{
    Q_OBJECT
public:
    explicit UserPanel(QWidget *parent = nullptr);

    void creatingLoginPanel(QSqlQuery& query);

    QWidget* returnGridLayout(){return gridLayout;};

private:

    QWidget *gridLayout;



signals:
};

#endif // USERPANEL_H
