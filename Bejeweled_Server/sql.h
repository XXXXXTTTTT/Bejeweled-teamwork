#ifndef SQL_H
#define SQL_H

#include <QSqlDatabase>
#include <QThread>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QReadWriteLock>

class sql
{
public:
    sql();

    //析构函数
    ~sql();
    //连接到数据库
    bool connectToDatabase();

    //断开连接
    void disconnectToDatabase();
    //注册时检测用户名是否存在(返回1代表可以注册,0代表用户名已存在,-1代表用户名格式不对,-2代表密码格式不对)
    int canRegisterOrNot(QString name, QString password);
    //保存用户注册的账号信息
    void saveUserRegisterData(QString name, QString password);
    //登入时检测是否能登入(返回1代表能够登入,0代表密码错误, -1代表用户名不存在)
    int canLoginOrNot(QString name, QString password);

    //获取对局数据

    //保存玩家游戏记录
    void savePlayerGameRecord(QString name, int score);

    void getTopScores();
private:
    bool m_connectOrNot;
    //数据库
    QSqlDatabase m_db;

    //当前连接名
    QString m_connectionName;

    static QReadWriteLock m_lock;
};

#endif // SQL_H
