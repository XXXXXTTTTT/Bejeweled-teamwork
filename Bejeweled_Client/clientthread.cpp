#include "clientthread.h"
#include "information.h"
#include "board.h"
ClientThread::ClientThread(const QString& host, quint16 port, QObject* parent)
    : QThread(parent), code(0), m_socket(nullptr), m_host(host),m_port(port)
{
    qDebug()<<"****************************\nconstructor called!";
}

QString ClientThread::m_ran="";
    ClientThread::~ClientThread() {
    if (m_socket) {
            qDebug()<<"****************************\ndestructor called!";
        m_socket->disconnectFromHost();
        m_socket->waitForDisconnected();
        delete m_socket;
    }
}
    ClientThread &ClientThread::instance()
    {

        static ClientThread instance("127.0.0.1", 12345, nullptr);
        // static ClientThread instance("172.20.10.7", 12345, nullptr);
        return instance;
    }
void ClientThread::run(){
    m_socket = new QTcpSocket();

    //连接信号与槽函数
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientThread::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientThread::onDisconnected);
    // connect(this, &ClientThread::receivedMessage, this, &ClientThread::dealWithMsg);

    // 连接到服务器
    m_socket->connectToHost(m_host, m_port);

    if (!m_socket->waitForConnected(5000)) { // 等待5秒连接
        qDebug() << "Failed to connect to server:" << m_socket->errorString();
        emit connectionFailed(m_socket->errorString());
        return;
    }

    qDebug() << "Connected to server";

    exec();
}

void ClientThread::sendMsg(const QJsonObject& message) {
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        // qDebug()<<"Send";
        QByteArray data = QJsonDocument(message).toJson(QJsonDocument::Compact);

        //添加分隔符
        data.append("\n");
        // 发送响应
        qint64 bytesWritten = m_socket->write(data);
        qDebug() << "Bytes written:" << bytesWritten;


        m_socket->flush();
    } else {
        qDebug() << "Socket not connected. Current state:" << m_socket->state();
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
            receivedMessage(json);
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
        m_res = message["res"].toInt();

        //登入
        if(m_res == 1) {
            //登入成功
            qDebug() << "登入成功";
        } else {
            //登入失败
            if(m_res == 0) {
                qDebug() << "用户名或密码错误";
            } else if(m_res == -1) {
                qDebug() << "查询失败";
            }
        }
    }
    else if(type.compare("Register") == 0)
    {
        m_res = message["res"].toInt();

        //注册
        if(m_res == 1) {
            //注册成功
            qDebug() << "注册成功";
        }
        //失败
        else if(m_res==0)
        {
            qDebug() << "注册失败：用户名已被使用";
        }
    }
    else if(type.compare("Match") == 0)
    {
        //点击开始匹配后的相应

    }
}

void ClientThread::onDisconnected() {
    qDebug() << "Server disconnected";
    emit disconnected();
    quit(); // 退出线程事件循环
}
void ClientThread::receivedMessage(const QJsonObject& message)
{

    QString type = message["type"].toString();
    // 解析响应并更新 m_res
    m_res = message["res"].toInt();
    //匹配
    if(type=="Match")
    {
        if(m_res==1&&message.contains("enemyId"))
        {
            m_ran=message["random"].toString();
            emit matchReceived(message["enemyId"].toString());
        }
    }
    else if(type=="game")
    {
        if(message["name"].toString().compare(information::instance().m_enemyName)==0)
        {
            qDebug()<<"message[name].toString()="<<message["name"].toString()<<" "<<"information::instance().m_enemyName)="<<information::instance().m_enemyName;
            information::instance().m_enemyScore=message["score"].toInt();
            emit scoreChanged(information::instance().m_enemyScore);
        }
    }
    else if(type=="mode")
    {
        information::instance().m_r=message["random"].toString();
        emit matchReceived("robot");
    }
    //注册与登录
    else
    {
        if(type=="LogIn")
        {
            information::instance().rankingUserName = message["rankName"].toString();
            information::instance().highScore = message["rankScore"].toString();
        }
        // 发射信号通知界面更新 UI
        emit resultReceived(m_res);
    }

}
