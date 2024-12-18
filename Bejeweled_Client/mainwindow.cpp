#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>

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

    // 初始化数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");
    if (!db.open()) {
        QMessageBox::critical(this, "错误", "无法连接到数据库");
    }

    // 创建用户表
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT)");

    // 手动连接槽函数
    connect(m_ui->loginButton, &QPushButton::clicked, this, &MainWindow::on_loginButton_clicked);
    connect(m_ui->registerButton, &QPushButton::clicked, this, &MainWindow::on_registerButton_clicked);
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

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        QMessageBox::information(this, "成功", "登录成功");
        emit loginSuccess();
        this->close();
    } else {
        QMessageBox::warning(this, "错误", "用户名或密码错误");
    }
}

void MainWindow::on_registerButton_clicked()
{
    Dialog *dialog = new Dialog(this);
    dialog->show();
}
