#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

//单例模式客户端线程类
class ClientThread : public QThread {
    Q_OBJECT

public:
    //获取线程单例
    static ClientThread &instance();

    ~ClientThread();
protected:
    void run() override;


public slots:
    void sendMsg(const QJsonObject& message);

    void dealWithMsg(const QJsonObject& message);
signals:
    void scoreChanged(int newValue);
    void resultReceived(int res);
    void matchReceived(QString name);

    void connectionFailed(const QString& error);

    void disconnected();

private slots:
    void onReadyRead();

    void onDisconnected();
private:
    void receivedMessage(const QJsonObject& message);
    ClientThread(const QString& host, quint16 port, QObject* parent = nullptr);
public:
    int m_res;
    int code;
    QTcpSocket* m_socket;
    static QString m_ran;
private:
    QString m_host;
    quint16 m_port;


};


#endif // CLIENTTHREAD_H
