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

class Play : public QMainWindow
{
    Q_OBJECT

public:
    Play(QWidget *parent = nullptr);
    ~Play();

    static QMainWindow* getUI();                 // 动画制作提供 UI

    int weizhiX(int);
    int weizhiY(int);                                     // 析构函数

signals:
    void statusCursorCchanger(int c);            // 信号

private slots:
               //void onGemSelected();         // 选中宝石按钮
               //void onHintButtonClicked();   // 提示按钮
               //void onResetButtonClicked();  // 手动重置按钮
    void startButtonClicked();
    void updateButtonClicked();
    void updateziji();

private:
    Ui::Play *m_ui;                 // 指向 UI 类的指针，确保使用完整类型
    static Play* m_widget;
    QAction *m_hint;
    int m_score;
    bool m_begin = false;

    Board *m_board;
    int m_jewel[8][8] = {{0}};      // 匹配检测

    QTimer *timer; // 定时器，显示在lcdnumber上
    int count;      // 显示的数字
};
#endif // PLAY_H
