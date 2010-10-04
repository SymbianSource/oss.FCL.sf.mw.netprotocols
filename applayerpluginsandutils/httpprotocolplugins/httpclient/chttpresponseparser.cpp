// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttpresponseparser.h"

#include <inetprottextutils.h>
#include <httpstringconstants.h>
#include <http/rhttpheaders.h>
#include <http/framework/mrxdataobserver.h>
#include <httperr.h>
#include <e32math.h>

#include "chttpclienttransaction.h"
#include "chttpconnectionmanager.h"
#include "chttprequestcomposer.h"
#include "mhttpresponseobserver.h"
#include "thttpclientpanic.h"
#include "chttpconnectioninfo.h"

_LIT8(KHeaderSeparator,	"\n");

CHttpResponseParser* CHttpResponseParser::NewL(CProtTransaction& aProtTrans, MRxDataObserver& aObserver, MHttpResponseObserver& aResponseObserver)
	{
	CHttpResponseParser* self = new (ELeave) CHttpResponseParser(aProtTrans, aObserver, aResponseObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;	
	}
	
CHttpResponseParser::~CHttpResponseParser()
	{
	iBodyParts.Reset();
	iMessageParser.Close();
//	__FLOG_CLOSE;
	}
	
CHttpResponseParser::CHttpResponseParser(CProtTransaction& aProtTrans, MRxDataObserver& aObserver, MHttpResponseObserver& aResponseObserver)
: CRxData(aProtTrans, aObserver), iStringTable(RHTTPSession::GetTable()), iResponseObserver(aResponseObserver)
	{
//	__FLOG_OPEN("http", "httpclienthandler.txt");
	}
	
void CHttpResponseParser::ConstructL()
	{
	iMessageParser.OpenL(*this);
	}

TInt CHttpResponseParser::CancelTransaction(TInt aError)
	{
	__FLOG_0(_T8("!! Cancelling transaction - internal error"));
	
	// Tell the connection manager to cancel this submission.
	CHttpClientTransaction& trans = static_cast<CHttpClientTransaction&>(*iProtTrans);
	CHttpConnectionManager* manager = trans.ConnectionManager();

	__ASSERT_DEBUG( manager != NULL, User::Invariant() );

	__FLOG_1(_T8("-> Trans %d : cancelling its connection manager"), iProtTrans->Transaction().Id());

	// Transaction is still alive - ask its connection manager to cancel it.
	MHttpRequest& request = static_cast<CHttpRequestComposer&>(iProtTrans->TxData());
	MHttpResponse& response = *this;
	manager->CancelSubmission(request, response);
	
	// Connection is now cancelled - remove the connection manager from the
	// transaction.
	trans.RemoveConnectionManager();		

	// Notify the rx-data observer that an error has occurred
	NotifyObserverError(aError);

	return KErrNone;
	}

void CHttpResponseParser::NotifyObserverError(TInt aError)
	{
	TRAPD( err, iObserver->SetStatusL(*this, aError) );
	if(err != KErrNone)
		{
		// Something has gone seriously wrong - fail the transaction
		iProtTrans->Transaction().Fail(THTTPFilterHandle::EProtocolHandler);
		}
	}
	
/*
 *	Methods from CRxData
 */
 
void CHttpResponseParser::ResetRxData()
	{
	iMessageParser.Reset();
	iBodyParts.Reset();
	iOverallDataSize = 0;
	iFlags = 0;
	}
	
/*
 *	Methods from MHTTPDataSupplier via CRxData
 */
 
TBool CHttpResponseParser::GetNextDataPart(TPtrC8& aDataPart)
	{
	TInt bodyPartsCount = iBodyParts.Count();
	__ASSERT_ALWAYS(bodyPartsCount > 0, THttpClientPanic::Panic(THttpClientPanic::ENoBodyPartInDataSupplier));
	
	// Provide the first chunk.
	aDataPart.Set(iBodyParts[0]);
	
	return (BodyComplete() && bodyPartsCount == 1);
	}

void CHttpResponseParser::ReleaseData()
	{
	// Remove the oldest chunk.
	if( iBodyParts.Count() > 0 )
		{
		iBodyParts.Remove(0);
		}
	
	// Are there any more chunks?
	if( iBodyParts.Count() > 0 )
		{
		// Yep - notify the client of more body data
		RHTTPTransaction trans = iProtTrans->Transaction();
		if(trans.SendEvent(THTTPEvent::EGotResponseBodyData, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)) != KErrNone)
			trans.Fail(THTTPFilterHandle::EProtocolHandler);
		}
	else
		{
		// Does this data packet need to be released?
		if( NotifyReleaseData() )
			{
			iResponseObserver.ResponseDataParsed();
			iFlags &= ~ENotifyReleaseData;
			}
		
		// Is the message complete?
		if( MessageComplete() )
			{
			// All the body data has been read by the client - this transaction 
			// is complete.			
			TRAPD(err, iObserver->SetStatusL(*this, THTTPEvent::EResponseComplete));
			if(err != KErrNone)
				iProtTrans->Transaction().Fail(THTTPFilterHandle::EProtocolHandler);
			}
		}
	}

