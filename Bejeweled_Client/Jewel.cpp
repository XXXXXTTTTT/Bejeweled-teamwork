#include "Jewel.h"
#include "music.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <play.h>

//初始化静态变量
Jewel* Jewel::m_currSelectedJewel = nullptr;


// 构造函数，初始化宝石的坐标、种类以及所在格子的位置
Jewel::Jewel(int x, int y, int type,QGraphicsPixmapItem *parent)
    : QGraphicsPixmapItem(parent), m_type(type), m_x(x), m_y(y) {
            m_mus=music::instance();



    //初始化图片文件

    // 根据宝石类型选择静态图片
    m_pixmap = QPixmap(":/images/media/media/" + QString::number(m_type) + ".png");

    //图片选择时播放的.gif
    m_movie = new QMovie(":/images/media/media/" + QString::number(m_type) + ".gif");
    // m_movie = new QMovie(":/images/media/media/blueR.gif");

    //图片被选择时的框体
    m_selector = QPixmap(":/images/media/media/selector.png");


    if (m_movie->isValid()) {
        m_movie->setScaledSize(QSize(64, 64));// 设置 GIF 动画的大小

        m_movie->setCacheMode(QMovie::CacheAll); // 缓存所有帧，避免频繁加载

        connect(m_movie, &QMovie::frameChanged, this, [this]() {
            // 清除图像
            this->setPixmap(QPixmap());  // 先清空当前 pixmap
            this->setPixmap(m_movie->currentPixmap());  // 再设置当前帧
            // this->update();
        });

        connect(m_movie, &QMovie::finished, this, [this]() {
            m_movie->start();  // 动画播放完成后重新开始(即无限循环)
        });

    } else {
        qWarning() << "Failed to load GIF for type" << m_type;
    }

    // 设置宝石为可选状态并可以接收鼠标点击事件
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, false);  // 禁止移动宝石

    // 计算宝石的位置
    // if (where == 1) {
    //     setPos(67 * (y + 1) + 175, 67 * (x + 1) + 36);
    // } else {
    //     setPos(67 * (y + 1) + 879, 67 * (x + 1) + 36);
    // }


    // 连接信号：每次 GIF 动画的帧变化时，调用 onFrameChanged
    // connect(m_movie, &QMovie::frameChanged, this, &Jewel::onFrameChanged);


    // 设置初始为静态显示
    setStaticDisplay();

    // 设置变换原点为中心
    setTransformOriginPoint(boundingRect().center());
}

//析构函数
Jewel::~Jewel() {


    update();

    if (m_movie) {
        m_movie->stop();
        disconnect(m_movie, &QMovie::frameChanged, this, nullptr);
        disconnect(m_movie, &QMovie::finished, this, nullptr);
        delete m_movie;  // 确保删除动画对象
    }

    if (!m_pixmap.isNull()) {
        m_pixmap = QPixmap();  // 清空 pixmap
    }

    m_choosed = false;
    qDebug() << "Jewel destructor called";  // 可选的调试输出
}

//动画函数条件

QPointF Jewel::pos() const {
    return QGraphicsPixmapItem::pos();
}

//设置位置
void Jewel::setPos(const QPointF& newPos) {
    QGraphicsPixmapItem::setPos(newPos); // 让 QGraphicsView 正确更新位置
    // qDebug() << "SETPOS: " << pos().x() << ": " << pos().y();
}

qreal Jewel::opacity() const {
    return m_opacity;
}

//设置透明度
void Jewel::setOpacity(qreal opacity) {
    if (!qFuzzyCompare(m_opacity, opacity)) {
        m_opacity = opacity;
        setOpacity(m_opacity); // 更新 QGraphicsItem 的透明度
        emit opacityChanged();
    }
}

// 返回当前缩放比例
qreal Jewel::scale() const {
    return QGraphicsItem::scale();
}

//设置缩放比例
void Jewel::setScale(qreal scaleFactor) {
    // 设置缩放比例
    QGraphicsPixmapItem::setScale(scaleFactor);
    emit scaleChanged(); // 发射缩放变化的信号
}



