#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent> // 添加 QResizeEvent 的头文件

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
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
    Ui::MainWindow *m_ui;
    QPixmap m_backgroundImage; // 背景图片
    double m_aspectRatio;      // 背景图片的宽高比例
};

#endif // MAINWINDOW_H
