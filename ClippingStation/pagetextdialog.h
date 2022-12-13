/************************************************************************
 * $Id: pagetextdialog.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/pagetextdialog.h $
 *
 ************************************************************************/

#ifndef PAGETEXTDIALOG_H
#define PAGETEXTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class PageTextDialog;
}

class PageTextDialog : public QDialog {
    Q_OBJECT
public:
    //! \fn PageTextDialog()
    //!     default constructor
    //! \param[in] text to add to the dialog
    //! \author elzubeir
    PageTextDialog(QWidget *parent = 0, QString text = "");

    //! \fn ~PageTextDialog()
    //!     destructor
    //! \author elzubeir
    ~PageTextDialog();

    //! \fn setText()
    //!     set text of the dialog
    //! \param[in] text to add to the dialog
    //! \author elzubeir
    void setText(QString &text);

private slots:
    //! \fn find()
    //!     find the word in the dialog's text and heighlight it
    //! \author elzubeir
    void find();

    //! \fn findAll();
    //!     find all the words in the dialog's text and heighlight them
    //! \author elzubeir
    void findAll();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PageTextDialog *m_ui;

    //! dialog text
    QString m_text;
};

#endif // PAGETEXTDIALOG_H
