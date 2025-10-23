//! \file clippingstation.h
/************************************************************************
 * $Id: clippingstation.h 667 2011-05-15 10:41:45Z elzubeir $
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
 *  $Date: 2011-05-15 14:41:45 +0400 (Sun, 15 May 2011) $
 *  $Author: elzubeir $
 *  $Revision: 667 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/clippingstation.h $
 *
 ************************************************************************/

#ifndef CLIPPINGSTATION_H
#define CLIPPINGSTATION_H

#include <QtWidgets>
#include <QtSql>
#include <QMainWindow>
#include <QModelIndex>

#include "user.h"
#include "publication.h"
#include "imagecachingdialog.h"
#include "tagswrapper.h"
#include "imagecutout.h"
#include "globalfunctions.h"
#include "searchpublicationlistdialog.h"
#include "wordcoordinates.h"
#include "tagcoordinates.h"

namespace Ui
{
    class ClippingStation;
}

//! \class ClippingStation
//!     The ClippingStation class is the main class devierd from the QMainWindow base class
//!   it incorporates all the functionality of the other modules, and preform the main operations, like
//!   clipping, drawer, importer

class ClippingStation : public QMainWindow
{
    Q_OBJECT

public:
    //! \fn ClippingStation()
    //!     default constructor
    //! \param[in] parent   QWidget pointer, set to zero, because this is the main window
    //! \author elzubeir
    ClippingStation(QWidget *parent = 0);


    //! \fn ~ClippingStation()
    //!     destructor
    //! \author elzubeir
    ~ClippingStation();

    //! \fn displayStatusMessage()
    //! \brief Display a message in the status bar.
    //! \param status the message.
    //! \author elzubeir
    void displayStatusMessage(const QString & status);

    //! \fn clearStatusMessage()
    //! \brief clear the status bar.
    //! \author elzubeir
    void clearStatusMessage();

    //! \fn displayCoordinatesStatus()
    //! \brief Display mouse coordinates in the status bar.
    //! \param x x mouse coordinate.
    //! \param y y mouse coordinate.
    //! \author elzubeir
    void displayCoordinatesStatus(qreal x, qreal y, qreal width = 0.0, qreal height = 0.0);

    //! \fn displayRotationAngleStatus()
    //! \brief Display angle in the status bar.
    //! \param status the message.
    //! \author elzubeir
    void displayRotationAngleStatus(qreal angle);

    //! \fn displayZoomStatus()
    //! \brief Display zoom in the status bar.
    //! \param status the message.
    //! \author elzubeir
    void displayZoomStatus(qreal zoom);

    //! \fn updateDrawerPageInformation()
    //!     update the page information for the drawer
    //! \author elzubeir
    void updateDrawerPageInformation();

    //! \fn thumbnail()
    //!     creates the thumbnail for the image containing the first cutout on the left pane
    //! \author elzubeir
    void thumbnail();

    //! \fn A4Scale()
    //!     returns the current scale for the A4 pages
    //! \return qreal   holding the current scale for the A4 page
    //! \author elzubeir
    qreal A4Scale();


    //! \fn calculatePosAndScale()
    //!     calculation the position for the drop image in drawer
    //! \param[in] cellSize size of the cell.
    //! \param[in] img dropped image.
    //! \param[in] type type of image (image, title or body)
    //! \param[in] resetarray to reset the array or not.
    //! \param[in] pos the postion of the image
    //! \param[in] scale the scale of that image.
    //! \return bool
    //!        - true if all the values are populated
    //!        - false otherwise
    //! \author elzubeir
    bool calculatePosAndScale(QSizeF cellSize, QImage &img, int type, bool resetarray, QPointF &pos, qreal &scale);

    //! \fn A4CellSize()
    //!     calculate the a4 cell size.
    //! \param[in] isfirst: is the first page in drawer.
    //! \return QSizeF the size of the cell
    //! \author elzubeir
    QSizeF A4CellSize(bool isfirst);

