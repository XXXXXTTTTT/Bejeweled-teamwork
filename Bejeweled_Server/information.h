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
private:
    information();

};

#endif // INFORMATION_H
