#include "qmedia.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMedia w;
    w.show();

    return a.exec();
}
