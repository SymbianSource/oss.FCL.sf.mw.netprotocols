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

// System includes
#include <http/framework/crxdata.h>
#include <http/rhttpheaders.h>
#include <http/thttpevent.h>
#include <httpstringconstants.h>
#include <wsp/wsptypes.h>
#include <wsp/cwsptransporthandler.h>
#include <wsp/mwspcosessioninvoker.h>
#include <wsp/mwspcomethodinvoker.h>
#include <wspstringconstants.h>
#include <wspstdconstants.h>
#include <wsperror.h>

// User includes
#include "cwspcotransaction.h"
#include "cwspcapabilityinfo.h"
#include "cwspproxyinfoprovider.h"
#include "cconnectiontimer.h"
#include "cwspprimitivesender.h"
#include "wsppanic.h"
#include "cwspheaderutils.h"
#include "cwspheadercodec.h"

// Class signature
#include "cwspcoprotocolhandler.h"

const TInt KDefaultConnectionTimeMicroSeconds	= 180000000;		// 3 minutes

/*
 * CWspCOProtocolHandler
 */

CWspCOProtocolHandler* CWspCOProtocolHandler::NewL(TAny* aSession)
	{
	RHTTPSession* session = REINTERPRET_CAST(RHTTPSession*, aSession);
	CWspCOProtocolHandler* self = new (ELeave) CWspCOProtocolHandler(*session);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CWspCOProtocolHandler::~CWspCOProtocolHandler()
	{
	
	if( iConnectionTimer )
		iConnectionTimer->Cancel();
	delete iConnectionTimer;

	delete iClientSessionHeaders;
	delete iTransportHandler;
	delete iClientCapInfo;
	delete iNegotiatedCapInfo;
	delete iProxyInfoProvider;
	delete iPrimitiveSender;
	delete iHdrUtils;
	}

CWspCOProtocolHandler::CWspCOProtocolHandler(RHTTPSession aSession)
: CProtocolHandler(aSession), /*iSessionState(ENull),*/ iTimedOutValue(KDefaultConnectionTimeMicroSeconds)
	{
	__OPEN_LOG("WspProtocolHandler.txt")
	}

void CWspCOProtocolHandler::ConstructL()
	{
	CProtocolHandler::ConstructL(iSession);

	// Open the WSP header field names table
	iSession.StringPool().OpenL(WSP::Table);

	// Create the proposed client capability object and the negotiated session capability object
	iClientCapInfo = CWspCapabilityInfo::NewL();
	iNegotiatedCapInfo = CWspCapabilityInfo::NewL();

	// Create the proxy info provider
	iProxyInfoProvider = CWspProxyInfoProvider::NewL(iSession);

	// Create the connection timer object
	iConnectionTimer = CConnectionTimer::NewL(*this);

	// Create the primitive sender active object
	iPrimitiveSender = CWspPrimitiveSender::NewL(*this);

	// Create the transport handler
	CreateWspTransportHandlerL();

	// Create the WSP header utilities
	iHdrUtils = CWspHeaderUtils::NewL(*STATIC_CAST(CWspHeaderCodec*, iCodec));
	}

void CWspCOProtocolHandler::CreateWspTransportHandlerL()
	{
	// Assert that the transport handler doesn't already exist
	__ASSERT_DEBUG(iTransportHandler == NULL, Panic(KWspPanicTransportHandlerAlreadyExists));

	// Pass through the session string pool when creating the transport handler
	RStringPool stringPool = iSession.StringPool();
	iTransportHandler = CWspTransportHandler::NewL(
												  stringPool,
												  iSecurityPolicy,
												  *this,				// Session CB
												  *iProxyInfoProvider,	// Proxy info
												  *this,				// Capability info
												  *this					// Session headers info
												  );

	// Check the supported services
	CWspTransportHandler::TWspSupportedServices suppSvc = iTransportHandler->SupportedServices();
	TBool sessionSupport = suppSvc & CWspTransportHandler::ECOSessionService;
	TBool methodSupport  = suppSvc & CWspTransportHandler::ECOMethodInvocationService;
	if( !sessionSupport || !methodSupport )
		{
		__LOG(_L("Session management facility or method invoke facility not supported by transport handler"));
		User::Leave(KWspErrRequiredServicesNotSupported);
		}
	// Configure the session invoker object
	iSessionInvoker = &iTransportHandler->COSessionInvoker();
	}


void CWspCOProtocolHandler::CheckClientCapabilities()
	{
	// Reset the proposed client properties - sets the capabilities to the 
	// default values.
	iClientCapInfo->Reset();

	// Check the session properties...
	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();

	// ...client message size...
	THTTPHdrVal clientMessageSize;
	TBool hasClientMessageSize = connInfo.Property(stringPool.StringF(HTTP::EWspCapClientMessageSize, RHTTPSession::GetTable()), clientMessageSize);
	if( hasClientMessageSize )
		{
		// Initial attempt to negotiate message size and SDU size will attempt to go
		// for a single PDU transfers, so set them the same
		__ASSERT_DEBUG( clientMessageSize.Type() == THTTPHdrVal::KTIntVal, Panic(KWspPanicBadClientMessageSize) );
		iClientCapInfo->SetClientMessageSize(clientMessageSize.Int());
		iClientCapInfo->SetClientSDUSize(clientMessageSize.Int());
		}
	// ... server message size...
	THTTPHdrVal serverMessageSize;
	TBool hasServerMessageSize = connInfo.Property(stringPool.StringF(HTTP::EWspCapServerMessageSize, RHTTPSession::GetTable()), serverMessageSize);
	if( hasServerMessageSize )
		{
		// Initial attempt to negotiate message size and SDU size will attempt to go
		// for a single PDU transfers, so set them the same
		__ASSERT_DEBUG( serverMessageSize.Type() == THTTPHdrVal::KTIntVal, Panic(KWspPanicBadServerMessageSize) );
		iClientCapInfo->SetServerMessageSize(serverMessageSize.Int());
		iClientCapInfo->SetServerSDUSize(serverMessageSize.Int());
		}
	// ...method MOR...
	THTTPHdrVal methodMOR;
	TBool hasMethodMOR = connInfo.Property(stringPool.StringF(HTTP::EWspCapMaxOutstandingRequests, RHTTPSession::GetTable()), methodMOR);
	if( hasMethodMOR )
		{
		__ASSERT_DEBUG( methodMOR.Type() == THTTPHdrVal::KTIntVal, Panic(KWspPanicBadMethodMOR) );
		__ASSERT_DEBUG( STATIC_CAST(TUint8, methodMOR.Int()) <= KMaxTUint8, Panic(KWspPanicBadMethodMOR) );
		__ASSERT_DEBUG( STATIC_CAST(TUint8, methodMOR.Int()) > 0, Panic(KWspPanicBadMethodMOR) );
		iClientCapInfo->SetMethodMOR( STATIC_CAST(TUint8, methodMOR.Int()) );
		}
	// Sort out the protocol options - by default try to use Large Data Transfer
	TUint8 protocolOptions = ELargeDataTransfer;
	THTTPHdrVal notUsed;
	// ...use acknowledgements...
	TBool hasUseAcknowledgements = connInfo.Property(stringPool.StringF(HTTP::EWspCapUseAcknowledgements, RHTTPSession::GetTable()), notUsed);
	if( hasUseAcknowledgements )
		{
		protocolOptions |= EAcknowledgementHeaders;
		}
	// ...support suspend resume facilty...
	TBool hasSuspendResume = connInfo.Property(stringPool.StringF(HTTP::EWspCapSupportSuspendResume, RHTTPSession::GetTable()), notUsed);
	if( hasSuspendResume )
		{
		protocolOptions |= ESessionResumeFacility;
		}
	iClientCapInfo->SetProtocolOptions(protocolOptions);

	// Get the connection time-out value
	THTTPHdrVal connectionTimeout;
	TBool hasConnectionTimeout = connInfo.Property(stringPool.StringF(HTTP::EWspProxyConnectionTimeout, RHTTPSession::GetTable()), connectionTimeout);
	if( hasConnectionTimeout )
		{
		__ASSERT_DEBUG((connectionTimeout.Type() == THTTPHdrVal::KTIntVal), Panic(KWspPanicBadConenctionTimeout));
		iTimedOutValue = connectionTimeout.Int();
		}
	else
		{
		// Use the default value
		iTimedOutValue = KDefaultConnectionTimeMicroSeconds;
		}
	}

TBool CWspCOProtocolHandler::UpdateNegotiatedCapabilitiesL()
	{
	// Check the capabilities proposed by the client
	TBool capabilitiesReduced = EFalse;

	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();

	// ...client message size...
	TUint32 clientMessageSize = iNegotiatedCapInfo->GetClientMessageSize();
	if( clientMessageSize != iClientCapInfo->GetClientMessageSize() )
		{
		// The server has reduced the client message size - update the return flag...
		capabilitiesReduced = ETrue;

		// ...and the session property...
		THTTPHdrVal clientMessageSizeProperty = clientMessageSize;
		connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspCapClientMessageSize, RHTTPSession::GetTable()), clientMessageSizeProperty);
		}
	// ...server message size...
	TUint32 serverMessageSize = iNegotiatedCapInfo->GetServerMessageSize();
	if( serverMessageSize != iClientCapInfo->GetServerMessageSize() )
		{
		// The server has reduced the server message size - update the return flag...
		capabilitiesReduced = ETrue;

		// ...and the session property...
		THTTPHdrVal serverMessageSizeProperty = serverMessageSize;
		connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspCapServerMessageSize, RHTTPSession::GetTable()), serverMessageSizeProperty);
		}
	// ...method MOR...
	TUint32 methodMOR = iNegotiatedCapInfo->GetMethodMOR();
	if( methodMOR != iClientCapInfo->GetMethodMOR() )
		{
		// The server has reduced the method MOR - update the return flag...
		capabilitiesReduced = ETrue;

		// ...and the session property...
		THTTPHdrVal methodMORProperty = methodMOR;
		connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspCapMaxOutstandingRequests, RHTTPSession::GetTable()), methodMORProperty);
		}
	// Check the protocol options
	TUint8 protocolOptions = iNegotiatedCapInfo->GetProtocolOptions();

	// ...acknowledgements...
	if( (protocolOptions & EAcknowledgementHeaders) != (iClientCapInfo->GetProtocolOptions() & EAcknowledgementHeaders) )
		{
		// The server has cleared the use acknowledgement headers flag - update
		// the return flag...
		capabilitiesReduced = ETrue;

		// ...and the session property...
		THTTPHdrVal notUsed = 0;
		connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspCapUseAcknowledgements, RHTTPSession::GetTable()), notUsed);
		}
	// ...suspend resume facility...
	if( (protocolOptions & ESessionResumeFacility) != (iClientCapInfo->GetProtocolOptions() & ESessionResumeFacility) )
		{
		// The server has cleared the support suspend/resume facility flag - 
		// update the return flag...
		capabilitiesReduced = ETrue;

		// ...and the session property...
		THTTPHdrVal notUsed = 0;
		connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspCapSupportSuspendResume, RHTTPSession::GetTable()), notUsed);
		}

	return capabilitiesReduced;
	}

