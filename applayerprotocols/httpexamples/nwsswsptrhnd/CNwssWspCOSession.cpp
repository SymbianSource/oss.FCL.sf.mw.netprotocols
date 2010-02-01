// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <http/mhttpdatasupplier.h>
#include <httpstringconstants.h>
#include <wsp/mwspsessionheadersprovider.h>
#include <wsp/mwspcomethodcallback.h>
#include <wsp/mwspcosessioncallback.h>
#include <wsp/mwspcapabilityprovider.h>
#include <wsp/mwspcapabilityviewer.h>
#include <wsp/mwspcapabilitysetter.h>
#include <wsp/mwspaliasaddresses.h>
#include <wsp/mwspextendedmethods.h>
#include <wsp/mwspheadercodepages.h>
#include <wsp/mwspunknowncapabilities.h>
#include <wsp/wsptypes.h>
#include <wsperror.h>
#include <capcodec.h>
#include <uri8.h>
#include <http/framework/logging.h>

// Local includes
#include "tnwsswsptrhndpanic.h"
#include "mconnectioninfoprovider.h"
#include "cnwsswspcoeventdispatcher.h"
#include "cnwsswsptrhnddatasupplier.h"
#include "cnwssconnectguard.h"
#include "cnwsswsptransactioneventfilter.h"
#include "cnwsswspsessioneventfilter.h"
#include "testoom.h"

// Class signature
#include "cnwsswspcosession.h"

// Constants used in this file
const TInt KMaxUriLength = 256;
const TUint KSessDataChunkSize = 256;
const TUint KTransLUTSize = 4;
const TInt KMaxCapabilityDataSize = 256;
const TUint32 KMaxClientSDUSize = 30000;
const TUint32 KMaxServerSDUSize = 30000;
//
_LIT8(KGet, "GET");
_LIT8(KPost, "POST");
_LIT8(KHead, "HEAD");
_LIT8(KTrace, "TRACE");
_LIT8(KOptions, "OPTIONS");
_LIT8(KPut, "PUT");
_LIT8(KDelete, "DELETE");


CNwssWspCOSession* CNwssWspCOSession::NewL(RStringPool& aStringPool,
										   MNwssWapServer& aWapStackProvider,
										   MConnectionInfoProvider& aConnInfoProvider,
										   MSecurityPolicy& aSecurityPolicy,
										   MWspCOSessionCallback& aSessionCB)
	{
	CNwssWspCOSession* me = new(ELeave)CNwssWspCOSession(aStringPool,
														 aWapStackProvider,
														 aConnInfoProvider,
														 aSecurityPolicy,
														 aSessionCB);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CNwssWspCOSession::~CNwssWspCOSession()
	{
	// It is important that the client correctly disconnects sessions before deleting the transport
	// handler.
#ifdef _DEBUG
	if( State() != CNwssWspSession::EDisconnected )
		{
		// WSP session could be suspended
		RWSPCOConn::TSessionState state;
		iWspCOSession.GetSessionState(state);
		
#ifndef __UNIT_TESTING__
		__ASSERT_DEBUG(state == RWSPCOConn::ESuspended, 
					TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EConnectedUponDestruction));
#endif
		}
#endif

	if( State() != CNwssWspSession::EDisconnected )
		// Ensure everything is closed down.  Since we're destructing, we don't care about
		// errors in this callback.
		(void)SessionDisconnectedCallback();

	if (iConnectGuard)
		iConnectGuard->Cancel();
	if (iTransEventFilter)
		iTransEventFilter->Cancel();
	if( iSessionEventFilter )
		iSessionEventFilter->Cancel();
	// Clean up

	delete iCOTransTable;
	delete iTransEventFilter;
	delete iSessionEventFilter;
	delete iEventDispatcher;
	delete iCapabilityCodec;
	delete iUriBuffer;
	delete iSessDataBuffer;
	delete iConnectGuard;
	delete iOOMCallBack;
	__CLOSELOG
	}

CNwssWspCOSession::CNwssWspCOSession(RStringPool& aStringPool,
								     MNwssWapServer& aWapStackProvider,
								     MConnectionInfoProvider& aConnInfoProvider,
									 MSecurityPolicy& aSecurityPolicy,
									 MWspCOSessionCallback& aSessionCB)
	: CNwssWspSession(aStringPool, aWapStackProvider, aConnInfoProvider, aSecurityPolicy),
	  iSessionCB(aSessionCB)
	{
	__OPENLOG("http", "nwsswsptrhnd.txt")
	__LOG_TITLE("WSP Transport Handler (NWSS bindings) Log")
	}

void CNwssWspCOSession::ConstructL()
	{
	BaseConstructL();

	// Create the transaction look-up table with a default initial size.  Create
	// the event dispatcher and capability codec.
	iCOTransTable = CNwssTransLookUpTable::NewL(KTransLUTSize);
	iTransEventFilter = CNwssWspTransactionEventFilter::NewL(*iCOTransTable, *this, KTransLUTSize);
	iSessionEventFilter = CNwssWspSessionEventFilter::NewL(*iCOTransTable, *this, *this);
	iEventDispatcher = CNwssWspCOEventDispatcher::NewL(iWspCOSession, *iSessionEventFilter, *iTransEventFilter, *this);
	iCapabilityCodec = CCapCodec::NewL();

	// Create scratch buffers
	iUriBuffer = HBufC::NewL(KMaxUriLength);
	iSessDataBuffer = HBufC8::NewL(KSessDataChunkSize);

	// Create the connect guard
	iConnectGuard = CNwssConnectGuard::NewL(*this, iSessionCB);

	// Create the one shot used if OOM occurs during a MethodInvoke or Connect
	iOOMCallBack = new (ELeave)CAsyncCallBack(CActive::EPriorityStandard);
	}


/*
 * Session Invocation API. 
 */
void CNwssWspCOSession::ConnectReq()
	{
	__LOG("Received S-Connect.req primitive")

	// Set connect limbo flag. This indicates that the S-Connect.req has been
	// received but not yet sent to the WAP Stack.
	iInConnectLimbo = ETrue;

	// All this stuff will be broken out, into different states in the parent class
	// This method will just request from its parent that the connection process is
	// started.  The parent, as an AO, will go through the various stages in its RunL.
	// That will involve calls down to this class to allow the connection to be
	// completed.

	InitiateProxyConnection();
	}

TInt CNwssWspCOSession::OpenWspSession(const TDesC8& aRemoteHost,
									   RWAPConn::TPort aRemotePort,
									   RWAPConn::TPort aLocalPort,
									   TBearer aBearer,
									   TBool aSecureConn)
	{
	// Open a WSP CO connection with the WAP stack
	__TESTOOMD(error, iWspCOSession.Open(iWapStackProvider.WapStack(), aRemoteHost, aRemotePort, aLocalPort, aBearer, aSecureConn));
	return error;	  
	}

TInt CNwssWspCOSession::CloseWspSession()
	{
	iEventDispatcher->Cancel();
	iWspCOSession.CancelAll();
	__TESTOOMD(stkErr, iWspCOSession.Close());
	return stkErr;
	}

