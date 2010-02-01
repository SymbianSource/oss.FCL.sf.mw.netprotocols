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

/**
 @file CNwssWspCOSession.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__CNWSSWSPCOSESSION_H__
#define __CNWSSWSPCOSESSION_H__

// System includes
#include <wapcli.h> // NWSS Wap Stack
#include <wsp/mwspcosessioninvoker.h>
#include <wsp/mwspcomethodinvoker.h>
#include <http/framework/logging.h>

// Local includes
#include "cnwsswspsession.h"
#include "cnwsstranslookuptable.h"
#include "mnwsssessioneventhandler.h"
#include "mnwsstransactioneventhandler.h"
#include "mnwssoomhandler.h"

// Forward declarations
class MNwssWapServer;
class MConnectionInfoProvider;
class MWspCOSessionCallback;
class CNwssWspCOEventDispatcher;
class CCapCodec;
class CNwssConnectGuard;
class CNwssWspTransactionEventFilter;
class CNwssWspSessionEventFilter;

/**
	@since			7.0
	This class encapsulates a Connection-Oriented WSP session by specialising
	CNwssWspSession.  It is responsible for making the majority of the calls on the
	underlying NWSS WAP protocol stack.  It has five main areas of functionality:
	1.	Opening, and connecting, the handle for a WSP CO session using the proxy
		information supplied by the client (::OpenWspSession() and
		::CompleteProxyConnectionL())
	2.	Converting the requested client capabilities from the M-class interfaces
		supplied by the client into a CCapCodec object, as used by the NWSS WAP
		stack (::PrepareClientCapabilities())
	3.	Doing the reverse for negotiated capabilities that are received from the
		WAP Proxy before passing back to the client (::SetNegotiatedCapabilitiesL()).
	4.	Implementing the MWspCOSessionInvoker and MWspCOMethodInvoker interfaces,
		and translating each of the primitives exposed by these interfaces into
		calls on the underlying NWSS WAP Stack.
	5.	Implementing the MNwssSessionEventHandler and MNwssTransactionEventHandler
		interfaces, in order that events received from the underlying WAP Stack (by
		the class CNwssWspCOEventDispatcher) may be converted into the primitives
		exposed by the client's callback interfaces MWspCOSessionCallback and
		MWspCOMethodCallback.
 */

