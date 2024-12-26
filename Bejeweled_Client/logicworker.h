#ifndef LOGICWORKER_H
#define LOGICWORKER_H

#include <QObject>
#include <QQueue>
#include <functional>
#include <QTimer>

//游戏处理逻辑管理类
class LogicWorker : public QObject {
    Q_OBJECT
public:
    explicit LogicWorker(QObject* parent = nullptr);

    // 添加任务到队列
    void addTask(const std::function<void()>& task);

    // 检查当前是否在进行任务
    bool isProcessingOrNot();

signals:
    void taskFinished(); // 当前任务完成信号

private slots:
    void processNextTask(); // 处理下一个任务

private:
    QQueue<std::function<void()>> taskQueue; // 任务队列
    bool isProcessingTask = false;          // 是否正在处理任务
};

#endif // LOGICWORKER_H
