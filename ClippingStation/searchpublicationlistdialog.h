/************************************************************************
 * $Id: searchpublicationlistdialog.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/searchpublicationlistdialog.h $
 *
 ************************************************************************/

#ifndef SEARCHPUBLICATIONLISTDIALOG_H
#define SEARCHPUBLICATIONLISTDIALOG_H

#include <QDialog>

namespace Ui {
    class SearchPublicationListDialog;
}

class SearchPublicationListDialog : public QDialog {
    Q_OBJECT
public:
    //! \fn SearchPublicationListDialog()
    //!     constructor
    //! \author elzubeir
    SearchPublicationListDialog(QWidget *parent = 0);

    //! \fn ~SearchPublicationListDialog()
    //!     destructor
    //! \author elzubeir
    ~SearchPublicationListDialog();

    //! \fn setKeywords()
    //!     set the keywords list of the dialog
    //! \param[in]  list of keywords
    //! \param[in]
    //! \author elzubeir
    void setKeywords(QStringList &keywords);

    //! \fn setCountryList()
    //!     set the country list of the dialog
    //! \param[in] list of countries
    //! \author elzubeir
    void setCountryList(QStringList &countries);

    //! \fn get()
    //!     get the values of the search dialog
    //! \param[out] keyword to search for
    //! \param[out] is date selected
    //! \param[out] start date
    //! \param[out] end date
    //! \param[out] type of publicaiton (mag/newspaper)
    //! \param[out] country
    //! \param[out] search word
    //! \author elzubeir
    void get(QString &keyword, bool &dateselected, QString &startDate, QString &endDate, QString &type, QString &country, QString &ocrSearchword, QString &status);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::SearchPublicationListDialog *m_ui;
};

#endif // SEARCHPUBLICATIONLISTDIALOG_H
