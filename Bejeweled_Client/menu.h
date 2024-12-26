#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QResizeEvent>
#include <QMovie>

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
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_startGameButton_clicked();
    void on_seQuenceButton_clicked();
    void onGifFinished(); // 处理初始 GIF 动画完成信号
    void onStartGameGifFinished(); // 处理开始游戏 GIF 动画完成信号

    void on_seQuenceButton_2_clicked();

    void on_radioButton_2_toggled(bool checked);

    void on_radioButton_toggled(bool checked);

    void on_num_valueChanged(int arg1);

private:
    void onResultReceived(QString enemyId); // 处理匹配结果
    void hideUiComponents(); // 隐藏 UI 组件
    void showUiComponents(); // 显示 UI 组件

    Ui::Menu *m_ui;
    QMovie *m_movie;           // 初始 GIF 动画
    QMovie *m_startGameMovie;  // 开始游戏 GIF 动画
};

#endif // MENU_H
