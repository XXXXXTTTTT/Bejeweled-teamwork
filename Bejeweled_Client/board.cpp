#include "board.h"

#include "information.h"

#include "music.h"
#include "play.h"
QString r="";

Board::Board(QString r0, QGraphicsScene *sc)
    : m_scene(sc) {
    r=r0;
    m_combo=0;
    m_mus =music::instance();
    m_grid.resize(8, std::vector<int>(8));

    //初始化
    m_allJewelItems = std::vector<std::vector<Jewel*>>(8, std::vector<Jewel *>(8, nullptr));

    // 创建逻辑线程和工作器
    m_logicThread = new QThread(this);
    m_logicWorker = new LogicWorker(this);
    m_logicWorker->moveToThread(m_logicThread);

    //连接对应信号和槽函数
    connect(this, &Board::enqueueTask, m_logicWorker, &LogicWorker::addTask);
    // connect(m_logicWorker, &LogicWorker::taskFinished, this, &Board::handleTaskFinished);

    m_logicThread->start();

    // 将传入的数组转换为vector
    // for (int i = 0; i < 8; ++i) {
    //     std::vector<int> row;
    //     for (int k = 0; k < 8; ++k) {
    //         row.push_back(j[i][k]);
    //     }
    //     m_grid.push_back(row);
    // }

    generateBoard(r);  // 生成棋盘
    // for (int i = 0; i < 8; ++i) {
    //     std::vector<int> row;
    //     for (int k = 0; k < 8; ++k) {
    //         row.push_back(j[i][k]);
    //         // 创建宝石对象并设置坐标
    //         Jewel* gem = new Jewel(i, k, j[i][k]);
    //         connect(gem, &Jewel::jewelSwap, this, &Board::enqueueSwap);
    //         gem->setPos(QPointF(i * 67 + offsetX, k * 68 + offsetY));
    //         m_scene->addItem(gem);  // 将宝石添加到场景中
    //         m_allJewelItems[i][k] = gem;

    //     }
    //     m_grid.push_back(row);

    // }

    // generateBoard();  // 生成棋盘

    //若无可消,判定是否僵局
    if(!isAvailableOrNot()) {

        // 弹出提示框告诉玩家棋盘进入僵局
        QMessageBox::information(nullptr, "游戏提示", "棋盘已进入僵局！请等待片刻...", QMessageBox::Ok);

        // 在 2 秒后执行棋盘更新
        QTimer::singleShot(2000, this, &Board::updateBoard);
    }

    // generateBoard(r);  // 生成棋盘
    // for (int i = 0; i < 8; ++i) {
    //     std::vector<int> row;
    //     for (int k = 0; k < 8; ++k) {
    //         row.push_back(j[i][k]);
    //         // 创建宝石对象并设置坐标
    //         Jewel* gem = new Jewel(i, k, j[i][k]);
    //         connect(gem, &Jewel::jewelSwap, this, &Board::enqueueSwap);
    //         gem->setPos(QPointF(i * 67 + offsetX, k * 68 + offsetY));
    //         m_scene->addItem(gem);  // 将宝石添加到场景中
    //         m_allJewelItems[i][k] = gem;

    //     }
    //     m_grid.push_back(row);

    // }
    // generateBoard();  // 生成棋盘


    //若无可消,判定是否僵局
    if(!isAvailableOrNot()) {

        // 弹出提示框告诉玩家棋盘进入僵局
        QMessageBox::information(nullptr, "游戏提示", "棋盘已进入僵局！请等待片刻...", QMessageBox::Ok);

        // 在 2 秒后执行棋盘更新
        QTimer::singleShot(2000, this, &Board::updateBoard);
    }

}

Board::~Board() {
    m_logicThread->quit();
    m_logicThread->wait();
    delete m_logicWorker;
}

