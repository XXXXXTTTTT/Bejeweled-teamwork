#include "information.h"

information::information()
    //在此更改默认值
    :m_LRange(1),m_RRange(8){}
information &information::instance()
{

    static information instance;

    return instance;
};
