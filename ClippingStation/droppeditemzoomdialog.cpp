/************************************************************************
 * $Id: droppeditemzoomdialog.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/droppeditemzoomdialog.cpp $
 *
 ************************************************************************/

#include "droppeditemzoomdialog.h"
#include "ui_droppeditemzoomdialog.h"


DroppedItemZoomDialog::DroppedItemZoomDialog(DroppedItem *droppedItem, QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DroppedItemZoomDialog),
        m_droppedItem(droppedItem)
{
    m_ui->setupUi(this);
}

DroppedItemZoomDialog::~DroppedItemZoomDialog()
{
    delete m_ui;
}

void DroppedItemZoomDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DroppedItemZoomDialog::setValue(int i)
{
    m_ui->zoomSlider->setValue(i);
}

void DroppedItemZoomDialog::zoom(int value)
{
    m_droppedItem->zoom(value);
}
