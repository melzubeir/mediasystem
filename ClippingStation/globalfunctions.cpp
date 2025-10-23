/************************************************************************
 * $Id: globalfunctions.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/globalfunctions.cpp $
 *
 ************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cstring>


#include "globalfunctions.h"

int g_currentUpload = 0;

ofstream g_logfile;

bool g_debug = false;




/*
//parse headers for Content-Length
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
        int r;
        long len = 0;

        r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
        if (r)
    *((long *) stream) = len;

        return size * nmemb;
}

// discard downloaded data
size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
        return size * nmemb;
}
*/

// read data to upload
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *f = (FILE*)stream;        //convert the stream fo file
    size_t n;

    if (ferror(f))
        return CURL_READFUNC_ABORT;

    n = fread(ptr, size, nmemb, f) * size;  //read bytes from the stream
    g_currentUpload += n;       //increase the uploaded data by n

    return n;
}


int upload(CURL *curlhandle, const char * remotepath, const char * localpath,
           long timeout, long tries, char * errorstr)
{
    FILE *f;
    CURLcode r = CURLE_GOT_NOTHING;
    int c;

    f = fopen(localpath, "rb");
    if (f == NULL) {
        perror(NULL);
        return 0;
    }

    curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);       //set the operation to upload

    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);      //set the file to remote path

    if (timeout)
        curl_easy_setopt(curlhandle, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);        //set timeout if timeout flag is set

    curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);       //this is the read functino that would read that data from f, as passed in the next line
    curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);      //where to read the data to upload from

    curl_easy_setopt(curlhandle, CURLOPT_FTPPORT, "-"); // disable passive mode
    curl_easy_setopt(curlhandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);  //create any missing directories if they do not exist
    //curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0L);   //no progress present
    //curl_easy_setopt(curlhandle, CURLOPT_PROGRESSFUNCTION, my_progress_func);

    for (c = 0; (r != CURLE_OK) && (c < tries); c++)    //try to upload mutilple times if tries > 0
    {
        curl_easy_setopt(curlhandle, CURLOPT_APPEND, 0L);   //this is the first time to uplaod the file
        r = curl_easy_perform(curlhandle);      //do the acutal upload
    }

    fclose(f);  //close the file

    if ( CURLE_OK == r )  //if operation is successfull, return 1
        return 1;
    else
    {   //otherwise, set the error string, and reutrn 0
        strcpy(errorstr, curl_easy_strerror(r));
        return 0;
    }
}


size_t writefunc(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out=(struct FtpFile *)stream;

    if(out && !out->stream)
    {
        /* open file for writing */
        out->stream=fopen(out->filename, "wb");

        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }

    return fwrite(buffer, size, nmemb, out->stream);
}


int download(CURL* curlhandle, const char * remotepath, const char * localpath,
             long timeout, long tries, char * strerror)
{
    FtpFile ftpfile=
    {
        localpath, // name to store the file as if succesful
        NULL
    };

    CURLcode res = CURLE_GOT_NOTHING;

    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, writefunc);    // Define our callback to get called when there's data to be written
    curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, &ftpfile);    // Set a pointer to our struct to pass to the callback


    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);    // Switch on full protocol/debug output

    res = curl_easy_perform(curlhandle);

    if(CURLE_OK != res)
    {
        /* we failed */
        strcpy(strerror, curl_easy_strerror(res));
        return 0;
    }

    if(ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */

    return 1;
}


void MyOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context)
    if(!g_debug)
        return;

    switch (type) {
        case QtDebugMsg:
            g_logfile << QTime::currentTime().toString().toLocal8Bit().data() << " Debug: \t" << msg.toLocal8Bit().data() << "\n";
            break;
        case QtCriticalMsg:
            g_logfile << QTime::currentTime().toString().toLocal8Bit().data() << " Critical: \t" << msg.toLocal8Bit().data() << "\n";
            break;
        case QtWarningMsg:
            //g_logfile << QTime::currentTime().toString().toLocal8Bit().data() << " Warning: \t" << msg.toLocal8Bit().data() << "\n";
            break;
        case QtFatalMsg:
            g_logfile << QTime::currentTime().toString().toLocal8Bit().data() <<  " Fatal: \t" << msg.toLocal8Bit().data() << "\n";
            abort();
        case QtInfoMsg:
            g_logfile << QTime::currentTime().toString().toLocal8Bit().data() << " Info: \t" << msg.toLocal8Bit().data() << "\n";
            break;
    }

    g_logfile.flush();
}
