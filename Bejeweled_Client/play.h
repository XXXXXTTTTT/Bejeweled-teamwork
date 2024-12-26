#ifndef PLAY_H
#define PLAY_H

#include <QMainWindow>
#include "Jewel.h"
#include "board.h"
QT_BEGIN_NAMESPACE

namespace Ui {
class Play;
}
QT_END_NAMESPACE

//游戏游玩界面
class Play : public QMainWindow
{
    Q_OBJECT

public:
    Play(QWidget *parent = nullptr);
    ~Play();// 析构函数



    static QMainWindow* getUI();                 // 动画制作提供 UI
    
    int weizhiX(int);
    int weizhiY(int);


    void statusCursorCchanger(int c);            // 信号

private slots:
    void updateScoreGUI(int score);    //将更新的得分输出在ui
    void updateCountdown();  // 更新倒计时
    void checkGameOver();    // 检查游戏结束

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_sliderMoved(int position);

    void checkValue() ;
public:
    music* m_mus;
    static float m_soundVolume;
private:
    Ui::Play *m_ui;                 // 指向 UI 类的指针，确保使用完整类型
    static Play* m_widget;
    QAction *m_hint;

    int m_score;                    //自己得分
    int m_totalScore = 0;  // 总得分
    bool m_begin = false;

    QVector<QVector<int>> m_board0; // 游戏网格
    Board *m_board;
    int m_jewel[8][8] = {{0}};      // 匹配检测

    QTimer *m_timer;       // 定时器
    int remainingTime;   // 剩余时间（秒）
};
#endif // PLAY_H
