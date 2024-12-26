#include "sql.h"
#include "information.h"

//初始化静态变量
QReadWriteLock sql::m_lock;

sql::sql() {
    m_connectOrNot = false;
}

sql::~sql() {

    disconnectToDatabase();
}

// 初始化数据库连接
bool sql::connectToDatabase() {


    // 如果已经连接，则直接打开
    if (m_connectOrNot) {
        if (!m_db.isOpen()) {
            m_db.open();
        }
        return true;
    }



    // 获取当前工作目录
    QString currentDir = QDir::currentPath();

    // 切换到项目目录
    QDir dir1(currentDir);
    if (dir1.cdUp()) {

        if (dir1.cdUp()) {
            QDir::setCurrent(dir1.path());
            currentDir = QDir::currentPath();
        } else {
            qDebug() << "无法切换到上一级目录！";
        }
    } else {
        qDebug() << "无法切换到上一级目录！";
    }

    // 生成唯一的连接名
    m_connectionName = QString("connection_%1").arg(QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId())));


    qDebug() << m_connectionName;

    if (QSqlDatabase::contains(m_connectionName)) {
        qDebug() << "数据库连接已经存在";
        return false;
    }

    // 打开 SQLite 数据库
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName); // 使用成员变量 m_db
    m_db.setDatabaseName(currentDir + "/resources/database.db");

    bool ok = m_db.open();
    if (ok) {
        m_connectOrNot = true;
        qDebug() << "数据库连接成功";
        QSqlQuery query(m_db);
        // query.exec("create table new_userList(username varchar(20),password varchar(28))");
        return true;
    } else {
        qDebug() << "数据库连接失败";
        return false;
    }
}


//断开连接
void sql::disconnectToDatabase() {

    if (m_connectOrNot) {
        // 关闭数据库连接
        m_db.close();

        qDebug() << "数据库连接已断开: " << m_connectionName;
    }


    m_connectOrNot = false;

    // 从连接池中移除连接
    QSqlDatabase::removeDatabase(m_connectionName);

}


//注册时检测用户名是否存在(返回1代表可以注册,0代表用户名已存在,-1代表用户名格式不对,-2代表密码格式不对, -3代表查询失败)
int sql::canRegisterOrNot(QString name, QString password) {


    //to do: 检测name和password是否合理
    QReadLocker locker(&m_lock); // 加读锁

    QSqlQuery query(m_db);
    //查询

    query.prepare("SELECT * FROM userList WHERE username = :username");
    query.bindValue(":username", name);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();


        return -3;
    }

    if (query.next()) {
        qDebug() << "Register failed: " << name <<" is used." ;
        return 0;
    } else {
        //无用户名重复
        qDebug() << "Register Successful:" << name;
        //将账号信息保存至数据库中
        saveUserRegisterData(name, password);

        return 1;
    }

}


//保存用户注册的账号信息
void sql::saveUserRegisterData(QString name, QString password) {
    // QWriteLocker locker(&m_lock); // 加写锁
    if (!m_db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    QSqlQuery query(m_db);
    // 获取当前最大 ID

    // query.prepare("SELECT MAX(id) FROM userList");
    // if (!query.exec()) {
    //     qDebug() << "Failed to get max ID:" << query.lastError().text();

    //     return;
    // }

    // int newId = 1; // 默认从 1 开始
    // if (query.next()) {
    //     int maxId = query.value(0).toInt(); // 获取最大 ID
    //     newId = maxId + 1;                 // 新 ID 为最大值 +1
    // }

    // 插入新用户信息
    query.prepare("INSERT INTO userList (username, password) VALUES (:username, :password)");
    // query.bindValue("id", "6");
    query.bindValue(":username", name);
    query.bindValue(":password", password);
    // query.prepare("INSERT INTO userList (username, password) VALUES (?, ?)");
    // query.addBindValue(name);
    // query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Failed to insert user:" << query.lastError().text();

        return;
    }

    qDebug() << "User registered successfully with ID:" ;
}


//登入时检测是否能登入(返回1代表能够登入,0代表用户名或密码错误, -1查询失败)
int sql::canLoginOrNot(QString name, QString password) {



    //若未连接直接返回-1
    if(!m_connectOrNot) return -1;


    QReadLocker locker(&m_lock); // 加写锁

    QSqlQuery query(m_db);


    //查询
    query.prepare("SELECT * FROM userList WHERE username = :username AND password = :password");
    query.bindValue(":username", name.trimmed());
    query.bindValue(":password", password.trimmed());
    // qDebug() << "Executing query: SELECT * FROM userList WHERE username ="
    //          << name << "AND password =" << password;

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        qDebug() << "Login Successful:" << name;
        return 1;
    } else {
        qDebug() << "Login failed: " << "Invalid username or password." ;
        qDebug() << "Last executed query:" << query.lastQuery();
        return 0;
    }


}


//获取对局数据



//保存玩家游戏记录
void sql::savePlayerGameRecord(QString username,int score) {
    QWriteLocker locker(&m_lock); // 加写锁
    QSqlQuery query(m_db);
    int highScore=-1;
    query.prepare("select highScore from userList where username= :username");
    query.bindValue(":username",username);
    if (query.exec()) {
        while (query.next()) {
            highScore = query.value(0).toInt();      // 获取 score 列
            qDebug() << "highScore:" << highScore;
        }
    }
    if(score>highScore)
    {
        query.prepare("UPDATE userList SET highScore = :score WHERE username = :username");
        query.bindValue(":score",score);
        query.bindValue(":username",username);
        if (query.exec()) {
            qDebug() << "Record updated successfully!";
        } else {
            qDebug() << "Update failed:" << query.lastError();
        }
    }
}

void sql::getTopScores()
{
    QSqlQuery query(m_db);
    query.exec("SELECT * FROM userList ORDER BY highScore DESC LIMIT 5");
    QString username="";
    QString highScore="";
    while (query.next()) {

        username += query.value("username").toString();
        highScore += query.value("highScore").toString();  // 假设有 score 列
        username += " ";
        highScore += " ";
        qDebug() << "ID:" << username << " Score:" << highScore;
    }
    information::instance().highScore=highScore;
    information::instance().rankingUserName=username;
}
