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

#include "digitalclock.h"

#include <cstdio>
using namespace std;

DigitalClock::DigitalClock(QWidget *parent) :
    QLCDNumber(parent)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::WindowText, Qt::darkGreen);
    setPalette(palette);

    setSegmentStyle(Filled);
    display("00:00");
}

void DigitalClock::showTime(int seconds)
{
    int min = seconds/60;
    int sec = seconds - (min*60);
    char frmt[10];
    sprintf(frmt, "%02i:%02i", min, sec);
    QString text(frmt);
    display(text);
}