    //! \fn addTagsToTagWrapper()
    //!     add tags to the tag wrapper
    //! \param[in] tagIds: list of tags to be added
    //! \param[in] tagCoord: list of coordinates to be added
    //! \param[in] removeAndThenAdd: remove tags from wrapper and then add
    //! \return void
    //! \author elzubeir
    void addTagsToTagWrapper(QList<int> &tagIds, QStringList tagCoord, bool removedAndThenAdd = false);

    QStringList getRelatedKeywords(QString tag);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    //! \fn readSettings()
    //!     This function reads the settings for the application from a config.ini file
    //! and populate the member variables with the values read from the ini file
    //! \see connectToDatabase()
    //! \return bool
    //!        - true if all the values are populated
    //!        - false otherwise
    //! \author elzubeir
    bool readSettings();

    //! \fn connectToDatabase()
    //!     Connect to master and slave databases based on the valuess read from "config.ini" file for the connection strings
    //! \see readSettings()
    //! \return bool
    //!        - true if the two connections are established correctly
    //!        - false if error occurs
    //! \author elzubeir
    bool connectToDatabase();

    //! \fn authenticate()
    //!     Authenticate the user who wants to login, it calls the authenticate function in the user class, if authentication is successful, the user object
    //!     is populated with the apropriate username, password, and userid
    //! \return bool
    //!     - true if authenticate is true
    //!     - false otherwise
    //! \author elzubeir
    bool authenticate();


    //! \fn createActions()
    //!     create actions for the main Window, and attach them to their correspoding slots
    //! \author elzubeir
    void createActions();

    //! \fn loadImage()
    //!     loads image in the full page view based on the user's selection
    //! \return bool
    //!     true    if operation successful
    //!     false   if not
    //! \author elzubeir
    bool loadImage();

    //! \fn cache()
    //!     caches pages from the current selected issue, this is done in background
    //! \see cacheOnePage()
    //! \author elzubeir
    void cache();

    //! \fn cacheOnePage()
    //!     Caches one page from src to local cache directory
    //! \param[in] srcPage  QString path to the source page
    //! \param[in] dstPage  QString path to the destination page
    //! \return bool
    //!     true    if operation successful
    //!     false   if not
    //! \author elzubeir
    bool cacheOnePage(QString srcPage, QString dstPage);

    //! \fn recursiveDelete()
    //!     Deletes directory recursively
    //! \param[in] dir      QDir path to the parent directory to be deleted
    //! \return bool
    //!     true    if operation successful
    //!     false   if not
    //! \author elzubeir
    bool recursiveDelete(const QDir &dir );

    //! \fn gotoClipPage()
    //!     When an article from the article tree view is selected, this functions higlights the appropriate page on the issues tree view
    //! \see articleTreeViewClicked()
    //! \author elzubeir
    void gotoClipPage();

    //! \fn createClipImages()
    //!     Creats screenshots of the clipped images
    //! \return QList List of screenshots images
    //! \see saveClipImagesToFileServer(), saveClipImagesToFtpServer()
    //! \author elzubeir
    QList<QImage> createClipImages();

    //! \fn saveClipImagesToFileServer()
    //!     Saves the clip images to fileserver
    //! \param[in]  id_article  articles id
    //! \param[in]  list List of images to save
    //! \return bool
    //!     true    if operation successful
    //!     false   if not
    //! \see createClipImages(), saveClipImagesToFtpServer()
    //! \author elzubeir
    static int saveClipImagesToFileServer(QString currentIssue, int id_article, QList<QImage> &list);

    //! \fn saveClipImagesToFileServer()
    //!     Static function that saves the clip images to FTP server
    //! \param[in]  currentIssue    current issue date
    //! \param[in]  id_article      article id
    //! \param[in]  list            List of images to save
    //! \param[in]  fromPreclip     boolean to show is this clip is from a preclip, or a fresh one
    //! \param[in]  database        QSqlDatabase instante to update table
    //! \return bool
    //!     true    if operation successful
    //!     false   if not
    //! \see saveClipImagesToFileServer(), saveClipImagesToFtpServer()
    //! \author elzubeir
    static bool saveClipImagesToFtpServer (QString &currentIssue, int &id_article, QList<QImage> &list, bool fromPreclip, QSqlDatabase &database);

