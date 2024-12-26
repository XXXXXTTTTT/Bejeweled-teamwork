#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QScrollBar>
#include <QPalette>
#include <QPixmap>

// 初始化静态成员
QTextEdit *MainWindow::debugTextEdit = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置背景图片
    QPixmap background(":/image/resources/image/2.jpg"); // 替换为你的图片路径
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation); // 缩放图片以覆盖整个窗口
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);

    // 获取 QTextEdit 控件
    debugTextEdit = ui->debugTextEdit;

    // 安装自定义消息处理器
    qInstallMessageHandler(customMessageHandler);

    // 设置 QTextEdit 的滚动条始终可见
    debugTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // 设置 QTextEdit 的样式表
    debugTextEdit->setStyleSheet(
        "QTextEdit {"
        "   color: white;" // 文字颜色为白色
        "   background: transparent;" // 背景透明
        "   border: 1px solid rgba(255, 255, 255, 0.2);" // 边框为半透明白色
        "   border-radius: 5px;" // 圆角
        "   padding: 5px;" // 内边距
        "   selection-background-color: rgba(255, 255, 255, 0.3);" // 选中文本的背景颜色
        "   selection-color: white;" // 选中文本的文字颜色
        "}"
        );


    ui->clearButton->setStyleSheet(
        "QPushButton {"
        "   color: white;" // 文字颜色为白色
        "   background: transparent;" // 背景透明
        "   border: 1px solid rgba(255, 255, 255, 0.2);" // 边框为半透明白色
        "   border-radius: 5px;" // 圆角
        "   padding: 5px 10px;" // 内边距
        "}"
        "QPushButton:hover {"
        "   background: rgba(255, 255, 255, 0.1);" // 鼠标悬停时的背景颜色
        "}"
        "QPushButton:pressed {"
        "   background: rgba(255, 255, 255, 0.2);" // 按钮按下时的背景颜色
        "}"
        );

    // 连接按钮的点击事件
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 自定义消息处理器实现
void MainWindow::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);

    if (debugTextEdit) {
        // 将消息追加到 QTextEdit 中
        debugTextEdit->append(msg);

        // 自动滚动到底部
        QScrollBar *scrollBar = debugTextEdit->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }

    // 保留默认的 qDebug 输出到控制台
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "%s\n", localMsg.constData());
    fflush(stderr);

}

// 清空 QTextEdit 内容的槽函数
void MainWindow::on_clearButton_clicked()
{
    if (debugTextEdit) {
        debugTextEdit->clear(); // 清空内容
    }
}
