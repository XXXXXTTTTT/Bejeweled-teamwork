

// #include "mainwindow.h"
#include "clientThread.h"
#include "play.h"
#include "mainwindow.h"
#include "menu.h"
#include "music.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <thread>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Bejeweled 客户端");
    parser.addHelpOption();
    QCommandLineOption hostOption("host", "服务器 IPv4 地址或主机名。", "host", "127.0.0.1");
    QCommandLineOption portOption("port", "服务器 TCP 端口。", "port", "12345");
    parser.addOption(hostOption);
    parser.addOption(portOption);
    parser.process(a);

    bool portOk = false;
    const quint16 port = parser.value(portOption).toUShort(&portOk);
    const QString host = parser.value(hostOption).trimmed();
    if (!portOk || port == 0 || host.isEmpty()) {
        qCritical() << "无效的服务器地址或端口。";
        return 1;
    }


    if (!ClientThread::configure(host, port)) {
        qCritical() << "客户端已启动，不能修改服务器地址。";
        return 1;
    }

    ClientThread::instance().start();

    MainWindow loginWindow=MainWindow();

    loginWindow.show();

    //测试游戏页面


    // Play w;

    // w.show();

    return a.exec();
}
