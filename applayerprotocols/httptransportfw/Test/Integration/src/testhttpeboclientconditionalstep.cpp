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
// Contains implementation of CHttpEboClientStep class
// @internalAll
// 
//

// User Include
#include "testhttpeboclientconditionalstep.h"

// Standard headers used by default
_LIT8(KUserAgent, "TestHttpEboClientConditional");
_LIT8(KAccept, "*/*");
_LIT8(KHttpUrl1, "http://www.google.co.in/");
_LIT8(KHttpUrl2, "http://www.live.com/");
_LIT8(KHttpUrl3, "http://www.search.com/");
_LIT8(KHttpUrl4, "http://www.find.com/");

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CHttpEboClientConditionalStep::CHttpEboClientConditionalStep()
	{
	
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestHttpEboClientConditionalStep);
	}


/**
Destructor
@internalTechnology
@test
*/
CHttpEboClientConditionalStep::~CHttpEboClientConditionalStep()
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
void CHttpEboClientConditionalStep::StartClientL()
	{
	if (!GetBoolFromConfig(ConfigSection(), KDisableTransOpt, iDisableTransOpt))
		{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KDisableTransOpt);
		SetTestStepResult(EFail);
		return;
		}

	RStringPool strP = iSess.StringPool();
	RStringF method;
	
	// method
	method = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());		
	TBuf8<256> url8;
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
void CHttpEboClientConditionalStep::InvokeHttpMethodL(const TDesC8& /*aUri*/, RStringF aMethod)
	{
	TUriParser8 uri1; 
	uri1.Parse( KHttpUrl1 );
	
	// Opening transactions	
	RHTTPTransaction trans1 = iSess.OpenTransactionL(uri1, *this, aMethod);
	CleanupClosePushL(trans1);
	RHTTPHeaders hdr1 = trans1.Request().GetHeaderCollection();
	SetHeaderL(hdr1, HTTP::EUserAgent, KUserAgent);
	SetHeaderL(hdr1, HTTP::EAccept, KAccept);
		
	TUriParser8 uri2; 
	uri2.Parse( KHttpUrl2 );
	RHTTPTransaction trans2 = iSess.OpenTransactionL(uri2, *this, aMethod);
	CleanupClosePushL(trans2);
	RHTTPHeaders hdr2 = trans2.Request().GetHeaderCollection();
	SetHeaderL(hdr2, HTTP::EUserAgent, KUserAgent);
	SetHeaderL(hdr2, HTTP::EAccept, KAccept);
	
	TUriParser8 uri3; 
	uri3.Parse( KHttpUrl3 );
	RHTTPTransaction trans3 = iSess.OpenTransactionL(uri3, *this, aMethod);
	CleanupClosePushL(trans3);
	RHTTPHeaders hdr3 = trans3.Request().GetHeaderCollection();
	SetHeaderL(hdr3, HTTP::EUserAgent, KUserAgent);
	SetHeaderL(hdr3, HTTP::EAccept, KAccept);
	
	TUriParser8 uri4; 
	uri4.Parse( KHttpUrl4 );
	RHTTPTransaction trans4 = iSess.OpenTransactionL(uri4, *this, aMethod);
	CleanupClosePushL(trans4);
	RHTTPHeaders hdr4 = trans4.Request().GetHeaderCollection();
	SetHeaderL(hdr4, HTTP::EUserAgent, KUserAgent);
	SetHeaderL(hdr4, HTTP::EAccept, KAccept);
		
	if (iDisableTransOpt)
		{
		iSess.SetupHttpDataOptimiser(*this);
		THTTPHdrVal disableOptimiser(iSess.StringPool().StringF(HTTP::EDisableHTTPTransOptimiser,RHTTPSession::GetTable()));
		trans3.PropertySet().SetPropertyL(iSess.StringPool().StringF(HTTP::EHTTPTransOptimiser,RHTTPSession::GetTable()), disableOptimiser);
		}
	else
		{
		trans1.SetupHttpDataOptimiser(*this);
		trans2.SetupHttpDataOptimiser(*this);
		trans4.SetupHttpDataOptimiser(*this);
		}
	
	trans1.SubmitL();
	CActiveScheduler::Start();
		
	trans2.SubmitL();
	CActiveScheduler::Start();
		
	trans3.SubmitL();
	CActiveScheduler::Start();
		
	trans4.SubmitL();
	CActiveScheduler::Start();
	
	CleanupStack::Pop(&trans4);
	CleanupStack::Pop(&trans3);
	CleanupStack::Pop(&trans2);
	CleanupStack::Pop(&trans1);
	}

/**
This is the implementation of interface method MHttpDataOptimiser::MHFRunL
If there is a reponse body it will be copied to a file.
Just printing the fired events to the log file.
@internalTechnology 
@test
@param		aTransaction Transaction which is being processed currently
@param      THTTPEvent Event for which it is called
@return		None
*/	
void CHttpEboClientConditionalStep::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			INFO_PRINTF2(_L("Processing Transaction %d\n"), aTransaction.Id());
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
		case KErrHttpOptimiserFailsTrans:
			{
			INFO_PRINTF1(_L("Cancelling/Failing Transaction\n"));
			aTransaction.Fail(THTTPFilterHandle::EProtocolHandler);
			//CActiveScheduler::Stop();
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
This is the implementation of interface method MHttpDataOptimiser::MHFRunError
@internalTechnology 
@test
@param		aError  Error which has been raised.	
@param		aTransaction Transaction which is being processed currently
@param      THTTPEvent Event which it is being handled currently.
@return		None
*/
TInt CHttpEboClientConditionalStep::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	INFO_PRINTF2(_L("MHFRunError fired with error code %D\n"), aError);
	return KErrNone;
	}


/**
It enocodes the request data.
@internalTechnology 
@test
@param		aHttpData  Actual request.
@param		aEncodedData Encoded request.
@return		None
*/
void CHttpEboClientConditionalStep::EncodeL (const TDesC8& aHttpData, HBufC8* &aEncodedData)
	{
	INFO_PRINTF1(_L("######## CHttpEboClientStep::EncodeL ########\n"));
	aEncodedData =  aHttpData.AllocLC();
	CleanupStack::Pop();
	INFO_PRINTF1(_L("Encoded/Changed the request\n"));
	}

	
/**
It decodes the responsing data before parsing.
@internalTechnology 
@test
@param		aHttpData  Actual response.
@param		aEncodedData Decoded response.
@return		None
*/
void CHttpEboClientConditionalStep::DecodeL (const TDesC8& aData, HBufC8*& aHttpData, TBool& /*aTransFail*/)
	{
	INFO_PRINTF1(_L("######## CHttpEboClientStep::DecodeL ########\n"));
	aHttpData = aData.AllocLC();
	CleanupStack::Pop();	
	INFO_PRINTF1(_L("Sent the actual response as a decoded response\n"));
	}

