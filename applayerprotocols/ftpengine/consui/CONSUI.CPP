// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Author: Philippe Gabriel
// A simple test program which uses the statically linked dll  "ftpprot.dll"
// 
//

/**
 @file CONSUI.CPP
 @internalComponent
*/

//standard example header
//#include "defs.h"
//#include "debug.h"
#include <e32base.h>
#include <es_sock.h>
#include "CONSUI.H"
#include "FTPSESS.H"
#include "CONSOLE.H"
#include "FTPUI.H"

TBuf<1512> myBuffer;
CFTPSession* myCFTPSession;
CmdConsole* myCmdConsole;
CFtpUI *myFtpUI;

LOCAL_C void doExampleL()
    {
//
//	TInetAddr myaddr(INET_ADDR(194,129,2,141),21);// myaddr
//	TPtrC	DNSName(_L("ftp.microsoft.com"));
//	TPtrC	DNSName(_L("ftp.sun.com"));
//	TPtrC	DNSName(_L("ftp.netscape.com"));
//	TPtrC	DNSName(_L("ftp.insignia.com"));
//	TPtrC	DNSName(_L("sparky"));
//	TPtrC	DNSName(_L("radian.ee.ic.ac.uk"));
//

	myFtpUI = CFtpUI::NewLC();
	CActiveScheduler* exampleScheduler=new (ELeave) CActiveScheduler;
    CleanupStack::PushL(exampleScheduler);
    CActiveScheduler::Install(exampleScheduler);
	myCFTPSession = CFTPSession::NewL(myFtpUI);
    CleanupStack::PushL(myCFTPSession);
	myCmdConsole = CmdConsole::NewL(myFtpUI,__FTPDebugConsole);
    CleanupStack::PushL(myCmdConsole);
	// Initialise FtpUI console
	myFtpUI->SetConsole(__FTPDebugConsole);
	// Initialise FtpUI Cmd console
	myFtpUI->SetCmdConsole(myCmdConsole);
	// Initialise FtpUI FTPSession
	myFtpUI->SetFTPSession(myCFTPSession);
	// Display some info
	myFtpUI->Help();
	// Get a console
	myCmdConsole->Reset();
	myCmdConsole->Start();
	CActiveScheduler::Start();
	//Delete my objects
	CleanupStack::Pop();
	delete myCmdConsole;
	CleanupStack::Pop();
	delete myCFTPSession;
	CleanupStack::Pop();
	delete myFtpUI;
	CleanupStack::Pop();
	delete exampleScheduler;
	}


