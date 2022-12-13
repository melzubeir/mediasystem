/*
 * This sample code is a part of the IRIS iDRS library.
 * Copyright (C) 2006-2009 Image Recognition Integrated Systems
 * All rights reserved.
 *
 * This source code is merely intended as a supplement to the iDRS reference and the electronic documentation provided with the library.
 *
 */
#ifndef _idrs_config_h_
#define _idrs_config_h_

#ifndef __FUNCTION__
#define __FUNCTION__ __FILE__ /* If it is not supported by the compiler, uses the file name */
#endif /* __FUNCTION__ */

#ifdef WIN32
#if _MSC_VER < 1300
#include <windows.h>
#include <fstream.h>
#else /* _MSC_VER < 1300 */
#if _MSC_VER >= 1400
#endif
#include <iostream>
#include <fstream>
using namespace std;
#endif /* _MSC_VER < 1300 */
#include "io.h"
#else /* WIN32 */
#if defined(macintosh) || (defined(__APPLE__) && defined(__MACH__))
#include <iostream>
#include <fstream>
using namespace std;
#else /* defined(macintosh) || (defined(__APPLE__) && defined(__MACH__)) */
#include <iostream>
#include <fstream>
using namespace std;
#endif /* defined(macintosh) || (defined(__APPLE__) && defined(__MACH__)) */
#endif /* WIN32 */


#include <idrs.h>
using namespace IDRS;


/**
 * A structure containing the information needed to setup iDRS in the sample application.
 */
typedef struct
{
  IDRS_LICENSE_TYPE ltLicenseType;
  IDRS_BOOL bLoadDRS;
  IDRS_LICENSE_DRS aLicenseDRS;
  IDRS_BOOL bLoadBCode;
  IDRS_LICENSE_BCODE aLicenseBCode;
  IDRS_BOOL bLoadPrepro;
  IDRS_LICENSE_PREPRO aLicensePrepro;
  IDRS_BOOL bLoadFmt;
  IDRS_LICENSE_FMT aLicenseFmt;
  IDRS_BOOL bLoadImageFile;
  IDRS_LICENSE_IMAGE_FILE aLicenseImageFile;
  IDRS_BOOL bLoadLeadtools;
  IDRS_LICENSE_LEADTOOLS aLicenseLeadtools;
  IDRS_BOOL bLoadFingerprint;
  IDRS_LICENSE_FINGERPRINT aLicenseFingerprint;
  IDRS_BOOL bLoadSnowbound;
  IDRS_LICENSE_SNOWBOUND aLicenseSnowbound;
} IDRS_FILE_SETUP_INFO;

extern "C"
{
  int SetupIDRS ( IDRS_FILE_SETUP_INFO & theFileSetupInfo );
}

extern ofstream out;

#endif /* _idrs_config_h_ */
