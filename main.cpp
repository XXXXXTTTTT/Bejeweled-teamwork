#include "mainwindow.h"
#include "sql.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // 连接数据库
    // sql* mysql = new sql();
    // if (!mysql->connectToDatabase()) {
    //     return -1;
    // }

    // 1. 打开 SQLite 数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db"); // 数据库文件路径

    if (!db.open()) {
        qDebug() << "无法打开数据库：" << db.lastError().text();
        return -1;
    }
    qDebug() << "成功连接到 SQLite 数据库";

    // 2. 创建表
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS userList ("
                    "id INTEGER COLLATE BINARY,"
                    "name TEXT,"
                    "password TEXT)")) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return -1;
    }

    // 3. 插入数据
    if (!query.exec("INSERT INTO userList (id, name, password) VALUES (3, 'Man', '89716')")) {
        qDebug() << "插入数据失败：" << query.lastError().text();
    } else {
        qDebug() << "插入数据成功";
    }

    // 4. 查询数据
    if (!query.exec("SELECT * FROM userList")) {
        qDebug() << "查询数据失败：" << query.lastError().text();
    } else {
        while (query.next()) {
            qDebug() << "ID:" << query.value("id").toInt()
            << "Name:" << query.value("name").toString()
            << "Password:" << query.value("password").toString();
        }
    }

    db.close();

    w.show();
    return a.exec();
}
