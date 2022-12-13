//! \file importerdialog.cpp
//! \brief the implementation of the class
#include <QtGui>
#include <QtGlobal> //Q_WS_WIN

#include "importerdialog.h"
#include "imageoutputformatdialog.h"
#include "section.h"
#include "importerthread.h"
#include "constants.h"
#include "globalfunctions.h"
#include "publication.h"

#include "ui_importerdialog.h"

ImporterDialog::ImporterDialog(QWidget *parent, QSqlDatabase &masterDb, QSqlDatabase &slaveDb, User &user)
        : QDialog(parent), ui(new Ui::ImporterDialog), m_masterDatabase(masterDb), m_slaveDatabase(slaveDb), m_user(user)
        //, m_section(m_masterDatabase, m_slaveDatabase)
{
    ui->setupUi(this);

    //hid the progressbar
    ui->totalProgressBar->setVisible(false);
    ui->totalUploadLabel->setVisible(false);
    ui->totalUploadLabel_2->setVisible(false);

    ui->fileUploadLabel->setVisible(false);
    ui->fileProgressBar->setVisible(false);
    ui->fileUploadLabel_1->setVisible(false);


    ui->publicationDate->setDate(QDate::currentDate()); //set the current date to today!
    ui->issueNumberLineEdit->setText(QDate::currentDate().toString(QString("yyyyMMdd")));   //set the form for the issue number to current date

    QStringList list;
    QSqlQuery query("SELECT id_publication, name_publication_en, skip_ocr FROM publication", m_slaveDatabase);
    while( query.next() )
    {
        int     id_publication  = query.value(0).toInt();
        QString publicationName = query.value(1).toString();
        bool    skipOcr         = query.value(2).toBool();
        list << publicationName;        //populate list with values from the database

        m_publicationHash[publicationName] = id_publication;     //also populate the hash table with name/id values
        m_publicationSkipOcrHash[publicationName] = skipOcr;     //populate the hash table with name/skipocr values
    }

    QCompleter* completer = new QCompleter(list, this);     //createa completer with the list of publication names
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->publicationNameLineEdit->setCompleter(completer);   //use the complete with the publicationLineEdit


    //get section entries from database;
    //list = m_section.sections();
    list = Section::sectionNames(m_slaveDatabase);
    ui->sectionComboBox->addItems(list);


    //get issues path from database, and set the m_issuesPath with that value
    query.exec("SELECT issues_path, issues_windows_drive_letter FROM system_configuration");
    while (query.next())
    {
        m_issuesPath = query.value(0).toString();
        m_issuesWindowDriveLetter = query.value(1).toString();
    }

    if( !m_issuesPath.startsWith("/") )
        m_issuesPath = "/" + m_imagesPath;

    if( !m_issuesPath.endsWith("/") )
        m_issuesPath += "/";

    //read ftp access
    m_thread = new ImporterThread(this);
//    connect(m_thread, SIGNAL(terminated()), this, SLOT(threadTerminated()) );

    m_timer = new QTimer(); //create a timer and connect it's timeout signal to the update slot
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    connect(m_thread, SIGNAL(importerThreadCompleted()), this, SLOT(importerThreadCompleted()));
}

ImporterDialog::~ImporterDialog()
{
    m_masterDatabase.close();
    m_slaveDatabase.close();
    delete ui;
}

void ImporterDialog::keyPressEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_Escape)//ignore the escape key pressed
        return;

    QDialog::keyPressEvent(key);
}

