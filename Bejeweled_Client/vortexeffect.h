#ifndef VORTEXEFFECT_H
#define VORTEXEFFECT_H

#include <QGraphicsPixmapItem>
#include <QMovie>

class VortexEffect : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit VortexEffect(const QString& gifPath, QGraphicsItem* parent = nullptr);
    ~VortexEffect();

    void stop(); // 停止播放

private slots:
    void updateFrame(); // 更新当前帧

private:
    QMovie* movie; // GIF 动画
};

#endif // VORTEXEFFECT_H
