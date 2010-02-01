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

#include "t_CLE-4Y5LC9.h"

_LIT8(KPutBody, "The Quick Brown Fox Jumped Over The Lazy Dog");

CTestFixCLE4Y5LC9Base::CTestFixCLE4Y5LC9Base(CScriptFile* aIniSettingsFile)	:
	CHttpTestBase(),
	iIniSettingsFile(aIniSettingsFile)
	{
	}

void CTestFixCLE4Y5LC9Base::DoRunL()
	{
	// Create a session and a transaction
	iSession.OpenL();
//	_LIT8(KTestURIString,"http://scooby:doo@WapTestName/upload/testchunked"); //--old
	_LIT8(KTestURIString,"WapTestName");
	
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KTestURIString(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUriPtr8=newUrl8->Des();
	newUriPtr8.Insert(0,_L8("http://scooby:doo@"));
	newUriPtr8.Append(_L8("/upload/testchunked"));

	TUriParser8 uri8; 
//	uri8.Parse(*newUrl8); // -- old
	uri8.Parse(newUriPtr8);

	iMyStrP = iSession.StringPool();
	iTransaction = iSession.OpenTransactionL(uri8, *this, iMyStrP.StringF(HTTP::EPUT,RHTTPSession::GetTable()));
	iTransaction.Request().SetBody(*this);
	THTTPHdrVal contentTypeHdr(iMyStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable()));
	iTransaction.Request().GetHeaderCollection().SetFieldL(iMyStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable()),contentTypeHdr);

	// Start at the start of the post data on the 1st submit, and
	// assume there's an error unless the right thing happens.
	iByteCount = 0;
	iSubmitCount = 0;
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

void CTestFixCLE4Y5LC9Base::DoCancel()
	{
	}

TBool CTestFixCLE4Y5LC9Base::GetNextDataPart(TPtrC8& aDataChunk)
	{
	aDataChunk.Set(KPutBody().Mid(iByteCount, 1));
	return (iByteCount == KPutBody().Length() - 1);
	}

void CTestFixCLE4Y5LC9Base::ReleaseData()
	{
	iByteCount++;

	// If it's the last byte of the first transaction, cancel and resubmit.
	if (iByteCount == KPutBody().Length())
		{
		if (iSubmitCount == 0 )
			{
			iSubmitCount++;

			iTransaction.Cancel();
			// In fact this can't fail as the transaction never compresses
			// the event queue.
			TRAP_IGNORE(iTransaction.SubmitL());
			}
		}
    else 
       {			
	   TRAP_IGNORE(iTransaction.NotifyNewRequestBodyPartL());
       }
	}

TInt CTestFixCLE4Y5LC9Base::OverallDataSize()
	{
	return -1;
	}

TInt CTestFixCLE4Y5LC9Base::MHFRunError(TInt /*aError*/,
										RHTTPTransaction aTransaction,
										const THTTPEvent& /*aEvent*/)
	{
	aTransaction.Fail();
	return 0;
	}

void CTestFixCLE4Y5LC9Base::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}

CTestFixCLE4Y5LC9_1* CTestFixCLE4Y5LC9_1::NewLC(CScriptFile* aIniSettingsFile)
	{
	CTestFixCLE4Y5LC9_1* that = new (ELeave) CTestFixCLE4Y5LC9_1(aIniSettingsFile);
	CleanupStack::PushL(that);
	return that;
	}

CTestFixCLE4Y5LC9_1::CTestFixCLE4Y5LC9_1(CScriptFile* aIniSettingsFile)	:
	CTestFixCLE4Y5LC9Base(aIniSettingsFile)
	{
	}

CTestFixCLE4Y5LC9_1::~CTestFixCLE4Y5LC9_1()
	{
	}

TInt CTestFixCLE4Y5LC9_1::Reset()
	{
	iByteCount = 0;
	return KErrNone;
	}

void CTestFixCLE4Y5LC9_1::MHFRunL(RHTTPTransaction aTransaction,
								  const THTTPEvent& aEvent)
	{
	switch(aEvent.iStatus)
		{
	case THTTPEvent::ESucceeded:
		iError = KErrNone;
		CActiveScheduler::Stop();
		break;

	case THTTPEvent::EFailed:
		CActiveScheduler::Stop();
		break;

	case THTTPEvent::EGotResponseBodyData:
		aTransaction.Response().Body()->ReleaseData();
		break;

	default:
		break;
		}
	}

const TDesC& CTestFixCLE4Y5LC9_1::TestName()
	{
	_LIT(KTestName,"Defect CLE-4Y5LC9 Test 1");
	return KTestName;
	}



CTestFixCLE4Y5LC9_2* CTestFixCLE4Y5LC9_2::NewLC(CScriptFile* aIniSettingsFile)
	{
	CTestFixCLE4Y5LC9_2* that = new (ELeave) CTestFixCLE4Y5LC9_2(aIniSettingsFile);
	CleanupStack::PushL(that);
	return that;
	}

CTestFixCLE4Y5LC9_2::CTestFixCLE4Y5LC9_2(CScriptFile* aIniSettingsFile)	:
	CTestFixCLE4Y5LC9Base(aIniSettingsFile)
	{
	}

CTestFixCLE4Y5LC9_2::~CTestFixCLE4Y5LC9_2()
	{
	}

TInt CTestFixCLE4Y5LC9_2::Reset()
	{
	return KErrNotSupported;
	}

void CTestFixCLE4Y5LC9_2::MHFRunL(RHTTPTransaction aTransaction,
								  const THTTPEvent& aEvent)
	{
	switch(aEvent.iStatus)
		{
	case KErrHttpCantResetRequestBody:
		iError = KErrNone;
		// drop through
	case THTTPEvent::ESucceeded:
	case THTTPEvent::EFailed:
		if (!iFinished)
			CActiveScheduler::Stop();
		iFinished = ETrue;
		break;

	case THTTPEvent::EGotResponseBodyData:
		aTransaction.Response().Body()->ReleaseData();
		break;

	default:
		break;
		}
	}

const TDesC& CTestFixCLE4Y5LC9_2::TestName()
	{
	_LIT(KTestName,"Defect CLE-4Y5LC9 Test 2");
	return KTestName;
	}