void Board::generateBoard(QString &r){

}
void Board::generateBoard() {


    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int gemType;
            if(!r.isEmpty())
            {
                gemType = r.at(0).digitValue();
                r.remove(0,1);
            }
            else
            {
                // 随机生成一个宝石类型
                gemType = QRandomGenerator::global()->bounded(1, information::instance().m_RRange);  // 随机生成1到7之间的宝石类型
            }
            // 需要检查该宝石是否符合规则
            while (checkForInvalidPlacement(i, j, gemType)) {
                if(!r.isEmpty())
                {
                    gemType = r.at(0).digitValue();
                    r.remove(0,1);
                }
                else
                {
                    // 随机生成一个宝石类型
                    gemType = QRandomGenerator::global()->bounded(1, information::instance().m_RRange);  // 随机生成1到7之间的宝石类型
                }
            }

            m_grid[i][j] = gemType;  // 更新 m_grid 中的宝石类型

            // 创建宝石对象并设置坐标
            Jewel* gem = new Jewel(i, j, gemType);
            connect(gem, &Jewel::jewelSwap, this, &Board::enqueueSwap);
            gem->setPos(QPointF(i * 67 + offsetX, j * 68 + offsetY));
            m_scene->addItem(gem);  // 将宝石添加到场景中
            m_allJewelItems[i][j] = gem;
        }
    }
}

bool Board::checkForInvalidPlacement(int x, int y, int gemType) {
    // 检查横向
    int count = 1;
    for (int i = x - 1; i >= 0 && m_grid[i][y] == gemType; --i) {
        ++count;
    }
    for (int i = x + 1; i < 8 && m_grid[i][y] == gemType; ++i) {
        ++count;
    }

    if (count >= 3) {
        return true;  // 如果横向有三个或更多相同宝石，返回 true
    }

    // 检查纵向
    count = 1;
    for (int j = y - 1; j >= 0 && m_grid[x][j] == gemType; --j) {
        ++count;
    }
    for (int j = y + 1; j < 8 && m_grid[x][j] == gemType; ++j) {
        ++count;
    }

    return count >= 3;  // 如果纵向有三个或更多相同宝石，返回 true
}

//刷新棋盘
void Board::updateBoard() {

    // 加锁范围，保护 m_grid 的一致性
    QMutexLocker locker(&m_mutex);

    bool boardValid = false;

    // 继续尝试生成有效的棋盘，直到成功
    while (!boardValid) {
        // 随机生成新的棋盘数据
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                // m_grid[i][j] = QRandomGenerator::global()->bounded(1, information::instance().m_RRange);  // 随机生成1到7之间的宝石类型

                if(!r.isEmpty())
                {
                    m_grid[i][j] = r.at(0).digitValue();
                    r.remove(0,1);
                }
                else
                {
                    // 随机生成一个宝石类型
                    m_grid[i][j] = QRandomGenerator::global()->bounded(1, information::instance().m_RRange);  // 随机生成1到7之间的宝石类型
                }
            }
        }

        // 检查棋盘是否有效：没有三颗相同宝石同行或同列
        boardValid = isBoardValid();
    }

    // 清空现有的棋盘上的宝石
    clearBoard();

    // 重新生成棋盘的宝石
    generateBoard();  // 根据新的 m_grid 数据生成棋盘
}



void Board::clearBoard() {
    // 清空现有棋盘上的宝石
    QList<QGraphicsItem*> items = m_scene->items();

    qDebug() << items.size();

    for (auto item : items) {
        m_scene->removeItem(item);
    }

    // 重置棋盘数据
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            qDebug() << "JewelManagerSize: " << m_grid[i].size();
            m_grid[i][j] = 0;  // 将棋盘上的所有宝石类型清空
            m_allJewelItems[i][j] = nullptr;
        }
    }

    //清空棋盘内容
    m_scene->clear();

}

