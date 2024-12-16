#ifndef SERVER_H
#define SERVER_H

#include "clienttask.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRunnable>
#include <QThreadPool>
#include <QDebug>

//服务端类
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent  = nullptr);

    ~Server();
    //启动Server端
    void startServer(quint16 port);
protected:
    //用户端连接
    void incomingConnection(qintptr socketDescriptor) override;

private:

    //客户端连接线程池
    QThreadPool m_threadPool;

};

#endif // SERVER_H
