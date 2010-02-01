// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include "T_INC053172.h"

// This is testing that we add a slash("\") when the path is missing in a URI.


void CTestFixINC053172::DoRunL()
	{
	// Create a session and a transaction
	iSession.OpenL();
	_LIT8(KTestURIString,"http://www.dealtime.com");
	TUriParser8 uri; 
	uri.Parse(KTestURIString);
	iMyStrP = iSession.StringPool();
	iTransaction = iSession.OpenTransactionL(uri, *this);

	iError = KErrGeneral;

	// Submit a transaction
	iTransaction.SubmitL();

	// Run the test
	CActiveScheduler::Start();

	iSession.Close();

	// Leave if it hasn't passed.
	User::LeaveIfError(iError);
	}

void CTestFixINC053172::DoCancel()
	{
	}


TInt CTestFixINC053172::MHFRunError(TInt /*aError*/,
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	aTransaction.Fail();
	return 0;
	}

CTestFixINC053172* CTestFixINC053172::NewLC()
	{
	CTestFixINC053172* that = new (ELeave) CTestFixINC053172;
	CleanupStack::PushL(that);
	return that;
	}

CTestFixINC053172::~CTestFixINC053172()
	{
	}


void CTestFixINC053172::MHFRunL(RHTTPTransaction aTransaction,
								  const THTTPEvent& aEvent)
	{
	switch(aEvent.iStatus)
		{
	case THTTPEvent::ESucceeded:
		// If nothing has gone wrong so far, we've passed
		if (iError == KErrGeneral)
			iError = KErrNone;
		CActiveScheduler::Stop();
		break;

	case THTTPEvent::EFailed:
		CActiveScheduler::Stop();
		break;

	case THTTPEvent::EGotResponseBodyData:
		{
		TPtrC8 ptr;
		aTransaction.Response().Body()->GetNextDataPart(ptr);
		aTransaction.Response().Body()->ReleaseData();
		}
		break;

	default:
		break;
		}
	}

const TDesC& CTestFixINC053172::TestName()
	{
	_LIT(KTestName,"Defect INC053172");
	return KTestName;
	}