    //! \fn addTextToClip()
    //!     add ocr text to the clip
    //! \param[in]  id of the article
    //! \param[in]  list of image cutouts.
    //! \param[in]  database instance
    //! \returns true if operation successful, otherwise false
    //! \author elzubeir
    static bool addTextToClip(int id_article, QList<ImageCutout*> &list, QSqlDatabase &database, bool isArabic);

    //! \fn getTextFromCutout()
    //!     gets a text from the cutout
    //! \param[in]  rec rectangle of the cutout
    //! \param[in]  list of WordCoordinates containing all the words in the cutout
    //! \returns a concatenated string of the text in the cutout
    //! \author elzubeir

    static QString getTextFromCutout( QRect rect, QList<WordCoordinates> wc );

    //! \fn enableDrawerButtons()
    //!     enable/disable drawer buttons
    //! \param[in] enable       boolean to enable/disable buttons
    //! \author elzubeir
    void enableDrawerButtons(bool enable = true);

    //! \fn setModifyClipButton()
    //!     toggle clip buttons text between Modify and Clip
    //! \param[in] modify       boolean to toggle text
    //! \author elzubeir
    void setModifyClipButton(bool modify);

    //! \fn setModifyPreclipButton()
    //!     toggle clip buttons text between Modify and Preclip
    //! \param[in] modify       boolean to toggle text
    //! \author elzubeir
    void setModifyPreclipButton(bool modify);

    //! \fn totalNumberOfCutoutsInDrawer()
    //!     returns the total number of image cutouts in drawer's A4 pages
    //! \return int     total number of cutouts
    //! \author elzubeir
    int  totalNumberOfCutoutsInDrawer();

    //! \fn addMargintoPos()
    //!     add A4 margins offset to cutout postition in auto arrange of cutouts
    //! \param[out] pos     QPointF holding the value of the new position after adding the margins offset
    //! \param[in]  isfirst boolean, different margins for first page than other pages
    //! \see onActionAutoArrange()
    //! \author elzubeir
    void addMargintoPos(QPointF &pos, bool isfirst);

    //! \fn orderList()
    //!     order the cutouts list based on their order numbers
    //! \param[out]     cutouts QList<ImageCutout*> list of cutout images
    //! \see onActionAutoArrange()
    //! \author elzubeir
    void orderList(QList<ImageCutout*> &cutouts);

    //! \fn isImageLocked()
    //!     checks if the image is locked by other user
    //! \param[in]      image       image name to check if  locked
    //! \param[out]     lockedBy    id of the user locking the page
    //! \return bool
    //!     true    if image is locked
    //!     false   if not
    //! \see lockImage(), unlockImages()
    //! \author elzubeir
    bool isImageLocked(QString image, int &lockedBy);

    //! \fn isImageLocked()
    //!     lock the image by the current user
    //! \param[in]      image       image name to try to lock
    //! \return bool
    //!     true    if image is locked successfully
    //!     false   if not
    //! \see isImageLocked(), unlockImages()
    //! \author elzubeir
    bool lockImage(QString image);

    //! \fn isImageLocked()
    //!     unlock the image(s) locked by the current user
    //! \return bool
    //!     true    if image is unlocked successfully
    //!     false   if not
    //! \see isImageLocked(), lockImage()
    //! \author elzubeir
    bool unlockImages();

    //! \fn loadImageHighlights()
    //!     load keyword coordinates and add them to image scene
    //! \param[in]  name namge of the image to load
    //! \author elzubeir
    void loadImageHighlights(int imageId);

    //! \fn loadImageText()
    //!     load the full text for the current image
    //! \param[in]  name namge of the image to load
    //! \param[in]  database: instance of the database to load
    //! \returns a list of WordCoordinates that contains all the words and their coordinates sorted.
    //! \author elzubeir
    static QList<WordCoordinates> loadImageText(QString name, QSqlDatabase &database);

