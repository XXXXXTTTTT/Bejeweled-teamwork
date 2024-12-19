#include "Jewel.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

// 构造函数，初始化宝石的坐标、种类以及所在格子的位置
Jewel::Jewel(int x, int y, int type, int where, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_x(x), m_y(y), m_type(type), m_where(where) {

    setIconForGem(type); // 设置宝石图标

    // 设置宝石为可选状态并可以接收鼠标点击事件
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, false);  // 禁止移动宝石

    // 计算宝石的位置
    if (where == 1) {
        setPos(67 * (y + 1) + 175, 67 * (x + 1) + 36);
    } else {
        setPos(67 * (y + 1) + 879, 67 * (x + 1) + 36);
    }


    m_movie = new QMovie(":/images/media/media/" + QString::number(type) + ".gif");  // 选择适合的gif路径

    m_movie->setScaledSize(QSize(64, 64));  // 设置 GIF 动画的大小

    // 连接信号：每次 GIF 动画的帧变化时，调用 onFrameChanged
    connect(m_movie, &QMovie::frameChanged, this, &Jewel::onFrameChanged);

}


Jewel::~Jewel() {
    if (m_movie) {
        delete m_movie;  // 确保删除动画对象
    }
    if (!m_pixmap.isNull()) {
        m_pixmap = QPixmap();  // 清空 pixmap
    }
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

void Jewel::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    if (m_movie && m_movie->state() == QMovie::Running) {
        // 使用 QMovie 播放 GIF 动画的当前帧
        painter->drawPixmap(0, 0, m_movie->currentPixmap());
    } else {
        // 如果没有 GIF 动画，则显示静态图片
        if (!m_pixmap.isNull()) {
            painter->drawPixmap(0, 0, m_pixmap);
        }
    }
}

void Jewel::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_choosed) {
        m_choosed = true;

        // 只有在第一次点击时才加载并开始播放 GIF 动画
        if (!m_movie) {
            // 根据类型选择不同的GIF动画
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

            // 检查是否成功加载了 GIF
            if (m_movie->isValid()) {
                qDebug() << "GIF loaded successfully!";
                m_movie->setScaledSize(QSize(64, 64));  // 设置 GIF 动画的大小
                m_movie->start();  // 启动动画播放
                m_pixmap = QPixmap();  // 清除原来的静态图标
            } else {
                qWarning() << "Failed to load GIF!";
            }
        }

        // 更新宝石的显示状态
        update();  // 触发重绘，使动画开始播放
        qDebug() << "Jewel at (" << m_x << "," << m_y << ") selected!";
        emit jewelClicked(m_x, m_y);  // 发射信号，通知外部
    }
}

// 新增槽函数，处理帧更新
void Jewel::onFrameChanged(int frameNumber) {
    qDebug() << "Frame changed: " << frameNumber;
    update();  // 强制重绘，显示新的一帧
}
