/*
* This sample code is a part of the IRIS iDRS library.
* Copyright (C) 2006-2009 Image Recognition Integrated Systems
* All rights reserved.
*
* This source code is merely intended as a supplement to the iDRS reference and the electronic documentation provided with the library.
*
*/
#include "idrs_config.h"
#include <iostream>
using namespace std;

/**
*
*/
int ReadFileSetupInfo ( const char * strFileName, IDRS_FILE_SETUP_INFO & theFileSetupInfo )
{
	FILE * hFile;
	size_t sz;
	char c;

	hFile = fopen ( strFileName, "rb" );
	if ( hFile == NULL )
	{
		return -1;
	}

	sz = fread ( & theFileSetupInfo, sizeof ( IDRS_FILE_SETUP_INFO ), 1, hFile );
	fread ( & c, sizeof ( char ), 1, hFile );  

	if (( sz == 1 ) && ( feof ( hFile ) != 0 ))
	{
		fclose ( hFile );
		return 0;
	}
	else
	{
		fclose ( hFile );
		memset ( & theFileSetupInfo, 0, sizeof ( IDRS_FILE_SETUP_INFO ));
		return -2;
	}
}

/**
*
*/
int WriteFileSetupInfo ( const char * strFileName, const IDRS_FILE_SETUP_INFO & theFileSetupInfo )
{
	FILE * hFile;

	hFile = fopen ( strFileName, "wb" );
	if ( hFile == NULL )
	{
		return -1;
	}

	fwrite ( & theFileSetupInfo, sizeof ( IDRS_FILE_SETUP_INFO ), 1, hFile );
	fclose ( hFile );
	return 0;
}

/**
*
*/
int CallSetup ( IDRS_FILE_SETUP_INFO & theFileSetupInfo )
{
	IDRS_CHAR strModuleName [ IDRS_MAX_PATH ];

	try
	{
		if ( theFileSetupInfo.bLoadDRS )
		{
			sprintf ( strModuleName, "IDRS_MODULE_DRS" );
			cout << "Loading " << strModuleName << endl;
			CIDRS::SetupModule ( IDRS_MODULE_DRS, & theFileSetupInfo.aLicenseDRS, sizeof ( IDRS_LICENSE_DRS ));
		}

		if ( theFileSetupInfo.bLoadPrepro )
		{
			sprintf ( strModuleName, "IDRS_MODULE_PREPRO" );
			cout << "Loading " << strModuleName << endl;
			CIDRS::SetupModule ( IDRS_MODULE_PREPRO, & theFileSetupInfo.aLicensePrepro, sizeof ( IDRS_LICENSE_PREPRO ));
		}

		if ( theFileSetupInfo.bLoadImageFile )
		{
			sprintf ( strModuleName, "IDRS_MODULE_IMAGE_FILE" );
			cout << "Loading " << strModuleName << endl;
			CIDRS::SetupModule ( IDRS_MODULE_IMAGE_FILE, & theFileSetupInfo.aLicenseImageFile, sizeof ( IDRS_LICENSE_IMAGE_FILE ));
		}
	}
	catch ( IDRSException & aIDRSException )
	{
		cout << "\tAn error occured in the iDRS." << endl;
		cout << "\tCode " << aIDRSException.m_code << endl;
		cout << "\tFile " << aIDRSException.m_strSrcFile << endl;
		cout << "\tLine " << aIDRSException.m_uiSrcLine << endl;

		cout << "\tERROR - Setting up "<< strModuleName << " failed.";
		switch ( aIDRSException.m_code )
		{
		case IDRS_ERROR_DRS_INVALID_KEY:
		case IDRS_ERROR_BARCODE_INVALID_KEY:
		case IDRS_ERROR_PREPRO_INVALID_KEY:
		case IDRS_ERROR_FMT_INVALID_KEY:
		case IDRS_ERROR_IMAGE_FILE_INVALID_KEY:
			cout << " Please check your software keys." << endl;
			break;

		default:
			cout << " Please check your settings and the license'type." << endl;
			break;
		}
		CIDRS::Unload ();
		return -1;
	}

	return 0;
}

