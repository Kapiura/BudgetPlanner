#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <databasemanager.h>
#include <userpanel.h>

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

    void load_users();

private slots:

private:
    Ui::MainWindow *ui;
    DatabaseManager db;
    UserPanel paneluser;
};
#endif // MAINWINDOW_H
