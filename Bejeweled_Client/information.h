#ifndef INFORMATION_H
#define INFORMATION_H

#include "qobject.h"
class information
{
public:
    static information &instance();
    //用户名
    QString m_userName;
    //对手名
    QString m_enemyName;
    //控制随机数左边界（未启用）
    int m_LRange;
    //控制随机数右边界（启用）（仅针对单机模式）
    int m_RRange;
private:
    information();

};

#endif // INFORMATION_H