    //! \fn currentSectionPagesStatus()
    //!     loads the state of the pages in the current section.
    //! \param[in]  id of the section
    //! \returns a hash contains the page name with an icon indicationg its states (ocr processed, not processed, or waiting to be processed)
    //! \author elzubeir
    QHash<QString, QIcon> currentSectionPagesStatus(int sectionId);

    //! \fn emptyDrawerPages()
    //!     returns a list of the pages in drawers that contains no cutouts.
    //! \returns a list of names
    //! \author elzubeir
    QStringList emptyDrawerPages();

    //! \fn setShortcuts()
    //!     set the shortcuts from settings
    //! \author elzubeir
    void setShortcuts();

    int getImageId(const QString name);

private slots:

    //! \fn onActionImporter();
    //!     Shows the dockable widget that contains the importer module if its closed, and if its open, it does nothing
    //! \author elzubeir
    void onActionImporter();

    //! \fn on_rejectButton_clicked();
    //!     reject the article
    //! \author elzubeir
    void on_rejectButton_clicked();

    //! \fn onActionKeyword()
    //!     Shows/hides the dockable widget that contains the keywords module
    //! \author elzubeir
    void onActionKeyword();

    //! \fn onActionPublications()
    //!     Shows/hides the dockable widget that contains the publication module
    //! \author elzubeir
    void onActionPublications();

    //! \fn onActionClip()
    //!     creates a clip
    //! \author elzubeir
    void onActionClip();

    //! \fn onActionPreclip()
    //!     creates a preclip
    //! \author elzubeir
    void onActionPreclip();

    //! \fn onActionModifyePreclip()
    //!     modify a selected preclip from the preclipped article list
    //! \author elzubeir
    void onActionModifyePreclip();

    //! \fn onActionDeletePreclip()
    //!     delete a selected preclip from the preclipped article list
    //! \author elzubeir
    void onActionDeletePreclip();

    //! \fn onActionAutoArrange()
    //!     auto arranges the selected cutouts in the drawer
    //! \author elzubeir
    void onActionAutoArrange();

    //! \fn onActionMarkIssueAsDone()
    //!     marks the current issue as done
    //! \author elzubeir
    void onActionMarkIssueAsDone();

    //! \fn onActionMarkIssueAsUndone()
    //!     marks the current issue as not done
    //! \author elzubeir
    void onActionMarkIssueAsUndone();

    //! \fn onActionSendPageToOCR()
    //!     send the page to OCR
    //! \author elzubeir
    void onActionSendPageToOCR();

    //! \fn onActionSendIssueToOCR()
    //!     send issue back to OCR
    //! \author elzubeir
    void onActionSendIssueToOCR();

    //! \fn onActionViewPageText()
    //!     view the pages OCR text
    //! \author elzubeir
    void onActionViewPageText();

    //! \fn onActionViewClipText();
    //!     view the clips OCR text
    //! \author elzubeir
    void onActionViewClipText();


    void onActionViewKeywordsSearchword();

    void onActionGoToKeywordWikiPage();

    //! \fn onActionAbout()
    //!     view about dialog box
    //! \author elzubeir
    void onActionAbout();

    //! \fn loadIssues();
    //!     Show the issues for a particular publication selected
    //! \author elzubeir
    void loadIssues();

    //! \fn issueTreeViewItemClicked()
    //!     selecting of issues
    //! \param[in]  index   QModelIndex of the selected issue/section/page
    //! \author elzubeir
    void issueTreeViewItemClicked(QModelIndex);

    //! \fn articleTreeViewClicked()
    //!     selecting of articles
    //! \param[in]  index   QModelIndex of the selected article
    //! \author elzubeir
    void articleTreeViewClicked(QModelIndex);

    //! \fn previousPage()
    //!     navigate to the previous page in the current issue
    //! \author elzubeir
    void previousPage();

    //! \fn nextPage();
    //!     navigate to the next page in the current issue
    //! \author elzubeir
    void nextPage();

    //! \fn firstPage();
    //!     navigate to the first page in the current issue
    //! \author elzubeir
    void firstPage();

    //! \fn previousPage()
    //!     navigate to the last page in the current issue
    //! \author elzubeir
    void lastPage();

