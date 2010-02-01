// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CINC110203.h"

// Literals used in the file
_LIT8(KHttpUrl,		"http://www.google.co.in");
_LIT8(KContentType,	"*/*");

CINC110203* CINC110203::NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile)
	{
	CINC110203* self = new(ELeave)CINC110203(aTestNumber, aIniSettingsFile);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

CINC110203::~CINC110203()
	{
	}

CINC110203::CINC110203(TInt aTestNumber, CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile),
	iTestNumber(aTestNumber)
	{
	}

void CINC110203::SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue)
	{
	RStringF valStr = iSession.StringPool().OpenFStringL(aHdrValue);
	THTTPHdrVal val(valStr);
	aHeaders.SetFieldL(iSession.StringPool().StringF(aHdrField,RHTTPSession::GetTable()), val);
	valStr.Close();
	}

void CINC110203::DoRunL()
	{
	TUriParser8 up;
	up.Parse(KHttpUrl);

	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);

	RStringPool strP = iSession.StringPool();

	// Open a GET transactions, specifying this object as the request body data supplier
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EGET,RHTTPSession::GetTable()));

	RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
	SetHeaderL(hdr, HTTP::EContentType, KContentType);
	
	iTransaction.SubmitL();
	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(&iSession); //close iSession

	}

TInt CINC110203::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CINC110203::DoCancel()
	{
	}

const TDesC& CINC110203::TestName()
	{
	_LIT(KHeaderTestName,"CINC110203");
	return KHeaderTestName;
	}

TInt CINC110203::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	_LIT(KLogDescription, "\nTest failed with error code %d on transaction ID=%d\n");
	iEngine->Utils().LogIt(KLogDescription, aError, aTransaction.Id());
	iFailureError = aError;
	CActiveScheduler::Stop();
	return KErrNone;
	}

void CINC110203::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received
			iEngine->Utils().LogIt(_L("<Got response headers>"));
			MHTTPDataSupplier* body = aTransaction.Response().Body ();
			if ( body )
				{
				body->ReleaseData ();
				}
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
			// Get the body data supplier
			MHTTPDataSupplier* iRespBody = aTransaction.Response().Body();
			// Append to the output file.
			TPtrC8 bodyData;
			TBool lastChunk = iRespBody->GetNextDataPart( bodyData );
			iRespBody->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			// The transaction succeeded
			iEngine->Utils().LogIt(_L("<Transaction succeeded>"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			}
			break;
		case THTTPEvent::EFailed:
			{
			// The transaction failed so fail the test
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

