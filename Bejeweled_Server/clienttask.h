#ifndef CLIENTTASK_H
#define CLIENTTASK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QRunnable>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
#include <QMutex>
#include <QDebug>
#include <QThread>
#include <sql.h>

//处理客户端任务
class ClientTask : public QRunnable
{
public:
    explicit ClientTask(qintptr socketDescriptor);

    // ~ClientTask();
    void run() override;

    //处理信息
    void dealWithMsg(const QJsonObject& message);

    //添加Client
    static void addClient(const QString &clientId, QTcpSocket *socket);

    //移除Client
    static void removeClient(const QString &clientId);

    //Client匹配
    bool matchPlayer(const QString &clientId);

    //结束游戏
    void endGame(const QString &clientId, const QString &opponentId);

    //发送信息
    void sendMsg(const QJsonObject& message,QTcpSocket* targetSocket);

    //静态函数销毁所有静态变量(在其头Server析构时调用)
    static void destoryStaticVar();

private:
    //套件初始化要素
    qintptr m_socketDescriptor;

    //当前套件
    QTcpSocket* m_socket;

    //当前套件uuid
    QString m_currUuid;

    //当前用户名
    QString m_userName;

    //当前状态(-1表在注册或登入,0表空闲中, 1表匹配中, 2表游戏中)
    int m_status;


    // 管理当前玩家与其他玩家的连接
    QMap<QString, QMetaObject::Connection> m_playerConnections;





    // 静态变量：所有 ClientTask 共享

    //数据库
    static sql *m_sql;

    // 客户端管理
    static QHash<QString, QTcpSocket *> m_clients;

    // 等待中的玩家
    static QString m_waitingPlayerName;
    static QString m_waitingPlayer;

    // 线程安全
    static QMutex* m_mutex;
};

#endif // CLIENTTASK_H
