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
#include <wsp/mwspproxyinfoprovider.h>
#include <http/framework/csecuritypolicy.h>
#include <http/framework/securitypolicypluginstrings.h>
#include <wsperror.h>
#include <http/framework/logging.h>

// Local includes
#include "tnwsswsptrhndpanic.h"
#include "mconnectioninfoprovider.h"
#include "testoom.h"

// Class signature
#include "cnwsswspsession.h"

const TInt KWtlsMaxFieldSize	= 256;

CNwssWspSession::~CNwssWspSession()
	{
	}

CNwssWspSession::CNwssWspSession(RStringPool& aStringPool,
								 MNwssWapServer& aWapStackProvider,
								 MConnectionInfoProvider& aConnInfoProvider,
								 MSecurityPolicy& aSecurityPolicy): 
	CActive(CActive::EPriorityStandard), iStringPool(aStringPool),
	iWapStackProvider(aWapStackProvider), iConnInfoProvider(aConnInfoProvider),
	iState(EDisconnected), iSecurityPolicy(aSecurityPolicy)
	{
	CActiveScheduler::Add(this);
	}

void CNwssWspSession::BaseConstructL()
	{
	// Add the security policy plug-in string table to the string pool
	__DEBUGTESTLEAVE
	iStringPool.OpenL(SecurityPolicyProperties::Table);
	}

void CNwssWspSession::InitiateProxyConnection()
	{
	if (iState == EDisconnected)
		CompleteSelf();
	}

CNwssWspSession::TState CNwssWspSession::State() const
	{
	return iState;
	}


TInt CNwssWspSession::SessionDisconnectedCallback()
	{
	TInt err = CloseWspSession();
	if (err == KErrNone)
		iState = EDisconnected;
	return err;
	}

void CNwssWspSession::RunL()
	{
	switch (iState)
		{
	case EDisconnected:
		{
		__LOG("--Running in state EDisconnected")
		// Make initial connection to the WAP Stack
		__DEBUGTESTLEAVE
		TBool useWtls = OpenWapProxyL();
		if (useWtls)
			iState = ESessionOpen;
		else
			iState = EReady;
		CompleteSelf();
		} break;
	case ESessionOpen:
		{
		__LOG("--Running in state ESessionOpen")
		// Do pre-handshake setup, followed by phase 1 of the connection
		__DEBUGTESTLEAVE
		DoWTLSConnectionPreHandshakeL();
		DoWTLSConnectionPhaseOne(ETrue);
		iHandshakeWasAnonymous = EFalse;
		iState = EDoingWtlsPhase1;
		} break;
	case EDoingWtlsPhase1:
		{
		__LOG("--Running in state EDoingWtlsPhase1")
		// Did phase 1 succeed?
		TInt completionCode = iStatus.Int();
		switch (completionCode)
			{
		case RWTLS::EHandshakeOk:
			{
			__LOG("--Phase 1 result is EHandshakeOK")
			__LOG("--indicates that an anonymous handshake was performed.");
			__LOG("--proceeding to check negotiated configuration")
			// The handshake is fully complete after one phase.  Since we always initiate a 2-phase
			// handshake, this indicates that anonymous key exchange suites were agreed.
			// Note - it would seem sensible to do a phase 2 to complete the handshake, since we initiated
			// it as 2 phase.  However doing so never works (ConnectPhase2() returns EErrGeneral). So we'll
			// have to consider the handshake complete now.
			
			// Store 'was anonymous' flag for the next AO iteration, where we will check against the policy
			// to ensure anonymous acceptable. 
			iHandshakeWasAnonymous = ETrue;
			iState = ECheckingNegotiated;
			CompleteSelf();
			} break;
		case RWTLS::EServerCertificateValid:
			{
			__LOG("--Phase 1 result is EServerCertificateValid")
			__LOG("--proceeding to do handshake phase 2")
			// Note - this error code indicates that the server certificate is valid, and that it
			// was verified by the WAP Stack against a root cert held on the device.  First get 
			// the server certificate in case the client asks to see it and then move to phase 2.
			__DEBUGTESTLEAVE
			GetServerCertL();
			DoWTLSConnectionPhaseTwo();
			iState = EDoingWtlsPhase2;
			} break;
        case RWTLS::EServerCertificateNotValid:
			{
			__LOG("--Phase 1 result is EServerCertificateNotValid")
			// Note - this error code indicates that although all fields of the server certificate
			// were valid, the WAP Stack couldn't verify the server identity against a root cert.
			// For a 2-phase (authenticating) handshake, this means we present the cert to the user
			// for verification
			__LOG("--proceeding to check untrusted server certificate")
			// Check the server cert before phase 2 of the handshake
			iState = ECheckingServerCert;
			__DEBUGTESTLEAVE
			GetServerCertL();
			ValidateUntrustedServerCertL();
			} break;
		case RWTLS::EHandshakeError:
		case RWTLS::EHandshakeUserAbort:
		case RWTLS::ECertificateRequested:
			{
#ifdef _DEBUG
			if (completionCode == RWTLS::EHandshakeError)
				{
				__LOG("--Phase 1 result is EHandshakeError")
				}
			else if (completionCode == RWTLS::EHandshakeUserAbort)
				{
				__LOG("--Phase 1 result is EHandshakeUserAbort")
				}
			else
				{
				// Presumably this refers to a client cert? Not supported by this client if that's the case.
				__LOG("--Phase 1 result is ECertificateRequested")
				}
#endif
			// Cancel phase 1 - the connect could not be established
			CancelWTLSConnectionPhaseOne();
			User::Leave(KWtlsErrPhase1HandshakeFailed);
			} break;
		default:
			{
			__LOG1("--Phase 1 result is unknown (%d)", completionCode)
			// Other error codes from completion of phase 1 connect
			if (completionCode >= 0)
				completionCode = KWtlsErrPhase1HandshakeFailed;
			User::Leave(completionCode);
			} break;
			}
		} break;
	case ECheckingServerCert:
		{
		__LOG("--Running in state ECheckingServerCert")
		// Was the server cert accepted? If not, leave
		TInt completionCode = iStatus.Int();
		__DEBUGTESTLEAVE
		User::LeaveIfError(completionCode);

		// Enter second phase of handshake
		DoWTLSConnectionPhaseTwo();
		iState = EDoingWtlsPhase2;
		} break;
	case EDoingWtlsPhase2:
		{
		__LOG("--Running in state EDoingWtlsPhase2")
		// Did phase 2 succeed? If not, leave
		TInt completionCode = iStatus.Int();
		__DEBUGTESTLEAVE
		if ( (completionCode != RWTLS::EHandshakeOk) &&
			 (iHandshakeWasAnonymous && (completionCode != RWTLS::EErrGeneral) ) )
			User::Leave( (completionCode > 0)?KWtlsErrPhase2HandshakeFailed:completionCode );

		// Move straight on to check the negotiated values
		iState = ECheckingNegotiated;
		CompleteSelf();
		} break;
	case ECheckingNegotiated:
		{
		__LOG("--Running in state ECheckingNegotiated")
		// Finalise the handshake by checking the negotiated settings
		__DEBUGTESTLEAVE
		DoWTLSConnectionPostHandshakeL();

		// Authenticating handshake has completed successfully
		iState = EReady;
		CompleteSelf();
		} break;
	case EReady:
		{
		__LOG("--Running in state EReady")
		// Can now complete the WSP session connection to the proxy
		__DEBUGTESTLEAVE
		CompleteProxyConnectionL();
		} break;
	default:
		{
		TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInvalidState);
		} break;
		}
	}

