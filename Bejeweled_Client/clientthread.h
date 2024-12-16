#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class ClientThread : public QThread {
    Q_OBJECT

public:
    explicit ClientThread(const QString& host, quint16 port, QObject* parent = nullptr);

    ~ClientThread();

protected:
    void run() override;

public slots:
    void sendMsg(const QJsonObject& message);

    void dealWithMsg(const QJsonObject& message);
signals:
    void receivedMessage(const QJsonObject& message);

    void connectionFailed(const QString& error);

    void disconnected();

private slots:
    void onReadyRead();

    void onDisconnected();

private:
    QString m_host;
    quint16 m_port;
    QTcpSocket* m_socket;
};


#endif // CLIENTTHREAD_H