//当棋盘无可消宝石时检测当前是否还有可消除交换操作
bool Board::isAvailableOrNot() {

    // 加锁范围，保护 m_grid 的一致性
    QMutexLocker locker(&m_mutex);

    //遍历每个宝石
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {

            //因为是从上到下从左到右的顺序故每次宝石只需跟其下的宝石,右的宝石进行交换测试即可

            //与下交换
            if(i+1 != 8 && m_grid[i][j] != m_grid[i+1][j]) {
                int tmp = m_grid[i][j];
                m_grid[i][j] = m_grid[i+1][j];
                m_grid[i+1][j] = tmp;


                //检测这两个宝石的改变是否形成可消除
                if(checkHorizontal(i, j)||checkVertical(i, j)||checkHorizontal(i+1, j)||checkVertical(i+1, j)) {
                    //还原

                    m_grid[i+1][j] = m_grid[i][j];
                    m_grid[i][j] = tmp;

                    //有可消
                    return true;
                }


                //还原

                m_grid[i+1][j] = m_grid[i][j];
                m_grid[i][j] = tmp;
            }


            //与右交换
            if(j+1 != 8 && m_grid[i][j] != m_grid[i][j+1]) {
                int tmp = m_grid[i][j];
                m_grid[i][j] = m_grid[i][j+1];
                m_grid[i][j+1] = tmp;

                //检测这两个宝石的改变是否形成可消除
                if(checkHorizontal(i, j)||checkVertical(i, j)||checkHorizontal(i, j+1)||checkVertical(i, j+1)) {
                    //还原

                    m_grid[i][j+1] = m_grid[i][j];
                    m_grid[i][j] = tmp;

                    //有可消
                    return true;
                }


                //还原

                m_grid[i][j+1] = m_grid[i][j];
                m_grid[i][j] = tmp;
            }
        }

    }

    return false;
}


bool Board::isBoardValid() {
    // 遍历棋盘的每个位置
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // 如果当前位置的宝石类型非零
            if (m_grid[i][j] != 0) {
                // 检查横向是否有三颗相同的宝石
                if (checkHorizontal(i, j)) {
                    return false;  // 如果有相同宝石横向连成三颗，返回无效
                }

                // 检查纵向是否有三颗相同的宝石
                if (checkVertical(i, j)) {
                    return false;  // 如果有相同宝石纵向连成三颗，返回无效
                }
            }
        }
    }

    // 如果没有发现三颗相同宝石，返回有效
    return true;
}


bool Board::checkHorizontal(int x, int y) {
    int gemType = m_grid[x][y];
    int count = 1;

    // 向左检查
    for (int i = x - 1; i >= 0 && m_grid[i][y] == gemType; --i) {
        ++count;
    }

    // 向右检查
    for (int i = x + 1; i < 8 && m_grid[i][y] == gemType; ++i) {
        ++count;
    }

    return count >= 3;  // 如果连续的宝石数>=3，则返回true，表示无效
}

bool Board::checkVertical(int x, int y) {
    int gemType = m_grid[x][y];
    int count = 1;

    // 向上检查
    for (int j = y - 1; j >= 0 && m_grid[x][j] == gemType; --j) {
        ++count;
    }

    // 向下检查
    for (int j = y + 1; j < 8 && m_grid[x][j] == gemType; ++j) {
        ++count;
    }

    return count >= 3;  // 如果连续的宝石数>=3，则返回true，表示无效
}

void Board::enqueueSwap(int x1, int y1, int x2, int y2) {

    if (m_logicWorker->isProcessingOrNot()) {
        qDebug() << "任务队列未完成，阻止新交换";
        return;
    }

    emit enqueueTask([=]() {
        swapJewels(x1, y1, x2, y2);
    });

}





//任务完成后处理下一个
void Board::handleTaskFinished() {
    qDebug() << "任务完成，准备处理下一个任务";
    // m_logicWorker->processNextTask();

}

