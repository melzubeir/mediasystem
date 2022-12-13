//! \file clippingstation.cpp
/************************************************************************
 * $Id: clippingstation.cpp 668 2011-05-15 11:08:10Z elzubeir $
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
 *  $Date: 2011-05-15 15:08:10 +0400 (Sun, 15 May 2011) $
 *  $Author: elzubeir $
 *  $Revision: 668 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/clippingstation.cpp $
 *
 *
 ************************************************************************/

#include <QtGui>
#include <QtConcurrentRun>
#include <QMutex>
#include <QDesktopServices>

#include <cstdio>
using namespace std;


#include "clippingstation.h"
#include "ui_clippingstation.h"
#include "Importer/importerdialog.h"
#include "publication.h"
#include "tag.h"
#include "pushbuttondelegate.h"
#include "tagswrapper.h"
#include "imagecutout.h"
#include "drawerview.h"
#include "systemconfiguration.h"
#include "constants.h"
#include "pagetextdialog.h"
#include "wordcoordinates.h"
#include "searchpublicationlistdialog.h"

/*
TODO: if caching has been canceled by user, or by selecting a different issue while the first one has not completed caching yet!
It should be handled, not handled yet!

*/

QMutex mutex;

QString ClippingStation::m_cacheDir;
QString ClippingStation::m_address;
QString ClippingStation::m_username;
QString ClippingStation::m_password;
int ClippingStation::m_port;
QString ClippingStation::saveClipImagesToFtpServerErrorMsg;
QImage ClippingStation::m_thumbnail;
QSqlDatabase ClippingStation::m_ocrDatabase;
QSqlDatabase ClippingStation::m_clipDatabase;

ClippingStation::ClippingStation(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::ClippingStation),
        m_imageCachingDialog(this, m_cachedImages)
{
    if ( !readSettings())       //if you cant read settings, exit
        exit(0);

    if( !connectToDatabase() )  //if cant connect to database, exit
        exit(0);

    if ( !authenticate() )      //if cant authenticate, exit
        exit(0);


    ui->setupUi(this);          //else, create the UI

    if( m_debug.compare("on", Qt::CaseInsensitive) == 0 )
    {
        g_logfile.open("clippingstation.log", ios::app);
        g_logfile << std::string(80, '=') << std::endl;
        g_debug = true;
    }
    else
        g_debug = false;

    //create the actions
    createActions();

    if( m_preclip.compare("on", Qt::CaseInsensitive) == 0 )
    {
        m_preclipContextMenu = new QMenu(this);
        m_preclipContextMenu->addAction(ui->actionModifyPreclip);
        m_preclipContextMenu->addSeparator();
        m_preclipContextMenu->addAction(ui->actionDeletePreclip);
    }
    else
    {
        ui->preclipButton->setEnabled(false);
        ui->cancelModifyPreclipButton->setEnabled(false);
    }

    m_clipContextMenu = new QMenu(this);
    m_clipContextMenu->addAction(ui->actionViewClipText);

    m_issueContextMenu = new QMenu(this);
    m_issueContextMenu->addAction(ui->actionSendIssuetoORC);

    m_issuePageContextMenu = new QMenu(this);
    m_issuePageContextMenu->addAction(ui->actionSendPagetoORC);
    m_issuePageContextMenu->addSeparator();
    m_issuePageContextMenu->addAction(ui->actionViewPageText);

    m_keywordsContextMenu = new QMenu(this);
    m_keywordsContextMenu->addAction(ui->actionViewKeywordsSearchword);
    m_keywordsContextMenu->addSeparator();
    m_keywordsContextMenu->addAction(ui->actionGoToKeywordWikiPage);


    m_publication.setDatabase(m_masterDatabase);
    m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);


    //populate all the publications in the publicaiton/issue drop-down list in the dock widget
    QStringList list;
    QSqlQuery query("SELECT id_publication, name_publication_en FROM publication", m_slaveDatabase);
    while( query.next() )
    {
        int publicationId = query.value(0).toString().toInt();
        QString publicationName = query.value(1).toString();
        list << publicationName;        //populate list with values from the database

        m_publicationHash[publicationName] = publicationId;     //also populate the hash table with name/id values
    }

    //load keywords from database and add them to searchPublicationdialog
    QStringList keywords;
    query.exec( "SELECT name_company FROM company WHERE date_end > NOW()" );
    while( query.next() )
    {
        QString name = query.value(0).toString();
        keywords.append(name);
    }

    query.exec("SELECT name_searchword  FROM company AS cm, searchword AS sw WHERE sw.id_company = cm.id_company AND cm.date_end > NOW() ");
    while( query.next() )
    {
        QString name = query.value(0).toString();
        keywords.append(name);
    }

    keywords.sort();
    keywords.removeDuplicates();
    m_searchPublicationsDlg.setKeywords(keywords);

    query.exec("SELECT id_country, name_country_en FROM country");
    while( query.next() )
    {
        int id = query.value(0).toString().toInt();
        QString name = query.value(1).toString();

        m_countriesHash[name] = id;     //also populate the hash table with name/id values
    }

    QList<QString> clist = m_countriesHash.keys();
    QStringList strlist;
    for(int i=0; i < clist.count(); i++)
        strlist.append(clist[i]);

    m_searchPublicationsDlg.setCountryList( strlist );


    QCompleter* completer = new QCompleter(list, this);     //createa completer with the list of publication names
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->publicationNameLineEdit_2->setCompleter(completer);   //use the complete with the publicationLineEdit

    // should use QDir's homePath to get the path and then use .socialhose/cache as the directory within the homePath
    QDir homeDir("/home");
#if defined Q_WS_X11
    m_cacheDir = homeDir.homePath() + "/.socialhose/cache/";
#elif defined Q_WS_MACX
    m_cacheDir = homeDir.homePath() + "/.socialhose/cache/";
#else
    m_cacheDir = QApplication::applicationDirPath();
    if(!m_cacheDir.endsWith("/"))
      m_cacheDir += "/";
    m_cacheDir += "cache/";
#endif

    ui->fullpageView->setClippingStation(this);
    ui->statusBar->addProgressBarWidget(&m_imageCachingDialog);

    ImporterDialog *dlg = new ImporterDialog(this, m_masterDatabase, m_slaveDatabase, m_user); //create the importer dialog
    ui->importerDockWidget->setWidget(dlg); //and insert it to the importer dock widget

    tabifyDockWidget(ui->importerDockWidget, ui->publicatonIssueDockWidget);
    ui->importerDockWidget          ->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->publicatonIssueDockWidget   ->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->importerDockWidget          ->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //ui->importerDockWidget->raise();

    SystemConfiguration::retreiveSystemConfiguration(m_slaveDatabase);

    m_tagsWrapper.set(ui->treeView, ui->treeWidget, ui->keywordLineEdit);

    ui->stackedDrawerViewWidget->removeWidget(ui->stackedDrawerViewWidget->widget(0) ); //remove the first page from the stak

    m_currentA4Scale = 1.0;

    drawerAddPage(true);

    m_runningMode = NORMAL_MODE;
    m_filterMode  = NO_FILTER;

    m_currentPreclipId = 0;
    m_firstPageInClip = ""; //reset it
    m_imageIndex = m_preclippedImageIndex = m_clippedImageIndex = 0;
    m_imageModelIndex = m_preclippedImageModelIndex = m_clippedImageModelIndex = QModelIndex();

    showMaximized();

    ui->cancelModifyPreclipButton->setVisible(false);

    //install filter to track mouse
    ui->fullpageView->installEventFilter(this);

    connect(&m_saveClipImagesToFtpServerWatcher, SIGNAL(finished() ), this, SLOT( saveClipImagesToFtpServerWatcherFinished() ) ); //this signal is used to upload clips to ftp server
    connect(&m_saveClipImagesToFileServerWatcher, SIGNAL(finished() ), this, SLOT( saveClipImagesToFileServerWatcherFinished() ) ); //this signal is used to upload clips to ftp server
    connect(&m_addTextToClipWatcher, SIGNAL(finished()), this, SLOT(addTextToClipWatcherFinished()));

    //set shortcuts
    setShortcuts();

    connect(&m_trackingTimer, SIGNAL(timeout()), this, SLOT(updateTrackLabel()));
    m_trackingTimer.setInterval(1000);
    m_trackingSeconds = 0;
    m_currentPageId = 0;
    m_isPaused = false;
}

ClippingStation::~ClippingStation()
{
    //qDebug("~ClippingStation()");

    delete ui;

    unlockImages();

    m_trackingTimer.stop();
    if(m_currentPageId > 0 && m_trackingSeconds > 0)
        m_user.updateTrack(m_currentPageId, m_trackingSeconds);

    m_masterDatabase.close();
    m_slaveDatabase.close();

    //delete all the files on the tmp directory
    //QDir dir(m_cacheDir);
    //recursiveDelete( dir );
    //qDebug("end of ~ClippingStation()");
}

bool ClippingStation::readSettings()
{
#ifdef Q_WS_X11
  QString config = "/etc/socialhose.ini";
#elif defined (Q_WS_MACX)
  QString config = "/etc/socialhose.ini";
#else
  QString config = QApplication::applicationDirPath()+ "/config.ini";
#endif

    QSettings settings(config, QSettings::IniFormat);

    m_masterDatabaseHost        = settings.value("master database/server").toString();          //read master database's server value from settings
    m_masterDatabaseName        = settings.value("master database/database").toString();        //read master database's database name value from settings
    m_masterDatabaseUser        = settings.value("master database/uid").toString();             //read master database's user id value from settings
    m_masterDatabasePassword    = settings.value("master database/pwd").toString();             //read master database's password value from settings
    m_masterDatabasePort        = settings.value("master database/port").toInt();               //read master database's port value from settings

    // if any string is empty return false, else return true
    if(m_masterDatabaseName.isEmpty() || m_masterDatabaseHost.isEmpty() || m_masterDatabaseUser.isEmpty() ||
       m_masterDatabasePassword.isEmpty() || ( m_masterDatabasePort == 0) )
    {
        QMessageBox::critical(this, tr("config error"), tr("Error reading 'config.ini' master database configurations\nMake sure the file exists, and its properly formatted!"));
        return false;
    }


    m_slaveDatabaseHost        = settings.value("slave database/server").toString();
    m_slaveDatabaseName        = settings.value("slave database/database").toString();
    m_slaveDatabaseUser        = settings.value("slave database/uid").toString();
    m_slaveDatabasePassword    = settings.value("slave database/pwd").toString();
    m_slaveDatabasePort        = settings.value("slave database/port").toInt();

    // if any string is empty return false, else return true
    if(m_slaveDatabaseName.isEmpty() || m_slaveDatabaseHost.isEmpty() || m_slaveDatabaseUser.isEmpty() ||
       m_slaveDatabasePassword.isEmpty() || ( m_slaveDatabasePort == 0) )
    {
        QMessageBox::critical(this, tr("config error"), tr("Error reading 'config.ini' slave database configurations\nMake sure the file exists, and its properly formatted!"));
        return false;
    }

    m_ocrDatabaseHost        = settings.value("ocr database/server").toString();
    m_ocrDatabaseName        = settings.value("ocr database/database").toString();
    m_ocrDatabaseUser        = settings.value("ocr database/uid").toString();
    m_ocrDatabasePassword    = settings.value("ocr database/pwd").toString();
    m_ocrDatabasePort        = settings.value("ocr database/port").toInt();


    m_useFtp        = settings.value("ftp access/useftp").toInt();    //if its set to 0, its false, otherwise it true
    if(m_useFtp)
    {
        m_address   = settings.value("ftp access/server").toString();
        m_username  = settings.value("ftp access/username").toString();
        m_password  = settings.value("ftp access/password").toString();
        m_port      = settings.value("ftp access/port").toInt();

        if(m_address.isEmpty() || m_username.isEmpty() || m_password.isEmpty() || (m_port == 0))
        {
            QMessageBox::critical(this, tr("config error"), tr("Error reading 'config.ini' Ftp server configurations\nMake sure the file is properly formatted!"));
            return false;
        }
    }

    //read shortcuts
    m_clipShortcut      = settings.value("shortcuts/Clip").toString();
    m_preclipShortcut   = settings.value("shortcuts/Preclip").toString();
    m_addPageShortcut   = settings.value("shortcuts/AddPage").toString();
    m_nextPageShortcut  = settings.value("shortcuts/NextPage").toString();
    m_previousPageShortcut  = settings.value("shortcuts/PreviousPage").toString();

    m_debug             = settings.value("debug mode/debug").toString();

    m_preclip           = settings.value("preclip mode/preclip", "on").toString();

    m_maxTimeLimit      = settings.value("time limit/limit").toInt();

    return true;
}

bool ClippingStation::connectToDatabase()
{
    // add a connection to databse based on the mysql driver, and name it master
    m_masterDatabase = QSqlDatabase::addDatabase("QMYSQL", "master_123");

    // initalize the connection
    m_masterDatabase.setHostName(m_masterDatabaseHost);
    m_masterDatabase.setPort(m_masterDatabasePort);
    m_masterDatabase.setDatabaseName(m_masterDatabaseName);
    m_masterDatabase.setUserName(m_masterDatabaseUser);
    m_masterDatabase.setPassword(m_masterDatabasePassword);

    // try to open the connection, if ok returns true, else returns false
    bool ok = m_masterDatabase.open();
    if(!ok)
    {
        // if error, get the error from the database text and print it to debug screen
        QString str = m_masterDatabase.lastError().databaseText();
        QMessageBox::critical(this, tr("Database Error"), tr("Error connecting to master database\n") + str);
        return false;
    }

    // add a connection to databse based on the mysql driver, and name it slave
    m_slaveDatabase = QSqlDatabase::addDatabase("QMYSQL", "slave");

    // initalize the connection
    m_slaveDatabase.setHostName(m_slaveDatabaseHost);
    m_slaveDatabase.setPort(m_slaveDatabasePort);
    m_slaveDatabase.setDatabaseName(m_slaveDatabaseName);
    m_slaveDatabase.setUserName(m_slaveDatabaseUser);
    m_slaveDatabase.setPassword(m_slaveDatabasePassword);

    // try to open the connection, if ok returns true, else returns false
    ok = m_slaveDatabase.open();
    if(!ok)
    {
        // if error, get the error from the database text and print it to debug screen
        QString str = m_slaveDatabase.lastError().databaseText();
        QMessageBox::critical(this, tr("Database Error"), tr("Error connecting to slave database\n") + str);
        return false;
    }


    m_ocrDatabase = QSqlDatabase::addDatabase("QMYSQL", "ocr");

    // initalize the connection
    m_ocrDatabase.setHostName(m_ocrDatabaseHost);
    m_ocrDatabase.setPort(m_ocrDatabasePort);
    m_ocrDatabase.setDatabaseName(m_ocrDatabaseName);
    m_ocrDatabase.setUserName(m_ocrDatabaseUser);
    m_ocrDatabase.setPassword(m_ocrDatabasePassword);

    // try to open the connection, if ok returns true, else returns false
    ok = m_ocrDatabase.open();
    if(!ok)
    {
        // if error, get the error from the database text and print it to debug screen
        QString str = m_ocrDatabase.lastError().databaseText();
        QMessageBox::critical(this, tr("Database Error"), tr("Error connecting to ocr database\n") + str);
        return false;
    }

    m_clipDatabase = m_masterDatabase.cloneDatabase(m_masterDatabase, "clip");
    m_clipDatabase.open();

    if(!m_clipDatabase.isOpen())
    {
        QString str = m_clipDatabase.lastError().text();
        QMessageBox::critical(this, tr("Database Error"), tr("Error opening clip database\n") + str);
        return false;
    }

    return true;
}

bool ClippingStation::authenticate()
{
    //after opening the database connection, we pass it to the user object
    m_user.setSlaveDatabase(m_slaveDatabase);
    m_user.setMasterDatabase(m_masterDatabase);

    return m_user.authenticate();
}


