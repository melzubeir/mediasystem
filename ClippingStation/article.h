/************************************************************************
 * $Id: article.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/article.h $
 *
 ************************************************************************/

#ifndef ARTICLE_H
#define ARTICLE_H

#include <QtCore>
#include <QtSql>

#include "imagecutout.h"
#include "tag.h"

class Publication;

class Article
{
public:

    //! \fn Article()
    //!     default constructor
    //! \author elzubeir
    Article();

    //! \fn ~Article()
    //!     default destructor
    //! \author elzubeir
    ~Article();

    //! \fn operator ==()
    //!     assignment operator
    //! \author elzubeir
    bool operator == (const Article & pc);

    //! \fn setId()
    //!     sets the id for the article
    //! \param[in]  articleId   integer holding the article id
    //! \author elzubeir
    void setId(int articleId);

    //! \fn setPagename()
    //!     sets the pagename for the article
    //! \param[in]  pagename string holding the pagename
    //! \author elzubeir
    void setPagename(QString pagename);

    //! \fn setImageCutouts()
    //!     sets the list of imagecutouts for the article
    //! \param[in]  cutoutsList list of cutouts belonging to the article
    //! \author elzubeir
    void setImageCutouts(QList<ImageCutout *> cutoutsList);

    //! \fn updateImageCutouts()
    //!     updates the list of imagecutouts for the article
    //! \param[in]  database    database object to update the query
    //! \param[in]  cutoutsList list of cutouts belonging to the article
    //! \author elzubeir
    void updateImageCutouts(QSqlDatabase &database, QList<ImageCutout *> cutoutsList);

    //! \fn setTags()
    //!     sets the list of tages belongs to the article
    //! \param[in]  tags    list of tags belonging to the article
    //! \author elzubeir
    void setTags(QList<Tag> &tags);

    //! \fn setTags()
    //!     sets the list of tages belongs to the article
    //! \param[in]  database    database object to update the query
    //! \param[in]  tags        list of tags to be updated
    //! \param[in]  isArabic    boolean to show if the tags are arabic or not
    //! \author elzubeir
    void updateTags(QSqlDatabase &database, QList<Tag> &tags, bool isArabic);

    void updateProminence(QSqlDatabase &database, Publication &publication, QList<Tag> &tags, int createdBy);

    //! \fn setImagesNumber()
    //!     sets the number of images contained in the article
    //! \param[in]  imagesNumber    int holding the number of images in article
    //! \author elzubeir
    void setImagesNumber(int imagesNumber);

    //! \fn updateImagesNumber()
    //!     updates the number of images contained in the article
    //! \param[in]  database        database object to update the query
    //! \param[in]  imagesNumber    int holding the new value for number of images
    //! \author elzubeir
    void updateImagesNumber(QSqlDatabase &database, int imagesNumber);

    //! \fn updateImagesNumber()
    //!     updates the number of images contained in the article
    //! \param[in]  database        database object to update the query
    //! \param[in]  articleId    	the id of the article
    //! \param[in]  imagesNumber    int holding the new value for number of images
    //! \author elzubeir
    //! \author elzubeir
    static void updateImagesNumber(QSqlDatabase &database, int articleId, int imagesNumber);


    //! \fn setImagesLocalPath()
    //!     sets the path for the local images belonging to the article
    //! \param[in]  path    string/string hash holding the value of section pagename, and its local location path
    //! \author elzubeir
    void setImagesLocalPath(QHash<QString, QString>  path);

    //! \fn setImagesRemotePath()
    //!     sets the path for the remote images belonging to the article
    //! \param[in]  path    string/string hash holding the value of section pagename, and its remote location path (could be file server or ftp server)
    //! \author elzubeir
    void setImagesRemotePath(QHash<QString, QString>  path);

    //! \fn setStatus()
    //!     sets the article status
    //! \param[in]  status  int holding the status of article
    //! \author elzubeir
    void setStatus(int status);

    //! \fn updateStatus()
    //!     updates the status of the article
    //! \param[in]  database    database object to update the query
    //! \param[in]  status      int the new status value for the article
    //! \author elzubeir
    void updateStatus(QSqlDatabase &database, int status);

    static void updateText(QSqlDatabase &database, int id, QString text);

    static void updateHeadline(QSqlDatabase &database, int id, QString headline, bool isArabic);