//宝石交换任务
void Board::swapJewels(int x1, int y1, int x2, int y2) {
    // 加锁范围，保护 m_grid 的一致性
    QMutexLocker locker(&m_mutex);


    qDebug() << "接收到了";

    // 检查是否是相邻的交换
    if (abs(x1 - x2) + abs(y1 - y2) != 1) {
        return; // 只有相邻的宝石才允许交换
    }

    qDebug() << "222";
    Jewel* jewel1 = m_allJewelItems[x1][y1];
    Jewel* jewel2 = m_allJewelItems[x2][y2];

    qDebug() << "1: " << x1 << y1;

    qDebug() << "2: " << x2 << y2;

    if (!jewel1 || !jewel2) return;

    qDebug() << "动画来咯";

    // 创建动画组(并行,保证两个动画同时进行)
    QParallelAnimationGroup* animationGroup = new QParallelAnimationGroup(this);

    // 创建动画
    QPropertyAnimation* anim1 = new QPropertyAnimation(jewel1, "pos");
    QPropertyAnimation* anim2 = new QPropertyAnimation(jewel2, "pos");

    anim1->setDuration(500);
    anim2->setDuration(500);


    anim1->setStartValue(QPointF(x1 * 67 + offsetX, y1 * 68 + offsetY));
    anim2->setStartValue(QPointF(x2 * 67 + offsetX, y2 * 68 + offsetY));

    anim1->setEndValue(QPointF(x2 * 67 + offsetX, y2 * 68 + offsetY));
    anim2->setEndValue(QPointF(x1 * 67 + offsetX, y1 * 68 + offsetY));

    animationGroup->addAnimation(anim1);
    animationGroup->addAnimation(anim2);

    // 连接动画组结束信号
    connect(animationGroup, &QParallelAnimationGroup::finished, this, [=]() {
        // 交换数据
        std::swap(m_grid[x1][y1], m_grid[x2][y2]);

        qDebug() << "交换前";
        qDebug() << "x1y1:" << m_allJewelItems[x1][y1]->getType();

        qDebug()<< "x2y2:" << m_allJewelItems[x2][y2]->getType();

        // 检查是否形成了有效的消除
        if (checkForMatches()) {


            //交换后宝石信息更新

            swapJewelsDestination(jewel1, jewel2);
            qDebug() << "交换完后";
            qDebug() << "x1y1:" << m_allJewelItems[x1][y1]->getType();

            qDebug()<< "x2y2:" << m_allJewelItems[x2][y2]->getType();



            emit enqueueTask([=]() {
                processMatches();
            });



        } else {
            // 如果没有形成消除，交换回去
            QParallelAnimationGroup* reverseGroup = new QParallelAnimationGroup(this);

            QPropertyAnimation* reverseAnim1 = new QPropertyAnimation(jewel1, "pos");
            reverseAnim1->setDuration(300);
            reverseAnim1->setEndValue(QPointF(x1 * 67 + offsetX, y1 * 68 + offsetY));
            reverseGroup->addAnimation(reverseAnim1);

            QPropertyAnimation* reverseAnim2 = new QPropertyAnimation(jewel2, "pos");
            reverseAnim2->setDuration(300);
            reverseAnim2->setEndValue(QPointF(x2 * 67 + offsetX, y2 * 68 + offsetY));
            reverseGroup->addAnimation(reverseAnim2);

            reverseGroup->start(QAbstractAnimation::DeleteWhenStopped);
            // 同时还原逻辑位置
            std::swap(m_grid[x1][y1], m_grid[x2][y2]);
        }
    });

    // 启动动画组
    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

//检查是否有可消除的宝石
bool Board::checkForMatches() {

    // 加锁范围，保护 m_grid 的一致性
    QMutexLocker locker(&m_mutex);

    // 遍历整个棋盘，检查是否有横向或纵向的三连消除
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (m_grid[i][j] != 0 && (checkHorizontal(i, j) || checkVertical(i, j))) {
                return true;
            }
        }
    }
    return false;
}

bool Board::checkForChains(){
    QSet<std::pair<int, int>> newMatches;

    // 遍历整个棋盘，检查是否有横向或纵向的匹配
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (m_grid[i][j] != 0 && (checkHorizontal(i, j) || checkVertical(i, j))) {
                // 记录匹配的宝石位置
                if (checkHorizontal(i, j)) {
                    for (int k = j; k < 8 && m_grid[i][k] == m_grid[i][j]; ++k) {
                        newMatches.insert({i, k});
                    }
                }
                if (checkVertical(i, j)) {
                    for (int k = i; k < 8 && m_grid[k][j] == m_grid[i][j]; ++k) {
                        newMatches.insert({k, j});
                    }
                }
            }
        }
    }

    // 如果有新的匹配的宝石，说明有连锁反应
    return !newMatches.isEmpty();  // 返回是否有新的匹配
}

