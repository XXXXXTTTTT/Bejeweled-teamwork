#include "board.h"
#include <QRandomGenerator>// 用于 time()

Board::Board(int j[8][8], QGraphicsScene *sc)
    : m_scene(sc) {
    m_grid.resize(8, std::vector<int>(8));
    generateBoard();  // 生成棋盘

}

void Board::generateBoard() {
    int offsetX = 250;  // X轴偏移量
    int offsetY = 40;   // Y轴偏移量

    // 遍历棋盘的每个格子
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // 随机生成宝石类型
            int gemType = QRandomGenerator::global()->bounded(1, 8);  // 生成1到7之间的随机数

            // 创建宝石对象
            Jewel* gem = new Jewel(i, j, gemType, 1);  // 创建宝石

            // 设置宝石的坐标
            gem->setPos(i * 67 + offsetX, j * 68 + offsetY);  // 设置宝石的坐标位置

            // 将宝石添加到场景中
            m_scene->addItem(gem);
        }
    }
}


void Board::updateBoard(int a[8][8]) {
    int i,j;
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            if(a[i][j]==0 || a[i][j] == -1){
                a[i][j]=rand()%7+1;
            }
        }
    }
}