TInt CHttpResponseParser::OverallDataSize()
	{
	return (iOverallDataSize < 0) ? KErrNotFound : iOverallDataSize;
	}
	
TInt CHttpResponseParser::Reset()
	{
	return KErrNotSupported;
	}

void CHttpResponseParser::OnResponseReceiveTimeOut ()
	{
	// Notify the client of Response Receive TimeOut.
	RHTTPTransaction trans = iProtTrans->Transaction();
	if(trans.SendEvent(THTTPEvent::EReceiveTimeOut, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)) != KErrNone)
		{
		trans.Fail(THTTPFilterHandle::EProtocolHandler);
		}
	else
		{
		// Notify the client that Transaction Failed.
	    if(trans.SendEvent(THTTPEvent::EFailed, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)) != KErrNone)
	        {
			trans.Fail(THTTPFilterHandle::EProtocolHandler);
	        }
		}
	}

TInt CHttpResponseParser::ReceiveTimeOutValue ()
	{
	RHTTPTransaction trans = iProtTrans->Transaction();
	RStringPool stringPool = trans.Session().StringPool();
	RStringF receiveTimeOut = stringPool.StringF(HTTP::EReceiveTimeOutValue, iStringTable);
	THTTPHdrVal receiveTimeOutVal;
	
	TBool ret = trans.PropertySet().Property(receiveTimeOut,receiveTimeOutVal); 
	if(ret && (receiveTimeOutVal.Type() == THTTPHdrVal::KTIntVal))
		{
		return receiveTimeOutVal.Int();
		}
	return 0;
	}

TBool CHttpResponseParser::ResponseInformational ()
	{
	return (HTTPStatus::IsInformational(iProtTrans->Transaction().Response().StatusCode()));
	}


/*
 *	Methods from MHttpResponse
 */	
	
void CHttpResponseParser::ResponseDataReceived(const TDesC8& aData)
	{
	__FLOG_2(_T8("Trans %d : parsing %d bytes of data"), iProtTrans->Transaction().Id(), aData.Length());

	iRawData.Set(aData);
	iMessageParser.ReceivedMessageData();		
	}

void CHttpResponseParser::CancelResponse()
	{
	__FLOG_1(_T8("-> Trans %d : response cancelled"), iProtTrans->Transaction().Id());

	iCancellingResponse = ETrue;
	RHTTPTransaction trans = iProtTrans->Transaction();
	RHTTPResponse response = trans.Response();
	iMessageParser.Flush ();		
	iMessageParser.Reset();
	iCancellingResponse = EFalse;	
	}

void CHttpResponseParser::ConnectionError(TInt aError)
	{
	// Check whether we need to resubmit this request once again.	
	CHttpClientTransaction* clientTrans = static_cast<CHttpClientTransaction*>(iProtTrans);
	if ( !clientTrans->NeedDisconnectNotification() && (iOverallDataSize != MHttpMessageParserObserver::EUnknown) 
			&& (clientTrans->RetryNeeded ()	&& ( aError == KErrEof || aError == KErrCancel || aError == KErrDisconnected ) ) )
		{
		if ( !clientTrans->ConnectionManager()->ConnectionInfo().IsNonPersistent () )
			aError = KErrHttpNonPipeliningError;
		}
	
	RHTTPTransaction trans = iProtTrans->Transaction();
	RHTTPResponse response = trans.Response();
	
	// Check there is a status code, no body and error status ( KErrEof & KErrDisconnected )
	if ( response.StatusCode() && !response.HasBody() && ( aError == KErrEof || aError == KErrDisconnected ) )
		{
		// Notify the client that all the response headers have been parsed.
		trans.SendEvent(THTTPEvent::EGotResponseHeaders, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler));
		}

