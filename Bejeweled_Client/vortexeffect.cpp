#include "vortexeffect.h"

VortexEffect::VortexEffect(const QString& gifPath, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), movie(new QMovie(gifPath)) {
    if (!movie->isValid()) {
        qWarning() << "Failed to load GIF:" << gifPath;
        return;
    }

    // 设置 GIF 动画的更新回调
    connect(movie, &QMovie::frameChanged, this, &VortexEffect::updateFrame);

    // 设置变换原点为几何中心
    setTransformOriginPoint(boundingRect().center());

    // 开始播放动画
    movie->start();
}

VortexEffect::~VortexEffect() {
    delete movie;
}

void VortexEffect::stop() {
    // 停止动画播放
    if (movie) {
        movie->stop();
    }
}

void VortexEffect::updateFrame() {
    // 更新当前帧到 QGraphicsPixmapItem
    if (movie) {
        setPixmap(movie->currentPixmap());
    }
}