    //! \fn gotoPage();
    //!     navigate to the page sent by the gotoPageLineEdit variable
    //! \author elzubeir
    void gotoPage();

    //! \fn zoomOut()
    //!     rotate the current image but rotateAngle
    //! \param[in]  rotateAngle double, the rotation value
    //! \author elzubeir
    void rotate(double rotateAngle);

    //! \fn saveImageRotation();
    //!     save the current image by the value rotated in the rotate dialog
    //! \author elzubeir
    void saveImageRotation();

    //! \fn zoomOut()
    //!     zoom out the current image
    //! \author elzubeir
    qreal zoomOut();

    //! \fn zoomIn()
    //!     zoom in the current image
    //! \author elzubeir
    qreal zoomIn();

    //! \fn bestFit()
    //!     best fit the current image to the available screen
    //! \author elzubeir
    void bestFit();

    //! \fn widthFit()
    //!     fit the current image to the available width
    //! \author elzubeir
    void widthFit();

    //! \fn actualSize()
    //!     show the current image in its original size
    //! \author elzubeir
    qreal actualSize();

    //! \fn drawerZoomOut()
    //!     zoom out the drawer
    //! \author elzubeir
    qreal drawerZoomOut();

    //! \fn drawerZoomIn()
    //!     zoom in the drawer
    //! \author elzubeir
    qreal drawerZoomIn();

    //! \fn bestFit()
    //!     best fit the current image to the available screen
    //! \author elzubeir
    void drawerBestFit();

    //! \fn widthFit()
    //!     fit the current image to the available width
    //! \author elzubeir
    void drawerWidthFit();

    //! \fn actualSize()
    //!     show the current image in its original size
    //! \author elzubeir
    qreal drawerActualSize();

    //! \fn searchKeyword()
    //!     search for a keyword, and displays the results in table
    //! \author elzubeir
    void searchKeyword();

    //! \fn checkAllKeywords()
    //!     check all keywords in the table
    //! \author elzubeir
    void checkAllKeywords();

    //! \fn uncheckAllKeywords()
    //!     uncheck all keywords in the table
    //! \author elzubeir
    void uncheckAllKeywords();

    //! \fn selectKeyword()
    //!     select checked keywords and add them to the OCR list
    //! \author elzubeir
    void selectKeyword();

    //! \fn drawerResetPages()
    //!     reset drawer pages, and removes all the items from it
    //! \author elzubeir
    void drawerResetPages();

    //! \fn drawerAddPage()
    //!     add a page to drawer, with the broders and margins
    //! \param isfirst bool
    //!     - true : add page with top margin for first page
    //!     - fasle: add page with top margin for other pages
    //! \author elzubeir
    void drawerAddPage(bool isfirst = false);

    //! \fn drawerRemovePage()
    //!     remove the current active page in drawer, the function does not remove the first page
    //! \see drawerResetPages()
    //! \author elzubeir
    void drawerRemovePage();

    //! \fn drawerPreviousPage()
    //!     navigate to previous page in drawer
    //! \author elzubeir
    void drawerPreviousPage();

    //! \fn drawerNextPage()
    //!     navigate to next page in drawer
    //! \author elzubeir
    void drawerNextPage();

    //! \fn clipsTreeContextMenu()
    //!     show context menu in the clips/preclips tree
    //! \param[in] point QPoint position of the mouse when clicked
    //! \author elzubeir
    void clipsTreeContextMenu(QPoint point);

    //! \fn issueTreeContextMenu()
    //!     show context menu in the issues tree
    //! \param[in] point QPoint position of the mouse when clicked
    //! \author elzubeir
    void issueTreeContextMenu(QPoint point);

    void keywordsTreeContextMenu(QPoint point);

    //! \fn preclip()
    //!     create a preclip
    //! \see clip()
    //! \author elzubeir
    void preclip();

    //! \fn clip()
    //!     create a clip
    //! \see preclip()
    //! \author elzubeir
    void clip();

    //! \fn createThumbnail()
    //!     create a thumbnail picture for the first page in the full page view and add it to the first page in drawer
    //! \see clip()
    //! \author elzubeir
    void createThumbnail();