TBool CWspCOProtocolHandler::SupportSuspendResume() const
	{
	return (iNegotiatedCapInfo->GetProtocolOptions() & ESessionResumeFacility);
	}

TBool CWspCOProtocolHandler::CanResume() const
	{
	TBool canResume = SupportSuspendResume();

	if( canResume && (iSessionState != EConnected && iSessionState != ESuspending && iSessionState != ESuspended) )
		{
		// In the wrong state
		canResume = EFalse;
		}
	return canResume;
	}

TBool CWspCOProtocolHandler::CanSuspend() const
	{
	TBool canSuspend = SupportSuspendResume();

	if( canSuspend && (iSessionState != EConnected && iSessionState != EResuming) )
		{
		// In the wrong state
		canSuspend = EFalse;
		}
	return canSuspend;
	}

#if defined (_DEBUG)
void CWspCOProtocolHandler::ResetAll()
	{
	// Delete helper objects and reset state
	delete iClientSessionHeaders;
	iClientSessionHeaders = NULL;
	// 
	if (iClientCapInfo)
		iClientCapInfo->Reset();
	if (iNegotiatedCapInfo)
		iNegotiatedCapInfo->Reset();
	if (iConnectionTimer)
		iConnectionTimer->Cancel();
	if (iProxyInfoProvider)
		iProxyInfoProvider->ResetProxyInfo();
	//
	iSessionState = ENull; 
	iTimedOutValue = 0;
	iConnectTimedOut = EFalse;
	iWaitingMethod  = EFalse;
	iDisconnectRequested = EFalse;
	iPendingCompletingMethods = 0;
	//
	// Reset the transport handler (stub only)
	iSessionInvoker->DisconnectReq((TWspReason)(-999));
	}
