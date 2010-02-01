// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// System Includes
#include <http/cuaproffilterinterface.h>

// User Includes
#include "T_UAProf.h"
#include "testcpimanager.h"


// Profile and Profile-Diff data to be added to session headers
_LIT8(KProfile1, "www.symbian.com/profiles1");
_LIT8(KProfile2, "www.symbian.com/profiles2");


CTestUAProf* CTestUAProf::NewL(TInt aTestNumber, const TDesC8& aProxyAddr, const TDesC8& aHostName)
	{
	CTestUAProf* self = new(ELeave)CTestUAProf(aTestNumber);
	CleanupStack::PushL(self);
	self->ConstructL(aProxyAddr, aHostName);
	CleanupStack::Pop(self);
	return self;
	}

CTestUAProf::~CTestUAProf()
	{
	delete iCpiManager;
	delete iProxyAddr;
	delete iHostName;
	}

CTestUAProf::CTestUAProf(TInt aTestNumber) :
	iTestNumber(aTestNumber)
	{
	}

void CTestUAProf::ConstructL(const TDesC8& aProxyAddr, const TDesC8& aHostName)
	{
	iProxyAddr = aProxyAddr.AllocL();
	iHostName = aHostName.AllocL();
	}

void CTestUAProf::DoRunL()
	{
	// Open the WSP session
	iSession.OpenL(_L8("WSP/WSP"));
	CleanupClosePushL(iSession);
	iSession.SetSessionEventCallback(this);

	// Install the UAProf filter using the Test CPI Manager
	iCpiManager = CTestCpiManager::NewL(iSession.StringPool());
	CUAProfFilterInterface::InstallFilterL(iSession, iCpiManager);

	// Test cases
	switch(iTestNumber)
		{
		case 1:
			DoUAProfTest1L();
			break;
		case 2:
			DoUAProfTest2L();
			break;
		case 3:
			DoUAProfTest3L();
			break;
		case 4:
			DoUAProfTest4L();
			break;
		case 5:
			DoUAProfTest5L();
			break;
		case 6:
			DoUAProfTest6L();
			break;
		default:
			__DEBUGGER(); // Invalid test number
		}

	delete iCpiManager;
	iCpiManager = NULL;
	CleanupStack::PopAndDestroy(); // iSession

	// If any transaction failed then fail the test
	User::LeaveIfError(iFailureError);
	}

TInt CTestUAProf::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTestUAProf::DoCancel()
	{
	}

const TDesC& CTestUAProf::TestName()
	{
	_LIT(KHeaderTestName,"CTestUAProf");
	return KHeaderTestName;
	}

void CTestUAProf::MHFSessionRunL(const THTTPSessionEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPSessionEvent::EConnect:
			{
			} break;
		case THTTPSessionEvent::EConnectedOK:
			{
			iEngine->Utils().LogIt(_L("<Session connected OK>"));
			CActiveScheduler::Stop();
			} break;
		case THTTPSessionEvent::EDisconnected:
			{
			iEngine->Utils().LogIt(_L("<Session disconnected>"));
			CActiveScheduler::Stop();
			} break;
		case THTTPSessionEvent::EConnectionTimedOut:
			{
			iEngine->Utils().LogIt(_L("<Connection timed out>"));
			CActiveScheduler::Stop();
			} break;
		default:
			{
			iEngine->Utils().LogIt(_L("<Unrecognised session event> %d"), aEvent.iStatus);
			iFailureError = KErrTestFailed;
			CActiveScheduler::Stop();
			}
			break;
		}
	}

TInt CTestUAProf::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTestUAProf::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			iEngine->Utils().LogIt(_L("<Got response headers>"));
			DumpRespHeaders(aTransaction);
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
			DumpResponseBody(aTransaction);
			} break;
		case THTTPEvent::EResponseComplete:
			{
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			aTransaction.Close();
			if (--iOpenTransactionCount == 0)
				{
				CActiveScheduler::Stop();
				}
			} break;
		case THTTPEvent::EFailed:
			{
			iEngine->Utils().LogIt(_L("<Transaction failed>"));
			aTransaction.Close();
			if (--iOpenTransactionCount == 0)
				CActiveScheduler::Stop();
			if (iFailureError == KErrNone)
				iFailureError = KErrTestFailed;
			} break;
		default:
			{
			iEngine->Utils().LogIt(_L("<Unrecognised transaction event> %d"), aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				iFailureError = aEvent.iStatus;
				CActiveScheduler::Stop();
				}
			} break;
		}
	}

TInt CTestUAProf::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTestUAProf::DoUAProfTest1L()
	{
	// No Profile or Profile-Diff files on the device

	DoGenericUAProfTestL();
	}

