#include "mainwindow.h"

#include <QApplication>
#include <databasemanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DatabaseManager* dbHandler = new DatabaseManager("gigaBudget","root","!@#QWE123qwe","localhost",3306);
    w.show();
    delete dbHandler;
    return a.exec();
}
