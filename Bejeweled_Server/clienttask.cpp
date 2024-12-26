#include "clienttask.h"
#include "QRandomGenerator"
#include "information.h"

// 初始化静态成员
sql* ClientTask::m_sql = new sql;
QString  ClientTask:: m_waitingPlayerName = "";
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
        if (m_socket->waitForReadyRead(0)) { // 等待5秒，避免线程一直阻塞//魏贤炀修改为:不等待
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
                    qDebug() << "Received JSON:" << json;
                    dealWithMsg(json);

                } else {
                    qDebug() << "Invalid or EMPTY JSON received.";
                }
            }

        } else {
            // qDebug() << "No data received, checking connection.";
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

            m_sql->getTopScores();
            response["rankName"]=information::instance().rankingUserName;
            response["rankScore"]=information::instance().highScore;
            information::instance().m_easyPlayerName="";
        } else {
            //登入失败

        }
    } else if(type.compare("Register") == 0) {
        //注册
        QString name = message["name"].toString();
        QString password = message["password"].toString();
        int res =m_sql->canRegisterOrNot(name,password);
        response["type"] = "Register";
        response["res"] = res;

        if(res == 1) {
            //注册成功
        }
        else if(res==0)
        {
            // 用户名已存在
        }
    } else if(type.compare("Match") == 0) {
        information::instance().m_RRange= 8;
        int res =matchPlayer(m_currUuid);
        response["type"] = "Match";
        response["res"] = res;
        //开始匹配
        if(res)
        {
            //匹配成功

        }
        else
        {
            //进入匹配
        }

    } else if(type.compare("game") == 0) {
        //游戏中
        response["type"] = "game";
        response["score"] = message["score"];
        response["name"]= m_userName;

    } else if(type.compare("end") == 0) {
        //游戏结束
        m_sql->savePlayerGameRecord(message["username"].toString(),message["score"].toInt());
        response["type"] = "end";
        response["score"] = message["score"];
        response["username"]= message["username"];
    }
    else if(type.compare("mode") == 0) {
        //改模式
        response["type"] = "mode";

        information::instance().m_RRange= message["mode"].toInt();
        QString r;
        for(int i=0;i<1000;i++)
        {
            r+=QString::number(QRandomGenerator::global()->bounded(1, information::instance().m_RRange));
        }
        response["random"]=r;
    }





    QByteArray responseData = QJsonDocument(response).toJson(QJsonDocument::Compact);

    //添加分隔符
    responseData.append("\n");
    // 发送响应
    if(type.compare("Match") != 0&&type.compare("game") != 0)
    {
        m_socket->write(responseData);
    }
    else if(type.compare("game") == 0)
    {
        sendMsg(response,m_clients[information::instance().m_enemyId]);
        qDebug()<<"m_enemyId="<<information::instance().m_enemyId;
        sendMsg(response,m_clients[information::instance().m_userId]);
        qDebug()<<"m_userId="<<information::instance().m_userId;
        qDebug()<<"m_currUuid="<<m_currUuid;

    }
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

    //生成随机数字串
    QString r="";
    for(int i=0;i<1000;i++)
    {
        r+=QString::number(QRandomGenerator::global()->bounded(1, information::instance().m_RRange));
    }

    if (m_waitingPlayer.isEmpty()) {
        // 如果没有等待的玩家，设置当前玩家为等待玩家
        m_waitingPlayer = clientId;
        m_waitingPlayerName = m_userName;
        return false;
    } else {
        // 匹配成功，移除等待玩家
        information::instance().m_enemyId = m_waitingPlayer;
        m_waitingPlayer.clear();

        information::instance().m_userId=clientId;

        QTcpSocket *player1 = m_clients[clientId];
        QTcpSocket *player2 = m_clients[information::instance().m_enemyId];

        QJsonObject json0;
        json0["type"] = "Match";
        json0["enemyId"] = m_waitingPlayerName;
        json0["res"]=1;
        json0["random"]=r;


        sendMsg(json0, player1);
        //向对手发送当前匹配到了玩家并传递玩家id
        QJsonObject json;
        json["type"] = "Match";
        json["enemyId"] = m_userName;
        json["res"]=1;
        json["random"]=r;


        sendMsg(json, player2);


        // if (player1 && player2) {
        //     qDebug() << "Matched players:" << clientId << "and" << m_enemyId;

        //     // 创建双向信号连接并记录
        //     QMetaObject::Connection conn1 = QObject::connect(player1, &QTcpSocket::readyRead, player2, [&]() {
        //         qDebug() <<"player2->write(player1->readAll());";
        //         player2->write(player1->readAll());
        //     });
        //     QMetaObject::Connection conn2 = QObject::connect(player2, &QTcpSocket::readyRead, player1, [&]() {
        //         qDebug() <<"player1->write(player2->readAll());";
        //         player1->write(player2->readAll());
        //     });

        //     // 保存连接信息
        //     m_playerConnections[clientId] = conn1;
        //     m_playerConnections[m_enemyId] = conn2;
        // }
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


