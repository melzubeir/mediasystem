/************************************************************************
 * $Id: pagemargin.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/pagemargin.h $
 *
 ************************************************************************/

#ifndef PAGEMARGIN_H
#define PAGEMARGIN_H

#include <QtCore>
#include <QGraphicsRectItem>

#include "globalfunctions.h"

class PageMargin : public QGraphicsRectItem
{
public:
    //! \fn PageMargin()
    //!     default constructor
    //! \param[in] is this the first page's margin
    //! \author elzubeir
    PageMargin(bool isfirst, QGraphicsRectItem* parent = 0);

    //! \fn ~PageMargin()
    //!     destructor
    //! \author elzubeir
    ~PageMargin();

    //! \fn setPageParent()
    //!     set the pages graphics item parent
    //! \author elzubeir
    void setPageParent(QGraphicsRectItem* parent);

    //! \fn top()
    //!     return the top value of the margin
    //! \author elzubeir
    qreal top();

    //! \fn bottom()
    //!     return the bottom value of the margin
    //! \author elzubeir
    qreal bottom();

    //! \fn right()
    //!     return the right value of the margin
    //! \author elzubeir
    qreal right();

    //! \fn left()
    //!     return the left value of the margin
    //! \author elzubeir
    qreal left();

    //! \fn hideMargins()
    //!     hide the margins
    //! \param[in] hide or show
    //! \author elzubeir
    void hideMargins(bool hide = true);

private:

    //! graphics line edit item for top margin
    QGraphicsLineItem m_topMargin;

    //! graphics line edit item for bottom margin
    QGraphicsLineItem m_bottomMargin;

    //! graphics line edit item for left margin
    QGraphicsLineItem m_leftMargin;

    //! graphics line edit item for right margin
    QGraphicsLineItem m_rightMargin;

    //! qreal top, bottom, left, and right vlaues of the margin
    qreal m_top, m_bottom, m_left, m_right;
};

#endif // PAGEMARGIN_H
