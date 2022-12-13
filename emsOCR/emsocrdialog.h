/************************************************************************
 * $Id: emsocrdialog.h 648 2011-03-10 03:42:39Z elzubeir $
 *
 * ------------
 * Description:
 * ------------
 *
 * (C) Copyright 2009,2010 ALLCONTENT. All rights reserved.
 *
 * -----------------
 * Revision Details:    (Updated by Revision Control System)
 * -----------------
 *  $Date: 2011-03-10 07:42:39 +0400 (Thu, 10 Mar 2011) $
 *  $Author: elzubeir $
 *  $Revision: 648 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/emsocrdialog.h $
 *
 ************************************************************************/

#ifndef EMSOCRDIALOG_H
#define EMSOCRDIALOG_H

#include <QtGui/QDialog>

#include "emsocr.h"


namespace Ui
{
    class emsOCRDialog;
}

class emsOCRDialog : public QDialog
{
    Q_OBJECT

public:
    //! \fn emsOCRDialog()
    //!     default constructor
	//! \param[in] parent widget
    //! \author elzubeir
    emsOCRDialog(bool autoStart = false, QWidget *parent = 0);

	//! \fn ~emsOCRDialog()
    //!     destructor
    //! \author elzubeir
	~emsOCRDialog();

    //! instance to the widget UI class
	Ui::emsOCRDialog *ui;

	//! total number of processed pages
    int m_totalNumberOfProcessedPages;

private:
	//! the ocr engine instance
    emsOCR m_emsOCR;

	//! the timer to run the ocr
    QTimer *m_timer;

	//! has the thread started
    bool m_isStarted;


private slots:

	//! \fn on_startButton_clicked()
    //!     this function starts/stops the ocr engine thread
    //! \author elzubeir
    void on_startButton_clicked();

	//! \fn threadTerminated()
    //!     this function is called when the ocr engin thread is terminated
    //! \author elzubeir
    void threadTerminated();
};

#endif // EMSOCRDIALOG_H
