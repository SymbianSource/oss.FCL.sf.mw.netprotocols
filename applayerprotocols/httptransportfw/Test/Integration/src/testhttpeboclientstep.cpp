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
#include "testhttpeboclientstep.h"

// Standard headers used by default
_LIT8(KUserAgent, "TestHttpEboClient");
_LIT8(KAccept, "*/*");

/**
Constructor: Sets the test step name.
@internalTechnology
@test
*/
CHttpEboClientStep::CHttpEboClientStep()
	{
	
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestHttpEboClientStep);
	}


/**
Destructor
@internalTechnology
@test
*/
CHttpEboClientStep::~CHttpEboClientStep()
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
void CHttpEboClientStep::StartClientL()
	{
	TPtrC uri;
	TPtrC proxyPtr;
	TBool proxyUsage;
	TPtrC resbodyFilename;
	TBool sessHttpOptUsage;
    if (!GetStringFromConfig(ConfigSection(), KIniUri, uri) ||
        !GetStringFromConfig(ConfigSection(), KIniFileName, resbodyFilename) ||
	    !GetBoolFromConfig(ConfigSection(), KIniSessProxyUsage, proxyUsage) 
	   )
		{
		ERR_PRINTF4(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n"
					  ),&KIniUri, &KIniFileName, &KIniSessProxyUsage
				   );
		SetTestStepResult(EFail);
		return;
		}
	iOutputFilename.Set(resbodyFilename);
	if (proxyUsage)
		if(!GetStringFromConfig(ConfigSection(), KIniSessProxy, proxyPtr)
	      )
			{
			ERR_PRINTF2(_L("Problem in reading values from ini.			\
							\nExpected field is: \n%S\n"
						  ),&KIniSessProxy
					   );
			SetTestStepResult(EFail);
			return;
			}
	RStringPool strP = iSess.StringPool();
	RStringF method;
	//setting proxy to session
	if(proxyUsage)
		{
		iSess.ConnectionInfo().SetPropertyL(iSess.StringPool().StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()), iSess.StringPool().StringF(HTTP::EUseProxy,RHTTPSession::GetTable()));
		TBuf8<256> prox8;
		prox8.Copy(proxyPtr);
		RStringF proxy = iSess.StringPool().OpenFStringL(prox8);
		CleanupClosePushL(proxy);
		iSess.ConnectionInfo().SetPropertyL(iSess.StringPool().StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()), proxy);
		CleanupStack::PopAndDestroy(&proxy);
		}
	// Setting HttpDataOptimiser to session
	if (!GetBoolFromConfig(ConfigSection(), KIniSessHttpOptUsage, sessHttpOptUsage))
	   	{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniSessHttpOptUsage
				   );
		SetTestStepResult(EFail);
		return;
		}
	if (sessHttpOptUsage)
		iSess.SetupHttpDataOptimiser(*this);
	
	// method
	method = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());		
	TBuf8<256> url8;
	url8.Copy(uri);
	TRAPD(err,InvokeHttpMethodL(url8, method));
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Teststep Failed: Leave occured in CTestHttpBaseStep::InvokeHttpMethodL: %D\n"
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
void CHttpEboClientStep::InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod)
	{

	TBool tranProxyUsage;
	TBool tranHttpOptUsage;
	TPtrC tranProxy;
	TUriParser8 uri; 
	uri.Parse( aUri );
	
	// Opening a transaction	
	iTrans = iSess.OpenTransactionL(uri, *this, aMethod);
	RHTTPHeaders hdr = iTrans.Request().GetHeaderCollection();
	// Add headers appropriate to all methods
	SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
	SetHeaderL(hdr, HTTP::EAccept, KAccept);
	
	if (!GetBoolFromConfig(ConfigSection(), KIniTranProxyUsage, tranProxyUsage))
	  	{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniTranProxyUsage
				   );
		SetTestStepResult(EFail);
		return;
		}
	if (tranProxyUsage)
		if (!GetStringFromConfig(ConfigSection(), KIniTranProxy, tranProxy))
		    {
			ERR_PRINTF2(_L("Problem in reading values from ini.			\
							\nExpected field is: \n%S\n"
						  ),&KIniTranProxy
					   );
			SetTestStepResult(EFail);
			return;
			}
	//setting transaction proxy
	RHTTPTransactionPropertySet transactionProperties = iTrans.PropertySet();
	RStringPool strP = iSess.StringPool();
	if (tranProxyUsage)
		{
		TBuf8<256> tranProx8;
		tranProx8.Copy(tranProxy);
		RStringF proxyAddr = strP.OpenFStringL(tranProx8);
		CleanupClosePushL(proxyAddr);
		THTTPHdrVal proxyUsage(strP.StringF(HTTP::EUseProxy, RHTTPSession::GetTable()));
		transactionProperties.SetPropertyL(strP.StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()), proxyUsage);
		transactionProperties.SetPropertyL(strP.StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()), proxyAddr);
		CleanupStack::PopAndDestroy(1,&proxyAddr);
		}
	
	// Setting HttpDataOptimiser for transaction.
	if (!GetBoolFromConfig(ConfigSection(), KIniTranHttpOptUsage, tranHttpOptUsage))
	  	{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniTranHttpOptUsage
				   );
		SetTestStepResult(EFail);
		return;
		}
	if (tranHttpOptUsage)
		iTrans.SetupHttpDataOptimiser(*this);
	// submit the transaction
	iTrans.SubmitL();
	// Start the scheduler, once the transaction completes or is cancelled on an error the scheduler will be
	// stopped in the event handler
	CActiveScheduler::Start();
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
void CHttpEboClientStep::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
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
            // Determine if the body will be saved to disk
			iSavingResponseBody = EFalse;
			if (resp.HasBody() && (status >= 200) && (status < 300) && (status != 204))
				{
				TInt dataSize = resp.Body()->OverallDataSize();
				if (dataSize >= 0)
					INFO_PRINTF2(_L("Response body size is %d\n"), dataSize);
				else
					INFO_PRINTF1(_L("Response body size is unknown\n"));
				
				// Save response to file in disk.
				iSavingResponseBody = ETrue;
				}

			    if (iSavingResponseBody) // If we're saving, then open a file handle for the new file
				{
				// Check it exists and open a file handle
				TInt valid = iFileServ.IsValidName(iOutputFilename);
				if (!valid)
					{
					ERR_PRINTF1(_L("The specified filename is not valid!.\n"));
					iSavingResponseBody = EFalse;
					}
				else
					{
					TInt err = iRespBodyFile.Replace(iFileServ,
													 iOutputFilename,
													 EFileWrite|EFileShareExclusive);
					if (err)
						{
						iSavingResponseBody = EFalse;
						User::Leave(err);
						}
					}
				}
            
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Get the body data supplier
			iRespBody = aTransaction.Response().Body();

			// Append to the output file if we're saving responses
			if (iSavingResponseBody)
				{
				INFO_PRINTF1(_L("Saving response body...\n"));
				TPtrC8 bodyData;
				TBool lastChunk = iRespBody->GetNextDataPart(bodyData);
				iRespBodyFile.Write(bodyData);
				if (lastChunk)
					iRespBodyFile.Close();
				}

			// Done with that bit of body data
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
TInt CHttpEboClientStep::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
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
void CHttpEboClientStep::EncodeL (const TDesC8& /*aHttpData*/, HBufC8* &aEncodedData)
	{
	
	INFO_PRINTF1(_L("######## CHttpEboClientStep::EncodeL ########\n"));
	TBool isEncoded;
	if (!GetBoolFromConfig(ConfigSection(), KIniIsEncoded, isEncoded))
	  	{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniIsEncoded
				   );
		SetTestStepResult(EFail);
		return;
		}
	// Checking whether enocoding is needed.
	if (isEncoded)
		{
		// Encoding: Changing the request data to http://www.google.co.uk/
		_LIT8(KTestRequest, "GET http://www.google.co.uk/ HTTP/1.1\r\nHost: www.google.co.uk\r\nAccept: */*\r\nUser-Agent: TestHttpEboClient\r\n\r\n");
		const TDesC8& encodedReq(KTestRequest);
		TRAPD(err,
			{
			aEncodedData =  encodedReq.AllocLC();
			CleanupStack::Pop();
			});	
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Test step failed: Leave occured while encoding the request: %D\n"), err);
       	    SetTestStepResult(EFail);
			}
		else
			{
			INFO_PRINTF1(_L("Encoded/Changed the request\n"));
			}
		}
	else
		INFO_PRINTF1(_L("Not encoding the request [passing NULL]\n"));	
	}

	
