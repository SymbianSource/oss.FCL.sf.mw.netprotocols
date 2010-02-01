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
#include "testhttpbuffersizestep.h"

// Standard headers used by default
_LIT8(KUserAgent, "TestHttpBufferSize");
_LIT8(KAccept, "*/*");


/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CTestHttpBufferSizeStep::CTestHttpBufferSizeStep()
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestHttpBufferSizeStep); 
	//Set the iChunkSize to the default buffer size of 6144 Bytes
	iChunkSize = 1024;
	}


/**
Destructor: Closes the iFileServ.
@internalTechnology
@test
*/
CTestHttpBufferSizeStep::~CTestHttpBufferSizeStep()
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
void CTestHttpBufferSizeStep::StartClientL()
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

	THTTPHdrVal hdrVal ( 1024 );
	RHTTPConnectionInfo httpConnInfo = iSess.ConnectionInfo();
	httpConnInfo.SetPropertyL ( iSess.StringPool().StringF(HTTP::ERecvBufferSize,RHTTPSession::GetTable()), hdrVal );
	INFO_PRINTF1(_L("Receive buffer size has been set to 1024 bytes\n"));
	
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
	ValidateTest();
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
void CTestHttpBufferSizeStep::InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod)
	{

	TUriParser8 uri; 
	uri.Parse( aUri );
	
	// Opening a transaction	
	iTrans = iSess.OpenTransactionL(uri, *this, aMethod);
	RHTTPHeaders hdr = iTrans.Request().GetHeaderCollection();
	// Add headers appropriate to all methods
	SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
	SetHeaderL(hdr, HTTP::EAccept, KAccept);

	// submit the transaction
	iTrans.SubmitL();
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
void CTestHttpBufferSizeStep::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
TInt CTestHttpBufferSizeStep::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	INFO_PRINTF2(_L("MHFRunError fired with error code %D\n"), aError);
	return KErrNone;
	}

void CTestHttpBufferSizeStep::ValidateTest()
	{
	if(iChunkSize <= 1024)
		{
		ERR_PRINTF1(_L("Teststep Passed: Buffer Size Feature is working\n"));
		}
	else
		{
		ERR_PRINTF2(_L("Received a chunk of size: %D\n"), iChunkSize);
		ERR_PRINTF1(_L("Received a chunk of size greater than the specified size of 1024 Bytes.\n"));
		ERR_PRINTF1(_L("Teststep Failed: Buffer Size Feature is not working\n"));
		SetTestStepResult(EFail);
		}
	}



