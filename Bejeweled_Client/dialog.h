#ifndef DIALOG_H
#define DIALOG_H

#include "clientThread.h"
#include <QDialog>
#include <QResizeEvent>

namespace Ui {
class Dialog;
}

//注册登入
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_registerButton_clicked(); // 处理“注册”按钮点击事件
    void on_cancelButton_clicked();   // 处理“取消”按钮点击事件
    void onResultReceived(int res);

private:
    Ui::Dialog *m_ui;
    QPixmap m_backgroundImage;
    double m_aspectRatio;
};

#endif // DIALOG_H