void ClippingStation::createActions()
{
    //create actions for the main window
    connect(ui->actionImporter,         SIGNAL(triggered()), this, SLOT(onActionImporter()));
    connect(ui->action_Exit,            SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionKeyword,          SIGNAL(triggered()), this, SLOT(onActionKeyword()));
    connect(ui->actionPublications,     SIGNAL(triggered()), this, SLOT(onActionPublications()));

    connect(ui->actionClip,             SIGNAL(triggered()), this, SLOT(onActionClip()));

    if( m_preclip.compare("on", Qt::CaseInsensitive) == 0 )
    {
        connect(ui->actionPreclip,          SIGNAL(triggered()), this, SLOT(onActionPreclip()));
        connect(ui->actionModifyPreclip,    SIGNAL(triggered()), this, SLOT(onActionModifyePreclip()));
        connect(ui->actionDeletePreclip,    SIGNAL(triggered()), this, SLOT(onActionDeletePreclip()));
    }
    //drawer actions
    connect(ui->actionDrawerAddPage,    SIGNAL(triggered()), this, SLOT(drawerAddPage()));
    connect(ui->actionDrawerRemovePage, SIGNAL(triggered()), this, SLOT(drawerRemovePage()));

    //page navigation actions
    connect(ui->actionFirstPage,        SIGNAL(triggered()), this, SLOT(firstPage()));
    connect(ui->actionPreviousPage,     SIGNAL(triggered()), this, SLOT(previousPage()));
    connect(ui->actionNextPage,         SIGNAL(triggered()), this, SLOT(nextPage()));
    connect(ui->actionLastPage,         SIGNAL(triggered()), this, SLOT(lastPage()));

    //zoom in/out
    connect(ui->actionZoomIn,           SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(ui->actionZoomOut,          SIGNAL(triggered()), this, SLOT(zoomOut()));

    //auto-arrange
    connect(ui->actionAutoArrange,      SIGNAL(triggered()), this, SLOT(onActionAutoArrange()));

    connect(ui->actionMarkIssueAsDone,  SIGNAL(triggered()), this, SLOT(onActionMarkIssueAsDone()));
    connect(ui->actionMarkIssueAsUndone,SIGNAL(triggered()), this, SLOT(onActionMarkIssueAsUndone()));
    connect(ui->actionSendIssuetoORC,   SIGNAL(triggered()), this, SLOT(onActionSendIssueToOCR()));
    connect(ui->actionSendPagetoORC,    SIGNAL(triggered()), this, SLOT(onActionSendPageToOCR()));
    connect(ui->actionViewPageText,     SIGNAL(triggered()), this, SLOT(onActionViewPageText()));

    connect(ui->actionViewClipText,     SIGNAL(triggered()), this, SLOT(onActionViewClipText()));

    connect(ui->actionAbout,            SIGNAL(triggered()), this, SLOT(onActionAbout()));

    connect(ui->actionViewKeywordsSearchword, SIGNAL(triggered()), this, SLOT(onActionViewKeywordsSearchword()));
    connect(ui->actionGoToKeywordWikiPage   , SIGNAL(triggered()), this, SLOT(onActionGoToKeywordWikiPage()));
}

void ClippingStation::onActionImporter()
{
    ui->importerDockWidget->setVisible(ui->actionImporter->isChecked());
}

void ClippingStation::onActionKeyword()
{
    ui->keywordDockWidget->setVisible(ui->actionKeyword->isChecked());
}

void ClippingStation::onActionPublications()
{
    ui->publicatonIssueDockWidget->setVisible(ui->actionPublications->isChecked());
}

void ClippingStation::onActionClip()
{
    clip();
}

void ClippingStation::onActionPreclip()
{
    preclip();
}


void ClippingStation::onActionModifyePreclip()
{
    QModelIndexList list = ui->clipsTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];

    articleTreeViewClicked(index);
    setModifyPreclipButton(true);
}


void ClippingStation::onActionDeletePreclip()
{
    QModelIndexList list = ui->clipsTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];
    if(index.isValid())
    {
        //check index to the value
        QAbstractItemModel *model = ui->clipsTreeView->model();

        QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );
        QModelIndex clipRootIndex    = model->index(1, 0, QModelIndex() );
        //if the user clicked on top level entries, just return
        if ( index ==  preclipRootIndex  ||
             index ==  clipRootIndex         )
            return;

        int id;
        if ( index.parent() == preclipRootIndex ||
             index.parent().parent() == preclipRootIndex )
        {
            m_runningMode = PRECLIPPING_MODE;
            QModelIndex currentIndex;

            m_clippedImageModelIndex = QModelIndex();


            //if the selection is the Preclip(), then it will contain the id
            if(index.parent() == preclipRootIndex)
                id = model->index(index.row(),         1, index.parent()           ).data(Qt::DisplayRole).toInt();
            else
                id = model->index(index.parent().row(), 1, index.parent().parent() ).data(Qt::DisplayRole).toInt();

            switch( QMessageBox::question(
                    this,
                    tr("Delete Preclip"),
                    tr("This will delete the preclip PERMANENTLY, Are you sure?"),
                    QMessageBox::Yes |
                    QMessageBox::No,
                    QMessageBox::No) )
            {
            case QMessageBox::Yes:
                break;
            case QMessageBox::No:
                return;
            default:
                return;
            }

            m_publication.currentIssue()->preclip(id).deleteArticle(m_masterDatabase);
            m_publication.currentIssue()->deleteArticleFromView(ui->clipsTreeView, id);
            m_publication.addImageCutoutsToIssues(ui->issuesTreeView, false);

            //m_publication.currentIssue()->loadArticles(m_slaveDatabase, m_filterMode);
            //m_publication.currentIssue()->showArticles(ui->clipsTreeView);
        }

    }
}


void ClippingStation::onActionAutoArrange()
{
    QList<ImageCutout*> cutouts = ui->fullpageView->imageCutoutsList();
    if(cutouts.count() <= 0)
        return;

    orderList(cutouts);

    DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->currentWidget() );
    QSizeF cellSize = A4CellSize(true);
    bool isfirstPage = true;

    bool resetArray = true;
    for(int i=0; i< cutouts.count(); i++)
    {
        QString currentImage;

        if(m_runningMode == NORMAL_MODE)
            currentImage = m_localImagesList[m_imageIndex];
        else if(m_runningMode == PRECLIPPING_MODE)
            currentImage = m_preclippedLocalImagesList[m_preclippedImageIndex];
        else if(m_runningMode == CLIPPING_MODE)
            currentImage = m_clippedLocalImagesList[m_clippedImageIndex];

        if(cutouts[i]->pageName() != currentImage)
        {
            if(m_runningMode == NORMAL_MODE)
            {
                int index = m_localImagesList.indexOf(cutouts[i]->pageName());
                if(index < 0)
                    return;
                m_imageIndex = index;
            }
            else if(m_runningMode == PRECLIPPING_MODE)
            {
                int index = m_preclippedLocalImagesList.indexOf(cutouts[i]->pageName());
                if(index < 0)
                    return;
                m_preclippedImageIndex = index;
            }
            else if(m_runningMode == CLIPPING_MODE)
            {
                int index = m_clippedLocalImagesList.indexOf(cutouts[i]->pageName());
                if(index < 0)
                    return;
                m_clippedImageIndex = index;
            }

            loadImage();
        }

        QImage img  = cutouts[i]->createCutout();
        int type    = cutouts[i]->areaType();
        QPointF pos;
        qreal scale = 1.0;
        if(calculatePosAndScale(cellSize, img, type, resetArray, pos, scale))
        {
            addMargintoPos(pos, isfirstPage);
            view->addCutout(img, pos, scale);
        }
        else
        {
            isfirstPage = false;
            drawerAddPage(isfirstPage);
            drawerNextPage();
            view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->currentWidget() );
            cellSize = A4CellSize(isfirstPage);

            if(calculatePosAndScale(cellSize, img, type, true, pos, scale))
            {
                addMargintoPos(pos, isfirstPage);
                view->addCutout(img, pos, scale);
            }
            else
            {
                //error, the cutout is larger than the avaialble space in the A4 page
                ui->stackedDrawerViewWidget->removeWidget( ui->stackedDrawerViewWidget->currentWidget() );
                updateDrawerPageInformation();
                QMessageBox::information(this, tr("Large Cutout"), tr("Cutout number %1 does not fit in the available space").arg(i+1));
                return;
            }
        }
        resetArray = false;
    }
    //iterate thru all the views, and center the images
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        ui->stackedDrawerViewWidget->setCurrentIndex(i);
        view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        bool isArabic = (m_publication.language() == "Arabic");
        view->centerCutouts(isArabic);
    }

    //return to the first page to create the thumbnail
    QString currentImage;

    if(m_runningMode == NORMAL_MODE)
        currentImage = m_localImagesList[m_imageIndex];
    else if(m_runningMode == PRECLIPPING_MODE)
        currentImage = m_preclippedLocalImagesList[m_preclippedImageIndex];
    else if(m_runningMode == CLIPPING_MODE)
        currentImage = m_clippedLocalImagesList[m_clippedImageIndex];

    if(cutouts[0]->pageName() != currentImage)
    {
        if(m_runningMode == NORMAL_MODE)
        {
            int index = m_localImagesList.indexOf(cutouts[0]->pageName());
            if(index < 0)
                return;
            m_imageIndex = index;
        }
        else if(m_runningMode == PRECLIPPING_MODE)
        {
            int index = m_preclippedLocalImagesList.indexOf(cutouts[0]->pageName());
            if(index < 0)
                return;
            m_preclippedImageIndex = index;
        }
        else if(m_runningMode == CLIPPING_MODE)
        {
            int index = m_clippedLocalImagesList.indexOf(cutouts[0]->pageName());
            if(index < 0)
                return;
            m_clippedImageIndex = index;
        }

        loadImage();
    }

    createThumbnail();
}


void ClippingStation::onActionMarkIssueAsDone()
{
    QModelIndexList list = ui->issuesTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];
    //QString date =

    if( !index.isValid() )
        return;

    switch( QMessageBox::question(
            this,
            tr("Mark Issue as Done"),
            tr("This will mark the current issue as Done\nAre you sure?"),
            QMessageBox::Yes |
            QMessageBox::No,
            QMessageBox::No) )
    {
    case QMessageBox::Yes:
        break;
    case QMessageBox::No:
        return;
    default:
        return;
    }


    QAbstractItemModel *model = ui->issuesTreeView->model();

    if(model->parent(index)                         == QModelIndex() ) index = index;
    else if(model->parent(index).parent()           == QModelIndex() ) index = index.parent();
    else if(model->parent(index).parent().parent()  == QModelIndex() ) index = index.parent().parent();
    else                                                               index = index.parent().parent().parent();

    QString date = model->data(index, Qt::DisplayRole).toString();

    m_publication.issue(date)->markIssueAsDone(m_masterDatabase, true, m_user.id());
    model->setData(index, QVariant(QIcon(":/icons/ok.png")) , Qt::DecorationRole);
}

void ClippingStation::onActionMarkIssueAsUndone()
{
    QModelIndexList list = ui->issuesTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];
    //QString date =

    if( !index.isValid() )
        return;

    switch( QMessageBox::question(
            this,
            tr("Mark Issue as Undone"),
            tr("This will mark the current issue as Undone\nAre you sure?"),
            QMessageBox::Yes |
            QMessageBox::No,
            QMessageBox::No) )
    {
    case QMessageBox::Yes:
        break;
    case QMessageBox::No:
        return;
    default:
        return;
    }


    QAbstractItemModel *model = ui->issuesTreeView->model();

    if(model->parent(index)                         == QModelIndex() ) index = index;
    else if(model->parent(index).parent()           == QModelIndex() ) index = index.parent();
    else if(model->parent(index).parent().parent()  == QModelIndex() ) index = index.parent().parent();
    else                                                               index = index.parent().parent().parent();

    QString date = model->data(index, Qt::DisplayRole).toString();

    m_publication.issue(date)->markIssueAsDone(m_masterDatabase, false, m_user.id());
    model->setData(index, QVariant(QIcon(":/icons/cancel.png")) , Qt::DecorationRole);
}


void ClippingStation::onActionSendPageToOCR()
{
    QAbstractItemModel *model = ui->issuesTreeView->model();

    QModelIndexList list = ui->issuesTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];

    QString pageName    = index                        .data(Qt::DisplayRole).toString();
    //convert pageName from Page # to ####.jpg
    pageName = QString("%1.jpg").arg(pageName.remove(QRegExp("[^\\d]")), 4, QChar('0'));

    QString sectionName = model->parent(index)         .data(Qt::DisplayRole).toString();
    QString issueDate   = model->parent(index).parent().data(Qt::DisplayRole).toString();

    //send the page to OCR to be processed again
    QSqlQuery query(m_slaveDatabase);
    query.prepare("SELECT id_publication_issue FROM publication_issue "
                  "WHERE id_publication = :id_publication AND id_issue = :id_issue AND issue_date = :issue_date");

    query.bindValue(":id_publication",  m_publication.id() );
    query.bindValue(":id_issue",        m_publication.issue(issueDate)->id() );
    query.bindValue(":issue_date",      issueDate);

    if(!query.exec())
    {
        //error happened
        return;
    }

    int id_publication_issue = 0;

    if(query.next())
    {
        id_publication_issue = query.value(0).toInt();
        //return id_publication_issue;
        //break;
    }


    query.prepare("SELECT id_issue_sections FROM issue_sections WHERE "
                  "id_publication_issue = :id_publication_issue AND id_section = :id_section");
    query.bindValue(":id_publication_issue", id_publication_issue);
    query.bindValue(":id_section", m_publication.issue(issueDate)->section(sectionName).id() );

    if ( !query.exec() )
        return;

    int id_issue_sections = 0;
    if( query.next() )
    {
        id_issue_sections = query.value(0).toInt();
        //break;
    }

    //we have the page name, and the id_issue_sections, we can now reset the pages to OCR


    //query.exec("LOCK TABLES section_pages WRITE, page_text WRITE, page_tag_coordinates WRITE;"); //query.exec("LOCK TABLES section_pages");

    //first remove the pre processed values
    query.prepare("SELECT section_pages FROM section_pages "
                  "WHERE "
                  " id_issue_sections = :id_issue_sections "
                  " AND page_name like :page_name");

    query.bindValue(":id_issue_sections" , id_issue_sections);
    query.bindValue(":page_name", pageName);

    if (query.exec() )
    {
        int id = 0;
        if(query.next())
            id = query.value(0).toInt();

        //delete the previous coordinates
        query.prepare("DELETE FROM page_tag_coordinates WHERE id_section_pages = :id");
        query.bindValue(":id", id);
        query.exec();

        query.prepare("DELETE FROM page_text WHERE id_section_pages = :id");
        query.bindValue(":id", id);
        query.exec();
    }

    query.prepare("UPDATE section_pages SET status = 1 "
                  "WHERE "
                  " section_pages.id_issue_sections = :id_issue_sections "
                  " AND section_pages.page_name like :page_name AND status != 1");

    query.bindValue(":id_issue_sections" , id_issue_sections);
    query.bindValue(":page_name", pageName);

    query.exec();

    //query.exec("UNLOCK TABLES");

    model->setData(index, QVariant(QIcon(":/icons/gray_circle.png")) , Qt::DecorationRole);

    //loadImage();      //no need to re-load the page because it may not by the page that the user is currently looking at
}

void ClippingStation::onActionSendIssueToOCR()
{
    switch( QMessageBox::question(
        this,
        tr("Send issue back to OCR"),
        tr("This operation will send the entire issue back to OCR\nAre you sure?"),
        QMessageBox::Yes |
        QMessageBox::No,
        QMessageBox::No) )
    {
        case QMessageBox::Yes:
            break;
        case QMessageBox::No:
            return;
        default:
            return;
    }


    QAbstractItemModel *model = ui->issuesTreeView->model();

    QModelIndexList list = ui->issuesTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex issueIndex = list[0];
    QString issueDate   = model->data(issueIndex, Qt::DisplayRole).toString();

    //send the entire issue to OCR to be processed again
    QSqlQuery query(m_slaveDatabase);

    query.exec("LOCK TABLES section_pages WRITE, issue_sections READ , publication_issue READ;");

    //first remove the pre processed values
    query.prepare("UPDATE section_pages, issue_sections, publication_issue "
                  " SET status = 1 "
                  " WHERE section_pages.id_issue_sections = issue_sections.id_issue_sections "
                  " AND issue_sections.id_publication_issue = publication_issue.id_publication_issue "
                  " AND id_issue = :id_issue AND publication_issue.id_publication = :id_publication;");

    query.bindValue(":id_issue" , m_publication.issue(issueDate)->id() );
    query.bindValue(":id_publication", m_publication.id() );

    if ( !query.exec() )
    {
        query.exec("UNLOCK TABLES");
        return;
    }

    query.exec("UNLOCK TABLES");

    //////////////////////////////////////////////////////////////
    int sectionsCount = model->rowCount(issueIndex);

    for(int j=0; j < sectionsCount; j++)
    {
        QString sectionName = model->index(j, 0, issueIndex).data(Qt::DisplayRole).toString();
        QModelIndex sectionIndex = model->index(j, 0, issueIndex);
        int pagesCount = model->rowCount(sectionIndex);

        for(int k=0; k< pagesCount; k++)
        {
            QModelIndex pageIndex = model->index(k, 0, sectionIndex);
            model->setData(pageIndex, QVariant(QIcon(":/icons/gray_circle.png")) , Qt::DecorationRole);
        }
    }
}

