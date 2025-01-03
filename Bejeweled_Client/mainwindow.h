#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent> // 添加 QResizeEvent 的头文件
#include <clientthread.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//主界面链接登入,主界面链接
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 声明 resizeEvent 方法
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

signals:
    void loginSuccess();

private:
    void onResultReceived(int res);
    Ui::MainWindow *m_ui;
    QPixmap m_backgroundImage; // 背景图片
    double m_aspectRatio;      // 背景图片的宽高比例
};

#endif // MAINWINDOW_H
