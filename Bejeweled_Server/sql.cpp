#include "sql.h"

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
    query.prepare("SELECT * FROM userList WHERE name = :username");
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
    QWriteLocker locker(&m_lock); // 加写锁
    QSqlQuery query(m_db);
    // 获取当前最大 ID
    query.prepare("SELECT MAX(id) FROM users");
    if (!query.exec()) {
        qDebug() << "Failed to get max ID:" << query.lastError().text();

        return;
    }

    int newId = 1; // 默认从 1 开始
    if (query.next()) {
        int maxId = query.value(0).toInt(); // 获取最大 ID
        newId = maxId + 1;                 // 新 ID 为最大值 +1
    }

    // 插入新用户信息
    query.prepare("INSERT INTO users (id, username, password) VALUES (:id, :username, :password)");
    query.bindValue(":id", newId);
    query.bindValue(":username", name);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Failed to insert user:" << query.lastError().text();

        return;
    }

    qDebug() << "User registered successfully with ID:" << newId;
}


//登入时检测是否能登入(返回1代表能够登入,0代表用户名或密码错误, -1查询失败)
int sql::canLoginOrNot(QString name, QString password) {



    //若未连接直接返回-1
    if(!m_connectOrNot) return -1;


    QReadLocker locker(&m_lock); // 加写锁

    QSqlQuery query(m_db);

    //查询
    query.prepare("SELECT * FROM userList WHERE name = :username AND password = :password");
    query.bindValue(":username", name);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        qDebug() << "Login Successful:" << name;
        return 1;
    } else {
        qDebug() << "Login failed: " << "Invalid username or password." ;
        return 0;
    }


}


//获取对局数据



//保存玩家游戏记录
void sql::savePlayerGameRecord() {
    QWriteLocker locker(&m_lock); // 加写锁
}

//保存游戏记录
// void sql::insertParkingRecord(int pos, QString licensePlate, int time, int fees) {
//     QSqlQuery query;

//     // 插入记录的 SQL 语句
//     query.prepare("INSERT INTO cardata (licensePlate, pos, time, fees) "
//                   "VALUES (:licensePlate, :pos, :time, :fees)");

//     // 绑定参数

//     query.bindValue(":licensePlate", licensePlate);
//     query.bindValue(":pos", pos);
//     query.bindValue(":time", time);
//     query.bindValue(":fees", fees);

//     // 执行插入操作
//     if (!query.exec()) {
//         qDebug() << "Failed to insert parking record:" << query.lastError().text();
//     } else {
//         qDebug() << "Parking record inserted successfully!";
//     }

// }

//注册时检测

//登入时检测


// int* sql::getTimeData() {
//     // 从数据库中获取停车时间
//     QSqlQuery query;
//     query.exec("SELECT time FROM cardata");

//     //假设车辆在100min内离开
//     int *timeData = new int[101] {0};
//     // 记录每个停车时间出现的次数
//     while (query.next()) {
//         int time = query.value(0).toInt();
//         timeData[time]++;
//     }

//     return timeData;
// }


//从数据库中获取road道路数据(返回road个数)
// void sql::getRoadInformation(QHash<int, road*> &roadList) {
//     // 从数据库中获取停车时间
//     QSqlQuery query;
//     if (!query.exec("SELECT * FROM road")) {
//         qDebug() << "Query execution failed:" << query.lastError();
//         return;
//     }

//     while (query.next()) {

//         int id = query.value(0).toInt();
//         qDebug() << query.value(1);
//         QString name= QString::fromUtf8(query.value(1).toByteArray());
//         int endPoint1= query.value(2).toInt();
//         int endPoint2= query.value(3).toInt();
//         int distance= query.value(4).toInt();

//         //添加到道路列表中
//         roadList.insert(id,new road(id, name,  endPoint1, endPoint2, distance));

//         qDebug() << id << name << endPoint1 << endPoint2 << distance;
//     }

// }


// //从数据库中获取sight景点数据(返回sight个数)
// void sql::getSightInformation(QHash<int, sight*> &sightList) {
//     QSqlQuery query;
//     if (!query.exec("SELECT * FROM sight")) {
//         qDebug() << "Query execution failed:" << query.lastError();
//         return;
//     }

//     while (query.next()) {

//         int id = query.value(0).toInt();
//         QString name= QString::fromUtf8(query.value(1).toByteArray());

//         double lon= strtod(QString::number(query.value(2).toDouble(), 'f', 6).toLocal8Bit().data(),nullptr);

//         double lat= (QString::number(query.value(3).toDouble(), 'f', 6)).toUtf8().toDouble();

//         // qDebug() << "LAT1" << query.value(2);

//         // qDebug() << "LAT1" << query.value(2).toDouble();
//         // qDebug() << "LAT2" << QString::number(query.value(2).toDouble(), 'f', 6);

//         // qDebug() << "LAT3" << QString::number(query.value(2).toDouble(), 'f', 6).toUtf8();


//         // qDebug() << lon;
//         // QString temp = "1.56461561159616";
//         // double d = strtod(temp.toLocal8Bit().data(),nullptr);
//         // qDebug() << "temp:" << temp << "  d:" << d;
//         QString information = QString::fromUtf8(query.value(4).toByteArray());

//         QString imagePath = QString::fromUtf8(query.value(5).toByteArray());
//         //添加到道路列表中
//         sightList.insert(id,new sight(id, name,  lon, lat, information,imagePath));

//         // qDebug() << id << name << lat << lon << information;
//     }

// }
