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

/**
 @file WspTypes.h
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __WSPTYPES_H__
#define __WSPTYPES_H__

// System includes
#include <e32base.h>
#include <stringpool.h>

//TODO - These will move to a common WAPMSG/WSPTransport file at a later stage,
//in the wap-stack subsystem, to be shared with the WAP Messaging API.

namespace Wap
/**
All type definitions and enumerations used in WSP are in placed in a wider WAP
namespace, to avoid name conflicts with other components.
@publishedAll
@released
*/
{

/** The TWspPrimitive type represents the WSP primitives defined in the WSP
	Specification, July 2001.
*/
enum TWspPrimitive
	{
	/**
	*/
	ESConnect			= 0,
	/**
	*/
	ESDisconnect,
	/**
	*/
	ESSuspend,
	/**
	*/
	ESResume,
	/**
	*/
	ESException,
	/**
	*/
	ESMethodInvoke,
	/**
	*/
	ESMethodInvokeData,
	/**
	*/
	ESMethodResult,
	/**
	*/
	ESMethodResultData,
	/**
	*/
	ESMethodAbort,
	/**
	*/
	ESPush,
	/**
	*/
	ESConfirmedPush,
	/**
	*/
	ESPushAbort
	};

/** The TWspProtocolOptions type represents the optional protocol functions 
	defined in the WSP Specification, July 2001. When the client sends the 
	Protocol Capability to the server, it indicates which functions it will
	accept from the server. The server returns from the set of functions sent
	by the client those that it will perform.
*/
enum TWspProtocolOptions
	{
	/** The Large Data Transfer feature includes support for multiple SDU's and
		support for the Data Fragment PDU. Setting this falg indicates support 
		for sending and receiving large data. If a client only wishes to receive
		large data, then it should set the Client and Server Message size fields
		appropriately.
	*/
	ELargeDataTransfer			= 0x08,
	/** The acknowledgement headers flag specifies that the client wishes to send
		acknowledgement headers. If the server cannot process them (indicated by
		negotiating down the flag) the client should not send the headers as they
		will be ignored.
	*/
	EAcknowledgementHeaders		= 0x10,
	/** The client enables the Suspend Resume facility when it wishes to suspend
		and resume the session. If the server does not support suspend and resume
		then this flag must be cleared in the negotiated capabilities.
	*/
	ESessionResumeFacility		= 0x20,
	/** The client uses this flag to indicate that it is able and willing to 
		receive push information. If the server cannot send pushes then this flag 
		must be cleared in the negotiated capabilities.
	*/
	EPushFacility				= 0x40,
	/** The client uses this flag to indicate that it is able and willing to 
		receive confirmed push information. If the server cannot send pushes 
		then this flag must be cleared in the negotiated capabilities.
	*/
	EConfirmedPushFacility		= 0x80
	};

/** The TWspSessionState type represents the states of a WSP session, as defined
	by the WSP Specification, July 2001.
*/
enum TWspSessionState
	{
	/** 
	*/
	ENull			= 0,
	/**
	*/
	EConnecting,
	/**
	*/
	EConnected,
	/**
	*/
	ESuspending,
	/**
	*/
	ESuspended,
	/**
	*/
	EClosing,
	/**
	*/
	EResuming
	};

/** The TWspMethodState type represents the states of a WSP method transaction,
	as defined by the WSP Specification, July 2001.
*/
enum TWspMethodState
	{
	/**
	*/
	ENullMethod			=0,
	/**
	*/
	ERequesting,
	/**
	*/
	EWaiting,
	/**
	*/
	EWaiting2,
	/**
	*/
	ECompleting,
	/**
	*/
	EAborting
	};

/** The TWspBearer type defines the set of supported bearers.
*/
enum TWspBearer
	{
	/** Specifier for any bearer.
	*/
	EAny		= 0,
	/** IP bearer.
	*/
	EIP,
	/** A 7-bit SMS bearer.
	*/
	ESMS7,
	/** An 8-bit SMS bearer.
	*/
	ESMS,
	/** A 7-bit WAP SMS bearer.
	*/
	EWAPSMS7,
	/** An 8-bit WAP SMS bearer.
	*/
	EWAPSMS
	};

/** The TWspSession type defines the 2 types of WSP Session Service; Connection
	Oriented and Connectionless.
*/
enum TWspSession
	{
	/** Specifier for Connection Oriented Session Service.
	*/
	EWspConnectionOriented			= 0,
	/** Specifier for Connectionless Session Service.
	*/
	EWspConnectionLess
	};

/** The TWspReason type is used by to report the cause of a particular indication 
	primitive. The WSP Specification, July 2001, specifies a given set that MUST
	be recognised (6.2.5). Further values may be added.
*/
enum TWspReason
	{
	/** The rules of the protocol were broken and in its current state, the peer
		could not perform the specified operation. E.g. the PDU was not allowed.
	*/
	EProtoErr			= 0xE0,
	/** The session was disconnected while the operation was still in progress.
	*/
	EDisconnect			= 0xE1,
	/** The session was suspended while the operation was still in progress.
	*/
	ESuspend			= 0xE2,
	/** The session was resumed while the operation was still in progress.
	*/
	EResume				= 0xE3,
	/** The request could not be processed due to a lack of resources.
	*/
	ECongestion			= 0xE4,
	/** The connection was prevented due to an error.
	*/ 
	EConnectErr			= 0xE5,
	/** The SDU size in a request exceeded the maximum negotiated SDU size.
	*/
	EMRUExceeded		= 0xE6,
	/** The negotiated value of the maximum outstanding transactions (either for
		push or method transactions) has been exceeded.
	*/
	EMORExceeded		= 0xE7,
	/** The operation was aborted by the Provider. 
	*/
	EPeerReq			= 0xE8,
	/** An underlying network error prevented the completion of a request.
	*/
	ENetErr				= 0xE9,
	/** The cause of the indication was an action by the Client.
	*/
	EUserReq			= 0xEA,
	/** The client has refesed the Push message, no specific reason, no retries.
	*/
	EUserRfs			= 0xEB,
	/** The Push message cannot be delivered to the intended destination.
	*/
	EUserPnd			= 0xEC,
	/** The Push message was discarded due to resource shortage.
	*/
	EUserDcr			= 0xED,
	/** The content-type of the Push message cannot be processed.
	*/
	EUserDcu			= 0xEE,
	/** The session connect request has been redirected to a new proxy. This is
		a temporary redirection and the original proxy should be used in the
		future when trying to access this service. This is not defined by the 
		WSP Specification.
	*/
	ETemporaryRedirectedProxy	=0,
	/** The session connect request has been redirected to a new proxy. This is
		a permanent redirection and the new proxy should be used in the future 
		when trying to access this service. The client will be notified of this
		event and the HTTP session property EWspProxyAddressnew will contain the
		new proxy's address. The client should update its access point database.
		This is not defined by the WSP Specification.
	*/
	EPermanentRedirectedProxy,
	/** The client had disconnected the session, but as Suspend Resume facility
		was being used, the session was suspended. The client then changed the
		proxy and did a connect. The suspended session cannot be resumed, so the
		the current session must be disconnected (with this reason) and a 
		session connect initiated with the new proxy. This is not defined by the
		WSP Specification.
	*/
	EChangedProxyInSuspendedSession,
	/** The client's WSP session was disconnected in the Connecting state because
		configuration of the underlying WTLS layer failed prior to the phase 1
		WTLS handshake.
	*/
	EWtlsConfigurationFailed,
	/** The client's WSP session was disconnected in the Connecting state because
		phase 1 of the WTLS handshake failed.
	*/
	EWtlsPhase1HandshakeFailed,
	/** The client's WSP session was disconnected in the Connecting state because
		phase 2 of the WTLS handshake failed.
	*/
	EWtlsPhase2HandshakeFailed,
	/** The client's WSP session was disconnected in the Connecting state because
		the proxy's WTLS certificate was invalid - e.g. badly formed, or out of date.
	*/
	EWtlsInvalidServerCert,
	/** The client's WSP session was disconnected in the Connecting state because
		the proxy's WTLS certificate could not be trusted.  This means it was not signed
		by any root certificate on the device, and the security policy criteria could not
		determine further whether it could be trusted or not.
	*/
	EWtlsUntrustedServerCert,
	/** The client's WSP session was disconnected in the Connecting state because
		the negotiated WTLS configuration at completion of the secure handshake was
		rejected by criteria in the security policy.
	*/
	EWtlsNegotiatedConfigRejected,
	/** The client's transaction or session has failed due to an out-of-memory situation.
	*/
	EOutOfMemory,
	/** The client's session has entered an inconsistent state, probably due to a failure caused
		by an out-of-memory situation.
	*/
	ESessionStateFailure
	};

/**	The enum TWspCapabilty is used to select a capability to reset (if required) 
	before setting the value of the capability.
*/
enum TWspCapability
	{
	/** Specifies the Alias Addresses capability.
	*/
	EAliasAddresses				= 0,
	/** Specifies the Extended Methods capability.
	*/
	EExtendedMethods,
	/** Specifies the Header Code Pages capability.
	*/
	EHeaderCodePages,
	/** Specifies the Method Maximum Outstanding Requests capability. Default value is 1.
	*/
	EMethodMOR,
	/** Specifies the Push Maximum Outstanding Requests capability. Default value is 1.
	*/
	EPushMOR,
	/** Specifies the Protocol Options capability. Default value is 0x00.
	*/
	EProtocolOptions,
	/** Specifies the Client SDU size capability. Default value is 1400 octets.
	*/
	EClientSDUSize,
	/** Specifies the Server SDU size capability. Default value is 1400 octets.
	*/
	EServerSDUSize,
	/** Specifies the Client Message size capability. Default value is 1400 octets.
	*/
	EClientMessageSize,
	/** Specifies the Server Message size capability. Default value is 1400 octets.
	*/
	EServerMessageSize,
	/** Specifies the Unknown capabilities.
	*/
	EUnknownCapabilities,
	/** Specifies all the capabilities.
	*/
	EAllCapabilities
	};

} // end of namespace Wap


