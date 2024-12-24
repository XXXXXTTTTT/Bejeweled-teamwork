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
class Jewel :  public QObject,public QGraphicsPixmapItem {
    Q_OBJECT

    // 声明可动画的属性

    //位置
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

    //透明度
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)

    //大小
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)

private:
    //QString color;
    int m_type;     //宝石的种类
    int m_x;        //当前宝石的x坐标
    int m_y;        //当前宝石的y坐标
    bool m_choosed; // 是否被选中
    QPixmap m_pixmap; // 宝石静态图像
    QMovie *m_movie;  // 用于显示gif动图
    QPixmap m_selector; //宝石被选中时显示的框体
    qreal m_opacity = 1.0; // 默认完全不透明

    //当前被选中的宝石
    static Jewel* m_currSelectedJewel;

public:
    //默认构造
    Jewel() {};

    //含参构造
    Jewel(int x , int y , int type,QGraphicsPixmapItem *parent = nullptr);

    //析构函数
    ~Jewel();


    int getX(){return m_x;}

    int getY(){return m_y;}

    void setXY(int x, int y) {m_x = x, m_y = y;}
    //实现动画所需函数

    //返回位置
    QPointF pos() const;

    //设置位置
    void setPos(const QPointF& newPos);

    //返回透明度
    qreal opacity() const;

    //设置透明度
    void setOpacity(qreal opacity);

    // 返回当前缩放比例
    qreal scale() const;

    //设置缩放比例
    void setScale(qreal scaleFactor); // 设置


    //设置为静态图片显示
    void setStaticDisplay();
    //设置为动态图片显示
    void setDynamicDisplay();

    int getType() const { return m_type; }
    void setType(int type) { m_type = type; }

    void setIconForGem(int type);
    void ShowJewel(int a[8][8]);

    // QGraphicsItem 的纯虚函数：返回项目的边界矩形
    QRectF boundingRect() const override;

    // 自定义绘制函数，用于绘制宝石
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // 鼠标点击事件处理函数
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    //判断当前操作是否为交换操作(两点是否满足交换)
    // static bool isSwapOrNot(int x1, int y1, int x2, int y2);
signals:
    void jewelSwap(int x1, int y1, int x2, int y2); // 声明信号：宝石被点击时发送

    void opacityChanged();//动画修改透明度需要使用

    void scaleChanged(); // 缩放变化时发射的信号
private slots:
    // void setButtonIcon(){setIconForGem(m_type);}
    //void sendPosInfo_onclicked();



};

#endif // JEWEL_H
