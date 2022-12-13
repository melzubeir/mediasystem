/************************************************************************
 * $Id: pagetextdialog.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/pagetextdialog.cpp $
 *
 ************************************************************************/

#include "pagetextdialog.h"
#include "ui_pagetextdialog.h"

PageTextDialog::PageTextDialog(QWidget *parent, QString text) :
    QDialog(parent),
    m_ui(new Ui::PageTextDialog),
    m_text(text)
{
    m_ui->setupUi(this);
    m_ui->textBrowser->setText(m_text);
}

PageTextDialog::~PageTextDialog()
{
    delete m_ui;
}

void PageTextDialog::changeEvent(QEvent *e)
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

void PageTextDialog::setText(QString &text)
{
    m_text = text;
}


void PageTextDialog::find()
{
    QString str = m_ui->findLineEdit->text().trimmed();
    static QString oldsearch;

    if(oldsearch != str)        //return to search from beginning.
    {
        m_ui->textBrowser->moveCursor(QTextCursor::Start);
        QList<QTextEdit::ExtraSelection> extraSelections;
        m_ui->textBrowser->setExtraSelections(extraSelections);
    }

    oldsearch = str;

    if( m_ui->textBrowser->find(str) )
        m_ui->textBrowser->setFocus();
}

void PageTextDialog::findAll()
{

    QString str = m_ui->findLineEdit->text().trimmed();

    m_ui->textBrowser->moveCursor(QTextCursor::Start);
    QColor color = QColor(Qt::blue).lighter(130);

    QList<QTextEdit::ExtraSelection> extraSelections;
    while(m_ui->textBrowser->find(str))
    {
        QTextEdit::ExtraSelection extra;
        extra.format.setBackground(color);

        extra.cursor = m_ui->textBrowser->textCursor();
        extraSelections.append(extra);
    }

    m_ui->textBrowser->setExtraSelections(extraSelections);
}
