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
    int m_res;
QTcpSocket* m_socket;
    ~ClientThread();
static QString m_ran;
static ClientThread *instance();
protected:
    void run() override;

public slots:
    void sendMsg(const QJsonObject& message);

    void dealWithMsg(const QJsonObject& message);
signals:
    void resultReceived(int res);
    void matchReceived(QString name);

    void connectionFailed(const QString& error);

    void disconnected();

private slots:
    void onReadyRead();

    void onDisconnected();

private:
    void receivedMessage(const QJsonObject& message);
    explicit ClientThread(const QString& host, quint16 port, QObject* parent = nullptr);
    static ClientThread * m_instance;
    QString m_host;
    quint16 m_port;


};


#endif // CLIENTTHREAD_H