void CNwssWspCOSession::CompleteProxyConnectionL()
	{
	__LOG("Sending S-Connect.req primitive")

	// Obtain the client headers
	MWspSessionHeadersProvider& prov = iConnInfoProvider.SessionHeadersProvider();
	const TDesC8& cliHdr = prov.ClientHeaders();
	__LOG("--Client session headers are:")
	__DUMP("--", "--", cliHdr)

	// Prepare the client's proposed capabilities
	PrepareClientCapabilities();

	// Request a connection to the proxy
	__TESTOOMD(error, iWspCOSession.Connect(cliHdr, iCapabilityCodec));
	__LOG1("--RWSPCOConn::Connect() returned with %d", error)
	User::LeaveIfError(error);

	// Ensure the event dispatcher is waiting for stack events
	iEventDispatcher->Start();

	// Unset the connect limbo flag - sent S-Connect.req to WAP Stack.
	iInConnectLimbo = EFalse;

	// Send the waiting method invoke (if exists)
	iConnectGuard->SendMethodInvokeReq();
	}

RWTLS CNwssWspCOSession::WtlsHnd()
	{
	return iWspCOSession.Wtls();
	}

void CNwssWspCOSession::DoRunError(TInt aError)
	{
	// Handling from the parent class state machine's RunError.
	// In all cases, this means that a ConnectReq() call in this class has failed.
	// Hence, a connection will never be possible with the gateway, and we should
	// indicate this to the client using DisconnectInd() to ensure their own notion of
	// the session state is correct.  The WTLS failure mode (aError) is indicated
	// using the disconnect reason.
	__ASSERT_DEBUG( iInConnectLimbo, User::Invariant() );

	__LOG("Returning S-Disconnect.ind primitive")

	// Convert the error code to a WSP reason
	TWspReason reason = Wap::EConnectErr;
	switch (aError)
		{
	case KWtlsErrConfigurationFailed:
		__LOG("--reason: KWtlsErrConfigurationFailed")
		reason = Wap::EWtlsConfigurationFailed;
		break;
	case KWtlsErrPhase1HandshakeFailed:
		__LOG("--reason: KWtlsErrPhase1HandshakeFailed")
		reason = Wap::EWtlsPhase1HandshakeFailed;
		break;
	case KWtlsErrPhase2HandshakeFailed:
		__LOG("--reason: KWtlsErrPhase2HandshakeFailed")
		reason = Wap::EWtlsPhase2HandshakeFailed;
		break;
	case KWtlsErrInvalidServerCert:
		__LOG("--reason: KWtlsErrInvalidServerCert")
		reason = Wap::EWtlsInvalidServerCert;
		break;
	case KWtlsErrUntrustedServerCert:
		__LOG("--reason: KWtlsErrUntrustedServerCert")
		reason = Wap::EWtlsUntrustedServerCert;
		break;
	case KWtlsErrNegotiatedConfigRejected:
		__LOG("--reason: KWtlsErrNegotiatedConfigRejected")
		reason = Wap::EWtlsNegotiatedConfigRejected;
		break;
	case KErrNoMemory:
		__LOG("--reason: KErrNoMemory")
		reason = Wap::EOutOfMemory;
		break;
	default:
		;
		}
	// Need to send S-Disconnect.ind - should deal with an invoked method
	// if it exists.
	iConnectGuard->SendDisconnectInd(reason);

	// Unset the connect limbo flag.
	iInConnectLimbo = EFalse;
	}

TBool CNwssWspCOSession::SubDoCancel()
	{
	// Called from the parent class DoCancel().  Cancel the outstanding requests in this class.
	iEventDispatcher->Cancel();

	// We don't want to return to a disconnected state after this.
	return EFalse;
	}

void CNwssWspCOSession::DisconnectReq(TWspReason aReason)
	{
	__ASSERT_DEBUG(State() != CNwssWspSession::EDisconnected,
					TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInvalidState));
	__LOG("Sending S-Disconnect.req primitive")

	// Check to see if the S-Connect.req has been sent to the WAP Stack
	if( iInConnectLimbo )
		{
		__LOG("---In connect limbo - cannot send S-Disconnect.req.")
		__LOG("---Cancelling proxy connection process.")
		// No, still trying to connect - cancel the proxy connection
		Cancel();

		// Need to send S-Disconnect.ind - should deal with an invoked method
		// if it exists.
		iConnectGuard->SendDisconnectInd(aReason);

		// Unset the connect limbo flag.
		iInConnectLimbo = EFalse;
		return;
		}
	// Request a disconnect
	__ASSERT_DEBUG(State() == CNwssWspSession::EReady,
					TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInvalidState));
	__TESTOOMD(error, iWspCOSession.Disconnect());	  

	// Handle OOM separately - we can recover from it.
	if (error == KErrNoMemory)
		{
		ScheduleDelayedOomDisconnect(EFalse);
		return;
		}
	__ASSERT_ALWAYS(error == KErrNone, MapToPanic(error));

	// Inform the transaction event filter that S-Disconnect.req has been sent
	iTransEventFilter->SessionDisconnectRequested();

	// Ensure the event dispatcher is waiting for stack events
	iEventDispatcher->Start();
	}

void CNwssWspCOSession::SuspendReq()
	{
	__LOG("Sending S-Suspend.req primitive")

	// Request suspension
	__TESTOOMD(error, iWspCOSession.Suspend());	  
	__ASSERT_ALWAYS(error == KErrNone, MapToPanic(error));

	// Ensure the event dispatcher is waiting for stack events
	iEventDispatcher->Start();
	}

void CNwssWspCOSession::ResumeReq()
	{
	__LOG("Sending S-Resume.req primitive")

	// Obtain the (possibly updated) client headers
	MWspSessionHeadersProvider& prov = iConnInfoProvider.SessionHeadersProvider();
	const TDesC8& cliHdr = prov.ClientHeaders();
	__LOG("--Client session headers are:")
	__DUMP("--", "--", cliHdr)

	// Request session resume.  Needs a blank RWSPCOConn - what to do with this?
	//@todo - this may require considerable work...
	RWSPCOConn blankConn;
	__TESTOOMD(error, iWspCOSession.Resume(blankConn, cliHdr));
	__ASSERT_ALWAYS(error == KErrNone, MapToPanic(error));

	// Ensure the event dispatcher is waiting for stack events
	iEventDispatcher->Start();
	}

/*
 * Method Invocation API. 
 */