void ClippingStation::onActionViewClipText()
{
    QAbstractItemModel *model = ui->clipsTreeView->model();

    QModelIndexList list = ui->clipsTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];

    QModelIndex clipRootIndex    = model->index(1, 0, QModelIndex() );
    //if the user clicked on top level entries, just return

    int id;
    if ( index.parent() == clipRootIndex ||
         index.parent().parent() == clipRootIndex )
        {
            //m_clippedImageModelIndex = QModelIndex();
            if(index.parent() == clipRootIndex)
                id = model->index(index.row(),         1, index.parent()           ).data(Qt::DisplayRole).toInt();
            else
                id = model->index(index.parent().row(), 1, index.parent().parent() ).data(Qt::DisplayRole).toInt();

            //get the text_ocr.
            QSqlQuery query(m_masterDatabase);
            query.prepare("SELECT `text_ocr` FROM article WHERE id_article = :id");
            query.bindValue(":id", id);
            if ( !query.exec() )
                return;

            QString text;

            while(query.next())
                text = query.value(0).toString();

            PageTextDialog dlg(this, text);
            dlg.exec();
        }
}

void ClippingStation::onActionViewPageText()
{
    QAbstractItemModel *model = ui->issuesTreeView->model();

    QModelIndexList list = ui->issuesTreeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];

    QString pageName    = index                        .data(Qt::DisplayRole).toString();
    pageName = QString("%1.jpg").arg(pageName.remove(QRegExp("[^\\d]")), 4, QChar('0'));

    QString sectionName = model->parent(index)         .data(Qt::DisplayRole).toString();
    QString issueDate   = model->parent(index).parent().data(Qt::DisplayRole).toString();

    QSqlQuery query(m_slaveDatabase);
    query.prepare("SELECT id_publication_issue FROM publication_issue "
                  "WHERE id_publication = :id_publication AND id_issue = :id_issue AND issue_date = :issue_date");

    query.bindValue(":id_publication",  m_publication.id() );
    query.bindValue(":id_issue",        m_publication.issue(issueDate)->id() );
    query.bindValue(":issue_date",      issueDate);

    if(!query.exec())
    {
        //error happened
        return;
    }

    int id_publication_issue = 0;

    if(query.next())
        id_publication_issue = query.value(0).toInt();


    query.prepare("SELECT id_issue_sections FROM issue_sections WHERE "
                  "id_publication_issue = :id_publication_issue AND id_section = :id_section");
    query.bindValue(":id_publication_issue", id_publication_issue);
    query.bindValue(":id_section", m_publication.issue(issueDate)->section(sectionName).id() );

    if ( !query.exec() )
        return;

    int id_issue_sections = 0;
    if( query.next() )
        id_issue_sections = query.value(0).toInt();

    query.exec("LOCK TABLES section_pages READ");

    query.prepare("SELECT section_pages FROM section_pages "
                  "WHERE "
                  " section_pages.id_issue_sections = :id_issue_sections "
                  " AND section_pages.page_name like :page_name");

    query.bindValue(":id_issue_sections" , id_issue_sections);
    query.bindValue(":page_name", pageName);

    if ( !query.exec() )
    {
        query.exec("UNLOCK TABLES");
        return;
    }

    int id = 0;
    while(query.next())
        id = query.value(0).toInt();

    query.exec("UNLOCK TABLES");


    query.prepare("SELECT `text` FROM page_text WHERE id_section_pages = :id");
    query.bindValue(":id", id);
    if ( !query.exec() )
        return;

    QString text;

    while(query.next())
        text = query.value(0).toString();

    PageTextDialog dlg(this, text);
    dlg.exec();

}


void ClippingStation::onActionViewKeywordsSearchword()
{
//    QAbstractItemModel *model = ui->treeView->model();

    QModelIndexList list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];

    QString pageName    = index                        .data(Qt::DisplayRole).toString();


    //get searchwords for this keywords
    QString lstring = Tag::loadSearchwords(m_slaveDatabase, pageName);
    QMessageBox::information(0, "Searchwords", QString("Searchwords are:\n%1").arg(lstring));

}

void ClippingStation::onActionGoToKeywordWikiPage()
{
//    QAbstractItemModel *model = ui->treeView->model();

    QModelIndexList list = ui->treeView->selectionModel()->selectedIndexes();
    if (list.count() == 0)
        return;

    QModelIndex index = list[0];

    QString keyword = index.data(Qt::DisplayRole).toString();

    keyword.replace(QRegExp("\\s"), "_");
    keyword.replace(' ', '_');
//    keyword.replace("'", "_");
//    keyword.replace("-", "_");

    QUrl url(QString("%1%2").arg(SystemConfiguration::wikiUrl()).arg(keyword));
//  QMessageBox::information(this, "test", QString("URL is %1").arg(url.toString()));

    QDesktopServices::openUrl (url);
}

void ClippingStation::onActionAbout()
{
    QMessageBox::about(this, tr("About Clipping Station"),
                       "<html><body><p style=""color: #696969""><b>Clipping Station</b> - $Revision: 668 $</p>"
                       "<p style=""color: #696969"">(c) Copyright 2009-2011 ALLCONTENT. All rights reserved.</p></body></html>");
}


bool ClippingStation::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->treeWidget)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Enter)
            {
                selectKeyword();
                return true;
            }
        }
    }

    return QMainWindow::eventFilter(obj, event);
}



void ClippingStation::loadIssues()
{
    QString name = ui->publicationNameLineEdit_2->text();
    int id = m_publicationHash[name];

    m_publication.setId(id);
    m_publication.setName(name);

    m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);
    //m_publication.loadIssues(ui->issuesTreeView, m_cacheDir);
    m_publication.loadIssuesOnly(ui->issuesTreeView);
    //m_publication.addImageCutoutsToIssues(ui->issuesTreeView, true);

    m_currentIssueDate = "";
    m_currentSectionName="";
}

void ClippingStation::loadIssuesWithPreclipsOnly()
{
    clock_t start = clock();

    QString name = ui->preclippedPublicationComboBox->currentText();

    if ( name.isEmpty() )
        return;

    int id = m_publicationHash[name];

    m_publication.setId(id);
    m_publication.setName(name);

    m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);
    m_publication.loadIssuesWithPreclipsOnly(ui->issuesTreeView, m_cacheDir);

    m_currentIssueDate = "";
    m_currentSectionName="";

    QAbstractItemModel *model = ui->issuesTreeView->model();

    QModelIndex issuesIndex = model->index(0, 0, QModelIndex() );
    if( issuesIndex.isValid() )
    {
        issueTreeViewItemClicked(issuesIndex);
        //load the first pre-clip
        QAbstractItemModel *model = ui->clipsTreeView->model();
        QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );
        QModelIndex pindex = preclipRootIndex.child(0, 0);
        if(pindex.isValid())
            articleTreeViewClicked(pindex);
    }


    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
    qDebug("loadIssuesWithPreclipsOnly() Time: %f", diff);

}

void ClippingStation::loadFilteredIssues()
{
    clock_t start = clock();

    QString name = ui->filteredPublicationComboBox->currentText();

    if ( name.isEmpty() )
        return;

    int id = m_publicationHash[name];

    m_publication.setId(id);
    m_publication.setName(name);

    m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);

    QString keyword, startDate, endDate, type, country, ocrSearchword, status;
    bool ischecked;
    m_searchPublicationsDlg.get(keyword, ischecked,  startDate, endDate, type, country, ocrSearchword, status);

    //now search for the publications that meet those criteria
    int countryId = -1;
    if(!country.isEmpty())
        countryId = m_countriesHash[country];

    int typeId = -1;
    if(!type.isEmpty())
    {
        if(type.compare("newspaper", Qt::CaseInsensitive) == 0) //NEWSPAPER 1
            typeId = 1;
        else if(type.compare("magazine", Qt::CaseInsensitive) == 0)
            typeId = 2;
    }

    m_publication.loadFilteredIssues(ui->issuesTreeView, m_ocrDatabase, m_cacheDir, startDate, endDate, keyword, ocrSearchword, status);

    m_currentIssueDate = "";
    m_currentSectionName="";

    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
    qDebug("loadIssuesWithPreclipsOnly() Time: %f", diff);
}

void ClippingStation::loadUndoneIssuesOnly()
{
    clock_t start = clock();

    QString name = ui->undonePublicationComboBox->currentText();

    if ( name.isEmpty() )
        return;

    int id = m_publicationHash[name];

    m_publication.setId(id);
    m_publication.setName(name);

    m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);
    m_publication.loadUndoneIssuesOnly(ui->issuesTreeView);

    m_currentIssueDate = "";
    m_currentSectionName="";
/*
    QAbstractItemModel *model = ui->issuesTreeView->model();

    QModelIndex issuesIndex = model->index(0, 0, QModelIndex() );
    if( issuesIndex.isValid() )
    {
        issueTreeViewItemClicked(issuesIndex);
        //load the first pre-clip
        QAbstractItemModel *model = ui->clipsTreeView->model();
        QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );
        QModelIndex pindex = preclipRootIndex.child(0, 0);
        if(pindex.isValid())
            articleTreeViewClicked(pindex);
    }

*/
    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
    qDebug("loadUndoneIssuesOnly() Time: %f", diff);
}

void ClippingStation::issueTreeViewItemClicked(QModelIndex index)
{
    qDebug("issueTreeViewItemClicked()");
    if( index.isValid() )
    {
        QAbstractItemModel *model = ui->issuesTreeView->model();

        if(model->parent(index) == QModelIndex() )
        {
            //qDebug("We are at Issue Level");
            //we may have not loaded the sections, so we do that, and then do our work
            //
            QString issueDate   = model->data(index           , Qt::DisplayRole).toString();
            if( !m_publication.issue(issueDate)->isLoaded() )
            {
                //load the issue, and
                QStandardItemModel *standardModel   = static_cast<QStandardItemModel*> ( ui->issuesTreeView->model() );
                QStandardItem* issueItem            = standardModel->itemFromIndex(index);
                m_publication.loadIssueSections(issueItem, issueDate, m_cacheDir);
            }

            QString sectionName = model->data(index.child(0,0), Qt::DisplayRole).toString();

            if( m_currentIssueDate != issueDate)
                ui->fullpageView->removeImageCutouts();

            m_currentIssueDate = issueDate;
            m_publication.setCurrentIssue(issueDate);
            m_publication.currentIssue()->showArticlesInView(m_masterDatabase, ui->clipsTreeView, m_filterMode);
            m_publication.currentIssue()->section(sectionName).getLocalAndRemoteFiles(m_localImagesList, m_remoteImagesList);

            m_publication.addImageCutoutsToIssues(ui->issuesTreeView, false);

            ui->issuesTreeView->expand(index);
            updateSectionPagesStatus();
            return;
        }

        if(model->parent(index).parent() == QModelIndex() )
        {
            //qDebug("We are at Section Level");
            QString issueDate   = model->data(index.parent(), Qt::DisplayRole).toString();
            QString sectionName = model->data(index         , Qt::DisplayRole).toString();

            if( m_currentIssueDate != issueDate)
                ui->fullpageView->removeImageCutouts();

            m_currentIssueDate = issueDate;
            m_publication.setCurrentIssue(issueDate);
            m_publication.currentIssue()->showArticlesInView(m_masterDatabase, ui->clipsTreeView, m_filterMode);
            m_publication.currentIssue()->section(sectionName).getLocalAndRemoteFiles(m_localImagesList, m_remoteImagesList);

            m_publication.addImageCutoutsToIssues(ui->issuesTreeView, false);

            ui->issuesTreeView->expand(index);
            updateSectionPagesStatus();
            return;
        }

        bool showArticleSelections;
        int articleId = 0;

        if(model->parent(index).parent().parent() == QModelIndex() )            //its the image
            showArticleSelections = false;
        else                //its a selection in the image
        {
            showArticleSelections   = true;
            articleId               = index.data(Qt::DisplayRole).toInt();
            index                   = index.parent();
            //gotoArticlePage(articleId);
        }


        QString sectionName = model->parent(index)         .data(Qt::DisplayRole).toString();
        QString issueDate   = model->parent(index).parent().data(Qt::DisplayRole).toString();

        if( m_currentIssueDate != issueDate)
        {
            ui->fullpageView->removeImageCutouts();
            updateSectionPagesStatus();
        }
        else if(m_currentSectionName != sectionName)
            ui->fullpageView->hideImageCutouts();


        m_currentIssueDate   = issueDate;
        m_currentSectionName = sectionName;

        m_publication.setCurrentIssue(issueDate);
        m_publication.currentIssue()->showArticlesInView(m_masterDatabase, ui->clipsTreeView, m_filterMode);
        m_publication.currentIssue()->section(sectionName).getLocalAndRemoteFiles(m_localImagesList, m_remoteImagesList);

        m_publication.addImageCutoutsToIssues(ui->issuesTreeView, false);

        if(m_runningMode != NORMAL_MODE && ui->preclipButton->text() == tr("Preclip") )
        {
            ui->fullpageView->removeImageCutouts();

            m_tagsWrapper.clearTags();
            m_tagsWrapper.showModel();

            if(m_runningMode == CLIPPING_MODE)
                drawerResetPages();
        }

        if ( ui->fullpageView->preselectedImageCutoutsCount() > 0 )
        {
            ui->fullpageView->removePreselectedImageCutouts();
            m_tagsWrapper.clearTags();
            m_tagsWrapper.showModel();
        }

        if(showArticleSelections)
        {
            ui->fullpageView->setPreselectedImageCutouts( m_publication.currentIssue()->getImageCutouts(articleId) );

            m_tagsWrapper.setOcrTags(m_publication.currentIssue()->articleTags(articleId));
            m_tagsWrapper.showModel();
        }

        m_imageModelIndex = index;
        m_imageIndex = index.row();

        m_clippedImageModelIndex = m_preclippedImageModelIndex = QModelIndex();

        loadImage();
        enableDrawerButtons(true);
        setModifyClipButton(false);

        m_runningMode = NORMAL_MODE;

        ui->gotoPageLineEdit->setText(QString("%1").arg(m_imageIndex+1) );

    }
}

