#include "mainwindow.h"

#include <QApplication>
#include <databasemanager.h>
#include <userpanel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create main window
    MainWindow w;
    w.show(); // Show main window

    // Call userPanelLoad after main window is shown
    w.userPanelLoad();

    return a.exec(); // Start event loop
}
