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
    //password
    QString m_password="";
    //控制随机数左边界（未启用）
    int m_LRange;
    //控制随机数右边界（启用）（仅针对单机模式）
    int m_RRange;
    //对手得分
    int m_enemyScore;

    QString rankingUserName="";

    QString highScore="";

    QString m_r="";
private:
    information();

};

#endif // INFORMATION_H
