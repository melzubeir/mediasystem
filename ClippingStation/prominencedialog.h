/************************************************************************
 * $Id: prominencedialog.h 583 2010-05-15 09:54:58Z elzubeir $
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
 *  $Date: 2010-05-15 13:54:58 +0400 (Sat, 15 May 2010) $
 *  $Author: elzubeir $
 *  $Revision: 583 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/prominencedialog.h $
 *
 ************************************************************************/

#ifndef PROMINENCEDIALOG_H
#define PROMINENCEDIALOG_H

#include <QtGui/QDialog>
#include "prominence.h"

namespace Ui {
    class ProminenceDialog;
}

class ProminenceDialog : public QDialog {
    Q_OBJECT
public:

    //! \fn ProminenceDialog()
    //!     default constructor
    //! \author elzubeir
    ProminenceDialog(QWidget *parent = 0);

    //! \fn ~ProminenceDialog()
    //!     destrct
    //! \author elzubeir
    ~ProminenceDialog();

    //! \fn prominenceDialog()
    //!     returns an object of the Prominence class containing the data populated
    //! \author elzubeir
    Prominence prominence();

    //! \fn setProminence()
    //!     set the prominence values
    //! \param[in] Prominence object to set its value
    //! \author elzubeir
    void setProminence(Prominence pr);

protected:
    //! \fn changeEvent()
    //!     a change event has occured
    //! \author elzubeir
    void changeEvent(QEvent *e);

    //! \fn accept()
    //!     the ok button is clicked
    //! \author elzubeir
    void accept();

private:
    //! \fn updateUi()
    //!     updates the ui with the values from the prominence object
    //! \author elzubeir
    void updateUi();
private:
    Ui::ProminenceDialog *m_ui;

    //! prominence object
    Prominence m_prominence;
};

#endif // PROMINENCEDIALOG_H
