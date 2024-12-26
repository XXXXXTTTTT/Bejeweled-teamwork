#include "mainwindow.h"
#include "server.h"
#include "sql.h"
#include <QDir>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //有bug弃用

    // MainWindow w;


    // w.show();



    Server server;



    server.startServer(12345);



    return a.exec();
}