//删除匹配的宝石
void Board::processMatches() {

    // 找到所有需要消除的宝石


    QSet<std::pair<int, int>> matches;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (m_grid[i][j] != 0) {
                if (checkHorizontal(i, j)) {
                    for (int k = i; k < 8 && m_grid[k][j] == m_grid[i][j]; ++k) {
                        matches.insert({k, j});
                    }
                }
                if (checkVertical(i, j)) {
                    for (int k = j; k < 8 && m_grid[i][k] == m_grid[i][j]; ++k) {
                        matches.insert({i, k});
                    }
                }
            }
        }
    }


    qDebug() << "消除个数：" << matches.size();
    if(m_combo<6&&matches.size()>=6)
    {
        // m_combo++;
    }
    if(m_combo<6&&matches.size()>=9)
    {
        // m_combo++;
    }
    if(m_combo<6)
    {
        m_combo++;
        m_mus->sound("combo_"+ QString::number(m_combo)+".wav",Play::m_soundVolume);
    }
    else
    {
        m_combo++;
        m_mus->sound("combo_"+ QString::number(6)+".wav",Play::m_soundVolume);
    }


    qDebug() << "消除个数：" << matches.size();
    int matchCount = matches.size();  // 获取当前匹配的宝石数量

    if (matchCount > 0) {
        // 检查是否有连锁反应
        bool isChain = checkForChains();  // 判断是否有连锁

        // 计算得分并发射信号
        calculateScore(matchCount, isChain);

        // 如果发生了连锁反应，增加连锁次数
        if (isChain) {
            currentChain++;
        } else {
            // 没有连锁反应，重置连锁次数
            currentChain = 1;  // 初始消除时，默认连锁次数为 1
        }
    }
    /*
    if(m_combo<6&&matches.size()>=6)
    {
        m_combo++;
    }
    if(m_combo<6&&matches.size()>=9)
    {
        m_combo++;
    }
    if(m_combo<6)
    {
        m_combo++;
    }
    m_mus->sound("combo_"+ QString::number(m_combo)+".wav",Play::m_soundVolume);
<<<<<<< HEAD
*/

    qDebug() << "消除个数：" << matches.size();

    QParallelAnimationGroup* deleteGroup = new QParallelAnimationGroup(this);

    // 加锁范围，保护 m_grid 的一致性
    {


        // 加锁范围，保护 m_grid 的一致性
        QMutexLocker locker(&m_mutex);

        // 消除匹配的宝石
        for (auto& match : matches) {

            int x = match.first;
            int y = match.second;

            Jewel* jewel = m_allJewelItems[x][y];
            if (jewel) {
                qDebug() << x << ": " << y;
                // 创建透明度动画
                QPropertyAnimation* fadeAnim = new QPropertyAnimation(jewel, "opacity");
                fadeAnim->setDuration(300);
                fadeAnim->setStartValue(1.0);  // 开始时完全不透明
                fadeAnim->setEndValue(0.0);    // 结束时完全透明

                // 创建缩放动画
                QPropertyAnimation* scaleAnim = new QPropertyAnimation(jewel, "scale");
                scaleAnim->setDuration(300);
                scaleAnim->setStartValue(1.0);  // 开始时大小为 1
                scaleAnim->setEndValue(0.0);    // 结束时大小为 0，完全消失

                // 将动画添加到动画组中
                deleteGroup->addAnimation(fadeAnim);
                deleteGroup->addAnimation(scaleAnim);

                m_grid[x][y] = 0;

                m_allJewelItems[x][y] = nullptr;

                deleteGroup->addAnimation(fadeAnim);
                deleteGroup->addAnimation(scaleAnim);
                connect(fadeAnim, &QPropertyAnimation::finished, this, [=]() {

                    qDebug() << "BEFORE: JEWELSIZE: " << m_scene->items().size();

                    m_scene->removeItem(jewel);

                    // delete jewel;

                    qDebug() << "AFTER: JEWELSIZE: " << m_scene->items().size();



                });
            }
        }



    }
    connect(deleteGroup, &QParallelAnimationGroup::finished, this, [=]() {
        qDebug() << "ALLAFTER: JEWELSIZE: " << m_scene->items().size();
        qDebug() << "消除完毕";

        emit enqueueTask([=]() {
            dropJewels();
        });
    });

    deleteGroup->start(QAbstractAnimation::DeleteWhenStopped);

}

