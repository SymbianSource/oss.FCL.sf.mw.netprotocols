// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "CHttpAccepTtestBase.h"
#include <http.h>

const TInt KNothingExpected = 0;


CHttpAcceptTestBase::CHttpAcceptTestBase()
	{
	iExpectedStatusCode=KNothingExpected;
	iExpectedError=KNothingExpected;
	

	}

void CHttpAcceptTestBase::DoRunL()
	{
	iOpenTransactionCount = 0;
	CleanupStack::PushL ( TCleanupItem ( CHttpAcceptTestBase::Cleanup, this ) );
	OpenTestSessionL(); // set up session and transaction here
	
	iTransaction.SubmitL();
	CleanupStack::Pop (); 
	CActiveScheduler::Start();
	CloseTestSession(); // close any strings, transaction and session
	if (iTestFail==1)
		{
		User::Leave(KErrNoMemory);
		}
	}
	
TInt CHttpAcceptTestBase::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpAcceptTestBase::DoCancel()
	{}

CHttpAcceptTestBase::~CHttpAcceptTestBase()
	{}	

void CHttpAcceptTestBase::Cleanup ( TAny* aPtr )
	{
	CHttpAcceptTestBase& httpTestBase = *( CHttpAcceptTestBase* )aPtr;
 	httpTestBase.CloseTestSession ();
	}


/** set this object active */
void CHttpAcceptTestBase::CompleteOwnRequest()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);	
	if (!IsActive())
		SetActive();
	}


TInt CHttpAcceptTestBase::MHFRunError(TInt aError, 
										RHTTPTransaction /*aTransaction*/,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}


// validates a response against all expected outcomes
void CHttpAcceptTestBase::Validate(RHTTPTransaction aTransaction)
	{
	if (iExpectedNumberRedirects)
		{
		THTTPHdrVal hdr;
		TBool hasValue;
		RStringPool strP = aTransaction.Session().StringPool();
		hasValue = aTransaction.PropertySet().Property(strP.StringF(HTTP::ERedirectCount,RHTTPSession::GetTable()), hdr);
		if (hasValue && hdr.Int() != iExpectedNumberRedirects)
			{
			iTestFail = 1;
			iEngine->Utils().LogIt(_L("<Number of Redirects don't match expected number>\n"));
			}
		}
	}