//private slots
void ImporterDialog::import()
{
    //make sure that the publication line edit is not empty
    QString publication_name = ui->publicationNameLineEdit->text().trimmed();
    if (publication_name.isEmpty() )
    {
        QMessageBox::warning(this, tr("Empty publication"), tr("Please select a publication\nEmpty publication names is not allowed"));
        ui->publicationNameLineEdit->setFocus();
        return;
    }

    //get the id of the publicaiton to be used when creating the structure in the file server
    int id_publication = m_publicationHash[publication_name];
    bool skipOcr       = m_publicationSkipOcrHash[publication_name];

    if(id_publication == 0)
    {
        QMessageBox::warning(this, tr("Error in publication ID"), tr("This publication has an id = 0, please check with your system administrator to fix this problem"));
        return;
    }

    //get the id_section from the article_sections table
    int id_section = Section::sectionId(m_slaveDatabase, ui->sectionComboBox->currentText());
    if( id_section == 0)
        return;

    //get date from publication date line edit, its in the ISODate format, i.e. yyy-MM-dd
    QString issue_date = ui->publicationDate->text();

    int id_issue = ui->issueNumberLineEdit->text().toInt();
    if( id_issue == 0 )
    {
        ui->issueNumberLineEdit->setText(QDate::currentDate().toString("yyyMMdd"));
        id_issue = QDate::currentDate().toString("yyyMMdd").toInt();   //if empty, set it up to use the current date as issuenumber
    }

    //get starting page number
    int startPagenumber = ui->startNumberlineEdit->text().toInt();
    if ( startPagenumber <= 0 )
        startPagenumber = 1;       //if its empty set the default value to 1

    bool keepPagenumber = ui->keepPageNumberCheckBox->isChecked();

    //call the ImageOutputFormatDialog dialog to show the user how the naming will look like
    ImageOutputFormatDialog dlg(this, m_srcFileNames, keepPagenumber, startPagenumber);

    if (dlg.exec() == QDialog::Rejected)        //if the naming is not acceptable, exit the function without performing the import
        return;

    setEnabled(false);

    m_dstFileNames = dlg.outputFileNames;
    //disable import button
    ui->importButton->setEnabled(false);

    if ( !m_imagesPath.endsWith("/") )
        m_imagesPath += "/";


    m_useFtp = false;
    readSettings();

    //check if this is update operation or not
    int id_publication_issue = publicationIssueId(id_publication, id_issue, issue_date);
    int id_issue_sections;
    //bool publication_issue_exist = publicationIssueExist(id_publication, id_issue, issue_date, id_publication_issue);
    //bool issue_section_exist = false;

    QStringList conflictList;

    if( id_publication_issue > 0 )
    {
        //check if there are any conflicted files between src and dst
        id_issue_sections = issueSectionsId(id_publication_issue, id_section);
        if ( id_issue_sections > 0)
        {
            bool conflict = conflictedFiles(id_issue_sections, id_section, m_dstFileNames, conflictList);
            if(conflict)
            {
                /*
                if ( !canUserReplaceFiles() )
                {
                    QMessageBox::critical(this, tr("Not sufficient permissions"), tr("You dont have necessary permission to replace files, pleaese contact your admin"));
                    //ui->importButton->setEnabled(true);
                    //return;
                }
                else
                {
                    //we should show the user a warning message!
                }
                */

                switch( QMessageBox::question(
                        this,
                        tr("conflicted files"),
                        tr("Files with the same name already exist in the destination directory, this operation will overwrite them\nAre you sure?"),
                        QMessageBox::Yes |
                        QMessageBox::No,
                        QMessageBox::No) )
                {
                case QMessageBox::Yes:
                    break;
                case QMessageBox::No:
                    setEnabled(true);
                    return;
                default:
                    setEnabled(true);
                    return;
                }

                //delete files
                if(!m_useFtp)
                {
                    QString dstPath = destinationPath(id_publication, id_section);
                    deleteFiles(dstPath, conflictList);
                }
                //now we remove those values from database
                deleteFilesFromDatabase(id_issue_sections, conflictList );

            }
        }
    }

    QString destPath = destinationPath(id_publication, id_section);

    m_totalFilesCompleted = 0;

    if ( m_useFtp )
    {
        //it may contains windows drive letter, so we remove it, also remove the beginning and the end /
        destPath = removeWindowsDriveLetter(destPath);

        //ftp format will be: ftp://user:pass.ALLCONTENT.com/testdir/1-level/afile1
        destPath = QString("ftp://%1:%2@%3/").arg(m_username).arg(m_password).arg(m_address) +  destPath;

        ui->fileProgressBar->setRange(0, 100);
        ui->fileProgressBar->setVisible(true);
        ui->fileUploadLabel->setVisible(true);

    }

    ui->totalProgressBar->setRange(0, m_srcFileNames.count() );
    ui->totalProgressBar->setVisible(true);
    ui->totalUploadLabel->setVisible(true);

    m_thread->set(m_imagesPath, m_srcFileNames, destPath, m_dstFileNames, skipOcr);
    m_thread->start();
    m_timer->start(1000);
}

