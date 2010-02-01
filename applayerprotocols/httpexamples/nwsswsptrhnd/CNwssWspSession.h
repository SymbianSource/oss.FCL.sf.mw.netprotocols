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
 @file CNwssWspSession.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef	__CNWSSWSPSESSION_H__
#define __CNWSSWSPSESSION_H__

// System includes
#include <e32base.h>
#include <wapcli.h>
#include <ssl.h>
#include <http/framework/logging.h>

// Local includes
#include "mnwsswapserver.h"

// Forward declarations
class MNwssWapServer;
class MConnectionInfoProvider;
class MSecurityPolicy;


/**
	@since			7.0
	This abstract class encapsulates a WSP session as implemented by the NWSS WAP
	Stack.  It must be specialised to provide Connection-Oriented or Connectionless
	WSP services.  It is an Active Object class, and uses an active state machine
	to drive a WTLS handshake through four phases.  For non-secure WSP sessions,
	the handshake is skipped, and two phases are used to complete a WSP connection.
 */

//##ModelId=3C4C46CF020B
class CNwssWspSession : public CActive
	{
// Unit testing only - give friendship to a public state accessor 
#ifdef __UNIT_TESTING__
	friend class TNwssWspCOSession_StateAccessor;
#endif

public: // type defections / enumerations

/** An enumeration of the states the CNwssWspSession class can be in.  When in the
	EReady state, further processing is done in the derived session class.
 */
	enum TState
		{
		EDisconnected, //< the initial state following construction in which there is no WSP session open or connected 
		ESessionOpen, //< the state in which a WSP session has been opened (either CO or CL)
		EDoingWtlsPhase1, //< the state during a phase 1 WTLS handshake
		ECheckingServerCert, //< the state during presentation of an untrusted WTLS certificate
		EDoingWtlsPhase2, //< the state during a phase 2 WTLS handshake
		ECheckingNegotiated, //< the state in which the negotiated security settings are checked with the security policy following a successful handshake	
		EReady //< the state in which the WSP session is processing events (either CO or CL)
		};

public: // methods

/** Destructor.
	@since			7.0
 */
	//##ModelId=3C4C46CF0311
	virtual ~CNwssWspSession();

	/** Get the Server Certificate for this session.
	@param	aServerCert A TCertInfo which will be filled with the certificate information
	@return	An error code.  KErrNone if aServerCert has been completed, otherwise one of 
			the system wide error codes
	*/
	//##ModelId=3C9B094B0393
	TInt ServerCert(TCertInfo& aCertInfo);

protected: // methods

/** Normal constructor.  Creates an instance of the session, keeping
					references to the supplied providers from the client.
	@since			7.0
	@param			aStringPool			(in) The client's string pool.
	@param			aWapStackProvider	(in) An owner of a WAP stack server session.
	@param			aConnInfoProvider	(in) A collection of client interfaces that
											 provide connection-related information
	@param			aSecurityPolicy		(in) A security policy provider.
 */
	//##ModelId=3C4C46CF02F3
	CNwssWspSession(RStringPool& aStringPool,
					MNwssWapServer& aWapStackProvider,
				    MConnectionInfoProvider& aConnInfoProvider,
					MSecurityPolicy& aSecurityPolicy);

/** Second phase construction.  Does initial allocations required
					for this class.
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory
 */
	//##ModelId=3C4C46CF02F2
	void BaseConstructL();

/** Used by sub-classes to initiate a new connection to a WAP proxy,
					the various steps of which are performed in an asynchronous
					state machine in this class.
	@since			7.0
 */
	//##ModelId=3C4C46CF02EA
	void InitiateProxyConnection();

/** Accessor to the state, for use by subclasses.
	@since			7.0
	@return			The current state of this object.
 */
	//##ModelId=3C591A970069
	TState State() const;

/** Callback for use by sub-classes, to indicate when the session
					with a WAP proxy has been disconnected.  Causes the WSP session
					to be closed.
	@since			7.0
	@return			A system error code.
 */
	//##ModelId=3C4C46CF02E9
	TInt SessionDisconnectedCallback();

/** Map NWSS stack error codes to local panic codes.  Panics the
					client using the mapped panic code.
	@since			7.0
 */
	//##ModelId=3C4C46CF02DF
	void MapToPanic(TInt aErrorCode) const;

private: // methods inherited from CActive

/** Called when the active object fires, ie. its asynchronous request
					has completed
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory and internal error codes
					generated for WTLS handshake failures, see <wsperror.h>
 */
	//##ModelId=3C4C46CF02DE
	virtual void RunL();

/** Called if RunL leaves, to allow the AO to handle the error before
					the scheduler does
	@since			7.0
	@param			aError	(in) The error code to be handled, which RunL() left with
	@return			A final error code, should aError not have been handled, which the
					active scheduler handles.
 */
	//##ModelId=3C4C46CF02D5
	virtual TInt RunError(TInt aError);

/** Cancellation protocol for the active object.  It cancels any
					outstanding requests it	has made, according to its state.
	@since			7.0
 */
	//##ModelId=3C4C46CF02D4
	virtual void DoCancel();

private: // methods to be implemented in derived classes

/**Hook that allows the class to cause a particular type of
					WSP session to the specified proxy to be opened.  Must be
					implemented in sub-classes which deal with either CO or CL
					WSP.
	@since			7.0
	@param			aRemoteHost	(in) The bearer-dependent address of the proxy.
	@param			aRemotePort	(in) The proxy port to which a connection is made.
	@param			aLocalPort	(in) (optional) The local port to which proxy replies
									 are sent. A value of zero means 'don't care'.
	@param			aBearer		(in) The bearer on which the connection is made.
	@param			aSecureConn	(in) Flag indicating whether WTLS is to be used.
	@return			A system error code originating from the NWSS WAP Stack.
 */
	//##ModelId=3C4C46CF02AC
	virtual TInt OpenWspSession(const TDesC8& aRemoteHost,
								RWAPConn::TPort aRemotePort,
								RWAPConn::TPort aLocalPort,
								TBearer aBearer,
								TBool aSecureConn) = 0;

/**Hook that allows the class to cause an opened WSP session to
					be closed.  Must be implemented in sub-classes which deal with
					either CO or CL WSP.
	@since			7.0
	@return			A system error code originating from the NWSS WAP Stack.
 */
	//##ModelId=3C591A97000E
	virtual TInt CloseWspSession() = 0;

/**Hook that allows the class to perform any required actions to
					complete a connection to the WAP Proxy after the WSP session
					has been opened, and optionally a WTLS handshake performed.
					Must be implemented in sub-classes which deal with either CO or
					CL WSP.
	@since			7.0
	@leave			A system error code originating from the NWSS WAP Stack.
 */
	//##ModelId=3C4C46CF02A3
	virtual void CompleteProxyConnectionL() = 0;

/** Obtains the WTLS handle from the current opened WSP session.
					Must be implemented in sub-classes which deal with either CO or
					CL WSP.
	@since			7.0
	@return			A handle for the WTLS layer of the WAP protocol stack.
 */
	//##ModelId=3C4C46CF02A2
	virtual RWTLS WtlsHnd() = 0;

/** Allows sub-classes to perform specific actions in response to
					this class' RunError().  This is called in the EReady state,
					where processing in the RunL is performed in the sub-class. In
					practice, this will handle situations where
					CompleteProxyConnectionL() has left.
	@since			7.0
	@param			aError	(in) The error code to be handled, which RunL() left with
 */
	//##ModelId=3C4C46CF029A
	virtual void DoRunError(TInt aError) = 0;

/** Allows sub-classes to perform specific actions in response to
					this class' DoCancel(). This is called in the EReady state.
	@since			7.0
	@return			Flag indicating whether, following cancellation, this class
					must return to the EDisconnected state.
 */
	//##ModelId=3C591A9603A6
	virtual TBool SubDoCancel() = 0;

private: // methods called from the RunL state machine

/** Open the WSP session to the proxy identified by the
					client's Proxy Info Provider.
	@since			7.0
	@leave			System-wide error codes, e.g. KErrNoMemory
	@return			Flag indicating whether WTLS is to be used or not.
 */
	//##ModelId=3C4C46CF0299
	TBool OpenWapProxyL();


/** Pre-handshake phase of WTLS negotiations with the WAP Stack.
					The WTLS settings are configured according to the security
					policy plug-in.
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory
 */
	//##ModelId=3C4C46CF0298
	void DoWTLSConnectionPreHandshakeL();

/**First phase of WTLS negotiations with the WAP Stack - the first
					of two phases for authentication, but the one and only phase
					for anonymous.
	@since			7.0
 */
	//##ModelId=3C4C46CF028E
	void DoWTLSConnectionPhaseOne(TBool aTwoPhaseHandshake);

/**Cancellation of phase 1 of the secure handshake, if eg. ciphers
					are not available or a certificate was not obtained, or the
					entire connection has been cancelled.
	@since			7.0
 */
	//##ModelId=3C4C46CF0287
	void CancelWTLSConnectionPhaseOne();

/**Mid phase of WTLS negotiations - used only for authenticating
					connections - obtains the WAP Gateway certificate information
					and asks the security policy to determine if it the server can
					be trusted.
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory and internal error codes
					generated for WTLS handshake failures, see <wsperror.h>
 */
	//##ModelId=3C4C46CF0286
	void ValidateUntrustedServerCertL();

/**Gets the server certificate from the wap stack and converts it
					to a TCertInfo which is stored in iServerCert.
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory and internal error codes
					generated for WTLS handshake failures, see <wsperror.h>
 */
	//##ModelId=3C9B094B0310
	void GetServerCertL();

/**Cancel the validation of an untrusted server cert.
	@since			7.0
 */
	//##ModelId=3C591A96036A
	void CancelValidateUntrustedServerCert();

/**Second phase of WTLS negotiations with the WAP Stack (used for
					authenticating connections only).
	@since			7.0
 */
	//##ModelId=3C4C46CF0285
	void DoWTLSConnectionPhaseTwo();

/**Cancellation of phase 2 of the secure handshake, if eg. ciphers
					are not available or a certificate was not obtained, or the
					entire connection has been cancelled.
	@since			7.0
 */
	//##ModelId=3C4C46CF0284
	void CancelWTLSConnectionPhaseTwo();

/**Post-security handshake finalisation of WTLS via the WAP Stack.
	@since			7.0
	@leave			System error codes, e.g. KErrNoMemory and internal error codes
					generated for WTLS handshake failures, see <wsperror.h>
 */
	//##ModelId=3C4C46CF027C
	void DoWTLSConnectionPostHandshakeL();

/**Short-cut for call to the security policy plug-in, which checks
					a named property to see if anonymous authentication is allowed
					or not.
	@return			Flag indicating whether, the policy allows anonymous handshakes.
	@since			7.0
 */
	//##ModelId=3C4C46CF027B
	TBool AnonymousAuthenticationAllowedL();

/**Active object self completion.
	@since			7.0
 */
	//##ModelId=3C4C46CF027A
	void CompleteSelf();

#ifdef _DEBUG
private: // Debug-only methods that dump negotiated WTLS parameters

/**Writes details to log of a ciphersuite, either requested or negotiated.
	@since			7.0
	@param			aCipherSuite	(in) A NWSS WAP stack cipher suite representation
	@param			aRequested		(in) Flag that determines if 'requested' or 'negotiated' is written
										 on the log line.
 */
	//##ModelId=3C9B094B01BC
	void DumpCipherSuite(RWTLS::TCipherSuite aCipherSuite, TBool aRequested);

/**Writes details to log of a key exchange suite, either requested or negotiated.
	@since			7.0
	@param			aKeyExchSuite	(in) A NWSS WAP stack key exchange suite representation
	@param			aStackIdType	(in) A NWSS WAP stack ID type representation
	@param			aRequested		(in) Flag that determines if 'requested' or 'negotiated' is written
										 on the log line.
 */
	//##ModelId=3C9B094A03E1
	void DumpKeyExchangeSuite(RWTLS::TKeyExchangeSuite aKeyExchSuite, RWTLS::TIdType aStackIdType, TBool aRequested);

/**Writes details to log of a sequence number mode, either requested or negotiated.
	@since			7.0
	@param			aSeqNumMode (in) A NWSS WAP stack sequence number mode representation
	@param			aRequested	(in) Flag that determines if 'requested' or 'negotiated' is written
									 on the log line.
 */
	//##ModelId=3C9B094A0297
	void DumpSequenceNumberMode(RWTLS::TSequenceNumberMode aSeqNumMode, TBool aRequested);

	//##ModelId=3C9B094A0156
	void DumpKeyRefreshRate(TUint8 aKeyRefreshRate, TBool aRequested);
	//##ModelId=3C9B094A0098
	void DumpSharedSecret(const TDesC8& aSharedSecret);
	//##ModelId=3C9B09490340
	void DumpRecordLengthUsage(TBool aRecordLengthUsage, TBool aRequested);

#endif

protected: // attributes

	/** Logger handle
	*/
	__DECLARE_LOG

	/** The client's string pool - not opened or closed here
	*/
	//##ModelId=3C4C46CF0272
	RStringPool& iStringPool;

	/** The WAP Stack provider
	*/
	//##ModelId=3C4C46CF0265
	MNwssWapServer& iWapStackProvider;

	/** The client connection information provider
	*/
	//##ModelId=3C4C46CF0253
	MConnectionInfoProvider& iConnInfoProvider;

private: // attributes

	/** State of the WSP session
	*/
	//##ModelId=3C4D961900D5
	TState iState;

	/** Security policy provider
	*/
	//##ModelId=3C4C46CF023D
	MSecurityPolicy& iSecurityPolicy;

	/** Flag that records if the secure handshake resulted in anonymous key exchange suites
		being agreed with the WAP proxy.
	*/
	//##ModelId=3C9B09490321
	TBool iHandshakeWasAnonymous;

	/** Storage for the WAP proxy's server certificate, in the form used by the
		security policy plugin
	*/
	//##ModelId=3C4D95A40325
	TCertInfo iServerCert;

	/** Flag indicating that the variable iServerCert contains a valid server certificate */
	//##ModelId=3C9B09490303
	TBool iServerCertValid;
	};


#endif // __CNWSSWSPSESSION_H__