#endif

void CWspCOProtocolHandler::SessionConnectL()
	{
	__ASSERT_DEBUG( iSessionState == ENull, Panic(KWspPanicSessionNotInValidState) );

	__LOG(_L("Initiating a session connect."));

	// Check for any client proposed capabilities
	CheckClientCapabilities();

	// Update the client session headers
	UpdateClientSessionHeadersL();

	// Update the proxy info
	iProxyInfoProvider->UpdateProxyInfoL();

	// Start connection timed-out timer
	iConnectionTimer->Start(iTimedOutValue);

	// Send S-Connect.req primitive to transport handler
	__LOG(_L("---Sending S-Connect.req primitive."));
	iSessionInvoker->ConnectReq();

	// Update session state
	__LOG(_L("---WSP Session in Connecting state."));
	iSessionState = EConnecting;

	// Check to see if there is a method transactions waiting.
	CheckWaitingMethod();
	}

void CWspCOProtocolHandler::SessionResumeL()
	{
	__ASSERT_DEBUG( CanResume(), Panic(KWspPanicSessionNotInValidState) );

	__LOG(_L("Initiating a session resume."));

	// Check to see if the proxy info has not changed.
	if( iProxyInfoProvider->ProxyInfoChangedAndUpdateL() )
		{
		__LOG(_L("---Proxy has changed. Disconnect from the old proxy and connect to the new one."));

		// The client has specified a different - disconnect this session
		SessionDisconnect(EChangedProxyInSuspendedSession);
		}
	else
		{
		// Check for any client proposed capabilities
		CheckClientCapabilities();

		// Update the client session headers
		UpdateClientSessionHeadersL();

		// Start connection timed-out timer
		iConnectionTimer->Start(iTimedOutValue);

		// Send S-Resume.req primitive to transport handler
		__LOG(_L("---Sending S-Resume.req primitive."));
		iSessionInvoker->ResumeReq();

		// Update session state
		__LOG(_L("---WSP Session in Resuming state."));
		iSessionState = EResuming;

		// Check to see if there is a method transactions waiting.
		CheckWaitingMethod();
		}
	}

void CWspCOProtocolHandler::SessionDisconnect(TWspReason aReason)
	{
	__ASSERT_DEBUG( iSessionState != ENull && iSessionState != EClosing, Panic(KWspPanicSessionNotInValidState) );

	// Are there any methods waiting to send their final .res primitive
	if( iPendingCompletingMethods > 0 )
		{
		// Yep, need to wait until there are done.
		iDisconnectRequested = ETrue;
		return;
		}
	__LOG(_L("Disconnecting session."));

	// Need to cancel the connection timer
	iConnectionTimer->Cancel();

	// Send S-Disconnect.req primitive to transport handler
	__LOG1(_L("---Sending S-Disconnect.req primitive. Reason code : %d"), aReason);
	iSessionInvoker->DisconnectReq(aReason);

	// Update session state
	__LOG(_L("---WSP Session in Closing state."));
	iSessionState = EClosing;
	}

