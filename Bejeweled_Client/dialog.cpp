#include "dialog.h"
#include "ui_dialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include <QResizeEvent>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Dialog) // 初始化成员变量
{
    m_ui->setupUi(this);
    this->setWindowTitle("注册");

    // 加载背景图片
    m_backgroundImage = QPixmap(":/images/resources/2.jpg");

    // 计算背景图片的宽高比例
    m_aspectRatio = static_cast<double>(m_backgroundImage.width()) / m_backgroundImage.height();

    // 设置背景图片
    QPalette palette;
    palette.setBrush(QPalette::Window, m_backgroundImage);
    this->setPalette(palette);
}

Dialog::~Dialog()
{
    delete m_ui;
}

void Dialog::resizeEvent(QResizeEvent *event)
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

    QDialog::resizeEvent(event); // 调用基类的 resizeEvent
}

void Dialog::on_buttonBox_accepted()
{
    QString username = m_ui->username->text();
    QString password = m_ui->password->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名或密码不能为空");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "注册成功");
        this->close();
    } else {
        QMessageBox::warning(this, "错误", "注册失败");
    }
}