void CNwssWspCOSession::MethodInvokeReq(
								MWspCOMethodCallback&		aMethodCallback, 
								RStringF					aMethod, 
								const TUriC8&				aRequestUri, 
								const TDesC8&				aRequestHeaders, 
								const TDesC8&				aRequestBody,
								TBool						aMoreData
								)
	{
	__LOG("Sending T-MethodInvoke.req primitive")
#ifdef __DEBUG
	if( aMoreData )
		{
		__LOG("Attempt to send T-MethodInvoke.req primitive failed because")
		__LOG("Large Data Transfer is not supported in this WAP Stack.")
		}
#endif
	__ASSERT_ALWAYS( !aMoreData, TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ELargeDataTransferNotSupported) );

	if( iInConnectLimbo )
		{
		__LOG("--In connect limbo - cannot send T-MethodInvoke.req.")
		__LOG("--Save information for later...")

		TRAPD(error, __DEBUGTESTLEAVE \
					  iConnectGuard->ReceivedMethodInvokeReqL(
														 aMethodCallback,
														 aMethod,
														 aRequestUri,
														 aRequestHeaders,
														 aRequestBody
														 ) );
		__ASSERT_ALWAYS( error == KErrNone, TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EDeferedMethodFailed) );
		return;
		}

	__ASSERT_ALWAYS(iWspCOSession.SubSessionHandle() != KNullHandle, TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ESessionNotConnected));

	RWAPConn::TMethod method = RWAPConn::EGet;
	const TDesC8& methodName = aMethod.DesC();
	__LOG1("--Method is '%S'", &methodName);
	if (methodName.CompareF(KGet) == 0)
		method = RWAPConn::EGet;
	else if (methodName.CompareF(KPost) == 0)
		method = RWAPConn::EPost;
	else if (methodName.CompareF(KHead) == 0)
		method = RWAPConn::EHead;
	else if (methodName.CompareF(KTrace) == 0)
		method = RWAPConn::ETrace;
	else if (methodName.CompareF(KOptions) == 0)
		method = RWAPConn::EOptions;
	else if (methodName.CompareF(KPut) == 0)
		method = RWAPConn::EPut;
	else if (methodName.CompareF(KDelete) == 0)
		method = RWAPConn::EDelete;
	else
		// @todo  -  extension methods?
		TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EUnknownMethod);

	// Convert the URI to Unicode
	__LOG1("--URI is '%S'", &(aRequestUri.UriDes()));
	if (aRequestUri.UriDes().Length() > KMaxUriLength)
		TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EURIOverflow);
	iUriBuffer->Des().Copy(aRequestUri.UriDes());

	// Log headers
	__LOG("--Encoded WSP header is:")
	__DUMP("--", "--", aRequestHeaders)

	// Log body data
	__LOG("--Body data is:")
	__DUMP("--", "--", aRequestBody)

	// Get the next look-up table entry for the new transaction
	CNwssTransLookUpTable::CNwssTransLUTEntry& entry = iCOTransTable->NewEntry();
	entry.iCallback = &aMethodCallback;

	// Create the WSP transaction on the WAP stack, and handle any resulting error
	__TESTOOMD(error, iWspCOSession.CreateTransaction(method, *iUriBuffer, aRequestHeaders, aRequestBody, entry.iStackTrans));
	__LOG1("--RWSPCOConn::CreateTransaction() returned %d", error);

	// Handle OOM separately - we can recover from it.
	if (error == KErrNoMemory)
		{
		ScheduleDelayedOomMethodAbort(*(entry.iCallback), EFalse);
		return;
		}
	else
		// But any other error is due to a client programming error, so panic
		__ASSERT_ALWAYS(error == KErrNone, MapToPanic(error));

	// Get the transaction ID
	error = entry.iStackTrans.Id(entry.iStackTransID);
	__LOG1("--RWSPCOTrans::Id() returned %d", error);
	__ASSERT_ALWAYS(error == KErrNone, MapToPanic(error));
	__LOG1("--Transaction Id is %d", entry.iStackTransID);

	// Ensure the event dispatcher is waiting for stack events
	iEventDispatcher->Start();
	}

void CNwssWspCOSession::MethodInvokeDataReq(
									MWspCOMethodCallback&	/*aTransactionCallback*/,
									const TDesC8&			/*aRequestBody*/,
									const TDesC8&			/*aTrailerHeaders*/,
									TBool					/*aMoreData*/
									)	
	{
	__LOG("Attempt to send T-MethodInvokeData.req primitive failed because")
	__LOG("Large Data Transfer is not supported in this WAP Stack.")
	TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ELargeDataTransferNotSupported);
	}

void CNwssWspCOSession::MethodAbortReq(MWspCOMethodCallback& aMethodCallback)
	{
	__LOG("Sending T-MethodAbort.req primitive")

	// Check to see if the T-MethodInvoke.req primitive has been sent
	if( iInConnectLimbo )
		{
		__LOG("--In connect limbo - cannot send T-MethodAbort.req.")
		__LOG("--Delete waiting method")

		// Send the T-MethodAbort.ind
		iConnectGuard->SendMethodAbortInd();
		return;
		}

	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iCOTransTable->LookUpByCallback(aMethodCallback,
																  found);
	if (found)
		{
		__LOG1("--Transaction Id is %d", lutEntry.iStackTransID);

		// Abort the transaction - choose an abort reason from the WSP spec since
		// <wapcli.h> doesn't enumerate it  :(
		// Unfortunately the NWSS stack moves the state to 'Done' prematurely, so
		// check for that now.
		RWSPCOTrans::TState trState = RWSPCOTrans::EInit;
		__TESTOOMD(stkErr, lutEntry.iStackTrans.GetState(trState));
		__LOG1("--Transaction is in state %d", trState);
		__LOG1("--RWSPCOTrans::GetState() returned %d", stkErr);
		if (trState != RWSPCOTrans::EDone)
			{
			__TESTOOM(stkErr, lutEntry.iStackTrans.Abort(EUserReq));
			__LOG1("--RWSPCOTrans::Abort() returned %d", stkErr);

			// Ensure the event dispatcher is waiting for stack events
			iEventDispatcher->Start();
			}
		else
			{
			// The NWSS stack has already received the T-MethodResult.ind for this
			// transaction and it won't allow it to be aborted. But the client
			// expects the T-MethodAbort.ind to 'finish' the method.
			__LOG("--Cannot abort transaction, need to create T-MethodAbort.ind event");

			iTransEventFilter->SendMethodAbortIndEvent(lutEntry.iStackTransID);
			}
		__ASSERT_ALWAYS(stkErr == KErrNone,
			TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EFailedToAbortTrans));
		}
	}

void CNwssWspCOSession::MethodResultRes(
								MWspCOMethodCallback&	aMethodCallback, 
								const TDesC8&			aAckHeaders
								)
	{
	__LOG("Sending T-MethodResult.res primitive")
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iCOTransTable->LookUpByCallback(aMethodCallback, found);
	if (found)
		{
		__LOG1("--Transaction Id is %d", lutEntry.iStackTransID);

		// If the 'use acknowledgment headers' capability was negotiated with the
		// proxy, then do so now
		if (iUseAckHdrs)
			{
			__LOG("--Acknowledgement headers:");
			__DUMP("--", "--", aAckHeaders);
			__TESTOOMD(stkErr, lutEntry.iStackTrans.Acknowledge(aAckHeaders));
			__LOG1("--RWSPCOTrans::Acknowledge() returned %d", stkErr);
			__ASSERT_ALWAYS(stkErr == KErrNone,
				TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EFailedToAcknowledgeTrans));
			}

		// Now release the transaction
		__TESTOOMD(stkErr, lutEntry.iStackTrans.Release());
		__LOG1("--RWSPCOTrans::Release() returned %d", stkErr);

		// If release failed due to OOM, then send a MethodAbort-ind (OOM)
		if (stkErr == KErrNoMemory)
			{
			ScheduleDelayedOomMethodAbort(*(lutEntry.iCallback), EFalse);
			stkErr = KErrNone;
			}
		else
			{
			__ASSERT_ALWAYS(stkErr == KErrNone,
				TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EFailedToReleaseTrans));

			// Ensure the event dispatcher is waiting for stack events
			iEventDispatcher->Start();
			}

		// Remove the entry from the look-up table - this deletes the data supplier object
		iCOTransTable->RemoveEntry(*(lutEntry.iCallback));
		}
	}