void CWspCOProtocolHandler::SessionSuspend()
	{
	__ASSERT_DEBUG( CanSuspend(), Panic(KWspPanicSessionNotInValidState) );
	
	// Are there any methods waiting to send their final .res primitive
	if( iPendingCompletingMethods > 0 )
		{
		// Yep, need to wait until there are done.
		iDisconnectRequested = ETrue;
		return;
		}
	__LOG(_L("Suspending session."));

	// Need to cancel the connection timer
	iConnectionTimer->Cancel();

	// Send S-Suspend.req primitive to transport handler
	__LOG(_L("---Sending S-Suspend.req primitive."));
	iSessionInvoker->SuspendReq();

	// Update session state
	__LOG(_L("---WSP Session in Suspending state."));
	iSessionState = ESuspending;
	}

void CWspCOProtocolHandler::DoSessionConnectedL()
	{
	// Cancel the connection timer
	iConnectionTimer->Cancel();

	// Check for the Encoding-Version header in the server session headers
 	RHTTPHeaders headers = iSession.ResponseSessionHeadersL();
 	THTTPHdrVal negotiatedVersion;
 	TInt err = headers.GetField(
 							   iSession.StringPool().StringF(WSP::EEncodingVersion, WSP::Table),
 							   0,		// Zero index -> first part
 							   negotiatedVersion
 							   );
	
	// Default version is 1.2 - this is the case if the header is missing.
 	CWspHeaderCodec::TWspVersion version = CWspHeaderCodec::EVersion1_2;

	if( (err != KErrNotFound) && (negotiatedVersion.Type() == THTTPHdrVal::KStrFVal) )
 		{
 		// Check what version the encoding is and set it accordingly in the codec
		switch(negotiatedVersion.StrF().Index(WSPStdConstants::Table) )
			{
			case WSPStdConstants::EWspVersion11:
				version = CWspHeaderCodec::EVersion1_1;
				break;
			case WSPStdConstants::EWspVersion13:
				version = CWspHeaderCodec::EVersion1_3;
				break;
			case WSPStdConstants::EWspVersion14:
				version = CWspHeaderCodec::EVersion1_4;
				break;
			default:
				// If the version is 1.2 or anything else, stay at 1.2
				break;
			}
 		}
	// Set the encoding value in the codec
	STATIC_CAST(CWspHeaderCodec*, iCodec)->SetWspVersion(version);

	// Inform client that the session is connected - check negotiated capabilities
	THTTPSessionEvent connectEvent = THTTPSessionEvent::EConnectedOK;

	// Check the negotiated capabilities
 	TBool capabilitiesReduced = UpdateNegotiatedCapabilitiesL();

	if( capabilitiesReduced )
		{
		// At least one of the proposed client capabilities has been negotiated 
		// down or rejected - need to inform client of this event.
		connectEvent = THTTPSessionEvent::EConnectedWithReducedCapabilities;
		}
	SendSessionEvent(connectEvent);

	// WSP session is connected - update state
	__LOG(_L("---WSP Session in Connected state"));
	iSessionState = EConnected;
	}

void CWspCOProtocolHandler::SendSessionEvent(THTTPSessionEvent aEvent)
	{
	// Send the event to the client - need to TRAPD
	TRAPD(error, iSession.SendSessionEventL(aEvent, THTTPSessionEvent::EIncoming, THTTPFilterHandle::EProtocolHandler));
	if( error != KErrNone )
		{
		__LOG1(_L("Error - Could not send event to the client, error code %d"), error);
		iSession.FailSessionEvent(THTTPFilterHandle::EProtocolHandler);
		}
	}

void CWspCOProtocolHandler::UpdateClientSessionHeadersL()
	{
	// Check for Encoding-Version header
	RStringPool stringPool = iSession.StringPool();
	RHTTPHeaders headers = iSession.RequestSessionHeadersL();
	THTTPHdrVal version;
	RStringF encodingVersionField = stringPool.StringF(WSP::EEncodingVersion, WSP::Table);
	TInt err = headers.GetField(encodingVersionField, 0 /* Zero index -> first part*/, version);
	TBool foundHeader = EFalse;

	// If the encoding-version header exists, check that it is valid
	if( err == KErrNone )
		{
		if( version.Type() == THTTPHdrVal::KStrFVal )
			{
			switch( version.StrF().Index(WSPStdConstants::Table) )
				{
				case WSPStdConstants::EWspVersion11:
				case WSPStdConstants::EWspVersion12:
				case WSPStdConstants::EWspVersion13:
				case WSPStdConstants::EWspVersion14:
					foundHeader = ETrue;
					break;
				default:
					break;
				}
			}
		}

	// If the header is not found or is invalid, then use WSP encoding v1.4
	if(!foundHeader)
		{
		// Remove existing header first if its invalid, this won't do anything if it doesn't exist
		headers.RemoveField(encodingVersionField);
		THTTPHdrVal encodingVersionValue(stringPool.StringF(WSPStdConstants::EWspVersion14, WSPStdConstants::Table));
		headers.SetFieldL(encodingVersionField, encodingVersionValue);
		__LOG(_L("---Updated WSP encoding version request to 1.4"));
		}

   	// Encode the headers...
	HBufC8* buf = iHdrUtils->EncodeHeadersL(stringPool, headers);

	// Update the client session headers buffer;
	delete iClientSessionHeaders;
	iClientSessionHeaders = buf;
	}

