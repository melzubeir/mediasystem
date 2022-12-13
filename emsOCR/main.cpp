/************************************************************************
 * $Id: main.cpp 648 2011-03-10 03:42:39Z elzubeir $
 *
 * ------------
 * Description:
 * ------------
 *
 * (C) Copyright 2009,2010 ALLCONTENT. All rights reserved.
 *
 * -----------------
 * Revision Details:    (Updated by Revision Control System)
 * -----------------
 *  $Date: 2011-03-10 07:42:39 +0400 (Thu, 10 Mar 2011) $
 *  $Author: elzubeir $
 *  $Revision: 648 $
 *  $HeadURL: file:///opt/svn/socialhose/trunk/app/emsOCR/main.cpp $
 *
 ************************************************************************/

#include <QtGui/QApplication>
#include "emsocrdialog.h"

#include <windows.h>
#include <tchar.h>
#include <dbghelp.h>
#include <stdio.h>


// Directives
// Link with DbgHelp.lib
#pragma comment ( lib, "dbghelp.lib" )

LONG __stdcall MyCustomFilter( EXCEPTION_POINTERS* pep );
void CreateMiniDump( EXCEPTION_POINTERS* pep );

int main(int argc, char *argv[])
{
	SetUnhandledExceptionFilter( MyCustomFilter );
	bool autoStart = false;

	if(argc >=2)
	{
		//if(stricmp(argv[1], "autoStart") == 0)
			autoStart = true;
	}

	QApplication a(argc, argv);
    emsOCRDialog w(autoStart);
    w.show();

	//SetErrorMode(SEM_NOGPFAULTERRORBOX);

	return a.exec();
}


LONG __stdcall MyCustomFilter( EXCEPTION_POINTERS* pep )
{
	CreateMiniDump( pep );

	return EXCEPTION_EXECUTE_HANDLER;
}


////////////////////////////////////////////////////////////////////////////////
// CreateMiniDump() function
//

void CreateMiniDump( EXCEPTION_POINTERS* pep )
{
	// Open the file

	HANDLE hFile = CreateFile( _T("MiniDump.dmp"), GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) )
	{
		// Create the minidump

		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId           = GetCurrentThreadId();
		mdei.ExceptionPointers  = pep;
		mdei.ClientPointers     = FALSE;

		MINIDUMP_TYPE mdt       = MiniDumpNormal;

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 );

		/*if( !rv )
			_tprintf( _T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError() );
		else
			_tprintf( _T("Minidump created.\n") );*/

		// Close the file

		CloseHandle( hFile );
	}
	else
	{
		//_tprintf( _T("CreateFile failed. Error: %u \n"), GetLastError() );
	}

	// Re-run emsOCR, and automatically start processing.
	QProcess prcess;
	QStringList list;
	QString appName = QApplication::applicationFilePath();

	list.append("autoStart");
	prcess.startDetached(appName, list);
}
