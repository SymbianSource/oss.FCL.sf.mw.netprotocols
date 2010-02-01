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

#ifndef	__CWSPCOPROTOCOLHANDLER_H__
#define __CWSPCOPROTOCOLHANDLER_H__

// System includes
#include <e32base.h>
#include <http/framework/cprotocolhandler.h>
#include <wsp/mwspcosessioncallback.h>
#include <wsp/mwspcapabilityprovider.h>
#include <wsp/mwspsessionheadersprovider.h>

// User includes
#include "mconnectiontimercallback.h"
#include "mwspprimitivesendercallback.h"
#include "mwspcomethodobserver.h"

// Forward declarations
class CWspCapabilityInfo;
class CWspProxyInfoProvider;
class CWspTransportHandler;
class MWspCOSessionInvoker;
class MWspCOMethodInvoker;
class CConnectionTimer;
class CWspPrimitiveSender;
class CRxData;
class CWspHeaderUtils;

// @todo docing
//##ModelId=3BBD7DB402C4
class CWspCOProtocolHandler : public CProtocolHandler, public MWspCOSessionCallback,
													   public MWspCapabilityProvider,
													   public MWspSessionHeadersProvider,
													   public MConnectionTimerCallback,
													   public MWspPrimitiveSenderCallback,
													   public MWspCOMethodObserver,
													   public MProtHandlerInterface
	{
public:	// Methods

/**	Static factory constructor.
	@since			7.0
	@leave			Leaves with KErrNoMemory if there was not enough memory to
					create the object.
	@param			aSession	A pointer to an RHTTPSession object.
	@return			A pointer to newly created object.
*/
	//##ModelId=3C4C439101F4
	static CWspCOProtocolHandler* NewL(TAny* aSession);

/** Standard destructor.
	@since			7.0
*/
	//##ModelId=3C4C439101D6
  ~CWspCOProtocolHandler();

private:	// Methods from CProtocolHandler

/** 
	@see			CProtocolHandler
*/
	//##ModelId=3C4C43910172
	virtual void CreateCodecL();

/** 
	@see			CProtocolHandler
*/
	//##ModelId=3C4C439100BD
	virtual CProtTransaction* CreateProtTransactionL(RHTTPTransaction aTransaction);

/** 
	@see			CProtocolHandler
*/
	//##ModelId=3C4C43910009
	virtual TBool ServiceL(CProtTransaction& aTrans);

/** 
	@see			CProtocolHandler
*/
	//##ModelId=3C4C4390033D
	virtual void ClosedTransactionHook(CProtTransaction* aTrans);

/** 
	@see			CProtocolHandler
*/
	//##ModelId=3C4C4390029D
	virtual void CancelTransactionHook(CProtTransaction& aTransaction);

/** 
	@see			CProtocolHandler
*/
	//##ModelId=3C4C439001F2
	virtual void NotifyNewRequestBodyPart(CProtTransaction& aTransaction);

	virtual void GetInterfaceL(TUid aInterfaceId, MProtHandlerInterface*& aInterfacePtr);

	/** Get the Server Certificate for the current session.
	@param	aServerCert A TCertInfo which will be filled with the certificate information
	@return	An error code.  KErrNone if aServerCert has been completed, otherwise one of 
			the system wide error codes
	*/
	TInt SessionServerCert(TCertInfo& aServerCert);

	/** Get the Server Certificate for the specified transaction.  This function returns
	KErrNotSupported because for WSP the certificate applies to the whole session.
	@param	aServerCert A TCertInfo which will be filled with the certificate information
	@param	aTransaction The transaction for which the certificate info is requested
	@return	KErrNotSupported
	*/
	TInt TransactionServerCert(TCertInfo& aServerCert, RHTTPTransaction aTransaction);


private:	// Methods from MHTTPFilterBase

/**
	@see			MHTTPFilterBase
*/
	//##ModelId=3C4C43900152
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);

/**
	@see			MHTTPFilterBase
*/
	//##ModelId=3C4C43900076
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

private:	// Methods from MWspCOSessionCallback

/**
	@see			MWspCOSessionCallback
*/
	//##ModelId=3C4C43900026
	virtual void ConnectCnf();

