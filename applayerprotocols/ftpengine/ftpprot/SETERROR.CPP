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
// Active object to set an error asynchronously
// Author:	Philippe Gabriel
// Set an error condition to be reported asynchronously
// so the client get the error condition from the scheduler
// 
//

/**
 @file SETERROR.CPP
 @internalComponent
*/

#include "debug.h"
#include "seterror.h"

CFTPSetError::CFTPSetError(MSetErrorNotifier *aNotifier
#if defined(__EPOCCONSOLETRACE__)
						,CConsoleBase* aDebugConsole		
#elif defined(__EPOCFILETRACE__)
						,RFile*	aDebugFile
#endif						   
						):CActive(CActive::EPriorityStandard)
	{
#if defined(__EPOCCONSOLETRACE__)
	__FTPDebugConsole = aDebugConsole;
#elif defined(__EPOCFILETRACE__)
	__FTPDebugFile = aDebugFile;
#endif
	iNotifier = aNotifier;
	}


CFTPSetError* CFTPSetError::NewL(MSetErrorNotifier *aNotifier
#if defined(__EPOCCONSOLETRACE__)
						,CConsoleBase* aDebugConsole		
#elif defined(__EPOCFILETRACE__)
						,RFile*	aDebugFile
#endif	
						)
	{
	CFTPSetError* self = new (ELeave) CFTPSetError(aNotifier
#if defined(__EPOCCONSOLETRACE__)
												,aDebugConsole		
#elif defined(__EPOCFILETRACE__)
												,aDebugFile
#endif	
		);
	CActiveScheduler::Add(self);
	return self;
	} 

void CFTPSetError::DoCancel()
{
}

void CFTPSetError::RunL()
	{
	FTPPROTDEBUG(_DBGFtpseterr,_L("CFTPSetError::RunL called "));
	switch (iStatus.Int())
		{
		case KErrNone:
			FTPPROTDEBUG(_DBGFtpseterr,_L("iStatus: NoError\n"));
			// Call Upper layer notifier
			iNotifier->SetErrorNotifier(iError);
			break;
		default:
			__ASSERT_DEBUG(FALSE, User::Panic(_L("CFtpSetError"), 0));
			break;
		}
	} 

CFTPSetError::~CFTPSetError()
	{
	FTPPROTDEBUG(_DBGFtpseterr,_L("CFTPSetError::~CFTPSetError called\n"));
	}

void CFTPSetError::SetError(const TInt aError)
	{
	TRequestStatus* p=&iStatus;	
	iError = aError;
	// Complete immediately
	SetActive();
	User::RequestComplete(p, KErrNone);
	}
