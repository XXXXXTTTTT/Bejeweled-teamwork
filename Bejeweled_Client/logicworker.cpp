#include "logicworker.h"
#include "qdebug.h"


LogicWorker::LogicWorker(QObject* parent)
    : QObject(parent), isProcessingTask(false) {}

void LogicWorker::addTask(const std::function<void()>& task) {
    taskQueue.enqueue(task);

    // 如果当前没有正在处理任务，则开始处理队列
    if (!isProcessingTask) {
        processNextTask();
    }
}

bool LogicWorker::isProcessingOrNot() {
    return isProcessingTask;
}

void LogicWorker::processNextTask() {
    // 如果队列为空，直接返回
    if (taskQueue.isEmpty()) {
        isProcessingTask = false;
        return;
    }

    // 设置任务处理状态为 true
    isProcessingTask = true;

    // 取出队列中的任务并执行
    auto task = taskQueue.dequeue();
    task();

    // 延迟一会后后标记任务完成，并处理下一个任务
    QTimer::singleShot(300, this, [this]() {
        emit taskFinished();
        processNextTask(); // 处理下一个任务
    });
}