void CNwssWspCOSession::MethodResultDataRes(
									MWspCOMethodCallback&	/*aMethodCallback*/, 
									const TDesC8&			/*aAckHeaders*/
									)
	{
	__LOG("Attempt to send T-MethodResultData.res primitive failed because")
	__LOG("Large Data Transfer is not supported in this WAP Stack.")
	TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ELargeDataTransferNotSupported);
	}

void CNwssWspCOSession::PrepareClientCapabilities()
	{
	__LOG("--Client's Requested Capabilities:");
	__LOG("----------------------------------");

	// Get all the capability handles from the client
	MWspCapabilityProvider& capsProv = iConnInfoProvider.CapabilityProvider();
	const MWspCapabilityViewer& cliCaps = capsProv.ClientCapabilities();

	// The NWSS WAP Stack doesn't support LDT, so cliMsgSize and svrMsgSize will be
	// ignored.  This transport handler doesn't currently support Push, so pushMOR
	// will be ignored.

	// Mask out all but the acknowledgement and suspend/resume capabilities -
	// we don't support push yet, and the current NWSS stack doesn't support LDT.
	TUint8 protOpts = cliCaps.GetProtocolOptions();
	protOpts &= (Wap::EAcknowledgementHeaders | Wap::ESessionResumeFacility);
	__LOG1("--Protocol options = %2x", protOpts);

	// Everything else is straightforward
	TUint8 methodMOR = cliCaps.GetMethodMOR();
	__LOG1("--Method MOR = %d", methodMOR);
	TUint32 cliSDUSize = cliCaps.GetClientSDUSize();
	__LOG1("--Client SDU Size = %d", cliSDUSize);
	TUint32 svrSDUSize = cliCaps.GetServerSDUSize();
	__LOG1("--Server SDU Size = %d", svrSDUSize);
	const MWspAliasAddresses& aliasAddrs = cliCaps.GetAliasAddresses();
	const MWspExtendedMethods& extMethods = cliCaps.GetExtendedMethods();
	const MWspHeaderCodePages& hdrCodePages = cliCaps.GetHeaderCodePages();
	const MWspUnknownCapabilities& unknownCaps = cliCaps.GetUnknownCapabilities();

	// Work around SDU size limitation in this WAP stack
	if (cliSDUSize > KMaxClientSDUSize)
		cliSDUSize = KMaxClientSDUSize;
	if (svrSDUSize > KMaxServerSDUSize)
		svrSDUSize = KMaxServerSDUSize;

	// Trival ones first
	iCapabilityCodec->SetMethodMOR(methodMOR);
	iCapabilityCodec->SetProtocolOptions(protOpts);
	iCapabilityCodec->SetClientSDUSize(cliSDUSize);
	iCapabilityCodec->SetServerSDUSize(svrSDUSize);

	// Alias addresses
	Wap::TWspBearer bearer;
	TUint16 port;
	TPtrC8 address;
	aliasAddrs.Start();
	while (aliasAddrs.GetNext(bearer, port, address) != KErrNotFound)
		{
		__LOG3("--Alias Address: bearer = %d, port = %d, address = %S", bearer, port, &address);
		TAliasAddress addr;
		addr.SetBearer((unsigned char)bearer);
		addr.SetPort(port);
		TInt err = addr.SetAddress(address);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		err = iCapabilityCodec->AddAliasAddress(addr);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		}

	// For the remaining non-trivial capabilities, we unfortunately have to make a copy
	// of the descriptor data into a writable buffer, since the CCapCodec API mandates
	// a TDes8-derived descriptor for data, even though only CCapCodec Set-fns are
	// being used!
	TBuf8<KMaxCapabilityDataSize> capData;
	
	// Extended methods
	TUint8 pduType;
	TPtrC8 methodName;
	extMethods.Start();
	while (extMethods.GetNext(pduType, methodName) != KErrNotFound)
		{
		__LOG2("--Extended Method: pduType = %d, methodName = %S", pduType, &methodName);
		__ASSERT_ALWAYS(methodName.Length() < KMaxCapabilityDataSize,
						TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ECapabilityOverrun));
		capData.Copy(methodName);
#ifdef _DEBUG
		TInt err =
#endif
			iCapabilityCodec->AddExtendedMethod(capData, pduType);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		}

	// Header code pages
	TUint8 pageCode;
	TPtrC8 pageName;
	hdrCodePages.Start();
	while (hdrCodePages.GetNext(pageCode, pageName) != KErrNotFound)
		{
		__LOG2("--Header Code Page: pageCode = %d, pageName = %S", pageCode, &pageName);
		__ASSERT_ALWAYS(pageName.Length() < KMaxCapabilityDataSize,
						TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ECapabilityOverrun));
		capData.Copy(pageName);
#ifdef _DEBUG
		TInt err =
#endif
			iCapabilityCodec->AddCodePage(capData, pageCode);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		}

	// Unknown capabilities
	TPtrC8 identifier;
	TPtrC8 value;
	unknownCaps.Start();
	while (unknownCaps.GetNext(identifier, value) != KErrNotFound)
		{
		__LOG2("--Unknown Capability: identifier = %S, value = %S", &identifier, &value);
		__ASSERT_ALWAYS(identifier.Length() < KMaxCapabilityDataSize,
						TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::ECapabilityOverrun));
		capData.Copy(identifier);
#ifdef _DEBUG
		TInt err =
#endif
			iCapabilityCodec->AddUnknownCap(capData, value);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		}
	__LOG("----------------------------------");
	}