void CWspCOProtocolHandler::HandleConnectRequestL()
	{
	// Check the WSP session state
	switch( iSessionState )
		{
	case ENull:
		{
		// Initiate a session connect
		SessionConnectL();
		} break;
	case ESuspended:
	case ESuspending:
		{
		// In suspending or suspended state - this implies that the WSP 
		// session supports the Suspend Resume facility and was suspended 
		// rather than disconnected for a more efficient re-connection.

		// Initiate a session resume
		SessionResumeL();
		} break;
	case EConnecting:
	case EResuming:
		{
		// A connect request has already been sent - inform the client
		SendSessionEvent(THTTPSessionEvent::EAlreadyConnecting);
		} break;
	case EConnected:
		{
		// A connect request has already been sent and the session is 
		// connected - inform the client
		SendSessionEvent(THTTPSessionEvent::EAlreadyConnected);
		} break;
	case EClosing:
		{
		// A disconnect request has already been sent - inform the client
		SendSessionEvent(THTTPSessionEvent::EAlreadyDisconnecting);
		} break;
	default:
		Panic(KWspPanicIllegalSessionState);
		break;
		}
	}

void CWspCOProtocolHandler::HandleDisconnectRequest()
	{
	// Disconnect has been requestd. Action depends of the WSP session state.
	switch( iSessionState )
		{
	case ENull:
	case ESuspended:
		{
		// A disconnect request has already been fulfilled - inform the client
		SendSessionEvent(THTTPSessionEvent::EAlreadyDisconnected);
		} break;
	case EClosing:
	case ESuspending:
		{
		// A disconnect request has already been sent - inform the client
		SendSessionEvent(THTTPSessionEvent::EAlreadyDisconnecting);
		} break;
	case EConnecting:
		{
		// The WSP session is in the Connecting state, then the session should be
		// disconnected - do SessionDisconnect. This will issue the 
		// S-Disconnect.req primitive and tell the transport handler to stop the 
		// connection. The transport handler will send the S-Disconnect.ind 
		// primitive which results in the client being notified.
		SessionDisconnect(EUserReq);
		} break;
	case EResuming:
		{
		// If the WSP session is resuming, then the Suspend Resume facility is
		// supported - can suspend the WSP session. This will issue the 
		// S-Suspend.req primitive and tell the transport handler to stop the 
		// session resume. The transport handler will send the S-Suspend.ind 
		// primitive to the protocol handler who will then inform the client.
		SessionSuspend();
		} break;
	case EConnected:
		{
		// Check if the Suspend Resume facility is supported. If so, then 
		// suspend the WSP session, rather then disconnect it.
		if( SupportSuspendResume() )
			{
			// Suspend the WSP session
			SessionSuspend();
			}
		else
			{
			// Disconnect the WSP session
			SessionDisconnect(EUserReq);
			}
		} break;
	default:
		Panic(KWspPanicIllegalSessionState);
		break;
		}
	}

void CWspCOProtocolHandler::CheckWaitingMethod()
	{
	if( iWaitingMethod )
		{
		// Self-complete the base class - this will start servicing the pending
		// transaction.
		CompleteSelf();

		// Clear the flag
		iWaitingMethod = EFalse;
		}
	}

/*
 * Methods from CProtocolHandler
 */

void CWspCOProtocolHandler::CreateCodecL()
	{
	iCodec = CWspHeaderCodec::NewL(iSession.StringPool(), WSP::Table);
	}

CProtTransaction* CWspCOProtocolHandler::CreateProtTransactionL(RHTTPTransaction aTransaction)
	{
	return CWspCOTransaction::NewL(aTransaction, iTransportHandler->COTransactionInvoker(), *iNegotiatedCapInfo, *this, *iHdrUtils);
	}

TBool CWspCOProtocolHandler::ServiceL(CProtTransaction& aTrans)
	{
	__LOG(_L("Attempting to invoke request."));

	// Check the WSP session state
	if( iSessionState == EClosing || iSessionState == ESuspending )
		{
		__LOG(_L("---Session is Closing or Suspending - cannot invoke methods."));

		// The WSP session is either disconnecting or suspending - leave
		User::Leave(KWspErrSessionClosingOrSuspending);
		}
	if( iSessionState == ENull || iSessionState == ESuspended )
		{
		__LOG(_L("---Session is Null or Suspended - cannot invoke methods. Wait for session connect initiation."));
		
		// Send back an event saying that session is not connected yet
		SendSessionEvent(THTTPSessionEvent::ENotConnected);

		// Set a flag to indicate that there is a transaction waiting to be 
		// serviced - need to self-complete once connected.
		iWaitingMethod = ETrue;

		//  Cannot service the method transaction now.
		return EFalse;
		}
	// Check to see if the method MOR has been reached.
	TUint32 activeTransactions = NumActiveTransactions();
	if( activeTransactions == iNegotiatedCapInfo->GetMethodMOR() )
		{
		__LOG1(_L("---Have reached method MOR (=%d) - cannot invoke request."), iNegotiatedCapInfo->GetMethodMOR());

		// Have reached the method MOR - cannot service this method transaction.
		return EFalse;
		}
	// Have not yet reached the negotiated method MOR - can service this 
	// transaction.

	// Create the Tx Data object for this transaction
	aTrans.CreateTxDataL();

	// Do the method invocation
	STATIC_CAST(CWspCOTransaction&, aTrans).InitRequestL();

	__LOG1(_L("---Initiated request - trans %d is active"), aTrans.Transaction().Id() );

	return ETrue;
	}

