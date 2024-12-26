

// #include "mainwindow.h"
#include "clientThread.h"
#include "play.h"
#include "mainwindow.h"
#include "menu.h"
#include "music.h"
#include <QApplication>
#include <thread>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    ClientThread::instance().start();
    qDebug() << "客户端已启动";

    MainWindow loginWindow=MainWindow();

    loginWindow.show();

    //测试游戏页面


    // Play w;

    // w.show();

    return a.exec();
}