/**
Maximum Proxy address length, this is large enough for a full IPv6 address.
@publishedAll
@released
*/
const TInt KMaxProxyAddrLen = 40;

//##ModelId=3C4C41B20079
class TWspRedirectedAddress
/**	
The TWspRedirectedAddress type represents an alternate address to which 
Clients must use to establish a session with the same service that was 
initially contacted. If the bearer or port fields have been excluded, then
the orginal bearer and port should be used to contact the new server. The 
WSP Specification, July 2001, defines an Address Type (8.2.2.3) upon which 
this class is based.
@publishedAll
@released
*/
	{
public:	// Attributes

	/** A flag to indicate that the port field is set.
	*/
	//##ModelId=3C4C41B200DD
	TBool		iHasPort;

	/** A flag to indicate that the bearer field is set.
	*/
	//##ModelId=3C4C41B200CB
	TBool		iHasBearer;

	/** The proxy address.
	*/
	//##ModelId=3C4C41B200C1
	TBuf8<KMaxProxyAddrLen>	iProxyAddress;

	/** The port number. 
	*/
	//##ModelId=3C4C41B200B5
	TUint16		iPort;

	/** The bearer type.
	*/
	//##ModelId=3C4C41B200A3
	Wap::TWspBearer	iBearer;
	};

