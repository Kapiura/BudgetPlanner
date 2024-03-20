#include "include/mainwindow.h"

#include <QApplication>
#include <include/databasemanager.h>
#include <include/userpanel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.userPanelLoad();
    return a.exec();
}