TInt CNwssWspSession::RunError(TInt aError)
	{
	__LOG1("--Handling WAP stack error code %d", aError)
	switch (iState)
		{
	case EDisconnected:
		{
		// Failed to open the WSP session
		DoRunError(aError);
		} break;
	case ESessionOpen:
		{
		DoRunError(KWtlsErrConfigurationFailed);
		} break;
	case EDoingWtlsPhase1:
		{
		// If error is KErrNotSupported then this is usually an error of insufficient crypto strength
		__ASSERT_DEBUG(aError != KErrNotSupported,
						TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInsufficientCryptoStrength));

		// Only expect an 'invalid server certificate' or 'phase 1 handshake failed' error in this state
		__ASSERT_DEBUG(aError == KWtlsErrInvalidServerCert || aError == KWtlsErrPhase1HandshakeFailed,
						TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInvalidState));
		DoRunError(aError);
		} break;
	case ECheckingServerCert:
		{
		// Only expect an 'invalid/untrusted server certificate' error in this state
		__ASSERT_DEBUG(aError == KWtlsErrInvalidServerCert || aError == KWtlsErrUntrustedServerCert,
						TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInvalidState));
		DoRunError(aError);
		} break;
	case EDoingWtlsPhase2:
		{
		DoRunError(KWtlsErrPhase2HandshakeFailed);
		} break;
	case ECheckingNegotiated:
		{
		DoRunError(KWtlsErrNegotiatedConfigRejected);
		} break;
	case EReady:
		{
		DoRunError(aError);
		} break;
	default:
		{
		// State machine screw-up!
		__ASSERT_DEBUG(ETrue, TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EInvalidState));
		} break;
		}

	// State after handling an error returns to 'disconnected'
	iState = EDisconnected;
	return KErrNone;
	}

void CNwssWspSession::DoCancel()
	{
	// Some cancellations will return the state machine to EDisconnected
	switch (iState)
		{
	case EDoingWtlsPhase1:
		{
		// Outstanding request is the phase 1 connect
		CancelWTLSConnectionPhaseOne();
		iState = EDisconnected;
		} break;
	case ECheckingServerCert:
		{
		// Cancel the validation
		CancelValidateUntrustedServerCert();
		iState = EDisconnected;
		} break;
	case EDoingWtlsPhase2:
		{
		// Outstanding request is the phase 2 connect
		CancelWTLSConnectionPhaseTwo();
		iState = EDisconnected;
		} break;
	case EReady:
		{
		// Defer to sub-class.  The SubDoCancel() returns a flag if it wishes to move the state machine
		// back to 'disconnected'.
		if ( SubDoCancel() )
			iState = EDisconnected;
		} break;
	default:
		{
		// Other states do not require asynch request cancellation
		} break;
		}
	}

