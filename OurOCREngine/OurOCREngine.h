
#ifndef _OUROCRENGINE_H_
#define _OUROCRENGINE_H_

#include <idrs.h>

#include <string>
#include <vector>

#include <QtSql/QtSql>
#include <QtCore/QMutex>

#include "Word.h"
#include "Tag.h"


using std::string;
using std::vector;

//using namespace mysqlpp;


struct PageInfo
{
	int m_sectionPagesId;
	QString m_remotePath;
	QString m_localPath;
	bool m_isArabic;
	bool m_isCopied;
	bool m_isProcessed;
};

struct TagInfo
{
	int m_id;
	QString m_name;
};


class OurOCREngine
{
public:
	OurOCREngine();
	~OurOCREngine();
	bool Initalize();

	friend void Start(OurOCREngine &engine);
	//void Pause();
	void Stop();

private:
	bool ReadSettings();
	bool ConnectToDatabase();
	void GetIssuePath();
	void ConstructPagesPath();

	void Process();
	bool ProcessPage(QString &pagePath, bool isArabic);

	void GetPagesToProcess();
	void ResetUnprocessedPages();


	QList<Tag> SearchTags(bool isArabic);
	QStringList GetTagCoordinates(QString tag, bool ismultiword);

	void DeleteFromDatabase(int id_section_pages);
	void AddToDatabase(int id_section_pages, QList<Tag> &tags);

	void ConvertToSingleBox(QString &str);

	void RemoveDashAndTheFollowingSpace(QString &text);

	void GetTagsFromDatabase(bool isArabic);

	QString CurrentDateTime();

	void CopyFiles();
	void DeleteFiles();

	void ShowException ( IDRS::IDRSException & theIDRSException );

private:
	IDRS::CReader m_reader;

	QList<PageInfo>	m_pagesToProcess;
	QList<Word>		m_words;
	QList<TagInfo>	m_databaseTags;

	bool m_isReady;

	QSqlDatabase m_database;

	//current page text
	QString m_pageText;

	//issues path
	QString m_issuePath;
	//database connection
	QString m_databaseName;
	QString m_server;
	QString m_user;
	QString m_password;
	int m_port;

	int m_currentIndex;
};

#endif //_OUROCRENGINE_H_

