/************************************************************************
 * $Id: drawerviewstackedwidget.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/drawerviewstackedwidget.h $
 *
 ************************************************************************/

#ifndef DRAWER_VIEW_STACKED_WIDGET_H
#define DRAWER_VIEW_STACKED_WIDGET_H

#include <QStackedWidget>

/**
        \brief Stack widget that contains all Destination View.
        Manages the display of each graphics view.
*/
class DrawerViewStackedWidget : public QStackedWidget
{
public:
    //! \fn DrawerViewStackedWidget()
    //!     default const
    //! \param[in]  parent of the calling widget
    //! \author elzubeir
    DrawerViewStackedWidget(QWidget * parent = 0); //ctor

    //! \fn ~DrawerViewStackedWidget()
    //!     destructor
    //! \author elzubeir
    virtual ~DrawerViewStackedWidget(); //dtor
};
#endif // DRAWER_VIEW_STACKED_WIDGET_H
