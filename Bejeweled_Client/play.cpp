#include "play.h"
#include "clientthread.h"
#include "music.h"
#include "ui_play.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QApplication>
#include <QRandomGenerator>
#include <QTimer>
using namespace std;
QGraphicsScene *scene_3;
float Play::m_soundVolume=0.5;
Play::Play(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::Play)
{

    this->setWindowTitle("宝石迷阵");
    //this->setWindowIcon(QIcon(":/new/prefix1/ICON/13369429051CA2411D99F227A90D19CB9BE4EA10C2.jpg"));
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

    m_score = 0;

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


    // 初始化计数器
    count = 0;
    // 创建定时器
    timer = new QTimer(this);
    // 连接定时器的timeout信号到updateLCD槽函数
    connect(timer, &QTimer::timeout, this, &Play::updateziji);
    // 设置定时器的更新时间间隔（比如 1000 毫秒，即每秒）
    timer->start(1000);  // 每1秒触发一次timeout信号
    m_mus = music::instance();
    // qDebug()<<m_ui->horizontalSlider->value();
    m_mus->m_audioOutput->setVolume(float(m_ui->horizontalSlider->value())/10000);
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

void Play::updateziji()
{
    count++;  // 增加数字
    m_ui->ziji->display(count); // 更新显示的数字
}

void Play::on_horizontalSlider_valueChanged(int value)
{
    m_mus->m_audioOutput->setVolume(float(m_ui->horizontalSlider->value())/10000);
}


void Play::on_horizontalSlider_2_sliderMoved(int position)
{
    m_soundVolume=float(position)/10000;
}
