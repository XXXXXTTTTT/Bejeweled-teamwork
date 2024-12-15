#ifndef MUSIC_H
#define MUSIC_H
#include "qapplication.h"
#include "qwidget.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <iostream>
class music
{
public:
    QAudioOutput *m_audioOutput;
    QMediaPlayer *m_mediaPlayer;

    music();
    void sound(QString string);
    void stop();
    ~music();
};
#endif // MUSIC_H
