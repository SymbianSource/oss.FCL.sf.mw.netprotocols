// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "chttprequestcomposer.h"

#include <httpstringconstants.h>
#include <http/rhttpheaders.h>

#include "chttpresponseparser.h"
#include "mhttprequestobserver.h"
#include "chttpclienttransaction.h"


CHttpRequestComposer* CHttpRequestComposer::NewL(CProtTransaction& aProtTrans, MHttpRequestObserver& aObserver)
	{
	CHttpRequestComposer* self = new (ELeave) CHttpRequestComposer(aProtTrans, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CHttpRequestComposer::~CHttpRequestComposer()
	{
    if(iCorkingEnabled)
        {
        // Disable corking.
        iObserver.SendingBodyData(EFalse);
        }   
    
	iMessageComposer.Close();
	iTrailerHeaders.Reset();
	
//	__FLOG_CLOSE;
	}
	
CHttpRequestComposer::CHttpRequestComposer(CProtTransaction& aProtTrans, MHttpRequestObserver& aObserver)
: CTxData(aProtTrans), iStringTable(RHTTPSession::GetTable()), 
                        iObserver(aObserver),
					   iStringPool(aProtTrans.Transaction().Session().StringPool()),
					   iRequest(aProtTrans.Transaction().Request()),
					   iFields(aProtTrans.Transaction().Request().GetHeaderCollection().Fields()),
					   iPropertySet(aProtTrans.Transaction().PropertySet())

	{
//	__FLOG_OPEN("http", "httpclienthandler.txt");
	}
	
void CHttpRequestComposer::ConstructL()
	{
	iMessageComposer.OpenL(*this);
	}
	
void CHttpRequestComposer::NotifyMoreRequestBodyData()
	{
	__FLOG_1(_T8("Trans %d : notify more request body data"), iProtTrans->Transaction().Id());

	iMessageComposer.MessageInfoAvailable();
	}

void CHttpRequestComposer::InitHeadersL()
	{
	// Extract header fields from the Trailer header, if it exists.
	THTTPHdrVal trailerHeaderVal;
	RHTTPHeaders headers = iRequest.GetHeaderCollection();
	RStringF trailerName = iStringPool.StringF(HTTP::ETrailer, iStringTable);
	TInt index =0;
	while( headers.GetField(
						   trailerName,
						   index,
						   trailerHeaderVal
						   ) != KErrNotFound )
		{
		__ASSERT_DEBUG( trailerHeaderVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );

		// @todo should we check to see if header is not Content-Length,
		// @todo Transfer-Encoding or Trailer?

		// Got a trailer header - append to the list
		RStringF header = trailerHeaderVal.StrF();
		User::LeaveIfError(iTrailerHeaders.Append(header));

		// Next...
		++index;
		}

	if( iTrailerHeaders.Count() > 0 )
		{
		// There are trailer headers - can only send as trailers if the request
		// has a body and it is going to be chunk encoded.
		MHTTPDataSupplier* body = iRequest.Body();

		if( body == NULL || body->OverallDataSize() != KErrNotFound )
			{
			// Either there is no body, or the body is not going to be chunk
			// encoded - remove the Trailer header field.
			headers.RemoveField(trailerName);
			iTrailerHeaders.Reset();
			}
		}

	// Reset the fields iterator
	iFields.First();	
	}

TBool CHttpRequestComposer::IsTrailerHeader(RStringF aHeader)
	{
	TInt count = iTrailerHeaders.Count();
	TBool found = EFalse;	
	while( count > 0 && !found )
		{
		if( iTrailerHeaders[--count] == aHeader )
			{
			// aHeader is a trailer header
			found = ETrue;
			}
		}
	return found;
	}
	
/*
 *	Methods from MHTTPDataSupplier, via CTxData
 */

TBool CHttpRequestComposer::GetNextDataPart(TPtrC8& /*aDataPart*/)
	{
	// @todo should we deprecate derivation from MHTTPDataSupplier?
	User::Invariant();
	return EFalse;
	}

void CHttpRequestComposer::ReleaseData()
	{
	// @todo should we deprecate derivation from MHTTPDataSupplier?
	User::Invariant();
	}

TInt CHttpRequestComposer::OverallDataSize()
	{
	// @todo should we deprecate derivation from MHTTPDataSupplier?
	User::Invariant();
	return KErrNotFound;
	}

TInt CHttpRequestComposer::Reset()
	{
	// @todo should we deprecate derivation from MHTTPDataSupplier?
	User::Invariant();
	return KErrNotFound;
	}
	
/*
 *	Methods from MHttpRequest
 */
	
void CHttpRequestComposer::StartRequest()
	{
	__FLOG_1(_T8("Trans %d : starting request"), iProtTrans->Transaction().Id());
	
	RStringF notifyContinue = iStringPool.StringF(HTTP::ENotify100Continue, iStringTable);
	RStringF enableNotification = iStringPool.StringF(HTTP::EEnableNotification, iStringTable);
	THTTPHdrVal notifyContinueVal;
	iPropertySet.Property(notifyContinue, notifyContinueVal);
	
	((notifyContinueVal.Type()==THTTPHdrVal::KStrFVal) && (notifyContinueVal.StrF() == enableNotification))?(iSuspendRequest = ETrue):(iSuspendRequest = EFalse);

	iMessageComposer.MessageInfoAvailable();
	}

void CHttpRequestComposer::CancelRequest()
	{
	__FLOG_1(_T8("-> Trans %d : request cancelled"), iProtTrans->Transaction().Id());

	iMessageComposer.Reset();
	}
	
void CHttpRequestComposer::RequestDataSent()
	{	
	if(!IsSuspendedRequest())
		{
		iMessageComposer.ReleaseMessageData();
		}
	}

void CHttpRequestComposer::OnRequestSendTimeOut()
	{
	// Notify the client of Request Send TimeOut.
	RHTTPTransaction trans = iProtTrans->Transaction();
	if(trans.SendEvent(THTTPEvent::ESendTimeOut, THTTPEvent::EIncoming, THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)) != KErrNone)
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

TInt CHttpRequestComposer::SendTimeOutValue()
	{
	RHTTPTransaction trans = iProtTrans->Transaction();
	RStringPool stringPool = trans.Session().StringPool();
	RStringF sendTimeOut = stringPool.StringF(HTTP::ESendTimeOutValue, iStringTable);
	THTTPHdrVal sendTimeOutVal;
	
	TBool ret = trans.PropertySet().Property(sendTimeOut,sendTimeOutVal); 
	if(ret && (sendTimeOutVal.Type() == THTTPHdrVal::KTIntVal))
		{
		return sendTimeOutVal.Int();
		}
	return 0;
	}

/*
 *	Methods from MHttpMessageComposerObserver
 */

void CHttpRequestComposer::StartLineL(TPtrC8& aMethod, TPtrC8& aRequestUri, TPtrC8& aVersion)
	{
	// The RFC2616 defines the Request-Line as follows - 
	// 
	// Request-Line = Method SP Request-URI SP HTTPVersion CRLF
	//
	// Get Method...
	RStringF method = iStringPool.StringF(iRequest.Method());
	aMethod.Set(method.DesC());
	
	RHTTPTransaction trans = iProtTrans->Transaction();
	
	// Get the Request-URI...
	THTTPHdrVal uri;
	if( !trans.PropertySet().Property(iStringPool.StringF(HTTP::EUri, iStringTable), uri) )
		{
		// The EUri property has not been set - leave!
		User::Leave(KErrCorrupt);
		}
	aRequestUri.Set(uri.Str().DesC());
	
	// Get HTTPVersion - check to see if client has set a version
	RHTTPConnectionInfo connectionInfo = trans.Session().ConnectionInfo();
	THTTPHdrVal httpVersion;
	if( connectionInfo.Property(iStringPool.StringF(HTTP::EHTTPVersion, iStringTable), httpVersion) )
		{
		// Use the client specified version
		aVersion.Set(httpVersion.StrF().DesC());		
		}
	else
		{
		// Assume HTTP/1.1
		aVersion.Set(iStringPool.StringF(HTTP::EHttp11, iStringTable).DesC());
		}
	
	__FLOG_4(_T8("Trans %d : request-line -> %S %S %S"), iProtTrans->Transaction().Id(), &aMethod, &aRequestUri, &aVersion);
	
	// Initialise header info
	InitHeadersL();
	}

TInt CHttpRequestComposer::NextHeaderL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue)
	{
	// Are there any more headers?
	TInt err = KErrNotFound;
	TBool done = EFalse;
	while( !iFields.AtEnd() && !done )
		{
		// Get field current field.
		RStringF name = iStringPool.StringF(iFields());

		// Check to see if a trailer header
		if( !IsTrailerHeader(name) )
			{
			// Ok, found a header - done!
			err = KErrNone;
			done = ETrue;
				
			// Get the OTA version of the field value
			TPtrC8 value;
			iRequest.GetHeaderCollection().GetRawFieldL(name, value);
			
			// Pass back these values
			aHeaderName.Set(name.DesC());
			aHeaderValue.Set(value);

			__FLOG_3(_T8("Trans %d : request header -> %S: %S"), iProtTrans->Transaction().Id(), &aHeaderName, &aHeaderValue);
			}
		
		// Move onto next header field...
		++iFields;		
		}
	return err;
	}

MHTTPDataSupplier* CHttpRequestComposer::HasBodyL()
	{
#if defined (_DEBUG) && defined (_LOGGING)
	MHTTPDataSupplier* body = iRequest.Body();

	if( body == NULL )
		{
		__FLOG_1(_T8("Trans %d : no request entity body"), iProtTrans->Transaction().Id());
		}
	else if( body->OverallDataSize() == KErrNotFound )
		{
		__FLOG_1(_T8("Trans %d : chunked request entity body"), iProtTrans->Transaction().Id());
		}
	else
		{
		__FLOG_2(_T8("Trans %d : request entity body length = %d"), iProtTrans->Transaction().Id(), body->OverallDataSize());
		}
#endif

	return iRequest.Body();
	}
	
TInt CHttpRequestComposer::NextTrailerL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue)
	{
	TInt err = KErrNotFound;

	if( iTrailerHeaders.Count() > 0 )
		{
		// Ok, still got trailers.
		err = KErrNone;

		// Get trailer field.
		RStringF name = iTrailerHeaders[0];

		// Get the OTA version of the field value
		TPtrC8 value;
		iRequest.GetHeaderCollection().GetRawFieldL(name, value);
		
		// Pass back these values
		aHeaderName.Set(name.DesC());
		aHeaderValue.Set(value);
		
		// Remove this trailer from the array.
		iTrailerHeaders.Remove(0);

		__FLOG_3(_T8("Trans %d : request trailer -> %S: %S"), iProtTrans->Transaction().Id(), &aHeaderName, &aHeaderValue);
		}
	return err;
	}

