#include "mainwindow.h"
#include <QApplication>
#include <databasemanager.h>
#include <userpanel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (w.returnSuccess() == false)
        w.userPanelLoginLoad();
    w.setDefaultPageIndex();
    w.addingCategoriesItems();
    w.show();
    qDebug() << "test";
    return a.exec();

}