void ClippingStation::articleTreeViewClicked(QModelIndex index)
{
    qDebug("articleTreeViewClicked()");
    static int clipid, preclipid;

    if( !index.isValid() )
        return;

    QAbstractItemModel *model = ui->clipsTreeView->model();

    QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );
    QModelIndex clipRootIndex    = model->index(1, 0, QModelIndex() );
    //if the user clicked on top level entries, just return
    if ( index ==  preclipRootIndex  ||
         index ==  clipRootIndex         )
    {
        ui->clipsTreeView->expand(index);
        return;
    }

    int id;
    if ( index.parent()          == preclipRootIndex ||
         index.parent().parent() == preclipRootIndex )
    {
        QModelIndex currentIndex;

        m_clippedImageModelIndex = QModelIndex();


        //if the selection is the Preclip(), then it will contain the id
        if(index.parent() == preclipRootIndex)
        {
            id = model->index(index.row(),         1, index.parent()           ).data(Qt::DisplayRole).toInt();
            currentIndex = index.child(0, 0);
        }
        else
        {
            id = model->index(index.parent().row(), 1, index.parent().parent() ).data(Qt::DisplayRole).toInt();
            currentIndex = index;
        }


        if (currentIndex == m_preclippedImageModelIndex )
            return;     //if its the same selection, do nothing


        if ( preclipid != id || m_currentPreclipId == 0 )
        {
            if(m_runningMode == CLIPPING_MODE)
                drawerResetPages();

            clipid              = 0;
            m_currentClipId     = 0;
            preclipid           = id;
            m_currentPreclipId  = id;
            m_firstPageInClip   = "";
        }

        if (ui->preclipButton->text() != tr("Preclip"))
        {
            QList<Tag> tags = m_tagsWrapper.ocrTags();
            m_tagsWrapper.setOcrTags( tags );

            QList<ImageCutout*> cutoutsList = ui->fullpageView->imageCutoutsList();
            m_publication.currentIssue()->preclip(id).setImageCutouts(cutoutsList);
        }
        else
            m_tagsWrapper.setOcrTags( m_publication.currentIssue()->preclip(id).tags() );


        ui->fullpageView->hideImageCutouts();
        ui->fullpageView->removePreselectedImageCutouts();
        ui->fullpageView->setImageCutouts(m_publication.currentIssue()->preclip(id).imageCutoutsList());


        m_tagsWrapper.showModel();


        //m_cachedImages.clear();
        m_publication.currentIssue()->preclip(id).getImagesLocalAndRemoateFiles(m_preclippedLocalImagesList, m_preclippedRemoteImagesList);


        m_preclippedImageIndex = currentIndex.row();
        m_preclippedImageModelIndex = currentIndex;

        QString section = model->data(currentIndex, Qt::DisplayRole).toString();
        section = section.left(section.indexOf("/"));
        m_currentSectionName = section;

        QString fname       = m_preclippedRemoteImagesList[0];       //its int the format of /ClippingStation/cach/2009/10/17/162/16/0001.jpg
        QStringList list    = fname.split("/");
        m_currentIssueDate  = list[list.count()-6] + "-" + list[list.count()-5]+ "-" + list[list.count()-4];
        m_publication.setCurrentIssue(m_currentIssueDate);
        m_publication.currentIssue()->section(m_currentSectionName).getLocalAndRemoteFiles(m_localImagesList, m_remoteImagesList);


        ui->clipsTreeView->selectionModel()->clearSelection();
        ui->clipsTreeView->selectionModel()->setCurrentIndex(m_preclippedImageModelIndex, QItemSelectionModel::Select);

        //ui->clipsTreeView->expand(m_preclippedImageModelIndex.parent().parent() );
        //ui->clipsTreeView->expand(m_preclippedImageModelIndex.parent() );
        ui->clipsTreeView->expand(m_preclippedImageModelIndex );


        enableDrawerButtons(true);
        setModifyClipButton(false);

        m_runningMode = PRECLIPPING_MODE;

    }
    else if ( index.parent()          == clipRootIndex ||
              index.parent().parent() == clipRootIndex )
    {
        m_runningMode = CLIPPING_MODE;
        QModelIndex currentIndex;
        m_preclippedImageModelIndex = QModelIndex();

        //qDebug("Columns count is %d", model->columnCount(index.parent()) );

        if(index.parent() == clipRootIndex)
        {
            id = model->index(index.row(),         1, index.parent()           ).data(Qt::DisplayRole).toInt();
            currentIndex = index.child(0, 0);
        }
        else
        {
            id = model->index(index.parent().row(), 1, index.parent().parent() ).data(Qt::DisplayRole).toInt();
            currentIndex = index;
        }

        if (currentIndex == m_clippedImageModelIndex )
            return;     //if its the same selection, do nothing


        if(clipid != id || m_currentClipId == 0)
        {
            drawerResetPages();
            clipid              = id;
            m_currentClipId     = id;
            preclipid           = 0;
            m_currentPreclipId  = 0;
            m_firstPageInClip   = "";
        }
        ui->fullpageView->hideImageCutouts();
        ui->fullpageView->removePreselectedImageCutouts();
        ui->fullpageView->setImageCutouts(m_publication.currentIssue()->clip(id).imageCutoutsList());


        //m_cachedImages.clear();
        m_publication.currentIssue()->clip(id).getImagesLocalAndRemoateFiles(m_clippedLocalImagesList, m_clippedRemoteImagesList);

        m_tagsWrapper.setOcrTags( m_publication.currentIssue()->clip(id).tags() );
        m_tagsWrapper.showModel();

        m_clippedImageIndex = currentIndex.row();
        m_clippedImageModelIndex = currentIndex;

        //ui->clipsTreeView->expand(m_clippedImageModelIndex.parent().parent() );
        //ui->clipsTreeView->expand(m_clippedImageModelIndex.parent() );
        ui->clipsTreeView->expand(m_clippedImageModelIndex );

        ui->clipsTreeView->selectionModel()->clearSelection();
        ui->clipsTreeView->selectionModel()->setCurrentIndex(m_clippedImageModelIndex, QItemSelectionModel::Select);

        QString section = model->data(currentIndex, Qt::DisplayRole).toString();
        section = section.left(section.indexOf("/"));
        m_currentSectionName = section;

        QString fname       = m_clippedRemoteImagesList[0];       //its int the format of G:/socialhose/app/ClippingStation/debug/cach/2009/10/17/162/16/0001.jpg
        QStringList list    = fname.split("/");
        m_currentIssueDate  = list[list.count()-6] + "-" + list[list.count()-5]+ "-" + list[list.count()-4];
        m_publication.setCurrentIssue(m_currentIssueDate);
        m_publication.currentIssue()->section(m_currentSectionName).getLocalAndRemoteFiles(m_localImagesList, m_remoteImagesList);

        loadClippingImages(id);
    }

    loadImage();
    gotoClipPage();
}

void ClippingStation::clearStatusMessage()
{
    ui->statusBar->clearMessage();
}

void ClippingStation::displayStatusMessage(const QString & status)
{
    ui->statusBar->showMessage(status);
}

void ClippingStation::displayCoordinatesStatus(qreal x, qreal y, qreal width, qreal height)
{
    ui->statusBar->displayCoordinates(x, y, width, height);
}

void ClippingStation::displayRotationAngleStatus(qreal angle)
{
    ui->statusBar->displayRotationAngle(angle);
}

void ClippingStation::updateDrawerPageInformation()
{
    ui->pageInformation->setText(QString("Page %1 of %2")
                                 .arg(ui->stackedDrawerViewWidget->currentIndex() +1)
                                 .arg(ui->stackedDrawerViewWidget->count()));
}

void ClippingStation::displayZoomStatus(qreal zoom)
{
    ui->statusBar->displayZoom(zoom);
}

void ClippingStation::firstPage()
{
    if ( m_imageModelIndex.isValid() )
    {
        int oldindex = m_imageIndex;
        m_imageIndex = 0;

        if (!loadImage())
        {
            m_imageIndex = oldindex;
            return;
        }

        m_imageModelIndex = ui->issuesTreeView->model()->index(0, 0, ui->issuesTreeView->model()->parent(m_imageModelIndex) );
        ui->issuesTreeView->selectionModel()->clearSelection();
        ui->issuesTreeView->selectionModel()->setCurrentIndex(m_imageModelIndex, QItemSelectionModel::Select);

        ui->gotoPageLineEdit->setText(QString("%1").arg(m_imageIndex+1) );

    }
}

void ClippingStation::previousPage()
{
    if ( m_imageIndex > 0 && m_imageModelIndex.isValid() )
    {
        --m_imageIndex;

        if (!loadImage())
        {
            ++m_imageIndex;
            return;
        }

        m_imageModelIndex = ui->issuesTreeView->model()->index(m_imageModelIndex.row()-1, 0, ui->issuesTreeView->model()->parent(m_imageModelIndex) );
        ui->issuesTreeView->selectionModel()->clearSelection();
        ui->issuesTreeView->selectionModel()->setCurrentIndex(m_imageModelIndex, QItemSelectionModel::Select);

        ui->gotoPageLineEdit->setText(QString("%1").arg(m_imageIndex+1) );
    }
}

void ClippingStation::nextPage()
{
    if( m_imageIndex < m_localImagesList.count()-1 )
    {
        m_imageIndex++;

        if (!loadImage())
        {
            --m_imageIndex;
            return;
        }

        m_imageModelIndex = ui->issuesTreeView->model()->index(m_imageModelIndex.row()+1, 0, ui->issuesTreeView->model()->parent(m_imageModelIndex) );
        ui->issuesTreeView->selectionModel()->clearSelection();
        ui->issuesTreeView->selectionModel()->setCurrentIndex(m_imageModelIndex, QItemSelectionModel::Select);

        ui->gotoPageLineEdit->setText(QString("%1").arg(m_imageIndex+1) );
    }
}

void ClippingStation::lastPage()
{
    if ( m_imageModelIndex.isValid() )
    {
        int oldindex = m_imageIndex;
        m_imageIndex = m_localImagesList.count() -1;
        if (!loadImage())
        {
            m_imageIndex = oldindex;
            return;
        }

        m_imageModelIndex = ui->issuesTreeView->model()->index(m_imageIndex, 0,ui->issuesTreeView->model()->parent(m_imageModelIndex) );
        ui->issuesTreeView->selectionModel()->clearSelection();
        ui->issuesTreeView->selectionModel()->setCurrentIndex(m_imageModelIndex, QItemSelectionModel::Select);

        ui->gotoPageLineEdit->setText(QString("%1").arg(m_imageIndex+1) );
    }
}

void ClippingStation::gotoPage()
{
    bool ok;
    int pagenum = ui->gotoPageLineEdit->text().toInt(&ok);

    if(!ok)
        return;

    if( m_imageModelIndex.isValid() )
    {
        if ( pagenum < 1 || pagenum > m_localImagesList.count() )
            return;

        int oldindex = m_imageIndex;
        m_imageIndex = pagenum -1 ;

        if(!loadImage())
        {
            m_imageIndex = oldindex;
            return;
        }

        m_imageModelIndex = ui->issuesTreeView->model()->index(m_imageIndex, 0, ui->issuesTreeView->model()->parent(m_imageModelIndex) );
        ui->issuesTreeView->selectionModel()->clearSelection();
        ui->issuesTreeView->selectionModel()->setCurrentIndex(m_imageModelIndex, QItemSelectionModel::Select);
    }
}

void ClippingStation::gotoClipPage()
{
    //qDebug("gotoClipPage()");

    QAbstractItemModel* model = ui->issuesTreeView->model();
    int issuesCount = model->rowCount( QModelIndex() );

    QString article;
    if(m_runningMode == PRECLIPPING_MODE)
        article = ui->clipsTreeView->model()->data(m_preclippedImageModelIndex, Qt::DisplayRole).toString();
    else if(m_runningMode == CLIPPING_MODE)
        article = ui->clipsTreeView->model()->data(m_clippedImageModelIndex   , Qt::DisplayRole).toString();

    for(int i=0; i< issuesCount; i++)
    {
        QString issue = model->index(i, 0, QModelIndex() ).data(Qt::DisplayRole).toString();
        if(issue != m_currentIssueDate)
            continue;

        QModelIndex issueIndex = model->index(i, 0, QModelIndex() );

        int sectionsCount = model->rowCount(issueIndex);
        for(int j=0; j < sectionsCount; j++)
        {
            QString section = model->index(j, 0, issueIndex).data(Qt::DisplayRole).toString();
            if( article.split("/").at(0) != section )// != m_currentSectionName)
                continue;

            QModelIndex sectionIndex = model->index(j, 0, issueIndex);
            int pagesCount = model->rowCount(sectionIndex);

            for(int k=0; k< pagesCount; k++)
            {
                QString page = model->index(k, 0, sectionIndex).data(Qt::DisplayRole).toString();
                if( !article.endsWith(page) )
                    continue;

                m_imageModelIndex = model->index(k, 0, sectionIndex);
                ui->issuesTreeView->selectionModel()->clearSelection();
                ui->issuesTreeView->selectionModel()->setCurrentIndex(m_imageModelIndex, QItemSelectionModel::Select);

                ui->issuesTreeView->expand(m_imageModelIndex.parent().parent());
                ui->issuesTreeView->expand(m_imageModelIndex.parent());
                //ui->issuesTreeView->expand(m_imageModelIndex);
            }
            return;
        }
        return;
    }
}

void ClippingStation::rotate(double i)
{
    ui->fullpageView->rotate(i);
    if(i == 0)
        ui->saveRotateButton->setEnabled(false);
    else
        ui->saveRotateButton->setEnabled(true);
}

void ClippingStation::saveImageRotation()
{
    //we first get the ORIGINAL image from the file server
    //we get its name/path
    QString localeImage, remoteImage;
    if(m_runningMode == NORMAL_MODE)
    {
        localeImage = m_localImagesList[m_imageIndex];
        remoteImage = m_remoteImagesList[m_imageIndex];
    }
    else if (m_runningMode == PRECLIPPING_MODE)
    {
        localeImage = m_preclippedLocalImagesList[m_preclippedImageIndex];
        remoteImage = m_preclippedRemoteImagesList[m_preclippedImageIndex];
    }
    else if (m_runningMode == CLIPPING_MODE )
    {
        localeImage = m_clippedLocalImagesList[m_clippedImageIndex];
        remoteImage = m_clippedRemoteImagesList[m_clippedImageIndex];
    }

    QImage image(remoteImage);

    //do the roation
    double rotate = ui->rotateSpinBox->value();

    QMatrix mx;
    mx = mx.rotate(rotate);
    image = image.transformed(mx, Qt::SmoothTransformation);

    //now we delete the original image, and save this back to that location
    if (QFile::remove(remoteImage) )
    {
        image.save(remoteImage, "jpg", JPG_SAVE_QUALITY);
        //also remove the file from the local cache
        QFile::remove(localeImage);
        //reset the rotate spin box
        ui->rotateSpinBox->setValue(0);
        //and re open the new image after it has been saved
        m_cachedImages[ localeImage ] = false;      //to force to reload the new image from server
        loadImage();
    }

    //also, load the 300dpi image, and rotate it as well
    QFileInfo inf(remoteImage);
    QString img300dpi= inf.absolutePath() + "/" + HIGH_RES_IMAGE_DIR + inf.fileName();

    if(!QFile::exists(img300dpi))
        return;

    QImage image300(img300dpi);
    image300 = image300.transformed(mx, Qt::SmoothTransformation);
    //now we delete the original image, and save this back to that location
    if (QFile::remove(img300dpi) )
        image300.save(img300dpi, "jpg", JPG_SAVE_QUALITY);
}


qreal ClippingStation::zoomOut()
{
    ui->bestFitButton->setChecked(false);
    ui->widthFitButton->setChecked(false);
    ui->fullpageView->setPageLayout(ACTUAL_SIZE);

    return ui->fullpageView->zoom(false);
}

qreal ClippingStation::zoomIn()
{
    ui->bestFitButton->setChecked(false);
    ui->widthFitButton->setChecked(false);
    ui->fullpageView->setPageLayout(ACTUAL_SIZE);

    return ui->fullpageView->zoom(true);
}

qreal ClippingStation::actualSize()
{
    ui->bestFitButton->setChecked(false);
    ui->widthFitButton->setChecked(false);
    ui->fullpageView->setPageLayout(ACTUAL_SIZE);

    return ui->fullpageView->actualSize();
}

void ClippingStation::bestFit()
{
    ui->bestFitButton->setChecked(true);
    ui->widthFitButton->setChecked(false);
    ui->fullpageView->setPageLayout(BEST_FIT);
}

void ClippingStation::widthFit()
{
    ui->bestFitButton->setChecked(false);
    ui->widthFitButton->setChecked(true);
    ui->fullpageView->setPageLayout(WIDTH_FIT);
}

qreal ClippingStation::drawerZoomOut()
{
    ui->drawerBestFitButton->setChecked(false);
    ui->drawerWidthFitButton->setChecked(false);


    int index = ui->stackedDrawerViewWidget->currentIndex();

    //ui->stackedDrawerViewWidget->setCurrentIndex(0);
    qreal scale = 1.0;
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        ui->stackedDrawerViewWidget->setCurrentIndex(i);
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        scale = view->zoom(false);
        m_currentA4Scale = view->currentScale();
    }

    ui->stackedDrawerViewWidget->setCurrentIndex(index);



    return scale;
}

qreal ClippingStation::drawerZoomIn()
{
    ui->drawerBestFitButton->setChecked(false);
    ui->drawerWidthFitButton->setChecked(false);


    int index = ui->stackedDrawerViewWidget->currentIndex();

    //ui->stackedDrawerViewWidget->setCurrentIndex(0);
    qreal scale = 1.0;
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        ui->stackedDrawerViewWidget->setCurrentIndex(i);
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        scale = view->zoom(true);
        m_currentA4Scale = view->currentScale();
    }

    ui->stackedDrawerViewWidget->setCurrentIndex(index);

    return scale;
}

qreal ClippingStation::drawerActualSize()
{
    ui->drawerBestFitButton->setChecked(false);
    ui->drawerWidthFitButton->setChecked(false);

    int index = ui->stackedDrawerViewWidget->currentIndex();

    ui->stackedDrawerViewWidget->setCurrentIndex(0);
    qreal scale = 1.0;
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        view->setPageLayout(ACTUAL_SIZE);
        scale = view->actualSize();
        m_currentA4Scale = view->currentScale();
    }

    ui->stackedDrawerViewWidget->setCurrentIndex(index);


    return scale;
}

void ClippingStation::drawerBestFit()
{
    ui->drawerBestFitButton->setChecked(true);
    ui->drawerWidthFitButton->setChecked(false);

    int index = ui->stackedDrawerViewWidget->currentIndex();

    ui->stackedDrawerViewWidget->setCurrentIndex(0);
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        view->setPageLayout(BEST_FIT);
        m_currentA4Scale = view->currentScale();
    }


    ui->stackedDrawerViewWidget->setCurrentIndex(index);
}

void ClippingStation::drawerWidthFit()
{
    ui->drawerBestFitButton ->setChecked(false);
    ui->drawerWidthFitButton->setChecked(true);

    int index = ui->stackedDrawerViewWidget->currentIndex();

    ui->stackedDrawerViewWidget->setCurrentIndex(0);
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        view->setPageLayout(WIDTH_FIT);
        m_currentA4Scale = view->currentScale();
    }


    ui->stackedDrawerViewWidget->setCurrentIndex(index);
}

