#include "mainwindow.h"
#include "sql.h"
#include <QDir>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;



    // 连接数据库
    sql* mysql = new sql();
    if (!mysql->connectToDatabase()) {
        return -1;
    }


    //测试数据库
    // mysql->canRegisterOrNot("XXXTTT", "123456");

    // mysql->canLoginOrNot("XXXTTT", "345678");

    w.show();
    return a.exec();
}
