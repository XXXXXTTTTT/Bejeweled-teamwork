#include "sql.h"



sql::sql() {
    connectOrNot = false;
}

sql::~sql() {
    //若连接成功则断开连接
    if(connectOrNot) {
        db.close();
    }

}

// 初始化数据库连接
bool sql::connectToDatabase() {
    db = (QSqlDatabase::addDatabase("QODBC"));
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("2");
    db.setUserName("root");
    db.setPassword("123456");
    // db.setDatabaseName("DRIVER={MySQL ODBC 8.0 Unicode Driver};SERVER=127.0.0.1;DATABASE=1;UID=root;PWD=123456;CHARSET=UTF8;");
    bool ok = db.open();
    if (ok){
        connectOrNot = true;
        QMessageBox::information(nullptr, "infor", "数据库连接成功");
        return true;
    }
    else {
        QMessageBox::information(nullptr, "infor", "数据库连接失败");

        return false;
        // qDebug()<<"error open database because"<<db.lastError().text();
    }
}

// 插入停车记录
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

//从数据库中获取time数据
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
