#include "mainwindow.h"
#include "menu.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include <information.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow) // 初始化成员变量
{
    m_ui->setupUi(this);
    this->setWindowTitle("登录");

    // 加载背景图片
    m_backgroundImage = QPixmap(":/images/resources/1.jpg");

    // 计算背景图片的宽高比例
    m_aspectRatio = static_cast<double>(m_backgroundImage.width()) / m_backgroundImage.height();

    // 设置背景图片
    QPalette palette;
    palette.setBrush(QPalette::Window, m_backgroundImage);
    this->setPalette(palette);

    // // 初始化数据库
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // db.setDatabaseName("users.db");
    // if (!db.open()) {
    //     QMessageBox::critical(this, "错误", "无法连接到数据库");
    // }

    // // 创建用户表
    // QSqlQuery query;
    // query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT)");

    // 手动连接槽函数
    // connect(m_ui->loginButton, &QPushButton::clicked, this, &MainWindow::on_loginButton_clicked);
    // connect(m_ui->registerButton, &QPushButton::clicked, this, &MainWindow::on_registerButton_clicked);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
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

    QMainWindow::resizeEvent(event); // 调用基类的 resizeEvent
}

void MainWindow::on_loginButton_clicked()
{
    QString username = m_ui->username->text();
    QString password = m_ui->password->text();


    QJsonObject json;
    json["type"] = "LogIn";
    json["name"] = username;
    json["password"] = password;
    information::instance().m_userName=username;
    ClientThread*clientThread=ClientThread::instance();
    clientThread->sendMsg(json);

    connect(clientThread, &ClientThread::resultReceived, this, &MainWindow::onResultReceived);

}

void MainWindow::on_registerButton_clicked()
{
    Dialog *dialog = new Dialog(this);
    dialog->show();
}
void MainWindow::onResultReceived(int res)
{
    disconnect(ClientThread::instance(), &ClientThread::resultReceived, this, &MainWindow::onResultReceived);

    // 处理服务器返回的结果，接收到信号后退出事件循环
    ClientThread::instance()->m_res = res;
    // 根据 m_res 判断注册是否成功
    if (ClientThread::instance()->m_res == 1) {
        QMessageBox::information(this, "登录成功", "登录成功");
        Menu *menu=new Menu();
        menu->show();
        this->close();
    } else if (ClientThread::instance()->m_res == 0) {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
    else
    {
        QMessageBox::warning(this, "登录失败", "查询失败");
    }
}