    //! \fn loadFilteredPublication()
    //!     load publication that has issues that contains preclips
    //! \param[in]  index   int represents the current index value from publicationComboBox
    //! \see loadIssuesWithPreclipsOnly()
    //! \author elzubeir
    void loadFilteredPublication(int index);

    //! \fn loadIssuesWithPreclipsOnly()
    //!     load issues that has preclips
    //! \see loadFilteredPublication()
    //! \author elzubeir
    void loadIssuesWithPreclipsOnly();

    void loadUndoneIssuesOnly();

    //! \fn loadFilteredIssues();
    //!     load issues that are filtered based on the filter criteria
    //! \see loadIssuesWithPreclipsOnly()
    //! \author elzubeir
    void loadFilteredIssues();

    //! \fn loadClippingImages()
    //!     loads clipping images for a particular image, it loads images from file server or ftp server
    //! \param[in]  clipid  int article id
    //! \see saveClipImagesToFileServer(), saveClipImagesToFtpServer()
    //! \author elzubeir
    void loadClippingImages(int clipid);

    //! \fn cancelModifyPreclip()
    //!     cancel modification of preclip
    //! \see setModifyPreclipButton()
    //! \author elzubeir
    void cancelModifyPreclip();

    //! \fn orderCutoutsSelections()
    //!     starts the ordering of image cutouts
    //! \author elzubeir
    void orderCutoutsSelections();

    //! \fn saveClipImagesToFtpServerWatcherFinished();
    //!     this is called when the saveClipImagesToFtpServer() function is finished
    //! \author elzubeir
    void saveClipImagesToFtpServerWatcherFinished();

    //! \fn saveClipImagesToFileServerWatcherFinished();
    //!     this is called when the saveClipImagesToFileServer() function is finished
    //! \author elzubeir
    void saveClipImagesToFileServerWatcherFinished();

    //! \fn addTextToClipWatcherFinished()
    //!     this is called when the addTextToClip() function is finished
    //! \author elzubeir
    void addTextToClipWatcherFinished();

    //! \fn updateSectionPagesStatus()
    //!     updates the current section's pages status
    //! \author elzubeir
    void updateSectionPagesStatus();

    //! \fn filterPublications()
    //!     filter the publications
    //! \author elzubeir
    void filterPublications();

    //! \fn pauseTracking()
    //!     pause Tracking
    //! \author elzubeir
    void pauseTracking();

    void updateTrackLabel();

    int publicationIssueId(int id_publication, QString issue_date);

    int issueSectionsId(int id_publication_issue, int id_section);

    void addRelatedKeywordsToTag(QList<TagCoordinates> &tagH);

    void onAppFocusChanged();

private:
    //! pointer to the Qt desginer UI class object
    Ui::ClippingStation *ui;

    //! Databases object
    QSqlDatabase m_masterDatabase;
    QSqlDatabase m_slaveDatabase;
    static QSqlDatabase m_ocrDatabase;
    static QSqlDatabase m_clipDatabase;

    //@{
    //! master database connection string

    //! master database host name
    QString m_masterDatabaseHost;

    //! master database name
    QString m_masterDatabaseName;

    //! master database username
    QString m_masterDatabaseUser;

    //! master database password
    QString m_masterDatabasePassword;

    //! master database port number
    int m_masterDatabasePort;

    //! slave database host name
    QString m_slaveDatabaseHost;

    //! slave database connection string

    //! slave database name
    QString m_slaveDatabaseName;

    //! slave database username
    QString m_slaveDatabaseUser;

    //! slave database password
    QString m_slaveDatabasePassword;

    //! slave database port number
    int m_slaveDatabasePort;

    //! ocr database host name
    QString m_ocrDatabaseHost;

    //! ocr database name
    QString m_ocrDatabaseName;

    //! ocr database username
    QString m_ocrDatabaseUser;

    //! ocr database password
    QString m_ocrDatabasePassword;

    //! ocr database port number
    int m_ocrDatabasePort;

    //@}

    //! use ftp for uploading or not
    bool m_useFtp;

    //! ftp server address
    static QString m_address;

