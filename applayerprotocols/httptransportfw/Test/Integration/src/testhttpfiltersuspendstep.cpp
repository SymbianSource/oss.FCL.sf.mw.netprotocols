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
// Contains implementation of CTestHttpBufferSizeStep class
// @internalAll
// 
//

// System Include
// for StartC32()
#include <c32comm.h>
#include <e32base.h>
#include <http.h>

// User Include
#include "testhttpfiltersuspendstep.h"


/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestHttpFilterSuspendStep::CTestHttpFilterSuspendStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestHttpFilterSuspendStep); 
	//Set the iChunkSize to the default buffer size of 6144 Bytes	
	}


/**
Destructor: Closes the iFileServ.
@internalTechnology
@test
*/
CTestHttpFilterSuspendStep::~CTestHttpFilterSuspendStep()
	{
	}
	

/**
Sets the proxy and HttpDataOptimiser implemetation to session
based on the patameter values in ini file and calls InvokeHttpMethodL()
@internalTechnology
@test
@param		None
@return		None
*/
void CTestHttpFilterSuspendStep::StartClientL()
	{
	TPtrC uri;
	
    if (!GetStringFromConfig(ConfigSection(), KIniUri, uri))
		{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniUri);
		SetTestStepResult(EFail);
		return;
		}	

	RHTTPConnectionInfo httpConnInfo = iSess.ConnectionInfo();	
	
	RStringPool strP = iSess.StringPool();
	RStringF method;
	
	// method
	method = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());		
	TBuf8<256> url8;
	url8.Copy(uri);
	TRAPD(err,InvokeHttpMethodL(url8, method));
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Teststep Failed: Leave occured in CTestHttpBufferSizeStep::InvokeHttpMethodL: %D\n"
					  ),err
				   );
		SetTestStepResult(EFail);
		}
	method.Close(); 		
	}


/**
Invoke the http method
This actually creates the transaction, sets the headers and HttpDataOptimiser implemetation to transaction
and then starts the transaction 
@internalTechnology
@test
@param		aUri 		Requested uri
@param		aMethod 	Requested method
@return		None		
*/
void CTestHttpFilterSuspendStep::InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod)
	{
	TBool failTransaction = EFalse;
	if(!GetBoolFromConfig(ConfigSection(), KIniFailTransaction, failTransaction))
	  	{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniFailTransaction
				   );
		SetTestStepResult(EFail);
		return;
		}			
	TUriParser8 uri; 
	uri.Parse( aUri );	
	
	CTestFilter* filter1 = CTestFilter::NewL(1);
	iSess.FilterCollection().AddFilterL(*filter1, THTTPEvent(THTTPEvent::ESubmit),
				 RStringF(), KAnyStatusCode, 1, RStringF());					 		
				 
	CTestFilter1* filter2 = CTestFilter1::NewL(2);
	CleanupStack::PushL(filter2);
	iSess.FilterCollection().AddFilterL(*filter2, THTTPEvent(22),
				 RStringF(), KAnyStatusCode, 2, RStringF());
	CleanupStack::Pop(filter2);				 
	
	CTestFilter1* filter3 = CTestFilter1::NewL(3);
	CleanupStack::PushL(filter3);
	iSess.FilterCollection().AddFilterL(*filter3, THTTPEvent(33),
				 RStringF(), KAnyStatusCode, 3, RStringF());
	CleanupStack::Pop(filter3);				 
	
				 
	CTestFilter* filter4 = CTestFilter::NewL(4);
	iSess.FilterCollection().AddFilterL(*filter4, THTTPEvent(22),
				 RStringF(), KAnyStatusCode, 4, RStringF());

	// Opening a transaction	
	iTrans = iSess.OpenTransactionL(uri, *this, aMethod);

	// submit the transaction
	if(failTransaction)
		{
		// Sending EResume event to test that transaction failing with error code InvalidEvent
		iTrans.SendEventL(THTTPEvent(THTTPEvent::EResume), THTTPEvent::EOutgoing,
				 THTTPFilterHandle(THTTPFilterHandle::EClient));
		}
	else
		{
		iTrans.SubmitL();
		}		
	// Start the scheduler, once the transaction completes or is cancelled on an error the scheduler will be
	// stopped in the event handler
	CActiveScheduler::Start();
	}