// Is the server closing the connection been expected?
	if( iOverallDataSize == MHttpMessageParserObserver::EUnknown && !BodyComplete() && 
				(aError == KErrEof || aError == KErrDisconnected) )
	{
		
	     __FLOG_1(_T8("Trans %d : waiting for connection close to mark end of response body"), iProtTrans->Transaction().Id());
			
			if (BodyPresent())
				{
				iFlags |= ( EMessageComplete | EBodyComplete ) ;
					
				// Append NULL to the body chunk 
				
				RHTTPTransaction trans = iProtTrans->Transaction();
				TInt err = iBodyParts.Append(TPtrC8());
                if ( err!=KErrNone ) 
                trans.Fail(THTTPFilterHandle::EProtocolHandler);

               
				// Notify the client that there is a body part 
				if(iProtTrans->Transaction().SendEvent(THTTPEvent::EGotResponseBodyData, 
                                                        THTTPEvent::EIncoming,
                                                        THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)) != KErrNone)
				    {
				    trans.Fail (THTTPFilterHandle::EProtocolHandler);
				    }
		    		
		}
			else
				{
				// Yep, since from RFC2616 section 4.4 Message Length, the end of a 
				// response body can be determined by the server closing the connection
				// Notify the parser that the connection close has happened.
				TRAPD( err, iMessageParser.CompletedBodyDataL() );
				if( err != KErrNone )
					{
					// Something has gone seriously wrong - fail the transaction
					iProtTrans->Transaction().Fail(THTTPFilterHandle::EProtocolHandler);
					}
				}
			
			}
		else
	        {
			// Need to map to appropriate error code if the disconnect notification is
			// asked by the client
			if ( clientTrans->NeedDisconnectNotification() )
				{
				CHttpRequestComposer& request = static_cast<CHttpRequestComposer&>(iProtTrans->TxData());
				// if we are consuming response then the request has been not sent fully.
				if ( ConsumingResponse () || !request.RequestSent() )
					aError = KErrHttpRequestNotSent;
				// Check if we received some response
				// Check for status code that means we received something as response
				// for this transaction				
				else if ( response.StatusCode () )					
					aError = KErrHttpPartialResponseReceived;
				else
					aError = KErrHttpResponseNotReceived;
				}
			
			// Notify the rx-data observer that an error has occurred
			   NotifyObserverError(aError);				
			}

		  }
	
/*
 *	Methods from MHttpMessageParserObserver
 */

void CHttpResponseParser::GetDataPacket(TPtrC8& aData)
	{
	aData.Set(iRawData);
	}

void CHttpResponseParser::ReleaseDataPacket()
	{
	if( ConsumingResponse() && MessageComplete() )
		{
		// Ok, the response was a 1xx message which has been consumed. Reset the
		// this object and continue parsing the next part of the response.
		ResetRxData();
		iMessageParser.ReceivedMessageData();		 
		}		
	else if( iBodyParts.Count() == 0 )
		{
		// Can release as there are no body chunks waiting to be passed to the
		// client.
		iResponseObserver.ResponseDataParsed();		
		}
	else
		{
		// Flag that the data needs to be released
		iFlags |= ENotifyReleaseData;
		}
	}

