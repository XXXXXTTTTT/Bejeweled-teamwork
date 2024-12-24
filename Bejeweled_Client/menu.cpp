#include "menu.h"
#include "clientthread.h"
#include "play.h"
#include "qjsonobject.h"
#include "ui_menu.h"
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include <QResizeEvent>
#include <information.h>

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Menu) // 初始化成员变量
{
    m_ui->setupUi(this);
    this->setWindowTitle("主界面");

    // 加载背景图片
    m_backgroundImage = QPixmap(":/images/resources/1.jpg");

    // 计算背景图片的宽高比例
    m_aspectRatio = static_cast<double>(m_backgroundImage.width()) / m_backgroundImage.height();

    // 设置背景图片
    QPalette palette;
    palette.setBrush(QPalette::Window, m_backgroundImage);
    this->setPalette(palette);

    // 连接按钮到槽函数
    // connect(m_ui->startGameButton, &QPushButton::clicked, this, &Menu::on_startGameButton_clicked);
    // connect(m_ui->startGameButton, &QPushButton::clicked, this, &Menu::on_seQuenceButton_clicked);
}

Menu::~Menu()
{
    delete m_ui;
}

void Menu::resizeEvent(QResizeEvent *event)
{
    // 在窗口大小变化时保持窗口比例与背景图片比例相同
    int newWidth = event->size().width();
    int newHeight = static_cast<int>(newWidth / m_aspectRatio);

    // 如果新高度超过窗口高度，则调整宽度
    if (newHeight > event->size().height()) {
        newHeight = event->size().height();
        newWidth = static_cast<int>(newHeight * m_aspectRatio);
    }

    // 调整窗口尺寸
    this->resize(newWidth, newHeight);

    // 重新设置背景图片
    QPixmap scaledBackground = m_backgroundImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, scaledBackground);
    this->setPalette(palette);

    QWidget::resizeEvent(event); // 调用基类的 resizeEvent
}

void Menu::on_startGameButton_clicked()
{
    // 匹配对手
    QMessageBox::information(this, "匹配中", "等待对手加入");

    QJsonObject json;
    json["type"] = "Match";
    ClientThread*clientThread=ClientThread::instance();
    clientThread->sendMsg(json);

    connect(clientThread, &ClientThread::matchReceived, this, &Menu::onResultReceived);
}

void Menu::on_seQuenceButton_clicked()
{}
void Menu::onResultReceived(QString enemyId)
{
    disconnect(ClientThread::instance(), &ClientThread::matchReceived, this, &Menu::onResultReceived);
    // 处理服务器返回的结果

        QMessageBox::information(this, "匹配成功", "对手："+enemyId);
        information::instance().m_enemyName=enemyId;
        Play * play = new Play();
        play -> show();
        this->close();

}