/**
*
*/
int SetupIDRS ( IDRS_FILE_SETUP_INFO & theFileSetupInfo )
{
	IDRS_CHAR
		c, str [ IDRS_MAX_PATH ], strExtensionName [ IDRS_MAX_PATH ];
	IDRS_BOOL bSetupFromSoftwareKeysFile;
	IDRS_UINT uiExtensionId;
	int iRet;
#ifdef IDRS_OS_WIN32
	IDRS_CHAR strDrive [ IDRS_MAX_PATH ], strDir [ IDRS_MAX_PATH ], strCurrentDir [ IDRS_MAX_PATH ];
	IDRS_CHAR strSoftwareKeysFile [] = ".//idrs_software_keys_14.inf";

	//Temporary change the current directory so strSoftwareKeysFile relative path is computed the expected way
	GetModuleFileNameA ( ::GetModuleHandle ( NULL ), str, IDRS_MAX_PATH );
	_splitpath_s ( str, strDrive, IDRS_MAX_PATH, strDir, IDRS_MAX_PATH, 0, 0, 0, 0 );
	_makepath_s ( str, IDRS_MAX_PATH, strDrive, strDir, 0, 0);

	GetCurrentDirectoryA ( IDRS_MAX_PATH, strCurrentDir );
	SetCurrentDirectoryA ( str );
#else /* IDRS_OS_WIN32 */
	IDRS_CHAR strSoftwareKeysFile [] = "idrs_software_keys14.inf";
#endif /* IDRS_OS_WIN32 */

	/*
	cout << "The SetupIDRS function illustrates how to setup the iDRS." << endl;
	cout << "Its full source code is available in the idrs_config.cpp file of the samples." << endl;
	cout << endl;
	cout << "iDRS is composed of many different modules, modular to your project requirements." << endl;
	cout << "During this step of the sample application, you will be asked all the information needed to setup the iDRS:" << endl;
	cout << "- your license type" << endl;
	cout << "- the modules and the extensions you want to load" << endl;
	cout << "- the corresponding software keys";
	cout << endl;
	cout << "At the end of the application, if SetupIDRS succeeds, the application will propose you to save the settings to the " << strSoftwareKeysFile;
	cout << " file. So the next time you run a sample you won't have to re-enter these information." << endl;
	cout << endl;
	*/

	//*
	bSetupFromSoftwareKeysFile = IDRS_FALSE;

	if ( ReadFileSetupInfo ( strSoftwareKeysFile, theFileSetupInfo ) == 0 )
	{
		/*
		cout << "SetupIDRS has detected an existing " << strSoftwareKeysFile << " file.";
		cout << "Do you want to setup iDRS with the information contained in this file (y/n)?" << endl;
		do
		{
		cin >> c;
		cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');*/
		c = 'y';

		bSetupFromSoftwareKeysFile = ( c == 'y' || c == 'Y' );
	}
	//*/
	if ( ! bSetupFromSoftwareKeysFile )
	{
		memset ( & theFileSetupInfo, 0, sizeof ( IDRS_FILE_SETUP_INFO ));

		/*
		* Getting the information from the user
		*/
		cout << "License type selection" << endl;
		cout << "2 - IDRS_LICENSE_CUSTOM_SOFTWARE - Custom software" << endl;
		cout << "4 - IDRS_LICENSE_SOFTWARE - Software key protection" << endl;
		cout << "Please enter your iDRS license type:" << endl;
		do
		{
			cin >> str;
			cout << endl;
		} while(strcmp(str, "2") != 0 && strcmp(str, "4") != 0);
		cout << endl;
		theFileSetupInfo.ltLicenseType = ( IDRS_LICENSE_TYPE ) atoi ( str );

		switch ( theFileSetupInfo.ltLicenseType )
		{
		case IDRS_LICENSE_CUSTOM_SOFTWARE:
		case IDRS_LICENSE_SOFTWARE:
			cout << "SetupIDRS will ask you for your software keys." << endl;
			break;

		default:
			cout << "\tERROR - Unknown license type." << endl;
			return -1;
		}

		/*
		* IDRS_MODULE_DRS
		*/

		cout << "IDRS_MODULE_DRS" << endl;

		cout << "IDRS_MODULE_DRS is the ocr/icr engine." << endl;
		cout << "Do you have a license for the IDRS_MODULE_DRS module (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadDRS = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadDRS )
		{
			cout << "Getting information for the IDRS_MODULE_DRS module." << endl;

			memset ( & theFileSetupInfo.aLicenseDRS, 0, sizeof ( IDRS_LICENSE_DRS ));
			theFileSetupInfo.aLicenseDRS.ltLicenseType = theFileSetupInfo.ltLicenseType;

			cout << "Please enter IDRS_MODULE_DRS software key:" << endl;

			cin.ignore ();

			cin.getline ( theFileSetupInfo.aLicenseDRS.strDRSSoftwareKey, IDRS_LICENSE_MAX_SIZE );
			cout << endl;

			for ( uiExtensionId = 0; uiExtensionId < IDRS_DRS_EXTENSION_COUNT; uiExtensionId ++ )
			{
				switch ( uiExtensionId )
				{
				case IDRS_DRS_EXTENSION_ASIAN:
					cout << "IDRS_DRS_EXTENSION_ASIAN extends the OCR languages with for 4 additional languages:" << endl;
					cout << "- Traditional Chinese" << endl;
					cout << "- Simplified Chinese" << endl;
					cout << "- Japanese" << endl;
					cout << "- Korean" << endl;
					sprintf ( strExtensionName, "IDRS_DRS_EXTENSION_ASIAN" );
					break;

				case IDRS_DRS_EXTENSION_HEBREW:
					cout << "IDRS_DRS_EXTENSION_HEBREW extends the OCR languages with the Hebrew language." << endl;
					sprintf ( strExtensionName, "IDRS_DRS_EXTENSION_HEBREW" );
					break;

#ifndef IDRS_OS_MACINTOSH
				case IDRS_DRS_EXTENSION_BANKING_FONTS:
					cout << "IDRS_DRS_EXTENSION_BANKING_FONTS allows to recognize the following banking fonts:" << endl;
					cout << "- OCR-A" << endl;
					cout << "- OCR-B" << endl;
					cout << "- E13B" << endl;
					cout << "- CMC-7" << endl;
					sprintf ( strExtensionName, "IDRS_DRS_EXTENSION_BANKING_FONTS" );
					break;
#endif /* ifndef IDRS_OS_MACINTOSH */

				case IDRS_DRS_EXTENSION_ICR:
					cout << "IDRS_DRS_EXTENSION_ICR is the standard ICR extension." << endl;
					cout << "The IRIS Intelligent Character Recognition (ICR) engine allows you to recognize all handwritten characters based on the Latin";
					cout << " alphabet." << endl;
					sprintf ( strExtensionName, "IDRS_DRS_EXTENSION_ICR" );
					break;

#ifndef IDRS_OS_MACINTOSH
				case IDRS_DRS_EXTENSION_ARABIC:
					cout << "IDRS_DRS_EXTENSION_ARABIC extends the OCR languages with the Arabic and Farsi languages." << endl;
					sprintf ( strExtensionName, "IDRS_DRS_EXTENSION_ARABIC" );
					break;
#endif /* ifndef IDRS_OS_MACINTOSH */

				default:
					strExtensionName [ 0 ] = 0;
					break;
				}

				if ( strExtensionName [ 0 ] == 0 )
				{
					theFileSetupInfo.aLicenseDRS.xbExtensionFlags [ uiExtensionId ] = IDRS_FALSE;
				}
				else
				{
					cout << "Do you have a license for the " << strExtensionName << " extension (y/n)?" << endl;
					do
					{
						cin >> c;
						cout << endl;
					} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
					theFileSetupInfo.aLicenseDRS.xbExtensionFlags [ uiExtensionId ] = ( c == 'y' || c == 'Y' );
				}

				if ( theFileSetupInfo.aLicenseDRS.xbExtensionFlags [ uiExtensionId ] )
				{
					cout << "Please enter " << strExtensionName << " software key:" << endl;
					cin.ignore (); 
					cin.getline ( theFileSetupInfo.aLicenseDRS.xstrExtensionSoftwareKey [ uiExtensionId ], IDRS_LICENSE_MAX_SIZE );
					cout << endl;
				}

				cout << endl;
			}

#ifdef IDRS_OS_WIN32
			do
			{
				cout << "Please enter the path to the OCR resource directory (the directory containing the .ytr files)" << endl;
				cout << "(Just enter '#' for C:\\Program Files\\I.R.I.S. SA\\iDRS\\resources)" << endl;
				fflush ( stdin );
				gets ( theFileSetupInfo.aLicenseDRS.strOCRResourcesPath );

				if ( strcmp ( theFileSetupInfo.aLicenseDRS.strOCRResourcesPath, "#" ) == 0 )
				{
					sprintf ( theFileSetupInfo.aLicenseDRS.strOCRResourcesPath, "C:\\Program Files\\I.R.I.S. SA\\iDRS\\resources" );
				}

				if ( strlen ( theFileSetupInfo.aLicenseDRS.strOCRResourcesPath ))
				{
					sprintf ( str, "%s\\eng.ytr", theFileSetupInfo.aLicenseDRS.strOCRResourcesPath );
				}
				else
				{
					sprintf ( str, "eng.ytr" );
				}

				if ( _access ( str, 0 ) != 0 )
				{
					cout << "\tWARNING - The english lexicon file " << str << " does not exist." << endl;
					cout << "Maybe you made a mistake when typing the path to the OCR resource directory." << endl;
					cout << "Double check the path to the OCR resource directory." << endl;
					cout << "Do you want to continue anyway or do you want to re-enter the path to the OCR resource directory (c/r)?" << endl;
					do
					{
						cin >> c;
						cout << endl;
					} while(c != 'c' && c != 'C' && c != 'r' && c != 'R');
				}
				else
				{
					c = 'c';
				}
			}
			while ( c != 'c' && c != 'C' );
#else /* IDRS_OS_WIN32 */
			theFileSetupInfo.aLicenseDRS.strOCRResourcesPath [ 0 ] = 0;
#endif /* IDRS_OS_WIN32 */
		}

#ifdef IDRS_OS_WIN32
		/*
		* IDRS_MODULE_BCODE
		*/

		cout << "IDRS_MODULE_BCODE" << endl;

		cout << "IDRS_MODULE_BCODE is the bar code recognition engine." << endl;
		cout << "The bar code module will identify barcodes located anywhere on the page." << endl;
		cout << "The barcodes can be used as separators and their content can be used for renaming files for example." << endl;
		cout << "Do you have a license for the IDRS_MODULE_BCODE module (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadBCode = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadBCode )
		{
			cout << "Getting information for the IDRS_MODULE_BCODE module." << endl;

			memset ( & theFileSetupInfo.aLicenseBCode, 0, sizeof ( IDRS_LICENSE_BCODE ));
			theFileSetupInfo.aLicenseBCode.ltLicenseType = theFileSetupInfo.ltLicenseType;

			cout << "Please enter IDRS_MODULE_BCODE software key:" << endl;
			cin.ignore (); 
			cin.getline ( theFileSetupInfo.aLicenseBCode.strBCodeSoftwareKey, IDRS_LICENSE_MAX_SIZE );
			cout << endl;

			for ( uiExtensionId = 0; uiExtensionId < IDRS_BCODE_EXTENSION_COUNT; uiExtensionId ++ )
			{
				switch ( uiExtensionId )
				{
				case IDRS_BCODE_EXTENSION_PDF417:
					cout << "The IDRS_BCODE_EXTENSION_PDF417 extension offers the recognition of the PDF 417 barcodes." << endl;
					sprintf ( strExtensionName, "IDRS_BCODE_EXTENSION_PDF417" );
					break;

				case IDRS_BCODE_EXTENSION_QRCODE:
					cout << "The IDRS_BCODE_EXTENSION_QRCODE extension offers the recognition of the QR barcodes." << endl;
					sprintf ( strExtensionName, "IDRS_BCODE_EXTENSION_QRCODE" );
					break;

				case IDRS_BCODE_EXTENSION_ENGINE_EVO_I:
					cout << "The IDRS_BCODE_EXTENSION_ENGINE_EVO_I extension offers the possibility of using an addditional barcode" << endl;
					cout << "engine, for better performances." << endl;
					sprintf ( strExtensionName, "IDRS_BCODE_EXTENSION_ENGINE_EVO_I" );
					break;

				case IDRS_BCODE_EXTENSION_DATAMATRIX:
					cout << "The IDRS_BCODE_EXTENSION_DATAMATRIX extension offers the recognition of the DataMatrix barcodes." << endl;
					sprintf ( strExtensionName, "IDRS_BCODE_EXTENSION_DATAMATRIX" );
					break;

				default:
					strExtensionName [ 0 ] = 0;
					break;
				}

				if ( strExtensionName [ 0 ] == 0 )
				{
					theFileSetupInfo.aLicenseBCode.xbExtensionFlags [ uiExtensionId ] = IDRS_FALSE;
				}
				else
				{
					cout << "Do you have a license for the " << strExtensionName << " extension (y/n)?" << endl;
					do
					{
						cin >> c;
						cout << endl;
					} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
					theFileSetupInfo.aLicenseBCode.xbExtensionFlags [ uiExtensionId ] = ( c == 'y' || c == 'Y' );
				}

				if ( theFileSetupInfo.aLicenseBCode.xbExtensionFlags [ uiExtensionId ] )
				{
					cout << "Please enter " << strExtensionName << " software key:" << endl;
					cin.ignore (); 
					cin.getline ( theFileSetupInfo.aLicenseBCode.xstrExtensionSoftwareKey [ uiExtensionId ], IDRS_LICENSE_MAX_SIZE );
					cout << endl;
				}

				cout << endl;
			}
		}
#endif /* IDRS_OS_WIN32 */


		/*
		* IDRS_MODULE_PREPRO
		*/

		cout << "IDRS_MODULE_PREPRO" << endl;

		cout << "IDRS_MODULE_PREPRO is the standard image pre-processing module." << endl;
		cout << "Before doing the OCR, the images may need to be optimized for the OCR process. This is done by different pre-processing functions." << endl;
		cout << "The IDRS_MODULE_PREPRO module allows you to clean-up the images for optimized OCR such as deskew, despeckle, binarize, etc." << endl;
		cout << "Do you have a license for the IDRS_MODULE_PREPRO module (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadPrepro = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadPrepro )
		{
			cout << "Getting information for the IDRS_MODULE_PREPRO module." << endl;

			memset ( & theFileSetupInfo.aLicensePrepro, 0, sizeof ( IDRS_LICENSE_PREPRO ));
			theFileSetupInfo.aLicensePrepro.ltLicenseType = theFileSetupInfo.ltLicenseType;

			cout << "Please enter IDRS_MODULE_PREPRO software key:" << endl;
			cin.ignore (); 
			cin.getline ( theFileSetupInfo.aLicensePrepro.strPreproSoftwareKey, IDRS_LICENSE_MAX_SIZE );
			cout << endl;

			for ( uiExtensionId = 0; uiExtensionId < IDRS_PREPRO_EXTENSION_COUNT; uiExtensionId ++ )
			{
				switch ( uiExtensionId )
				{
#ifndef IDRS_OS_MACINTOSH
		case IDRS_PREPRO_EXTENSION_ADVANCED_PREPRO:
			cout << "IDRS_PREPRO_EXTENSION_ADVANCED_PREPRO is the advanced pre-processing extension." << endl;
			cout << "This extension allows to do black border removal, line removal, advanced despeckling, color conversion, etc." << endl;
			sprintf ( strExtensionName, "IDRS_PREPRO_EXTENSION_ADVANCED_PREPRO" );
			break;
#endif /* ifndef IDRS_OS_MACINTOSH */

		default:
			strExtensionName [ 0 ] = 0;
			break;
				}

				if ( strExtensionName [ 0 ] == 0 )
				{
					theFileSetupInfo.aLicensePrepro.xbExtensionFlags [ uiExtensionId ] = IDRS_FALSE;
				}
				else
				{
					cout << "Do you have a license for the " << strExtensionName << " extension (y/n)?" << endl;
					do
					{
						cin >> c;
						cout << endl;
					} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
					theFileSetupInfo.aLicensePrepro.xbExtensionFlags [ uiExtensionId ] = ( c == 'y' || c == 'Y' );
				}

				if ( theFileSetupInfo.aLicensePrepro.xbExtensionFlags [ uiExtensionId ] )
				{
					cout << "Please enter " << strExtensionName << " software key:" << endl;
					cin.ignore (); 
					cin.getline ( theFileSetupInfo.aLicensePrepro.xstrExtensionSoftwareKey [ uiExtensionId ], IDRS_LICENSE_MAX_SIZE );
					cout << endl;
				}

				cout << endl;
			}
		}

#ifdef IDRS_OS_WIN32
		/*
		* IDRS_MODULE_FMT
		*/

		cout << "IDRS_MODULE_FMT" << endl;

		cout << "IDRS_MODULE_FMT is the formatting module." << endl;
		cout << "Once the recognition of the document has been done, you might still need to generate an output file." << endl;
		cout << "This is done by the formatting module." << endl;
		cout << "Do you have a license for the IDRS_MODULE_FMT module (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadFmt = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadFmt )
		{
			cout << "Getting information for the IDRS_MODULE_FMT module." << endl;

			memset ( & theFileSetupInfo.aLicenseFmt, 0, sizeof ( IDRS_LICENSE_FMT ));
			theFileSetupInfo.aLicenseFmt.ltLicenseType = theFileSetupInfo.ltLicenseType;

			cout << "Please enter IDRS_MODULE_FMT software key:" << endl;
			cin.ignore (); 
			cin.getline ( theFileSetupInfo.aLicenseFmt.strFmtSoftwareKey, IDRS_LICENSE_MAX_SIZE );
			cout << endl;

			for ( uiExtensionId = 0; uiExtensionId < IDRS_FMT_EXTENSION_COUNT; uiExtensionId ++ )
			{
				switch ( uiExtensionId )
				{
				case IDRS_FMT_IHQC_EXTENSION:
					cout << "IDRS_FMT_IHQC_EXTENSION is the intelligent High Quality compression extension." << endl;
					sprintf ( strExtensionName, "IDRS_FMT_IHQC_EXTENSION" );
					break;

				default:
					strExtensionName [ 0 ] = 0;
					break;
				}

				if ( strExtensionName [ 0 ] == 0 )
				{
					theFileSetupInfo.aLicenseFmt.xbExtensionFlags [ uiExtensionId ] = IDRS_FALSE;
				}
				else
				{
					cout << "Do you have a license for the " << strExtensionName << " extension (y/n)?" << endl;
					do
					{
						cin >> c;
						cout << endl;
					} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
					theFileSetupInfo.aLicenseFmt.xbExtensionFlags [ uiExtensionId ] = ( c == 'y' || c == 'Y' );
				}

				if ( theFileSetupInfo.aLicenseFmt.xbExtensionFlags [ uiExtensionId ] )
				{
					cout << "Please enter " << strExtensionName << " software key:" << endl;
					cin.ignore (); 
					cin.getline ( theFileSetupInfo.aLicenseFmt.xstrExtensionSoftwareKey [ uiExtensionId ], IDRS_LICENSE_MAX_SIZE );
					cout << endl;
				}

				cout << endl;
			}
		}
#endif /* IDRS_OS_WIN32 */

		/*
		* IDRS_MODULE_IMAGE_FILE
		*/

		cout << "IDRS_MODULE_IMAGE_FILE" << endl;

		cout << "IDRS_MODULE_IMAGE_FILE is the iDRS imaging module." << endl;
		cout << "This module allows you to open the most known input formats including TIFF G4 and BMP." << endl;
		cout << "Do you have a license for the IDRS_MODULE_IMAGE_FILE module (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadImageFile = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadImageFile )
		{
			cout << "Getting information for the IDRS_MODULE_IMAGE_FILE module." << endl;

			memset ( & theFileSetupInfo.aLicenseImageFile, 0, sizeof ( IDRS_LICENSE_IMAGE_FILE ));
			theFileSetupInfo.aLicenseImageFile.ltLicenseType = theFileSetupInfo.ltLicenseType;

			cout << "Please enter IDRS_MODULE_IMAGE_FILE software key:" << endl;
			cin.ignore (); 
			cin.getline ( theFileSetupInfo.aLicenseImageFile.strImageFileSoftwareKey, IDRS_LICENSE_MAX_SIZE );
			cout << endl;

			for ( uiExtensionId = 0; uiExtensionId < IDRS_IMAGE_FILE_EXTENSION_COUNT; uiExtensionId ++ )
			{
				switch ( uiExtensionId )
				{
				case IDRS_FILE_EXTENSION_JPEG:
					cout << "IDRS_FILE_EXTENSION_JPEG is the JPEG extension." << endl;
					sprintf ( strExtensionName, "IDRS_FILE_EXTENSION_JPEG" );
					break;

				default:
					strExtensionName [ 0 ] = 0;
					break;
				}

				if ( strExtensionName [ 0 ] == 0 )
				{
					theFileSetupInfo.aLicenseImageFile.xbExtensionFlags [ uiExtensionId ] = IDRS_FALSE;
				}
				else
				{
					cout << "Do you have a license for the " << strExtensionName << " extension (y/n)?" << endl;
					do
					{
						cin >> c;
						cout << endl;
					} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
					theFileSetupInfo.aLicenseImageFile.xbExtensionFlags [ uiExtensionId ] = ( c == 'y' || c == 'Y' );
				}

				if ( theFileSetupInfo.aLicenseImageFile.xbExtensionFlags [ uiExtensionId ] )
				{
					cout << "Please enter " << strExtensionName << " software key:" << endl;
					cin.ignore (); 
					cin.getline ( theFileSetupInfo.aLicenseImageFile.xstrExtensionSoftwareKey [ uiExtensionId ], IDRS_LICENSE_MAX_SIZE );
					cout << endl;
				}

				cout << endl;
			}
		}

#ifdef IDRS_OS_WIN32
		/*
		* LEADTOOLS
		*/

		cout << "LEADTOOLS" << endl;
		cout << "LEADTOOLS is an external imaging SDK." << endl;
		cout << "The iDRS can use the raster imaging features of leadtools to extend its imaging functionalities." << endl;
		cout << "Do you have a license for Leadtools raster imaging (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadLeadtools = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadLeadtools )
		{
			cout << "Getting information for the IDRS_MODULE_LEADTOOLS module." << endl;

			memset ( & theFileSetupInfo.aLicenseLeadtools, 0, sizeof ( IDRS_LICENSE_LEADTOOLS ));

			cout << endl;

			theFileSetupInfo.aLicenseLeadtools.uiMajorVersionIdentifier = 15;
			theFileSetupInfo.aLicenseLeadtools.uiMinorVersionIdentifier = 0;

			for ( uiExtensionId = 0; uiExtensionId < IDRS_LEADTOOLS_EXTENSION_COUNT; uiExtensionId ++ )
			{
				switch ( uiExtensionId )
				{
				case IDRS_LEADTOOLS_EXTENSION_JPEG2000:
					cout << "IDRS_LEADTOOLS_EXTENSION_JPEG2000 is the JPEG 2000 extension." << endl;
					sprintf ( strExtensionName, "IDRS_LEADTOOLS_EXTENSION_JPEG2000" );
					break;

				default:
					strExtensionName [ 0 ] = 0;
					break;
				}

				if ( strExtensionName [ 0 ] == 0 )
				{
					theFileSetupInfo.aLicenseLeadtools.xbExtensionFlags [ uiExtensionId ] = IDRS_FALSE;
				}
				else
				{
					cout << "Do you have a license for the " << strExtensionName << " extension (y/n)?" << endl;
					do
					{
						cin >> c;
						cout << endl;
					} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
					theFileSetupInfo.aLicenseLeadtools.xbExtensionFlags [ uiExtensionId ] = ( c == 'y' || c == 'Y' );
				}

				if ( theFileSetupInfo.aLicenseLeadtools.xbExtensionFlags [ uiExtensionId ] )
				{
					cout << "Please enter " << strExtensionName << " software key:" << endl;
					cin.ignore (); 
					cin.getline ( theFileSetupInfo.aLicenseLeadtools.xstrExtensionSoftwareKey [ uiExtensionId ], IDRS_LICENSE_MAX_SIZE );
					cout << endl;
				}

				cout << endl;
			}
		}
#endif /* IDRS_OS_WIN32 */

#ifdef IDRS_OS_WIN32
		/*
		* IDRS_MODULE_FINGERPRINT
		*/

		cout << "IDRS_MODULE_FINGERPRINT" << endl;

		cout << "IDRS_MODULE_FINGERPRINT is the fingerprint module." << endl;
		cout << "Do you have a license for the IDRS_MODULE_FINGERPRINT module (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadFingerprint = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadFingerprint )
		{
			cout << "Getting information for the IDRS_MODULE_FINGERPRINT module." << endl;

			memset ( & theFileSetupInfo.aLicenseFingerprint, 0, sizeof ( IDRS_LICENSE_FINGERPRINT ));
			theFileSetupInfo.aLicenseFingerprint.ltLicenseType = theFileSetupInfo.ltLicenseType;

			cout << "Please enter IDRS_MODULE_FINGERPRINT software key:" << endl;
			cin.ignore (); 
			cin.getline ( theFileSetupInfo.aLicenseFingerprint.strFingerprintSoftwareKey, IDRS_LICENSE_MAX_SIZE );
			cout << endl;
		}
#endif /* IDRS_OS_WIN32 */

#ifdef IDRS_OS_WIN32
		/*
		* Snowbound
		*/

		cout << "Snowbound" << endl;
		cout << "Snowbound is an external imaging SDK." << endl;
		cout << "The iDRS can use the imaging features of Snowbound to extend its imaging functionalities." << endl;
		cout << "Do you have a license for Snowbound SDK (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');
		theFileSetupInfo.bLoadSnowbound = ( c == 'y' || c == 'Y' );

		if ( theFileSetupInfo.bLoadSnowbound )
		{
			cout << "Getting information for the IDRS_MODULE_SNOWBOUND module." << endl;

			memset ( & theFileSetupInfo.aLicenseSnowbound, 0, sizeof ( IDRS_MODULE_SNOWBOUND ));

			cout << endl;

			theFileSetupInfo.aLicenseSnowbound.uiMajorVersionIdentifier = 1;
			theFileSetupInfo.aLicenseSnowbound.uiMinorVersionIdentifier = 0;
		}
#endif /* IDRS_OS_WIN32 */


	}
	iRet = CallSetup ( theFileSetupInfo );

	if ( iRet == 0 && ! bSetupFromSoftwareKeysFile )
	{
		/*
		* Save the information to a file.
		*/

		cout << "The setup was successfull." << endl;
		cout << "This sample can save the information you enter to the " << strSoftwareKeysFile;
		cout << " file, so you won't have to re-enter the information next time you'll run this application." << endl;
		cout << "Do you want to save the information you entered to the " << strSoftwareKeysFile << " file (y/n)?" << endl;
		do
		{
			cin >> c;
			cout << endl;
		} while(c != 'y' && c != 'Y' && c != 'n' && c != 'N');

		if ( c == 'y' || c == 'Y' )
		{
			WriteFileSetupInfo ( strSoftwareKeysFile, theFileSetupInfo );
		}
	}

#ifdef IDRS_OS_WIN32
	//Restore the current directory
	SetCurrentDirectoryA ( strCurrentDir );
#endif /* IDRS_OS_WIN32 */

	return iRet;
}