void CHttpResponseParser::StartLineL(const TDesC8& aStartLine)
	{
	__START_PERFORMANCE_LOGGER();
	// The RFC2616 defines the Status-Line as follows - 
	// 
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	//
	// First extract the HTTP-Version
	TPtrC8 data = aStartLine;
	User::LeaveIfError(InetProtTextUtils::RemoveWhiteSpace(data, InetProtTextUtils::ERemoveLeft));
	
	// HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
	TInt consumed = data.Locate('/');
	User::LeaveIfError(consumed);
	
	RHTTPTransaction trans = iProtTrans->Transaction();
	RStringPool stringPool = trans.Session().StringPool();

	TPtrC8 http = data.Left(consumed);
	if( http.CompareF(stringPool.StringF(HTTP::EHTTP, iStringTable).DesC()) != 0 )
		User::Leave(KErrCorrupt);
		
	// Skip past the HTTP and "/"
	data.Set(data.Mid(consumed + 1));
	
	// Extract the major version number
	TInt major;
	consumed = InetProtTextUtils::ConvertDescriptorToInt(data, major);
	User::LeaveIfError(consumed);
	
    if( data.Length() > consumed )
		{
	    // Skip past major version number and the "."
	    data.Set(data.Mid(consumed + 1));
		}
	else
		{
		User::Leave(KErrCorrupt); 
		}

	// Extract the minor version number
	TInt minor;
	consumed = InetProtTextUtils::ConvertDescriptorToInt(data, minor);
	User::LeaveIfError(consumed);
    
	if( data.Length() > consumed )
		{
	    // Skip past minor version number and the SP
	    data.Set(data.Mid(consumed + 1));
	
	    // Clear any extra surrounding whitespace
	    User::LeaveIfError(InetProtTextUtils::RemoveWhiteSpace(data, InetProtTextUtils::ERemoveBoth));
		}
	else
		{
		 User::Leave(KErrCorrupt);
		}
	
	// Extract the status code
	TInt status;
	consumed = InetProtTextUtils::ConvertDescriptorToInt(data, status);
	User::LeaveIfError(consumed);
	
	if( data.Length() > consumed )
		{
		// Skip past status code and the SP
		data.Set(data.Mid(consumed + 1));
	
		// Remaining data is the status reason - trim any leading whitespace as right side already trimmed
		User::LeaveIfError(InetProtTextUtils::RemoveWhiteSpace(data, InetProtTextUtils::ERemoveLeft));
		}
	else
		{
		// No reason phrase following the status code
		data.Set(KNullDesC8());
		}
	// Populate transaction with Status-Line info
	RStringF reason = stringPool.OpenFStringL(data);
	
	RHTTPResponse response = trans.Response();

	TVersion version(major, minor, 0);
	response.SetVersion(version);
	response.SetStatusCode(status);
	response.SetStatusText(reason);
	reason.Close();
	
	__FLOG_5(_T8("Trans %d : status-line -> HTTP/%d.%d %d %S"), iProtTrans->Transaction().Id(), major, minor, status, &response.StatusText().DesC());

	// Check to see if a body is expected
	if( HTTPStatus::IsInformational(status) )
		{
		// 1xx status - no body and need to consume this respons
		iFlags |= EConsumingResponse;
		
		__FLOG_1(_T8("Trans %d : informational 1xx status - consuming response"), iProtTrans->Transaction().Id());
		}	
		__END_PERFORMANCE_LOGGER(_L(",CHttpResponseParser::StartLineL()"));	
	}

