#include "music.h"
#include "qapplication.h"
#include "qwidget.h"
#include<mainwindow.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <iostream>
#include <QDir>
#include <QRandomGenerator>
// 音源位置:
// Bejeweled-teamwork\Bejeweled_Client\build\Desktop_Qt_6_5_3_MinGW_64_bit-Debug\debug\sound
// 背景音乐启动方式：构造music();
// 背景音乐停止函数：stop();
// 音效启动函数:例:sound("click.wav");
music * music:: m_music = nullptr;
music::music()
{

}
music*music::instance()
{
    if (!m_music) {
        m_music = new music();
    }
    return m_music;
}
void music::sound(QString string,float volume)
{
    auto m_audioOutput = new QAudioOutput();
    auto m_mediaPlayer = new QMediaPlayer();

    // 设置音频输出
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(volume);

    // 获取当前应用程序的目录

    QString audioFilePath = "qrc:/music/resources/sound/"+ string;

    // 设置音乐文件路径
    m_mediaPlayer->setSource(QUrl(audioFilePath));

    // 播放音乐
    m_mediaPlayer->play();
}
void music::start_random()
{
    m_audioOutput = new QAudioOutput();
    m_mediaPlayer = new QMediaPlayer();
    QString string=QString::number( QRandomGenerator::global()->bounded(1, 5))+".mp3" ;
    QString audioFilePath = "qrc:/music/resources/sound/"+ string;

    // 设置音频输出
    m_mediaPlayer->setAudioOutput(m_audioOutput);

    // 设置音乐文件路径
    qDebug() << "Resource URL: " << QUrl("qrc:/music/resources/sound/"+string).toString();
    m_mediaPlayer->setSource(QUrl(audioFilePath));



    // 播放音乐
    m_mediaPlayer->play();

    // 监听播放状态变化，检测播放完毕
    MainWindow::connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status) {
        // 如果播放完成，重置播放进度并重新开始播放
        if (status == QMediaPlayer::EndOfMedia) {
            m_mediaPlayer->setPosition(0);  // 重置播放进度
            m_mediaPlayer->play();          // 重新开始播放
        }
    });
}
void music::stop()
{

}
music:: ~music()
{
}