//##ModelId=3BBDCF00036A
class CNwssWspCOSession : public CNwssWspSession,
						  public MWspCOSessionInvoker,
						  public MWspCOMethodInvoker,
						  public MNwssSessionEventHandler,
						  public MNwssTransactionEventHandler,
						  public MNwssOomHandler
	{
// Unit testing only - give friendship to a public state accessor 
#ifdef __UNIT_TESTING__
	friend class TNwssWspCOSession_StateAccessor;
#endif

public: // methods

/**
	Factory construction of the CNwssWspCOSession class.
	@since			7.0
	@param			aStringPool			(in) The client's string pool.
	@param			aWapStackProvider	(in) An owner of a WAP stack server session.
	@param			aConnInfoProvider	(in) A collection of client interfaces that
											 provide connection-related information
	@param			aSecurityPolicy		(in) A security policy provider.
	@param			aSessionCB			(in) The client's callback for receiving
											 WSP session events
	@return			The newly constructed CNwssWspCOSession instance.
	@leave			System error codes, e.g. KErrNoMemory.
*/
	//##ModelId=3C4C49D202E6
	static CNwssWspCOSession* NewL(RStringPool& aStringPool,
								   MNwssWapServer& aWapStackProvider,
								   MConnectionInfoProvider& aConnInfoProvider,
								   MSecurityPolicy& aSecurityPolicy,
								   MWspCOSessionCallback& aSessionCB);

/** Destructor.
	@since			7.0
*/
	//##ModelId=3C4C49D2021E
	virtual ~CNwssWspCOSession();

private: // methods inherited from CNwssWspSession

/** Hook provided to the parent class allowing it to cause a
					Connection-Oriented WSP session to the specified proxy to be
					opened.
	@since			7.0
	@param			aRemoteHost	(in) The bearer-dependent address of the proxy.
	@param			aRemotePort	(in) The proxy port to which a connection is made.
	@param			aLocalPort	(in) (optional) The local port to which proxy replies
									 are sent. A value of zero means 'don't care'.
	@param			aBearer		(in) The bearer on which the connection is made.
	@param			aSecureConn	(in) Flag indicating whether WTLS is to be used.
	@return			A system error code originating from the NWSS WAP Stack.
 */
	//##ModelId=3C4C49D10014
	virtual TInt OpenWspSession(const TDesC8& aRemoteHost,
								RWAPConn::TPort aRemotePort,
								RWAPConn::TPort aLocalPort,
								TBearer aBearer,
								TBool aSecureConn);

/** Hook provided to the parent class allowing it to cause an
					opened Connection-Oriented WSP session to be closed.
	@since			7.0
	@return			A system error code originating from the NWSS WAP Stack.
 */
	//##ModelId=3C591A9A0171
	virtual TInt CloseWspSession();

/** 	Hook provided to the parent class allowing it to perform any
					required actions to complete a connection to the WAP Proxy
					after the WSP session has been opened, and optionally a WTLS
					handshake performed.
					For connection-oriented WSP, this implies the use of
					RWSPCOConn::Connect() to send the S-Connect.cnf primitive to
					the proxy.  This initiates the connection, with an exchange of
					capability information and session headers.
	@since			7.0
	@leave			A system error code originating from the NWSS WAP Stack.
 */
	//##ModelId=3C4C49D002F7
	virtual void CompleteProxyConnectionL();

/** Hook provided to the parent class allowing it to obtain the
					WTLS handle from the current opened Connection-Oriented WSP
					session.
	@since			7.0
	@return			A handle for the WTLS layer of the WAP protocol stack.
 */
	//##ModelId=3C4C49D001FD
	virtual RWTLS WtlsHnd();

/** Allows this class to to perform specific actions in response to
					the parent class' RunError().  This is called in the EReady
					state.
					This handles situations where CompleteProxyConnectionL() has
					left.  In practice, this handles errors arising from the use
					of RWSPCOConn::Connect().
	@since			7.0
	@param			aError	(in) The error code to be handled, with which
					CompleteProxyConnectionL() left.
 */
	//##ModelId=3C4C49D0001C
	virtual void DoRunError(TInt aError);

/** Called from DoCancel ofthe parent class' RunError() in the
					EReady state.  This class cancels outstanding asynchronous
					requests it makes from the Event Dispatcher.
	@since			7.0
	@return			Flag indicating whether, following cancellation, this class
					must return to the EDisconnected state.
 */
	//##ModelId=3C591A9A0103
	virtual TBool SubDoCancel();

private: // methods inherited from MWspCOSessionInvoker

	/** This represents the S-Connect.req primitive. 
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CF030A
	virtual void ConnectReq();

	/** This represents the S-Disconnect.req primitive.
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CF0133
	virtual void DisconnectReq(TWspReason aReason);

	/** This represents the S-Suspend.req primitive.
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CF0043
	virtual void SuspendReq();

	/** This represents the S-Resume.req primitive. 
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CE0345
	virtual void ResumeReq();

private: // methods inherited from MWspCOMethodInvoker

	/** This represents the S-MethodInvoke.req primitive.
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CD01D1
	virtual void MethodInvokeReq(
								MWspCOMethodCallback&		aTransactionCallback, 
								RStringF					aMethod, 
								const TUriC8&				aRequestUri, 
								const TDesC8&				aRequestHeaders, 
								const TDesC8&				aRequestBody,
								TBool						aMoreData
								);

	/** This represents the S-MethodInvokeData.req primitive. 
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CC031A
	virtual void MethodInvokeDataReq(
									MWspCOMethodCallback&	aTransactionCallback,
									const TDesC8&			aRequestBody,
									const TDesC8&			aTrailerHeaders,
									TBool					aMoreData
									);

	/** This represents the S-MethodAbort.req primitive.
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CC0161
	virtual void MethodAbortReq(MWspCOMethodCallback& aTransactionCallback);

	/** This represents the S-MethodResult.res primitive.
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CB02BE
	virtual void MethodResultRes(
								MWspCOMethodCallback&	aTransactionCallback, 
								const TDesC8&				aAckHeaders
								);

	/** This represents the S-MethodResultData.res primitive. 
		@see MWspCOSessionInvoker
	*/
	//##ModelId=3C4C49CB0047
	virtual void MethodResultDataRes(
									MWspCOMethodCallback&	aTransactionCallback, 
									const TDesC8&				aAckHeaders
									);


private: // methods inherited from MNwssSessionEventHandler,

	/** This represents the S-Disconnect.ind primitive.
		@see MNwssSessionEventHandler
	*/
	//##ModelId=3C4C49CA035D
	virtual void HandleDisconnectIndL();

	/** This represents the S-Suspend.ind primitive.
		@see MNwssSessionEventHandler
	*/
	//##ModelId=3C4C49CA0231
	virtual void HandleSuspendIndL();

	/** This represents the S-Resume.cnf primitive.
		@see MNwssSessionEventHandler
	*/
	//##ModelId=3C4C49CA0168
	virtual void HandleResumeCnfL();

	/** This represents the S-Connect.cnf primitive.
		@see MNwssSessionEventHandler
	*/
	//##ModelId=3C4C49CA00AA
	virtual void HandleConnectCnfL();

	/** This represents the S-Disconnect.ind (redirect) primitive.
		@see MNwssSessionEventHandler
	*/
	//##ModelId=3C4C49C903D4
	virtual void HandleRedirectIndL();

	/** This represents the E-Exception.ind primitive.
		@see MNwssSessionEventHandler
	*/
	//##ModelId=3C4C49C9031F
	virtual void HandleExceptionIndL();

private: // methods inherited from MNwssTransactionEventHandler

	/** This represents the T-MethodInvoke.cnf primitive.
		@see MNwssTransactionEventHandler
	*/
	//##ModelId=3C4C49C901AD
	virtual void HandleMethodInvokeCnfL(RWSPCOTrans::TTransID aTransId);

	/** This represents the T-MethodResult.ind primitive.
		@see MNwssTransactionEventHandler
	*/
	//##ModelId=3C4C49C90044
	virtual void HandleMethodResultIndL(RWSPCOTrans::TTransID aTransId);

	/** This represents the T-MethodAbort.ind primitive.
		@see MNwssTransactionEventHandler
	*/
	//##ModelId=3C4C49C802CE
	virtual void HandleAbortIndL(RWSPCOTrans::TTransID aTransId);

private: // structure used to hold items sent to the delayed OOM handling for methodabort below

	struct TDelayedMethodAbortPckg
		{
		CNwssWspCOSession* iSession;
		MWspCOMethodCallback* iCallback;
		TBool iAbortOnStack;
		};
	//##ModelId=3C9B09540101
	void ScheduleDelayedOomMethodAbort(MWspCOMethodCallback& aMethodToAbort, TBool aAbortOnStack);
	//##ModelId=3C9B0954001A
	static TInt DelayedSendOomMethodAbort(TAny* aPtr);

private: // structure used to hold items sent to the delayed OOM handling for disconnect below

	struct TDelayedDisconnectPckg
		{
		CNwssWspCOSession* iSession;
		TBool iDisconnectOnStack;
		};
	//##ModelId=3C9B09530344
	void ScheduleDelayedOomDisconnect(TBool aDisconnectOnStack);
	//##ModelId=3C9B09530268
	static TInt DelayedSendOomDisconnect(TAny* aPtr);

private: // methods inherited from MNwssOomHandler

/**		Used as an asynchronous callback, in an OOM situation, to cause the method that
					couldn't complete at the moment of OOM, to be cleanly aborted for the client.
	@param			aMethodToAbort	(in) The method which is to be aborted due to the OOM situation
	@param			aAbortOnStack	(in) Flag indicating whether the method must be aborted on the
										 WAP stack as well.
	@since			7.0
 */
	//##ModelId=3C9B095203C5
	virtual void SendOomMethodAbort(MWspCOMethodCallback& aMethodToAbort, TBool aAbortOnStack);

/**	Used as an asynchronous callback, in an OOM situation, to cause the method that
					couldn't complete at the moment of OOM, to be cleanly aborted for the client.
	@param			aTransId		(in) A WAP stack transaction ID that identifies the method which
										 is to be aborted due to the OOM situation
	@param			aAbortOnStack	(in) Flag indicating whether the method must be aborted on the
										 WAP stack as well.
	@since			7.0
 */
	//##ModelId=3C9B09530131
	virtual void SendOomMethodAbort(RWSPCOTrans::TTransID aTransId, TBool aAbortOnStack);

/**		Used as an asynchronous callback, in an OOM situation, to cause the session that
					couldn't connect at the moment of OOM, to be cleanly disconnected for the client.
	@param			aDisconnectOnStack	(in) Flag indicating whether the session must be disconnected
											 on the WAP stack as well.
	@since			7.0
 */
	//##ModelId=3C9B095202F2
	virtual void SendOomDisconnect(TBool aDisconnectOnStack);

private: // methods

/** Normal constructor.  Creates an instance of the session, keeping
					references to the supplied providers from the client.
	@since			7.0
	@param			aStringPool			(in) The client's string pool.
	@param			aWapStackProvider	(in) An owner of a WAP stack server session.
	@param			aConnInfoProvider	(in) A collection of client interfaces that
											 provide connection-related information
	@param			aSecurityPolicy		(in) A security policy provider.
	@param			aSessionCB			(in) The client's callback for receiving
											 WSP session events
 */
	//##ModelId=3C4C49C702A4
	CNwssWspCOSession(RStringPool& aStringPool,
					  MNwssWapServer& aWapStackProvider,
				      MConnectionInfoProvider& aConnInfoProvider,
					  MSecurityPolicy& aSecurityPolicy,
					  MWspCOSessionCallback& aSessionCB);

/**	Second phase of construction.  Allocations may be made here.
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory.
 */
	//##ModelId=3C4C49C70236
	void ConstructL();

/** 	Helper to ConnectReq, configures a capability codec to reflect
					the capabilities proposed by the client.
	@since			7.0
 */
	//##ModelId=3C4C49C701C8
	void PrepareClientCapabilities();

/**	Helper to HandleConnectCnf, uses the capability codec to return
					the capabilities negotiated with the proxy.
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory.
 */
	//##ModelId=3C4C49C7015A
	void SetNegotiatedCapabilitiesL();

/** Helper in the extraction of WSP session data from the NWSS WAP
					stack.  Should be used when the data size is known in advance
					and may be passed as a buffer like TPckgBuf<TUint> for example.
	@warning		It will Panic the client if there is more data available than
					the buffer can hold!
	@since			7.0
	@param			aBuffer		(in) The client's pre-allocated buffer
	@param			aType		(in) The type of the session data to be extracted
	@return			A WAP stack error code
 */
	//##ModelId=3C4C49C6037F
	TInt GetSessionData(TDes8& aBuffer, RWSPCOConn::TSessionDataType aType);

/** Helper in the extraction of WSP session data from the NWSS WAP
					stack.  This method retrieves all data of a named type,
					allocating a heap descriptor large enough to hold it all.
					Should be used for data of unknown size.
	@since			7.0
	@param			aType		(in) The type of the session data to be extracted
	@return			A heap-allocated 8-bit buffer to contain the session data. This
					buffer is left on the clean-up stack.
	@leave			System error codes, e.g. KErrNoMemory
 */
	//##ModelId=3C4C49C6025D
	HBufC8* GetAllSessionDataLC(RWSPCOConn::TSessionDataType aType);

/**	Helper in the extraction of WSP transaction data from the NWSS
					WAP stack.  Should be used when the data size is known in
					advance and may be passed as a buffer like TPckgBuf<TUint> for
					example.
	@warning		It will Panic the client if there is more data available than
					the buffer can hold!
	@since			7.0
	@param			aTransaction(in) The stack transaction handle
	@param			aBuffer		(in) The client's pre-allocated buffer
	@param			aType		(in) The type of the transaction data to be extracted
	@return			A WAP stack error code
 */
	//##ModelId=3C4C49C503CE
	TInt GetTransactionData(RWSPCOTrans aTransaction,
							TDes8& aBuffer,
							RWSPCOTrans::TDataType aType);

/** Helper in the extraction of WSP transaction data from the NWSS
					WAP stack.  This method retrieves all data of a named type,
					allocating a heap descriptor large enough to hold it all.
					Should be used for data of unknown size.
	@since			7.0
	@param			aTransaction(in) The stack transaction handle
	@param			aType		(in) The type of the transaction data to be extracted
	@return			A heap-allocated 8-bit buffer to contain the transaction data. This
					buffer is left on the clean-up stack.
	@leave			System error codes, e.g. KErrNoMemory
 */
	//##ModelId=3C4C49C5020B
	HBufC8* GetAllTransactionDataLC(RWSPCOTrans aTransaction,
									RWSPCOTrans::TDataType aType);

/** Utility to convert a buffer received as the 'redirect addresses'
					after an S-Disconnect.ind (Redirect) event, into the structure
					used in the adaptation API to represent a redirect address. The
					address is written into the structure iRedirectAddr.
	@since			7.0
	@param			aRedirectedAddresses (in) The raw WSP-encoded address data
	@leave			System error codes, e.g. KErrNoMemory
 */
	//##ModelId=3C591A9A0027
	void ExtractRedirectedProxyInfoL(const TDesC8& aRedirectedAddresses);

	//##ModelId=3C9B0952008F
	void DoSendDisconnectInd(Wap::TWspReason			aReason, 
							  TBool						aRedirectSecurity, 
							  TWspRedirectedAddress&	aRedirectAddress, 
							  const TDesC8&				aErrorHeader, 
							  const TDesC8&				aErrorBody);

private: // type definitions

	/** An enumeration of the reasons for aborting a transaction. Not all might
		might have meaning for both CO and CL WSP sessions.  Note that this type
		should be in <wapcli.h> but was omitted there for some reason.
	 */
	enum TAbortReason 
		{
		EUnknown = -1,
		EWapStackTimeout = 8,
		// abort reason code assignments - taken from NWSS source file wps/source/wsp/common/common_dmu,h)
		EProtoErr      =  0xe0,
		EDisconnect    =  0xe1,
		ESuspend       =  0xe2,
		EResume        =  0xe3,
		ECongestion    =  0xe4,
		EConnectErr    =  0xe5,
		EMRUExceeded   =  0xe6,
		EMORExceeded   =  0xe7,
		EPeerReq       =  0xe8,
		ENetErr        =  0xe9,
		EUserReq       =  0xea
		};

private: // attributes

	/** Connection-oriented WAP Stack session
	*/
	//##ModelId=3C4C49C50201
	RWSPCOConn iWspCOSession;

	/** The transaction look-up table
	*/
	//##ModelId=3C4C49C501ED
	CNwssTransLookUpTable* iCOTransTable;

	/** The client's callback for session events
	*/
	//##ModelId=3C4C49C501D9
	MWspCOSessionCallback& iSessionCB;

	/** A hub to receive events from the WAP stack
	*/
	//##ModelId=3C4C49C501C5
	CNwssWspCOEventDispatcher* iEventDispatcher;

	/** A codec for client and server capabilities
	*/
	//##ModelId=3C4C49C501B1
	CCapCodec* iCapabilityCodec;

	/** A scratch buffer used for URLs, which must be converted from 8-bit to Unicode
		when submitting transactions to the WAP stack
	*/
	//##ModelId=3C4D9875006F
	HBufC* iUriBuffer;

	/** A scratch buffer used for retrieving session data
	*/
	//##ModelId=3C4D9865036F
	HBufC8* iSessDataBuffer;

	/** The negotiated Method Maximum Outstanding Requests (MOR) value
	*/
	//##ModelId=3C4D984301D6
	TUint8 iMethodMOR;

	/** The negotiated 'Use Acknowledgement Headers' protocol option
	*/
	//##ModelId=3C4D982503C7
	TBool iUseAckHdrs;

	/** A parsed redirect address
	*/
	//##ModelId=3C591A9903C9
	TWspRedirectedAddress iRedirectAddr;

	/** A flag indicating that S-Connect.req has been sent by the client but has
		yet to be sent to the WAP Stack.
	*/
	//##ModelId=3C9B0952007B
	TBool	iInConnectLimbo;

	/** Object that handles the connect limbo state.
	*/
	//##ModelId=3C9B09520053
	CNwssConnectGuard*	iConnectGuard;

	/** Object used to filter transaction events from the NWSS stack and to ensure
		that T-MethodAbort.ind events are sent for transactions in EDone state.
	*/
	//##ModelId=3C9B0952002B
	CNwssWspTransactionEventFilter* iTransEventFilter;

	/** Object used to filter session events from the NWSS stack to ensure WSP
		spec compliance.		
	*/
	//##ModelId=3C9B0952000D
	CNwssWspSessionEventFilter* iSessionEventFilter;

	/** Object used to get an asynchronous, one-off callback, in order to break the callstack
		from the client when OOM forces us to abort their T-MethodInvoke.req
	*/
	//##ModelId=3C9B095103CD
	CAsyncCallBack* iOOMCallBack;

	/** Structure that holds some info needed by the TCallback method that sends a deferred
		T-MethodAbort.ind in OOM situations experienced during a T-MethodInvoke.req
	*/
	//##ModelId=3C9B095103A5
	TDelayedMethodAbortPckg iAbortPckg;

	/** Structure that holds some info needed by the TCallback method that sends a deferred
		S-Disconnect.ind in OOM situations experienced during a S-Disconnect.req
	*/
	//##ModelId=3C9B09510387
	TDelayedDisconnectPckg iDisconnectPckg;
	};


#endif // __CNWSSWSPCOSESSION_H__
