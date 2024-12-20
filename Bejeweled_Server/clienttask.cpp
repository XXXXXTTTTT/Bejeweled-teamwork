#include "clienttask.h"


// 初始化静态成员
sql* ClientTask::m_sql = new sql;
QString  ClientTask::m_waitingPlayer = "";
QMutex* ClientTask::m_mutex = new QMutex;
QHash<QString, QTcpSocket *> ClientTask::m_clients;


ClientTask::ClientTask(qintptr socketDescriptor) : m_socketDescriptor(socketDescriptor){
    setAutoDelete(true); // 任务完成后自动删除

    m_sql->connectToDatabase();
}

//析构
// ClientTask::~ClientTask() {

// }

//静态函数销毁所有静态变量(在其头Server析构时调用)
void ClientTask::destoryStaticVar() {
    delete m_sql;
    delete m_mutex;
}


void ClientTask::run() {
    m_socket = new QTcpSocket();
    if (!m_socket->setSocketDescriptor(m_socketDescriptor)) {
        qDebug() << "Failed to set socket descriptor";
        return;
    }

    qDebug() << "Client connected from" << m_socket->peerAddress().toString();

    // m_socket->moveToThread(QThread::currentThread());

    //生成当前uuid
    m_currUuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    //添加至客户端套件组中
    addClient(m_currUuid, m_socket);

    //初始化为登入注册状态
    m_status = -1;

    while (m_socket->state() == QAbstractSocket::ConnectedState) {
        // 检查是否有数据可读
        if (m_socket->waitForReadyRead(5000)) { // 等待5秒，避免线程一直阻塞
            QByteArray data = m_socket->readAll();

            while (data.contains('\n')) {
                //定位第一个'\n'
                int index = data.indexOf('\n');
                QByteArray jsonData = data.left(index);
                data = data.mid(index + 1); // 移除已处理部分

                // 解析 JSON
                QJsonDocument doc = QJsonDocument::fromJson(jsonData);

                if (!doc.isNull() && !doc.isEmpty() && doc.isObject()) {
                    QJsonObject json = doc.object();
                    //处理信息
                    dealWithMsg(json);
                    qDebug() << "Received JSON:" << json;
                } else {
                    qDebug() << "Invalid or EMPTY JSON received.";
                }
            }

        } else {
            qDebug() << "No data received, checking connection.";
            if (m_socket->state() != QAbstractSocket::ConnectedState) {
                qDebug() << "Client disconnected during idle wait.";
                break;
            }
        }
    }

    // 断开连接

    //从客户套件组中删除当前套件
    removeClient(m_currUuid);

    //断开连接
    m_socket->disconnectFromHost();

    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->waitForDisconnected();
    }

    qDebug() << "Client disconnected";
}

//处理信息
void ClientTask::dealWithMsg(const QJsonObject& message) {

    QString type = message["type"].toString();

    // 构造 JSON 响应
    QJsonObject response;

    if(type.compare("MSG") == 0) {
        //仅消息
        qDebug() << "MSG:" <<  message["content"].toString();

    } else if(type.compare("LogIn") == 0) {
        //登入
        QString name = message["name"].toString();
        QString password = message["password"].toString();

        //数据库请求
        int res = m_sql->canLoginOrNot(name, password);

        response["type"] = "LogIn";

        response["res"] = res;

        if(res == 1) {
            //登入成功
            m_userName = name;

            //状态改为空闲中
            m_status = 0;
        } else {
            //登入失败

        }
    } else if(type.compare("Register") == 0) {
        //注册
    } else if(type.compare("Match") == 0) {

        //开始匹配
        matchPlayer(m_userName);

    } else if(type.compare("Playing") == 0) {
        //游戏中
    } else if(type.compare("GameOver") == 0) {
        //游戏结束
    }




    QByteArray responseData = QJsonDocument(response).toJson(QJsonDocument::Compact);

    //添加分隔符
    responseData.append("\n");
    // 发送响应
    m_socket->write(responseData);

    //是否一写入就发送
    // m_socket.flush();

    if (!m_socket->waitForBytesWritten()) {
        qDebug() << "Failed to write data to client.";
    }
}

//发送信息
void ClientTask::sendMsg(const QJsonObject& message, QTcpSocket* targetSocket) {
    if (targetSocket && targetSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = QJsonDocument(message).toJson(QJsonDocument::Compact);

        //添加分隔符
        data.append("\n");

        // 发送响应
        targetSocket->write(data);
        // m_socket->flush();
    } else {
        qDebug() << "Socket not connected. Cannot send message.";
    }
}

//添加客户套件
 void ClientTask::addClient(const QString &clientId, QTcpSocket *socket) {
    QMutexLocker locker(m_mutex);
    m_clients[clientId] = socket;
}

//移除客户套件
 void ClientTask::removeClient(const QString &clientId) {
    QMutexLocker locker(m_mutex);
    m_clients.remove(clientId);
    if (m_waitingPlayer == clientId) {
        m_waitingPlayer.clear();
    }
    qDebug() << "Client removed:" << clientId;
}

//匹配玩家
bool ClientTask::matchPlayer(const QString &clientId) {
    QMutexLocker locker(m_mutex);

    if (m_waitingPlayer.isEmpty()) {
        // 如果没有等待的玩家，设置当前玩家为等待玩家
        m_waitingPlayer = clientId;
        return false;
    } else {
        // 匹配成功，移除等待玩家
        QString m_enemyId = m_waitingPlayer;
        m_waitingPlayer.clear();




        QTcpSocket *player1 = m_clients[clientId];

        QTcpSocket *player2 = m_clients[m_enemyId];

        //向对手发送当前匹配到了玩家并传递玩家id
        QJsonObject json;
        json["type"] = "Matched";

        json["enemyId"] = clientId;

        sendMsg(json, player2);


        if (player1 && player2) {
            qDebug() << "Matched players:" << clientId << "and" << m_enemyId;

            // 创建双向信号连接并记录
            QMetaObject::Connection conn1 = QObject::connect(player1, &QTcpSocket::readyRead, player2, [&]() {
                player2->write(player1->readAll());
            });
            QMetaObject::Connection conn2 = QObject::connect(player2, &QTcpSocket::readyRead, player1, [&]() {
                player1->write(player2->readAll());
            });

            // 保存连接信息
            m_playerConnections[clientId] = conn1;
            m_playerConnections[m_enemyId] = conn2;
        }
        return true;
    }
}


//to do:修改

void ClientTask::endGame(const QString &clientId, const QString &opponentId) {
    QMutexLocker locker(m_mutex);

    // 获取并移除连接
    if (m_playerConnections.contains(clientId)) {
        QObject::disconnect(m_playerConnections[clientId]);
        m_playerConnections.remove(clientId);
    }

    if (m_playerConnections.contains(opponentId)) {
        QObject::disconnect(m_playerConnections[opponentId]);
        m_playerConnections.remove(opponentId);
    }

    qDebug() << "Game ended between" << clientId << "and" << opponentId;
}


