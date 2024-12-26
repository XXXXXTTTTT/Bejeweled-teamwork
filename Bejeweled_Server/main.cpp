#include "mainwindow.h"
#include "server.h"
#include "sql.h"
#include <QDir>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;






    Server server;
    server.startServer(12345);


    // w.show();
    return a.exec();
}
