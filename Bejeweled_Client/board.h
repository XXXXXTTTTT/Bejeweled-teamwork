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
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageBox>
#include "Jewel.h"
#include "logicworker.h"
#include "music.h"

class Board : public QObject{
    Q_OBJECT
public:
    music * m_mus;
    int m_combo;
    Board(QString r , QGraphicsScene *scene);
    Board(int j[8][8] ,QGraphicsScene *scene);
    ~Board();
    //产生棋盘
    void generateBoard();
    void generateBoard(QString &r);
    //刷新棋盘
    void updateBoard();

    //检测宝石位置是否不合法
    bool checkForInvalidPlacement(int x, int y, int gemType);
    //获取宝石种类
    int getJewelType(int x, int y) const;

    //检测当前是否还有可消除操作
    bool isAvailableOrNot();

    //给予提示
    void giveHint();

signals:
    // 添加任务信号
    void enqueueTask(const std::function<void()>& task);
private:
    std::vector<std::vector<int>> m_grid; //存储每个格子中宝石的类型

    QGraphicsScene *m_scene;              //管理和绘制所有宝石

    std::vector<std::vector<Jewel*>> m_allJewelItems; //每个位置的宝石管理

    QGridLayout *m_gridLayout;

    const std::vector<int> m_JewelType = {1,2,3,4,5,6,7}; //7种宝石


    int m_j[8][8];

    const int offsetX = 252;  // X轴偏移量
    const int offsetY = 45;   // Y轴偏移量

    //线程锁
    QMutex m_mutex;

    //游戏逻辑线程
    QThread* m_logicThread;

    //游戏逻辑任务分配
    LogicWorker* m_logicWorker;

//私有函数部分

    bool isBoardValid();  //棋盘是否合理
    void clearBoard();   // 清空棋盘

    bool checkHorizontal(int x, int y);
    bool checkVertical(int x, int y);

//游戏逻辑任务
    //宝石交换任务
    void swapJewels(int x1, int y1, int x2, int y2);

    //检查是否有可消除的宝石
    bool checkForMatches();

    //删除匹配的宝石任务
    void processMatches();

    //匹配后的宝石下落任务
    void dropJewels();

    //随机生成新宝石任务
    void generateNewJewels();

    //根据索引查找当前宝石items
    // Jewel* findJewelAt(int x, int y);


    //交换宝石位置信息
    void swapJewelsDestination(Jewel* j1,Jewel* j2);
private slots:
    //任务完成后处理下一个
    void handleTaskFinished();
    //交换宝石动作
    void enqueueSwap(int x1, int y1, int x2, int y2);
};


#endif // BOARD_H