TBool CNwssWspSession::OpenWapProxyL()
	{
	// Get details of the desired WAP proxy from the client's connection info provider
	MWspProxyInfoProvider& prov = iConnInfoProvider.ProxyInfoProvider();
	const TDesC8& addr = prov.ProxyAddress();
	__LOG1("--Proxy Address is '%S'", &addr)
	TUint16 remPort = prov.RemotePort();
	__LOG1("--Remote Port is %d", remPort)
	TUint16 locPort = prov.LocalPort();
	__LOG1("--Local Port is %d", locPort)
	TWspBearer bearer = prov.Bearer();
	const TDesC8& svcCentNum = prov.ServiceCentreNumber();
	TWspSession sessSvc = prov.WspSessionService();
	__LOG1("--WSP Session Type is %d (0 = CO, 1 = CL)", sessSvc)
	TBool isSecure = prov.SecureConnection();
	__LOG1("--Security is %d (0 = off, 1 = on)", isSecure)

	// It is an error for ConnectReq() to be used when the proxy specifies a CL session!
	__ASSERT_ALWAYS(sessSvc == EWspConnectionOriented,
					TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EWrongSessionType));

	// Need to supply different parameters, according to whether the bearer is IP or
	// SMS
	TInt error = KErrNone;
	switch (bearer)
		{
		// scope enumerations to the global namespace since that's where they were
		// defined in <wapcli.h>  :(
	case ::EIP:
		{
		__LOG("--Bearer is IP")

		// Open the WSP CO connection with the WAP stack, using IP bearer
		error = OpenWspSession(addr, remPort, locPort, (TBearer)bearer, isSecure);	  
		} break;
	case ::EWAPSMS7:
	case ::EWAPSMS:
		{
		__LOG("--Bearer is SMS")
		__LOG1("--Service Centre Number is '%S'", &svcCentNum)

		// Open the WSP CO connection with the WAP stack, using SMS bearer
		error = OpenWspSession(svcCentNum, remPort, locPort, (TBearer)bearer, isSecure);	  
		} break;
	case ::ESMS7:
	case ::ESMS:
	case ::EAll:
	default:
		__ASSERT_DEBUG(ETrue, 
					   TNwssWspTrHndPanic::Panic(TNwssWspTrHndPanic::EWrongBearerType));
		}

	__LOG1("--RWSPCOConn::Open() returned with %d", error)
	__DEBUGTESTLEAVE
	User::LeaveIfError(error);

	// Return the secure flag, so the state machine can go on to make the WTLS
	// handshake if appropriate, for secure proxies
	return isSecure;
	}

void CNwssWspSession::MapToPanic(TInt aErrorCode) const
	{
	// Error handling is severe, since all WSP state management is expected to be done
	// in the client.
	TNwssWspTrHndPanic::TNwssWspTrHndPanicCode panic;
	switch (aErrorCode)
		{
		case RWAPConn::EBearerError:
			panic = TNwssWspTrHndPanic::EBearerError;
			break;
		case RWAPConn::EPortAlreadyBound:
			panic = TNwssWspTrHndPanic::EPortAlreadyBound;
			break;
		case RWAPConn::ECannotOpenPort:
			panic = TNwssWspTrHndPanic::ECannotOpenPort;
			break;
		case RWAPConn::ETooManyConnections:
			panic = TNwssWspTrHndPanic::ETooManyConnections;
			break;
		case RWAPConn::EBadConnection:
			panic = TNwssWspTrHndPanic::EBadConnection;
			break;
		case RWAPConn::EBufferTooSmall:
			panic = TNwssWspTrHndPanic::EBufferTooSmall;
			break;
		case RWSPCOConn::EBufferTooSmall:
			panic = TNwssWspTrHndPanic::EBufferTooSmall;
			break;
		case RWSPCOConn::EInvalidSession:
			panic = TNwssWspTrHndPanic::EInvalidSession;
			break;
		case RWSPCOConn::EInvalidTransaction:
			panic = TNwssWspTrHndPanic::EInvalidTransaction;
			break;
		case RWSPCOConn::EParameterError:
			panic = TNwssWspTrHndPanic::EParameterError;
			break;
		case RWSPCOConn::ESessionNotConnected:
			panic = TNwssWspTrHndPanic::ESessionNotConnected;
			break;
		case RWSPCOConn::ENotSupported:
			panic = TNwssWspTrHndPanic::ENotSupported;
			break;
		case RWSPCOConn::EInvalidState:
			panic = TNwssWspTrHndPanic::EInvalidState;
			break;
		case RWSPCOConn::ESessionClosed:
			panic = TNwssWspTrHndPanic::ESessionClosed;
			break;
		case RWSPCOConn::EMRUExceeded:
			panic = TNwssWspTrHndPanic::EMRUExceeded;
			break;
		case RWSPCOConn::EInvalidBufferSize:
			panic = TNwssWspTrHndPanic::EInvalidBufferSize;
			break;
		case RWSPCOConn::EDataNotAvailable:
			panic = TNwssWspTrHndPanic::EDataNotAvailable;
			break;
		case RWSPCOConn::EErrorTimeout:
			panic = TNwssWspTrHndPanic::EErrorTimeout;
			break;


	default:
		panic = TNwssWspTrHndPanic::EUnknownPanic;
		}
	TNwssWspTrHndPanic::Panic(panic);
	}

void CNwssWspSession::CompleteSelf()
	{
	if (!IsActive())
		{
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrNone);
		SetActive();
		}
	}


