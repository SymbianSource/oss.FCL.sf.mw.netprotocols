/**
* Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* PG: Hacked from eustd.h
* 
*
*/



/**
 @file CONSUI.H
 @internalComponent
*/

#ifndef __FTPTEST_H
#define __FTPTEST_H

#include <e32cons.h>
#include <f32file.h>
#include "COMINIT.H"
// Disable MSVC++ 5.0 aggressive warnings about non-expansion of inline functions. 
#pragma warning(disable : 4710)	// function '...' not expanded

/**
write all your messages to this
*/
LOCAL_D CConsoleBase* __FTPDebugConsole; 

LOCAL_D RFs	iFs;
LOCAL_D RFile	__FTPDebugFile;

/**
code this function for the real example
@internalComponent
*/
LOCAL_C void doExampleL(); 

/**
private
Initialize with cleanup stack, then do example
@internalComponent
*/
LOCAL_C void callExampleL(); 

/**
main function called by E32
*/
GLDEF_C TInt E32Main() 
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error,callExampleL()); // more initialization, then do example
	__ASSERT_ALWAYS(!error,User::Panic(_L("EPOC32EX"),error));
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	return 0; // and return
    }

/**
Initialize and call example code under cleanup stack
@internalComponent
*/
LOCAL_C void callExampleL() 
    {
	__FTPDebugConsole=Console::NewL(_L("FTP Session Client console interface"),
		TSize(KConsFullScreen,KConsFullScreen));
	CleanupStack::PushL(__FTPDebugConsole);
	CommInitL();
	__FTPDebugConsole->Printf(_L("Starting...\n"));
	TRAPD(error,doExampleL()); // perform example function
	if (error)
	    {
	    __FTPDebugConsole->Printf(_L("Failed: leave code=%d"), error);
            //_LIT(KMsgPressAnyKey,_L("[press any key]"));
            __FTPDebugConsole->Printf(_L("[press any key]"));
            __FTPDebugConsole->Getch();
	    }
	CleanupStack::PopAndDestroy(); // close console
    }

#endif
