#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <QGridLayout>
#include <QObject>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QRandomGenerator>// 用于 time()
#include "Jewel.h"

class Board : public QObject{
public:
    Board(int j[8][8] ,QGraphicsScene *scene);
    void generateBoard();
    void updateBoard();
    bool checkForInvalidPlacement(int x, int y, int gemType);
    int getJewelType(int x, int y) const;

private:
    std::vector<std::vector<int>> m_grid; //存储每个格子中宝石的类型

    QGraphicsScene *m_scene;              //管理和绘制所有宝石

    std::vector<std::vector<Jewel*>> m_allJewelItems; //每个位置的宝石管理

    QGridLayout *m_gridLayout;

    const std::vector<int> m_JewelType = {1,2,3,4,5,6,7}; //7种宝石

    int m_j[8][8];

    const int offsetX = 252;  // X轴偏移量
    const int offsetY = 45;   // Y轴偏移量

    bool isBoardValid();
    void clearBoard();   // 清空棋盘

    bool checkHorizontal(int x, int y);
    bool checkVertical(int x, int y);

    //检查是否有可消除的宝石
    bool checkForMatches();

    //删除匹配的宝石
    void processMatches();

    //匹配后的宝石下落
    void dropJewels();

    //随机生成新宝石
    void generateNewJewels();

    //根据索引查找当前宝石items
    // Jewel* findJewelAt(int x, int y);


    //交换宝石位置信息
    void swapJewelsDestination(Jewel* j1,Jewel* j2);
private slots:

    //交换宝石
    void swapJewels(int x1, int y1, int x2, int y2);
};


#endif // BOARD_H
