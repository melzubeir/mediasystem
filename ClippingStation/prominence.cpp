/************************************************************************
 * $Id: prominence.cpp 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/prominence.cpp $
 *
 ************************************************************************/

#include "prominence.h"

Prominence::Prominence()
{
    m_mentionScore  = 1;
    m_visualScore   = 0;
    m_toneScore     = 1;
    m_note          = "";
    m_size          = 0.0;
}
