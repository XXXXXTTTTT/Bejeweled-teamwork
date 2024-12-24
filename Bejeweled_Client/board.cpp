#include "board.h"


Board::Board(int j[8][8], QGraphicsScene *sc)
    : m_scene(sc) {
    m_grid.resize(8, std::vector<int>(8));

    //初始化
    m_allJewelItems = std::vector<std::vector<Jewel*>>(8, std::vector<Jewel *>(8, nullptr));

    // 将传入的数组转换为vector
    for (int i = 0; i < 8; ++i) {
        std::vector<int> row;
        for (int k = 0; k < 8; ++k) {
            row.push_back(j[i][k]);
        }
        m_grid.push_back(row);
    }
    generateBoard();  // 生成棋盘

}

void Board::generateBoard() {


    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // 随机生成一个宝石类型
            int gemType = QRandomGenerator::global()->bounded(1, 8);  // 随机生成1到7之间的宝石类型

            // 需要检查该宝石是否符合规则
            while (checkForInvalidPlacement(i, j, gemType)) {
                // 如果生成的宝石在当前位置会造成三颗相同的宝石，重新生成
                gemType = QRandomGenerator::global()->bounded(1, 8);
            }

            m_grid[i][j] = gemType;  // 更新 m_grid 中的宝石类型

            // 创建宝石对象并设置坐标
            Jewel* gem = new Jewel(i, j, gemType, 1);
            connect(gem, &Jewel::jewelSwap, this, &Board::swapJewels);
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

void Board::updateBoard() {
    bool boardValid = false;

    // 继续尝试生成有效的棋盘，直到成功
    while (!boardValid) {
        // 随机生成新的棋盘数据
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                m_grid[i][j] = QRandomGenerator::global()->bounded(1, 8);  // 随机生成1到7之间的宝石类型
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


//宝石交换
void Board::swapJewels(int x1, int y1, int x2, int y2) {

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
    // qDebug() << "1: " << jewel1->getX() << jewel1->getY();

    // qDebug() << "2: " << jewel2->getX() << jewel2->getY();
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
    connect(animationGroup, &QParallelAnimationGroup::finished, [=]() {
        // 交换数据
        std::swap(m_grid[x1][y1], m_grid[x2][y2]);

        qDebug() << "交换前";
        qDebug() << "x1y1:" << m_allJewelItems[x1][y1]->getType();

        qDebug()<< "x2y2:" << m_allJewelItems[x2][y2]->getType();

        // 检查是否形成了有效的消除
        if (checkForMatches()) {




            swapJewelsDestination(jewel1, jewel2);
            qDebug() << "交换完后";
            qDebug() << "x1y1:" << m_allJewelItems[x1][y1]->getType();

            qDebug()<< "x2y2:" << m_allJewelItems[x2][y2]->getType();
            //交换后宝石信息更新


            processMatches(); // 处理消除
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
    // 消除匹配的宝石
    for (auto& match : matches) {
        int x = match.first;
        int y = match.second;

        Jewel* jewel = m_allJewelItems[x][y];
        if (jewel) {
            qDebug() << x << ": " << y;
            QPropertyAnimation* fadeAnim = new QPropertyAnimation(jewel, "opacity");
            fadeAnim->setDuration(100);
            fadeAnim->setEndValue(0.0);
            connect(fadeAnim, &QPropertyAnimation::finished, [=]() {
                m_scene->removeItem(jewel);
                m_allJewelItems[x][y] = nullptr;

                // delete jewel;
            });
            fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }

        m_grid[x][y] = 0;
    }

    qDebug() << "消除完毕";

    QTimer::singleShot(500, this, [=]() {
        dropJewels();
    });
}

//匹配后的宝石下落
void Board::dropJewels() {
    for (int x = 0; x < 8; ++x) {
        for (int y = 7; y >= 0; --y) {
            if (m_grid[x][y] == 0) {
                for (int k = y - 1; k >= 0; --k) {
                    if (m_grid[x][k] != 0) {
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
                            dropAnim->start(QAbstractAnimation::DeleteWhenStopped);


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

    QTimer::singleShot(500, this, [=]() {
        // if (checkForMatches()) {
        //     processMatches(); // 处理消除
        // }
        generateNewJewels();
    });
}

//随机生成新宝石
void Board::generateNewJewels() {

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (m_grid[x][y] == 0) {
                int gemType = QRandomGenerator::global()->bounded(1, 8);
                m_grid[x][y] = gemType;

                Jewel* gem = new Jewel(x, y, gemType, 1);
                //连接交换信号和槽函数
                connect(gem, &Jewel::jewelSwap, this, &Board::swapJewels);
                gem->setPos(QPointF(x * 67 + 252, -100)); // 从上方掉落
                m_scene->addItem(gem);
                m_allJewelItems[x][y] = gem;

                QPropertyAnimation* dropAnim = new QPropertyAnimation(gem, "pos");
                dropAnim->setDuration(300);
                dropAnim->setEndValue(QPointF(x * 67 + 252, y * 68 + 45));

                //处理消除
                // connect(dropAnim, &QPropertyAnimation::finished, [=]() {

                //     if(checkForMatches()) {
                //         processMatches();
                //     }
                // });

                dropAnim->start(QAbstractAnimation::DeleteWhenStopped);

            }
        }
    }

    QTimer::singleShot(500, this, [=]() {
        if (checkForMatches()) {
            processMatches(); // 处理消除
        }
    });
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
