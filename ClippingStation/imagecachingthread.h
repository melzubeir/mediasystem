/************************************************************************
 * $Id: imagecachingthread.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imagecachingthread.h $
 *
 ************************************************************************/

#ifndef IMAGECACHINGTHREAD_H
#define IMAGECACHINGTHREAD_H

#include <QtCore>
#include <QThread>

class ImageCachingDialog;


class ImageCachingThread : public QThread
{
    Q_OBJECT

public:
    //! \fn ImageCachingThread()
    //!     The constructor
    //! \param[in] dlg      ImageCachingDialog pointer to the UI dialog that created this thread
    //! \author elzubeir
    ImageCachingThread(ImageCachingDialog * dlg);

    //! \fn setValues()
    //! \param[in] srcPath  QString source path for the files to be cached
    //! \param[in] srcFiles QStringList list of files to be copiled
    //! \param[in] dstPath  QString destination path for the files to be cached
    //! \param[in] dstFiles QStringList list of destination names files
    //! \author elzubeir
    void setValues(QString srcPath, QStringList srcFiles, QString dstPath, QStringList dstFiles);

    //! \fn cacheFromFileServer()
    //!     caches one page from file server to local directory
    //! \param[in] srcPage  QString source path for the file to be cached
    //! \param[in] dstPage  QString destination path for the file to be cached
    //! \return bool
    //!        - true if the page chaching is successfull
    //!        - false otherwise
    //! \author elzubeir
    bool cacheFromFileServer(QString srcPage, QString dstPage);

    //! \fn cacheFromFtpServer()
    //!     caches one page from ftp server to local directory
    //! \param[in] srcPage  QString source path for the file to be caeched
    //! \param[in] dstPage  QString destination path for the file to be cached
    //! \return bool
    //!        - true if the page chaching is successfull
    //!        - false otherwise
    //! \author elzubeir
    bool cacheFromFtpServer(QString srcPage, QString dstPage);

protected:
    //! \fn run()
    //!     The starting point for the thread. After calling start(), the thread calls this function
    //! where it does the actual ftp process, like uploading of files
    void run();

private:
    //! a reference to the calling FtpDialog instance that created this thread
    ImageCachingDialog& m_imageCacheDlg;

    //! to hold the path of the source files
    QString m_srcPath;

    //! to hold a list of the source files
    QStringList m_srcFiles;

    //! to hold the path to the destination path, it will be of ftp address,
    //! mainly in this format ftp://user:password@ftpaddress/location/to/desired/path
    QString m_dstPath;

    //! to hold a list of the destination files
    QStringList m_dstFiles;
};

#endif // IMAGECACHINGTHREAD_H
