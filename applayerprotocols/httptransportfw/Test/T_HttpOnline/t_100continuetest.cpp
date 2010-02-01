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

CTestClient* CTestClient::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CTestClient* self = new(ELeave)CTestClient(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

CTestClient::~CTestClient()
	{
	}

CTestClient::CTestClient(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

void CTestClient::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	RStringPool strP = iSession.StringPool();
	TUriParser8 up;
	up.Parse(KHttpUrl);

	RHTTPTransaction trans = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	
	RHTTPHeaders hdr = trans.Request().GetHeaderCollection();

	RStringF expect = strP.StringF(HTTP::EExpect, RHTTPSession::GetTable());
	RStringF expectValStrF = strP.OpenFStringL(_L8("100-Continue"));
	CleanupClosePushL(expectValStrF);
	THTTPHdrVal expectVal;
	expectVal.SetStrF(expectValStrF);
	hdr.SetFieldL(expect, expectVal);
	CleanupStack::PopAndDestroy(&expectValStrF);

	RStringF ct = strP.StringF(HTTP::EContentType, RHTTPSession::GetTable());
	RStringF ctValStrF = strP.OpenFStringL(_L8("text/html"));
	CleanupClosePushL(ctValStrF);
	THTTPHdrVal ctVal;
	ctVal.SetStrF(ctValStrF);
	hdr.SetFieldL(ct, ctVal);
	CleanupStack::PopAndDestroy(&ctValStrF);

	RHTTPTransactionPropertySet propSet = trans.PropertySet();
	RStringF notifyContinue = strP.StringF(HTTP::ENotify100Continue, RHTTPSession::GetTable());
	RStringF enableNotification = strP.StringF(HTTP::EEnableNotification, RHTTPSession::GetTable());
	THTTPHdrVal val(enableNotification);
	propSet.SetPropertyL(notifyContinue, val);
	
	trans.Request().SetBody(*this);
	
	trans.SubmitL();
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	}


TInt CTestClient::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTestClient::DoCancel()
	{
	}

const TDesC& CTestClient::TestName()
	{
	_LIT(KHeaderTestName,"100-Continue Test: 1");
	return KHeaderTestName;
	}

TInt CTestClient::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTestClient::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
	
TBool CTestClient::GetNextDataPart(TPtrC8& aDataPart)
	{
	aDataPart.Set(KBody());	
	return ETrue;
	}

void CTestClient::ReleaseData()
	{
		
	}

TInt CTestClient::OverallDataSize()
	{
	return (12);
	}

TInt CTestClient::Reset()
	{
	return KErrNotSupported;	
	}


//
//100-Continue test with timeout properties set on transaction//	
//

CTestClient_5* CTestClient_5::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CTestClient_5* self = new(ELeave)CTestClient_5(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

CTestClient_5::~CTestClient_5()
	{
	}

CTestClient_5::CTestClient_5(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

void CTestClient_5::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	RStringPool strP = iSession.StringPool();
	TUriParser8 up;
	up.Parse(KHttpUrl);

	RHTTPTransaction trans = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	
	RHTTPHeaders hdr = trans.Request().GetHeaderCollection();

	RStringF expect = strP.StringF(HTTP::EExpect, RHTTPSession::GetTable());
	RStringF expectValStrF = strP.OpenFStringL(_L8("100-Continue"));
	CleanupClosePushL(expectValStrF);
	THTTPHdrVal expectVal;
	expectVal.SetStrF(expectValStrF);
	hdr.SetFieldL(expect, expectVal);
	CleanupStack::PopAndDestroy(&expectValStrF);

	RStringF ct = strP.StringF(HTTP::EContentType, RHTTPSession::GetTable());
	RStringF ctValStrF = strP.OpenFStringL(_L8("text/html"));
	CleanupClosePushL(ctValStrF);
	THTTPHdrVal ctVal;
	ctVal.SetStrF(ctValStrF);
	hdr.SetFieldL(ct, ctVal);
	CleanupStack::PopAndDestroy(&ctValStrF);

	RHTTPTransactionPropertySet propSet = trans.PropertySet();
	RStringF notifyContinue = strP.StringF(HTTP::ENotify100Continue, RHTTPSession::GetTable());
	RStringF enableNotification = strP.StringF(HTTP::EEnableNotification, RHTTPSession::GetTable());
	THTTPHdrVal val(enableNotification);
	propSet.SetPropertyL(notifyContinue, val);

	//Set TimeOut Values
	RStringF sendTimeOutProp = strP.StringF(HTTP::ESendTimeOutValue, RHTTPSession::GetTable());
	THTTPHdrVal sendTimeOutPropValue;
	sendTimeOutPropValue.SetInt(10);
	propSet.SetPropertyL(sendTimeOutProp, sendTimeOutPropValue);

	RStringF recvTimeOutProp = strP.StringF(HTTP::EReceiveTimeOutValue, RHTTPSession::GetTable());
	THTTPHdrVal recvTimeOutPropValue;
	recvTimeOutPropValue.SetInt(10);
	propSet.SetPropertyL(recvTimeOutProp, recvTimeOutPropValue);

	trans.Request().SetBody(*this);
	
	trans.SubmitL();
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession
	}


TInt CTestClient_5::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTestClient_5::DoCancel()
	{
	}

const TDesC& CTestClient_5::TestName()
	{
	_LIT(KHeaderTestName,"100-Continue Test: 5");
	return KHeaderTestName;
	}

TInt CTestClient_5::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CTestClient_5::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
	
TBool CTestClient_5::GetNextDataPart(TPtrC8& aDataPart)
	{
	aDataPart.Set(KBody());	
	return ETrue;
	}

void CTestClient_5::ReleaseData()
	{
		
	}

TInt CTestClient_5::OverallDataSize()
	{
	return (12);
	}

TInt CTestClient_5::Reset()
	{
	return KErrNotSupported;	
	}











	


