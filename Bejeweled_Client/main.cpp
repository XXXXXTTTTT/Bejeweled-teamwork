#include "mainwindow.h"
#include "clientThread.h"
#include <QApplication>
#include <music.h>
#include <thread>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    ClientThread clientThread("127.0.0.1", 12345);
    clientThread.start();

    w.show();



    //测试代码

    //bgm播放
    music m=music();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("start_timi.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("click.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("match.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("combo_2.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("combo_3.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("combo_4.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("combo_5.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("combo_6.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("combo_7.wav");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("great.mp3");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("unbelievable.mp3");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    //音效播放
    m.sound("fail.wav");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    //停止
    // m.stop();

        return a.exec();
}