/**
	@see			MWspCOSessionCallback
*/
	//##ModelId=3C4C438F03B4
	virtual void ResumeCnf();

/**
	@see			MWspCOSessionCallback
*/
	//##ModelId=3C4C438F0223
	virtual void DisconnectInd(
							  TWspReason				aReason, 
							  TBool						aRedirectSecurity, 
							  TWspRedirectedAddress&	aRedirectAddress, 
							  const TDesC8&				aErrorHeader, 
							  const TDesC8&				aErrorBody
							  );

/**
	@see			MWspCOSessionCallback
*/
	//##ModelId=3C4C438F0197
	virtual void SuspendInd(TWspReason aReason);

/**
	@see			MWspCOSessionCallback
*/
	//##ModelId=3C4C438F0115
	virtual void ExceptionInd(const TDesC8& aExceptionData);

private:	// Methods from MWspCapabilityProvider

/**
	@see			MWspCapabilityProvider		
*/
	//##ModelId=3C4C438F00CF
	virtual const MWspCapabilityViewer& ClientCapabilities() const;

/**
	@see			MWspCapabilityProvider		
*/
	//##ModelId=3C4C438F0092
	virtual MWspCapabilitySetter& ServerCapabilities() const;

private:	// Methods from MWspSessionHeadersProvider

/**
	@see			MWspSessionHeadersProvider		
*/
	//##ModelId=3C4C438F004C
	virtual const TDesC8& ClientHeaders() const;

/** 
	@see			MWspSessionHeadersProvider		
*/
	//##ModelId=3C4C438E03C6
	virtual void SetServerHeadersL(const TDesC8& aBuffer);

private:	// Methods from MConnectionTimerCallback

/**
	@see			MConnectionTimerCallback
*/
	//##ModelId=3C4C438E038A
	virtual void HandleConnectionTimedOut();

private:	// Methods from MWspPrimitiveSenderCallback

/**
	@see			MWspPrimitiveSenderCallback
*/
	//##ModelId=3C4C438E0326
	virtual void SendPrimitiveL(TWspPrimitive aPrimitive);

/**
	@see			MWspPrimitiveSenderCallback
*/
	//##ModelId=3C4C438E02C2
	virtual TInt WspPrimitiveSenderCallbackError(TInt aError);

private:	// Methods from MWspCOMethodObserver

/**
	@see			MWspCOMethodObserver
*/
	//##ModelId=3C4C438E025E
	virtual void HandleMethodAbort(CWspCOTransaction& aTransaction);

/**
	@see			MWspCOMethodObserver
*/
	virtual void NotifyPendingCompletingMethod();

/**
	@see			MWspCOMethodObserver
*/
	virtual void NotifyMethodComplete();

private:	// Methods from MRxDataObserver, inherited via MWspCOMethodObserver

/**
	@see			MRxDataObserver
*/
	//##ModelId=3C4C438E01DC
	virtual void SetStatusL(CRxData& aRecv, TInt aStatus);

private:	// Methods from MProtHandlerInterface

	/** Get the Server Certificate for the specified transaction.  This function returns
	NULL because for WSP the certificate applies to the whole session.
	@return	NULL
	*/
	const CCertificate* SessionServerCert();


	/** Get the Server Certificate for the current session.
	@param	aTransaction The transaction for which the certificate is requested
	@return	A pointer to CCertificate, or NULL if not found.
	*/
	const CCertificate* TransactionServerCert(RHTTPTransaction aTransaction);


private:	// Methods

/**
	@since			7.0
	@param			aSession	The HTTP session.
*/
	//##ModelId=3C4C438E013B
	CWspCOProtocolHandler(RHTTPSession aSession);

/**
	@since			7.0
	@todo			leaves
	@pre			First phase construction has been done.
	@post			The object is fully initialised.
*/
	//##ModelId=3C4C438E0127
	void ConstructL();