void CNwssWspSession::DoWTLSConnectionPreHandshakeL()
	{
	// Set the Key Exchange Suites. There might be several desired cipher suites, so
	// they are returned as an array
	const RArray<TWtlsKeyExchangeSuite>& keyExchSuites = 
		iSecurityPolicy.GetWtlsKeyExchangeSuites();
	TInt idx = 0;
	const TInt numKeyExchSuites = keyExchSuites.Count();
	RWTLS::TKeyExchangeSuite stackSuite;
	RWTLS::TIdType stackIdType;
	for (idx = 0; idx < numKeyExchSuites; ++idx)
		{
		// Get the next suite, convert and set
		TWtlsKeyExchangeSuite suite = keyExchSuites[idx];
		stackSuite = (RWTLS::TKeyExchangeSuite)suite.iKeyExchangeSuite;
		stackIdType = (RWTLS::TIdType)suite.iKeyIdType;
#ifdef _DEBUG
		DumpKeyExchangeSuite(stackSuite, stackIdType, ETrue);
#endif
		__TESTOOMD(stkErr, WtlsHnd().SetKeyExchangeSuite(stackSuite, stackIdType, suite.iKeyId));
		if (stkErr < 0)
			{
			__LOG1("--RWTLS::SetKeyExchangeSuite returned %d", stkErr);
			__LOG1("--Offensive key ex suite is %d", suite.iKeyExchangeSuite);
			User::Leave(stkErr);
			}
		}

	// Set the Cipher Suites. There might be several desired cipher suites, so
	// they are returned as an array
	const RArray<TWtlsCipherSuite>& cipherSuites =
		iSecurityPolicy.GetWtlsCipherSuites();
	const TInt numCipherSuites = cipherSuites.Count();
	if(numCipherSuites>0)
		{
		// Make an array to pass in to RWTLS::SetCipherSuites
		RWTLS::CipherSuiteArray* stackSuites =
						new (ELeave) RWTLS::CipherSuiteArray(numCipherSuites);
		CleanupStack::PushL(stackSuites);
		for (idx = 0; idx < numCipherSuites; ++idx)
			{
			// Get the next suite, convert and append to the stack array
			TWtlsCipherSuite suite = cipherSuites[idx];
			RWTLS::TCipherSuite stackSuite;
			stackSuite.iBulkEncryptionAlgorithm =
				(RWTLS::TBulkEncryptionAlgorithm)suite.iBulkEncryptionAlgorithm;
			stackSuite.iMacAlgorithm =
				(RWTLS::TMacAlgorithm)suite.iMacAlgorithm;
#ifdef _DEBUG
			DumpCipherSuite(stackSuite, ETrue);
#endif
			stackSuites->AppendL(stackSuite);
			}
		__TESTOOMD(stkErr, WtlsHnd().SetCipherSuites(*stackSuites));
		if (stkErr < 0)
			{
			__LOG1("--RWTLS::SetCipherSuites returned %d", stkErr);
			User::Leave(stkErr);
			}

		CleanupStack::PopAndDestroy(stackSuites);
		}

	// Set the Sequence Number Mode.
	RString  seqNumModeValStr;
	RString seqNumModePropName =
		iStringPool.String(SecurityPolicyProperties::ESequenceNumberMode,
							SecurityPolicyProperties::Table);

	RStringF seqNumName = iStringPool.OpenFStringL(seqNumModePropName.DesC());
	CleanupClosePushL(seqNumName);

	TInt policyErr = iSecurityPolicy.GetNamedPolicyProperty(seqNumName,
															seqNumModeValStr);
	CleanupClosePushL(seqNumModeValStr);

	if (policyErr == KErrNone)
		{
		RWTLS::TSequenceNumberMode stackMode;
		switch (seqNumModeValStr.Index(SecurityPolicyProperties::Table))
			{
			case SecurityPolicyProperties::EImplicit:
				stackMode = RWTLS::EImplicit;
				break;
			case SecurityPolicyProperties::EExplicit:
				stackMode = RWTLS::EExplicit;
				break;
			case SecurityPolicyProperties::ENotUsed:
			default:
				stackMode = RWTLS::ENotUsed;
				break;
			}		
#ifdef _DEBUG
			DumpSequenceNumberMode(stackMode, ETrue);
#endif
		__TESTOOMD(stkErr, WtlsHnd().SetSequenceNumberMode(stackMode));
		if (stkErr < 0)
			{
			__LOG1("--RWTLS::SetSequenceNumberMode returned %d", stkErr);
			User::Leave(stkErr);
			}
		}
	CleanupStack::PopAndDestroy(2); // seqNumName, seqNumModeValStr


	// Set the Key Refresh Rate.
	RString  keyRefreshRateValStr;

	RString keyRefreshRatePropName =
		iStringPool.String(SecurityPolicyProperties::EKeyRefreshRate,
							SecurityPolicyProperties::Table);

	RStringF refreshRateName = iStringPool.OpenFStringL(keyRefreshRatePropName.DesC());
	CleanupClosePushL(refreshRateName);

	policyErr = iSecurityPolicy.GetNamedPolicyProperty(refreshRateName,
													   keyRefreshRateValStr);
	CleanupClosePushL(keyRefreshRateValStr);

	if (policyErr == KErrNone)
		{
		TUint8 stackKRR = 0;
		TLex8 lexer(keyRefreshRateValStr.DesC());
		User::LeaveIfError( lexer.Val(stackKRR, EDecimal) );
#ifdef _DEBUG
		DumpKeyRefreshRate(stackKRR, ETrue);
#endif
		__TESTOOMD(stkErr, WtlsHnd().SetKeyRefreshRate(stackKRR));
		if (stkErr < 0)
			{
			__LOG1("--RWTLS::SetKeyRefreshRate returned %d", stkErr);
			User::Leave(stkErr);
			}
		}
	CleanupStack::PopAndDestroy(2); // refreshRateName, keyRefreshRateValStr


	// Set the Shared Secret
	RString  sharedSecretValStr;

	RString sharedSecretPropName =
		iStringPool.String(SecurityPolicyProperties::ESharedSecret,
							SecurityPolicyProperties::Table);

	RStringF sharedSecretName = iStringPool.OpenFStringL(sharedSecretPropName.DesC());
	CleanupClosePushL(sharedSecretName);

	policyErr = iSecurityPolicy.GetNamedPolicyProperty(sharedSecretName,
													   sharedSecretValStr);
	CleanupClosePushL(sharedSecretValStr);

	if (policyErr == KErrNone)
		{
		const TDesC8& stackSS = sharedSecretValStr.DesC();
#ifdef _DEBUG
		DumpSharedSecret(stackSS);
#endif
		__TESTOOMD(stkErr, WtlsHnd().SetSharedSecret(stackSS));
		if (stkErr < 0)
			{
			__LOG1("--RWTLS::SetSharedSecret returned %d", stkErr);
			User::Leave(stkErr);
			}
		}
	CleanupStack::PopAndDestroy(2); // sharedSecretName, sharedSecretValStr


	// Set the Record Length Usage.
	RString  recordLengthUsageValStr;

	RString recordLengthUsagePropName =
		iStringPool.String(SecurityPolicyProperties::ERecordLengthUsage,
							SecurityPolicyProperties::Table);

	RStringF recLenUsageName = iStringPool.OpenFStringL(recordLengthUsagePropName.DesC());
	CleanupClosePushL(recLenUsageName);

	policyErr = iSecurityPolicy.GetNamedPolicyProperty(recLenUsageName,
													   recordLengthUsageValStr);
	CleanupClosePushL(recordLengthUsageValStr);

	if (policyErr == KErrNone)
		{
		TBool stackRLU = EFalse;
		switch (recordLengthUsageValStr.Index(SecurityPolicyProperties::Table))
			{
			case SecurityPolicyProperties::EUsed:
				stackRLU = ETrue;
				break;
			case SecurityPolicyProperties::ENotUsed:
			default:
				stackRLU = EFalse;
				break;
			}
#ifdef _DEBUG
		DumpRecordLengthUsage(stackRLU, ETrue);
#endif
		__TESTOOMD(stkErr, WtlsHnd().SetRecordLengthUsage(stackRLU));
		if (stkErr < 0)
			{
			__LOG1("--RWTLS::SetRecordLengthUsage returned %d", stkErr);
			User::Leave(stkErr);
			}
		}
	CleanupStack::PopAndDestroy(2); // recLenUsageName, recordLengthUsageValStr
	}


