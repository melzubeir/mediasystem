/************************************************************************
 * $Id: searchpublicationlistdialog.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/searchpublicationlistdialog.cpp $
 *
 ************************************************************************/

#include "searchpublicationlistdialog.h"
#include "ui_searchpublicationlistdialog.h"

#include <QCompleter>

SearchPublicationListDialog::SearchPublicationListDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SearchPublicationListDialog)
{
    m_ui->setupUi(this);
    m_ui->startDateEdit->setDate(QDate::currentDate()); //set the current date to today!
    m_ui->endDateEdit->setDate(QDate::currentDate());
}

SearchPublicationListDialog::~SearchPublicationListDialog()
{
    delete m_ui;
}

void SearchPublicationListDialog::changeEvent(QEvent *e)
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

void SearchPublicationListDialog::setKeywords(QStringList &keywords)
{
    QCompleter* completer = new QCompleter(keywords, this);     //createa completer with the list of publication names
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui->keywordLineEdit->setCompleter(completer);   //use the complete with the publicationLineEdit
}


void SearchPublicationListDialog::setCountryList(QStringList &countries)
{
    countries.insert(0, "");
    m_ui->countryComboBox->insertItems(0, countries);
}

void SearchPublicationListDialog::get(QString &keyword, bool &dateselected, QString &startDate, QString &endDate,
                                      QString &type, QString &country, QString &ocrSearchword, QString &status)
{
    keyword = m_ui->keywordLineEdit->text().trimmed();
    dateselected = m_ui->dateRadioButton->isChecked();
    if(dateselected)
    {
        startDate = m_ui->startDateEdit->text();
        endDate = m_ui->endDateEdit->text();
    }
    else
        startDate = endDate = "";

    type = m_ui->typeComboBox->currentText();
    country = m_ui->countryComboBox->currentText();
    ocrSearchword = m_ui->ocrSearchLineEdit->text();
    status = m_ui->statusComboBox->currentText().trimmed();
}
