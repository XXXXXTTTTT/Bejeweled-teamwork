#include "play.h"
#include "ui_play.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QApplication>
using namespace std;
QGraphicsScene *scene_3;
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
    m_ui->label_3->setText("时间限制:1分钟");

    //测试己方宝石
    scene_3 = new QGraphicsScene(m_ui->graphicsView);
    scene_3->setSceneRect(0,67,830,620);
    m_ui->graphicsView->setScene(scene_3);

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
    m_board = new Board(initialBoard, scene);

    // 设置 QGraphicsView 显示场景
    m_ui->graphicsView->setScene(scene);

    // 设置 QGraphicsView 的视图选项
    m_ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    m_ui->graphicsView->setRenderHint(QPainter::TextAntialiasing);
    m_ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);

    connect(m_ui->start, &QPushButton::clicked, m_board, &Board::generateBoard);

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