void CHttpRequestComposer::MessageComplete()
	{
	__FLOG_1(_T8("Trans %d : request complete"), iProtTrans->Transaction().Id());
	if(iCorkingEnabled)
	    {
	    // Disable corking.
	    iCorkingEnabled = iRequestHeaderSent = EFalse;
	    iObserver.SendingBodyData(EFalse);
	    }	
		
	iObserver.RequestComplete();
	iRequestSent = ETrue;
	}

void CHttpRequestComposer::MessageDataReadyL()
	{
    if(iRequestHeaderSent)
        {
        // If we hit this function that means we need to send body data.
        // So we are sending the body data. Enable corking.
        if(!iCorkingEnabled)
            {
            __FLOG_1(_T8("Trans %d : Corking is enabled"), iProtTrans->Transaction().Id());            
            iCorkingEnabled = ETrue;
            iObserver.SendingBodyData(ETrue);
            }       
        else
            {
            __FLOG_1(_T8("Trans %d : Corking is already enabled"), iProtTrans->Transaction().Id());
            }
        }
    else
        {
        __FLOG_1(_T8("Trans %d : request header is set"), iProtTrans->Transaction().Id());        
        iRequestHeaderSent = ETrue;
        }		
    // Notify the observer that there is message data ready to send.
	iMessageComposer.GetMessageData(iData);
	
	iObserver.SendRequestDataL(iData);
	}

