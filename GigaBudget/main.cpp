#include "mainwindow.h"

#include <QApplication>
#include <databasemanager.h>
#include <userpanel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.userPanelLoginLoad();
    w.userPanelLoad();
    return a.exec();
}