void CNwssWspSession::DoWTLSConnectionPhaseOne(TBool aTwoPhaseHandshake)
	{
	WtlsHnd().Connect(iStatus, aTwoPhaseHandshake);
	SetActive();
	__LOG1("--WTLS::Connect(phase 1 of %d)", (aTwoPhaseHandshake?2:1));
	}


void CNwssWspSession::CancelWTLSConnectionPhaseOne()
	{
	WtlsHnd().CancelConnect();
	__LOG("--WTLS - CancelConnect() phase 1");
	}


void CNwssWspSession::ValidateUntrustedServerCertL()
	{
	// Make request of security policy plugin for validation
	iSecurityPolicy.ValidateUntrustedServerCert(iServerCert, iStatus);
	SetActive();
	}

void CNwssWspSession::GetServerCertL()
	{
	iServerCertValid = EFalse;
	// Create space for subject, issuer and fingerprint fields in cert info
	TBuf8<KWtlsMaxFieldSize> subject;
	TBuf8<KWtlsMaxFieldSize> issuer;
	TBuf8<KWtlsMaxFieldSize> fingerprint;

	RCertificate::TInfo wtlsCertInfo;
#ifdef _DEBUG
	// shut up some WINS UDEB compiler warnings...
	wtlsCertInfo.iValidNotBefore = 0;
	wtlsCertInfo.iValidNotAfter = 0;
	wtlsCertInfo.iSubjectCharacterSet = 0;
	wtlsCertInfo.iIssuerCharacterSet = 0;
	wtlsCertInfo.iServerCertificateStatus = RCertificate::EOk;
#endif
	wtlsCertInfo.iSubject		= &subject;
	wtlsCertInfo.iIssuer		= &issuer;
	wtlsCertInfo.iFingerPrint	= &fingerprint;

	// Get the certificate from the WAP stack
	RCertificate wtlsCert = WtlsHnd().ServerCert();
	__TESTOOMD(stkErr, wtlsCert.Info(wtlsCertInfo));
	if (stkErr != KErrNone)
		User::Leave(KWtlsErrInvalidServerCert);

	// Convert to a form accepted by the security policy plugin
 	_LIT(KUnixEpochStart,"19700000:000000.000000");

	// Start date
	TTime startTime(KUnixEpochStart);
	TTimeIntervalSeconds addTime = wtlsCertInfo.iValidNotBefore;
	iServerCert.iStartValDate = startTime + addTime;

	// End date
	TTime endTime(KUnixEpochStart);
	addTime = wtlsCertInfo.iValidNotAfter;
	iServerCert.iEndValDate = endTime + addTime;

	// Subject. Ignore the character set from the WTLS cert - it should be UTF-8
	TInt len = Min((wtlsCertInfo.iSubject)->Length(),
				   iServerCert.iSubjectDNInfo.iCommonName.MaxLength());
	iServerCert.iSubjectDNInfo.iCommonName.Copy((wtlsCertInfo.iSubject)->Left(len));

	// Issuer. Ignore the character set from the WTLS cert - it should be UTF-8
	len = Min((wtlsCertInfo.iIssuer)->Length(),
				   iServerCert.iIssuerDNInfo.iCommonName.MaxLength());
	iServerCert.iIssuerDNInfo.iCommonName.Copy((wtlsCertInfo.iIssuer)->Left(len));

	// Ignore server certificate status - no field exists for it in TCertInfo

	// Fingerprint
	len = Min((wtlsCertInfo.iFingerPrint)->Length(), iServerCert.iFingerprint.MaxLength());
	iServerCert.iFingerprint.Copy((wtlsCertInfo.iFingerPrint)->Left(len));

	// We have a complete TCertInfo
	iServerCertValid = ETrue;

	// Log certificate data
	__LOG("--Server Certificate:");
	__LOG("-----------------------------------");
	__LOG1("--Subject:     %S", wtlsCertInfo.iSubject);
	__LOG1("--Issuer:      %S", wtlsCertInfo.iIssuer);
	__LOG1("--Fingerprint: %S", wtlsCertInfo.iFingerPrint);
#ifdef _DEBUG
	_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");
	TBuf<40> dateTimeString;
	TBuf8<40> dateTimeString8;
	__DEBUGTESTLEAVE
	iServerCert.iStartValDate.FormatL(dateTimeString, KDateFormat);
	dateTimeString8.Copy(dateTimeString);
	__LOG1("--Start Date:  %S", &dateTimeString8);
	__DEBUGTESTLEAVE
	iServerCert.iEndValDate.FormatL(dateTimeString, KDateFormat);
	dateTimeString8.Copy(dateTimeString);
	__LOG1("--Start Date:  %S", &dateTimeString8);
#endif
	__LOG("-----------------------------------");
	}

