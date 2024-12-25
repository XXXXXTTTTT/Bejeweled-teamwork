

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

    //测试代码
    
    // //bgm播放
    // music m=music();
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // // //音效播放
    // m.sound("start_timi.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // // //音效播放
    // m.sound("click.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("match.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("combo_2.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("combo_3.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("combo_4.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("combo_5.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("combo_6.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("combo_7.wav");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("great.mp3");

    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("unbelievable.mp3");

    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // //音效播放
    // m.sound("fail.wav");

    // std::this_thread::sleep_for(std::chrono::seconds(1));

    //停止
    // m.stop();

    //测试Client通信线程部分

    
    // ClientThread clientThread("127.0.0.1", 12345);
    // clientThread.start();

    
    //测试登入注册页面

    // ClientThread* clientThread =ClientThread::instance();
    // clientThread->start();
    // qDebug() << "客户端已启动";

    // MainWindow loginWindow=MainWindow();
    // Menu mainMenu;

    // QObject::connect(&loginWindow, &MainWindow::loginSuccess, &mainMenu, &Menu::show);

    // loginWindow.show();

    //测试游戏页面
    Play w;

    w.show();

    return a.exec();
}
