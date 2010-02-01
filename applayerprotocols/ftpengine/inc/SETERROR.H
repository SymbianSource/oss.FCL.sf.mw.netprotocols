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
* Active object to set an error asynchronously
* Author:	Philippe Gabriel
* Set an error condition to be reported asynchronously
* so the client get the error condition from the scheduler
* 
*
*/



/**
 @file SETERROR.H
 @internalComponent
*/

#if !defined(__SETERROR_H__)
#define __SETERROR_H__

#include "DEBUG.H"
#include "SETERROR.H"
#include <e32base.h>

//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////

class MSetErrorNotifier
/**
@internalComponent
*/
{
public:
	virtual ~MSetErrorNotifier(){};
	// Notify of normal completion of an operation
	virtual void SetErrorNotifier(const TInt)=0;
};

class CFTPSetError : public CActive
/**
@internalComponent
*/
{
public:

CFTPSetError(MSetErrorNotifier *aNotifier):CActive(CActive::EPriorityStandard)
{
	iNotifier = aNotifier;
}

static CFTPSetError* NewL(MSetErrorNotifier* aNotifier)
{
	CFTPSetError* self = new (ELeave) CFTPSetError(aNotifier);
	CActiveScheduler::Add(self);
	return self;
} 
~CFTPSetError()
{
	FTPPROTDEBUG(_DBGFtpseterr,_L("CFTPSetError::~CFTPSetError called\n"));
}

void SetError(const TInt aError)
{
TRequestStatus* p=&iStatus;	
iError = aError;
// Complete immediately
SetActive();
User::RequestComplete(p, KErrNone);
}
protected:
void RunL()
	{
	// 
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
void DoCancel()
{
}
private:
	/** The error to report to my client */
	TInt	iError; 
	/** Notifier to call to report error */
	MSetErrorNotifier* iNotifier; 
};
#endif // __SETERROR_H__
