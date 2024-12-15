#include "clientthread.h"
ClientThread::ClientThread(const QString& host, quint16 port, QObject* parent)
    : QThread(parent), m_host(host), m_port(port), m_socket(nullptr) {}

    ClientThread::~ClientThread() {
    if (m_socket) {
        m_socket->disconnectFromHost();
        m_socket->waitForDisconnected();
        delete m_socket;
    }
}

void ClientThread::run(){
    m_socket = new QTcpSocket();

    //连接信号与槽函数
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientThread::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientThread::onDisconnected);
    connect(this, &ClientThread::receivedMessage, this, &ClientThread::dealWithMsg);

    // 连接到服务器
    m_socket->connectToHost(m_host, m_port);

    if (!m_socket->waitForConnected(5000)) { // 等待5秒连接
        qDebug() << "Failed to connect to server:" << m_socket->errorString();
        emit connectionFailed(m_socket->errorString());
        return;
    }

    qDebug() << "Connected to server";

    //测试


    QJsonObject json1;
    json1["type"] = "MSG";

    json1["content"] = "你好";
    sendMsg(json1);

    QJsonObject json2;
    json2["type"] = "LogIn";

    json2["name"] = "XXXTTT";

    json2["password"] = "345678";
    sendMsg(json2);

    QJsonObject json3;
    json3["type"] = "LogIn";

    json3["name"] = "XXXTTT";

    json3["password"] = "123456";
    sendMsg(json3);

    // 进入事件循环
    exec();
}

void ClientThread::sendMsg(const QJsonObject& message) {
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = QJsonDocument(message).toJson(QJsonDocument::Compact);

        //添加分隔符
        data.append("\n");
        // 发送响应
        m_socket->write(data);
        // m_socket->flush();
    } else {
        qDebug() << "Socket not connected. Cannot send message.";
    }
}

void ClientThread::onReadyRead() {
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

    // qDebug() << "Received:" << data;
}

//处理接收信息
void ClientThread::dealWithMsg(const QJsonObject& message) {
    QString type = message["type"].toString();

    if(type.compare("LogIn") == 0) {
        int res = message["res"].toInt();

        //登入
        if(res == 1) {
            //登入成功
            qDebug() << "登入成功";
        } else {
            //登入失败
            if(res == 0) {
                qDebug() << "用户名或密码错误";
            } else if(res == -1) {
                qDebug() << "查询失败";
            }
        }
    } else if(type.compare("Register") == 0) {
        //注册
    } else if(type.compare("Match") == 0) {
        //点击开始匹配后的相应

    } else if(type.compare("Matched") == 0) {
        //匹配成功

        //获取对手名
        QString enemyId = message["enemyId"].toString();

    }
}

void ClientThread::onDisconnected() {
    qDebug() << "Server disconnected";
    emit disconnected();
    quit(); // 退出线程事件循环
}
