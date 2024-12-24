#include "information.h"

information::information()
    :m_userName("user"),m_enemyName("enemy"){}
information &information::instance()
{

    static information instance;

    return instance;
};