void CHttpResponseParser::HeaderL(const TDesC8& aFieldName, TDesC8& aFieldValue)
	{
	// Only set the headers if we are not consuming the response.
	if( !ConsumingResponse() )
		{
		// Add the header to the response header collection
		RHTTPTransaction trans = iProtTrans->Transaction();
		RStringPool stringPool = trans.Session().StringPool();
		
		RStringF name = stringPool.OpenFStringL(aFieldName);
		CleanupClosePushL(name);
		
		RHTTPHeaders headers = trans.Response().GetHeaderCollection();
		headers.SetRawFieldL(name, aFieldValue, KHeaderSeparator);
		
		/**************************************************************************************
 		* DEF143319   - HTTPS page stops downloading between ~25 and 50 KBytes    
 		* When multiple requests are concatenated in ONE buffer and sent to the server,
 		* the WebLogic server does not process all requests.
 		* So pipelining feature has to be disabled.
 		* This fix enhances the robustness of HTTP stack to interoperate with different servers.
 		***************************************************************************************/
 		if (name.DesC().CompareF(stringPool.StringF(HTTP::EServer,RHTTPSession::GetTable()).DesC()) == KErrNone)
 		    {
             _LIT8(KWebLogicServerName,"WebLogic");
             if (aFieldValue.FindF(KWebLogicServerName)!= KErrNotFound)
                 {
                 RHTTPHeaders headReq = trans.Request().GetHeaderCollection();
                 RStringF hostStr = stringPool.StringF(HTTP::EHost, RHTTPSession::GetTable());
                 THTTPHdrVal hostVal;
                 
                 if( headReq.GetField(hostStr, 0, hostVal) == KErrNotFound )
                     {
                     // No Host header
                     User::Leave(KErrHttpGeneralHeaderMissingHost);
                     }
 
                 __ASSERT_DEBUG( hostVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );
                
                 CHttpClientTransaction& protTran = static_cast<CHttpClientTransaction&>(*iProtTrans);
                 CHttpConnectionManager* manager = protTran.ConnectionManager();
                 #if defined (_DEBUG) && defined (_LOGGING)
                     __FLOG_0(_T8("Pipelining is disabled for WebLogic Server- \n"));
                 #endif
                 manager->AppendPipelineFailedHost(hostVal.StrF().DesC());
                 }
		}
 		
 		if (name.DesC().CompareF(stringPool.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable()).DesC()) == KErrNone)
            {
            _LIT8(KNtlmProtocolName,"NTLM");
            if (aFieldValue.FindF(KNtlmProtocolName)!= KErrNotFound)
                {
                CHttpClientTransaction& protTran = static_cast<CHttpClientTransaction&>(*iProtTrans);
                CHttpConnectionManager* manager = protTran.ConnectionManager();
                _LIT8( KNtlmConnId, "NTLMConnId" );
                if (aFieldValue.Length() >=  4 )
                    {
                    if (manager->GetNtlmConnId() == KErrNotFound)
                        {
                        TInt ntmlConnId= Math::Random()%5789; //some magic number to get random connection  id
                        manager->SetNtlmConnId(ntmlConnId);
                        RStringF ntlmId= stringPool.OpenFStringL( KNtlmConnId );
                        CleanupClosePushL(ntlmId);
                        THTTPHdrVal value;
                        value.SetInt( ntmlConnId );
                        trans.PropertySet().SetPropertyL( ntlmId, value );
                        CleanupStack::PopAndDestroy(&ntlmId);
                        }
                    else
                        {
                        RStringF ntlmId= stringPool.OpenFStringL( KNtlmConnId );
                        CleanupClosePushL(ntlmId);
                        THTTPHdrVal value;
                        value.SetInt(manager->GetNtlmConnId());
                        trans.PropertySet().SetPropertyL( ntlmId, value );    
                        CleanupStack::PopAndDestroy(&ntlmId);
                        }
                    }
               }
            }
		
		CleanupStack::PopAndDestroy(&name);
		
		if( BodyComplete() && !GotTrailers() )	
			{
			// This was a trailer header - flag that there are trailers.
			iFlags |= EGotTrailers;
			}

		#if defined (_DEBUG) && defined (_LOGGING)
			if( GotTrailers() )
				__FLOG_3(_T8("Trans %d : response trailer -> %S: %S"), iProtTrans->Transaction().Id(), &aFieldName, &aFieldValue);
			else
				__FLOG_3(_T8("Trans %d : response header -> %S: %S"), iProtTrans->Transaction().Id(), &aFieldName, &aFieldValue);
		#endif
		}
	else
		{
		__FLOG_1(_T8("Trans %d : informational 1xx status - Headers will not be added"), iProtTrans->Transaction().Id());
		// Do nothing
		}
	}

