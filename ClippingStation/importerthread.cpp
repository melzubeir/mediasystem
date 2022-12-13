/************************************************************************
 * $Id: importerthread.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/importerthread.cpp $
 *
 ************************************************************************/

#include "importerthread.h"
#include "Importer/importerdialog.h"
#include "globalfunctions.h"
#include "constants.h"

#include <QMessageBox>
#include <curl/curl.h>

ImporterThread::ImporterThread(ImporterDialog *dlg)
        : m_dlg(*dlg)
{

}

void ImporterThread::set(QString srcPath, QStringList srcFiles, QString dstPath, QStringList dstFiles, bool skipOcr)
{
    m_srcPath = srcPath;
    m_srcFiles= srcFiles;

    m_dstPath = dstPath;
    m_dstFiles= dstFiles;

    m_skipOcr = skipOcr;
}


void ImporterThread::run()
{
    if(m_dlg.m_useFtp)
        upload();
    else
        copy();

    emit importerThreadCompleted();
}



void ImporterThread::copy()
{
    QDir dir;
    qDebug() << "Creating the directory for the " << m_dstPath << "/" << HIGH_RES_IMAGE_DIR;
    if ( !dir.exists(m_dstPath + HIGH_RES_IMAGE_DIR) )
    {
        bool ok = dir.mkpath(m_dstPath + HIGH_RES_IMAGE_DIR);     //this function creates the path, with all its necessary parents;

        if(!ok)
        {
            //error message, could not create the required directory structure!
            //qDebug() << "Path: " << destPath + HIGH_RES_IMAGE_DIR;
            QMessageBox::critical(NULL, tr("Error!"), tr("Error creating directory structure for path %1").arg(m_dstPath));
            m_dlg.m_completedSuccessfully = false;
            return;
        }
    }

    m_dlg.m_totalFilesCompleted = 0;        //number of completed files it set to zero

    for( int i=0; i< m_srcFiles.count(); i++)       //iterate thru the files to upload
    {
        g_currentUpload = 0;   //sets the gloal variable that represents the numer of bytes currently uploaded ot the ftp server, its reset for every file uploaded

        QFileInfo f(QString(m_srcPath + m_srcFiles[i]));    //get the current files size
        m_dlg.m_currentFileSize = f.size();

        //get a 96 dip image to save.
        QString image96DpiFilePath = get96DPIImagePath(m_srcPath+ m_srcFiles[i]);
        if(image96DpiFilePath == "")
            continue;

        if( !QFile::copy(image96DpiFilePath, m_dstPath+ m_dstFiles[i]) )
        {
            //QMessageBox::critical(this, tr("Error!"), tr("Error copying 96 dpi image %1 to %2.%3").arg(image96DpiFilePath).arg(dstPath).arg(dstFiles[i]) );
            //m_dlg.m_errorString = "";

            qDebug() << "Error copying 96 dpi image " << image96DpiFilePath << " to " << m_dstPath << "/" << m_dstFiles[i];
            m_dlg.m_completedSuccessfully = false;
            //m_dlg.m_generalError = true;
        }
        else
        {
            //we double check that its really copied
            int j = 0;
            while(!QFile::exists(m_dstPath + m_dstFiles[i]) && j < 2)
            {
                if( !QFile::copy(image96DpiFilePath, m_dstPath+ m_dstFiles[i]) )
                {
                    //QMessageBox::critical(this, tr("Error!"), tr("Error copying 96 dpi image %1 to %2.%3").arg(filepath).arg(dstPath).arg(dstFiles[i]) );
                    //m_dlg.m_errorString = "";
                    qDebug() << "Error copying 96 dpi image " << image96DpiFilePath << " to " << m_dstPath << "/" << m_dstFiles[i];
                    m_dlg.m_completedSuccessfully = false;
                    //m_dlg.m_generalError = true;
                }
                j++;
            }

            if(j < 2)
                m_dlg.m_totalFilesCompleted++;//no error happened, increase the number of completed files by 1

        }
        //finally we remove the 96 dip from our station
        QFile::remove(image96DpiFilePath);

        if(!m_skipOcr)
        {
            if( !QFile::copy(m_srcPath+ m_srcFiles[i], m_dstPath+ HIGH_RES_IMAGE_DIR + m_dstFiles[i]) )
            {
                //QMessageBox::critical(this, tr("Error!"), tr("Error copying 300dip image %1 to %2").arg(m_srcFiles[i]).arg(m_dstPath).arg(m_dstFiles[i]) );
                qDebug() << "Error copying 300 dpi image " << m_srcPath+ m_srcFiles[i] << " to " << m_dstPath+ HIGH_RES_IMAGE_DIR + m_dstFiles[i];
            }
        }
    }

    /*
    for(int i=0; i< dstFiles.count(); i++)
    {
        if ( conflictList.contains(dstFiles[i], Qt::CaseInsensitive) )
        {
            if( !QFile::remove(dstPath+dstFiles[i]))
            {
                QMessageBox::critical(this, tr("Error!"), tr("Error deleting image %1 from issues path %2").arg(dstFiles[i]).arg(dstPath) );
            }
        }
    }
    */
}