void ImporterDialog::addNewSection()
{
    //get the name of the section from the lineEdit
    QString newSectionName = ui->newSectionNameLineEdit->text().trimmed();
    if (newSectionName.isEmpty())       //if its empty, do nothing
        return;

    //check if the new name already exists in the drop-down list
    int i = ui->sectionComboBox->findText(newSectionName,Qt::MatchFixedString);
    if (i != -1)
    {
        QMessageBox::information(this, tr("Duplicate!"), tr("Section '%1' already exists in the list").arg(newSectionName));
        ui->newSectionNameLineEdit->setFocus();
        return;
    }

    if( !Section::addNewSection(m_masterDatabase, newSectionName, m_user.id()) )
        return;

    //add it to the end of the sectionComboBox
    ui->sectionComboBox->addItem(newSectionName);

    QMessageBox::information(this, tr("Added!"), tr("Section '%1' was added successfully").arg(newSectionName));
}

void ImporterDialog::browseScannedImages()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select directory"), QDir::currentPath());   //get directory location from user

    if( !directory.isEmpty() )
    {
        QDir::setCurrent(directory);
        readScannedImagesNames(directory);      //read images from the directory, and populate the imagelist
        ui->scannedImagePathLineEdit->setText(directory);   //set the path in the path lineEdit
    }

}

void ImporterDialog::scannedImageLineEditTextChanged()
{
    QString directory = ui->scannedImagePathLineEdit->text();
    QDir::setCurrent(directory);
    readScannedImagesNames(directory);      //read images from the directory, and populate the imagelist
}

void ImporterDialog::readScannedImagesNames(QString path)
{
    QDir directory(path);               //open the directory in path location
    QString fileName = "*.jpg";         //only filter jpg images
    QStringList filter = QStringList(fileName);

    m_imagesPath = path;
    m_srcFileNames.clear();
    m_srcFileNames = directory.entryList(filter, QDir::Files | QDir::NoSymLinks, QDir::Name);     //read files from directory, and save them in imagelist
    m_srcFileNames.sort();

    ui->filesTreeWidget->clear();

    QTreeWidgetItem *dirItem = new QTreeWidgetItem(QStringList(path));
    ui->filesTreeWidget->insertTopLevelItem(0, dirItem);        //create a top level item in the tree widget

    if(m_srcFileNames.count() > 0)       //if there are images, insert them into the treelist
    {
        for(int i=0; i < m_srcFileNames.size(); i++)
        {
            QTreeWidgetItem *filenameItem = new QTreeWidgetItem(QStringList(m_srcFileNames[i]) );
            ui->filesTreeWidget->itemAt(0,0)->addChild(filenameItem);
        }

        ui->importButton->setEnabled(true); //enable the import button
    }
    else                //if the directory does not contain any .jpg files, then tell the user that, and disable import button!
    {
        QTreeWidgetItem *filenameItem = new QTreeWidgetItem(QStringList(tr("No jpg files in this directory!") ) );
        ui->filesTreeWidget->itemAt(0,0)->addChild(filenameItem);
        ui->importButton->setEnabled(false);
    }

    ui->filesTreeWidget->expandAll();
}

bool ImporterDialog::readSettings()
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

int ImporterDialog::publicationIssueId(int id_publication, int id_issue, QString issue_date)
{
    qDebug() << "publicationIssueId()";
    QSqlQuery query(m_slaveDatabase);
    query.prepare("SELECT id_publication_issue FROM publication_issue "
                       "WHERE id_publication = :id_publication AND issue_date = :issue_date");
    query.bindValue(":id_publication", id_publication);
    query.bindValue(":issue_date", issue_date);

    qDebug() << "id_publication_issue query: " << query.lastQuery();
    qDebug() << "id_publication: " << id_publication;
    qDebug() << "issue_date: " << issue_date;

    if(!query.exec())
    {
        qDebug() << "select id_publication_issue query error: " << query.lastError().text();
        qDebug() << "returning 0";
        return 0;
    }

    int id_publication_issue = 0;

    if(!query.isValid())
        qDebug() << "The query is not valid";

    if(query.next())
    {
        id_publication_issue = query.value(0).toInt();
        qDebug() << "id_publication_issue : " << id_publication_issue;
        return id_publication_issue;
    }

    qDebug() << "return " << id_publication_issue;
    qDebug() << "end of publicationIssueId()";

    return id_publication_issue;
}

