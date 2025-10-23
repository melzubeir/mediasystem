/************************************************************************
 * $Id: imagecachingthread.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imagecachingthread.cpp $
 *
 ************************************************************************/

#include "imagecachingthread.h"
#include "imagecachingdialog.h"
#include "globalfunctions.h"

#include <QMessageBox>
#include <curl/curl.h>

ImageCachingThread::ImageCachingThread(ImageCachingDialog * dlg):
        m_imageCacheDlg(*dlg)
{

}


void ImageCachingThread::setValues(QString srcPath, QStringList srcFiles, QString dstPath, QStringList dstFiles )
{
    m_srcPath   = srcPath;  //src path
    m_srcFiles  = srcFiles; //src files
    m_dstPath   = dstPath;  //dst path
    m_dstFiles  = dstFiles; //dst files
}

void ImageCachingThread::run()
{
    m_imageCacheDlg.m_totalFilesCompleted = 0;        //number of completed files it set to zero

    m_dstPath = m_srcPath = "";
    //char strerror[256]; //string to hold the error message, if any occurs

    for( int i=0; i< m_srcFiles.count(); i++)       //iterate thru the files to upload
    {
        while( !m_imageCacheDlg.m_resume )
            return;

        //qDebug() << "Copying " << m_srcFiles[i].toLocal8Bit();

        if (m_imageCacheDlg.m_cachedImages[ m_dstPath+m_dstFiles[i] ] )
        {
            m_imageCacheDlg.m_totalFilesCompleted++;
        }
        else
        {
            bool ret = false;
            if(m_imageCacheDlg.useFtp())
                ret = cacheFromFtpServer(m_srcPath+m_srcFiles[i], m_dstPath+m_dstFiles[i]);
            else
                ret = cacheFromFileServer(m_srcPath+m_srcFiles[i], m_dstPath+m_dstFiles[i]);

            m_imageCacheDlg.m_cachedImages[ m_dstPath+m_dstFiles[i] ] = ret;
            m_imageCacheDlg.m_totalFilesCompleted++;
        }
    }
}


bool ImageCachingThread::cacheFromFtpServer(QString srcPage, QString dstPage)
{
    //remove windows drive letter, if any
    QString dst = srcPage.mid(srcPage.indexOf(":")+1, srcPage.length());
    //dst = dst.left( dst.length() -1 );      //to remove the last /

    //if ( !dst.endsWith("/") )
      //  dst += "/";

    srcPage = dst;
    srcPage = QString("ftp://%1:%2@%3/").arg( m_imageCacheDlg.username() ).arg( m_imageCacheDlg.password() ).arg( m_imageCacheDlg.address()) +  srcPage;


    CURL *curlhandle = NULL;        //the libcurl easy handle

    curl_global_init(CURL_GLOBAL_ALL);      //initate it
    curlhandle = curl_easy_init();

    char strerror[256];

    bool ret = download(curlhandle, srcPage.toLocal8Bit(), dstPage.toLocal8Bit(), 0, 1, strerror);

    curl_easy_cleanup(curlhandle);
    curl_global_cleanup();

    return ret;
}

bool ImageCachingThread::cacheFromFileServer(QString srcPage, QString dstPage)
{
    if (!QFile::exists(dstPage) && !QFile::copy(srcPage, dstPage) )
    {
        QMessageBox::critical(NULL, tr("Error!"), tr("Error copying image %1 to %2").arg(srcPage).arg(dstPage) );
        return false;
    }

    return true;
}
