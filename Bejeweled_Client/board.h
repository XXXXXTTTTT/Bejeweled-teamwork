#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <QGridLayout>
#include <QObject>
#include "Jewel.h"

class Board : public QObject{
public:
    Board(int j[8][8] ,QGraphicsScene *scene);
    void generateBoard();
    void updateBoard(int j[8][8]);

private:
    std::vector<std::vector<int>> m_grid; //存储每个格子中宝石的类型
    QGraphicsScene *m_scene;              //管理和绘制所有宝石
    QGridLayout *m_gridLayout;
    const std::vector<int> m_JewelType = {1,2,3,4,5,6,7}; //7种宝石
    int m_j[8][8];
};

#endif // BOARD_H