bool ClippingStation::loadImage()
{
//    qDebug("loadImage()");
//    clock_t start = clock();
    static bool firstPage = true;

    m_trackingTimer.stop();
/*
------------------------
| id_user_page_track   |
| id_users             |
| id_section_pages     |
| last_modified        |
| first_access         |
| last_access          |
| issue_created        |
| id_publication       |
| id_publication_type  |
| publication_language |
| publication_market   |
| issue_date           |
| seconds              |
------------------------
*/

    if(!firstPage && m_trackingSeconds > 0)
    {
        QString language = "en";
        if(m_publication.language() == "Arabic")
            language = "ar";
        m_user.updateTrack(m_currentPageId, m_trackingSeconds);
    }

    bool ret = true;
    QString localeImage, remoteImage;

         if (m_runningMode == NORMAL_MODE)
    {
        qDebug() << "\tImage: "<< m_remoteImagesList[m_imageIndex];
        localeImage = m_localImagesList[m_imageIndex];
        remoteImage = m_remoteImagesList[m_imageIndex];
    }
    else if (m_runningMode == PRECLIPPING_MODE)
    {
        qDebug() << "\tPreclipping Image: "<< m_preclippedRemoteImagesList[m_preclippedImageIndex];
        localeImage = m_preclippedLocalImagesList[m_preclippedImageIndex];
        remoteImage = m_preclippedRemoteImagesList[m_preclippedImageIndex];
    }
    else if (m_runningMode == CLIPPING_MODE )
    {
        qDebug() << "\tClipping Image: "<< m_clippedRemoteImagesList[m_clippedImageIndex];
        localeImage = m_clippedLocalImagesList[m_clippedImageIndex];
        remoteImage = m_clippedRemoteImagesList[m_clippedImageIndex];
    }

    /*int lockedBy;
    if( isImageLocked(localeImage, lockedBy) )
    {
        QString username = User::username(m_slaveDatabase, lockedBy);
        QMessageBox::warning(this, tr("Page Locked"), tr("You cant load this page because its locked by user (%1)").arg(username) );
        return false;
    }

    lockImage(localeImage);*/

    if( !m_cachedImages[ localeImage ] )
    {
        if ( !m_imageCachingDialog.cachingThreadStarted() )
        {
            ret = cacheOnePage(remoteImage, localeImage);
        }
        else
        {
            m_imageCachingDialog.pause();
            ret = cacheOnePage(remoteImage, localeImage);
        }
    }

    if(ret)
        ret = ui->fullpageView->load(localeImage);

    if(ret)
    {
        QStringList parts = localeImage.split("/");
        QString image = parts[parts.count()-1];
        m_currentPageId = getImageId(image);
        loadImageHighlights(m_currentPageId);//load the highlights
    }

//    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
//    qDebug("loadImage() Time: %f", diff);

    cache();

    if(m_isPaused)  //if in pause state, change it to resume.
        pauseTracking();

    m_trackingSeconds = 0;
    m_trackingTimer.start(1000);

    int pubIssueId = publicationIssueId(m_publication.id(), m_publication.currentIssue()->date());
    int issueSecId = issueSectionsId(pubIssueId, m_publication.currentIssue()->section(m_currentSectionName).id());

    m_user.track(m_currentPageId, m_publication.id(), pubIssueId, issueSecId,
                 m_publication.type(), m_publication.country().toInt(),
                 m_publication.language(), m_publication.currentIssue()->date(), m_publication.currentIssue()->created());

    firstPage = false;

    return ret;
}

void ClippingStation::cache()
{
    //qDebug("cache()");
    clock_t start = clock();

    if(m_runningMode == NORMAL_MODE)
        m_imageCachingDialog.setValues(m_remoteImagesList, m_localImagesList);
    else if(m_runningMode == CLIPPING_MODE )
        m_imageCachingDialog.setValues(m_clippedRemoteImagesList, m_clippedLocalImagesList);
    else if(m_runningMode == PRECLIPPING_MODE )
        m_imageCachingDialog.setValues(m_preclippedRemoteImagesList, m_preclippedLocalImagesList);

    m_imageCachingDialog.startCaching();
    m_imageCachingDialog.show();        //m_imageCachingDialog.exec();

    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
    qDebug("cache() Time: %f", diff);

}

bool ClippingStation::cacheOnePage(QString srcPage, QString dstPage)
{
    clock_t start = clock();

    m_imageCachingDialog.pause();
    return m_imageCachingDialog.cacheOnePage(srcPage, dstPage);

    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;
    qDebug("cacheOnePage() Time: %f", diff);
}

bool ClippingStation::recursiveDelete(const QDir &dir)
{
    QList<QFileInfo> infLst = dir.entryInfoList();

    foreach(QFileInfo finfo, infLst )
    {
        if(finfo.fileName() == "." || finfo.fileName() == ".." )
            continue;

        if(finfo.isDir() )
        {
            QDir subDir(finfo.absoluteFilePath());
            qDebug() << "SubDir: " << subDir.absolutePath().toAscii();
            QDir dir1 = dir;

            if ( recursiveDelete(subDir) )
            {
                dir1.cdUp();
                if ( !dir1.rmdir(finfo.absoluteFilePath() ) )
                {
                    qDebug() << "Error deleting directory: " << finfo.absoluteFilePath().toAscii();
                }
            }
        }
        else
        { //file
            QDir dir1;
            if( !dir1.remove(finfo.absoluteFilePath()) )
                qDebug() << "Error deleting file: " << finfo.absoluteFilePath();
        }
    }

    return true;
}

void ClippingStation::searchKeyword()
{
    QString search = ui->keywordLineEdit->text();
    if( search.isEmpty() || search.isNull() )
        return;

    m_tagsWrapper.searchKeyword(m_masterDatabase, search);
}

void ClippingStation::checkAllKeywords()
{
    m_tagsWrapper.checkAllKeywords();
}

void ClippingStation::uncheckAllKeywords()
{
    m_tagsWrapper.uncheckAllKeywords();
}

void ClippingStation::selectKeyword()
{
    m_tagsWrapper.selectKeyword();
}

void ClippingStation::clipsTreeContextMenu(QPoint point)
{
    QModelIndex index = ui->clipsTreeView->indexAt(point);
    if( index.isValid() )
    {
        point.setY(point.y() + 25);
        QAbstractItemModel *model = ui->clipsTreeView->model();

        QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );
        QModelIndex clipRootIndex    = model->index(1, 0, QModelIndex() );
        //if the user clicked on top level entries, just return
        if ( index ==  preclipRootIndex  ||
             index ==  clipRootIndex         )
            return;

        int id;
        if ( index.parent() == preclipRootIndex ||
             index.parent().parent() == preclipRootIndex )
        {
            m_runningMode = PRECLIPPING_MODE;
            QModelIndex currentIndex;

            m_clippedImageModelIndex = QModelIndex();


            //if the selection is the Preclip(), then it will contain the id
            if(index.parent() == preclipRootIndex)
            {
                id = model->index(index.row(),         1, index.parent()           ).data(Qt::DisplayRole).toInt();
                //currentIndex = index.child(0, 0);
            }
            else
            {
                id = model->index(index.parent().row(), 1, index.parent().parent() ).data(Qt::DisplayRole).toInt();
                //currentIndex = index;
            }

            m_preclipContextMenu->exec( ui->clipsTreeView->mapToGlobal(point));
        }
        else
        {
            m_clipContextMenu->exec( ui->clipsTreeView->mapToGlobal(point));
        }
    }
}

void ClippingStation::issueTreeContextMenu(QPoint point)
{
    QModelIndex index = ui->issuesTreeView->indexAt(point);
    if( index.isValid() )
    {
        point.setY(point.y() + 25);
        QAbstractItemModel *model = ui->issuesTreeView->model();

        if(index == QModelIndex() )
            return;

        if(model->parent(index) == QModelIndex() )
        {
            m_issueContextMenu->exec(ui->issuesTreeView->mapToGlobal(point));
            return;
        }

        if(model->parent(index).parent() == QModelIndex() )
            return;

        if(model->parent(index).parent().parent() == QModelIndex() )            //its the image
            m_issuePageContextMenu->exec( ui->issuesTreeView->mapToGlobal(point));
        else
            qDebug() << "not an image selection";
   }
}

void ClippingStation::keywordsTreeContextMenu(QPoint point)
{
    QModelIndex index = ui->treeView->indexAt(point);
    if(!index.isValid())
        return;

    point.setY(point.y()+25);
//  QAbstractItemModel *model = ui->treeView->model();

    m_keywordsContextMenu->exec( ui->treeView->mapToGlobal(point));
}

void ClippingStation::preclip()
{
    qDebug("preclip()");

    QList<ImageCutout *> cutoutsList = ui->fullpageView->imageCutoutsList();
    if(cutoutsList.count() == 0)
    {
        QMessageBox::information(this, "No selections", "Please select at least one cutout to be able to clip");
        return;
    }

    QList<Tag> tags = m_tagsWrapper.ocrTags();
    if( tags.count() == 0 )
    {
        QMessageBox::information(this, "No Tags", "Please add the associated tags to be able to clip");
        return;
    }


    qreal image_size            = ui->fullpageView->pageSize();

    if(ui->preclipButton->text() == tr("Modify"))
    {
        m_publication.currentIssue()->preclip(m_currentPreclipId).updateArticle(m_masterDatabase, m_publication, tags, cutoutsList, image_size);
        m_publication.currentIssue()->addArticleToView(m_masterDatabase, ui->clipsTreeView, m_currentPreclipId, false, true);
    }
    else
    {
        int articleId = Article::clip(m_masterDatabase, m_publication, m_user.id(), tags, cutoutsList, m_currentSectionName, image_size, 0, 1, "");
        m_publication.currentIssue()->addArticleToView(m_masterDatabase, ui->clipsTreeView, articleId, false, false);
    }

    drawerResetPages();
    m_firstPageInClip   = "";   //reset pages for clipping
    m_currentPreclipId  = 0;    //reset the preclip id;
    m_currentClipId     = 0;


        //loadIssuesWithPreclipsOnly();

    m_tagsWrapper.clearTags();
    ui->fullpageView->removeImageCutouts();

    m_publication.addImageCutoutsToIssues(ui->issuesTreeView, false);

    setModifyPreclipButton(false);

    m_runningMode = NORMAL_MODE;

    ui->statusBar->showMessage("Preclipping successfull");

    qDebug("end of preclip()");
}

void ClippingStation::clip()
{
    qDebug("clip()");

    qDebug("before cutoutList()");
    QList<ImageCutout *> cutoutsList = ui->fullpageView->imageCutoutsList();

    if(cutoutsList.count() == 0)
    {
        QMessageBox::information(this, "No selections", "Please select at least one area to be able to clip");
        return;
    }

    qDebug("before ocrTags()");
    QList<Tag> tags = m_tagsWrapper.ocrTags();
    if( tags.count() == 0 )
    {
        QMessageBox::information(this, "No Tags", "Please add the associated tags to be able to clip");
        return;
    }

    static QSqlDatabase db = m_masterDatabase.cloneDatabase(m_masterDatabase, "master2222");
    db.open();

    if (m_runningMode != CLIPPING_MODE)
    {
        int cutoutsCount  = totalNumberOfCutoutsInDrawer();
        if( cutoutsCount <= 0 )
        {
            QMessageBox::information(this, "Blank clipping", "Please add the cut outs be able to clip");
            return;
        }

        QStringList pages = emptyDrawerPages();
        if(pages.count() > 0)
        {
            QString page;
            if(pages.count() == 1)
                page = QString("Page (%1) in drawer is empty, please remove it before clipping").arg(pages[0]);
            else
                page = QString("Pages (%1) in drawer are empty, please remove them before clipping").arg(pages.join(","));

            QMessageBox::information(this, "Empty pages", page);
            return;
        }

        if( cutoutsList.count() > cutoutsCount )
        {
            switch( QMessageBox::question(
                    this,
                    tr("Warning"),
                    tr("You only copied %1/%2 cutouts from left pane, Do you want to CONTINUE the clipping process?").arg( cutoutsCount ).arg( cutoutsList.count() ),
                    QMessageBox::Yes |
                    QMessageBox::No,
                    QMessageBox::No) )
            {
            case QMessageBox::Yes:
                break;
            case QMessageBox::No:
                return;
            default:
                return;
            }
        }
    }

    if (m_runningMode == PRECLIPPING_MODE)
    {
        int images_number           = ui->stackedDrawerViewWidget->count();     //the count of image
        QString text                = ui->fullpageView->text();

        bool isArabic = (m_publication.language() == "Arabic");
        m_publication.currentIssue()->preclip(m_currentPreclipId).updateTags(m_masterDatabase, tags, isArabic);
        m_publication.currentIssue()->preclip(m_currentPreclipId).updateImagesNumber(m_masterDatabase, images_number);
        m_publication.currentIssue()->preclip(m_currentPreclipId).updateProminence(m_masterDatabase, m_publication, tags, m_user.id());

        QList<QImage> list = createClipImages();
        bool ret = true;

        if(m_useFtp)
        {
            QFuture<bool> future = QtConcurrent::run(&saveClipImagesToFtpServer, m_currentIssueDate, m_currentPreclipId, list, true, m_masterDatabase);
            m_saveClipImagesToFtpServerWatcher.setFuture(future);
        }
        else
        {
            QFuture<int> future = QtConcurrent::run(&saveClipImagesToFileServer, m_currentIssueDate, m_currentPreclipId, list);
            m_saveClipImagesToFileServerWatcher.setFuture(future);            //ret = saveClipImagesToFileServer(m_currentPreclipId, list);         //save the clip images in the clip directories
            //ret = saveClipImagesToFileServer(m_currentIssueDate, m_currentPreclipId, list, true, m_masterDatabase);
        }

        if(!m_publication.skipOcr())
        {
            qDebug() << m_publication.name() << " not skip OCR, Insert to db";
            bool isArabic = (m_publication.language() == "Arabic");
            QFuture<bool> future2 = QtConcurrent::run(&addTextToClip, m_currentPreclipId, cutoutsList, db, isArabic);
            m_addTextToClipWatcher.setFuture(future2);
        }
        else
            qDebug() << m_publication.name() << " has skip OCR set, wont insert text to db";


        if(!ret)
        {
            m_publication.currentIssue()->preclip(m_currentPreclipId).updateStatus(m_masterDatabase, 1);
            ui->statusBar->showMessage("Clipping Error");
        }
        else
        {
            m_publication.currentIssue()->addArticleToView(m_masterDatabase, ui->clipsTreeView, m_currentPreclipId, true, true);
            ui->statusBar->showMessage("Clipping successfull");
        }
    }
    else if (m_runningMode == CLIPPING_MODE)
    {
        bool isArabic = (m_publication.language() == "Arabic");
        m_publication.currentIssue()->clip(m_currentClipId).updateTags(m_masterDatabase, tags, isArabic);
        m_publication.currentIssue()->clip(m_currentClipId).updateProminence(m_masterDatabase, m_publication, tags, m_user.id());

        ui->statusBar->showMessage("Clip modified successfully");
    }
    else    //fresh clip
    {

        qreal image_size            = ui->fullpageView->pageSize();
        int images_number           = ui->stackedDrawerViewWidget->count();     //the cound of image
        QString text                = ui->fullpageView->text();

        int id_article = Article::clip(m_masterDatabase, m_publication, m_user.id(), tags, cutoutsList, m_currentSectionName, image_size, images_number, 1);

        //create the images
        QList<QImage> list = createClipImages();
        bool ret = true;
        if(m_useFtp)
        {
            QFuture<bool> future = QtConcurrent::run(&saveClipImagesToFtpServer, m_currentIssueDate, id_article, list, false, m_masterDatabase);
            m_saveClipImagesToFtpServerWatcher.setFuture(future);
        }
        else
        {
            QFuture<int> future = QtConcurrent::run(&saveClipImagesToFileServer, m_currentIssueDate, id_article, list);
            m_saveClipImagesToFileServerWatcher.setFuture(future);            //ret = saveClipImagesToFileServer(m_currentPreclipId, list);         //save the clip images in the clip directories
            //ret = saveClipImagesToFileServer(m_currentIssueDate, id_article, list, true, m_masterDatabase);
        }

        if(!m_publication.skipOcr())
        {
            qDebug() << m_publication.name() << " No skip OCR set, insert text to db";
            //QSqlDatabase db = m_masterDatabase.cloneDatabase(m_masterDatabase, "master2222");
            //db.open();
            bool isArabic = (m_publication.language() == "Arabic");
            QFuture<bool> future2 = QtConcurrent::run(&addTextToClip, id_article, cutoutsList, db, isArabic);
            m_addTextToClipWatcher.setFuture(future2);
        }
        else
            qDebug() << m_publication.name() << " has skip OCR set, wont insert text to db";

        if(!ret)
        {
            Article::deleteArticle(m_masterDatabase, id_article);
            ui->statusBar->showMessage("Clipping Error");
        }
        else
        {
            m_publication.currentIssue()->addArticleToView(m_masterDatabase, ui->clipsTreeView, id_article, true, false);
            ui->statusBar->showMessage("Clipping successfull");
        }
    }


    drawerResetPages();
    m_firstPageInClip   = "";     //reset pages for clipping
    m_currentPreclipId  = 0;     //reset the preclip id;
    m_currentClipId     = 0;


    //if (m_filterMode == PRECLIPSONLY_FILTER)
        //loadIssuesWithPreclipsOnly();
    ui->fullpageView->removeImageCutouts();

    m_publication.addImageCutoutsToIssues(ui->issuesTreeView, false);
    m_tagsWrapper.clearTags();
    //m_tagsWrapper.showModel();

    m_runningMode = NORMAL_MODE;

    if (m_filterMode == PRECLIPSONLY_FILTER)
    {
        //go to the next preclip
        QAbstractItemModel *model = ui->clipsTreeView->model();

        QModelIndex preclipRootIndex = model->index(0, 0, QModelIndex() );
        //if(preclipRootIndex.model()->children().count() > 0)
        {
            QModelIndex pindex = preclipRootIndex.child(0, 0);
            if(pindex.isValid())
                articleTreeViewClicked(pindex);
            else
            {
                //we have reached the end of this issues preclip, so, remove the issue from list.
                m_publication.removeIssueFromIssues(ui->issuesTreeView, m_currentIssueDate);
                m_currentIssueDate = "";
                //now go to the next issue
                QAbstractItemModel *model = ui->issuesTreeView->model();

                QModelIndex issuesIndex = model->index(0, 0, QModelIndex() );
                if( issuesIndex.isValid() )
                    issueTreeViewItemClicked(issuesIndex);
                else
                {
                    //remove the publication from the drop-down list
                    ui->preclippedPublicationComboBox->removeItem(ui->preclippedPublicationComboBox->currentIndex());
                }
            }
        }
    }

    qDebug("end of clip()");
}