TInt CHttpResponseParser::BodySizeL()
	{
	__START_PERFORMANCE_LOGGER();
	RHTTPTransaction trans = iProtTrans->Transaction();
	RHTTPResponse response = trans.Response();

	if( !ConsumingResponse() )
		{
		// Notify the client that all the response headers have been parsed.
		trans.SendEventL(
						THTTPEvent::EGotResponseHeaders, 
						THTTPEvent::EIncoming,
						THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
						);
		}

	RStringPool stringPool = trans.Session().StringPool();
	RHTTPHeaders headers = response.GetHeaderCollection();

	if (CheckForNonPersistentConnection())
		{
		// Notify the connection manager that the connection should be 
		// non-persistent.
		CHttpClientTransaction& trans = static_cast<CHttpClientTransaction&>(*iProtTrans);
		CHttpConnectionManager* manager = trans.ConnectionManager();

		__ASSERT_DEBUG( manager != NULL, User::Invariant() );

		manager->MakeConnectionNonPersistent();
		}

	// Check for a body...
	if( ConsumingResponse() ||	// this implies that the status code was 1xx - no body
		response.StatusCode() == 204 || 
		response.StatusCode() == 304 ||
		trans.Request().Method().Index(iStringTable) == HTTP::EHEAD ||
		trans.Request().Method().Index(iStringTable) == HTTP::ECONNECT && // if 2xx response...
		( HTTPStatus::IsSuccessful(response.StatusCode()) ) )
		{
		__FLOG_1(_T8("Trans %d : no response entity body"), iProtTrans->Transaction().Id());

		// No entity body is expected as specified in RFC2616 section 4.4.
		iOverallDataSize = MHttpMessageParserObserver::ENoBody;
		iFlags |= EBodyComplete;
		__END_PERFORMANCE_LOGGER(_L(",CHttpResponseParser::BodySizeL()"));
		return iOverallDataSize;
		}
		
	// A body is expected - find the length. First check for a Transfer-Encoding
	// header field.
	response.SetBody(*this);
	THTTPHdrVal value;
	RStringF name = stringPool.StringF(HTTP::ETransferEncoding, iStringTable);

	if( headers.GetField(name, 0, value) == KErrNone )
		{
		// It exists - what's the value?
		if( value.Type() == THTTPHdrVal::KStrFVal &&
			value.StrF().Index(iStringTable) == HTTP::EChunked )
			{
			// The Transfer-Encoding header is Chunked and as the chunked
			// encoding is removed, we remove the header.
			headers.RemoveField(name);
			
			__FLOG_1(_T8("Trans %d : chunked response entity body"), iProtTrans->Transaction().Id());
		
			// As the entity body is chunked the overall data size is unknown.
			iOverallDataSize = MHttpMessageParserObserver::EChunked;
				__END_PERFORMANCE_LOGGER(_L(",CHttpResponseParser::BodySizeL()"));
			return iOverallDataSize;			
			}
		}

	// Either no Transfer-Encoding header was present - now check for a 
	// Content-Length header.
	name = stringPool.StringF(HTTP::EContentLength, iStringTable);
	TInt err = headers.GetField(name, 0, value);
	
	if( err != KErrNone && err != KErrNotFound )
		User::Leave(err);
		
	if( err == KErrNone && value.Type() == THTTPHdrVal::KTIntVal )
		{
		// Content-Length header value specified the length of entity in bytes.
		iOverallDataSize = value.Int();

		__FLOG_2(_T8("Trans %d : response entity body length = %d"), iProtTrans->Transaction().Id(), iOverallDataSize);

       	__END_PERFORMANCE_LOGGER(_L(",CHttpResponseParser::BodySizeL()"));
		return iOverallDataSize;
		}
		
	__FLOG_1(_T8("Trans %d : unknown entity body length"), iProtTrans->Transaction().Id());

	// There was no Content-Length header either, so the server will signal the
	// end of the message by closing the connection - overall data size unknown.
	iOverallDataSize = MHttpMessageParserObserver::EUnknown;
		__END_PERFORMANCE_LOGGER(_L(",CHttpResponseParser::BodySizeL()"));
	return iOverallDataSize;
 	}

void CHttpResponseParser::BodyChunkL(const TDesC8& aData)
	{
	// Append the body chunk to the array.
	iFlags |= EBodyPresent;
	TPtrC8 chunk = aData;
	User::LeaveIfError(iBodyParts.Append(chunk));

	__FLOG_2(_T8("Trans %d : received %d bytes of body data"), iProtTrans->Transaction().Id(), chunk.Length());


	// Only notify the client that there is a body part if there is only one.
	if( iBodyParts.Count() == 1 )
		{
		RHTTPTransaction trans = iProtTrans->Transaction();	
		trans.SendEventL(
						THTTPEvent::EGotResponseBodyData, 
						THTTPEvent::EIncoming,
						THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
						);
		}	
}

void CHttpResponseParser::BodyCompleteL()
	{
	__FLOG_1(_T8("Trans %d : body complete"), iProtTrans->Transaction().Id());

	iFlags |= EBodyComplete;
	}

void CHttpResponseParser::MessageCompleteL(const TPtrC8& aExcessData)
	{
	__FLOG_2(_T8("Trans %d : message complete - received %d bytes of excess data"), iProtTrans->Transaction().Id(), aExcessData.Length());

	iFlags |= EMessageComplete;

	if( !ConsumingResponse() )
		{
		iResponseObserver.ResponseComplete(aExcessData);

		if ( iCancellingResponse )
			{
			return;
			}
		if( GotTrailers() )
			{
			// Inform the client that there were trailers
			RHTTPTransaction trans = iProtTrans->Transaction();	
			trans.SendEventL(
							THTTPEvent::EGotResponseTrailerHeaders, 
							THTTPEvent::EIncoming,
							THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
							);
			}
		if( iBodyParts.Count() == 0 )
			{
			// All the body data has been read by the client - this transaction is 
			// complete.
			iObserver->SetStatusL(*this, THTTPEvent::EResponseComplete);
			}
		}
	else
		{
		// There could be excess data - this data is for this response and needs
		// to be parsed. Set it as the raw data.
		CHttpRequestComposer& request = static_cast<CHttpRequestComposer&>(iProtTrans->TxData());
		if(request.IsSuspendedRequest())
			{
			RHTTPTransaction trans = iProtTrans->Transaction();	
			trans.SendEventL(
							THTTPEvent::EReceived100Continue, 
							THTTPEvent::EIncoming,
							THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
							);
			
			request.ResumeSuspendedRequest();
			}
			
		else
			{
			iRawData.Set(aExcessData);	
			}	
		}
	}