void CNwssWspSession::CancelValidateUntrustedServerCert()
	{
	iSecurityPolicy.CancelValidateUntrustedServerCert();
	}

void CNwssWspSession::DoWTLSConnectionPhaseTwo()
	{
	WtlsHnd().ConnectPhaseTwo(iStatus);
	SetActive();
	__LOG("--WTLS::Connect(phase 2 of 2)");
	}


void CNwssWspSession::CancelWTLSConnectionPhaseTwo()
	{
	WtlsHnd().CancelConnectPhaseTwo();
	__LOG("--WTLS - CancelConnect() phase 2");
	}


void CNwssWspSession::DoWTLSConnectionPostHandshakeL()
	{
	TInt stkErr = KErrNone;
	// Dump the negotiated cipher suite and sequence number mode (debug only)
#ifdef _DEBUG
	// Find out which cipher suite was negotiated
	RWTLS::TCipherSuite cipherSuite;

	// shut up some WINS UDEB compiler warnings
	cipherSuite.iBulkEncryptionAlgorithm = RWTLS::EBulkNull;
	cipherSuite.iMacAlgorithm = RWTLS::ESha_0;
	__TESTOOM(stkErr, WtlsHnd().GetCipherSuite(cipherSuite));
	if (stkErr)
		{
		__LOG1("--RWTLS::GetCipherSuite() returned %d", stkErr);
		}
	else
		DumpCipherSuite(cipherSuite, EFalse);

	// Find out which sequence number mode was negotiated
	RWTLS::TSequenceNumberMode mode = RWTLS::ENotUsed;
	__TESTOOM(stkErr, WtlsHnd().GetSequenceNumberMode(mode));
	if (stkErr)
		{
		__LOG1("--RWTLS::GetSequenceNumberMode() returned %d", stkErr);
		}
	else
		DumpSequenceNumberMode(mode, EFalse);
#endif

	// Find out which key exchange suite was negotiated
	RWTLS::TKeyExchangeSuite keyExchSuite(RWTLS::ESharedSecret);
	__TESTOOM(stkErr, WtlsHnd().GetKeyExchangeSuite(keyExchSuite));
	if (stkErr == KErrNone)
		{
#ifdef _DEBUG
		// Dump the negotiated key exch suite (debug only)
		DumpKeyExchangeSuite(keyExchSuite, (RWTLS::TIdType)KErrNotFound, EFalse);
#endif

		// Figure out if we got an anonymous one or not.  Note, this is
		// authoritative, so overwrite any flag set earlier.
		switch (keyExchSuite)
			{
			case RWTLS::EDHAnon:
			case RWTLS::EDHAnon512:
			case RWTLS::EDHAnon768:
			case RWTLS::ERsaAnon:
			case RWTLS::ERsaAnon512:
			case RWTLS::ERsaAnon768:
			case RWTLS::EEcdhAnon:
			case RWTLS::EEcdhAnon113:
			case RWTLS::EEcdhAnon131:
				iHandshakeWasAnonymous = ETrue;
			default:
				iHandshakeWasAnonymous = EFalse;
				break;
			}
		}
#ifdef _DEBUG
	else
		{
		__LOG1("--RWTLS::GetKeyExchangeSuite() returned %d", stkErr);
		}
#endif

	// If it's anonymous, and anonymous is not allowed, then reject now.
	__DEBUGTESTLEAVE
	if (iHandshakeWasAnonymous && !AnonymousAuthenticationAllowedL())
		User::Leave(KWtlsErrNegotiatedConfigRejected);
	}