TInt CHttpRequestComposer::HandleComposeError(TInt aError)
	{
	__FLOG_1(_T8("!! Error : %d"), aError);
	__FLOG_1(_T8("-> Trans %d : request composing error - cancelling transaction"), iProtTrans->Transaction().Id());

	return static_cast<CHttpResponseParser&>(iProtTrans->RxData()).CancelTransaction(aError);
	}
	
TBool CHttpRequestComposer::CheckRequestPendingComplete()
	{
	return iMessageComposer.CheckMessagePendingComplete();
	}
TBool CHttpRequestComposer::RequestSent () const
	{
	return iRequestSent;		
	}

TBool CHttpRequestComposer::NeedDisconnectNotification ()
	{
	CHttpClientTransaction* clientTrans = static_cast < CHttpClientTransaction* > ( iProtTrans );
    return clientTrans->NeedDisconnectNotification ();
	}

void CHttpRequestComposer::Reserved_MHttpMessageComposerObserver()
	{
	User::Invariant();
	}

void CHttpRequestComposer::ResumeSuspendedRequest()
	{
	iSuspendRequest = EFalse;
	iMessageComposer.ReleaseMessageData();	
	}

TBool CHttpRequestComposer::IsSuspendedRequest() const
	{
	return iSuspendRequest;	
	}

