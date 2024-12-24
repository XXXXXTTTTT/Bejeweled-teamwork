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
private:
    information();

};

#endif // INFORMATION_H
