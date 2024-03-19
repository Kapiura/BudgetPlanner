#include "mainwindow.h"

#include <QApplication>
#include <databasemanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DatabaseManager dbHandler("gigaBudget","root","!@#QWE123qwe","localhost",3306);
    w.show();
    return a.exec();
}