/**
It decodes the responsing data before parsing.
@internalTechnology 
@test
@param		aHttpData  Actual response.
@param		aEncodedData Decoded response.
@return		None
*/
void CHttpEboClientStep::DecodeL (const TDesC8& aData, HBufC8*& aHttpData, TBool& aTransFail)
	{
	INFO_PRINTF1(_L("######## CHttpEboClientStep::DecodeL ########\n"));
	TBool isDecoded;
	TBool failTransaction;
	if (!GetBoolFromConfig(ConfigSection(), KIniIsDecoded, isDecoded))
	  	{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniIsDecoded
				   );
		SetTestStepResult(EFail);
		return;
		}
	// Checking whether decoding is needed.
	if (isDecoded)
		{
		TRAPD(err, 
			{
			// Copying same actual response to decoded response
			aHttpData = aData.AllocLC();
			CleanupStack::Pop();	
			});
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Test step failed: Leave occured while decoding the response: %D\n"), err);
       	    SetTestStepResult(EFail);
			}
		else
			{
			INFO_PRINTF1(_L("Sent the actual response as a decoded response\n"));
			}
		}
	else
		INFO_PRINTF1(_L("Not decoding the response [passing NULL]\n"));
	
	if(!GetBoolFromConfig(ConfigSection(), KIniFailTransaction, failTransaction))
	  	{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ),&KIniFailTransaction
				   );
		SetTestStepResult(EFail);
		return;
		}
	// Checking to fail the transaction.
	if (failTransaction)
		{
		aTransFail = ETrue;
		INFO_PRINTF1(_L("CHttpEboClientStep::DecodeL returing ETrue(intend to fail the transaction)\n"));	
		}
	else
		{
		aTransFail = EFalse;	
		INFO_PRINTF1(_L("CHttpEboClientStep::DecodeL returing EFalse(not failing the transaction)\n"));	
		}
	}

