#include "Jewel.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

// 构造函数，初始化宝石的坐标、种类以及所在格子的位置
Jewel::Jewel(int x, int y, int type, int where, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_x(x), m_y(m_y), m_type(m_type), m_where(m_where) {

    setIconForGem(type); // 设置宝石图标

    // 设置宝石为可选状态并可以接收鼠标点击事件
    setFlag(QGraphicsItem::ItemIsSelectable);
    // 禁用拖动
    setFlag(QGraphicsItem::ItemIsMovable, false);  // 禁止移动宝石

    // 设置宝石项的初始位置（计算位置）
    if (where == 1) {
        setPos(67 * (y + 1) + 175, 67 * (x + 1) + 36); // 计算宝石位置
    } else {
        setPos(67 * (y + 1) + 879, 67 * (x + 1) + 36);
    }

    m_movie = new QMovie(":/images/media/media/" + QString::number(type) + ".gif");  // 选择适合的gif路径
    m_movie->setScaledSize(QSize(64, 64));  // 设置动画的大小
}

Jewel::~Jewel() {
    // 如果有动态分配的内存资源，这里需要进行清理
    qDebug() << "Jewel destructor called";  // 可选的调试输出
}

// 设置宝石图标
void Jewel::setIconForGem(int type) {
    // 根据宝石类型选择静态图片
    switch (type) {
    case 1:
        m_pixmap = QPixmap(":/images/media/media/1.png");
        break;
    case 2:
        m_pixmap = QPixmap(":/images/media/media/2.png");
        break;
    case 3:
        m_pixmap = QPixmap(":/images/media/media/3.png");
        break;
    case 4:
        m_pixmap = QPixmap(":/images/media/media/4.png");
        break;
    case 5:
        m_pixmap = QPixmap(":/images/media/media/5.png");
        break;
    case 6:
        m_pixmap = QPixmap(":/images/media/media/6.png");
        break;
    case 7:
        m_pixmap = QPixmap(":/images/media/media/7.png");
        break;
    }
}

// 返回宝石项的边界矩形
QRectF Jewel::boundingRect() const {
    // 返回宝石项的矩形区域，通常是宝石的大小
    return QRectF(0, 0, 63, 63);
}

// 自定义绘制函数，用于绘制宝石
void Jewel::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // 如果有 GIF 动画并且它正在播放
    if (m_movie && m_movie->state() == QMovie::Running) {
        // 使用 QMovie 播放 GIF 动画的当前帧
        painter->drawPixmap(0, 0, m_movie->currentPixmap());
    } else {
        // 如果没有 GIF 动画，则显示静态图片
        if (!m_pixmap.isNull()) {
            // 绘制静态图片（pixmap 仅在未播放动画时使用）
            painter->drawPixmap(0, 0, m_pixmap);
        }
    }
}


// 鼠标点击事件处理函数
void Jewel::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_choosed) {
        m_choosed = true;

        // 创建并播放对应类型的 GIF 动画
        if (!m_movie) {
            switch (m_type) {
            case 1:
                m_movie = new QMovie(":/images/media/media/1.gif");
                break;
            case 2:
                m_movie = new QMovie(":/images/media/media/2.gif");
                break;
            case 3:
                m_movie = new QMovie(":/images/media/media/3.gif");
                break;
            case 4:
                m_movie = new QMovie(":/images/media/media/4.gif");
                break;
            case 5:
                m_movie = new QMovie(":/images/media/media/5.gif");
                break;
            case 6:
                m_movie = new QMovie(":/images/media/media/6.gif");
                break;
            case 7:
                m_movie = new QMovie(":/images/media/media/7.gif");
                break;
            default:
                break;
            }

            m_movie->setScaledSize(QSize(64, 64));  // 设置 GIF 动画的大小
            connect(m_movie, &QMovie::finished, m_movie, &QMovie::start);  // 设置动画循环播放
        }

        if (m_movie->state() != QMovie::Running) {
            m_movie->start();  // 开始播放动画
        }

        // 更新宝石的显示状态
        update();  // 触发重绘，使动画开始播放

        qDebug() << "Jewel at (" << m_x << "," << m_y << ") selected!";
        emit jewelClicked(m_x, m_y);  // 发射信号，通知外部
    }
}


