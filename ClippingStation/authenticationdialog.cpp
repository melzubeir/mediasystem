//! \file authenticationdialog.cpp
/************************************************************************
 * $Id: authenticationdialog.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/authenticationdialog.cpp $
 *
 ************************************************************************/

#include "authenticationdialog.h"
#include "ui_authenticationdialog.h"

AuthenticationDialog::AuthenticationDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AuthenticationDialog)
{
    m_ui->setupUi(this);
}

AuthenticationDialog::~AuthenticationDialog()
{
    delete m_ui;
}

void AuthenticationDialog::accept()
{
    //set m_username, m_password from user input
    m_username = m_ui->usernameLineEdit->text();
    m_password = m_ui->passwordLineEdit->text();

    //call base class's accept method
    QDialog::accept();
}
