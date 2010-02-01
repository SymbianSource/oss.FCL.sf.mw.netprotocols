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

CTestClient_3* CTestClient_3::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CTestClient_3* self = new(ELeave)CTestClient_3(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

CTestClient_3::~CTestClient_3()
	{
	}

CTestClient_3::CTestClient_3(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

void CTestClient_3::DoRunL()
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
	
	RHTTPTransactionPropertySet propSet = iTrans.PropertySet();
	RStringF notifyContinue = strP.StringF(HTTP::ENotify100Continue, RHTTPSession::GetTable());
	RStringF enableNotification = strP.StringF(HTTP::EEnableNotification, RHTTPSession::GetTable());
	THTTPHdrVal val(enableNotification);
	propSet.SetPropertyL(notifyContinue, val);
	
	iTrans.Request().SetBody(*this);
	iTrans.SubmitL();
	
	CHttpTimer* timer = new(ELeave) CHttpTimer(*this);
	TTimeIntervalMicroSeconds32 time(10000000);
	timer->After(time);

	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	delete timer;
	}


TInt CTestClient_3::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTestClient_3::DoCancel()
	{
	}

const TDesC& CTestClient_3::TestName()
	{
	_LIT(KHeaderTestName,"100-Continue Test: 3");
	return KHeaderTestName;
	}

TInt CTestClient_3::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTestClient_3::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
	
CHttpTimer::CHttpTimer(MTestClient& aClient)
:CActive(EPriorityStandard), iClient(aClient)
	{
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
	}

CHttpTimer::~CHttpTimer()
	{
	Cancel();
	iTimer.Close();	
	}
	
void CHttpTimer::After(TTimeIntervalMicroSeconds32 anInterval)
	{
	iTimer.After(iStatus, anInterval);	
	SetActive();
	}

void CHttpTimer::DoCancel()
	{
	
	}
	
void CHttpTimer::RunL()
	{
	iClient.iTrans.SendEventL(THTTPEvent::ECancelWaitFor100Continue, THTTPEvent::EOutgoing, THTTPFilterHandle::EClient);
	}	

TBool CTestClient_3::GetNextDataPart(TPtrC8& aDataPart)
	{
	aDataPart.Set(KBody());	
	return ETrue;
	}

void CTestClient_3::ReleaseData()
	{
		
	}

TInt CTestClient_3::OverallDataSize()
	{
	return (12);
	}

TInt CTestClient_3::Reset()
	{
	return KErrNotSupported;	
	}
	