/**
Maximum length of a key ID for WTLS
@publishedAll
@released
*/
const TInt KWtlsMaxKeyIdLength = 512;

class TWtlsKeyExchangeSuite
/**
The class TWtlsKeyExchangeSuite contain the definitions of WTLS
Key Exchange Suites as defined in the WAP WTLS Specification, July 2001
@publishedAll
@released 
*/
	{
public:
	enum TKeyExchangeSuite
		{
		ENULL					=0,
		ESHARED_SECRET			=1,
		EDH_anon				=2,
		EDH_anon_512			=3,
		EDH_anon_768			=4,
		ERSA_anon				=5,
		ERSA_anon_512			=6,
		ERSA_anon_768			=7,
		ERSA					=8,
		ERSA_512				=9,
		ERSA_768				=10,
		EECDH_anon				=11,
		EECDH_anon_113			=12,
		EECDH_anon_131			=13,
		EECDH_ECDSA				=14,
		EECDH_anon_uncomp		=15,
		EECDH_anon_uncomp_113	=16,
		EECDH_anon_uncomp_131	=17,
		EECDH_ECDSA_uncomp		=18
		};
	enum TKeyIdType 
		{
		EIdNull			= 0,
		EText			= 1, 
		EBinary			= 2, 
		EKeyHashSha		= 254, 
		EX509Name		= 255
		};

public:
	TWtlsKeyExchangeSuite(TKeyExchangeSuite aKeyExchangeSuite, TKeyIdType aKeyIdType, const TBuf8<KWtlsMaxKeyIdLength>& aKeyId);

public:
	TKeyExchangeSuite iKeyExchangeSuite;
	TKeyIdType iKeyIdType;
	TBuf8<KWtlsMaxKeyIdLength> iKeyId;
	};

