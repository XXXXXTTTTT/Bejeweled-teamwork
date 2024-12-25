#include "play.h"
#include "clientthread.h"
#include "information.h"
#include "music.h"
#include "ui_play.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QApplication>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTimer>
using namespace std;
QGraphicsScene *scene_3;
float Play::m_soundVolume=0.5;
Play::Play(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::Play)
    , remainingTime(60)
    , m_score(0)
    , m_oppscore(0)
{
    this->setWindowTitle("宝石迷阵");
    m_ui->setupUi(this);


    QGraphicsView *view = m_ui->graphicsView;

    // 移除边框和滚动条
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);

    // 设置背景样式表
    view->setStyleSheet("QGraphicsView {"
                        "background-image: url(:/images/media/media/board.png);"
                        "background-position: center;"
                        "background-repeat: no-repeat;"
                        //"background-size: contain;"  // 适应视图大小
                        "}");

    // 动态设置场景（可选）
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, view->width(), view->height());
    view->setScene(scene);


    //auto layout = new QHBoxLayout(this);
    //QMenuBar *menubar = new QMenuBar(this);
    //layout->setMenuBar(menubar);

    m_hint = new QAction(this);
    m_hint->setText("提示");

    //m_score = 0;

    //测试己方宝石
    scene_3 = new QGraphicsScene(m_ui->graphicsView);
    scene_3->setSceneRect(0,67,830,620);
    m_ui->graphicsView->setScene(scene_3);

    //to do后序需要网络生成一个然后传使得双方初始棋盘内容一致
    int initialBoard[8][8] = {
        {1, 2, 3, 4, 5, 6, 7, 1},
        {2, 3, 4, 5, 6, 7, 1, 2},
        {3, 4, 5, 6, 7, 1, 2, 3},
        {4, 5, 6, 7, 1, 2, 3, 4},
        {5, 6, 7, 1, 2, 3, 4, 5},
        {6, 7, 1, 2, 3, 4, 5, 6},
        {7, 1, 2, 3, 4, 5, 6, 7},
        {1, 2, 3, 4, 5, 6, 7, 1}
    };

    // 创建 Board 对象，传递初始化的数组和场景
    m_board = new Board(ClientThread::m_ran, scene);
   // 设置 QGraphicsView 显示场景
    m_ui->graphicsView->setScene(scene);

    // 设置 QGraphicsView 的视图选项
    m_ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    m_ui->graphicsView->setRenderHint(QPainter::TextAntialiasing);
    m_ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);

    //connect(m_ui->start, &QPushButton::clicked, m_board, &Board::generateBoard);
    connect(m_ui->pushButton, &QPushButton::clicked, m_board, &Board::updateBoard);

    m_ui->lcdNumber->setDigitCount(5);
    m_ui->lcdNumber->display("00:10");

    //m_ui->ziji->display(m_score);

    m_timer = new QTimer(this);

    // 连接定时器信号到更新槽函数
    connect(m_timer, &QTimer::timeout, this, &Play::updateCountdown);

    // 界面加载后自动开始倒计时
    m_timer->start(1000);  // 每秒触发一次

    // 初始化计数器
    //count = 0;
    // 创建定时器
    //timer = new QTimer(this);
    // 连接定时器的timeout信号到updateLCD槽函数
    //connect(timer, &QTimer::timeout, this, &Play::updateziji);
    // 设置定时器的更新时间间隔（比如 1000 毫秒，即每秒）
    // timer->start(1000);  // 每1秒触发一次timeout信号
    m_mus = music::instance();
    // qDebug()<<m_ui->horizontalSlider->value();
    m_mus->m_audioOutput->setVolume(float(m_ui->horizontalSlider->value())/10000);
    m_ui->label_3->setText(information::instance().m_userName+"'s score");
    m_ui->label_4->setText(information::instance().m_enemyName+"'s score");
    //timer->start(1000);  // 每1秒触发一次timeout信号
    connect(m_board, &Board::scoreUpdated, this, &Play::updateScoreGUI);
    m_ui->ziji->display(0);  // 初始化得分为

}

Play::~Play()
{
    delete m_ui;
}

void Play::startButtonClicked()
{
    // 处理点击事件，例如调用 board 的生成宝石方法
    m_board->generateBoard();
}

void Play::updateButtonClicked() {
    // 触发 Board 类的 updateBoard 方法，更新棋盘
    m_board->updateBoard();
}

// void Play::updateziji()
// {
//     count++;  // 增加数字
//     m_ui->ziji->display(count); // 更新显示的数字
// }

void Play::on_horizontalSlider_valueChanged(int value)
{
    m_mus->m_audioOutput->setVolume(float(m_ui->horizontalSlider->value())/10000);
}

void Play::on_horizontalSlider_2_sliderMoved(int position)
{
    m_soundVolume=float(position)/10000;


}
void Play::updateScoreGUI(int score) {
    qDebug() << "score is :" << score;
    m_totalScore += score;  // 累加得分到总得分
    m_score = m_totalScore;
    m_ui->ziji->display(m_totalScore);  // 更新 LCD 显示器上的总得分
}

void Play::updateCountdown() {
    if (remainingTime > 0) {
        --remainingTime;

        // 格式化时间为 mm:ss
        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;
        QString timeText = QString("%1:%2")
                               .arg(minutes, 2, 10, QChar('0'))
                               .arg(seconds, 2, 10, QChar('0'));
        m_ui->lcdNumber->display(timeText);
    } else {
        m_timer->stop();
        m_ui->lcdNumber->display("00:00");

        checkGameOver();
    }
}

void Play::checkGameOver(){
    if (remainingTime <= 0) {
        if(m_score > m_oppscore) {
            qDebug() << "m_score is :" << m_score;
            //qDebug() << "m_oppscore is :" << m_oppscore;
            QMessageBox::information(this, "游戏结束", "时间到了！你赢了！");
            return;
        }else if(m_score < m_oppscore) {
            qDebug() << "m_score is :" << m_score;
            //qDebug() << "m_oppscore is :" << m_oppscore;
            QMessageBox::information(this, "游戏结束", "时间到了！你输了！");
            return;
        }else if(m_score == m_oppscore) {
            qDebug() << "m_score is :" << m_score;
            //qDebug() << "m_oppscore is :" << m_oppscore;
            QMessageBox::information(this, "游戏结束", "时间到了！平局！");
            return;
        }
    }
}
