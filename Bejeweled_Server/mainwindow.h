#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 自定义消息处理器
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private slots:
    void on_clearButton_clicked(); // 清空 QTextEdit 内容的槽函数

private:
    Ui::MainWindow *ui;
    static QTextEdit *debugTextEdit; // 用于显示调试信息的 QTextEdit
};

#endif // MAINWINDOW_H