    //! ftp username
    static QString m_username;

    //! ftp password
    static QString m_password;

    //! ftp port
    static int m_port;

    //! object of the User class, used for authentication, checking the roles, ..etc
    User m_user;

    //! hash object to hold name/id values for publications
    QHash<QString, int> m_publicationHash;

    //! hash object to hold name/id values for countries
    QHash<QString, int> m_countriesHash;

    //! publication object of the current opened publication
    Publication m_publication;

    //! tags wrapper object
    TagsWrapper m_tagsWrapper;

    //! current selected issue
    QString m_currentIssueDate;

    //! current selected section
    QString m_currentSectionName;

    //! list of images of current issue in local directory
    QStringList m_localImagesList;

    //! list of images of current issue in remote directory
    QStringList m_remoteImagesList;

    //! list of images of current preclipped article images in local directory
    QStringList m_preclippedLocalImagesList;

    //! list of images of current preclipped article images in remote directory
    QStringList m_preclippedRemoteImagesList;

    //! list of images of current clipped article images in local directory
    QStringList m_clippedLocalImagesList;

    //! list of images of current clipped article images in remote directory
    QStringList m_clippedRemoteImagesList;

    //! index of current selected image
    int m_imageIndex;

    //! index of current selected preclip image
    int m_clippedImageIndex;

    //! index of current selected clip image
    int m_preclippedImageIndex;

    //! index of current selected image model
    QModelIndex m_imageModelIndex;

    //! index of current selected preclip image model
    QModelIndex m_clippedImageModelIndex;

    //! index of current selected clip image model
    QModelIndex m_preclippedImageModelIndex;

    //! ImageCachingDialog object for caching images from file/ftp server
    ImageCachingDialog m_imageCachingDialog;

    //! hash object for holding image names and their caching status
    QHash<QString, bool> m_cachedImages;

    //! cache directory for the application
    static QString m_cacheDir;

    //! name of the first page in the clip, needed for the thumbnail creation
    QString m_firstPageInClip;

    //! current scale for the A4 page
    qreal m_currentA4Scale;

    //! running mode for the full page view
    RUNNING_MODE m_runningMode;

    //! filter mode for the publication view
    FILTER_MODE  m_filterMode;

    //! current preclip id number
    int m_currentPreclipId;

    //! current clip id number
    int m_currentClipId;

    //! a context menu for the preclipped articles, to modify and delete
    QMenu *m_preclipContextMenu;

    //! a context menu for the clipped articles, to view text
    QMenu *m_clipContextMenu;

    //! a context menu for issue, to re-send to OCR engine
    QMenu *m_issueContextMenu;

    //! a context menu for the issue pages, to re-send to OCR engine
    QMenu *m_issuePageContextMenu;

    QMenu *m_keywordsContextMenu;

    //! object to monitor the saveClipImagesToFtpServer future object
    QFutureWatcher<bool> m_saveClipImagesToFtpServerWatcher;

    //! object to monitor the saveClipImagesToFileServer future object
    QFutureWatcher<int> m_saveClipImagesToFileServerWatcher;

    QFutureWatcher<bool> m_addTextToClipWatcher;

    //! string to hold the error message returned by the ftp server for uploading the clipped images
    static QString saveClipImagesToFtpServerErrorMsg;

    //! image to hold the thumbnail
    static QImage m_thumbnail;

    //! the search dialog list
    SearchPublicationListDialog m_searchPublicationsDlg;

    //! string for the clip shortcut
    QString m_clipShortcut;

    //! string for the preclip shortcut
    QString m_preclipShortcut;

    //! string for the add page shortcut
    QString m_addPageShortcut;

    //! string for the next page shortcut
    QString m_nextPageShortcut;

    //! string for the previous page shortcut
    QString m_previousPageShortcut;

    //! string for the debug mode (on/off)
    QString m_debug;

    QString m_preclip;

    bool m_isPaused;

    QTimer m_trackingTimer;

    int m_trackingSeconds;

    int m_currentPageId;

    int m_maxTimeLimit;

};

#endif // CLIPPINGSTATION_H