int ImporterDialog::issueSectionsId(int id_publication_issue, int id_section)
{
    qDebug() << "issueSectionsId()";

    QSqlQuery query(m_slaveDatabase);
    query.prepare("SELECT id_issue_sections FROM issue_sections WHERE "
                       "id_publication_issue = :id_publication_issue AND id_section = :id_section");
    query.bindValue(":id_publication_issue", id_publication_issue);
    query.bindValue(":id_section", id_section);

    QString str = QString("SELECT id_issue_sections FROM issue_sections WHERE "
                       "id_publication_issue = %1 AND id_section = %2")
            .arg(id_publication_issue)
            .arg(id_section);

    qDebug() << "id_issue_sections query: " << str;

    if ( !query.exec() )
    {
        qDebug() << "select id_issue_sections query error: " << query.lastError().text();
        qDebug() << "returning 0";
        return 0;
    }

    int id_issue_sections = 0;
    if( query.next() )
        id_issue_sections = query.value(0).toInt();

    qDebug() << "return " << id_publication_issue;
    qDebug() << "end of issueSectionsId()";

    return id_issue_sections;
}

bool ImporterDialog::conflictedFiles(int id_issue_sections, int id_section, QStringList outputfiles, QStringList &conflictList)
{
    QSqlQuery query(m_slaveDatabase);

    query.exec("LOCK TABLE section_pages READ");

    query.prepare("SELECT page_name FROM section_pages WHERE id_issue_sections = :id_issue_sections");
    query.bindValue(":id_issue_sections" , id_issue_sections);

    if( !query.exec() )
    {
        query.exec("UNLOCK TABLES");
        return false;
    }

    conflictList.clear();

    QStringList dbfiles;
    while( query.next() )
    {
        dbfiles.append(query.value(0).toString());
    }

    query.exec("UNLOCK TABLES");

    //check if there are any conflicting files in the names, if so, check the permission
    bool conflict = false;
    foreach (QString str, dbfiles)
    {
        if(outputfiles.contains(str, Qt::CaseInsensitive))
        {
            conflictList.append(str);
            conflict = true;;
        }
    }

    return conflict;
}

bool ImporterDialog::canUserReplaceFiles()
{
    return (m_user.userRole() == ADMIN_ROLE);
}

QString ImporterDialog::destinationPath(int id_publication, int id_section)
{
    QString day     = QString::number( ui->publicationDate->date().day  ()  );
    QString month   = QString::number( ui->publicationDate->date().month()  );
    QString year    = QString::number( ui->publicationDate->date().year ()  );

    QString dst = "";

    //if we are under Windows, then we have a Windows drive letter, like C:/ or D: or I:\, so we add this to the issues path
#ifdef Q_WS_WIN
    dst += m_issuesWindowDriveLetter;
    if ( dst.endsWith("/") || dst.endsWith("\\") )
        dst= dst.left(dst.length()-1);
#endif

    // fileserver/path/Issues/YYYY/MM/DD/ID_PUBLICATION/ID_SECTION/
    dst += m_issuesPath + QString("%1/%2/%3/%4/%5/").
           arg(year , 4, QChar('0')).
           arg(month, 2, QChar('0')).
           arg(day  , 2, QChar('0')).
           arg(id_publication).
           arg(id_section);

    return dst;
}

QString ImporterDialog::removeWindowsDriveLetter(QString destination)
{
    QString dst = destination.mid(destination.indexOf(":")+1, destination.length());
    dst = dst.left( dst.length() -1 );      //to remove the last /

    if ( !dst.endsWith("/") )
        dst += "/";

    return dst;
}

