#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <databasemanager.h>
#include <userpanel.h>
#include <QtSql/QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void userPanelLoad();

private:
    Ui::MainWindow *ui;
    DatabaseManager* dbHandler;
    UserPanel* up;

};
#endif // MAINWINDOW_H