void ClippingStation::drawerResetPages()
{
    qDebug("drawerResetPages()");

    int count = ui->stackedDrawerViewWidget->count();
    for(int i=count -1 ; i >= 0; i--)
    {
        ui->stackedDrawerViewWidget->setCurrentIndex(i);
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        ui->stackedDrawerViewWidget->removeWidget( ui->stackedDrawerViewWidget->widget(i) );
        delete view;
    }
    drawerAddPage(true);

}

void ClippingStation::drawerAddPage(bool isfirst)
{

    DrawerView * newview = new DrawerView(isfirst, ui->page);

    qreal oldscale = 1.0;       //for the first page insertion
    //first we restore all the current pages to actual size, add the new page, and then restore them back to current size;
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        oldscale = view->actualSize();
    }

    ui->stackedDrawerViewWidget->addWidget(newview);

    //restore to old scale value
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        view->setScale(oldscale);
    }


    ui->stackedDrawerViewWidget->setCurrentIndex(ui->stackedDrawerViewWidget->count() - 1);

    if(isfirst)
        drawerBestFit();            //best fit mod
    //    else
    //        newview->setPageLayout(ACTUAL_SIZE);
    m_currentA4Scale = newview->currentScale();
    //qDebug("Add Page scale %.2f", m_currentA4Scale);

    updateDrawerPageInformation();
    connect(newview, SIGNAL(createThumbnail()), this, SLOT(createThumbnail()));

}

void ClippingStation::drawerRemovePage()
{
    int i = ui->stackedDrawerViewWidget->currentIndex();
    if (i == 0)     //first page
    {
        QMessageBox::critical(this, "Error", "You cant remove the first page in drawer");
        return;
    }

    switch( QMessageBox::question(
            this,
            tr("Delete Page"),
            tr("Delete page from the Drawer, are you sure?"),
            QMessageBox::Yes |
            QMessageBox::No,
            QMessageBox::No) )
    {
    case QMessageBox::Yes:
        break;
    case QMessageBox::No:
        return;
    default:
        return;
    }

    //DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
    //view->removeEventFilter(this);
    ui->stackedDrawerViewWidget->removeWidget( ui->stackedDrawerViewWidget->widget(i) );
    updateDrawerPageInformation();
}

void ClippingStation::drawerPreviousPage()
{
    int currentIndex = ui->stackedDrawerViewWidget->currentIndex();
    if (currentIndex != 0)
    {
        A4Scale();
        ui->stackedDrawerViewWidget->setCurrentIndex(currentIndex-1);
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->currentWidget());
        view->setScale(m_currentA4Scale);
        updateDrawerPageInformation();
    }
}

void ClippingStation::drawerNextPage()
{
    int currentIndex = ui->stackedDrawerViewWidget->currentIndex();
    if (currentIndex != ui->stackedDrawerViewWidget->count() - 1)
    {
        A4Scale();
        ui->stackedDrawerViewWidget->setCurrentIndex(currentIndex+1);
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->currentWidget());
        view->setScale(m_currentA4Scale);
        updateDrawerPageInformation();
    }
}

QList<QImage> ClippingStation::createClipImages()
{
    QList<QImage> list;
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));

        ui->stackedDrawerViewWidget->setCurrentIndex(i);
        if(view->cutoutsCount() > 0)
        {
            QImage img = view->screenShot();
            list.append(img);
        }
    }

    return list;
}

int ClippingStation::saveClipImagesToFileServer(QString currentIssue, int id_article, QList<QImage> &list)
{
    mutex.lock();
    qDebug("saveClipImages() for article %d", id_article);
    //we save the images on this format ClippingsPath/YYYY/MM/DD/ID_ARTICLE-X

    QString path = SystemConfiguration::clippingsPath();

    QStringList dateSplit = currentIssue.split("-");
    path  = path + QString("%1/%2/%3/").arg(dateSplit[0], 4, QChar('0')).arg(dateSplit[1], 2, QChar('0')).arg(dateSplit[2], 2, QChar('0'));


#ifdef Q_WS_WIN
    path = SystemConfiguration::clippingsWindowsDriveLetter() + path;   //if we are under Windows, add Windows drive letter
#endif
    QDir dir;

    qDebug("saveClipImages() for article %d: create the directory structure", id_article);
    if( !dir.exists(path) )
    {
        if( !dir.mkpath(path) )
        {
            QMessageBox::critical(0, tr("saveClipImagesToFileServer() Error"), tr("Error creating directory structure for clippings!"));
            qDebug("saveClipImagesToFileServer(): Error creating directory ");
            mutex.unlock();
            return 0;
        }
    }

    bool ret = true;
    int i = 0;

    //first save the file locally, and then move it the file server
    qDebug("saveClipImages(): create clippings directory in cache");

    QString localpath = m_cacheDir + "clippings/";
    QDir dir2;
    if( !dir2.exists(localpath) )
    {
        if( !dir2.mkpath(localpath) )
        {
            QMessageBox::critical(0, tr("saveClipImagesToFileServer() Error"), tr("Error creating directory structure for local clippings!"));
            qDebug("saveClipImagesToFileServer(): Error creating directory ");
            mutex.unlock();
            return 0;
        }
    }

    qDebug("saveClipImages(): trying to save %d images", list.count() );
    QFile file;
    for(i=0; i< list.count(); i++)
    {
        QString fname = QString("%1-%2.jpg").arg(id_article).arg(i+1);
        //if( file.exists(localpath + fname) || file.exists(path + fname) ) //try to remove the files if they exist
        {
            file.remove(localpath+ fname);        //remove the file from cache if it already exist (from previous un-successful clip)
            file.remove(path     + fname);        //and from the file server
        }

        ret = list[i].save(localpath + fname, "jpg", 100);
        if(ret)
        {
            qDebug() << "saveClipImages(): saving image " << localpath+fname << " Successfull";


            if (file.copy(localpath + fname, path + fname))
                qDebug() << "saveClipImages(): copying image to " << path + fname << " Successfull";
            else
            {
                qDebug() << "saveClipImages(): copying image to " << path + fname << " Unsuccessfull: reason: " << file.errorString();
                ret = false;
                break;
            }
        }
        else
        {
            qDebug() << "saveClipImages(): Saving image " << fname << " Unsuccessfull";
            ret = false;
            break;
        }
    }

    if(!ret)
    {
        //error happend in the complete set of images, so we delete what is already created
        qDebug() << "saveClipImages(): Error happened while saving, remove the saved images!";
        for(int j=0; j< i; j++)
        {
            QString fname = QString("%1-%2.jpg").arg(id_article).arg(j+1);
            file.remove(localpath +fname);    //remove it locally
            file.remove(path      +fname);    //and from file server
        }
        mutex.unlock();
        return 0;
    }

    //we also check to see if the image does REALLY exist on that folder
    qDebug() << "saveClipImages(): Checking if the images exist on the server and the images sizes is larger than 30kb";
    for(int i=0; i< list.count(); i++)
    {
        QString fname = path + QString("%1-%2.jpg").arg(id_article).arg(i+1);
        QFileInfo fi(fname);
        if( !fi.exists() || (fi.size() < 20 * 1024))
        {
            qDebug() << "saveClipImages(): Image " << fname << " DOES not exist, or size less than 30 kb: " << file.errorString();
            ret= false;
            break;
        }
        else
            qDebug() << "saveClipImages(): Image " << fname << " exist on the server";
    }

    //now save the thumbnail to server
    qDebug() << "saveClipImages(): save the thumbnail";
    QString thumbnailName = QString("%1_thumb.jpg").arg(id_article);

    if(file.exists(localpath+ thumbnailName))
    {
        file.remove(localpath + thumbnailName);
        file.remove(path + thumbnailName);
    }

    m_thumbnail.save(localpath + thumbnailName, "jpg", 100);
    file.copy(localpath + thumbnailName, path + thumbnailName);
    qDebug() << "saveClipImages(): save the thumbnail DONE";

/*
    qDebug() << "saveClipImages(): Trying to open the saved images";
    for(int i=0; i< list.count(); i++)
    {
        QString fname = path + QString("%1-%2.jpg").arg(id_article).arg(i+1);
        QFile file(fname);
        if( !file.open(QFile::ReadOnly) )
        {
            qDebug() << "saveClipImages(): Image " << fname << " DOES not open, reason: " << file.errorString();
            ret= false;
            break;
        }
        else
            qDebug() << "saveClipImages(): Image " << fname << " opens OK";
        file.close();
    }
*/

    if(ret)
    {
        Article::updateImagesNumber(m_clipDatabase, id_article, list.count());
        Article::updateStatus(m_clipDatabase, id_article, 2);
        qDebug("saveClipImages(): Everything is OK for article %d, updating the status =2 and images number = %d", id_article, list.count());
        mutex.unlock();
        return id_article;
    }

    //if error happens, return to preclip, if pre-clip, or delete it from clipping
    Article::updateImagesNumber(m_clipDatabase, id_article, 0);
    qDebug("saveClipImages(): Error happend for article %d. roll back to original values", id_article);
    mutex.unlock();
    return 0;
}

bool ClippingStation::saveClipImagesToFtpServer (QString &currentIssue, int &id_article, QList<QImage> &list, bool fromPreclip, QSqlDatabase &database)
{
    QString path = SystemConfiguration::clippingsPath();

    QStringList dateSplit = currentIssue.split("-");
    path  = path + QString("%1/%2/%3/").arg(dateSplit[0], 4, QChar('0')).arg(dateSplit[1], 2, QChar('0')).arg(dateSplit[2], 2, QChar('0'));

    CURL *curlhandle = NULL;        //the libcurl easy handle

    path = QString("ftp://%1:%2@%3/").arg(m_username).arg(m_password).arg(m_address) +  path;

    curl_global_init(CURL_GLOBAL_ALL);      //initate it
    curlhandle = curl_easy_init();

    bool ret = true;
    char str[255];
    saveClipImagesToFtpServerErrorMsg = "";

    for(int i=0; i< list.count(); i++)
    {
        QString tmpfilename = QString("%1-%2.jpg").arg(id_article).arg(i+1);
        QString filename = path + tmpfilename;

        if( list[i].save(tmpfilename, "jpg", 100) )
        {
            ret = upload(curlhandle, filename.toAscii(), tmpfilename.toAscii(), 0, 3, str);
            if(!ret)
            {
                qDebug() << "saveClipImagesToFtpServer(): Error uploading image " << filename.toAscii() << ": " << str;
                QFile::remove(tmpfilename);
                saveClipImagesToFtpServerErrorMsg = str;
                ret = false;
                break;
            }

            QFile::remove(tmpfilename);
        }
        else
        {
            ret = false;
            break;
        }
    }

    //now save the thumbnail image as well
    {
        QString tmpfilename = QString("%1_thumb.jpg").arg(id_article);
        QString filename = path + tmpfilename;

        if( m_thumbnail.save(tmpfilename, "jpg", 100) )
        {
            ret = upload(curlhandle, filename.toAscii(), tmpfilename.toAscii(), 0, 3, str);
            if(!ret)
            {
                qDebug() << "saveClipImagesToFtpServer(): Error uploading image " << filename.toAscii() << ": " << str;
                QFile::remove(tmpfilename);
                saveClipImagesToFtpServerErrorMsg = str;
                //ret = false;
                //break;
            }

            QFile::remove(tmpfilename);
        }
    }

    if(ret)
        return true;

    if(fromPreclip)
        Article::updateStatus(database, id_article, 1);
    else
        Article::deleteArticle(database, id_article);

    return false;
}

bool ClippingStation::addTextToClip(int id_article, QList<ImageCutout*> &list, QSqlDatabase &database, bool isArabic)
{
    //first, get the words from database, and then get the text and add it to database.
    qDebug() << "addTextToClip()";

    QStringList pages = ImageCutout::fullPages(list);

    QHash<QString, QList<WordCoordinates> > wcHash;

    for(int i=0; i< pages.count(); i++)
    {
        QList<WordCoordinates> wc = loadImageText(pages[i], database);
        qDebug("word counts = %d", wc.count());
        wcHash.insert(pages[i], wc);
    }

    QString headline = "";
    //now get coordinates of the text
    QString text = "";
    for(int i=0; i< list.count(); i++)
    {
        QString pagename = list[i]->pageName();
        QString t = getTextFromCutout( list[i]->boundingRect().toRect(), wcHash.value(pagename) );
        text+= t + " ";
        if(list[i]->areaType() == TITLE)
            headline += t + " ";
    }

    qDebug() << "addTextToClip() headline is : " << headline;
    qDebug() << "addTextToClip() Text     is : " << text;

    //now add the text to clip.
    Article::updateText(database, id_article, text);

    if(headline.trimmed().isEmpty())
    {
        int index = text.indexOf("\n");
        if(index > -1 && index < text.length())
            headline = text.left(index);
        else
            headline = text.left(100).trimmed();

        qDebug() << "addTextToClip() new headline is : " << headline;
    }

    if(!headline.trimmed().isEmpty())
        Article::updateHeadline(database, id_article, headline, isArabic);

    qDebug() << "end of addTextToClip()";
    return true;
}

QString ClippingStation::getTextFromCutout( QRect rect, QList<WordCoordinates> wc )
{
    QStringList words;

    for(int i=0; i<wc.count(); i++)
    {
            Coordinates crd = wc[i].coordinates();
            QPointF pt1(crd.left(), crd.top());
            QPointF pt2(crd.right(), crd.bottom());

            QRectF rec(pt1, pt2);

            if( rect.contains( rec.toRect() ) )
                words.append(wc[i].word());
    }
    return words.join(" ");
}

void ClippingStation::saveClipImagesToFtpServerWatcherFinished()
{
    bool t = m_saveClipImagesToFtpServerWatcher.result();
    if(!t)  //if error happen, show a message
        QMessageBox::critical(this, tr("Ftp Clipping Error"), tr("Error uploading clipping images to Ftp (%1)").arg(saveClipImagesToFtpServerErrorMsg) );
}

void ClippingStation::saveClipImagesToFileServerWatcherFinished()
{
    int id = m_saveClipImagesToFileServerWatcher.result();
    if(id == 0)  //if error happen, show a message
        QMessageBox::critical(this, tr("Clipping Error"), tr("Error copying clipping images to file server (%1)").arg(saveClipImagesToFtpServerErrorMsg) );
    //else
    //    Article::updateStatus(m_masterDatabase, id, 2);
}

void ClippingStation::addTextToClipWatcherFinished()
{

}

void ClippingStation::on_rejectButton_clicked()
{
    //reject the current clip, i.e. set the reject value to 1.
    //m_currentClipId;
    QSqlQuery query(m_masterDatabase);

    query.prepare("UPDATE article SET reject = 1 where id_article = :id");
    query.bindValue(":id", m_currentClipId);

    if( query.exec() )
    {
        //the clip is rejected, move it from the list
        m_publication.currentIssue()->deleteArticleFromView(ui->clipsTreeView, m_currentClipId, false);
        drawerResetPages();
        //remove the cutouts
        ui->fullpageView->removeImageCutouts();
    }
}