void CNwssWspCOSession::SetNegotiatedCapabilitiesL()
	{
	__LOG("--Proxy's Negotiated Capabilities:");
	__LOG("----------------------------------");

	// Get the encoded capabilites from the session data, and pass to the codec
	__DEBUGTESTLEAVE
	HBufC8* encodedCaps = GetAllSessionDataLC(RWSPCOConn::ECapabilities);
	__LOG("--Encoded data:");
	__DUMP("--", "--", (*encodedCaps));
	CCapCodec* negCapsCodec = CCapCodec::NewL(*encodedCaps);
	CleanupStack::PushL(negCapsCodec);

	// Get all the capability handles from the client
	MWspCapabilityProvider& capsProv = iConnInfoProvider.CapabilityProvider();
	MWspCapabilitySetter& negCaps = capsProv.ServerCapabilities();
	negCaps.Reset(EAllCapabilities);

	// Trivial ones first - keep a copy of method MOR and protocol opts (acks flag)
	TUint32 protOpts;
	negCapsCodec->GetProtocolOptions(protOpts);
	TUint8 protOpts8 = STATIC_CAST(TUint8, protOpts);
	negCaps.SetProtocolOptions(protOpts8);
	__LOG1("--Protocol options = %2x", protOpts);
	iUseAckHdrs = ((protOpts & Wap::EAcknowledgementHeaders) == Wap::EAcknowledgementHeaders);
	//
	negCapsCodec->MethodMOR(iMethodMOR);
	negCaps.SetMethodMOR(iMethodMOR);
	__LOG1("--Method MOR = %d", iMethodMOR);
	//
	// We don't support the Message size capability, but we know the client might, so
	// set it to the same as the negotiated SDU size
	TUint32 size;
	negCapsCodec->ClientSDUSize(size);
	negCaps.SetClientSDUSize(size);
	negCaps.SetClientMessageSize(size);
	__LOG1("--Client SDU Size = %d", size);
	//
	negCapsCodec->GetServerSDUSize(size);
	negCaps.SetServerSDUSize(size);
	negCaps.SetServerMessageSize(size);
	__LOG1("--Server SDU Size = %d", size);

	// Now the more complex ones - alias addresses first
	TInt numAliasAddrs = 0;
	negCapsCodec->NumAliasAddress(numAliasAddrs);
	TInt idx = 0; 
	for (idx = 0; idx < numAliasAddrs; ++idx)
		{
		TAliasAddress address;
		TInt err = negCapsCodec->GetAliasAddress(idx, address);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		if (!err)
			{
			Wap::TWspBearer bearer = Wap::EIP;
			if (address.HasBearer())
				bearer = (Wap::TWspBearer)address.Bearer();
			TUint16 port = 0;
			if (address.HasPort())
				port = address.Port();
			__DEBUGTESTLEAVE
			negCaps.AddAliasAddressL(bearer, port, address.Address());
			__LOG3("--Alias Address: bearer = %d, port = %d, address = %S", bearer, port, &(address.Address()));
			}
		}

	// For the remaining non-trivial capabilities, we need a writable buffer to place
	// certain values into.  Sadly the CCapCodec API offers no means of finding out how
	// big particular values need to be before calling...
	TBuf8<KMaxCapabilityDataSize> capData;
	
	// Extended methods
	TInt numExtMethods = 0;
	negCapsCodec->NumExtendedMethods(numExtMethods);
	for (idx = 0; idx < numExtMethods; ++idx)
		{
		TUint8 val;
		TInt err = negCapsCodec->GetExtendedMethod(idx, capData, val);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		if (!err)
			{
			__DEBUGTESTLEAVE
			negCaps.AddExtendedMethodL(val, capData);
			__LOG2("--Extended Method: pduType = %d, methodName = %S", val, &capData);
			}
		}

	// Code pages
	TInt numCodePages = 0;
	negCapsCodec->NumCodePages(numCodePages);
	for (idx = 0; idx < numCodePages; ++idx)
		{
		TUint8 val;
		TInt err = negCapsCodec->GetCodePage(idx, capData, val);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		if (!err)
			{
			__DEBUGTESTLEAVE
			negCaps.AddHeaderCodePageL(val, capData);
			__LOG2("--Header Code Page: pageCode = %d, pageName = %S", val, &capData);
			}
		}

	// Unknown capabilities
	TInt numUnknownCaps = 0;
	negCapsCodec->NumUnkownCap(numUnknownCaps); // sic
	TBuf8<KMaxCapabilityDataSize> unknownCapVal;
	for (idx = 0; idx < numUnknownCaps; ++idx)
		{
		TInt err = negCapsCodec->GetUnknownCap(idx, capData, unknownCapVal);
		__ASSERT_DEBUG(err == KErrNone,
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInternalCapCodecError));
		if (!err)
			{
			__LOG2("--Unknown Capability: identifier = %S, value = %S", &unknownCapVal, &capData);
			__DEBUGTESTLEAVE
			negCaps.AddUnknownCapabilityL(unknownCapVal, capData);
			}
		}

	// Cleanup
	CleanupStack::PopAndDestroy(2, encodedCaps); // and negCapsCodec
	__LOG("----------------------------------");
	}

void CNwssWspCOSession::HandleDisconnectIndL()
	{
	__LOG("Received S-Disconnect.ind primitive")

	// Get the disconnection reason buffer from the session data
	TPckgBuf<TUint8> disconReasonBuff;
	TInt stkErr = GetSessionData(disconReasonBuff, RWSPCOConn::EDisconReason);
	if (stkErr == KErrNoMemory)
		{
		ScheduleDelayedOomDisconnect(EFalse);
		return;
		}
	__ASSERT_ALWAYS(stkErr == KErrNone, MapToPanic(stkErr));
	Wap::TWspReason disconReason = (Wap::TWspReason)disconReasonBuff();

	// Get the error header/body buffers from the session data
	__DEBUGTESTLEAVE
	HBufC8* errorHdrs = GetAllSessionDataLC(RWSPCOConn::EErrorHeaders);		
	__DEBUGTESTLEAVE
	HBufC8* errorBody = GetAllSessionDataLC(RWSPCOConn::EErrorBody);

	// Inform the transaction event filter that S-Disconnect.ind has been received.
	iTransEventFilter->SessionDisconnected();

	// Do it
	TWspRedirectedAddress emptyAddr;
	DoSendDisconnectInd(disconReason, EFalse, emptyAddr, *errorHdrs, *errorBody);

	// Clean up
	CleanupStack::PopAndDestroy(2, errorHdrs);
	}

void CNwssWspCOSession::DoSendDisconnectInd(Wap::TWspReason			aReason, 
										  TBool						aRedirectSecurity, 
										  TWspRedirectedAddress&	aRedirectAddress, 
										  const TDesC8&				aErrorHeader, 
										  const TDesC8&				aErrorBody)
	{
	// Send the disconnect indication _after_ informing the parent class that we're disconnected
	// This may seem to be the wrong order, but if there is any failure in the parent class to
	// maintain the session state correctly (e.g. due to OOM in the WAP Stack) then the disconnect
	// reason is transformed to one which indicates the state failure.
	// Notify the client of the session event
	TInt error = SessionDisconnectedCallback();
	if (error != KErrNone)
		aReason = Wap::ESessionStateFailure;

	iSessionCB.DisconnectInd(aReason, aRedirectSecurity, aRedirectAddress, aErrorHeader, aErrorBody);
	}

void CNwssWspCOSession::HandleSuspendIndL()
	{
	__LOG("Received S-Suspend.ind primitive")

	// Get the suspend reason buffer from the session data
	TPckgBuf<TUint8> suspendReasonBuff;
	TInt stkErr = GetSessionData(suspendReasonBuff, RWSPCOConn::ESuspendReason);
	if (stkErr == KErrNoMemory)
		{
		ScheduleDelayedOomDisconnect(EFalse);
		return;
		}
	__ASSERT_ALWAYS(stkErr == KErrNone, MapToPanic(stkErr));
	Wap::TWspReason suspendReason = (Wap::TWspReason)suspendReasonBuff();

	// Notify the client of the session event
	iSessionCB.SuspendInd(suspendReason);
	}

