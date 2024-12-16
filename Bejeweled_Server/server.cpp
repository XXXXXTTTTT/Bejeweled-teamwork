#include "server.h"


Server::Server(QObject *parent) : QTcpServer(parent)  {
    m_threadPool.setMaxThreadCount(10); // 设置线程池最大线程数
}

Server::~Server() {

    //调用ClientTask的静态变量销毁函数
    ClientTask::destoryStaticVar();
}

void Server::startServer(quint16 port) {
    if (listen(QHostAddress::Any, port)) {
        //开启成功
        qDebug() << "Server started on port" << port;
    } else {
        //开启失败
        qDebug() << "Failed to start server:" << errorString();
    }


}

//新客户端连接
void Server::incomingConnection(qintptr socketDescriptor){
    qDebug() << "New connection received";
    auto *task = new ClientTask(socketDescriptor);



    m_threadPool.start(task); // 将任务交给线程池处理
}
