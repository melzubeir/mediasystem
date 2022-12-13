/************************************************************************
 * $Id: drawerviewstackedwidget.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $Date: 2011-04-05 17:16:53 +0400 (Tue, 05 Apr 2011) $
 *  $Author: elzubeir $
 *  $Revision: 661 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/drawerviewstackedwidget.cpp $
 *
 ************************************************************************/

#include "drawerviewstackedwidget.h"

DrawerViewStackedWidget::DrawerViewStackedWidget(QWidget * parent) :
        QStackedWidget(parent)
{

}

DrawerViewStackedWidget::~DrawerViewStackedWidget()
{
    qDebug("~DrawerViewStackedWidget()");
}
