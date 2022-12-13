#include <QtGui/QApplication>
#include "sqldialog.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8") );

    QApplication a(argc, argv);
    SQLDialog w;
    w.show();
    return a.exec();
}
