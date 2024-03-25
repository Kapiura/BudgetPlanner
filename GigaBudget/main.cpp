#include "mainwindow.h"
#include <QApplication>
#include <databasemanager.h>
#include <userpanel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.userPanelLoginLoad();
    w.setDefaultPageIndex();
    w.show();
    qDebug() << "test";
    return a.exec();

}