void ImporterThread::upload()
{
    CURL *curlhandle = NULL;        //the libcurl easy handle

    curl_global_init(CURL_GLOBAL_ALL);      //initate it
    curlhandle = curl_easy_init();

    m_dlg.m_totalFilesCompleted = 0;        //number of completed files it set to zero

    char strerror[256]; //string to hold the error message, if any occurs

    for( int i=0; i< m_srcFiles.count(); i++)       //iterate thru the files to upload
    {
        g_currentUpload = 0;   //sets the gloal variable that represents the numer of bytes currently uploaded ot the ftp server, its reset for every file uploaded


        QString image96DpiFilePath = get96DPIImagePath(m_srcPath+ m_srcFiles[i]);
        if(image96DpiFilePath == "")
            continue;


        QFileInfo f(image96DpiFilePath);    //get the current files size
        m_dlg.m_currentFileSize = f.size();


        //do the actual upload process
        //first we upload the 96 dpi, then we upload the 300 dpi
        if (!::upload(curlhandle, QString(m_dstPath + m_dstFiles[i]).toAscii(), image96DpiFilePath.toAscii(), 0, 1, strerror))
        {
            //if error happens, then set the apropriate values for the calling object, and exit the thread
            QString errMsg(strerror);
            //m_dlg.m_errorString = errMsg;

            m_dlg.m_completedSuccessfully = false;
            //m_dlg.m_generalError = true;
        }
        else
        {
            if(!m_skipOcr)
            {
                g_currentUpload = 0;   //sets the gloal variable that represents the numer of bytes currently uploaded ot the ftp server, its reset for every file uploaded
                QFileInfo f2(QString(m_srcPath + m_srcFiles[i]));    //get the current files size
                m_dlg.m_currentFileSize = f2.size();

                if (!::upload(curlhandle, QString(m_dstPath + m_dstFiles[i]).toAscii(), QString(m_srcPath + m_srcFiles[i]).toAscii(), 0, 1, strerror))
                {
                    //if error happens, then set the apropriate values for the calling object, and exit the thread
                    QString errMsg(strerror);
                    //m_dlg.m_errorString = errMsg;

                    m_dlg.m_completedSuccessfully = false;
                }
            }

            m_dlg.m_totalFilesCompleted++;//no error happened, increase the number of completed files by 1
        }

        QFile::remove(image96DpiFilePath);

    }

    //clean up before exit
    curl_easy_cleanup(curlhandle);
    curl_global_cleanup();
}


QString ImporterThread::get96DPIImagePath(QString filename)
{
    QImage originalImage(filename);
    QSize size(originalImage.width()/3, originalImage.height()/3);
    QImage image(size, QImage::Format_RGB888);

    image = originalImage.scaledToWidth(originalImage.width()/3, Qt::SmoothTransformation);
    //return image;

    QString filename96  = filename.replace(".jpg", "");
    filename96 += "_96dpi.jpg";

    //qDebug() << "get96DPIImagePath(): 96 dpi Image path: " << filename96;
    if( image.save(filename96, "jpg", JPG_SAVE_QUALITY) )
        return filename96;
    else
    {
        //QMessageBox::critical(this, tr("Error!"), tr("Error creating 96 dpi image %1").arg(filename96));
        return "";
    }
}
