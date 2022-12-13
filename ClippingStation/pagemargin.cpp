/************************************************************************
 * $Id: pagemargin.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/pagemargin.cpp $
 *
 ************************************************************************/

#include "pagemargin.h"

#include <QPen>
#include <QDesktopWidget>
#include <QApplication>
#include <QGraphicsLineItem>

#include "constants.h"
#include "systemconfiguration.h"

PageMargin::PageMargin(bool isfirst,QGraphicsRectItem * parent)
       :     QGraphicsRectItem(parent)
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

    m_top       = mv.top    * dpi / 2.54;
    m_bottom    = mv.bottom * dpi / 2.54;
    m_left      = mv.left   * dpi / 2.54;
    m_right     = mv.right  * dpi / 2.54;

    qreal w = pageWidth  - (m_left + m_right);
    qreal h = pageHeight - (m_top + m_bottom);

    QColor color = Qt::gray;
    QPen pen(Qt::DashDotLine);
    pen.setColor(color);
    //pen.setWidth(1);          //DONT uncomment this

    m_topMargin     .setLine(m_left  , m_top  , m_left+w, m_top   );
    m_bottomMargin  .setLine(m_left  , m_top+h, m_left+w, m_top+h );
    m_leftMargin    .setLine(m_left  , m_top  , m_left  , m_top+h );
    m_rightMargin   .setLine(m_left+w, m_top  , m_left+w, m_top+h );


    m_topMargin     .setPen(pen);
    m_bottomMargin  .setPen(pen);
    m_leftMargin    .setPen(pen);
    m_rightMargin   .setPen(pen);
}


PageMargin::~PageMargin()
{
}


void PageMargin::setPageParent(QGraphicsRectItem* parent)
{
    this           ->setParentItem(parent);
    m_topMargin     .setParentItem(parent);
    m_bottomMargin  .setParentItem(parent);
    m_leftMargin    .setParentItem(parent);
    m_rightMargin   .setParentItem(parent);
}


void PageMargin::hideMargins(bool hide)
{
    if(hide)
    {
        m_topMargin     .hide();
        m_bottomMargin  .hide();
        m_leftMargin    .hide();
        m_rightMargin   .hide();
    }
    else
    {
        m_topMargin     .show();
        m_bottomMargin  .show();
        m_leftMargin    .show();
        m_rightMargin   .show();
    }
}

qreal PageMargin::top()
{
    return m_top;
}

qreal PageMargin::bottom()
{
    return m_bottom;
}

qreal PageMargin::left()
{
    return m_left;
}

qreal PageMargin::right()
{
    return m_right;
}
