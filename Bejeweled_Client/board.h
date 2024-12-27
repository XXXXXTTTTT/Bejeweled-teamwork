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
#include <algorithm>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageBox>
#include "Jewel.h"
#include "logicworker.h"
#include "music.h"
#include "vortexeffect.h"
#include "information.h"
#include "music.h"

class Board : public QObject{
    Q_OBJECT
public:
    Board(QString r , QGraphicsScene *scene);

    ~Board();

    //产生棋盘
    void generateBoard(QString &r);
    //刷新棋盘
    void updateBoard();

    //生成新宝石并设置到对应位置(x,y表其在棋盘中的位置,gemType表宝石种类,state表示宝石是什么状态:0表示直接出现在棋盘相应位置,1表示从上落下)
    Jewel* setNewJewelInformation(int x, int y, int gemType, int state);

    //检测宝石位置是否不合法
    bool checkForInvalidPlacement(int x, int y, int gemType);
    //获取宝石种类
    int getJewelType(int x, int y) const;

    //检测当前是否还有可消除操作
    bool isAvailableOrNot();

    //给予提示
    void giveHint();

signals:
    //int getEli();
    // 添加任务信号
    void enqueueTask(const std::function<void()>& task);

    void scoreUpdated(int score);  //发射得分信号

private:
//私有函数部分

    bool isBoardValid();  //棋盘是否合理

    void clearBoard();   // 清空棋盘

    QSet<std::pair<int, int>> matches;
    int checkHorizontal(int x, int y);//检查水平

    int checkVertical(int x, int y);//检查竖直

//游戏逻辑任务
    //宝石交换任务
    void swapJewels(int x1, int y1, int x2, int y2);

    //检查是否有可消除的宝石
    bool checkForMatches();

    bool checkForChains();

    //删除匹配的宝石任务(第一个参数若交换时有用魔力宝石则为魔力宝石,第二个参数为另一个交换的宝石)
    void processMatches(Jewel *mightMagicJewel = nullptr, Jewel *normalSwappedJewel = nullptr);

    //匹配后的宝石下落任务
    void dropJewels();

    //随机生成新宝石任务
    void generateNewJewels();

    //根据索引查找当前宝石items
    // Jewel* findJewelAt(int x, int y);

    //交换宝石位置信息
    void swapJewelsDestination(Jewel* j1,Jewel* j2);

    void calculateScore(int matchCount ,bool isChain);
private slots:
    //任务完成后处理下一个
    void handleTaskFinished();
    //交换宝石动作
    void enqueueSwap(int x1, int y1, int x2, int y2);

public:
    music * m_mus;

    int m_combo;
private:


    int currentChain = 0;  // 当前连锁次数


    int currentScore = 0; // 当前回合的得分

    std::vector<std::vector<int>> m_grid; //存储每个格子中宝石的类型

    QGraphicsScene *m_scene;              //管理和绘制所有宝石

    std::vector<std::vector<Jewel*>> m_allJewelItems; //每个位置的宝石管理

    QGridLayout *m_gridLayout;

    const std::vector<int> m_JewelType = {1,2,3,4,5,6,7}; //7种宝石




    QPair<int,int> m_currAvailablePos; // 当前可交换位置

    const int offsetX = 252;  // X轴偏移量
    const int offsetY = 45;   // Y轴偏移量

    //线程锁
    QMutex m_mutex;

    //游戏逻辑线程
    QThread* m_logicThread;

    //游戏逻辑任务分配
    LogicWorker* m_logicWorker;
};


#endif // BOARD_H
