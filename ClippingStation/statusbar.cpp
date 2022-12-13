/************************************************************************
 * $Id: statusbar.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/statusbar.cpp $
 *
 ************************************************************************/

#include "statusbar.h"

#include <QHBoxLayout>

StatusBar::StatusBar(QWidget * parent) :
	QStatusBar(parent)
{
    m_coordinatesStatusLabel = new QLabel(this);
    addPermanentWidget(m_coordinatesStatusLabel, 1);

    m_angleRotationStatusLabel = new QLabel(this);
    //m_angleRotationStatusLabel->setText("Rotation : 0.0");
    addPermanentWidget(m_angleRotationStatusLabel, 2);

    m_zoomStatusLabel = new QLabel(this);
    //m_zoomStatusLabel->setText("Zoom : 100 %");
    addPermanentWidget(m_zoomStatusLabel, 3);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

}

void StatusBar::addProgressBarWidget(ImageCachingDialog *dlg)
{
    addPermanentWidget((QWidget*)dlg, 1);
    dlg->hide();
}

StatusBar::~StatusBar()
{

}

void StatusBar::displayCoordinates(qreal x, qreal y, qreal width /*=0.0*/, qreal height /*=0.0*/)
{
	if (width == 0 && height == 0)
                m_coordinatesStatusLabel->setText(tr("x : %1 y : %2").arg(x).arg(y));
	else
                m_coordinatesStatusLabel->setText(tr("x: %1 y: %2 width: %3 height: %4").arg(x).arg(y).arg(width).arg(height));
}

void StatusBar::displayRotationAngle(qreal angle)
{
        m_angleRotationStatusLabel->setText(tr("Rotation : %1").arg(angle));
}

void StatusBar::displayZoom(qreal zoom)
{
        m_zoomStatusLabel->setText(tr("Zoom : %1 %2").arg(zoom*100).arg("%") );
}
