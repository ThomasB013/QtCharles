#include "mainwindow.h"

#include <QApplication>

MainWindow* mainwindow; // Make mainwindow accessible for commands.h (code smell?)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    mainwindow = &w;
    w.show();
    return a.exec();
}