//匹配后的宝石下落
void Board::dropJewels() {

    QParallelAnimationGroup* dropGroup = new QParallelAnimationGroup(this);

    int num = 0;

    {



        // 加锁范围，保护 m_grid 的一致性
        QMutexLocker locker(&m_mutex);






        for (int x = 0; x < 8; ++x) {
            for (int y = 7; y >= 0; --y) {
                //寻找消除了的宝石
                if (m_grid[x][y] == 0) {
                    // qDebug() << "x,y: " << x << y;
                    for (int k = y - 1; k >= 0; --k) {
                        //寻找顶替消除宝石位置的宝石
                        if (m_grid[x][k] != 0) {
                            // qDebug() << "x,k: " << x << k;
                            Jewel* jewel = m_allJewelItems[x][k];
                            if (jewel) {
                                jewel->setXY(x, y);
                                m_allJewelItems[x][y] = jewel;
                                m_allJewelItems[x][k] = nullptr;
                                m_grid[x][y] = m_grid[x][k];
                                m_grid[x][k] = 0;
                                QPropertyAnimation* dropAnim = new QPropertyAnimation(jewel, "pos");
                                dropAnim->setDuration(300);
                                dropAnim->setEndValue(QPointF(x * 67 + offsetX, y * 68 + offsetY));
                                dropGroup->addAnimation(dropAnim);

                                num++;
                                //处理消除
                                // connect(dropAnim, &QPropertyAnimation::finished, [=]() {

                                    //     if(checkForMatches()) {
                                    //         processMatches();
                                    //     }
                                    // });

                            }

                            break;
                        }
                    }
                }
            }
        }

    }

    qDebug() << "BEFOR: num:" << num;
    if(num != 0) {
        connect(dropGroup, &QParallelAnimationGroup::finished, this, [=]() {
            qDebug() << "AFTER: num:" << num;
            qDebug() << "下落完毕";
            emit enqueueTask([=]() {
                generateNewJewels();
            });
        });

        dropGroup->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        qDebug() << "无下落需要";
        emit enqueueTask([=]() {
            generateNewJewels();
        });
    }



}

//随机生成新宝石
void Board::generateNewJewels() {



    QParallelAnimationGroup* generateNewGroup = new QParallelAnimationGroup(this);
    // {
    // 加锁范围，保护 m_grid 的一致性
    QMutexLocker locker(&m_mutex);

        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                if (m_grid[x][y] == 0) {

                // qDebug() << "OK";
                int gemType;
                if(!r.isEmpty())
                {
                    gemType = r.at(0).digitValue();
                    r.remove(0,1);
                }
                else
                {
                    // 随机生成一个宝石类型
                    gemType = QRandomGenerator::global()->bounded(1, information::instance().m_RRange);  // 随机生成1到7之间的宝石类型
                }

                m_grid[x][y] = gemType;

                    Jewel* gem = new Jewel(x, y, gemType);
                    //连接交换信号和槽函数
                    connect(gem, &Jewel::jewelSwap, this, &Board::enqueueSwap);
                    gem->setPos(QPointF(x * 67 + 252, -100)); // 从上方掉落
                    m_scene->addItem(gem);
                    m_allJewelItems[x][y] = gem;

                    QPropertyAnimation* dropAnim = new QPropertyAnimation(gem, "pos");
                    dropAnim->setDuration(300);
                    dropAnim->setEndValue(QPointF(x * 67 + 252, y * 68 + 45));

                    generateNewGroup->addAnimation(dropAnim);
                    //处理消除
                    // connect(dropAnim, &QPropertyAnimation::finished, [=]() {

                    //     if(checkForMatches()) {
                    //         processMatches();
                    //     }
                    // });
                }
            }
        }

    // }

    connect(generateNewGroup, &QParallelAnimationGroup::finished, this, [=]() {
        qDebug() << "生成完毕";

            //若有可消
            emit enqueueTask([=]() {

                if (checkForMatches()) {

                    processMatches();
                } else {
                    //根据连击播放音效
                    if(m_combo==6)
                    {
                        m_mus->sound("unbelievable.mp3",Play::m_soundVolume);
                    }
                    else if(m_combo==5)
                    {
                        m_mus->sound("amazing.wav",Play::m_soundVolume);
                    }
                    else if(m_combo==4)
                    {
                        m_mus->sound("excellent.wav",Play::m_soundVolume);
                    }

                    else if(m_combo==3)
                    {
                        m_mus->sound("great.mp3",Play::m_soundVolume);
                    }
                    else if(m_combo==2)
                    {
                        m_mus->sound("good.wav",Play::m_soundVolume);
                    }
                    m_combo=0;

                    qDebug() << "检测是否僵局";
                    //若无可消,判定是否僵局
                    if(!isAvailableOrNot()) {

                        // 弹出提示框告诉玩家棋盘进入僵局
                        QMessageBox::information(nullptr, "游戏提示", "棋盘已进入僵局！请等待片刻...", QMessageBox::Ok);

                        // 在 2 秒后执行棋盘更新
                        QTimer::singleShot(2000, this, &Board::updateBoard);
                    }
                }
            });



        });

    generateNewGroup->start(QAbstractAnimation::DeleteWhenStopped);

}

