#include "sqldialog.h"
#include "ui_sqldialog.h"

#include <QtSQL>
#include <QFileDialog>
#include <QTextEncoder>

SQLDialog::SQLDialog(QWidget *parent)
        : QDialog(parent), ui(new Ui::SQLDialog)
{
    //QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates) );

    
    ui->setupUi(this);

    readSettings();
    connectToDB();
}

SQLDialog::~SQLDialog()
{
    delete ui;
}

void SQLDialog::readSettings()
{
    QString config = QApplication::applicationDirPath()+ "/config.ini";
    QSettings settings(config, QSettings::IniFormat);

    m_masterDatabaseHost        = settings.value("master database/server").toString();          //read master database's server value from settings
    m_masterDatabaseName        = settings.value("master database/database").toString();        //read master database's database name value from settings
    m_masterDatabaseUser        = settings.value("master database/uid").toString();             //read master database's user id value from settings
    m_masterDatabasePassword    = settings.value("master database/pwd").toString();             //read master database's password value from settings
    m_masterDatabasePort        = settings.value("master database/port").toInt();               //read master database's port value from settings

}

void SQLDialog::connectToDB()
{
    m_masterDatabase = QSqlDatabase::addDatabase("QMYSQL", "master");

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
    }
}


void SQLDialog::on_pushButton_clicked()
{
    QSqlQuery query(m_masterDatabase);

    QString qtext = QString("INSERT INTO sample_page_text ( pageText ) VALUES (\"%1\")").arg(fileContent);

    if( !query.exec( qtext )  )       //try it also query.exec(qtext);
    {
        QMessageBox::information(this, "Error inserting to database", query.lastError().text());
        qDebug () << qtext;
    }
}

void SQLDialog::on_pushButton_2_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                            ".",
                                            tr("All (*.*)"));

    if(fileName.isNull())
        return;

    //read the file
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    //in.setCodec("UTF-8");

    fileContent = in.readAll();

    file.close();

    qDebug() << fileContent;

    QFile file2(fileName+"_sample");

    if (!file2.open(QIODevice::WriteOnly| QIODevice::Text))
        return;

    QTextStream out(&file2);
    //out.setCodec("UTF-8");

    out << fileContent;
    file2.close();

    fileContent.remove("\"");
    fileContent.remove("'");
    //fileContent.remove("\n");
    ui->pushButton->setEnabled(true);
}

void SQLDialog::on_pushButton_3_clicked()
{
    QSqlQuery query(m_masterDatabase);

    QString qtext = "select name_searchword from searchword where id_company = 1125";

    if( !query.exec( qtext )  )       //try it also query.exec(qtext);
    {
        QMessageBox::information(this, "Error database", query.lastError().text());
        qDebug () << qtext;
        return;
    }

    while(query.next())
    {
        QString str = query.value(0).toString();
        qDebug() << str;
    }


}

void SQLDialog::on_pushButton_4_clicked()
{
    QString searchword = "ÏÈí";
    QSqlQuery query(m_masterDatabase);

    //query.prepare("SELECT cm.id_company, name_company, cm.desc_company, cm.towrite FROM company AS cm, searchword AS sw WHERE name_searchword LIKE :name AND sw.id_company = cm.id_company AND cm.date_end > NOW()"
    //              " UNION SELECT id_company, name_company, desc_company, towrite FROM company WHERE name_company LIKE :name2 AND date_end > NOW()");
    query.prepare("SELECT cm.id_company, name_searchword    FROM company AS cm, searchword AS sw WHERE sw.id_company = cm.id_company AND cm.date_end > NOW()");
    //query.bindValue(":name", searchword);
    //query.bindValue(":name2", searchword);

    if ( !query.exec() )
    {
        qDebug() << "searchTags() SQL Error: " << query.lastError().text().toAscii();
        qDebug() << "Executed Query: " << query.executedQuery().toAscii();
    }

    while(query.next())
    {
        qDebug() << query.value(0).toString();
        qDebug() << query.value(1).toString();
        //qDebug() << query.value(2).toString();
    }
}

void SQLDialog::on_pushButton_5_clicked()
{
    QSqlQuery query(m_masterDatabase);

    query.prepare("SELECT `text` FROM page_text");

    if ( !query.exec() )
    {
        qDebug() << "Executed Query: " << query.executedQuery().toAscii();
    }

    QString str;
    while(query.next())
    {
        str = query.value(0).toString();
        //qDebug() << str;
        break;      //just read the first text for testing.
    }

    QString tag     = "([Dd]ubai|Abu\\s?Dhabi)"; //--> Dubai, dubai, abu dhabi, abudhabi
    //QString pattern = QString("\\b(%1)\\b").arg(tag);     //this can be used if you want to check for whole words only \b for word boundary.

    QRegExp regEx(tag, Qt::CaseInsensitive);
    int count = 0;
    int pos   = 0;
    while( (pos = regEx.indexIn(str, pos) ) != -1)
    {
        QString foundTag = str.mid(pos, regEx.matchedLength() );
        pos += regEx.matchedLength();
        qDebug() << foundTag;

    }
}
