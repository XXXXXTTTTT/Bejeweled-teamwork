#include "play.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Play w;

    w.show();
    return a.exec();
}
