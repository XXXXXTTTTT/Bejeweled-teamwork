#include "mainwindow.h"
#include "sql.h"

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

    w.show();
    return a.exec();
}