QString ImporterDialog::get96DPIImagePath(QString filename)
{
    QImage originalImage(filename);
    QSize size(originalImage.width()/3, originalImage.height()/3);
    QImage image(size, QImage::Format_RGB888);

    image = originalImage.scaledToWidth(originalImage.width()/3, Qt::SmoothTransformation);
    //return image;

    QString filename96  = filename.replace(".jpg", "");
    filename96 += "_96dpi.jpg";

    if( image.save(filename96, "jpg", JPG_SAVE_QUALITY) )
        return filename96;
    else
    {
        QMessageBox::critical(this, tr("Error!"), tr("Error creating 96 dpi image %1").arg(filename96));
        return "";
    }
}

void ImporterDialog::importerThreadCompleted()
{
    m_timer->stop();        //stop the time,
    update();               //and call the update one last time to reflect the last changes

    if(m_totalFilesCompleted < m_srcFileNames.count())
    {
        QString str;
        if(m_useFtp)
            str = tr("Error happened while importing images to FTP server, only %1/%2 files were imported successfully, Do you want to CANCEL the import process?").arg(m_totalFilesCompleted).arg(m_srcFileNames.count());
        else
            str = tr("Error happened while importing images to File server, only %1/%2 files were imported successfully, Do you want to CANCEL the import process?").arg(m_totalFilesCompleted).arg(m_srcFileNames.count());

        switch( QMessageBox::question(
                this,
                tr("Error in importing files"),
                str,
                QMessageBox::Yes |
                QMessageBox::No,
                QMessageBox::Yes) )
        {
        case QMessageBox::Yes:
            {
                setEnabled(true);
                ui->importButton->setEnabled(true);
                return;
            }
        case QMessageBox::No:
            break;
        default:
            {
                setEnabled(true);
                ui->importButton->setEnabled(true);
                return;
            }
        }
    }

    updateDatabase();
}

