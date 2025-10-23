//! \file authenticationdialog.h
/************************************************************************
 * $Id: authenticationdialog.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/authenticationdialog.h $
 *
 ************************************************************************/

#ifndef AUTHENTICATIONDIALOG_H
#define AUTHENTICATIONDIALOG_H

#include <QDialog>

namespace Ui {
    class AuthenticationDialog;
}

//! \class AuthenticationDialog
//! \brief This class creates a username/password input dialog,
//!        and passes the value obtained to the main application for verification
//! \author elzubeir

class AuthenticationDialog : public QDialog {
    Q_OBJECT
public:
    //! \fn AuthenticationDialog(QWidget *parent = 0)
    //!     default constructor
    //! \param[in] parent   QWidget pointer set to the calling widget
    //! \author elzubeir
    AuthenticationDialog(QWidget *parent = 0);

    //! \fn ~AuthenticationDialog()
    //!     destructor
    //! \author elzubeir
    ~AuthenticationDialog();

protected:
    //! \fn accept()
    //! Overrides the base function, and sets the m_username/m_password from the users input, and then calls
    //! the base calss's accept method
    //! \author elzubeir
    void accept();

public:
    //! saves the username obtained from the user
    QString m_username;
    //! saves the password obtained from the user
    QString m_password;

private:
    //! pointer to the Qt desginer UI class object
    Ui::AuthenticationDialog *m_ui;
};

#endif // AUTHENTICATIONDIALOG_H
