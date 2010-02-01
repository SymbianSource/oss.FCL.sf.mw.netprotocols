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

#include "t_KHN-4Z7DT7.h"

_LIT8(KExpected, "The quick brown fox jumped over the lazy dog!");

void CTestFixKHN4Z7DT7::DoRunL()
	{
	// Create a session and a transaction
	iSession.OpenL();
//	_LIT8(KTestURIString,"http://WapTestIP:9000/anything?req=quickchunkedfox.req.txt&resp=quickchunkedfox.txt");
	_LIT8(KTestURIString,"WapTestName");
	
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KTestURIString(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUriPtr8=newUrl8->Des();
	newUriPtr8.Insert(0,_L8("http://"));
	newUriPtr8.Append(_L8(":9000/anything?req=quickchunkedfox.req.txt&resp=quickchunkedfox.txt"));

	TUriParser8 uri8; 
//	uri8.Parse(*newUrl8); // -- old
	uri8.Parse(newUriPtr8);

	iMyStrP = iSession.StringPool();
	iTransaction = iSession.OpenTransactionL(uri8, *this);

	iError = KErrGeneral;

	// Submit a transaction
	iTransaction.SubmitL();

	// Run the test
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(newUrl8);

	iSession.Close();

	// Leave if it hasn't passed.
	User::LeaveIfError(iError);
	}

void CTestFixKHN4Z7DT7::DoCancel()
	{
	}


TInt CTestFixKHN4Z7DT7::MHFRunError(TInt /*aError*/,
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	aTransaction.Fail();
	return 0;
	}

CTestFixKHN4Z7DT7* CTestFixKHN4Z7DT7::NewLC(CScriptFile* aIniSettingsFile)
	{
	CTestFixKHN4Z7DT7* that = new (ELeave) CTestFixKHN4Z7DT7(aIniSettingsFile);
	CleanupStack::PushL(that);
	return that;
	}

CTestFixKHN4Z7DT7::~CTestFixKHN4Z7DT7()
	{
	}

CTestFixKHN4Z7DT7::CTestFixKHN4Z7DT7(CScriptFile* aIniSettingsFile)	:
	CHttpTestBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

void CTestFixKHN4Z7DT7::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
		TBool finished = aTransaction.Response().Body()->GetNextDataPart(ptr);
		if (KExpected().Length()>=ptr.Length())
			{
			if (ptr.Compare(KExpected().Mid(iCount, ptr.Length())))
				{
				iError = KErrNotFound;
				}
			iCount += ptr.Length();
			}
		if (finished && iCount != KExpected().Length())
			iError = KErrEof;
		aTransaction.Response().Body()->ReleaseData();
		}
		break;

	default:
		break;
		}
	}

const TDesC& CTestFixKHN4Z7DT7::TestName()
	{
	_LIT(KTestName,"Defect KHN-4Z7DT7");
	return KTestName;
	}