void ImporterDialog::updateDatabase()
{
    qDebug() << "updateDatabase()";

    QString publication_name = ui->publicationNameLineEdit->text().trimmed();
    int id_publication       = m_publicationHash[publication_name];
    bool skipOcr             = m_publicationSkipOcrHash[publication_name];
    int id_section           = Section::sectionId(m_slaveDatabase, ui->sectionComboBox->currentText());
    QString issue_date       = ui->publicationDate->text();
    int id_issue             = ui->issueNumberLineEdit->text().toInt();


    QSqlQuery queryMaster(m_masterDatabase);

    int id_publication_issue = publicationIssueId(id_publication, id_issue, issue_date);
    qDebug() << "publication issue id: " << id_publication_issue;

    if( id_publication_issue == 0 )
    {
        //if not an update process, add the entry to publication_issue table, set the publication_status to 1
        queryMaster.prepare("INSERT INTO publication_issue (id_publication, id_issue, publication_status, issue_date, pages_number, created_by) "
                            "VALUES( :id_publication, :id_issue, '1', :issue_date, :pagesNumber, :created_by)" );

        queryMaster.bindValue(":id_publication" , id_publication);
        queryMaster.bindValue(":id_issue"       , id_issue);
        queryMaster.bindValue(":issue_date"     , issue_date);
        queryMaster.bindValue(":pagesNumber"    , m_srcFileNames.count() );
        queryMaster.bindValue(":created_by"     , m_user.id());

        qDebug() << "insert publication_issue entry by user: " << m_user.username() << ", id = : " << m_user.id();

        QString str = QString("INSERT INTO publication_issue (id_publication, id_issue, publication_status, issue_date, pages_number, created_by) "
                            "VALUES( %1, %2, '1', %3, %4, %5)")
                .arg(id_publication)
                .arg(id_issue)
                .arg(issue_date)
                .arg(m_srcFileNames.count())
                .arg(m_user.id());
        qDebug() << "publication_issue insert statement: " << str;

        if( !queryMaster.exec())
        {
            QMessageBox::critical(this, tr("SQL Error"), queryMaster.lastError().databaseText());
            ui->importButton->setEnabled(true);
            setEnabled(true);
            return;
        }

        id_publication_issue = queryMaster.lastInsertId().toInt();
    }
    else
    {
        queryMaster.prepare("Update publication_issue set created_by = :created_by where id_publication_issue = :id");
        queryMaster.bindValue(":created_by", m_user.id());
        queryMaster.bindValue(":id", id_publication_issue);
        if( !queryMaster.exec() )
        {
            qDebug() << "update publication issue: " << queryMaster.lastQuery().toAscii();
            qDebug() << "user id: " << m_user.id();
        }

        qDebug() << "update publication_issue entry by user: " << m_user.username() << ", id = : " << m_user.id();
        QString str = QString("Update publication_issue set created_by = %1 where id_publication_issue = %2 ")
                .arg(m_user.id())
                .arg(id_publication_issue);
        qDebug() << "publication_issue update statement: " << str;
    }

    int id_issue_sections = issueSectionsId(id_publication_issue, id_section);
    qDebug() << "issue sections id: " << id_issue_sections;
    if( id_issue_sections == 0 )
    {
        queryMaster.prepare("INSERT INTO issue_sections (id_publication_issue, id_section, pages_reset, page_start, page_end, pages_number) "
                            "VALUES (:id_publication_issue, :id_section, :pagesRest, :pageStart, :pageEnd, :pages_number)");

        queryMaster.bindValue(":id_publication_issue"   , id_publication_issue);
        queryMaster.bindValue(":id_section"             , id_section);
        queryMaster.bindValue(":pagesRest"              , 1);
        queryMaster.bindValue(":pageStart"              , m_dstFileNames[0]);
        queryMaster.bindValue(":pageEnd"                , m_dstFileNames[m_dstFileNames.count()-1]);
        queryMaster.bindValue(":pages_number"           , m_dstFileNames.count() );


        QString str = QString("INSERT INTO issue_sections (id_publication_issue, id_section, pages_reset, page_start, page_end, pages_number) "
                              "VALUES (%1, %2, %3, %4, %5, %6)")
                .arg(id_publication_issue)
                .arg(id_section)
                .arg(1)
                .arg(m_dstFileNames[0])
                .arg(m_dstFileNames[m_dstFileNames.count()-1])
                .arg(m_dstFileNames.count());
        qDebug() << "issue_sections insert statement: " << str;

        if( !queryMaster.exec())
        {
            QMessageBox::critical(this, tr("SQL Error"), queryMaster.lastError().databaseText());
            qDebug() << "issue_sections insert query error: " << queryMaster.lastQuery().toAscii();
            ui->importButton->setEnabled(true);
            setEnabled(true);
            return;
        }

        id_issue_sections = queryMaster.lastInsertId().toInt();
    }

    //first lock the database to make sure no one is using it
    queryMaster.exec("LOCK TABLE section_pages WRITE");

    int status = 1;
    if (skipOcr)
        status = 0;

    //insert pages to database, and set its request for the ocr to start processing it now
    queryMaster.prepare("INSERT INTO section_pages (id_issue_sections, page_name, status) "
                        "VALUES (:id_issue_sections, :page_name, :status)");

    queryMaster.bindValue(":id_issue_sections", id_issue_sections);
    queryMaster.bindValue(":status", status);

    foreach (QString page_name, m_dstFileNames)
    {
        queryMaster.bindValue(":page_name", page_name);
        if( !queryMaster.exec() )
        {
            qDebug() << "insert section_pages Error: " << queryMaster.lastError();
        }
        QString str = QString("INSERT INTO section_pages (id_issue_sections, page_name, status) "
                              "VALUES (%1, %2, %3)")
                .arg(id_issue_sections)
                .arg(page_name)
                .arg(status);
        qDebug() << "section_pages insert statement: " << str;
    }

    //unlock pages
    queryMaster.exec("UNLOCK TABLES");

    //update the pages_number in publication_issue table
    queryMaster.prepare("UPDATE publication_issue SET pages_number = "
                        "(SELECT count(*) FROM section_pages s, issue_sections iss where s.id_issue_sections = iss.id_issue_sections AND "
                        " iss.id_publication_issue = :id1) WHERE id_publication_issue = :id2;");
    queryMaster.bindValue(":id1", id_publication_issue);
    queryMaster.bindValue(":id2", id_publication_issue);

    if(!queryMaster.exec())
    {
        qDebug() << "Error: " << queryMaster.lastError();
    }

//*/
    QMessageBox::information(this, tr("Finished"), tr("Importing %1/%2 images was done successfully!").arg(m_totalFilesCompleted).arg(m_srcFileNames.count()));

    ui->scannedImagePathLineEdit->clear();
    ui->filesTreeWidget->clear();

    ui->totalProgressBar->setVisible(false);
    ui->totalUploadLabel->setVisible(false);
    ui->totalUploadLabel_2->setVisible(false);

    ui->fileUploadLabel->setVisible(false);
    ui->fileProgressBar->setVisible(false);
    ui->fileUploadLabel_1->setVisible(false);

    setEnabled(true);

    qDebug() << "end of updateDatabase()";

}