/**
This is the implementation of interface method CTestHttpBufferSizeStep::MHFRunL
If there is a reponse body it will be copied to a file.
Just printing the fired events to the log file.
@internalTechnology 
@test
@param		aTransaction Transaction which is being processed currently
@param      THTTPEvent Event for which it is called
@return		None
*/	
void CTestHttpFilterSuspendStep::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received. We can determine now if there is
			// going to be a response body to save.
			RHTTPResponse resp = aTransaction.Response();
			TInt status = resp.StatusCode();
			RStringF statusStr = resp.StatusText();
			TBuf<32> statusStr16;
			statusStr16.Copy(statusStr.DesC());
			INFO_PRINTF3(_L("Status: %D (%S)\n"),status, &statusStr16);

			if (resp.HasBody() && (status >= 200) && (status < 300) && (status != 204))
				{
				TInt dataSize = resp.Body()->OverallDataSize();
				if (dataSize >= 0)
					INFO_PRINTF2(_L("Response body size is %d\n"), dataSize);
				else
					INFO_PRINTF1(_L("Response body size is unknown\n"));
				}
			} break;
		case THTTPEvent::EGotResponseBodyData:			
			{
			// Get the body data supplier
			iRespBody = aTransaction.Response().Body();
			INFO_PRINTF1(_L("Receiving response body...\n"));
			
			TPtrC8 bodyData;
			TBool lastChunk = iRespBody->GetNextDataPart(bodyData);
			if(bodyData.Size() > iChunkSize)
				{
				iChunkSize = bodyData.Size();
				}
			iRespBody->ReleaseData();
			} break;		
			
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			INFO_PRINTF1(_L("\nTransaction Complete\n"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			INFO_PRINTF1(_L("Transaction Successful\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EFailed:
			{
			INFO_PRINTF1(_L("Transaction Failed\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::ERedirectedPermanently:
			{
			INFO_PRINTF1(_L("Permanent Redirection\n"));
			} break;
		case THTTPEvent::ERedirectedTemporarily:
			{
			INFO_PRINTF1(_L("Temporary Redirection\n"));
			} break;
		case THTTPEvent::ERedirectRequiresConfirmation:
 			{
			// 301(Moved Permanently), 302(Found) or 307(Temporary Redirect) status is received 
			// from a transaction and hence ERedirectRequiresConfirmation is sent by filter
			// client has opted to close the transaction
			INFO_PRINTF1(_L("Redirect requires confirmation\n"));
 			aTransaction.Close();
 			CActiveScheduler::Stop();
 			} break;
		default:
			{
			INFO_PRINTF2(_L("<unrecognised event: %D>\n"), aEvent.iStatus);
			// close off the transaction if it's an error
			if (aEvent.iStatus < 0)
				{
				aTransaction.Close();
				CActiveScheduler::Stop();
				}
			} break;
		}
	}


/**
This is the implementation of interface method CTestHttpBufferSizeStep::MHFRunError
@internalTechnology 
@test
@param		aError  Error which has been raised.	
@param		aTransaction Transaction which is being processed currently
@param      THTTPEvent Event which it is being handled currently.
@return		None
*/
TInt CTestHttpFilterSuspendStep::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	INFO_PRINTF2(_L("MHFRunError fired with error code %D\n"), aError);
	return KErrNone;
	}
CTestFilter1* CTestFilter1::NewL(TInt aIndex)
	{
	return new (ELeave) CTestFilter1(aIndex);
	}

void CTestFilter1::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	}

void CTestFilter1::MHFUnload(RHTTPSession, THTTPFilterHandle)
	{
	delete iUtils;
	delete this;
	}
void CTestFilter1::MHFRunL(RHTTPTransaction /*aTransaction*/,
						const THTTPEvent& aEvent)
	{
		
	switch(aEvent.iStatus)
		{
		case 22:		
			{			
			iUtils->LogIt(_L("Handeling Event 22...CTestFilter1"));	
			}
			break;
		case 33:
			{
			iUtils->LogIt(_L("Handeling Event 33...CTestFilter1"));				
			}	
			break;
		}
		
	}

void CTestFilter1::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CTestFilter1::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

TInt CTestFilter1::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}
	
CTestFilter* CTestFilter::NewL(TInt aIndex)
	{
	return new (ELeave) CTestFilter(aIndex);
	}

void CTestFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	}

void CTestFilter::MHFUnload(RHTTPSession, THTTPFilterHandle)
	{
	delete iTestUtils;
	delete this;
	}
void CTestFilter::RunL()
	{
	iTestUtils->LogIt(_L("Transaction Resume..."));							
    iTrans.SendEventL(THTTPEvent(THTTPEvent::EResume), THTTPEvent::EIncoming,
								THTTPFilterHandle(
									THTTPFilterHandle::ECurrentFilter));	
	
	}
void CTestFilter::MHFRunL(RHTTPTransaction aTransaction,
						const THTTPEvent& aEvent)
	{
		
	switch(aEvent.iStatus)
		{
		case THTTPEvent::ESubmit:		
			{
			iTestUtils->LogIt(_L("Handeling Event ESubmit..."));					
			iTestUtils->LogIt(_L("Transaction Suspended..."));	
			iTrans = aTransaction;			
			aTransaction.SendEventL(THTTPEvent(THTTPEvent::ESuspend), THTTPEvent::EIncoming,
								THTTPFilterHandle(
									THTTPFilterHandle::ECurrentFilter));
			aTransaction.SendEventL(THTTPEvent(22), THTTPEvent::EOutgoing,
								THTTPFilterHandle(
									THTTPFilterHandle::EClient));
			aTransaction.SendEventL(THTTPEvent(33), THTTPEvent::EIncoming,
								THTTPFilterHandle(
									THTTPFilterHandle::ECurrentFilter));		
    		CTimer::After(5000000);  // wait 5 second	    	
			}
			break;
		case 22:			
			iTestUtils->LogIt(_L("Handeling Event 22... CTestFilter"));					
			break;
		case 33:
			__ASSERT_DEBUG((33), HTTPPanic::Panic(HTTPPanic::EInvalidEvent));
			break;
		}
		
	}

void CTestFilter::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{
	}

TInt CTestFilter::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

TInt CTestFilter::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}	
