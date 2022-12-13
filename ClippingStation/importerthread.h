/************************************************************************
 * $Id: importerthread.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/importerthread.h $
 *
 ************************************************************************/

#ifndef IMPORTERTHREAD_H
#define IMPORTERTHREAD_H

#include <QtCore>
#include <QThread>

class ImporterDialog;


//!  \class ImporterThread
//!  This is a thread class to implement different ftp operations like uploading of files inside a thread

class ImporterThread : public QThread
{
    Q_OBJECT

public:
    //! \fn ImporterThread()
    //!     The constructor
    //! \see ImporterDialog();
    //! \param[in] ftp      ImporterDialog pointer to the UI dialog that created this thread
    //! \author elzubeir
    ImporterThread(ImporterDialog* dlg);

    //! \fn set()
    //!     sets the value for the thread
    //! \param[in] srcPath  QString source path for the files to be copied
    //! \param[in] srcFiles QStringList list of files to be copiled
    //! \param[in] dstPath  QString destination path for the files to be copied
    //! \param[in] dstFiles QStringList list of destination names files
    //! \author elzubeir
    void set(QString srcPath, QStringList srcFiles, QString dstPath, QStringList dstFiles, bool skipOcr);

signals:
    //! \fn importerThreadCompleted()
    //!   signal is fired when the thread has completed
    //! \author elzubeir
    void importerThreadCompleted();


protected:
    //! \fn run()
    //!     The starting point for the thread. After calling start(), the thread calls this function
    //! where it does the actual ftp process, like uploading of files
    //! \author elzubeir
    void run();

private:
    //! \fn upload()
    //!     upload the image to ftp server
    //! \author elzubeir
    void upload();

    //! \fn copy()
    //!     copy the image to file server
    //! \author elzubeir
    void copy();

    //! \fn get96DPIImagePath()
    //!     get the 96 dip image path to copy to file server while importing
    //! \param[in] filename: name of the 300 dpi image
    //! \author elzubeir
    QString get96DPIImagePath(QString filename);

private:
    //! a reference to the calling ImporterDialog instance that created this thread
    ImporterDialog& m_dlg;

    //! to hold the path of the source files
    QString m_srcPath;

    //! to hold a list of the source files
    QStringList m_srcFiles;

    //! to hold the path to the destination path, it will be of ftp address,
    //! mainly in this format ftp://user:password@ftpaddress/location/to/desired/path
    QString m_dstPath;

    //! to hold a list of the destination files
    QStringList m_dstFiles;

    //! skip ocr
    bool m_skipOcr;
};

#endif // IMPORTERTHREAD_H