// 返回宝石项的边界矩形
QRectF Jewel::boundingRect() const {
    if (m_movie && m_movie->state() == QMovie::Running) {
        // qDebug() << " Width: " <<  m_movie->currentPixmap().width() << " hEIGHT: " << m_movie->currentPixmap().height() ;
        return QRectF(0, 0, m_movie->currentPixmap().width(), m_movie->currentPixmap().height());
    }
    // 返回宝石项的矩形区域，通常是宝石的大小
    return QRectF(0, 0, 63, 63);
}

void Jewel::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // 清除绘制区域，避免内容叠加
    painter->fillRect(boundingRect(), Qt::transparent);

    // 绘制选择框图片
    if (m_choosed) {

        painter->drawPixmap(0, 0, 65, 65, m_selector);  // 绘制选择框图片
    }

    if (m_movie && m_movie->state() == QMovie::Running) {
        QPixmap frame = m_movie->currentPixmap();
        QRectF targetRect = boundingRect();  // 使用边界矩形作为目标区域
        QRectF sourceRect(0, 0, frame.width(), frame.height());
        painter->drawPixmap(targetRect, frame, sourceRect);
    } else {
        // 如果没有 GIF 动画，则显示静态图片
        if (!m_pixmap.isNull()) {
            painter->drawPixmap(5, 5, m_pixmap);
        }
    }
}

void Jewel::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (!m_choosed) {
        m_mus->sound("click.wav",Play::m_soundVolume);
        // 取消之前选中的宝石的状态
        if (m_currSelectedJewel && m_currSelectedJewel != this) {
            m_currSelectedJewel->setStaticDisplay();

        }


        //首先得先排除m_currSelectedJewel是空,若为空则直接进行更新
        if(m_currSelectedJewel && abs(m_currSelectedJewel->getX() - m_x) + abs(m_currSelectedJewel->getY() - m_y) == 1) {

            qDebug() << "LastSelected：(" << m_currSelectedJewel->getX() << "," << m_currSelectedJewel->getY() << ")" << m_currSelectedJewel->getType();            // 设置动态显示
            qDebug() << "CurrSelected：(" << m_x << "," << m_y << ")" << m_type;            // 设置动态显示
            // 发射信号，通知外部
            emit jewelSwap(m_x, m_y, m_currSelectedJewel->getX(), m_currSelectedJewel->getY());
            //若是进行交换操作(即上次选中的宝石与当前选中宝石邻近表交换)
            m_currSelectedJewel = nullptr;


        } else {
            //若不是则


            // 更新选中的宝石
            m_currSelectedJewel = this;
            m_choosed = true;


            qDebug() << "selected：(" << m_currSelectedJewel->getX() << "," << m_currSelectedJewel->getY() << ")" << m_currSelectedJewel->getType();            // 设置动态显示
            setDynamicDisplay();
        }


        // // 更新选中的宝石
        // m_currSelectedJewel = this;
        // m_choosed = true;

        // // 设置动态显示
        // setDynamicDisplay();





    }
    else {


        // // 更新选中的宝石
        // m_currSelectedJewel = this;
        // m_choosed = true;
        // 更新选中的宝石
        m_currSelectedJewel = this;
        m_choosed = false;

        // qDebug() << "selected：(" << m_currSelectedJewel->getX() << "," << m_currSelectedJewel->getY() << ")" << m_currSelectedJewel->getType();            // 设置动态显示
        // setDynamicDisplay();
        setStaticDisplay();
    }
}

//停止播放宝石gif动画
// void Jewel::stopGifAnimation() {
//     if (m_movie) {
//         m_movie->stop();  // 停止 GIF 动画
//     }
//     m_choosed = false;
//     m_pixmap = QPixmap(":/images/media/media/" + QString::number(m_type) + ".png");  // 恢复到初始静态图标
//     // qDebug() << m_pixmap.isDetached();
//     update();  // 触发重绘
// }


//设置为静态图片
void Jewel::setStaticDisplay() {
    m_choosed = false;
    if (m_movie) {
        m_movie->stop();  // 停止动画
    }
    setPixmap(m_pixmap);  // 切换到静态图标
    // update();  // 触发重绘
}

//设置为动态gif图片
void Jewel::setDynamicDisplay() {
    if (m_movie && m_movie->isValid()) {
        m_movie->start();  // 启动动画播放
        // 将 QMovie 设置为当前 Pixmap 的来源
        setPixmap(QPixmap());
    }
    // update();  // 触发重绘
}
