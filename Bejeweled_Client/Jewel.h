#ifndef JEWEL_H
#define JEWEL_H
#include<QString>
#include<QLabel>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QDrag>
#include<QDebug>
#include<QMovie>
#include<QApplication>
#include<QPushButton>
#include <QGraphicsItem>
//宝石类的声明
class Jewel :  public QObject,public QGraphicsItem {
    Q_OBJECT
private:
    //QString color;
    int m_type;     //宝石的种类
    int m_x;        //当前宝石的x坐标
    int m_y;        //当前宝石的y坐标
    int m_where;
    bool m_choosed; // 是否被选中
    QPixmap m_pixmap; // 宝石图像
    QMovie *m_movie;  // 用于显示gif动图
public:

    //重写的时候默认使用button的构造器初始化 gxc
    Jewel(int x , int y , int type,int where ,QGraphicsItem *parent = nullptr);      //初始化宝石的坐标和对应的QLabel的生成以及图片的加载
    ~Jewel();                           //析构函数                   //获取当前宝石的button
    int getX(){return m_x;}
    int getY(){return m_y;}

    void setIconForGem(int type);
    void ShowJewel(int a[8][8]);

    // QGraphicsItem 的纯虚函数：返回项目的边界矩形
    QRectF boundingRect() const override;

    // 自定义绘制函数，用于绘制宝石
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // 鼠标点击事件处理函数
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void setButtonIcon(){setIconForGem(m_type);}
    //void sendPosInfo_onclicked();

signals:
    void jewelClicked(int x, int y); // 声明信号：宝石被点击时发送
};

#endif // JEWEL_H