void CWspCOProtocolHandler::ClosedTransactionHook(CProtTransaction* aTrans)
	{
	__LOG1(_L("Trans %d has been closed"), aTrans->Transaction().Id() );

	// Down-cast the CProtTransaction
	CWspCOTransaction& wspTransaction = *STATIC_CAST(CWspCOTransaction*, aTrans);

	// Abort this transaction
	wspTransaction.AbortRequest();

	// Tell the method transaction to suicide - the transaction will ensure that 
	// it deletes itself at the appropriate time.
	wspTransaction.Suicide();
	}

void CWspCOProtocolHandler::CancelTransactionHook(CProtTransaction& aTransaction)
	{
	__LOG1(_L("Trans %d has been cancelled"), aTransaction.Transaction().Id() );

	// Abort this transaction
	STATIC_CAST(CWspCOTransaction&, aTransaction).AbortRequest();
	}

void CWspCOProtocolHandler::NotifyNewRequestBodyPart(CProtTransaction& aTransaction)
	{
	__LOG(_L("Client has more request data..."));

	// Send the S-MethodInvokeData primitive
	STATIC_CAST(CWspCOTransaction&, aTransaction).NotifyMoreRequestData();
	}

void CWspCOProtocolHandler::GetInterfaceL(TUid aInterfaceId, MProtHandlerInterface*& aInterfacePtr)
	{
	switch(aInterfaceId.iUid)
		{
	case KProtHandlerSessionServerCertUid:
		{
		aInterfacePtr = this;
		break;
		}
	default:
		{
		CProtocolHandler::GetInterfaceL(aInterfaceId, aInterfacePtr);
		break;
		}
		}
	}

/*
 * Methods from MHTTPFilterBase
 */

void CWspCOProtocolHandler::MHFSessionRunL(const THTTPSessionEvent& aEvent)
	{
	__ASSERT_DEBUG( iTransportHandler, Panic(KWspPanicTransportHandlerDoesNotExist) );

	// Check the session event
	switch(aEvent.iStatus)
		{
	case THTTPSessionEvent::EConnect:
		{
		__LOG(_L("Processing EConnect session event"));

		// Handle the connect request
		HandleConnectRequestL();
		} break;
	case THTTPSessionEvent::EDisconnect:
		{
		__LOG(_L("Processing EDisconnect session event"));

		// Handle the disconnect request
		HandleDisconnectRequest();
		} break;
#if defined (_DEBUG)
	case -999:
		{
		// This unpublished value is used in testing to cause a whole protocol
		// handler reset and to reset the transport handler. It should be used with caution!
		__LOG(_L("PROTOCOL HANDLER RESETTING!"));
		ResetAll();

		// Ack the owner
		SendSessionEvent(-999);
		} break;
#endif
	default:
		// Ignore the unknown session event.
		__LOG1(_L("Received unknown session event : %d"), aEvent.iStatus);
		break;
		}
	}

TInt CWspCOProtocolHandler::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	// Send the error as a session event.
	SendSessionEvent(aError);
	return KErrNone;
	}

/*
 * Methods from MWspCOSessionCallback
 */

void CWspCOProtocolHandler::ConnectCnf()
	{
	__LOG(_L("Received S-Connect.cnf primitive"));

	// Did we connect to a secure proxy?  If so, send the 'authenticated OK' session event
	MWspProxyInfoProvider& proxyInfo = STATIC_CAST(MWspProxyInfoProvider&, *iProxyInfoProvider);
	if( proxyInfo.SecureConnection() )
		{
		__LOG(_L("---WTLS authentication was successful."));

		// Send EAuthenticatedOK event...
		SendSessionEvent(THTTPSessionEvent::EAuthenticatedOK);
		}

	// Process the session connected event
	DoSessionConnected();
	}

void CWspCOProtocolHandler::ResumeCnf()
	{
	__LOG(_L("Received S-Resume.cnf primitive"));

	// Process the session connected event
	DoSessionConnected();
	}

void CWspCOProtocolHandler::DoSessionConnected()
	{
	// Process the session connected event
	TRAPD(error, DoSessionConnectedL());
 	if( error != KErrNone )
 		{
 		__LOG(_L("Error - Could not process S-Connect.cnf/S-Resume.cnf. Disconnect the WSP session."));
 
 		// Disconnect this session
 		SessionDisconnect(EPeerReq);
 		}
	}

