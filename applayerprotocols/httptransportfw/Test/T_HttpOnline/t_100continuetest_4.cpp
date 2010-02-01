// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "t_100continuetest.h"

CTestClient_4* CTestClient_4::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CTestClient_4* self = new(ELeave)CTestClient_4(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

CTestClient_4::~CTestClient_4()
	{
	}

CTestClient_4::CTestClient_4(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

void CTestClient_4::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	RStringPool strP = iSession.StringPool();
	TUriParser8 up;
	up.Parse(KHttpUrl);

	iTrans = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	
	RHTTPHeaders hdr = iTrans.Request().GetHeaderCollection();
	RStringF ct = strP.StringF(HTTP::EContentType, RHTTPSession::GetTable());
	RStringF ctValStrF = strP.OpenFStringL(_L8("text/html"));
	CleanupClosePushL(ctValStrF);
	THTTPHdrVal ctVal;
	ctVal.SetStrF(ctValStrF);
	hdr.SetFieldL(ct, ctVal);
	CleanupStack::PopAndDestroy(&ctValStrF);
	
	iTrans.Request().SetBody(*this);
	iTrans.SubmitL();
	
	CHttpTimer* timer = new(ELeave) CHttpTimer(*this);
	TTimeIntervalMicroSeconds32 time(10000000);
	timer->After(time);

	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	delete timer;
	}


TInt CTestClient_4::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTestClient_4::DoCancel()
	{
	}

const TDesC& CTestClient_4::TestName()
	{
	_LIT(KHeaderTestName,"100-Continue Test: 4");
	return KHeaderTestName;
	}

TInt CTestClient_4::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTestClient_4::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			MHTTPDataSupplier* data = aTransaction.Response().Body();
			data->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			} break;
		case THTTPEvent::ESucceeded:
			{
			aTransaction.Close();
			CActiveScheduler::Stop();
			}
			break;
		case THTTPEvent::EFailed:
			{
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EReceived100Continue:
			{
			iEngine->Utils().LogIt(_L("Recevied 100-Continue response notification"));	
			}break;
		default:
			{
			_LIT(KLogDescription, "<unrecognised event> %d");
			iEngine->Utils().LogIt(KLogDescription,aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				iFailureError = aEvent.iStatus;
				CActiveScheduler::Stop();
				}
			} 
			break;
		}
	
	return;
	}
	
TBool CTestClient_4::GetNextDataPart(TPtrC8& aDataPart)
	{
	aDataPart.Set(KBody());	
	return ETrue;
	}

void CTestClient_4::ReleaseData()
	{
		
	}

TInt CTestClient_4::OverallDataSize()
	{
	return (12);
	}

TInt CTestClient_4::Reset()
	{
	return KErrNotSupported;	
	}
	