void CNwssWspCOSession::HandleResumeCnfL()
	{
	__LOG("Received S-Resume.cnf primitive")

	// Notify the client of the session event
	iSessionCB.ResumeCnf();
	}

void CNwssWspCOSession::HandleConnectCnfL()
	{
	__LOG("Received S-Connect.cnf primitive")

	// Inform the client of the negotiated capabilities
	__DEBUGTESTLEAVE
	SetNegotiatedCapabilitiesL();

	// Inform the client of the server's session headers
	MWspSessionHeadersProvider& prov = iConnInfoProvider.SessionHeadersProvider();
	__DEBUGTESTLEAVE
	HBufC8* serverHeaders = GetAllSessionDataLC(RWSPCOConn::EServerHeaders);
	prov.SetServerHeadersL(*serverHeaders);
	__LOG("--Server session headers are:")
	__DUMP("--", "--", (*serverHeaders))
	CleanupStack::PopAndDestroy(serverHeaders);
	
	// Resize the transaction look-up table to match the MethodMOR capability for
	// the session (which might be greater than the original table size)
	__DEBUGTESTLEAVE
	iCOTransTable->ResizeTableL(iMethodMOR);

	// Also, resize the trans Id table in the transaction filter.
	__DEBUGTESTLEAVE
	iTransEventFilter->ResizeTableL(iMethodMOR);

	// Notify the client of the session event
	iSessionCB.ConnectCnf();
	}

void CNwssWspCOSession::HandleRedirectIndL()
	{
	__LOG("Received S-Disconnect.ind (redirection) primitive")

	// Notify the client of the session event.  Note that this event
	// is really sent using DisconnectInd, as specified in [WSP]. The
	// NWSS wap stack has simplified things by presenting it as a new
	// event 'RedirectInd', which isn't defined in [WSP] at all.

	// Get the redirection options buffer from the session data
	TPckgBuf<TUint8> redirOptsBuff;
	TInt stkErr = GetSessionData(redirOptsBuff, RWSPCOConn::ERedirectOptions);
	if (stkErr == KErrNoMemory)
		{
		ScheduleDelayedOomDisconnect(EFalse);
		return;
		}
	__ASSERT_ALWAYS(stkErr == KErrNone, MapToPanic(stkErr));
	
	// Check for the permanent/temporary flag and the 'Reuse Security
	// Session' flag
	TBool permanentRedirect = ((redirOptsBuff() & 0x80) != 0);
	TBool redirectSecurity  = ((redirOptsBuff() & 0x40) != 0);

	// Set the disconnect reason to temporary/permanent redirection
	TWspReason disconReason = permanentRedirect?
							  EPermanentRedirectedProxy:
							  ETemporaryRedirectedProxy;

	// Get the redirection address(es) buffer from the session data
	__DEBUGTESTLEAVE
	HBufC8* redirAddr = GetAllSessionDataLC(RWSPCOConn::ERedirectAddresses);		

	// Convert the descriptor into a parsed form stored in iRedirectAddr
	__DEBUGTESTLEAVE
	ExtractRedirectedProxyInfoL(*redirAddr);

	// Don't bother with error headers or body for redirection.
	// Notify the client of the session event
	DoSendDisconnectInd(disconReason, redirectSecurity, iRedirectAddr,
							 KNullDesC8(), KNullDesC8());
	// Clean up
	CleanupStack::PopAndDestroy(redirAddr);
	}

void CNwssWspCOSession::HandleExceptionIndL()
	{
	__LOG("Received E-Exception.ind primitive")

	// Notify the client of the session event.  Note that the exception data is
	// not available from the NWSS WAP stack, so send empty data instead.
	iSessionCB.ExceptionInd(KNullDesC8());
	}

void CNwssWspCOSession::HandleMethodInvokeCnfL(RWSPCOTrans::TTransID aTransId)
	{
	__LOG("Received T-MethodInvoke.cnf primitive")

	// Look-up the transaction by ID
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iCOTransTable->LookUpByTransId(aTransId, found);
	__ASSERT_DEBUG(found,
			TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EEventOnUnknownTransaction));
	__LOG1("--Transaction Id is %d", lutEntry.iStackTransID);

	// Inform the client of the transaction event
	lutEntry.iCallback->MethodInvokeCnf();
	}

void CNwssWspCOSession::HandleMethodResultIndL(RWSPCOTrans::TTransID aTransId)
	{
	__LOG("Received T-MethodResult.ind primitive")

	// Look-up the transaction by ID
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iCOTransTable->LookUpByTransId(aTransId, found);
	__ASSERT_DEBUG(found,
			TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EEventOnUnknownTransaction));
	__LOG1("--Transaction Id is %d", lutEntry.iStackTransID);

	// Get the WSP status code.  This is expected to be a single byte, in one buffer.
	TPckgBuf<TUint8> status;
	TInt stkErr = GetTransactionData(lutEntry.iStackTrans, status, RWSPCOTrans::EResultStatus);
	if (stkErr == KErrNoMemory)
		{
		ScheduleDelayedOomMethodAbort(*(lutEntry.iCallback), EFalse);
		return;
		}
	
	// Get the response headers
	__DEBUGTESTLEAVE
	HBufC8* respHdrs = GetAllTransactionDataLC(lutEntry.iStackTrans, RWSPCOTrans::EResultHeaders);
	__LOG("--Result Header is:")
	__DUMP("--", "--", (*respHdrs))

	// Create a response body data supplier to stream the response body back to the
	// client.  Associate it with the transaction in the look-up table.
	lutEntry.iResponseBodyHandler = CNwssWspTrHndDataSupplier::NewL(lutEntry.iStackTrans, *this);

	// Inform the client of the transaction event
	lutEntry.iCallback->MethodResultInd(status(),
										*respHdrs,
										*(lutEntry.iResponseBodyHandler),
										EFalse);

	// Clean up
	CleanupStack::PopAndDestroy(respHdrs);
	}

void CNwssWspCOSession::HandleAbortIndL(RWSPCOTrans::TTransID aTransId)
	{
	__LOG("Received T-MethodAbort.ind primitive")

	// Look-up the transaction by ID
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iCOTransTable->LookUpByTransId(aTransId, found);
	__ASSERT_DEBUG(found,
			TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EEventOnUnknownTransaction));
	__LOG1("--Transaction Id is %d", lutEntry.iStackTransID);

	TInt stkErr = KErrNone;
#ifdef _DEBUG
	// Check the state
	RWSPCOTrans::TState trState = RWSPCOTrans::EInit;
	__TESTOOM(stkErr, lutEntry.iStackTrans.GetState(trState));
	__LOG1("--Transaction is in state %d", trState);
	__LOG1("--RWSPCOTrans::GetState() returned %d", stkErr);