void CWspCOProtocolHandler::DisconnectInd(TWspReason					aReason, 
										  TBool							/*aRedirectSecurity*/, 
										  TWspRedirectedAddress&		aRedirectAddress, 
										  const TDesC8&					/*aErrorHeader*/, 
										  const TDesC8&					/*aErrorBody*/)
	{
	__ASSERT_DEBUG( NumActiveTransactions() == 0, Panic(KWspPanicMethodsStillOutstanding) );

	__LOG(_L("Received S-Disconnect.ind primitive"));
	__LOG1(_L("---Reason code : %d"), aReason);

	// Cancel the connection timer
	iConnectionTimer->Cancel();

	// Check the reason for the disconnect
	switch( aReason )
		{
	case EPermanentRedirectedProxy:
		{
		__LOG(_L("---Permanent proxy redirection"));

		// Update the proxy info with the redirected proxy addresses. This will
		// place the new address in the EWspProxyAddress session property.
		TRAPD(error, iProxyInfoProvider->SetPermanentRedirectedProxyL(aRedirectAddress) );

		if( error != KErrNone )
			{
			// Something went wrong - just disconnect
			SendSessionEvent(THTTPSessionEvent::EDisconnected);
			}
		else
			{
			// Need to inform client that the proxy has been redirected permanently.
			SendSessionEvent(THTTPSessionEvent::ERedirected);

			// Send the S-Connect primitive
			iPrimitiveSender->InitiateSend(ESConnect);
			}
		} break;
	case ETemporaryRedirectedProxy:
		{
		__LOG(_L("---Temporary proxy redirection"));

		// Update the proxy info with the redirected proxy addresses.
		TRAPD(error, iProxyInfoProvider->SetTemporaryRedirectedProxyL(aRedirectAddress) );;

		if( error != KErrNone )
			{
			// Something went wrong - just disconnect
			SendSessionEvent(THTTPSessionEvent::EDisconnected);
			}
		else
			{
			// Send the S-Connect primitive
			iPrimitiveSender->InitiateSend(ESConnect);
			}
		} break;
	case EChangedProxyInSuspendedSession:
		{
		__LOG(_L("---Client changed proxy whilst suspended"));
		__LOG(_L("---Forced disconnect, then connection to new proxy"));

		// The client had disconnected the session (resulting in the session
		// being suspended) and then did a connect, but with a different 
		// proxy. This resulted in the suspended session being disconnected.
		// Now need to initiate a session connect to the new proxy.
		iPrimitiveSender->InitiateSend(ESConnect);
		} break;
	case EWtlsConfigurationFailed:
	case EWtlsPhase1HandshakeFailed:
	case EWtlsPhase2HandshakeFailed:
	case EWtlsInvalidServerCert:
	case EWtlsUntrustedServerCert:
	case EWtlsNegotiatedConfigRejected:
		{
		__LOG(_L("---WTLS handshake failure"));

		// Various failure modes for the WTLS handshake.  All result in a
		// 'authentication failure' event being sent to the client, followed by
		// a 'disconnected' event.
		SendSessionEvent(THTTPSessionEvent::EAuthenticationFailure);
		SendSessionEvent(THTTPSessionEvent::EDisconnected);
		} break;
	case EOutOfMemory:
		{
		// There was a memory allocation failure in the transport handler - inform
		// the client.
		SendSessionEvent(KErrNoMemory);

		// Inform the client that the sesion is disconnected
		SendSessionEvent(THTTPSessionEvent::EDisconnected);
		} break;
	case ESessionStateFailure:
		{
		// The session is effectively disconnected
		SendSessionEvent(THTTPSessionEvent::EDisconnected);

		// The transport handler has entered an invalid or indeterminate state - possibly due
		// to a memory allocation failure - the session is no longer valid and should be
		// discontinued by the client - inform the client.
		__LOG(_L("Transport Handler has signalled a session state failure"));
		iSession.FailSessionEvent(THTTPFilterHandle::EProtocolHandler);
		} break;
	case EUserReq:
		{
		// This is ind to a S-Disconnect.req. Was it due to a connect time-out?
		if( iConnectTimedOut )
			{
			__LOG(_L("---Connection has timed-out"));

			// Inform the client that the resume timed-out
			SendSessionEvent(THTTPSessionEvent::EConnectionTimedOut);
			iConnectTimedOut = EFalse;
			break;
			}
		// Drop-through if there was no connect time-out
		} 
	default:
		// Session disconnected for some other reason. Inform the client, but 
		// ignore the information in aErrorHeader and aErrorBody. The WSP 
		// Specification states that the provider MAY choose not to communicate
		// this information - the protocol handler is making that choice!!
		SendSessionEvent(THTTPSessionEvent::EDisconnected);
		break;
		}
	// WSP session is disconnected - update state
	__LOG(_L("---WSP Session in Null state"));
	iSessionState = ENull;
	}

void CWspCOProtocolHandler::SuspendInd(TWspReason /*aReason*/)
	{
	__ASSERT_DEBUG( NumActiveTransactions() == 0, Panic(KWspPanicMethodsStillOutstanding) );

	__LOG(_L("Received S-Suspend.ind primitive"));

	// Do not care about the reason, just inform the client. First check if this
	// suspend is the result of a session resume timed-out.
	if( iConnectTimedOut )
		{
		__LOG(_L("---Resume has timed-out"));

		// Inform the client that the resume timed-out
		SendSessionEvent(THTTPSessionEvent::EConnectionTimedOut);
		iConnectTimedOut = EFalse;
		}
	else
		{
		// Inform the client that the session is disconnected - this event is 
		// sent as the the client does not understand suspend/resume and the 
		// Suspend Resume facility is being used for efficiency.
		SendSessionEvent(THTTPSessionEvent::EDisconnected);
		}
	// WSP session is suspended - update state
	__LOG(_L("---WSP Session in Suspended state"));
	iSessionState = ESuspended;
	}

void CWspCOProtocolHandler::ExceptionInd(const TDesC8& aExceptionData)
	{
	RStringPool stringPool = iSession.StringPool();

	// Generic handling for exception data
	__LOG(_L("Have received exception information."));

	// Add the exception data as the EWspProxyExceptionInfo property
	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	TRAP_IGNORE(
		RStringF exceptionData = stringPool.OpenFStringL(aExceptionData);
		CleanupClosePushL(exceptionData);
		THTTPHdrVal exceptionProperty = exceptionData;
		connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspProxyExceptionInfo, RHTTPSession::GetTable()), exceptionProperty);
		CleanupStack::PopAndDestroy(&exceptionData);
		);
	
	// Send the exception info event
	SendSessionEvent(THTTPSessionEvent::EExceptionInfo);
	}

