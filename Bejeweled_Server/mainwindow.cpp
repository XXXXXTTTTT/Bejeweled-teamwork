#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QScrollBar>
#include <QPalette>
#include <QPixmap>
#include <QMutex>
#include <QDateTime>

QTextEdit *MainWindow::debugTextEdit = nullptr;
QMutex MainWindow::mutex;
void myMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    Q_UNUSED(type);
    Q_UNUSED(context);

    mutex.lock(); // 锁

    // 写入
    MainWindow::setText(msg);
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "%s\n", localMsg.constData());
    fflush(stderr);

    mutex.unlock();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置背景
    QPixmap background(":/image/resources/image/2.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, background);
    this->setPalette(palette);

    // 获取 QTextEdit 控件
    debugTextEdit = ui->debugTextEdit;
    qInstallMessageHandler(myMsgOutput);

    // 设置样式表
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

    // 设置按钮的样式表
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
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setText(const QString &msg)
{
    QMutexLocker locker(&mutex); // 加锁，确保线程安全

    if (debugTextEdit) {
        // 消息追加
        debugTextEdit->append(msg);
        QScrollBar *scrollBar = debugTextEdit->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}
void MainWindow::on_clearButton_clicked()
{
    if (debugTextEdit) {
        debugTextEdit->clear(); // 清空内容
    }
}
