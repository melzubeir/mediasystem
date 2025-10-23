/************************************************************************
 * $Id: globalfunctions.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/globalfunctions.h $
 *
 ************************************************************************/

#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <curl/curl.h>

#include <iostream>
#include <fstream>

using namespace std;

#include <QtCore>

//! gloabl variable to hold the number of currently read bytes from the uploaded file
extern int g_currentUpload;
extern ofstream g_logfile;
extern bool g_debug;


//! \struct FtpFile
//!     structu to holds the filename, and the stream used to write the local file when an ftp get operation is performed
struct FtpFile {
  const char *filename;
  FILE *stream;
};

struct MarginValues{
    qreal top;
    qreal bottom;
    qreal left;
    qreal right;
};

//! \fn readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
//!     Reads block of data from stream of element count of nmemb, and each elements of size size, and stores it in prt
//! \param[out]  prt     void* to hold the read data
//! \param[in]  size    size_t holds the size of individual element to read
//! \param[in]  nmemb   size_t holds the number of elements to read
//! \param[in]  stream  void* holds the stream to read from
//! \return    size_t
//!     value > 0   the total number of elements successfully read
//!     value = 0   end of stream is reached
//!     value < 0   error happened
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream);

//! \fn upload(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries, char * strerror)
//!     Uploads the file to the ftp server
//! \param[in]  curlhandle  CURL handle to do the acutal ftp operation
//! \param[in]  remotepath  char* holds the remote files path
//! \param[in]  localpath   char* holds the local files path
//! \param[in]  timeout     whether to set a timeout for the ftp operation
//! \param[in]  tries       number of tries for the ftp operation, currently its set to 0
//! \param[out] strerror    char* to hold the error string if error happened

//! \return    integer
//!     value > 0   operations is successfull
//!     value <=0   error happend
int upload(CURL *curlhandle, const char * remotepath, const char * localpath,
           long timeout, long tries, char * strerror);


//! \fn writefunc(void *buffer, size_t size, size_t nmemb, void *stream)
//!     Write block of data from stream of element count of nmemb, and each elements of size size, and stores it in prt
//! \param[out]  prt    void* to hold the data to be written
//! \param[in]  size    size_t holds the size of each element to be written
//! \param[in]  nmemb   size_t holds the number of elements to be written
//! \param[in]  stream  void* holds the stream to write to
//! \return    size_t
//!     value > 0   the total number of elements successfully written
//!     value = 0   end of stream is reached
//!     value < 0   error happened

/*static */size_t writefunc(void *buffer, size_t size, size_t nmemb, void *stream);


//! \fn download(CURL* curlhandle, const char * remoepath, const char * localpath, long timeout, long tries, char * strerror)
//!     downloads the file from the ftp server
//! \param[in]  curlhandle  CURL handle to do the acutal ftp operation
//! \param[in]  remotepath  char* holds the remote files path
//! \param[in]  localpath   char* holds the local files path
//! \param[in]  timeout     whether to set a timeout for the ftp operation
//! \param[in]  tries       number of tries for the ftp operation, currently its set to 0
//! \param[out] strerror    char* to hold the error string if error happened

//! \return    integer
//!     value > 0   operations is successfull
//!     value <=0   error happend
int download(CURL* curlhandle, const char * remoepath, const char * localpath,
             long timeout, long tries, char * strerror);


void MyOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // GLOBALFUNCTIONS_H