void CHttpAcceptTestBase::MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent)
	{
	if (aEvent.iStatus < 0)
		{
		if (aEvent.iStatus == iExpectedError)
			{
			iEngine->Utils().LogIt(_L("<The error event %d was expected>\n"), aEvent.iStatus);
			iExpectError = ETrue;
			}
		else 
			{
			iTestFail = 1;
			iEngine->Utils().LogIt(_L("<The error event %d was not expected>\n"), aEvent.iStatus);
			}
			if ( aEvent.iStatus != KErrNoMemory )			
		return;
		}

	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<EGotResponseHeaders (%d)\n"),aEvent.iStatus);
		
		TInt  statusCode = aTransaction.Response().StatusCode();
		iEngine->Utils().LogIt(_L("Status Code: %d \n"),statusCode);
		DumpRespHeaders(aTransaction);
		iEngine->SetCurrentStatusCode(aTransaction.Response().StatusCode());
		if (iExpectedStatusCode !=KNothingExpected && iExpectedStatusCode != aTransaction.Response().StatusCode() &&
			iExpectedStatusCode < THTTPEvent::ERedirectedPermanently)
			{
			iTestFail= ETrue;
			iEngine->Utils().LogIt(_L("<Unexpected status code: %d, Expected was %d>\n"), statusCode,iExpectedStatusCode);
			}
		else
			{
			iTestFail = EFalse;
			}
 		// To verify ETransferEncoding header field is removed from the response header
 		// on receiving the value of EChunked in it
 		THTTPHdrVal tmpVal;
 		RStringPool strP = aTransaction.Session().StringPool();
 			
 		if(aTransaction.Response().GetHeaderCollection().GetField(strP.StringF(
 			HTTP::ETransferEncoding,RHTTPSession::GetTable()),0,tmpVal) == KErrNone)
 			{
 			if(tmpVal.StrF().Index(RHTTPSession::GetTable()) == HTTP::EChunked ) 
				{
 				iEngine->Utils().LogIt(_L("EChunked Detected in Header. Hence the Test Fails"));
				iTestFail= ETrue;
				}
 			}
 		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<EGotResponseBodyData received (%d)>\n"),aEvent.iStatus);
		// for each chunk of data received we have to empty the buffer before to be able to receive 
		MHTTPDataSupplier* body = aTransaction.Response().Body();
		body->ReleaseData();
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<EResponseComplete received (%d)>\n"),aEvent.iStatus);
		} break;
	case THTTPEvent::ESucceeded:
		{
		if (iExpectedError < 0 && (!iExpectError))
			{
			iEngine->Utils().LogIt(_L("<Expected Error Code Not Recieved (%d)>\n"),iExpectedError);
			iTestFail = ETrue;
			}
		else
			{
			iTestFail = EFalse;
			}
		iEngine->Utils().LogIt(_L("<ESucceeded received from the VF (%d)>\n"),aEvent.iStatus);
		if (--iOpenTransactionCount == 0)	
			CActiveScheduler::Stop();
		} break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<EFailed received from the VF (%d)>\n"),aEvent.iStatus);
		Validate(aTransaction);
		if (--iOpenTransactionCount == 0)
			CActiveScheduler::Stop();
		} break;
	case KErrHttpRequestHasBody:
		iEngine->Utils().LogIt(_L("<KErrHttpRequestHasBody  received from the VF (%d)>\n"),aEvent.iStatus);
		break;
	case KErrHttpEntityHeaderMissingContentType:
		iEngine->Utils().LogIt(_L("<KErrHttpEntityHeaderMissingContentType  received from the VF (%d)>\n"),aEvent.iStatus);
		break;
	case KErrHttpRequestBodyMissing:
		{
		iEngine->Utils().LogIt(_L("<KErrHttpRequestBodyMissing  received from the VF(%d)>\n"),aEvent.iStatus);
		} break;
	case KErrTimedOut:							//-33
		{
		iEngine->Utils().LogIt(_L("<KErrTimedOut  received from the VF (%d)>\n"),aEvent.iStatus);
		if ((iExpectedError != 0) && (iExpectedError != KErrTimedOut))	
			{
			iTestFail = 1;
			iEngine->Utils().LogIt(_L("<The event received will cause a failure (%d)>\n"),aEvent.iStatus);
			}
		} break;
	case KErrHttpRedirectExceededLimit:			
		{
		iEngine->Utils().LogIt(_L("<KErrHttpRedirectExceededLimit received (%d)>\n"),aEvent.iStatus);
		if ((iExpectedError != 0) && (iExpectedError != KErrHttpRedirectExceededLimit))	
			{
			iTestFail = 1;
			iEngine->Utils().LogIt(_L("<The event received will cause a failure>\n"));
			}
		} break;
	case KErrHttpRedirectNoLocationField:	
		{
		iEngine->Utils().LogIt(_L("<KErrHttpRedirectNoLocationField received (%d)>\n"),aEvent.iStatus);
		if ((iExpectedError != 0) && (iExpectedError != KErrHttpRedirectNoLocationField))
			{
			iTestFail = 1;
			iEngine->Utils().LogIt(_L("<The event received will cause a failure>\n"));
			}
		} break;

	// 301( Moved Permanently) or 302(Found) or 307(Temporary Redirect) status code is 
	// obtained for a transaction with a method other than GET or HEAD and hence this event
	// has been sent by the filter.
	case THTTPEvent::ERedirectRequiresConfirmation:			
 		{
 		iEngine->Utils().LogIt(_L("<Redirect requires confirmation (%d)>\n"),aEvent.iStatus);
 		// Client has opted to Submit the transaction
		if(iExpectedStatusCode==200)
 			aTransaction.SubmitL();
 		else
		// client has opted to close the transaction
 			{
 			if (--iOpenTransactionCount == 0)
 				CActiveScheduler::Stop();
 			}
 		} break;
	case THTTPEvent::EGotResponseTrailerHeaders:
		{
		iEngine->Utils().LogIt(_L("<EGotResponseTrailerHeaders received (%d)>\n"),aEvent.iStatus);
		}
		break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event>\n %d"),aEvent.iStatus);
		if (aEvent.iStatus < THTTPEvent::ERedirectedPermanently)
			{
			iTestFail=1;
			iEngine->Utils().LogIt(_L("Test Failed: The event received was not expected and it's not just a warning\n"));
			iEngine->PressAnyKey();
			}
		} 
		break;
		}
	}