/*
 * Methods from MWspCapabilityInfoProvider
 */

const MWspCapabilityViewer& CWspCOProtocolHandler::ClientCapabilities() const
	{
	return *iClientCapInfo;
	}

MWspCapabilitySetter& CWspCOProtocolHandler::ServerCapabilities() const
	{
	return *iNegotiatedCapInfo;
	}

/*
 * Methods from MWspSessionHeadersProvider
 */

const TDesC8& CWspCOProtocolHandler::ClientHeaders() const
	{
	return *iClientSessionHeaders;
	}

void CWspCOProtocolHandler::SetServerHeadersL(const TDesC8& aBuffer)
	{
	RHTTPHeaders serverHeaders = iSession.ResponseSessionHeadersL();
	iHdrUtils->DecodeHeadersL(iSession.StringPool(), aBuffer, serverHeaders);
	}

/*
 *	Methods from MConnectionTimerCallback
 */

void CWspCOProtocolHandler::HandleConnectionTimedOut()
	{
	__LOG(_L("Connection timed-out - appropriate action taken"));

	// The connection has timed-out - flag
	iConnectTimedOut = ETrue;

	// Request a disconnect. 
	HandleDisconnectRequest();
	}

/*
 * Methods from MWspPrimitiveSenderCallback
 */

void CWspCOProtocolHandler::SendPrimitiveL(TWspPrimitive aPrimitive)
	{
	// Check that the primitive is one that is supported
	switch( aPrimitive )
		{
	case ESConnect:
		{
		SessionConnectL();
		} break;
	case ESDisconnect:
	case ESSuspend:
		{
		HandleDisconnectRequest();
		} break;
	default:
		// Unsupported primitive
		User::Leave(KWspErrUnsupportedSendPrimitive);
		break;
		}
	}

TInt CWspCOProtocolHandler::WspPrimitiveSenderCallbackError(TInt /*aError*/)
	{
	// The initiate session connect has failed. Need to unlock the proxy info.
	iProxyInfoProvider->UnlockProxyInfo();

	// Inform the client - send disconnected signal
	SendSessionEvent(THTTPSessionEvent::EDisconnected);

	return KErrNone;
	}

/*
 * Methods from MWspCOMethodObserver
 */

void CWspCOProtocolHandler::HandleMethodAbort(CWspCOTransaction& aTransaction)
	{
	// The CWspCOTransaction has failed the transaction - inform the base class
	// protocol handler to change the state of this transaction.
	TransactionFailed(aTransaction.Transaction());

	__LOG1(_L("Transaction %d has failed."), aTransaction.Transaction().Id() );
	}

void CWspCOProtocolHandler::NotifyPendingCompletingMethod()
	{
	// Increment the number of pending methods waiting to move from Completing
	// to Null state.
	++iPendingCompletingMethods;

	__LOG1(_L("Number of methods waiting to send final .res primitive : %d"), iPendingCompletingMethods);
	}

void CWspCOProtocolHandler::NotifyMethodComplete()
	{
	// The final .res primitive has been sent for one of the methods - decrement
	// count.
	--iPendingCompletingMethods;

	__LOG1(_L("Number of methods waiting to send final .res primitive : %d"), iPendingCompletingMethods);

	if( iPendingCompletingMethods == 0 && iDisconnectRequested )
		{
		__LOG(_L("---All methods have sent final .res primitive. Requesting disconnect."));

		// No more methods waiting to send the final .res primitive and the
		// client has requested the session be disconnected - request it now.
		iPrimitiveSender->InitiateSend(ESDisconnect);
		}
	}


/*
 * Methods from MRxDataObserver
 */

void CWspCOProtocolHandler::SetStatusL(CRxData& aRxData, TInt aStatus)
	{
	// Have received a status message from an Rx data object - check the status.
	switch( aStatus )
		{
	case THTTPEvent::EResponseComplete:
		{
		__LOG1(_L("Transaction %d has completed"), aRxData.ProtTrans().Transaction().Id() );
		// The response is complete - the client has been passed all the data
		// and released it. It has no further use for the Rx data object. The
		// transaction is now complete - inform the base class.
		TransactionCompletedL(aRxData.ProtTrans().Transaction(), THTTPEvent::EResponseComplete);
		} break;
	default:
		// Unknown status - do nothing.
		__LOG1(_L("Received unknown status %d"), aStatus);
		break;
		}
	}

TInt CWspCOProtocolHandler::SessionServerCert(TCertInfo& aServerCert)
	{
	return iTransportHandler->ServerCert(aServerCert);
	}

const CCertificate* CWspCOProtocolHandler::SessionServerCert()
	{
	return iTransportHandler->ServerCert();
	}

/*
 * Methods from MProtHandlerInterface
 */

TInt CWspCOProtocolHandler::TransactionServerCert(TCertInfo& /*aServerCert*/, RHTTPTransaction /*aTransaction*/)
	{
	// Transaction Server certificate does not make sense for WSP
	return KErrNotSupported;
	}

const CCertificate* CWspCOProtocolHandler::TransactionServerCert(RHTTPTransaction /*aTransaction*/)

	{
	return NULL;
	}
