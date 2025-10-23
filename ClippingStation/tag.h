/************************************************************************
 * $Id: tag.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/tag.h $
 *
 ************************************************************************/

#ifndef TAG_H
#define TAG_H

#include <QString>
#include <QList>

#include "constants.h"
#include "prominence.h"

class Tag
{
public:

    //! \fn set
    //!     default constructor
    //! \author elzubeir
    Tag();

    //! \fn Tag
    //!     constructor
    //! \param[in] id of the tag
    //! \param[in] name of the tag
    //! \param[in] araic tag
    //! \param[in] comment of the tag
    //! \param[in] is the tag from ocr
    //! \param[in] is the tag selected
    //! \author elzubeir
    Tag(int id, QString tag, QString arabicTag, int towrite, QString comment = "", bool fromOCR = false, bool selected = false);

    //! \fn operator ==()
    //!     comparison operator
    //! \author elzubeir
    bool operator == (const Tag& right) const;

    //! \fn id()
    //! \return string holding the tag id
    //! \author elzubeir
    int id();

    //! \fn tag()
    //! \return string holding the tag name
    //! \author elzubeir
    QString tag();

    //! \fn arabicTag()
    //! \return string holding the arabic tag
    //! \author elzubeir
    QString arabicTag();

    //! \fn comment()
    //! \return string holding the tag comment
    //! \author elzubeir
    QString comment();

    //! \fn selected()
    //! \return bool is the tag is selected
    //! \author elzubeir
    bool selected();

    //! \fn fromOCR()
    //! \return bool is the tag from ocr
    //! \author elzubeir
    bool fromOCR();

    //! \fn towrite()
    //! \return int to write value
    //! \author elzubeir
    int towrite();

    //! \fn prominence()
    //! \return the prominence reference
    //! \author elzubeir
    Prominence& prominence();    //QVector< QVector<int> > aves();

    //! \fn coordinates()
    //! \return string holding the tag's coordinates
    //! \author elzubeir
    QString coordinates();

    //! \fn set
    //!     sets the tag values
    //! \param[in] id of the tag
    //! \param[in] name of the tag
    //! \param[in] araic tag
    //! \param[in] comment of the tag
    //! \author elzubeir
    void set(int id, QString tag, QString arabicTag, QString comment = "");

    //! \fn setSelected(
    //!     sets if tag is selected
    //! \param[in] bool is the tag selected
    //! \author elzubeir
    void setSelected(bool selected);

    //! \fn setId()
    //!     sets the tag id
    //! \param[in] id of the tag
    //! \author elzubeir
    void setId(int id);

    //! \fn setTag()
    //!     sets the tag name
    //! \param[in] name of the tag
    //! \author elzubeir
    void setTag(QString tag);

    //! \fn setArabicTag()
    //!     sets the tag arabic name
    //! \param[in] tags arabic name
    //! \author elzubeir
    void setArabicTag(QString tag);

    //! \fn setComment()
    //!     set the tags comment
    //! \param[in] the comment
    //! \author elzubeir
    void setComment(QString comment);

    //! \fn setTowrite()
    //!     sets the to write value
    //! \param[in] value of to write
    //! \author elzubeir
    void setTowrite(int write);

    //! \fn setProminence()
    //!     set the tags prominence value
    //! \param[in] prominence value
    //! \author elzubeir
    void setProminence(Prominence pr); //void setAves(QVector< QVector<int> > aves);

    //! \fn setFromOCR()
    //!     is this tag came from the ocr
    //! \param[in] has it come from ocr
    //! \author elzubeir
    void setFromOCR(bool fromOCR);

    //! \fn toggel()
    //!     toggel the tags selection (selected/unselected)
    //! \author elzubeir
    void toggel();

    //! \fn setCoordinates()
    //!     set the tags coordinates, if this tag is from the ocr
    //! \param[in] coordinates of the tag
    //! \author elzubeir
    void setCoordinates(QString coord);

public:

    //! \fn searchTags()
    //!     search for tags from database
    //! \param[in] database object
    //! \param[in] search word to look for
    //! \return a list of tags
    //! \author elzubeir
    static QList<Tag> searchTags(QSqlDatabase &database, QString &searchword);

    //! \fn searchTags()
    //!     search for tags from database
    //! \param[in] database object
    //! \param[in] list of tag ids to search for
    //! \return a list of tags
    //! \author elzubeir
    static QList<Tag> searchTags(QSqlDatabase &database, QList<int> &tagIds);

    //! \fn loadArticleTags()
    //!     load the tags article
    //! \param[in] database object
    //! \param[in] article id
    //! \return a list of tags for the article
    //! \author elzubeir
    static QList<Tag> loadArticleTags(QSqlDatabase &database, int articleId);

    //! \fn setArabicTag()
    //!     set tags for the particular article
    //! \param[in] database object
    //! \param[in] id of the article to add tags to
    //! \param[in] list of tags to add to the article
    //! \return a list of tags
    //! \author elzubeir
    static void       setArticleTags(QSqlDatabase &database, int articleId, QList<Tag> &tags);

    //! \fn headlineEnglish()
    //!     construct an english headline from tags
    //! \param[in] list of tags
    //! \return a string of tags joined by /
    //! \author elzubeir
    static QString    headlineEnglish(QList<Tag> & list);

    //! \fn loadTagProminence()
    //!     loads the tags prominence values
    //! \param[in] database object
    //! \param[ou] list of tags to set their prominence
    //! \param[in] article id
    //! \author elzubeir
    static void       loadTagProminence(QSqlDatabase& database, QList<Tag>& tags, int articleId);


    static QString loadSearchwords(QSqlDatabase& database, QString keyword);

private:

    //! tag id
    int m_id;

    //! tag name
    QString m_tag;

    //!  aratic tag name
    QString m_arabicTag;

    //! tag comments
    QString m_comment;

    //! is tag selected
    bool m_selected;

    //! is tag from ocr
    bool m_fromOCR;

    //! to write value
    int m_towrite;

    //! tags coordinates
    QString m_coordinates;

    //! tags prominence
    Prominence m_prominence;
};

#endif // TAG_H
