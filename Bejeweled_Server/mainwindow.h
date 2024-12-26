#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMutex>

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
    static void setText(const QString &msg);

private slots:
    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    static QTextEdit *debugTextEdit;
    static QMutex mutex;
};

#endif // MAINWINDOW_H
