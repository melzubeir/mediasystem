/************************************************************************
 * $Id: imagecachingdialog.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imagecachingdialog.cpp $
 *
 ************************************************************************/

#include "imagecachingdialog.h"
#include "ui_imagecachingdialog.h"

#include <QtCore>
#include <QMessageBox>


ImageCachingDialog::ImageCachingDialog(QWidget *parent, QHash<QString, bool> &cachedImages) :
        QDialog(parent),
        m_ui(new Ui::ImageCachingDialog), m_thread(this), m_cachedImages(cachedImages)
{
    m_ui->setupUi(this);

    //initalize the bool values
    m_generalError = false;
    m_terminated = false;
    m_completedSuccessfully = false;

    //connect the termianted signal
    connect(&m_thread, SIGNAL(terminated()), this, SLOT(threadTerminated()) );


    m_timer = new QTimer(); //create a timer and connect it's timeout signal to the update slot
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    readSettings();
}

ImageCachingDialog::~ImageCachingDialog()
{
    delete m_ui;

    if(m_thread.isRunning())
        m_thread.quit();

    m_timer->stop();
    delete m_timer;
    m_timer = 0;
}


void ImageCachingDialog::setValues(QStringList srcFiles, QStringList dstFiles)
{
    //m_srcPath       = srcPath;
    m_srcFiles      = srcFiles;
    //m_dstPath       = dstPath;
    m_dstFiles      = dstFiles;
    m_totalFiles    = srcFiles.count();
}

bool ImageCachingDialog::createCacheDirectory(QString dstPath)
{
    //QString
    QDir dir(dstPath);
    if( dir.exists() )
        return true;

    //make the directory if it does not exist
    return dir.mkpath( dstPath );
}

/*
bool ImageCachingDialog::recursiveDelete(const QDir &dir)
{
    QString abspath =  dir.absolutePath();
    qDebug() <<abspath.toAscii();

    //QList<QFileInfo> infLst = dir.entryInfoList(QStringList("*.jpg"), QDir::NoSymLinks);
    QStringList flist = dir.entryList(QStringList("*.jpg"), QDir::NoSymLinks | QDir::Files );

    qDebug ("File list is %d", flist.count() );
    //foreach(QFileInfo finfo, infLst )
    foreach (QString fname, flist)
    {
        //
        if(finfo.isDir() )
        {
            QDir subDir(finfo.absolutePath());
            if (recursiveDelete(subDir) )
                return dir.rmdir(subDir.absolutePath());

            return false;
        }
        else
        { //file
            QDir dir;
            if( !dir.remove(abspath + "/" + fname))
                return false;
        }
    }
    return true;
}
*/
/*
void ImageCachingDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}*/

void ImageCachingDialog::keyPressEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_Escape)//ignore the escape key pressed
        return;

    QDialog::keyPressEvent(key);
}


void ImageCachingDialog::startCaching()
{
    //m_cachedImages.clear();
    m_resume = true;

    QDir dir;
    //QString dstPath = dir.filePath(m_srcFiles[0]);
    for(int i=0; i< m_dstFiles.count(); i++)
    {
        int index = m_dstFiles[i].lastIndexOf("/");
        QString path = m_dstFiles[i].left(index);

        if(!createCacheDirectory(path))
        {
            qDebug() << "error creating dst Directory";
            return;
        }
    }

    populateFilesFromCacheDirectory();

    m_ui->progressBar->setRange(0, m_totalFiles);
    //m_thread.setValues(m_srcPath, m_srcFiles, m_dstPath);
    m_thread.setValues("", m_srcFiles, "", m_dstFiles);
    m_thread.start();   //start the read,

    if(m_timer->isActive() )
        m_timer->stop();

    m_timer->start(1000);   //and the timer with 1 sec timeout
}

void ImageCachingDialog::stopCaching()
{
    m_thread.terminate();
}


void ImageCachingDialog::pause()
{
    m_resume = false;
}

void ImageCachingDialog::resume()
{
    m_resume = true;
}

bool ImageCachingDialog::cacheOnePage(QString srcPage, QString dstPage)
{
    //m_srcPath = srcpath;
    QDir dir;
    //QString path = dir.filePath(dstPage);

    int index = dstPage.lastIndexOf("/");
    QString path = dstPage.left(index);

    if (!createCacheDirectory(path) )
        return false;

    //after we checked the cache directory

    if( !m_cachedImages[ dstPage ] )
    {
        bool ret = false;
        if(m_useFtp)
            ret = m_thread.cacheFromFtpServer(srcPage, dstPage);
        else
            ret = m_thread.cacheFromFileServer(srcPage, dstPage);

        m_cachedImages[ dstPage ] = ret;
        m_totalFilesCompleted++;
    }

    return true;
}

void ImageCachingDialog::threadTerminated()
{
    m_terminated = true;        //the user has termianted the process
    m_completedSuccessfully = false;    //and it has not completed successfully
    m_timer->stop();
    done(QDialog::Accepted);
}

void ImageCachingDialog::update()
{
    if(m_generalError)  //if general error has occured, then close the dialog
    {
        done(QDialog::Accepted);
    }

    m_ui->progressBar->setValue(m_totalFilesCompleted);         //values from the ftp thread


    if(m_totalFilesCompleted == m_totalFiles)   //if thread is finished with the ftp operation, then close the ImporterDialog box
    {
        m_timer->stop();
        m_thread.wait();
        m_completedSuccessfully = true; //set the flag to completed successfully
        done(QDialog::Accepted);
        //qDebug() << "Thread completed successfully";
    }

    if( !m_resume)
    {
        m_timer->stop();
        //m_thread.wait();
        m_completedSuccessfully = true; //set the flag to completed successfully
        done(QDialog::Accepted);
        qDebug() << "Thread exited pre-complition";
    }
}

bool ImageCachingDialog::completedSuccessfully()
{
    return m_completedSuccessfully;
}

bool ImageCachingDialog::terminated()
{
    return m_terminated;
}

QString ImageCachingDialog::lastError()
{
    return m_errorString;
}

int ImageCachingDialog::cachedFilesCount()
{
    return m_totalFilesCompleted;
}

bool ImageCachingDialog::cachingThreadStarted()
{
    return m_thread.isRunning();
}

void ImageCachingDialog::populateFilesFromCacheDirectory()
{
    QDir dir;
    foreach (QString fname, m_dstFiles)
    {
        if( dir.exists(fname) )
            m_cachedImages[fname] = true;
        else
            m_cachedImages[fname] = false;
    }
}

bool ImageCachingDialog::readSettings()
{
    QString ftp_ini = QApplication::applicationDirPath()+ "/config.ini";
    QSettings settings(QString(ftp_ini), QSettings::IniFormat);

    m_useFtp      = settings.value("ftp access/useftp").toInt();    //if its set to 0, its false, otherwise it true
    if(m_useFtp)
    {
        m_address   = settings.value("ftp access/server").toString();
        m_username  = settings.value("ftp access/username").toString();
        m_password  = settings.value("ftp access/password").toString();
        m_port      = settings.value("ftp access/port").toInt();

        if(m_address.isEmpty() || m_username.isEmpty() || m_password.isEmpty() || (m_port == 0))
            return false;
    }
    else
        return false;

    return true;
}


bool ImageCachingDialog::useFtp()
{
    return m_useFtp;
}

QString ImageCachingDialog::address()
{
    return m_address;
}

QString ImageCachingDialog::username()
{
    return m_username;
}

QString ImageCachingDialog::password()
{
    return m_password;
}

int ImageCachingDialog::port()
{
    return m_port;
}