void CTestUAProf::DoUAProfTest2L()
	{
	// Single Profile file on the device

	HBufC8* prof = HBufC8::NewLC(KProfile1().Length());
	*prof = KProfile1();
	iCpiManager->SetProfileDataL(prof);

	CleanupStack::Pop(prof);

	DoGenericUAProfTestL();
	}

void CTestUAProf::DoUAProfTest3L()
	{
	// Single Profile-Diff file on the device - should not get added to the session header

	TBuf8<32> buf;
	buf.Append(0x01);
	buf.Append(0x05);
	buf.Append(0x04);
	HBufC8* diff = buf.AllocLC();
	iCpiManager->SetProfileDiffDataL(diff);

	CleanupStack::Pop(diff);

	DoGenericUAProfTestL();
	}

void CTestUAProf::DoUAProfTest4L()
	{
	// Single Profile file and single Profile-Diff file on the device

	HBufC8* prof = HBufC8::NewLC(KProfile2().Length());
	*prof = KProfile2();
	iCpiManager->SetProfileDataL(prof);

	TBuf8<32> buf;
	buf.Append(0x01);
	buf.Append(0x05);
	buf.Append(0x04);
	HBufC8* diff = buf.AllocLC();
	iCpiManager->SetProfileDiffDataL(diff);

	CleanupStack::Pop(2, prof); // diff, prof

	DoGenericUAProfTestL();
	}

void CTestUAProf::DoUAProfTest5L()
	{
	// Single Profile file and two Profile-Diff files on the device
	
	HBufC8* prof2 = HBufC8::NewLC(KProfile2().Length());
	*prof2 = KProfile2();
	iCpiManager->SetProfileDataL(prof2);

	TBuf8<32> buf1;
	buf1.Append(0x01);
	buf1.Append(0x05);
	buf1.Append(0x04);
	HBufC8* diff1 = buf1.AllocLC();
	iCpiManager->SetProfileDiffDataL(diff1);

	TBuf8<32> buf2;
	buf2.Append(0x01);
	buf2.Append(0x05);
	buf2.Append(0x04);
	HBufC8* diff2 = buf2.AllocLC();
	iCpiManager->SetProfileDiffDataL(diff2);

	CleanupStack::Pop(3, prof2); // diff2. diff1, prof2

	DoGenericUAProfTestL();
	}

void CTestUAProf::DoUAProfTest6L()
	{
	// Two Profile files and two Profile-Diff files on the device
	
	HBufC8* prof1 = HBufC8::NewLC(KProfile1().Length());
	*prof1 = KProfile1();
	iCpiManager->SetProfileDataL(prof1);

	HBufC8* prof2 = HBufC8::NewLC(KProfile2().Length());
	*prof2 = KProfile2();
	iCpiManager->SetProfileDataL(prof2);

	TBuf8<32> buf1;
	buf1.Append(0x01);
	buf1.Append(0x05);
	buf1.Append(0x04);
	HBufC8* diff1 = buf1.AllocLC();
	iCpiManager->SetProfileDiffDataL(diff1);

	TBuf8<32> buf2;
	buf2.Append(0x01);
	buf2.Append(0x05);
	buf2.Append(0x04);
	HBufC8* diff2 = buf2.AllocLC();
	iCpiManager->SetProfileDiffDataL(diff2);

	CleanupStack::Pop(4, prof1); // diff2, diff1, prof2, prof1

	DoGenericUAProfTestL();
	}

void CTestUAProf::DoGenericUAProfTestL()
	{
	// Connect to the WAP proxy
	SessionConnectL();

	// Submit a transaction
	CreateTransactionL();

	// Disconnect from the WAP proxy
	SessionDisconnectL();
	}

void CTestUAProf::SessionConnectL()
	{
	RStringPool strP = iSession.StringPool();
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	RStringF proxy = strP.OpenFStringL(*iProxyAddr);
	CleanupClosePushL(proxy);
	THTTPHdrVal proxyAddr(proxy);
	connInfo.SetPropertyL(strP.StringF(HTTP::EWspProxyAddress, RHTTPSession::GetTable()), proxyAddr);
	iSession.ConnectL();
	CleanupStack::PopAndDestroy(); // proxy
	CActiveScheduler::Start();
	}

void CTestUAProf::CreateTransactionL()
	{
	iOpenTransactionCount = 0;
	RStringF get = iSession.StringPool().StringF(HTTP::EGET, RHTTPSession::GetTable());
	TBuf8<70> url;
	url.Zero();
	url.Format(*iHostName);
	TUriParser8 urlParser;
	urlParser.Parse(url);
	RHTTPTransaction tr = iSession.OpenTransactionL(urlParser, *this, get);
	tr.SubmitL();
	get.Close();
	++iOpenTransactionCount;
	CActiveScheduler::Start();
	}

void CTestUAProf::SessionDisconnectL()
	{
	iSession.DisconnectL();
	CActiveScheduler::Start();
	}
