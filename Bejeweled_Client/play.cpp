#include "play.h"
#include "clientthread.h"
#include "information.h"
#include "menu.h"
#include "music.h"
#include "ui_play.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QApplication>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTimer>
#include<information.h>
using namespace std;
QGraphicsScene *scene_3;
float Play::m_soundVolume=0.5;
Play::Play(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::Play)
    , m_score(0)
    , remainingTime(60)
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


    m_hint = new QAction(this);
    m_hint->setText("提示");

    //m_score = 0;

    //测试己方宝石
    scene_3 = new QGraphicsScene(m_ui->graphicsView);
    scene_3->setSceneRect(0,67,830,620);
    m_ui->graphicsView->setScene(scene_3);

    // 创建 Board 对象，传递初始化的数组和场景
    m_board = new Board(ClientThread::m_ran, scene);
   // 设置 QGraphicsView 显示场景
    m_ui->graphicsView->setScene(scene);

    // 设置 QGraphicsView 的视图选项
    m_ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    m_ui->graphicsView->setRenderHint(QPainter::TextAntialiasing);
    m_ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);

    //connect(m_ui->start, &QPushButton::clicked, m_board, &Board::generateBoard);

    m_ui->lcdNumber->setDigitCount(5);
    m_ui->lcdNumber->display("01:00");

    //m_ui->ziji->display(m_score);

    m_timer = new QTimer(this);

    // 连接定时器信号到更新槽函数
    connect(m_timer, &QTimer::timeout, this, &Play::updateCountdown);

    // 界面加载后自动开始倒计时
    m_timer->start(1000);  // 每秒触发一次
    
    connect(m_ui->update, &QPushButton::clicked, m_board, &Board::updateBoard);
    connect(m_ui->hint, &QPushButton::clicked, m_board, &Board::giveHint);

    m_mus = music::instance();
    m_mus->start_random();


    m_mus->m_audioOutput->setVolume(float(m_ui->horizontalSlider->value())/10000);


    m_ui->label_3->setText(information::instance().m_userName+"'s score");
    m_ui->label_4->setText(information::instance().m_enemyName+"'s score");


    connect(m_board, &Board::scoreUpdated, this, &Play::updateScoreGUI);
    m_ui->ziji->display(0);  // 初始化得分为
    connect(&ClientThread::instance(), &ClientThread::scoreChanged, this, &Play::checkValue);
    this->setWindowTitle("WECOME PLAYER["+information::instance().m_userName+"]!");
}

Play::~Play()
{
    delete m_ui;
}


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

    QJsonObject json;
    json["type"] = "game";
    json["score"]=m_totalScore;
    qDebug()<<"clientThread.code= " <<ClientThread::instance().code;
    ClientThread::instance().sendMsg(json);
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
        QJsonObject json;
        json["type"] = "end";
        json["username"]=information::instance().m_userName;
        json["score"]=m_totalScore;

        ClientThread::instance().sendMsg(json);
        if(m_score > information::instance().m_enemyScore) {
            qDebug() << "m_score is :" << m_score;
            //qDebug() << "m_oppscore is :" << m_oppscore;
            music::instance()->sound("start.wav",m_soundVolume);
            QMessageBox::information(this, "游戏结束", "时间到了！你赢了！");
        }else if(m_score < information::instance().m_enemyScore) {
            qDebug() << "m_score is :" << m_score;
            //qDebug() << "m_oppscore is :" << m_oppscore;
            music::instance()->sound("fail.wav",m_soundVolume);
            QMessageBox::information(this, "游戏结束", "时间到了！你输了！");

        }else if(m_score == information::instance().m_enemyScore) {
            qDebug() << "m_score is :" << m_score;
            //qDebug() << "m_oppscore is :" << m_oppscore;
            music::instance()->sound("start.wav",m_soundVolume);
            QMessageBox::information(this, "游戏结束", "时间到了！平局！");
        }
        // music::instance()->m_audioOutput->setVolume(0);
        music::instance()->m_mediaPlayer->stop();
        if(information::instance().m_RRange==8)
        {
            QJsonObject json0;
            json0["type"] = "LogIn";
            json0["name"] = information::instance().m_userName;
            json0["password"] =information::instance().m_password;
            ClientThread::instance().sendMsg(json0);
        }
        Menu *menu=new Menu();
        menu->show();
        this->close();
    }
}
void Play::checkValue() {
    // 定期检查 value 的变化
    m_ui->lcdNumber_2->display(information::instance().m_enemyScore);
}
