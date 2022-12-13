/************************************************************************
 * $Id: emsocrdialog.cpp 648 2011-03-10 03:42:39Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/emsocrdialog.cpp $
 *
 ************************************************************************/

#include "emsocrdialog.h"
#include "ui_emsocrdialog.h"

#include <QtGui/QMessageBox>

emsOCRDialog::emsOCRDialog(bool autoStart, QWidget *parent)
    : QDialog(parent), ui(new Ui::emsOCRDialog)
{
    ui->setupUi(this);

	Qt::WindowFlags flags;
	flags = Qt::Window | Qt::WindowMinimizeButtonHint;
	setWindowFlags( flags );

    //m_timer = new QTimer(); //create a timer and connect it's timeout signal to the update slot
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    if (!m_emsOCR.Initalize())
	{
		QMessageBox::information(this, "Error intializing application", "Error happened in Inializing application, please see the log file for more information on the error happened");
        exit(0);
	}

    m_emsOCR.SetOCRDialog(this);
    m_isStarted = false;

    connect(&m_emsOCR, SIGNAL(terminated()), this, SLOT(threadTerminated()) );

    m_totalNumberOfProcessedPages = 0;

	if(autoStart)
		on_startButton_clicked();
}

emsOCRDialog::~emsOCRDialog()
{
    delete ui;
}

void emsOCRDialog::on_startButton_clicked()
{
    if(!m_isStarted)
    {
        ui->startButton->setText(tr("Stop"));
        m_isStarted = true;
        m_emsOCR.start();
        //QTimer::singleShot( 100, this, SLOT(start()) );
    }
    else
    {
        m_emsOCR.Stop();
        m_isStarted = false;
        ui->startButton->setText(tr("Stopping"));
    }
}

void emsOCRDialog::threadTerminated()
{
    ui->startButton->setText(tr("Start"));
    m_isStarted = false;
}