void ClippingStation::thumbnail()
{
    //first restore the page to its original size, then take the screenshot
    if ( m_firstPageInClip.isEmpty() )
    {
        if (m_runningMode == PRECLIPPING_MODE      )           m_firstPageInClip = m_preclippedLocalImagesList  [m_preclippedImageIndex];
        else if (m_runningMode == NORMAL_MODE      )           m_firstPageInClip = m_localImagesList            [m_imageIndex          ];
        else if (m_runningMode == CLIPPING_MODE    )           m_firstPageInClip = m_clippedLocalImagesList     [m_clippedImageIndex   ];
    }

    QString currentPage;
    if (m_runningMode == PRECLIPPING_MODE      )           currentPage = m_preclippedLocalImagesList  [m_preclippedImageIndex];
    else if (m_runningMode == NORMAL_MODE      )           currentPage = m_localImagesList            [m_imageIndex          ];
    else if (m_runningMode == CLIPPING_MODE    )           currentPage = m_clippedLocalImagesList     [m_clippedImageIndex   ];

    if ( m_firstPageInClip == currentPage)
    {
        qreal oldscale = ui->fullpageView->actualSize();
        QImage img = ui->fullpageView->screenShot();
        ui->fullpageView->setScale(oldscale);

        img = img.scaledToWidth(THUMBNAIL_WIDTH, Qt::SmoothTransformation);
        m_thumbnail = img;

        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(0));
        view->addThumbnail(img);
    }
}

void ClippingStation::createThumbnail()
{
    thumbnail();
}

qreal ClippingStation::A4Scale()
{
    int i = ui->stackedDrawerViewWidget->currentIndex();
    DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));

    m_currentA4Scale = view->currentScale();

    return m_currentA4Scale;
}

void ClippingStation::enableDrawerButtons(bool enable)
{
    ui->addPageButton       ->setEnabled(enable);
    ui->removePageButton    ->setEnabled(enable);
    ui->clipButton          ->setEnabled(enable);
    ui->rejectButton        ->setEnabled(!enable);
}

void ClippingStation::setModifyClipButton(bool modify)
{
    ui->clipButton->setEnabled(true);
    if(modify)
        ui->clipButton->setText(tr("Modify"));
    else
        ui->clipButton->setText(tr("Clip"));
}

void ClippingStation::setModifyPreclipButton(bool modify)
{
    if(modify)
    {
        ui->preclipButton->setText(tr("Modify"));
        ui->cancelModifyPreclipButton->setVisible(true);
    }
    else
    {
        ui->preclipButton->setText(tr("Preclip"));
        ui->cancelModifyPreclipButton->setVisible(false);
    }
}

void ClippingStation::cancelModifyPreclip()
{
    qDebug("cancelModifyPreclip()");

    setModifyPreclipButton(false);
    loadImage();
}

void ClippingStation::loadFilteredPublication(int index)
{
    if( index == 0 )
    {
        m_filterMode = NO_FILTER;
        m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);
        return;
    }
    else if( index == 1)
    {
        m_filterMode = PRECLIPSONLY_FILTER;
        m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);

        QStringList list = Publication::publicationsWithPreclips(m_slaveDatabase);
        list.insert(0, "");

        ui->preclippedPublicationComboBox->clear();
        ui->preclippedPublicationComboBox->addItems(list);
    }
    else if(index == 2)
    {
        m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);
    }
    else if(index == 3)
    {
        m_filterMode = NO_FILTER;
        m_publication.initalizeViews(ui->issuesTreeView, ui->clipsTreeView);

        QStringList list = Publication::publicationsWithUndoneIssues(m_slaveDatabase);
        list.insert(0, "");

        ui->undonePublicationComboBox->clear();
        ui->undonePublicationComboBox->addItems(list);
    }
}

void ClippingStation::loadClippingImages(int clipid)
{
    qDebug("loadClippingImages()");

    //get the number of a4 images for this clip, in addition to the path
    QString path = SystemConfiguration::clippingsPath();

    QStringList dateSplit = m_currentIssueDate.split("-");
    path  = path + QString("%1/%2/%3/").arg(dateSplit[0], 4, QChar('0')).arg(dateSplit[1], 2, QChar('0')).arg(dateSplit[2], 2, QChar('0'));

    CURL *curlhandle = NULL;        //the libcurl easy handle

    if( m_useFtp)
    {
        path = QString("ftp://%1:%2@%3/").arg(m_username).arg(m_password).arg(m_address) +  path;

        curl_global_init(CURL_GLOBAL_ALL);      //initate it
        curlhandle = curl_easy_init();
    }
    else
    {
#ifdef Q_WS_WIN
        path = SystemConfiguration::clippingsWindowsDriveLetter() + path;
#endif
    }

    drawerResetPages();
    int count = m_publication.currentIssue()->clip(clipid).imagesNumber();      //get number of clipped images
    char str[255];

    for(int i=0; i< count -1 ; i++)     //add pages
        drawerAddPage(false);

    for(int i=0; i< count; i++)
    {
        QString tmpfilename = QString("%1-%2.jpg").arg(clipid).arg(i+1);
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));

        if(m_useFtp)
        {
            QString file = path+ tmpfilename;
            if (!download(curlhandle, file.toAscii(), tmpfilename.toAscii(), 0, 3, str))
            {
                qDebug() << "loadClippingImages(): Error loading image " << file.toAscii() << ": " << str;
                QMessageBox::critical(this, "error loading clip", QString("Ftp Error (%1) : %2").arg(file).arg(str) );
            }
            else
            {
                view->setPixmap(QPixmap(tmpfilename));
                QFile::remove(tmpfilename);
            }
        }
        else
        {
            QString fname = path+ tmpfilename;
            view->setPixmap(QPixmap(fname));
        }
    }

    ui->stackedDrawerViewWidget->setCurrentIndex(0);
    updateDrawerPageInformation();
    enableDrawerButtons(false);
    setModifyClipButton(true);
}

int ClippingStation::totalNumberOfCutoutsInDrawer()
{
    int count = 0;
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        count += view->cutoutsCount();
    }
    return count;
}

QStringList ClippingStation::emptyDrawerPages()
{
    QStringList pages;
    for (int i=0; i < ui->stackedDrawerViewWidget->count(); i++)
    {
        DrawerView* view = static_cast<DrawerView*>(ui->stackedDrawerViewWidget->widget(i));
        int count = view->cutoutsCount();
        if(count == 0)
            pages.append( QString("%1").arg(i+1) );
    }
    return pages;
}

void ClippingStation::orderCutoutsSelections()
{
    ui->fullpageView->setOrderMode( ui->orderButton->isChecked() );
    ui->preclipButton->setEnabled( !ui->orderButton->isChecked() );
    ui->autoArrangeButton->setEnabled(!ui->orderButton->isChecked() );
}

void ClippingStation::addMargintoPos(QPointF &pos, bool isfirst)
{
    int dpi = QApplication::desktop()->logicalDpiX();

    MarginValues mv;
    if(isfirst)
        mv = SystemConfiguration::firstPageMargin();
    else
        mv = SystemConfiguration::otherPagesMargin();

    qreal left      = mv.left   * dpi / 2.54;
    qreal top       = mv.top    * dpi / 2.54;

    pos.setX( pos.x() + left );
    pos.setY( pos.y() + top  );
}

void ClippingStation::orderList(QList<ImageCutout*> &cutouts)
{
    for(int i=0; i< cutouts.count() -1 ; i++)
    {
        int min = i;
        for(int j=i+1 ; j< cutouts.count(); j++)
        {
            if(cutouts[min]->order() > cutouts[j]->order())
                min = j;
        }
        if(min != i)
            cutouts.swap(i, min);
    }
}

QSizeF ClippingStation::A4CellSize(bool isfirst)
{
    // we compute pixels dimensions for the page.
    int dpi = QApplication::desktop()->logicalDpiX();
    qreal pageWidth = dpi * A4_WIDTH;
    qreal pageHeight = dpi * A4_HEIGHT;

    MarginValues mv ;

    if(isfirst)
        mv = SystemConfiguration::firstPageMargin();
    else
        mv = SystemConfiguration::otherPagesMargin();

    qreal m_top       = mv.top    * dpi / 2.54;
    qreal m_bottom    = mv.bottom * dpi / 2.54;
    qreal m_left      = mv.left   * dpi / 2.54;
    qreal m_right     = mv.right  * dpi / 2.54;

    qreal w = pageWidth  - (m_left + m_right);
    qreal h = pageHeight - (m_top + m_bottom);

    w /= 100.0;
    h /= 100.0;

    return QSizeF(w, h);
}

bool ClippingStation::calculatePosAndScale(QSizeF cellSize, QImage &img, int type, bool resetarray, QPointF &pos, qreal &scale)
{

#define CELLROWS 100
#define CELLCOLS 100

    scale = 1.0;

    static int cells[CELLROWS][CELLCOLS] = {0};

    if(resetarray)
    {
        for(int j=0; j< CELLROWS; j++)
            for(int i=0; i<CELLCOLS; i++)
                cells[j][i] = 0;
    }

    //we try to find an open space with adjacent h and v cells
    qreal minScale = 1.0;
    if(type == BODY)
        minScale = .9;
    else
        minScale = .4;

    while(scale >= minScale)
    {
        qreal width = img.width() * scale;
        qreal height= img.height()* scale;

        qreal hw = width / cellSize.width();
        qreal hl = height / cellSize.height();

        qreal horizentalCells = ceil(hw);
        qreal verticalCells = ceil(hl);

        bool isArabic = (m_publication.language() == "Arabic");

        if(isArabic)
        {
            for(int row =CELLROWS-1; row >=0; row--)
            {
                for(int col=0; col <CELLCOLS; col++)
                {
                    //appropriate length
                    if(cells[row][col] == 0)
                    {
                        //check if we have the necessary cells to go thru
                        bool valid = true;
                        for(int i=0; i < verticalCells; i++)
                        {
                            if(col+i >= CELLCOLS)
                            {
                                valid = false;
                                break;
                            }

                            if(cells[row][col+i] == 0)
                                continue;

                            valid = false;
                            break;
                        }

                        if(!valid)
                            continue;

                        for(int i=0; i < horizentalCells; i++)
                        {
                            if(row+i >= CELLROWS)
                            {
                                valid = false;
                                break;
                            }

                            if(cells[row+i][col] == 0)
                                continue;

                            valid = false;
                            break;
                        }

                        if(!valid)
                            continue;

                        //now check the internal cells
                        for(int i=0; i<horizentalCells; i++)
                        {
                            for(int j=0; j < verticalCells; j++)
                            {
                                if(cells[row+i][col+j] != 0)
                                {
                                    valid = false;
                                    break;
                                }
                            }
                        }

                        if(!valid)
                            continue;

                        for(int i=0; i<horizentalCells; i++)
                        {
                            for(int j=0; j < verticalCells; j++)
                                cells[row+i][col+j] = 1;
                        }
                        /*
                        qDebug("After the settings of 1's");
                        for(int i=0; i<20; i++)
                        {
                            QString str;
                            for(int j=0; j< 20; j++)
                                str += QString("%1").arg(cells[i][j]) + " ";
                            qDebug() << str.toAscii();
                        }
                        qDebug("*****************************************************");
                        */

                        //point would be
                        pos.setX(row*cellSize.width() );
                        pos.setY(col*cellSize.height());

                        return true;

                    }
                }
            }//end of for
        }
        else        //not arabic
        {
            for(int row =0; row < CELLROWS; row++)
            {
                for(int col=0; col <CELLCOLS; col++)
                {
                    //appropriate length
                    if(cells[row][col] == 0)
                    {
                        //check if we have the necessary cells to go thru
                        bool valid = true;
                        for(int i=0; i < verticalCells; i++)
                        {
                            if(col+i >= CELLCOLS)
                            {
                                valid = false;
                                break;
                            }

                            if(cells[row][col+i] == 0)
                                continue;

                            valid = false;
                            break;
                        }

                        if(!valid)
                            continue;

                        for(int i=0; i < horizentalCells; i++)
                        {
                            if(row+i >= CELLROWS)
                            {
                                valid = false;
                                break;
                            }

                            if(cells[row+i][col] == 0)
                                continue;

                            valid = false;
                            break;
                        }

                        if(!valid)
                            continue;

                        //now check the internal cells
                        for(int i=0; i<horizentalCells; i++)
                        {
                            for(int j=0; j < verticalCells; j++)
                            {
                                if(cells[row+i][col+j] != 0)
                                {
                                    valid = false;
                                    break;
                                }
                            }
                        }

                        if(!valid)
                            continue;

                        for(int i=0; i<horizentalCells; i++)
                        {
                            for(int j=0; j < verticalCells; j++)
                                cells[row+i][col+j] = 1;
                        }
                        /*
                        qDebug("After the settings of 1's");
                        for(int i=0; i<20; i++)
                        {
                            QString str;
                            for(int j=0; j< 20; j++)
                                str += QString("%1").arg(cells[i][j]) + " ";
                            qDebug() << str.toAscii();
                        }
                        qDebug("*****************************************************");
                        */

                        //point would be
                        pos.setX(row*cellSize.width() );
                        pos.setY(col*cellSize.height());

                        return true;

                    }
                }
            }//end of for
        }
        scale -= .02;
    }

    return false;
}

bool ClippingStation::isImageLocked(QString image, int &lockedBy)
{
    clock_t start = clock();

    int id_publication  = m_publication.id();
    int id_issue        = m_publication.currentIssue()->id();
    int id_section      = m_publication.currentIssue()->section(m_currentSectionName).id();

    QFileInfo fi(image);
    QString page_name = fi.fileName();


    QSqlQuery query(m_slaveDatabase);
    bool ret = query.exec("LOCK TABLES publication_issue as pub issue_sections as iss section_pages as sec READ");

    query.prepare("SELECT locked_by FROM publication_issue as pub, issue_sections as iss, section_pages as sec WHERE "
                  " pub.id_publication_issue  = iss.id_publication_issue AND "
                  " iss.id_issue_sections     = sec.id_issue_sections AND "
                  " pub.id_publication        = :id_publication  AND "
                  " pub.id_issue              = :id_issue AND "
                  " iss.id_section            = :id_section AND "
                  " sec.page_name             = :page_name");

    query.bindValue(":id_publication",  id_publication);
    query.bindValue(":id_issue",        id_issue);
    query.bindValue(":id_section",      id_section);
    query.bindValue(":page_name",       page_name);

    if ( !query.exec() )
    {
        qDebug() << "isImageLocked() SQL query Error :" << query.lastError().databaseText().toAscii();
        qDebug("%d\t%d\t%d", id_publication, id_issue, id_section);
        qDebug() << page_name;
        ret = query.exec("UNLOCK TABLES");
        return false;
    }

    int user_id = 0;
    if( query.next() )
        user_id = query.value(0).toInt();

    query.exec("UNLOCK TABLES");


    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;

    qDebug("isImageLocked() Time: %f", diff);

    if(user_id != 0 && user_id != m_user.id() )
    {
        lockedBy = user_id;
        return true;
    }

    return false;
}

bool ClippingStation::lockImage(QString image)
{
    clock_t start = clock();

    unlockImages();

    int id_publication  = m_publication.id();
    int id_issue        = m_publication.currentIssue()->id();
    int id_section      = m_publication.currentIssue()->section(m_currentSectionName).id();
    int id_user         = m_user.id();

    QFileInfo fi(image);
    QString page_name = fi.fileName();

    QSqlQuery query(m_slaveDatabase);
    bool ret = query.exec("LOCK TABLES publication_issue AS pub READ, section_pages AS sec WRITE, issue_sections AS iss READ");
    query.prepare("UPDATE publication_issue as pub, issue_sections as iss, section_pages as sec "
                  " SET locked_by = :id_user "
                  " WHERE "
                  " pub.id_publication_issue  = iss.id_publication_issue AND "
                  " iss.id_issue_sections     = sec.id_issue_sections AND "
                  " pub.id_publication        = :id_publication  AND "
                  " pub.id_issue              = :id_issue AND "
                  " iss.id_section            = :id_section AND "
                  " sec.page_name             = :page_name");

    query.bindValue(":id_publication",  id_publication);
    query.bindValue(":id_issue",        id_issue);
    query.bindValue(":id_section",      id_section);
    query.bindValue(":page_name",       page_name);
    query.bindValue(":id_user",         id_user);

    ret = query.exec();
    query.exec("UNLOCK TABLES");

    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;

    qDebug("lockImage() Time: %f", diff);
    return ret;
}

bool ClippingStation::unlockImages()
{
    clock_t start = clock();
    int id_user         = m_user.id();

    QSqlQuery query(m_slaveDatabase);
    query.exec("LOCK TABLE section_pages as sec WRITE");
    query.prepare("UPDATE section_pages as sec "
                  " SET locked_by =  0 "
                  " WHERE "
                  " sec.locked_by = :id_user ");
    query.bindValue(":id_user",         id_user);

    bool ret = query.exec();
    query.exec("UNLOCK TABLES");

    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;

    qDebug("unlockImages() Time: %f", diff);

    return ret;
}

