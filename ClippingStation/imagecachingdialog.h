/************************************************************************
 * $Id: imagecachingdialog.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/imagecachingdialog.h $
 *
 ************************************************************************/

#ifndef IMAGECACHINGDIALOG_H
#define IMAGECACHINGDIALOG_H

#include <QtGui/QDialog>
#include <QTimer>
#include <QKeyEvent>

#include "imagecachingthread.h"

namespace Ui {
    class ImageCachingDialog;
}

class ImageCachingDialog : public QDialog {
    Q_OBJECT

friend class ImageCachingThread;

public:
    ImageCachingDialog(QWidget *parent, QHash<QString, bool> &cachedImages);

    //! \fn ~ImageCachingDialog()
    //!     destructor
    //! \author elzubeir
    ~ImageCachingDialog();

    //! \fn completedSuccessfully()
    //!     This function returns whether the ftp operation completed successfully or not
    //! \return bool
    //!     - true if operation completed successfully
    //!     - false otherwise
    //! \author elzubeir
    bool completedSuccessfully();

    //! \fn terminated()
    //!     This function returns whether the ftp operation was terminated by user or not
    //! \return bool
    //!     - true if operation terminated by user
    //!     - false otherwise
    //! \author elzubeir
    bool terminated();

    //! \fn cachedFilesCount()
    //!     This function returns the number of successfully cached files
    //! \return int
    //!     - the number of files cached successfully
    //! \author elzubeir
    int cachedFilesCount();

    //! \fn lastError()
    //!     This function returns the last error that occured in the ftp operation
    //! \return QString
    //!     - the last error that happened
    //! \author elzubeir
    QString lastError();

    //! \fn cachingThreadStarted()
    //!     is the caching process started
    //! \return boolean to represent if the caching thread has started or not
    //! \author elzubeir
    bool cachingThreadStarted();

    //! \fn startCaching()
    //!     stats the caching process
    //! \author elzubeir
    void startCaching();

    //! \fn stopCaching()
    //!     stops the caching process
    //! \author elzubeir
    void stopCaching();

    //! \fn pause()
    //!     pause the caching process
    //! \author elzubeir
    void pause();

    //! \fn resume()
    //!     resume the caching process
    //! \author elzubeir
    void resume();

    //! \fn cacheOnePage()
    //!     Caches one page from src to local cache directory
    //! \param[in] srcPage  QString path to the source page
    //! \param[in] dstPage  QString path to the destination page
    //! \return bool
    //!     true    if operation successful
    //!     false   if not
    //! \author elzubeir
    bool cacheOnePage(QString srcPage, QString dstPage);

    //! \fn setValues()
    //!     set the values for the local and remote file names, this function is used before starting the caching process to map
    //! the remote file paths to local file paths
    //! \param[in] srcFiles  QString list of the source (remote) file paths
    //! \param[in] dstFiles  QString list of the destination (local) file paths
    //! \author elzubeir
    void setValues(QStringList srcFiles, QStringList dstFiles );

    //! \fn useFtp()
    //!     this functions returns if the operation to be done on ftp server ot not
    //! \return bool
    //!     true    if use ftp
    //!     false   if not
    //! \author elzubeir
    bool useFtp();

    //! \fn address()
    //!     this functions returns the ftp's address
    //! \return QString the ftp's addrss
    //! \author elzubeir
    QString address();

    //! \fn username()
    //!     this functions returns the ftp's username
    //! \return QString the ftp's username
    //! \author elzubeir
    QString username();

    //! \fn password()
    //!     this functions returns the ftp's password()
    //! \return QString the ftp's password
    //! \author elzubeir
    QString password();

    //! \fn port()
    //!     this functions returns the ftp's port
    //! \return int     the ftp's port
    //! \author elzubeir
    int port();

private:
    //! \fn createCacheDirectory()
    //!     This function creates the cache directory on the local device
    //! dstPath     string holding the path of the cache directory
    //! \return bool
    //!        - true if the cache directory is created successfully
    //!        - false otherwise
    //! \author elzubeir
    bool createCacheDirectory(QString dstPath);

    //! \fn createCacheDirectory()
    //!     This function checks if the files for the publication already cached in the directory, if so,
    //! it updates the hash object to reflect that case, this function is useful for not re-cache the files from the file/ftp server
    //! \author elzubeir
    void populateFilesFromCacheDirectory();

    //! \fn readSettings()
    //!     This function reads the settings for the ftp usage from a config.ini file
    //! and populate the member variables with the values read from the ini file
    //! \return bool
    //!        - true if all the values are populated
    //!        - false otherwise
    //! \author elzubeir
    bool readSettings();

protected:
    //! \fn keyPressEvent(QKeyEvent *key)
    //!     override function from QDialog base class to trap the escape key pressed to prevent it from closing the dialog
    //! \param[in] key  QKeyEvent pointer that represents the pressed key
    //! \author elzubeir
    void keyPressEvent(QKeyEvent *key);

    //void changeEvent(QEvent *e);

private slots:
    //! \fn update()
    //!     the update function that is called by the timer's timeout() signal,
    //! it updates the GUI to reflect the latest changes from the ftp operation
    //! \author elzubeir
    void update();

    //! \fn update()
    //!     this function is called in response to threads terminated() singal
    //! \author elzubeir
    void threadTerminated();

private:
    //! pointer to the Qt desginer UI class object
    Ui::ImageCachingDialog *m_ui;

    //! object of the threading class
    ImageCachingThread m_thread;

    //! Timer object to do GUI updates, and check status of operations
    QTimer * m_timer;

    //! the size of the current file being processed by the ftp thread
    int m_currentFileSize;

    //! the number of completed files already processed by the ftp thread
    int m_totalFilesCompleted;

    //! the total number of files to be processed by the ftp thread
    int m_totalFiles;

    //! has the process been termianted by user
    bool m_terminated;

    //! has the process completed successfully
    bool m_completedSuccessfully;

    //! has a general error occured
    bool m_generalError;

    //! the errors string
    QString m_errorString;

    //! string list containing the source filenames
    QStringList m_srcFiles;

    //! string list containing the destination filenames
    QStringList m_dstFiles;

    //! hash object with name of the image, and has it been hashed or not
    QHash<QString, bool> &m_cachedImages;

    //! resume the thread of not
    bool m_resume;

    //! use ftp for uploading or not
    bool m_useFtp;

    //! ftp server address
    QString m_address;

    //! ftp username
    QString m_username;

    //! ftp password
    QString m_password;

    //! ftp port
    int m_port;
};

#endif // IMAGECACHINGDIALOG_H
