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

    m_hint = new QMovie(":/images/media/media/hint.gif");


    if (m_movie->isValid() && m_hint->isValid()) {
        //图片.gif设置
        m_movie->setScaledSize(QSize(64, 64));// 设置 GIF 动画的大小

        m_movie->setCacheMode(QMovie::CacheAll); // 缓存所有帧，避免频繁加载

        connect(m_movie, &QMovie::frameChanged, this, [this]() {
            // 清除图像
            // this->setPixmap(QPixmap());  // 先清空当前 pixmap
            this->setPixmap(m_movie->currentPixmap());  // 再设置当前帧
            // this->update();
        });

        connect(m_movie, &QMovie::finished, this, [this]() {
            m_movie->start();  // 动画播放完成后重新开始(即无限循环)
        });

        //提示时的框体.gif设置
        m_hint->setScaledSize(QSize(65, 65));// 设置 GIF 动画的大小

        m_hint->setCacheMode(QMovie::CacheAll); // 缓存所有帧，避免频繁加载

        connect(m_hint, &QMovie::frameChanged, this, [this]() {
            this->setPixmap(m_hint->currentPixmap());  // 再设置当前帧
            // this->update();
        });

        connect(m_hint, &QMovie::finished, this, [this]() {
            m_hint->start();  // 动画播放完成后重新开始(即无限循环)
        });

    } else {
        qWarning() << "Failed to load GIF for type" << m_type;
    }

    // 设置宝石为可选状态并可以接收鼠标点击事件
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, false);  // 禁止移动宝石


    // 设置初始为静态显示
    setStaticDisplay();

    // 设置变换原点为中心
    setTransformOriginPoint(boundingRect().center());
}

//属性设置
void Jewel::initValue(){
    //初始化图片文件

    // 根据宝石类型选择静态图片
    m_pixmap = QPixmap(":/images/media/media/" + QString::number(m_type) + ".png");

    //若之前已经初始化过则先删除之前的
    if (m_movie) {
        m_movie->stop();
        disconnect(m_movie, &QMovie::frameChanged, this, nullptr);
        disconnect(m_movie, &QMovie::finished, this, nullptr);
        delete m_movie;  // 确保删除动画对象
    }

    if(m_hint) {
        m_hint->stop();
        disconnect(m_hint, &QMovie::frameChanged, this, nullptr);
        disconnect(m_hint, &QMovie::finished, this, nullptr);
        delete m_hint;  // 确保删除动画对象
    }


    //图片选择时播放的.gif
    m_movie = new QMovie(":/images/media/media/" + QString::number(m_type) + ".gif");

    //图片被选择时的框体
    m_selector = QPixmap(":/images/media/media/selector.png");

    m_hint = new QMovie(":/images/media/media/hint.gif");


    if (m_movie->isValid() && m_hint->isValid()) {
        //图片.gif设置
        m_movie->setScaledSize(QSize(64, 64));// 设置 GIF 动画的大小

        m_movie->setCacheMode(QMovie::CacheAll); // 缓存所有帧，避免频繁加载

        connect(m_movie, &QMovie::frameChanged, this, [this]() {
            // 清除图像
            // this->setPixmap(QPixmap());  // 先清空当前 pixmap
            this->setPixmap(m_movie->currentPixmap());  // 再设置当前帧
            // this->update();
        });

        connect(m_movie, &QMovie::finished, this, [this]() {
            m_movie->start();  // 动画播放完成后重新开始(即无限循环)
        });

        //提示时的框体.gif设置
        m_hint->setScaledSize(QSize(65, 65));// 设置 GIF 动画的大小

        m_hint->setCacheMode(QMovie::CacheAll); // 缓存所有帧，避免频繁加载

        connect(m_hint, &QMovie::frameChanged, this, [this]() {
            this->setPixmap(m_hint->currentPixmap());  // 再设置当前帧
            // this->update();
        });

        connect(m_hint, &QMovie::finished, this, [this]() {
            m_hint->start();  // 动画播放完成后重新开始(即无限循环)
        });

    } else {
        qWarning() << "Failed to load GIF for type" << m_type;
    }

    // 设置宝石为可选状态并可以接收鼠标点击事件
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, false);  // 禁止移动宝石


    // 设置初始为静态显示
    setStaticDisplay();

    qDebug() << "静态播放";
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

    if(m_hint) {
        m_hint->stop();
        disconnect(m_hint, &QMovie::frameChanged, this, nullptr);
        disconnect(m_hint, &QMovie::finished, this, nullptr);
        delete m_hint;  // 确保删除动画对象
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
    return QRectF(0, 0, 65, 65);
}

//设置点击区域为整个矩形内区域
QPainterPath Jewel::shape() const {
    QPainterPath path;
    path.addRect(boundingRect()); // 将整个边界矩形定义为可点击区域
    return path;
}

void Jewel::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // 清除绘制区域，避免内容叠加
    painter->fillRect(boundingRect(), Qt::transparent);

    // 绘制选择框图片
    if (m_choosed) {
        if (m_hint) {
            m_hint->stop();  // 停止动画
        }
        painter->drawPixmap(0, 0, 65, 65, m_selector);  // 绘制选择框图片
    } else if(!m_choosed && m_hint && m_hint->state() == QMovie::Running) {
        QPixmap frame = m_hint->currentPixmap();
        QRectF targetRect = boundingRect();  // 使用边界矩形作为目标区域
        QRectF sourceRect(0, 0, frame.width(), frame.height());
        painter->drawPixmap(targetRect, frame, sourceRect);
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
    qDebug() << "选中n";
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





    }   else {

        // 更新选中的宝石
        m_currSelectedJewel = this;
        m_choosed = false;

        setStaticDisplay();
    }

}


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

//显示提示
void Jewel::DrawHint() {
    if (m_hint && m_hint->isValid()) {
        m_hint->start();
        // 将 QMovie 设置为当前 Pixmap 的来源
        setPixmap(QPixmap());
    }
}




