/************************************************************************
 * $Id: statusbar.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/statusbar.h $
 *
 ************************************************************************/

#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>

#include "imagecachingdialog.h"

class StatusBar : public QStatusBar
{
public:
	StatusBar(QWidget * parent); ///< ctor
	virtual ~StatusBar(); ///< dtor

	/**
		\brief display message.
		\param the message
	*/
	void displayMessage(const QString & message);

	/**
		\brief display mouse coordiantes.
		\param the message
	*/
	void displayCoordinates(qreal x, qreal y, qreal width = 0.0, qreal height = 0.0);

	/**
		\brief display rotation angle.
		\param the message
	*/
	void displayRotationAngle(qreal angle);

	/**
		\brief display rotation angle.
		\param the message
	*/
	void displayZoom(qreal zoom);

        void addProgressBarWidget(ImageCachingDialog *dlg);

private:
	QLabel * m_coordinatesStatusLabel;
	QLabel * m_angleRotationStatusLabel;
	QLabel * m_zoomStatusLabel;

        //QLabel * m_cachingStatusLabel;
        //QProgressBar * m_cachingProgressBar;
};

#endif // STATUS_BAR_H
