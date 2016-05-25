#include "mainwindow.h"
#include"database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    if(!createConnection())
    {
        return 1;
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