void ImporterDialog::update()
{
    ui->fileUploadLabel->setText(QString("%1/%2 KB").arg(g_currentUpload/1024).arg(m_currentFileSize/1024) );     //update the labels, and the
    ui->fileProgressBar->setValue(g_currentUpload*100/m_currentFileSize);                                   //progress bars to represent
    ui->totalProgressBar->setValue(m_totalFilesCompleted);                                                        //values from the ftp thread
    ui->totalUploadLabel->setText(QString("%1/%2 File(s)").arg( m_totalFilesCompleted ).arg( m_srcFileNames.count() ) );        //operation
    //qDebug("ImporterDialog::update() Total files completed %d", m_totalFilesCompleted+1);
}

void ImporterDialog::deleteFiles(QString path, QStringList filenames)
{
    for(int i=0; i< filenames.count(); i++)
    {
        if(!QFile::remove(path + filenames[i]))
            qDebug() << "Could not remove image: " << path + filenames[i];

        if(!QFile::remove(path + HIGH_RES_IMAGE_DIR + filenames[i]))
          qDebug() << "Could not remove image: " << path + HIGH_RES_IMAGE_DIR + filenames[i];
    }
}

void ImporterDialog::setEnabled(bool enable)
{
    ui->publicationNameLineEdit->   setEnabled(enable);
    ui->publicationDate->           setEnabled(enable);
    ui->issueNumberLineEdit->       setEnabled(enable);
    ui->sectionComboBox->           setEnabled(enable);
    ui->newSectionCheckBox->        setEnabled(enable);
    ui->scannedImagePathLineEdit->  setEnabled(enable);
    ui->browseButton->              setEnabled(enable);
    ui->startNumberlineEdit->       setEnabled(enable);
    ui->keepPageNumberCheckBox->    setEnabled(enable);
}


void ImporterDialog::deleteFilesFromDatabase(int idSection, QStringList filenames )
{
    //qDebug() << "deleteFilesFromDatabase()";
    QSqlQuery query(m_slaveDatabase);

    //query.exec("LOCK TABLES section_pages page_text page_word_coordinates page_tag_coordinates WRITE");
    query.exec("LOCK TABLES section_pages page_text page_tag_coordinates WRITE");

    foreach (QString filename, filenames)
    {
        /*query.prepare("DELETE FROM  section_pages, page_text, page_word_coordinates, page_tag_coordinates "
                      "USING section_pages, page_text, page_word_coordinates, page_tag_coordinates "
                      "WHERE "
                      " section_pages.id_issue_sections = :id_issue_sections "
                      " AND section_pages.page_name like :page_name "
                      " AND section_pages.section_pages = page_text.id_section_pages "
                      " AND section_pages.section_pages = page_word_coordinates.id_section_pages "
                      " AND section_pages.section_pages = page_tag_coordinates.id_section_pages;");*/

        /*query.prepare("DELETE FROM  section_pages, page_text, page_tag_coordinates "
                      "USING section_pages, page_text, page_tag_coordinates "
                      "WHERE "
                      " section_pages.id_issue_sections = :id_issue_sections "
                      " AND section_pages.page_name like :page_name "
                      " AND section_pages.section_pages = page_text.id_section_pages "
                      " AND section_pages.section_pages = page_tag_coordinates.id_section_pages;");

        query.bindValue(":id_issue_sections" , idSection);
        query.bindValue(":page_name", filename);
        bool ret = query.exec();*/

        //now delete the pages if they dont have any entries in the ocr tables.
        query.prepare("DELETE FROM section_pages "
                      "WHERE "
                      " section_pages.id_issue_sections = :id_issue_sections "
                      " AND section_pages.page_name like :page_name");

        query.bindValue(":id_issue_sections" , idSection);
        query.bindValue(":page_name", filename);
        query.exec();
    }

    query.exec("UNLOCK TABLES");
}