int ClippingStation::getImageId(const QString name)
{
    QSqlQuery query2(m_slaveDatabase);
    query2.prepare("SELECT section_pages FROM "
                  " publication_issue as pub, issue_sections as iss, section_pages as sec "
                  " WHERE "
                  " pub.id_publication_issue  = iss.id_publication_issue AND "
                  " iss.id_issue_sections     = sec.id_issue_sections AND "
                  " pub.id_publication        = :id_publication  AND "
                  " pub.id_issue              = :id_issue AND "
                  " iss.id_section            = :id_section AND "
                  " sec.page_name             = :page_name");

    query2.bindValue(":id_publication",  m_publication.id());
    query2.bindValue(":id_issue",        m_publication.currentIssue()->id() );
    query2.bindValue(":id_section",      m_publication.currentIssue()->section(m_currentSectionName).id() );
    query2.bindValue(":page_name",       name);

    if ( !query2.exec() )
    {
        qDebug() << "Error in slave db query " << query2.lastQuery().toAscii();
        qDebug() << "Error is " << query2.lastError().text();
        return -1;
    }
    int id = 0;
    while(query2.next() )
        id = query2.value(0).toInt();

    return id;
}

void ClippingStation::loadImageHighlights(int imageId)
{
    clock_t start = clock();

    QSqlQuery query(m_ocrDatabase);
    query.prepare("SELECT  id_company, coordinates, tag FROM "
                  " page_tag_coordinates "
                  " WHERE "
                  " id_section_pages = :id ");

    query.bindValue(":id",  imageId);

    if ( !query.exec() )
    {
        qDebug() << "Error in ocr db query " << query.lastQuery().toAscii();
        qDebug() << "Error is " << query.lastError().text();
        return;
    }

    QList<TagCoordinates> tagH;

    qDebug() << "retreive the results of page_tag_coordiantes";
    while( query.next() )
    {
        int tagId = query.value(0).toInt();
        QStringList coor = query.value(1).toString().split(":");
        QStringList crd = coor[0].split(",");
        int top     = crd[0].toInt();
        int left    = crd[1].toInt();

        int bottom  =crd[2].toInt();
        int right   =crd[3].toInt();

        QString  tag = query.value(2).toString();

        TagCoordinates t(tagId, top, left, bottom, right, tag);

        for(int i=1; i< coor.count(); i++)
        {
            QStringList crd = coor[i].split(",");
            int top     = crd[0].toInt();
            int left    = crd[1].toInt();

            int bottom  =crd[2].toInt();
            int right   =crd[3].toInt();

            //QRectF rec(QPointF(left, top), QPointF(right, bottom) );
            //QPolygonF ply(rec);
            //ui->fullpageView->createTagsHighlightes(ply);

            t.AddCoordinates(top, left, bottom, right);
        }
        tagH.append(t);
    }

    qDebug() << "End get info from page_tag_coordinates" << endl;

    //QTimer::singleShot(100, this, SLOT(addRelatedKeywordsToTag(tagH)));

    //we add the name to the tags
    /*QSqlQuery query2(m_slaveDatabase);
    query2.prepare("SELECT name_company FROM company AS cm, searchword AS sw "
                   " WHERE name_searchword LIKE :name1 AND sw.id_company = cm.id_company AND cm.date_end > NOW() "
                   " UNION SELECT name_company FROM company WHERE name_company LIKE :name2 AND date_end > NOW()");
    for(int i=0; i< tagH.count(); i++)
    {
        qDebug() << "Searching for tag: " << tagH[i].tag();
        query2.bindValue(":name1", tagH[i].tag() );
        query2.bindValue(":name2", tagH[i].tag() );

        if(query2.exec())
        {
            QStringList keywords;
            while(query2.next())
            {
                QString name = query2.value(0).toString();
                qDebug() << "related tag: " << name;
                keywords.append(name);
            }
            qDebug() << "for tag " << tagH[i].tag() << ", the number of related keyword is " << keywords.count();
            tagH[i].setRelatedKeywords(keywords);
        }
    }*/

    ui->fullpageView->addTagCoordinates(tagH);

    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;

    qDebug("loadImageHighlights() Time: %f", diff);
}


void ClippingStation::addRelatedKeywordsToTag(QList<TagCoordinates> &tagH)
{
    qDebug() << "addRelatedKeywordsToTag()";
    QSqlQuery query2(m_slaveDatabase);
    query2.prepare("SELECT name_company FROM company AS cm, searchword AS sw "
                   " WHERE name_searchword LIKE :name1 AND sw.id_company = cm.id_company AND cm.date_end > NOW() "
                   " UNION SELECT name_company FROM company WHERE name_company LIKE :name2 AND date_end > NOW()");
    for(int i=0; i< tagH.count(); i++)
    {
        qDebug() << "Searching for tag: " << tagH[i].tag();
        query2.bindValue(":name1", tagH[i].tag() );
        query2.bindValue(":name2", tagH[i].tag() );

        if(query2.exec())
        {
            QStringList keywords;
            while(query2.next())
            {
                QString name = query2.value(0).toString();
                qDebug() << "related tag: " << name;
                keywords.append(name);
            }
            qDebug() << "for tag " << tagH[i].tag() << ", the number of related keyword is " << keywords.count();
            tagH[i].setRelatedKeywords(keywords);
        }
    }
    ui->fullpageView->addTagCoordinates(tagH);

    qDebug() << "end of addRelatedKeywordsToTag()";
}


QList<WordCoordinates> ClippingStation::loadImageText(QString page, QSqlDatabase &database)
{
    qDebug() << "loadImageText(QString page, QSqlDatabase &database)";

    clock_t start = clock();

    QStringList list = page.split("/");
    QString id_issue    = list[list.count()-6] + list[list.count()-5]+ list[list.count()-4];
    QString page_name   = list[list.count()-1];
    int id_section      = list[list.count()-2].toInt();
    int id_publication  = list[list.count()-3].toInt();

    QSqlQuery query(database);
    query.prepare("SELECT section_pages FROM "
                  " publication_issue as pub, issue_sections as iss, section_pages as sec "
                  " WHERE "
                  " pub.id_publication_issue  = iss.id_publication_issue AND "
                  " iss.id_issue_sections     = sec.id_issue_sections AND "
                  " pub.id_publication        = :id_publication  AND "
                  " pub.id_issue              = :id_issue AND "
                  " iss.id_section            = :id_section AND "
                  " sec.page_name             = :page_name");

    query.bindValue(":id_publication",  id_publication);
    query.bindValue(":id_issue",        id_issue );
    query.bindValue(":id_section",      id_section );
    query.bindValue(":page_name",       page_name);

    int section_pages = 0;

    QList<WordCoordinates> tagW;

    if (!query.exec())
    {
        qDebug() << query.lastError().text().toAscii();
        return tagW;
    }

    if(query.next())
        section_pages = query.value(0).toInt();

    QSqlQuery query2(m_ocrDatabase);
    query2.prepare("SELECT  word, coordinates FROM "
                  " page_word_coordinates"
                  " WHERE "
                  " id_section_pages    = :id "
                  " order by id_page_word_coordinates");

    query2.bindValue(":id", section_pages);


    if ( !query2.exec() )
    {
        qDebug() << query2.lastError().text().toAscii();
        return tagW;
    }

    while( query2.next() )
    {
        QString word = query2.value(0).toString();
        qDebug() << "word is " << word;

        QStringList coor = query2.value(1).toString().split(":");

        QStringList crd = coor[0].split(",");
        int top     = crd[0].toInt();
        int left    = crd[1].toInt();

        int bottom  =crd[2].toInt();
        int right   =crd[3].toInt();

        WordCoordinates t(word, top, left, bottom, right);

        tagW.append(t);
    }

    //ui->fullpageView->addWordCoordinates(tagW);

    double diff = ( clock() - start ) / (double)CLOCKS_PER_SEC;

    qDebug("loadImageText() Time: %f", diff);
    return tagW;
}

void ClippingStation::addTagsToTagWrapper(QList<int> &tagIds, QStringList tagCoord, bool removedAndThenAdd)
{
    QList<Tag> tags = Tag::searchTags(m_slaveDatabase, tagIds);

    for(int i=0; i< tags.count(); i++)
    {
        if(tags[i].id() == tagIds[i])
            tags[i].setCoordinates(tagCoord[i]);
    }

    if(removedAndThenAdd)
        m_tagsWrapper.removedOcrTags(tags);
    else
        m_tagsWrapper.appenedOcrTags(tags);

    m_tagsWrapper.showModel();
}

void ClippingStation::updateSectionPagesStatus()
{

    QAbstractItemModel* model = ui->issuesTreeView->model();
    int issuesCount = model->rowCount( QModelIndex() );

    for(int i=0; i< issuesCount; i++)
    {
        if(m_currentIssueDate.isEmpty())
            return;

        QString issue = model->index(i, 0, QModelIndex() ).data(Qt::DisplayRole).toString();
        if(issue != m_currentIssueDate) //get current issue
            continue;

        QModelIndex issueIndex = model->index(i, 0, QModelIndex() );

        int sectionsCount = model->rowCount(issueIndex);
        for(int j=0; j < sectionsCount; j++)
        {
            //get pages status for this particular section from database.
            //the data is returned as mab (name, status);
            QString section = model->index(j, 0, issueIndex).data(Qt::DisplayRole).toString();
            int sectionId = m_publication.currentIssue()->section(section).id();

            QHash<QString, QIcon> hash = currentSectionPagesStatus(sectionId);

            QModelIndex sectionIndex = model->index(j, 0, issueIndex);

            int pagesCount = model->rowCount(sectionIndex);

            for(int k=0; k< pagesCount; k++)
            {
                QString pageName = model->index(k, 0, sectionIndex).data(Qt::DisplayRole).toString();
                pageName = QString("%1.jpg").arg(pageName.remove(QRegExp("[^\\d]")), 4, QChar('0'));

                QModelIndex pageIndex = model->index(k, 0, sectionIndex);
                if( hash.find(pageName) != hash.end() )
                    model->setData(pageIndex, QVariant(hash[pageName]) , Qt::DecorationRole);
            }
        }
    }
}



QHash<QString, QIcon> ClippingStation::currentSectionPagesStatus(int sectionId)
{
    QHash<QString, QIcon> hash;

    QSqlQuery query(m_masterDatabase);

    query.prepare("SELECT id_issue_sections FROM issue_sections, publication_issue "
            "WHERE publication_issue.id_publication_issue = issue_sections.id_publication_issue AND "
            "id_publication = :id_publication AND issue_date = :issue_date AND id_section = :id_section");
    query.bindValue(":id_publication",  m_publication.id() );
    query.bindValue(":issue_date",      m_publication.currentIssue()->date() );
    query.bindValue(":id_section",      sectionId);

    if(! query.exec() )
        return hash;

    int id_issue_sections = 0;
    if( query.next())
        id_issue_sections = query.value(0).toInt();

    query.exec("LOCK TABLE section_pages READ");

    query.prepare("SELECT page_name, status FROM section_pages "
            "WHERE id_issue_sections = :id_issue_sections ORDER BY page_name");
    query.bindValue(":id_issue_sections",      id_issue_sections);

    if ( !query.exec() )
    {
        query.exec("UNLOCK TABLES");
        return hash;
    }


    while ( query.next() )
    {
        QString filename = query.value(0).toString();
        int status   = query.value(1).toInt();
        QIcon icon;

        if(status < 1)                                      //not processed
            icon.addFile(":/icons/no_icon.png");
        else if (status == 1 )                              //waiting to be processed
            icon.addFile(":/icons/gray_circle.png");
        else if (status == 2 )                              //being procssed
            icon.addFile(":/icons/yellow_circle.png");
        else                                                //finished processing
            icon.addFile(":/icons/green_circle.png");

        hash.insert(filename, icon);
    }

    query.exec("UNLOCK TABLES");

    return hash;
}

void ClippingStation::filterPublications()
{
    if(m_searchPublicationsDlg.exec() == QDialog::Accepted )
    {
        //get the values from the
        QString keyword, startDate, endDate, type, country, ocrSearchword, status;
        bool ischecked;
        m_searchPublicationsDlg.get(keyword, ischecked,  startDate, endDate, type, country, ocrSearchword, status);
        //now search for the publications that meet those criteria
        int countryId = -1;
        if(!country.isEmpty())
             countryId = m_countriesHash[country];

        int typeId = -1;
        if(!type.isEmpty())
        {
            if(type.compare("newspaper", Qt::CaseInsensitive) == 0) //NEWSPAPER 1
                typeId = 1;
            else if(type.compare("magazine", Qt::CaseInsensitive) == 0)
                typeId = 2;
        }

        QStringList list = m_publication.publicationsFiltered(m_slaveDatabase, m_ocrDatabase, keyword, startDate, endDate, typeId, countryId, ocrSearchword, status);
        list.insert(0, "");

        ui->filteredPublicationComboBox->clear();
        ui->filteredPublicationComboBox->addItems(list);
    }
}

void ClippingStation::setShortcuts()
{
    if( !m_clipShortcut.isEmpty() )
        ui->actionClip->setShortcut(m_clipShortcut);

    if( !m_preclipShortcut.isEmpty() )
        ui->actionPreclip->setShortcut(m_preclipShortcut);

    if( !m_addPageShortcut.isEmpty() )
        ui->actionDrawerAddPage->setShortcut(m_addPageShortcut);

    if( !m_nextPageShortcut.isEmpty() )
        ui->actionNextPage->setShortcut(m_nextPageShortcut);

    if( !m_previousPageShortcut.isEmpty() )
        ui->actionPreviousPage->setShortcut(m_previousPageShortcut);
}


void ClippingStation::pauseTracking()
{
    if(m_currentPageId <= 0)
        return;

    if(m_isPaused)
    {
        //resume
        ui->fullpageView->setImageVisibility(true);
        m_trackingTimer.start(1000);
        ui->pauseTrackButton->setText(tr("Pause Tracking"));
        m_isPaused = false;
    }
    else
    {
        ui->fullpageView->setImageVisibility(false);
        m_trackingTimer.stop();
        ui->pauseTrackButton->setText(tr("Resume Tracking"));
        m_isPaused = true;
    }
}

void ClippingStation::updateTrackLabel()
{
    ui->lcdCounter->showTime(++m_trackingSeconds);
    if((m_trackingSeconds % m_maxTimeLimit) == 0)
    {
        if(!m_isPaused)
            pauseTracking();
    }
}

int ClippingStation::publicationIssueId(int id_publication, QString issue_date)
{
    QSqlQuery query(m_slaveDatabase);
    query.prepare("SELECT id_publication_issue FROM publication_issue "
                       "WHERE id_publication = :id_publication AND issue_date = :issue_date");
    query.bindValue(":id_publication", id_publication);
    query.bindValue(":issue_date", issue_date);

    if(!query.exec())
    {
        qDebug() << "select id_publication_issue query error: " << query.lastError().text();
        qDebug() << "returning 0";
        return 0;
    }

    int id_publication_issue = 0;

    if(query.next())
    {
        id_publication_issue = query.value(0).toInt();
        return id_publication_issue;
    }

    return id_publication_issue;
}

int ClippingStation::issueSectionsId(int id_publication_issue, int id_section)
{
    QSqlQuery query(m_slaveDatabase);
    query.prepare("SELECT id_issue_sections FROM issue_sections WHERE "
                       "id_publication_issue = :id_publication_issue AND id_section = :id_section");
    query.bindValue(":id_publication_issue", id_publication_issue);
    query.bindValue(":id_section", id_section);

    if ( !query.exec() )
    {
        qDebug() << "select id_issue_sections query error: " << query.lastError().text();
        qDebug() << "returning 0";
        return 0;
    }

    int id_issue_sections = 0;
    if( query.next() )
        id_issue_sections = query.value(0).toInt();

    return id_issue_sections;
}


QStringList ClippingStation::getRelatedKeywords(QString tag)
{
  QSqlQuery query2(m_slaveDatabase);
  query2.prepare("SELECT name_company FROM company AS cm, searchword AS sw "
                 " WHERE name_searchword LIKE :name1 AND sw.id_company = cm.id_company AND cm.date_end > NOW() "
                 " UNION SELECT name_company FROM company WHERE name_company LIKE :name2 AND date_end > NOW()");

  qDebug() << "Searching for tag: " << tag;
  query2.bindValue(":name1", tag );
  query2.bindValue(":name2", tag );

  QStringList keywords;

  if(query2.exec())
  {
      while(query2.next())
      {
        QString name = query2.value(0).toString();
        qDebug() << "related tag: " << name;
        keywords.append(name);
      }
      qDebug() << "for tag " << tag << ", the number of related keyword is " << keywords.count();
    }

    return keywords;
  }


void ClippingStation::onAppFocusChanged()
{
    if(m_currentPageId <= 0)        //no pages has been opened yet
        return;

    if(QApplication::focusWidget())
    {
        if(m_isPaused)     //if its pause, return without doing anything
            return;

        ui->fullpageView->setImageVisibility(true);
        m_trackingTimer.start(1000);
    }
    else
    {
        ui->fullpageView->setImageVisibility(false);
        m_trackingTimer.stop();
    }
}
