#ifndef MUSIC_H
#define MUSIC_H
#include "qapplication.h"
#include "qwidget.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <iostream>

//音乐播放类
class music
{

public:
    static music * instance();
    QAudioOutput *m_audioOutput;
    QMediaPlayer *m_mediaPlayer;

    void sound(QString string, float volume);
    void stop();
    void start_random();
    ~music();
private:
    explicit music();
    static music * m_music;
};
#endif // MUSIC_H
