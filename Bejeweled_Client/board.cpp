#include "board.h"
#include <QRandomGenerator>// 用于 time()

Board::Board(int j[8][8], QGraphicsScene *sc)
    : m_scene(sc) {
    m_grid.resize(8, std::vector<int>(8));

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
    int offsetX = 252;  // X轴偏移量
    int offsetY = 45;   // Y轴偏移量

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
            gem->setPos(i * 67 + offsetX, j * 68 + offsetY);
            m_scene->addItem(gem);  // 将宝石添加到场景中
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
    for (auto item : items) {
        m_scene->removeItem(item);
        delete item;  // 删除宝石对象
    }

    // 重置棋盘数据
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            m_grid[i][j] = 0;  // 将棋盘上的所有宝石类型清空
        }
    }
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



