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

#include "crecvbufsizetest1.h"

// Literals used in the file
_LIT8(KUrl,		"http://www.google.co.in");
const TInt KRecvBufSize = 0;


CRecvBufSizeTest1* CRecvBufSizeTest1::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CRecvBufSizeTest1* self = new(ELeave)CRecvBufSizeTest1(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

CRecvBufSizeTest1::~CRecvBufSizeTest1()
	{
	}

CRecvBufSizeTest1::CRecvBufSizeTest1(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

void CRecvBufSizeTest1::DoRunL()
	{
	TUriParser8 up;
	up.Parse(KUrl());

	iSession.OpenL();
	CleanupClosePushL(iSession);

	RStringPool strP = iSession.StringPool();
	RStringF method = strP.StringF(HTTP::EGET, RHTTPSession::GetTable());
	RStringF recBufSizeStr = strP.StringF(HTTP::ERecvBufferSize, RHTTPSession::GetTable());
	THTTPHdrVal val(KRecvBufSize);
	
	iSession.ConnectionInfo().SetPropertyL(recBufSizeStr, val);	
	
	iTransaction = iSession.OpenTransactionL(up, *this, method);
	
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession

	}

TInt CRecvBufSizeTest1::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CRecvBufSizeTest1::DoCancel()
	{
	}

const TDesC& CRecvBufSizeTest1::TestName()
	{
	_LIT(KHeaderTestName,"CRecvBufSizeTest1");
	return KHeaderTestName;
	}

TInt CRecvBufSizeTest1::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CRecvBufSizeTest1::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			iEngine->Utils().LogIt(_L("<Got response headers>"));
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			iEngine->Utils().LogIt(_L("<Got a chunk of data>"));

			MHTTPDataSupplier* iRespBody = aTransaction.Response().Body();

			TPtrC8 bodyData;
			TBool lastChunk = iRespBody->GetNextDataPart( bodyData );

			iRespBody->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			iEngine->Utils().LogIt(_L("<Transaction succeeded>"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			}
			break;
		case THTTPEvent::EFailed:
			{
			iEngine->Utils().LogIt(_L("<Transaction failed>"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
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