#endif

	// Now release the transaction
	__TESTOOM(stkErr, lutEntry.iStackTrans.Release());
	__LOG1("--RWSPCOTrans::Release() returned %d", stkErr);
	__ASSERT_ALWAYS(stkErr == KErrNone,
		TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EFailedToReleaseTrans));

	// The WAP stack can't provide abort reasons, so we use a hardcoded one.
	// Inform the client of the aborted transaction
	lutEntry.iCallback->MethodAbortInd(Wap::EPeerReq);

	// Remove the entry from the look-up table - this deletes the data supplier object
	iCOTransTable->RemoveEntry(*(lutEntry.iCallback));
	}

void CNwssWspCOSession::ScheduleDelayedOomMethodAbort(MWspCOMethodCallback& aMethodToAbort,
													  TBool aAbortOnStack)
	{
	iAbortPckg.iSession  = CONST_CAST(CNwssWspCOSession*, this);
	iAbortPckg.iCallback = &aMethodToAbort;
	iAbortPckg.iAbortOnStack = aAbortOnStack;
  	TCallBack delayedMethodAbortCB(CNwssWspCOSession::DelayedSendOomMethodAbort, (TAny*)(&iAbortPckg));
  	iOOMCallBack->Set(delayedMethodAbortCB);
  	iOOMCallBack->CallBack();
	}

TInt CNwssWspCOSession::DelayedSendOomMethodAbort(TAny* aPtr)
  	{
	TDelayedMethodAbortPckg* pckg = REINTERPRET_CAST(TDelayedMethodAbortPckg*, aPtr);
  	CNwssWspCOSession* sess  = pckg->iSession;
	MWspCOMethodCallback* cb = pckg->iCallback;
	TBool stkAbort = pckg->iAbortOnStack;
	sess->SendOomMethodAbort(*cb, stkAbort);
  	return KErrNone;
	}

void CNwssWspCOSession::SendOomMethodAbort(MWspCOMethodCallback& aMethodToAbort, TBool /*aAbortOnStack*/)
	{
	// The abort reason is hardcoded to 'EOutOfMemory'.
	// Inform the client of the aborted transaction
	aMethodToAbort.MethodAbortInd(Wap::EOutOfMemory);

	// Remove the entry from the look-up table - this deletes the data supplier object
	iCOTransTable->RemoveEntry(aMethodToAbort);
	}

void CNwssWspCOSession::SendOomMethodAbort(RWSPCOTrans::TTransID aTransId, TBool aAbortOnStack)
	{
	// Look-up the transaction by ID
	TBool found = EFalse;
	CNwssTransLookUpTable::CNwssTransLUTEntry& lutEntry = iCOTransTable->LookUpByTransId(aTransId, found);
	if (found)
		{
		__LOG1("--Transaction Id is %d", lutEntry.iStackTransID);

		// Re-use the other variant
		SendOomMethodAbort(*(lutEntry.iCallback), aAbortOnStack);
		}
#ifdef _DEBUG
	else
		__LOG1("--Unknown transaction Id %d - ignoring OOM method abort", aTransId);
#endif
	}

void CNwssWspCOSession::ScheduleDelayedOomDisconnect(TBool aDisconnectOnStack)
	{
	iDisconnectPckg.iSession  = CONST_CAST(CNwssWspCOSession*, this);
	iDisconnectPckg.iDisconnectOnStack = aDisconnectOnStack;
  	TCallBack delayedDisconnectCB(CNwssWspCOSession::DelayedSendOomDisconnect, (TAny*)(&iDisconnectPckg));
  	iOOMCallBack->Set(delayedDisconnectCB);
  	iOOMCallBack->CallBack();
	}

TInt CNwssWspCOSession::DelayedSendOomDisconnect(TAny* aPtr)
  	{
	TDelayedDisconnectPckg* pckg = REINTERPRET_CAST(TDelayedDisconnectPckg*, aPtr);
  	CNwssWspCOSession* sess  = pckg->iSession;
	TBool stkDisconn = pckg->iDisconnectOnStack;
	sess->SendOomDisconnect(stkDisconn);
  	return KErrNone;
	}

void CNwssWspCOSession::SendOomDisconnect(TBool aDisconnectOnStack)
	{
	// First, disconnect from the stack if directed - necessary in some cases
	// to keep the client and the WAP stack's session state in sync.
	if (aDisconnectOnStack)
		DisconnectReq(Wap::EOutOfMemory);

	// Inform the transaction event filter that S-Disconnect.ind has been received.
	iTransEventFilter->SessionDisconnected();

	// The disconnect reason is hardcoded to 'EOutOfMemory'.
	// Inform the client that the session has been disconnected
	TWspRedirectedAddress emptyAddr;
	DoSendDisconnectInd(Wap::EOutOfMemory, EFalse, emptyAddr, KNullDesC8(),KNullDesC8());
	}

TInt CNwssWspCOSession::GetSessionData(TDes8& aBuffer,
									   RWSPCOConn::TSessionDataType aType)
	{
	__TESTOOMD(stkErr, iWspCOSession.GetSessionData(aBuffer, aType));
	__LOG1("--RWSPCOConn::GetSessionData() returned %d", stkErr);
	return stkErr;
	}

HBufC8* CNwssWspCOSession::GetAllSessionDataLC(RWSPCOConn::TSessionDataType aType)
	{
	// Allocate the buffer to be returned. This will be expanded as necessary.
	HBufC8* scratchBuff = HBufC8::NewLC(KSessDataChunkSize);
	TPtr8 scratchPtr = scratchBuff->Des();
	TPtr8 sessDataPtr = iSessDataBuffer->Des();

	// Get the initial chunk of session data
	__TESTOOMD(stkErr, iWspCOSession.GetSessionData(scratchPtr, aType));
	__LOG1("--RWSPCOConn::GetSessionData() returned %d", stkErr);
	while (stkErr == RWAPConn::EMoreData)
		{
		// The buffer wasn't big enough - try reallocating.  This will leave if
		// it fails to get a big enough chunk, and the working buffer will be
		// cleaned up on the way out.
		HBufC8* oldScratchBuff = scratchBuff;
		__DEBUGTESTLEAVE
		scratchBuff = oldScratchBuff->ReAllocL(scratchBuff->Length() +
											   KSessDataChunkSize);
		CleanupStack::Pop(oldScratchBuff);
		CleanupStack::PushL(scratchBuff);

		// There's more data to extract. Use the iSessionData buffer to get the
		// subsequent chunks
		__TESTOOM(stkErr, iWspCOSession.GetSessionData(sessDataPtr, aType));
		__LOG1("--RWSPCOConn::GetSessionData() returned %d", stkErr);
		if ((stkErr == KErrNone) || (stkErr == RWAPConn::EMoreData))
			{
			// Got some data - append it to the scratch buffer
			scratchPtr.Set(scratchBuff->Des());
			scratchPtr.Append(*iSessDataBuffer);
			}
		}

	// We don't deal with other errors from the WAP stack here - the final stkErr
	// value must be KErrNone indicating the final block of session data extracted OK.
	__DEBUGTESTLEAVE
	User::LeaveIfError(stkErr);
	return scratchBuff;
	}

