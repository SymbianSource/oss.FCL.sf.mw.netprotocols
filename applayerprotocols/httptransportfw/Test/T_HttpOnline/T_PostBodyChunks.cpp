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
// defines test class: CTextModeTestPostBodyChunks
// 
//

#include "T_PostBodyChunks.h"

// User include
#include "csrvaddrval.h"

const TInt KMilliSeconds = 1000000;

// Constants used in CTextModeTestPostBodyChunks test
_LIT8(KHttpPostBodyChunksUrl,	"http://WapTestIP/perl/dumpform.pl");
_LIT8(KHttpPostBodyChunk1, 		"abc=def&ghi=jkl&");
_LIT8(KHttpPostBodyChunk2, 		"mno=pqr&stu=vwx&");
_LIT8(KHttpPostBodyChunk3, 		"y=z");

// CTextModeTestPostBodyChunks-----------------------------------

/**
	This function is called to create a new instance of the class 
	CTextModeTestCookies

	@param aIniSettingsFile	  The script file that holds the sections
							  from which the value has to be retrieved
	@leave 					  KErrNoMemory if there is insufficient memory
*/
CTextModeTestPostBodyChunks* CTextModeTestPostBodyChunks::NewL(CScriptFile* aIniSettingsFile)
	{
	CTextModeTestPostBodyChunks* self = new(ELeave) CTextModeTestPostBodyChunks(aIniSettingsFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

/**
	Destructor
*/
CTextModeTestPostBodyChunks::~CTextModeTestPostBodyChunks()
	{
	}

void CTextModeTestPostBodyChunks::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received
			//iEngine->Utils().LogIt(_L("<Got response headers>"));
			DumpRespHeaders(aTransaction);
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			//iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
			DumpResponseBody(aTransaction);
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			iEngine->Utils().LogIt(_L("<Transaction validated successfully>"));
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EFailed:
			{
			iEngine->Utils().LogIt(
				_L("<Transaction NOT validated successfully>"));
			iFailureError = KErrTestFailed;
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EUnrecoverableError:
			{
			iEngine->Utils().LogIt(_L("<Test failed unrecoverably>"));
			iFailureError = KErrTestFailed;
			} break;
		default:
			{
			iEngine->Utils().LogIt(_L("<unrecognised event> %d"), aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				iFailureError = aEvent.iStatus;

				// Special case for doing heap-failure testing only - in some 
				// failure circumstances, the outstanding socket read/writes 
				// complete with a time out
				if ((iLeakTests > 0) && (iFailureError == KErrTimedOut))
					{
					iFailureError = KErrTestFailed;
					}
					
				CActiveScheduler::Stop();
				}
			} 
			break;
		}
	}

TInt CTextModeTestPostBodyChunks::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	iEngine->Utils().LogIt(
		_L("\nMHFRunL left: error code %d on transaction ID=%d for event %d\n"),
		aError, aTransaction.Id(), aEvent.iStatus);
	return KErrNone;
	}

/**
	This function is called to get the next chunk of data.

	@param aDataChunk	The pointer which holds the next data chunk.
*/
TBool CTextModeTestPostBodyChunks::GetNextDataPart(TPtrC8& aDataChunk)
	{
	TBool retVal = EFalse;
	switch (iReqBodyChunkCount)
		{
		case 0:
			{
			aDataChunk.Set(KHttpPostBodyChunk1());
			} break;
		case 1:
			{
			aDataChunk.Set(KHttpPostBodyChunk2());
			} break;
		case 2:
			{
			aDataChunk.Set(KHttpPostBodyChunk3());
			retVal = ETrue;
			} break;
		}
	iReqBodyChunkCount++;
	return retVal;
	}

/**
	Set timer to fire when we want to signal the availability of the next 
	chunk, provided we've not finished
*/
void CTextModeTestPostBodyChunks::ReleaseData()

	{
	if (iReqBodyChunkCount < 3)
		{
		iReleaseTimer->After(KMilliSeconds);
		}
	}

TInt CTextModeTestPostBodyChunks::OverallDataSize()
	{
	return (KHttpPostBodyChunk1().Length() + KHttpPostBodyChunk2().Length() + KHttpPostBodyChunk3().Length());
	}

const TDesC& CTextModeTestPostBodyChunks::TestName()
	{
	_LIT(KHeaderTestName,"CTextModeTestPostBodyChunks");
	return KHeaderTestName;
	}

/**
	This function is called to initiate the tests.
	It may leave with one of the system-wide error codes.
*/
void CTextModeTestPostBodyChunks::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	RStringPool strP = iSession.StringPool();

	// Open a POST transactions, specifying this object as the request body 
	// data supplier
	TUriParser8 up;

	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KHttpPostBodyChunksUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	up.Parse(newUrlPtr8);
	
	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	RHTTPRequest rq = iTransaction.Request();
	rq.SetBody(*this);
	RHTTPHeaders hdr = rq.GetHeaderCollection();
	THTTPHdrVal length(OverallDataSize());
	hdr.SetFieldL(strP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()), length);
	THTTPHdrVal contType(strP.StringF(HTTP::EApplicationXWwwFormUrlEncoded, RHTTPSession::GetTable()));
	hdr.SetFieldL(strP.StringF(HTTP::EContentType, RHTTPSession::GetTable()), contType);
	
	// Create the delayed notifier
	iReleaseTimer = CDelayedBodyDataNotifier::NewL(iTransaction);
	CleanupStack::PushL(iReleaseTimer);
	CActiveScheduler::Add(iReleaseTimer);

	// Submit the transaction
	iTransaction.SubmitL();
	iFailureError = KErrNone;
	CActiveScheduler::Start();

	// Get the completion code and inform the engine.  Anything other than 
	// HTTP/200 status is a failure for this test.
	iEngine->SetCurrentStatusCode(iTransaction.Response().StatusCode());
	iExpectedStatusCode = 200;

	// Close anything opened
	CleanupStack::PopAndDestroy(3, &iSession); // and iReleaseTimer, newUrl8;

	// Check for failure error codes caught in MHFRunL
	User::LeaveIfError(iFailureError);
	}

TInt CTextModeTestPostBodyChunks::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}
	
void CTextModeTestPostBodyChunks::DoCancel()
	{
	iReleaseTimer->Cancel();
	}

CTextModeTestPostBodyChunks::CTextModeTestPostBodyChunks(CScriptFile* aIniSettingsFile) :
	iIniSettingsFile(aIniSettingsFile)
	{
	}

/**
	This constructor is called in the NewL function and used by the class to
	construct self.  Currently this has no functionality.  In future this 
	constructor can be used.
*/
void CTextModeTestPostBodyChunks::ConstructL()
	{
	}

CTextModeTestPostBodyChunks::CDelayedBodyDataNotifier*
CTextModeTestPostBodyChunks::CDelayedBodyDataNotifier::NewL(RHTTPTransaction aTrans)
	{
	CDelayedBodyDataNotifier* self = new(ELeave) CDelayedBodyDataNotifier(aTrans);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTextModeTestPostBodyChunks::CDelayedBodyDataNotifier::CDelayedBodyDataNotifier(RHTTPTransaction aTrans)
	: 	CTimer(EPriorityStandard), iTrans(aTrans)
	{
	}

void CTextModeTestPostBodyChunks::CDelayedBodyDataNotifier::RunL()
	{
	iTrans.NotifyNewRequestBodyPartL();
	}

