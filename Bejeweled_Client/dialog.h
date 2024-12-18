#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QResizeEvent> // 添加 QResizeEvent 的头文件

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected:
    // 声明 resizeEvent 方法
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *m_ui;
    QPixmap m_backgroundImage; // 背景图片
    double m_aspectRatio;      // 背景图片的宽高比例
};

#endif // DIALOG_H