	//! \fn updateStatus()
    //!     static function to updates the status of the article passed by its id
    //! \param[in]  database    database object to update the query
    //! \param[in]  articleId   int the id of the article to update its status
    //! \param[in]  status      int the new status value for the article
    //! \author elzubeir
    static void updateStatus(QSqlDatabase &database, int articleId, int status);

    //! \fn deleteArticle()
    //!     delete the article from database
    //! \param[in]  database    database object to update the query
    //! \author elzubeir
    void deleteArticle(QSqlDatabase &database);

    //! \fn deleteArticle()
    //!     static version to delete an article from database
    //! \param[in]  database    database object to update the query
    //! \param[in]  articleId   int the id of the article to be deleted
    //! \author elzubeir
    static void deleteArticle(QSqlDatabase &database, int articleId);

    //! \fn updateArticle()
    //!     update the article with different values
    //! \param[in]  database    database object to update the query
    //! \param[in]  publication Publication object holds the publication this article belongs to
    //! \param[in]  tags        list of new tags belonging to this article
    //! \param[in]  cutoutsList list of new image cutouts belonging to this article
    //! \param[in]  imageSize   float the new size of the images for the article
    //! \author elzubeir
    bool updateArticle(QSqlDatabase &database, Publication &publication, QList<Tag> &tags, QList<ImageCutout *> &cutoutsList, qreal imageSize);

    //! \fn getImagesLocalAndRemoateFiles()
    //!     returns the local and remoate list of images belonging to the article
    //! \param[out] locafiles       list of local image paths
    //! \param[out] remoatefiles    list of remoate image paths
    //! \author elzubeir
    void getImagesLocalAndRemoateFiles(QStringList &localfiles, QStringList &remotefiles);

    //! \fn id()
    //!     returns the id of the article
    //! \return int holding the article id
    //! \author elzubeir
    int id();

    //! \fn pageNames()
    //!     returns list of page names contained in the article
    //! \return string list holding the article page names
    //! \author elzubeir
    QStringList pageNames();

    //! \fn imageCutoutsList()
    //!     returns list of cutouts of the article
    //! \return image cutout list in the article
    //! \author elzubeir
    QList<ImageCutout *> imageCutoutsList();

    //! \fn tags()
    //!     returns list of tags of the article
    //! \return tags list in the article
    //! \author elzubeir
    QList<Tag> tags();

    //! \fn status()
    //!     returns the status the article
    //! \return int represents the status of article
    //! \author elzubeir
    int status();

    //! \fn imagesNumber()
    //!     returns the number of images in the article
    //! \return int represents the number of images in article
    //! \author elzubeir
    int imagesNumber();

    //! \fn preclippedArticles()
    //!     returns a list of preclippedarticles based on passed parameters
    //! \param[in]  database        database object to update the query
    //! \param[in]  publicationId   Publication object holds the publication this article belongs to
    //! \param[in]  issueNumber     int issueNumber of the required articles
    //! \param[in]  localIssuePath  string holding the local path for the issue
    //! \param[in]  remoteIssuePath string holding the remote path for the issue
    //! \return list of preclipped articles
    //! \author elzubeir
    static QList<Article> preclippedArticles          (QSqlDatabase &database, int publicationId, int issueNumber,
                                                                QString &localIssuePath, QString &remoteIssuePath);
    //! \fn clippedArticles()
    //!     returns a list of clipped articles based on passed parameters
    //! \param[in]  database        database object to update the query
    //! \param[in]  publicationId   Publication object holds the publication this article belongs to
    //! \param[in]  issueNumber     int issueNumber of the required articles
    //! \param[in]  localIssuePath  string holding the local path for the issue
    //! \param[in]  remoteIssuePath string holding the remote path for the issue
    //! \return list of clipped articles
    //! \author elzubeir
    static QList<Article> clippedArticles             (QSqlDatabase &database, int publicationId, int issueNumber,
                                                                QString &localIssuePath, QString &remoteIssuePath);
    //! \fn article()
    //!     returns an article based on passed parameters
    //! \param[in]  database        database object to update the query
    //! \param[in]  articleId       int id of the article to return
    //! \param[in]  localIssuePath  string holding the local path for the issue
    //! \param[in]  remoteIssuePath string holding the remote path for the issue
    //! \return article object
    //! \author elzubeir
    static      Article  article                      (QSqlDatabase &database, int articleId, QString &localIssuePath, QString &remoteIssuePath);

