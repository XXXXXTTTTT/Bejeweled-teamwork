#include "menu.h"
#include "clientthread.h"
#include "play.h"
#include "qjsonobject.h"
#include "ui_menu.h"
#include <QMessageBox>
#include <QPixmap>
#include <QResizeEvent>
#include <QMovie>
#include <QLabel>
#include <QPainter>
#include <music.h>

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Menu),
    m_movie(nullptr),
    m_startGameMovie(nullptr)
{
    m_ui->setupUi(this);
    this->setWindowTitle("主界面");

    // 获取 Play 窗口的大小
    QSize playSize = QSize(828, 761); // Play 窗口的大小
    this->resize(playSize); // 设置 Menu 窗口的大小与 Play 窗口一致

    // 加载初始 GIF 图片
    m_movie = new QMovie(":/images/resources/12.gif");
    if (m_movie->isValid()) {
        m_movie->setCacheMode(QMovie::CacheAll); // 缓存所有帧，避免频繁加载
        connect(m_movie, &QMovie::frameChanged, this, [this]() {
            update(); // 每次帧变化时更新窗口
        });
        connect(m_movie, &QMovie::finished, this, &Menu::onGifFinished); // 连接 GIF 动画完成信号
        m_movie->start(); // 启动 GIF 动画
    } else {
        qWarning() << "Failed to load initial GIF";
    }

    // 隐藏所有 UI 组件，直到初始 GIF 动画完成
    hideUiComponents();


    this->setWindowTitle("WECOME PLAYER["+information::instance().m_userName+"]!");

    // 初始化顶部图片标签
    m_ui->topImageLabel->hide(); // 先隐藏顶部图片标签

}

Menu::~Menu()
{
    delete m_ui;
    if (m_movie) {
        m_movie->stop();
        delete m_movie;
    }
    if (m_startGameMovie) {
        m_startGameMovie->stop();
        delete m_startGameMovie;
    }
}

void Menu::resizeEvent(QResizeEvent *event)
{
    // 在窗口大小变化时保持窗口比例与背景图片比例相同
    int newWidth = event->size().width();
    int newHeight = event->size().height();

    // 调整窗口尺寸
    this->resize(newWidth, newHeight);

    QWidget::resizeEvent(event);
}

void Menu::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // 绘制初始 GIF 动画
    if (m_movie && m_movie->isValid()) {
        QPainter painter(this);
        QPixmap frame = m_movie->currentPixmap();
        // 等比例缩放 GIF 动画，填充整个窗口
        QPixmap scaledFrame = frame.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, scaledFrame);
    }
}

void Menu::onGifFinished()
{
    // 初始 GIF 动画完成一次循环后，显示所有 UI 组件
    showUiComponents();

    // 加载并显示顶部图片
    m_ui->topImageLabel->setPixmap(QPixmap(":/images/resources/10.png").scaled(m_ui->topImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_ui->topImageLabel->show();
}

void Menu::on_startGameButton_clicked()
{
    music::instance()->sound("click.wav",1);
    hideUiComponents();
    m_ui->topImageLabel->hide();

    // 加载开始游戏 GIF 动画
    m_startGameMovie = new QMovie(":/images/resources/11.gif"); // 替换为你的开始游戏 GIF 路径
    if (m_startGameMovie->isValid()) {
        m_startGameMovie->setCacheMode(QMovie::CacheAll);
        connect(m_startGameMovie, &QMovie::frameChanged, this, [this]() {
            update();
        });
        connect(m_startGameMovie, &QMovie::finished, this, &Menu::onStartGameGifFinished);
        m_startGameMovie->start();

        // 调整 GIF 的大小以适应 topImageLabel
        QSize labelSize = m_ui->topImageLabel->size();
        m_startGameMovie->setScaledSize(labelSize); // 设置 GIF 的缩放大小

        // 显示 GIF 在 topImageLabel 的位置
        m_ui->topImageLabel->setMovie(m_startGameMovie);
        m_ui->topImageLabel->show(); // 显示 topImageLabel
    } else {
        qWarning() << "Failed to load start game GIF";
    }


    // 匹配对手
    QMessageBox::information(this, "匹配中", "等待对手加入");

    QJsonObject json;
    json["type"] = "Match";
    ClientThread::instance().sendMsg(json);


    connect(&ClientThread::instance(), &ClientThread::matchReceived, this, &Menu::onResultReceived);

    // music::instance()->stop();
}

void Menu::on_seQuenceButton_clicked()
{
    music::instance()->sound("click.wav",1);
    // 排行榜按钮点击事件
    QMessageBox::information(this, "排行榜", "排行榜功能暂未实现");
}

void Menu::onResultReceived(QString enemyId)
{
    disconnect(&ClientThread::instance(), &ClientThread::matchReceived, this, &Menu::onResultReceived);
    // 处理服务器返回的结果


        // QMessageBox::information(this, "匹配成功", "对手："+enemyId);
        information::instance().m_enemyName=enemyId;
        Play * play = new Play();
        play -> show();
        this->close();
}

void Menu::onStartGameGifFinished()
{
    // 开始游戏 GIF 动画完成后，跳转到游戏界面

    Play *play = new Play();
    play->show();
    this->close();

}

void Menu::hideUiComponents()
{
    m_ui->startGameButton->hide();
    m_ui->seQuenceButton->hide();
    m_ui->seQuenceButton_2->hide();
    m_ui->radioButton->hide();
    m_ui->radioButton_2->hide();
    m_ui->label_2->hide();
}

void Menu::showUiComponents()
{
    m_ui->startGameButton->show();
    m_ui->seQuenceButton->show();
    m_ui->seQuenceButton_2->show();
    m_ui->radioButton->show();
    m_ui->radioButton_2->show();
    m_ui->label_2->show();
}




void Menu::on_seQuenceButton_2_clicked()
{
    music::instance()->sound("click.wav",1);
    this->close();
}





void Menu::on_radioButton_2_toggled(bool checked)
{
    music::instance()->sound("click.wav",1);
    if(checked)
    {
        information::instance().m_RRange=8;
    }
    qDebug()<<information::instance().m_RRange;
}


void Menu::on_radioButton_toggled(bool checked)
{
    music::instance()->sound("click.wav",1);
    if(checked)
    {
        information::instance().m_RRange=6;
    }
    qDebug()<<information::instance().m_RRange;

}

