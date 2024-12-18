#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QResizeEvent> // 添加 QResizeEvent 的头文件

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

protected:
    // 声明 resizeEvent 方法
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_startGameButton_clicked();
    void on_seQuenceButton_clicked();

private:
    Ui::Menu *m_ui;
    QPixmap m_backgroundImage; // 背景图片
    double m_aspectRatio;      // 背景图片的宽高比例
};

#endif // MENU_H
