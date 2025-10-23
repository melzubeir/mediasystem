/************************************************************************
 * $Id: main.cpp 667 2011-05-15 10:41:45Z elzubeir $
 *
 * ------------
 * Description:
 * ------------
 *
 * (C) Copyright 2009-2011 ALLCONTENT. All rights reserved.
 *
 * -----------------
 * Revision Details:    (Updated by Revision Control System)
 * -----------------
 *  $Date: 2011-05-15 14:41:45 +0400 (Sun, 15 May 2011) $
 *  $Author: elzubeir $
 *  $Revision: 667 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/main.cpp $
 *
 ************************************************************************/

#include <QApplication>
#include <QPluginLoader>

#include "globalfunctions.h"
#include "clippingstation.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //g_logfile.open("clippingstation.log", ios::app);
    qInstallMessageHandler(MyOutputHandler);

    ClippingStation w;

    QObject::connect(&a, SIGNAL(focusChanged( QWidget*, QWidget* ) ), &w, SLOT( onAppFocusChanged()));

    w.show();

    return a.exec();
}