TInt CNwssWspCOSession::GetTransactionData(RWSPCOTrans aTransaction,
										   TDes8& aBuffer,
										   RWSPCOTrans::TDataType aType)
	{
	TInt bytesRemaining = 0;
	__TESTOOMD(stkErr, aTransaction.GetData(aBuffer, aType, &bytesRemaining));
	__LOG1("--RWSPCOTrans::GetData() returned %d", stkErr);
	__ASSERT_ALWAYS(bytesRemaining == 0, TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EFailedToGetTransactionData));
	return stkErr;
	}

HBufC8* CNwssWspCOSession::GetAllTransactionDataLC(RWSPCOTrans aTransaction,
												   RWSPCOTrans::TDataType aType)
	{
	// Allocate the buffer to be returned. This will be expanded as necessary.
	HBufC8* scratchBuff = HBufC8::NewLC(KSessDataChunkSize);
	TPtr8 scratchPtr = scratchBuff->Des();
	TPtr8 sessDataPtr = iSessDataBuffer->Des();

	// Get the initial chunk of session data
	TInt bytesRemaining = 0;
	__TESTOOMD(stkErr, aTransaction.GetData(scratchPtr, aType, &bytesRemaining));
	__LOG1("--RWSPCOTrans::GetData() returned %d", stkErr);
	while (stkErr == RWAPConn::EMoreData)
		{
		// The buffer wasn't big enough - try reallocating.  This will leave if
		// it fails to get a big enough chunk, and the working buffer will be
		// cleaned up on the way out.  Try to get all the rest in one go.
		HBufC8* oldScratchBuff = scratchBuff;
		__DEBUGTESTLEAVE
		scratchBuff = oldScratchBuff->ReAllocL(scratchBuff->Length() +
											   bytesRemaining);
		CleanupStack::Pop(oldScratchBuff);
		CleanupStack::PushL(scratchBuff);

		// There's more data to extract. Use the iSessionData buffer to get the
		// subsequent chunks
		__TESTOOM(stkErr, aTransaction.GetData(sessDataPtr, aType, &bytesRemaining));
		__LOG1("--RWSPCOTrans::GetData() returned %d", stkErr);
		if ((stkErr == KErrNone) || (stkErr == RWAPConn::EMoreData))
			{
			// Got some data - append it to the scratch buffer
			scratchPtr.Set(scratchBuff->Des());
			scratchPtr.Append(*iSessDataBuffer);
			}
		}

	// We don't deal with other errors from the WAP stack here - the final stkErr
	// value must be KErrNone indicating the final block of session data extracted OK.
	__DEBUGTESTLEAVE
	User::LeaveIfError(stkErr);
	return scratchBuff;
	}

void CNwssWspCOSession::ExtractRedirectedProxyInfoL(const TDesC8& aRedirectedAddresses)
	{
	const TUint8 KBearerMask			= 0x80;
	const TUint8 KPortMask				= 0x40;
	const TUint8 KAddressLengthMask		= 0x3F;
	const TInt KPortLength				= 2;
	const TUint8 KBearerAny				= 0x00;
	const TUint8 KBearerSMS				= 0x03;
	const TUint8 KBearerCSD				= 0x0A;
	const TUint8 KBearerGPRS			= 0x0B;
	const TInt KByteLength				= 8;

	// Extract bearer and port presence and address length from first byte
	TInt index = 0;
	TUint8 byte = aRedirectedAddresses[index];
	iRedirectAddr.iHasBearer = byte & KBearerMask;
	iRedirectAddr.iHasPort = byte & KPortMask;
	TInt addressLength = byte & KAddressLengthMask;

	++index;

	// Extract bearer and port - update the length of data
	TInt dataLength = index + addressLength;
	if( iRedirectAddr.iHasBearer )
		{
		// Bearer present - increment length
		++dataLength;
		}
	if( iRedirectAddr.iHasPort )
		{
		// Port present - increase length by KPortLength
		dataLength += KPortLength;
		}

	// Check to see if there is enough data - if not 'aRedirectedAddresses' wasn't well-formed.
	if( dataLength > aRedirectedAddresses.Length() )
		{
		// Oops not enough data - leave
		__LOG("--Redirect data is corrupt - not enough to extract an address")
		__DEBUGTESTLEAVE
		User::Leave(KWspErrBadRedirectedAddressData);
		}

	// Is there a bearer
	if( iRedirectAddr.iHasBearer )
		{
		TUint8 bearerType = aRedirectedAddresses[index];
		switch (bearerType)
			{
			case KBearerSMS:
				{
				// According to WDP spec, this is GMS network using SMS bearer
				iRedirectAddr.iBearer = Wap::EWAPSMS;
				__LOG("--Redirected to bearer WAPSMS");
				} break;
			case KBearerCSD:
			case KBearerGPRS:
			case KBearerAny:
				{
				// According to WDP spec, this is GMS network using CDS bearer 
				// (0x0A), or using a GPRS bearer (0x0B).
				iRedirectAddr.iBearer = Wap::EIP;
#ifdef _DEBUG
				if (bearerType == KBearerCSD)
					__LOG("--Redirected to bearer CSD - using IPv4")
				else if (bearerType == KBearerGPRS)
					__LOG("--Redirected to bearer GPRS - using IPv4")
				else if (bearerType == KBearerAny)
					__LOG("--Redirected to bearer 'any' - using IPv4")
#endif 
				} break;
			default:
				{
				// Bearer not supported - leave
				__LOG("--Redirected to unknown bearer");
				__DEBUGTESTLEAVE
				User::Leave(KWspErrBearerNotSupported);
				} break;
			}
		++index;
		}
	// Is there a port
	if( iRedirectAddr.iHasPort )
		{
		// Get the port -  this is 16 bit number...
		iRedirectAddr.iPort = STATIC_CAST(TUint16, (aRedirectedAddresses[index] << KByteLength) + aRedirectedAddresses[index+1]);
		__LOG1("--Redirected to port %d", iRedirectAddr.iPort)
		index += KPortLength;
		}

	// Get the proxy address, and format according to the bearer
	TPtrC8 proxyAddress = aRedirectedAddresses.Mid(index, addressLength);
	if (iRedirectAddr.iBearer == Wap::EIP)
		{
		if (addressLength < 4)
			{
			// Oops not enough data - leave
			__DEBUGTESTLEAVE
			User::Leave(KWspErrBadRedirectedAddressData);
			}
		iRedirectAddr.iProxyAddress.Format(_L8("%i.%i.%i.%i"),
											proxyAddress[0],
											proxyAddress[1],
											proxyAddress[2],
											proxyAddress[3]);
		}
	else // do a straight copy for anything else since not sure what to expect - a phone number for SMS?
		iRedirectAddr.iProxyAddress.Copy(proxyAddress);

	__LOG1("--Redirected to proxy address %S", &(iRedirectAddr.iProxyAddress))
	}
