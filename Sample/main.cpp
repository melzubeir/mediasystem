#include <QtGui/QApplication>
#include "clippingwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClippingWindow w;
    w.show();
    return a.exec();
}