    //! \fn clip()
    //!     static function to insert a new article to the article's table
    //! \param[in]  database        database object to insert the query
    //! \param[in]  publication     Publication object that the article belongs to
    //! \param[in]  createdBy       int the user id who created the article
    //! \param[in]  tags            list of tags to insert to this article
    //! \param[in]  cutoutsList     list of image cutouts to insert to this article
    //! \param[in]  section         string the section name containing the article
    //! \param[in]  imageSize       float the total size of the images for the article
    //! \param[in]  imagesNumber    int holding the number of images in article
    //! \param[in]  clippingStatus  int status of the article
    //! \param[in]  text_ocr        int text of the article
    //! \return int value holding the id for the newley created article
    //! \author elzubeir
    static int clip(QSqlDatabase &database, Publication &publication, int createdBy, QList<Tag> &tags,
                   QList<ImageCutout *> &cutoutsList, QString section, qreal imageSize, int imagesNumber, int clippingStatus, QString text_ocr = "");
private:

    //! \fn setCippingCoordinates()
    //!     static function to set the image cutouts clipping corrdinates
    //! \param[in]  database        database object to update the query
    //! \param[in]  articleId       int id of the article to return
    //! \param[in]  cutoutsList     list of cutouts of the article
    //! \author elzubeir
    static void setCippingCoordinates(QSqlDatabase &database, int articleId, QList<ImageCutout*> &cutoutsList);

    //! \fn articles()
    //!     static function to calculate the total size of the article
    //! \param[in]  publication     Publication object that the article belongs to
    //! \param[in]  imageSize       the size of the total image
    //! \param[out] sizeL           float the total length of all the image cutouts of the article
    //! \param[out] sizeW           float the total width of all the image cutouts of the article
    //! \param[out] size            float the total size of all the cutouts in the article, if the article is newspaper
    //! \param[out] realSize        float the total size of all the cutouts in the article, if the article is magazine
    //! \return float value represents the total AVE of the article
    //! \author elzubeir
    static void getSize(Publication &publication, qreal imageSize, qreal &sizeL, qreal &sizeW, qreal &size,
                        qreal &realSize, QList<ImageCutout *> &cutoutsList);

    //! \fn articles()
    //!     static function to calculate the total AVE of the article
    //! \param[in]  isMagazine      boolean if the article is magazine or newspaper
    //! \param[in]  adrate          float the add rate value for the article
    //! \param[in]  size            float the size of the article
    //! \param[in]  realSize        float the real size of the article, needed when
    //! \return float value represents the total AVE of the article
    //! \author elzubeir
    static qreal calculateAVE (bool isMagazine, qreal adrate, qreal size, qreal real_size, bool round);

    //! \fn articles()
    //!     static function to return a list of articles based on passed parameters
    //! \param[in]  database        database object to update the query
    //! \param[in]  publicationId   Publication object holds the publication this article belongs to
    //! \param[in]  issueNumber     int issueNumber of the required articles
    //! \param[in]  localIssuePath  string holding the local path for the issue
    //! \param[in]  remoteIssuePath string holding the remote path for the issue
    //! \param[in]  clippingStatus  int status of the clippings to search for
    //! \return list of articles
    //! \author elzubeir
    static QList<Article> articles (QSqlDatabase &database, int publicationId, int issueNumber,
                                    QString &localIssuePath, QString &remoteIssuePath, int clippingStatus);

    //! \fn setArticleTowrite()
    //!     sets the article attribute to write
    //! \param[in]  database    database object to update the query
    //! \param[in]  idArticle   int article's id
    //! \param[in]  toWrite     int to write value for the article
    //! \param[in]  isArabic    boolean is this article an arabic one or not
    //! \author elzubeir
    static void setArticleTowrite(QSqlDatabase &database, int idArticle, int toWrite, bool isArabic);

    static void insertPromience(QSqlDatabase &database, Publication &publication, int idArticle, QList<Tag> &tags, int createdBy);

private:

    //! the article id
    int m_id;

    //! the pagename will be the section ID, and the pagename
    QString m_pagename;

    //! hash of the section name/local image paths for the article
    QHash<QString, QString>  m_imagesLocalPath;

    //! hash of the section name/remote image paths for the article
    QHash<QString, QString>  m_imagesRemotePath;

    //! list of image cutouts in the article
    QList<ImageCutout *> m_imageCutoutsList;

    //! list of tags in the article
    QList<Tag> m_tags;

    //! status of the article
    int m_status;

    //! number of images contained in the article
    int m_imagesNumber;
};

#endif // ARTICLE_H
