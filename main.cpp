#include "convfilter.h"
#include <QApplication>

// Practice comment

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConvFilter w;
    w.show();

    return a.exec();
}
