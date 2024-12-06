#ifndef SQL_H
#define SQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class sql
{
public:
    sql();

    //析构函数
    ~sql();
    //连接到数据库
    bool connectToDatabase();
    //向其中插入数据
    // void insertParkingRecord(int idOfParkingSpace, QString licensePlate, int time, int fee);

    //
    //从数据库中获取road道路数据
    // void getRoadInformation(QHash<int, road*>& roadList);
    // //从数据库中获取sight景点数据
    // void getSightInformation(QHash<int, sight*>& sightList);
private:
    bool connectOrNot;
    //数据库
    QSqlDatabase db;
};

#endif // SQL_H