/**Initiates a session connection.
	@since			7.0
	@warning		This function will panic with KWspPanicSessionNotInValidState
					if the WSP session cannot be connected.
	@todo			leaves
	@pre			The WSP session is in the Null state.
	@post			The WSP session is in the Connecting state. The connection 
					timer has been started.
*/
	//##ModelId=3C4C438E0109
	void SessionConnectL();

/**Initiates a session resume.
	@since			7.0
	@warning		This function will panic with KWspPanicSessionNotInValidState
					if the WSP session cannot be resumed.
	@todo			leaves
	@pre			The WSP session is in the Connected, Suspending or 
					Suspended state. The Suspend Resume facility is supported
					in this WSP session.
	@post			The WSP session is in the Resuming state. The connection 
					timer has been started.
*/
	//##ModelId=3C4C438E00EB
	void SessionResumeL();
	
/**The session should be disconnected.
	@since			7.0
	@warning		This function will panic with KWspPanicSessionNotInValidState
					if the WSP session cannot be disconnected.
	@todo			leaves
	@pre			The WSP session is in the Connecting, Connected, Suspending,
					Suspended, or Resuming state.
	@post			The WSP session is in the Closing state. The connection 
					timer has been cancelled.
*/
	//##ModelId=3C4C438E00A5
	void SessionDisconnect(TWspReason aReason);

/**The session should be suspended.
	@since			7.0
	@warning		This function will panic with KWspPanicSessionNotInValidState
					if the WSP session cannot be suspended.
	@todo			leaves
	@pre			The WSP session is in the Connected or Resuming state. The 
					Suspend Resume facility is supported in this WSP session.
	@post			The WSP session is in the Closing state. The connection 
					timer has been cancelled.
*/
	//##ModelId=3C4C438E0087
	void SessionSuspend();

/**Processes the session connected event.
	@since			7.0
	@pre			
	@post			
*/
	//##ModelId=3C4C438E0073
	void DoSessionConnectedL();

/**Processes the session connected event, this is a non-leaving
						method of DoSessionConnectedL()
	@since			7.0
	@pre			
	@post			
*/
	void DoSessionConnected();

/**	@since			7.0
	@todo			docing
	@param			aEvent	The session event to send.
	@pre
	@post
*/
	//##ModelId=3C4C438E0037
	void SendSessionEvent(THTTPSessionEvent aEvent);

/**@since			7.0
	@todo			docing
	@leave			Leaves with KErrNotFound if the WSP transport handler cannot
					be found. Leaves with KErrNotSupported if the loaded WSP
					transport handler does not support Connection-Orientated 
					Session Management facility and Method Invocation facility.
	@post			The
*/
	//##ModelId=3C4C438D03BB
	void CreateWspTransportHandlerL();

/**Updates the client session headers that is supplied to 
						the transport handler
	@since			7.0
	@leave			Leaves with KErrNoMemory if the header buffer cannot be 
					created.
	@post			The client session headers buffer is upto date.
*/
	//##ModelId=3C4C438D039D
	void UpdateClientSessionHeadersL();

/**Checks the properties in the HTTP session for any client
						proposed capablities and a specified connection time-out
						value.
	@since			7.0
	@post			The client proposed capabilities and the connection time-out
					value have been updated.
*/
	//##ModelId=3C4C438D0389
	void CheckClientCapabilities();

/**Checks the client-requested capabilities against the 
						corresponding negotiated capabilities to see if they 
						have been reduced or denied.
	@since			7.0
	@todo			leaves
	@return			A boolean value of ETrue if one or more of the client-
					requested capabilities have been reduced or denied.
	@pre			The negotiated capabilities have been passed up from the
					transport handler. The S-Connect.cnf primitive has been
					received.
	@post			If any of the client-requested capabilities have been 
					reduced or rejected, the session property representing that
					capabililty is set the appropriate negotiated value.
*/
	//##ModelId=3C4C438D0375
	TBool UpdateNegotiatedCapabilitiesL();

/**
	Handles the receipt of a EConnect session event. Will inform the client if
	the WSP session is not in the appropriate state to receive this event.
	@since			7.0
	@leave		This function will leave if the session connect/resume fails.
*/
	//##ModelId=3C4C438D0361
	void HandleConnectRequestL();