TBool CNwssWspSession::AnonymousAuthenticationAllowedL()
	{
	RString  anonAllowedValStr;
	RString anonAllowedPropName =
		iStringPool.String(SecurityPolicyProperties::EAnonymousAllowed,
							SecurityPolicyProperties::Table);

	__DEBUGTESTLEAVE
	RStringF anonAllowName = iStringPool.OpenFStringL(anonAllowedPropName.DesC());

	TInt policyErr = iSecurityPolicy.GetNamedPolicyProperty(anonAllowName,
															anonAllowedValStr);
	TBool retVal = ( (policyErr == KErrNone) &&
					 (anonAllowedValStr == iStringPool.String(SecurityPolicyProperties::EAllowed,
							SecurityPolicyProperties::Table) ) );

	anonAllowedValStr.Close();
	anonAllowName.Close();
	return retVal;
	}

TInt CNwssWspSession::ServerCert(TCertInfo& aCertInfo)
	{
	TInt retVal = KErrNotSupported;
	switch(iState)
		{
		case EDisconnected:
		case ESessionOpen:
		case EDoingWtlsPhase1:
			retVal = KErrNotReady;
			break;
		case ECheckingServerCert:
		case EDoingWtlsPhase2:
		case ECheckingNegotiated:
		case EReady:
			if(iServerCertValid)
				{
				retVal = KErrNone;
				aCertInfo = iServerCert;
				}
			break;
		default:
			break;
		}

	return retVal;
	}

#ifdef _DEBUG

void CNwssWspSession::DumpCipherSuite(RWTLS::TCipherSuite aCipherSuite, TBool aRequested)
	{
	TBuf8<100> msg;
	if (aRequested)
		{
		_LIT8(KCipherSuiteMsg, "--Requested cipher suite is: (");
		msg.Append(KCipherSuiteMsg());
		}
	else
		{
		_LIT8(KCipherSuiteMsg, "--Negotiated cipher suite is: (");
		msg.Append(KCipherSuiteMsg());
		}
	switch (aCipherSuite.iBulkEncryptionAlgorithm)
		{
		case RWTLS::EBulkNull:
			msg.Append(_L("EBulkNull,"));
			break;
		case RWTLS::ERc5_cbc_40:
			msg.Append(_L("ERc5_cbc_40,"));
			break;
		case RWTLS::ERc5_cbc_56:
			msg.Append(_L("ERc5_cbc_56,"));
			break;
		case RWTLS::ERc5_cbc:
			msg.Append(_L("ERc5_cbc,"));
			break;
		case RWTLS::EDes_cbc_40:
			msg.Append(_L("EDes_cbc_40,"));
			break;
		case RWTLS::EDes_cbc:
			msg.Append(_L("EDes_cbc,"));
			break;
		case RWTLS::E3Des_cbc_ede:
			msg.Append(_L("E3Des_cbc_ede,"));
			break;
		case RWTLS::EIdea_cbc_40:
			msg.Append(_L("EIdea_cbc_40,"));
			break;
		case RWTLS::EIdea_cbc_56:
			msg.Append(_L("EIdea_cbc_56,"));
			break;
		case RWTLS::EIdea_cbc:
			msg.Append(_L("EIdea_cbc,"));
			break;
		default:
			msg.Append(_L("<Unknown bulk alg>,"));
			break;
		}
	switch (aCipherSuite.iMacAlgorithm)
		{
		case RWTLS::ESha_0:
			msg.Append(_L("ESha_0)"));
			break;
		case RWTLS::ESha_40:
			msg.Append(_L("ESha_40)"));
			break;
		case RWTLS::ESha_80:
			msg.Append(_L("ESha_80)"));
			break;
		case RWTLS::ESha:
			msg.Append(_L("ESha)"));
			break;
		case RWTLS::ESha_xor_40:
			msg.Append(_L("ESha_xor_40)"));
			break;
		case RWTLS::EMd5_40:
			msg.Append(_L("EMd5_40)"));
			break;
		case RWTLS::EMd5_80:
			msg.Append(_L("EMd5_40)"));
			break;
		case RWTLS::EMd5:
			msg.Append(_L("EMd5)"));
			break;
		default:
			msg.Append(_L("<Unknown mac alg>)"));
			break;
		}
	__LOG1("%S", &msg);
	}

