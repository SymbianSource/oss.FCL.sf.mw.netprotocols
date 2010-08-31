// Copyright (c) 1998-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// PG: Hacked from eustd.h
// 
//

#ifndef __FTPTEST_H
#define __FTPTEST_H

#ifndef __EPOC32__
#define PDD_NAME _L("ECDRV")
#define LDD_NAME _L("ECOMM")
#else
#define PDD_NAME _L("EUART1")
#define LDD_NAME _L("ECOMM")
#endif

#include <e32cons.h>

// public
LOCAL_D CConsoleBase* __FTPDebugConsole; // write all your messages to this
LOCAL_C void doExampleL(); // code this function for the real example

// private
LOCAL_C void callExampleL(); // initialize with cleanup stack, then do example

GLDEF_C TInt E32Main() // main function called by E32
    {
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error,callExampleL()); // more initialization, then do example
	__ASSERT_ALWAYS(!error,User::Panic(_L("EPOC32EX"),error));
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	return 0; // and return
    }
LOCAL_C void InitCommsL()
	{
#ifndef SIROCCO_CODE_MIGRATION
	TInt err;

	err=User::LoadPhysicalDevice(PDD_NAME);
	if (err!=KErrNone && err!=KErrAlreadyExists)
		User::Leave(err);

	err=User::LoadLogicalDevice(LDD_NAME);
	if (err!=KErrNone && err!=KErrAlreadyExists)
		User::Leave(err);
#endif //SIROCCO_CODE_MIGRATION
	}

LOCAL_C void callExampleL() // initialize and call example code under cleanup stack
    {
	__FTPDebugConsole=Console::NewL(_L("FTP Test Code"),
		TSize(KConsFullScreen,KConsFullScreen));
	CleanupStack::PushL(__FTPDebugConsole);
	InitCommsL();
	TRAPD(error,doExampleL()); // perform example function
	if (error) __FTPDebugConsole->Printf(_L("failed: leave code=%d"), error);
	else __FTPDebugConsole->Printf(_L("ok"));
	__FTPDebugConsole->Printf(_L(" [press any key]"));
	__FTPDebugConsole->Getch(); // get and ignore character
	CleanupStack::PopAndDestroy(); // close console
    }

#endif