void CHttpRequestComposer::CancelWaitFor100Continue()
	{
	__FLOG_1(_T8("Trans %d : client declined to wait for the 100-Continue Response"), iProtTrans->Transaction().Id());
	if(IsSuspendedRequest())
		{
		ResumeSuspendedRequest();	
		}
	}
	
MHttpDataOptimiser* CHttpRequestComposer::HttpDataOptimiser(TBool& aBatchingEnabled)
	{
	RHTTPTransaction trans = iProtTrans->Transaction();
 	MHttpDataOptimiser* httpOptimiser = trans.HttpDataOptimiser();
 	RHTTPSession sess = trans.Session();
 	
 	RStringPool stringPool = sess.StringPool();
	THTTPHdrVal value;
	RStringF string = stringPool.StringF(HTTP::EHttpBatching, iStringTable);
	RStringF str = stringPool.StringF(HTTP::EEnableBatching, iStringTable);
	if(sess.ConnectionInfo().Property(string, value))
		{
		if(value.Type() == THTTPHdrVal::KStrFVal)
			{
			aBatchingEnabled = value.StrF() == str;
			}
		}
	 	
 	if( httpOptimiser && !aBatchingEnabled )
 		{
		// the optimiser has not been set for the session.
 		__FLOG_1(_T8("-> Trans %d : Http optimiser has been set for this transaction"), iProtTrans->Transaction().Id());
 		return (httpOptimiser);
 		}
 	
 	// the MHttpDataOptimiser hasn't been set for the transaction
 	// check whether the session encapsulating the transaction is having MHttpDataOptimiser.
 	httpOptimiser  = sess.HttpDataOptimiser();
 	if( httpOptimiser )
 		{
 		// the optimiser has been set for the session.
 		__FLOG_1(_T8("-> Trans %d : HTTP optimiser has been set for the session encapsulating this transaction"), iProtTrans->Transaction().Id());
 		// check if the client has disabled optimiser for a particular transaction.
 		TBool disableTransOptimiser = EFalse;
 		string = stringPool.StringF(HTTP::EHTTPTransOptimiser, iStringTable);
 		str = stringPool.StringF(HTTP::EDisableHTTPTransOptimiser, iStringTable);
 		if(iProtTrans->Transaction().PropertySet().Property(string, value))
 			{
 			if(value.Type() == THTTPHdrVal::KStrFVal)
 				{
 				disableTransOptimiser = ( value.StrF() == str );
 				}
 			}
 		
 		if ( disableTransOptimiser )
 			{
 			// the client has disabled the optimiser for a particular transaction.
 			__FLOG_1(_T8("-> Trans %d : HTTP optimiser has been disabled by the client"), iProtTrans->Transaction().Id());
 			// do not return the optimiser.
 			return NULL;
 			}
 		
 		// the client has not diasabled the optimiser for any transaction in particular.
 		// return the optimiser.
 		return (httpOptimiser);
 		}
 	
 	// MHttpDataOptimiser has not been set for the session and transaction
 	__FLOG_1(_T8("-> Trans %d : HTTP optimiser was never set for the transaction and the session enclosing it"), iProtTrans->Transaction().Id());
 	return NULL;	
	}

