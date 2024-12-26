#ifndef INFORMATION_H
#define INFORMATION_H

#include "qobject.h"
class information
{
public:
    static information &instance();
    //控制随机数左边界（未启用）
    int m_LRange;
    //控制随机数右边界（启用）（仅针对联机模式）
    int m_RRange;
    //对手id
    QString m_enemyId;
    //当前用户id
    QString m_userId;

    QString rankingUserName="";

    QString highScore="";

    QString m_easyPlayerName="";

    int m_saveOrNot=0;
private:
    information();

};

#endif // INFORMATION_H