TInt CHttpResponseParser::HandleParserError(TInt aError)
	{
	__FLOG_1(_T8("!! Error : %d"), aError);
	__FLOG_1(_T8("-> Trans %d : response parsing error - cancelling transaction"), iProtTrans->Transaction().Id());

	// Cancel the transaction - this also notifies the rx-data observer that an 
	// error has occurred.
	return CancelTransaction(aError);
	}

TBool CHttpResponseParser::CheckForNonPersistentConnection()
	{
	RHTTPTransaction trans = iProtTrans->Transaction();
	RStringPool stringPool = trans.Session().StringPool();
	TBool keepAlive = EFalse;

	// Check for a Connection: close header
	//
	RStringF name = stringPool.StringF(HTTP::EConnection, iStringTable);
	THTTPHdrVal value;
	RHTTPHeaders headers = trans.Response().GetHeaderCollection();
	if( headers.GetField(name, 0, value) == KErrNone  && value.Type() == THTTPHdrVal::KStrFVal)
		{
		// It exists - what's the value?
		if( value.StrF().Index(iStringTable) == HTTP::EClose )
			{
			// The Connection header has a value of close.
			__FLOG_1(_T8("Trans %d : server has specified a non-persistent connection"), iProtTrans->Transaction().Id());
			return ETrue;
			}
		else if( value.StrF().Index(iStringTable) == HTTP::EKeepAlive )
			{
			// This request by the server to persist the connection is used by HTTP/1.0
			// servers to override default behaviour
			keepAlive = ETrue;
			}
		}


	// Is this a HTTP/1.0 response?
	//
	TVersion version = trans.Response().Version();
	if( !keepAlive && version.iMinor == 0 && version.iMajor == 1 && 
		trans.Request().Method().Index(iStringTable) != HTTP::ECONNECT )
		{
		__FLOG_1(_T8("Trans %d : an HTTP/1.0 server, default to non-persistent connection"), iProtTrans->Transaction().Id());
		return ETrue;
		}		

	// So treat connection as persistent.
	return EFalse;
	}

TBool CHttpResponseParser::ResponseCompleted ()
	{	
	// Check if we received a full response. No matter if client is not fully notified
	__FLOG_2 (_T8 ("Message completed? %d Body Completed? %d"), MessageComplete (), BodyComplete () );
	return MessageComplete ();
	}

TBool CHttpResponseParser::NeedCompletion ()
	{
	RHTTPTransaction trans = iProtTrans->Transaction ();
	if ( HTTPStatus::IsRedirection ( trans.Response().StatusCode() ) )
		{
		// If it is a redirection message then we need to complete the response
		return ETrue;			
		}
	return EFalse;		
	}

TBool CHttpResponseParser::CompleteResponse ( const TDesC8& aData )
	{
	return iMessageParser.CompleteMessage ( aData );
	}

void CHttpResponseParser::FailTransactionL()
	{
	iProtTrans->Transaction().SendEventL(KErrHttpOptimiserFailsTrans,
 						  	        THTTPEvent::EIncoming,
 							        THTTPFilterHandle::EProtocolHandler);
	}

void CHttpResponseParser::Reserved_MHttpMessageParserObserver()
	{
	User::Invariant();
	}

void CHttpResponseParser::FlushBodyDataIfNotRead()
{
 // Message is completed but the client is not yet read the complete body data
 // and we are cancelling. So we need to clear the parsed body data as another 
 // request would have been sent via the connection. The flushing of the body
 // data is needed to make the connection manager to read further response from the
 // socket otherwise it hangs. See the error: 
 
 // Note: This function should be called from CancelTransactionHook and only in the
 // case of client cancelling the transaction. ie; using RHTTPTransaction::Cancel();
 if(MessageComplete() && iBodyParts.Count() > 0)
  {
  iBodyParts.Reset(); // Reset the body array.  
  }
} 