//交换宝石位置信息
void Board::swapJewelsDestination(Jewel* j1,Jewel* j2) {
    // 获取各自的坐标
    int x1 = j1->getX();
    int y1 = j1->getY();
    int x2 = j2->getX();
    int y2 = j2->getY();
    // 交换指针
    std::swap(m_allJewelItems[x1][y1], m_allJewelItems[x2][y2]);


    j1->setXY(x2, y2);
    j2->setXY(x1, y1);
}
//根据索引查找当前宝石items
// Jewel* Board::findJewelAt(int x, int y) {
//     //todo: 优化
//     QList<QGraphicsItem*> items = m_scene->items();

//     // qDebug()  << "XXX" << x * 67 + offsetX << ": " << y * 68 + offsetY;
//     for (auto item : items) {

//         if (Jewel* jewel = dynamic_cast<Jewel*>(item)) {

//             //全局坐标对比
//             if(jewel->scenePos() == QPointF(x * 67 + offsetX, y * 68 + offsetY)) {
//                 return jewel;
//             }


//         }
//     }
//     return nullptr;
// }

//给予提示
// void Board::giveHint() {

// }
//int Board::getEli(){
//    return matches.size();
//}
/*
void Board::updateScore(int matchCount) {
    matchCount = Eli;
    qDebug() << "Eli is " << Eli;
    // 调用分数计算函数来更新得分
    calculateScore(matchCount);
}

void Board::calculateScore(int matchCount) {
    matchCount = getEli();
    int scoreIncrement = 0;

    // 每消除一个宝石加基本得分
    scoreIncrement += matchCount * BASE_SCORE;

    // 如果有连锁反应，额外加分
    if (currentChain > 0) {
        scoreIncrement += currentChain * CHAIN_BONUS;
    }

    // 如果是四连或五连消除，加上特殊奖励
    if (matchCount == 4) {
        scoreIncrement += FOUR_MATCH_BONUS;
    } else if (matchCount == 5) {
        scoreIncrement += FIVE_MATCH_BONUS;
    }

    // 更新总得分
    m_myscore += scoreIncrement;

    qDebug() << "myscore is " << m_myscore;

    // 发射分数更新信号
    emit scoreUpdated(m_myscore);
}
*/
void Board::calculateScore(int matchCount, bool isChain) {
    const int BASE_SCORE = 10;  // 每个宝石的基本分数
    int score = 0;

    if (!isChain) {
        // 如果是连锁反应，每个宝石得分乘以当前连锁次数
        qDebug() << "发生连锁反应" ;
        score = matchCount * BASE_SCORE * (currentChain + 1);  // 连锁得分
    } else {
        // 如果没有连锁反应，按基础分数计算

        score = matchCount * BASE_SCORE;
        qDebug() << "score is :" << score;
        qDebug() << "matchCount is :" << matchCount;
    }

    // 四连消除奖励
    const int FOUR_MATCH_BONUS = 50;
    const int FIVE_MATCH_BONUS = 100;

    // 如果消除的是 4 个宝石，奖励加分
    if (matchCount == 4) {
        score += FOUR_MATCH_BONUS;
    }
    // 如果消除的是 5 个宝石，奖励加分
    else if (matchCount == 5) {
        score += FIVE_MATCH_BONUS;
    }

    // 发射得分更新信号
    emit scoreUpdated(score);
}