void CNwssWspSession::DumpKeyExchangeSuite(RWTLS::TKeyExchangeSuite aKeyExchSuite, RWTLS::TIdType aStackIdType, TBool aRequested)
	{
	TBuf8<100> msg;
	if (aRequested)
		{
		_LIT8(KKeyExchSuiteMsg, "--Requested key exchange suite is: ");
		msg.Append(KKeyExchSuiteMsg());
		}
	else
		{
		_LIT8(KKeyExchSuiteMsg, "--Negotiated key exchange suite is: ");
		msg.Append(KKeyExchSuiteMsg());
		}
	switch (aKeyExchSuite)
		{
		case RWTLS::ESharedSecret:
			msg.Append(_L("(ESharedSecret,"));
			break;
		case RWTLS::EDHAnon:
			msg.Append(_L("(EDHAnon,"));
			break;
		case RWTLS::EDHAnon512:
			msg.Append(_L("(EDHAnon512,"));
			break;
		case RWTLS::EDHAnon768:
			msg.Append(_L("(EDHAnon768,"));
			break;
		case RWTLS::ERsaAnon:
			msg.Append(_L("(ERsaAnon,"));
			break;
		case RWTLS::ERsaAnon512:
			msg.Append(_L("(ERsaAnon512,"));
			break;
		case RWTLS::ERsaAnon768:
			msg.Append(_L("(ERsaAnon768,"));
			break;
		case RWTLS::ERsa:
			msg.Append(_L("(ERsa,"));
			break;
		case RWTLS::ERsa512:
			msg.Append(_L("(ERsa512,"));
			break;
		case RWTLS::ERsa768:
			msg.Append(_L("(ERsa768,"));
			break;
		case RWTLS::EEcdhAnon:
			msg.Append(_L("(EEcdhAnon,"));
			break;
		case RWTLS::EEcdhAnon113:
			msg.Append(_L("(EEcdhAnon113,"));
			break;
		case RWTLS::EEcdhAnon131:
			msg.Append(_L("(EEcdhAnon131,"));
			break;
		case RWTLS::EEcdhEcdsa:
			msg.Append(_L("(EEcdhEcdsa,"));
			break;
		default:
			msg.Append(_L("(<Unknown key exchange suite>,"));
			break;
		}
		switch (aStackIdType)
		{
		case RWTLS::EIdNull:
			msg.Append(_L("EIdNull)"));
			break;
		case RWTLS::EText:
			msg.Append(_L("EText)"));
			break;
		case RWTLS::EBinary:
			msg.Append(_L("EBinary)"));
			break;
		case RWTLS::EKeyHashSha:
			msg.Append(_L("EKeyHashSha)"));
			break;
		case RWTLS::EX509Name:
			msg.Append(_L("EX509Name)"));
			break;
		default:
			msg.Append(_L("<Unknown ID type>)"));
			break;
		}
	__LOG1("%S", &msg);
	}


void CNwssWspSession::DumpSequenceNumberMode(RWTLS::TSequenceNumberMode aSeqNumMode, TBool aRequested)
	{
	TBuf8<100> msg;
	if (aRequested)
		{
		_LIT8(KSeqNumModeMsg, "--Requested sequence number mode is: ");
		msg.Append(KSeqNumModeMsg());
		}
	else
		{
		_LIT8(KSeqNumModeMsg, "--Negotiated sequence number mode is: ");
		msg.Append(KSeqNumModeMsg());
		}
	switch (aSeqNumMode)
		{
		case RWTLS::ENotUsed: 
			msg.Append(_L("ENotUsed"));
			break;
		case RWTLS::EImplicit:
			msg.Append(_L("EImplicit"));
			break;
		case RWTLS::EExplicit:
			msg.Append(_L("EExplicit"));
			break;
		default:
			msg.Append(_L("<Unknown>"));
			break;
		}
	__LOG1("%S", &msg);
	}

void CNwssWspSession::DumpKeyRefreshRate(TUint8 aKeyRefreshRate, TBool aRequested)
	{
	_LIT8(KReqKeyRefRateMsg, "--Requested key refresh rate is: %d");
	_LIT8(KNegKeyRefRateMsg, "--Negotiated key refresh rate is: %d");
	TBuf8<100> msg;
	if (aRequested)
		{
		msg.Format(KReqKeyRefRateMsg(), aKeyRefreshRate);
		}
	else
		{
		msg.Format(KNegKeyRefRateMsg(), aKeyRefreshRate);
		}
	__LOG1("%S", &msg);
	}

void CNwssWspSession::DumpSharedSecret(const TDesC8& aSharedSecret)
	{
	__LOG("--Shared secret is: ");
	__DUMP("--", "--", aSharedSecret)
	}

void CNwssWspSession::DumpRecordLengthUsage(TBool aRecordLengthUsage, TBool aRequested)
	{
	_LIT8(KReqRecLenUsageMsg, "--Requested record length usage is: %S");
	_LIT8(KNegRecLenUsageMsg, "--Negotiated record length usage is: %S");
	_LIT8(KUsed, "Used");
	_LIT8(KNotUsed, "Not Used");
	TBuf8<100> msg;
	if (aRequested)
		{
		msg.Format(KReqRecLenUsageMsg(), aRecordLengthUsage?&(KUsed()):&(KNotUsed()));
		}
	else
		{
		msg.Format(KNegRecLenUsageMsg(), aRecordLengthUsage?&(KUsed()):&(KNotUsed()));
		}
	__LOG1("%S", &msg);
	}


#endif
