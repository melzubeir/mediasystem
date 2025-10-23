/************************************************************************
 * $Id: droppeditemzoomdialog.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/droppeditemzoomdialog.h $
 *
 ************************************************************************/

#ifndef DROPPEDITEMZOOMDIALOG_H
#define DROPPEDITEMZOOMDIALOG_H

#include <QDialog>

#include "droppeditem.h"


namespace Ui {
    class DroppedItemZoomDialog;
}

class DroppedItem;

class DroppedItemZoomDialog : public QDialog {
    Q_OBJECT

public:
    //! \fn DroppedItemZoomDialog()
    //!     constructor to zoom the dropped it in the drawer page
    //! \param[in] dropped item pointer to the item in the drawer to be zoomed in/out
    //! \author elzubeir
    DroppedItemZoomDialog(DroppedItem *droppedItem, QWidget *parent = 0);

    //! \fn ~DroppedItemZoomDialog()
    //!     destructor
    //! \author elzubeir
    ~DroppedItemZoomDialog();

    //! \fn setValue()
    //!     set the zoom value of the dialog
    //! \param[in] value of the zooming
    //! \author elzubeir
    void setValue(int i);

protected:
    void changeEvent(QEvent *e);

private slots:
    //! \fn zoom()
    //!     do the zoom operation
    //! \param[in] the value of zooming
    //! \author elzubeir
    void zoom(int);

private:
    Ui::DroppedItemZoomDialog *m_ui;

    //! dropped item pointer
    DroppedItem *m_droppedItem;
};

#endif // DROPPEDITEMZOOMDIALOG_H