class TWtlsCipherSuite
/**
The class TWtlsCipherSuite encapsulates a WTLS cipher suite as defined in
the WAP WTLS Specification, July 2001
This is a pair made up of a Bulk Encryption Algorithm and a Mac Algorithm
@publishedAll
@released
*/
	{
public:
			
	enum TBulkEncryptionAlgorithm
		{
		ENULL			=0,
		ERC5_CBC_40		=1,
		ERC5_CBC_56		=2,
		ERC5_CBC		=3,
		EDES_CBC_40		=4,
		EDES_CBC		=5,
		E3DES_CBC_EDE	=6,
		EIDEA_CBC_40	=7,
		EIDEA_CBC_56	=8,
		EIDEA_CBC		=9,
		ERC5_CBC_64		=10,
		EIDEA_CBC_64	=11
		};

	enum TMacAlgorithm
		{
		ESHA_0			=0,
		ESHA_40			=1,
		ESHA_80			=2,
		ESHA			=3,
//		N/A (removed)	=4,
		EMD5_40			=5,
		EMD5_80			=6,
		EMD5			=7,
		};

	TWtlsCipherSuite(TBulkEncryptionAlgorithm aBulkEncryptionAlgorithm, TMacAlgorithm aMacAlgorithm);

public:
	TBulkEncryptionAlgorithm iBulkEncryptionAlgorithm;
	TMacAlgorithm iMacAlgorithm;
	};

inline TWtlsCipherSuite::TWtlsCipherSuite(TBulkEncryptionAlgorithm aBulkEncryptionAlgorithm, TMacAlgorithm aMacAlgorithm)
	: iBulkEncryptionAlgorithm(aBulkEncryptionAlgorithm), iMacAlgorithm(aMacAlgorithm)
	{
	}

inline TWtlsKeyExchangeSuite::TWtlsKeyExchangeSuite(TKeyExchangeSuite aKeyExchangeSuite, TKeyIdType aKeyIdType, const TBuf8<KWtlsMaxKeyIdLength>& aKeyId)
	: iKeyExchangeSuite(aKeyExchangeSuite), iKeyIdType(aKeyIdType), iKeyId(aKeyId)
	{
	}


#endif	// __WSPTYPES_H__
