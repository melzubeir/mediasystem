/************************************************************************
 * $Id$
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
 *  $Date$
 *  $Author$
 *  $Revision$
 *  $HeadURL$
 *
 ************************************************************************/

#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QLCDNumber>

class DigitalClock : public QLCDNumber
{
    Q_OBJECT
public:
    explicit DigitalClock(QWidget *parent = 0);
    void showTime(int seconds);
};

#endif // DIGITALCLOCK_H
