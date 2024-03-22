#include "mainwindow.h"

#include <QApplication>
#include <databasemanager.h>
#include <userpanel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setDefaultPageIndex();

    // switch(w.returnPageIndex())
    // {
    // case 0:

    //     qDebug() << "showing userpanel login page ";
    //     break;
    // case 1:

    //     qDebug() << "Showing userpanel ";
    //     break;
    // default:
    //     qDebug() << "Nothing else to show sowwy ;3";
    //     break;
    // }
    w.userPanelLoginLoad();
    w.userPanelLoad();
    w.show();
    return a.exec();
}