/**
	Handles the receipt of a EDisconnect session event. Will inform the client if
	the WSP session is not in the appropriate state to receive this event.
	@since			7.0
*/
	//##ModelId=3C4C438D034D
	void HandleDisconnectRequest();

/**
	Checks to see if a method is waiting. If so it sets the base class active
	so that the next pending transaction gets serviced.
	@since			7.0
*/
	//##ModelId=3C4C438D0339
	void CheckWaitingMethod();

/**
	Indicates if the Suspend Resume facility is supported by
						the WSP session.
	@since			7.0
	@return			A boolean value of ETrue if the WSP session can support the 
					Suspend Resume facility.
*/
	//##ModelId=3C4C438D0325
	TBool SupportSuspendResume() const;

/**Indicates whether the WSP session can be resumed. The 
						Suspend Resume facility must be supported and the WSP
						session must in the Connected, Suspending or Suspended
						state.
	@since			7.0
	@return			A boolean value of ETrue if the WSP session can be resumed.
*/
	//##ModelId=3C4C438D0311
	TBool CanResume() const;

/**Indicates whether the WSP session can be suspended. The
						Suspend Resume facility must be supported and the WSP
						session must in the Connected or Resuming state.
	@since			7.0
	@return			A boolean value of ETrue if the WSP session can be suspended.
*/
	//##ModelId=3C4C438D02F2
	TBool CanSuspend() const;

/**Resets the state of the protocol handler to that upon
						initial construction, without changing the transport
						handler.  This is NOT a published mechanism!
	@since			7.0
*/
	//##ModelId=3C4C438D02E8
	void ResetAll();

private:	// Attributes

	/** A pointer to the Wsp Transport Handler.
	*/
	//##ModelId=3C4C438D02CA
	CWspTransportHandler*		iTransportHandler;

	/** A pointer to the CO Session Invoker object. Owned by the transport 
		handler.
	*/
	//##ModelId=3C4C438D02B6
	MWspCOSessionInvoker*		iSessionInvoker;

	/** An object that holds the negotiated capabilties for the current Wsp 
		session.
	*/
	//##ModelId=3C4C438D02A2
	CWspCapabilityInfo*			iNegotiatedCapInfo;

	/** An object that holds the proposed client capabilities.
	*/
	//##ModelId=3C4C438D0284
	CWspCapabilityInfo*			iClientCapInfo;

	/** A pointer to the proxy info provider.
	*/
	//##ModelId=3C4C438D0272
	CWspProxyInfoProvider*		iProxyInfoProvider;

	/** The state of the WSP session.
	*/
	//##ModelId=3C4C438D0266
	TWspSessionState			iSessionState;

	/** The connection timed-out value in microseconds. The protocol handler 
		will abort a session connect after this time if a connection has not 
		been established.
	*/
	//##ModelId=3C4C438D0252
	TInt						iTimedOutValue;

	/** A pointer to the connection timed-out timer.
	*/
	//##ModelId=3C4C438D023E
	CConnectionTimer*			iConnectionTimer;

	/** An active object that is used to invoke the sending of certain WSP
		primitives.
	*/
	//##ModelId=3C4C438D022A
	CWspPrimitiveSender*		iPrimitiveSender;

	/** A buffer holding the encoded client session headers.
	*/
	//##ModelId=3C4C438D0218
	HBufC8*						iClientSessionHeaders;

	/** A flag indicating that a session connect or resume has timed-out.
	*/
	//##ModelId=3C4C438D020C
	TBool						iConnectTimedOut;

	/** A flag indicating that there is a method transaction waiting to be 
		serviced.
	*/
	//##ModelId=3C4C438D01F8
	TBool						iWaitingMethod;

	/** A pointer to the WSP header utils
	*/
	//##ModelId=3C4C438D01E4
	CWspHeaderUtils*			iHdrUtils;

	/** The number of methods in Completing state waiting to send the final .res
		primitive.
	*/
	TInt						iPendingCompletingMethods;

	/** A flag indicating that the client has requested a disconnect.
	*/
	TBool						iDisconnectRequested;

	};

#endif	// __CWSPCOPROTOCOLHANDLER_H__
