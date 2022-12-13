/************************************************************************
 * $Id: constants.h 661 2011-04-05 13:16:53Z elzubeir $
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
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/ClippingStation/constants.h $
 *
 ************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

// version
#define VERSION_NUMBER 0.5

// zoom factor
#define ZOOM_FACTOR 0.25
// zoom limits
#define MIN_SCALE 0.0625
#define MAX_SCALE 16.0

// values are in inch
#define A4_WIDTH 8.27
#define A4_HEIGHT 11.69
#define DPI 96.0

#define FULLPAGE_MIN_WIDTH 8.27
#define FULLPAGE_MIN_HEIGHT  11.69

enum PAGE_LAYOUT
{
    ACTUAL_SIZE = 0,
    WIDTH_FIT,
    BEST_FIT
};

enum SELECTION_AREA_TYPE
{
    IMAGE = 0,
    TITLE,
    BODY
};

enum SELECTION_MODE
{
    SELECT = 0,
    VIEW,
    ORDER
};

enum RUNNING_MODE
{
    NORMAL_MODE         = 0,
    PRECLIPPING_MODE,
    CLIPPING_MODE
};

enum FILTER_MODE
{
    NO_FILTER           =0,
    PRECLIPSONLY_FILTER
};

#define AVEDIALOG_ROW_COUNT 4
#define AVEDIALOG_COLUMN_COUNT 4

#define NEWSPAPER 1
#define MAGAZINE 2

#define NEWSPAPER_COLUMN_WIDTH 5.0

#define THUMBNAIL_WIDTH 70

#define JPG_SAVE_QUALITY 85

#define HIGH_RES_IMAGE_DIR "300dpi/"

//update the page status every 7.5 seconds
#define PAGE_STATUS_TIMERS 7.5*1000

#endif // CONSTANTS_H
