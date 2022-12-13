#include <QtGui/QApplication>
#include "importerdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImporterDialog w;
    w.show();
    return a.exec();
}
