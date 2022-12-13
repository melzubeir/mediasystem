/************************************************************************
 * $Id: prominencedialog.cpp 558 2010-04-21 09:00:15Z elzubeir $
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
 *  $Date: 2010-04-21 13:00:15 +0400 (Wed, 21 Apr 2010) $
 *  $Author: elzubeir $
 *  $Revision: 558 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/prominencedialog.cpp $
 *
 ************************************************************************/

#include "prominencedialog.h"
#include "ui_prominencedialog.h"

ProminenceDialog::ProminenceDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ProminenceDialog)
{
    m_ui->setupUi(this);
}

ProminenceDialog::~ProminenceDialog()
{
    delete m_ui;
}

void ProminenceDialog::changeEvent(QEvent *e)
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

void ProminenceDialog::accept()
{
    m_prominence.m_note = m_ui->noteLineEdit->text().trimmed();
    m_prominence.m_size = m_ui->sizeLineEdit->text().toDouble();
    int index = m_ui->mentionScoreComboBox->currentIndex();
    if(index == 0)
        m_prominence.m_mentionScore = 1;
    else if (index == 1)
        m_prominence.m_mentionScore = 2;
    else
        m_prominence.m_mentionScore = 3;

    index = m_ui->visualScoreComboBox->currentIndex();
    if(index == 0)
        m_prominence.m_visualScore = 0;
    else if(index == 1)
        m_prominence.m_visualScore = -99;
    else if (index == 2)
        m_prominence.m_visualScore = 1;
    else
        m_prominence.m_visualScore = 2;

    index = m_ui->toneScoreComboBox->currentIndex();
    if(index == 0)
        m_prominence.m_toneScore = 1;
    else if(index == 1)
        m_prominence.m_toneScore = 2;
    else
        m_prominence.m_toneScore = -2;

    QDialog::accept();
}


Prominence ProminenceDialog::prominence()
{
    return m_prominence;
}

void ProminenceDialog::setProminence(Prominence pr)
{
    m_prominence = pr;

    updateUi();
}

void ProminenceDialog::updateUi()
{
    m_ui->noteLineEdit->setText(m_prominence.m_note);
    m_ui->sizeLineEdit->setText(QString("%1").arg(m_prominence.m_size) );
    int index = m_prominence.m_mentionScore;
    if(index == 1)
        m_ui->mentionScoreComboBox->setCurrentIndex(0);
    else if (index == 2)
        m_ui->mentionScoreComboBox->setCurrentIndex(1);
    else
        m_ui->mentionScoreComboBox->setCurrentIndex(2);

    index = m_prominence.m_visualScore;
    if(index == 0)
        m_ui->visualScoreComboBox->setCurrentIndex(0);
    else if(index == -99)
        m_ui->visualScoreComboBox->setCurrentIndex(1);
    else if (index == 1)
        m_ui->visualScoreComboBox->setCurrentIndex(2);
    else
        m_ui->visualScoreComboBox->setCurrentIndex(3);

    index = m_prominence.m_toneScore;
    if(index == 1)
         m_ui->toneScoreComboBox->setCurrentIndex(0);
    else if(index == 2)
         m_ui->toneScoreComboBox->setCurrentIndex(1);
    else
         m_ui->toneScoreComboBox->setCurrentIndex(2);
}
