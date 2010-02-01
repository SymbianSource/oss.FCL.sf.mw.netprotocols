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
#include <cdbstore.h>
#include <cdbcols.h>
#include <httpstringconstants.h>
#include <wsperror.h>
#include <wsp/wsptypes.h>

// User includes
#include "wsppanic.h"

// Class signature
#include "cwspproxyinfoprovider.h"

// Constants used in this file
const TInt KPortSecureCO			= 9203;
const TInt KPortInsecureCO			= 9201;
const TInt KPortSecureCL			= 9202;
const TInt KPortInsecureCL			= 9200;

CWspProxyInfoProvider* CWspProxyInfoProvider::NewL(RHTTPSession aSession)
	{
	return new (ELeave) CWspProxyInfoProvider(aSession);
	}

CWspProxyInfoProvider::~CWspProxyInfoProvider()
	{
	// Cleanup the proxy info
	iProxyInfo.Close();
	}

CWspProxyInfoProvider::CWspProxyInfoProvider(RHTTPSession aSession)
: CBase(), iSession(aSession)
	{
	}

void CWspProxyInfoProvider::UpdateProxyInfoL()
	{
	// Has a proxy redirection occured?
	if( iProxyInfoLocked )
		{
		// Yes the current proxy info is the redirected proxy info - don't 
		// change, but reset the redirection flag
		iProxyInfoLocked = EFalse;
		}
	else
		{
		// Get the client specified proxy info
		TProxyInfo proxyInfo = GetProxyInfoL();

		// Cleanup the current proxy info
		iProxyInfo.Close();

		// Set the proxy info to its new value
		iProxyInfo = proxyInfo;
		}
	}
void CWspProxyInfoProvider::SetTemporaryRedirectedProxyL(TWspRedirectedAddress&	aRedirectAddress)
	{
	// Change current proxy info to the redirected one
	ChangeToRedirectedProxyInfoL(aRedirectAddress);
	}

void CWspProxyInfoProvider::SetPermanentRedirectedProxyL(TWspRedirectedAddress& aRedirectAddress)
	{
	// Change current proxy info to the redirected one
	ChangeToRedirectedProxyInfoL(aRedirectAddress);

	// Set HTTP session properties with the redirected proxy info
	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();

	// Set the proxy address
	THTTPHdrVal address = iProxyInfo.iProxyAddress;	
	connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspProxyAddress, RHTTPSession::GetTable()), address);

	// Set the bearer
	THTTPHdrVal bearer;
	if( iProxyInfo.iBearer == EIP )
		{
		bearer = THTTPHdrVal(stringPool.StringF(HTTP::EWspIp, RHTTPSession::GetTable()));

		// Need to set the port as this is an IP bearer
		THTTPHdrVal port = iProxyInfo.iRemotePort;
		connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspRemotePort, RHTTPSession::GetTable()), port);
		}
	else
		{
		__ASSERT_DEBUG( iProxyInfo.iBearer == EWAPSMS, Panic(KWspPanicBadBearerType) );

		// Must be an SMS bearer otherwise would have left before this
		bearer = THTTPHdrVal(stringPool.StringF(HTTP::EWspSMS, RHTTPSession::GetTable()));
		}
	connInfo.SetPropertyL(stringPool.StringF(HTTP::EWspBearer, RHTTPSession::GetTable()), bearer);
	}

TBool CWspProxyInfoProvider::ProxyInfoChangedAndUpdateL()
	{
	// Get the client specified proxy info
	TProxyInfo proxyInfo = GetProxyInfoL();

	// Compare against current proxy info
	TBool hasChanged = proxyInfo != iProxyInfo;

	// Cleanup the current proxy info
	iProxyInfo.Close();

	// Set the proxy info to its new value
	iProxyInfo = proxyInfo;

	// Set the locked flag
	iProxyInfoLocked = ETrue;

	return hasChanged;
	}

void CWspProxyInfoProvider::UnlockProxyInfo()
	{
	// Clear the locked flag
	iProxyInfoLocked = EFalse;
	}

void CWspProxyInfoProvider::ResetProxyInfo()
	{
	// Reset to the state after construction.
	UnlockProxyInfo();
	iProxyInfo.Close();
	}

void CWspProxyInfoProvider::ChangeToRedirectedProxyInfoL(TWspRedirectedAddress& aRedirectAddress)
	{
	// Set the proxy info - first the proxy address, convert the descriptor to an RStringF
	TProxyInfo proxyInfo;
	proxyInfo.iProxyAddress = iSession.StringPool().OpenFStringL(aRedirectAddress.iProxyAddress);
	CleanupStack::PushL(&proxyInfo.iProxyAddress);

	// Check for bearer type
	if( aRedirectAddress.iHasBearer )
		{
		// Use the given bearer
		proxyInfo.iBearer = aRedirectAddress.iBearer;

		if( proxyInfo.iBearer == ESMS )
			{
			// Get the Service centre number from the Comms DB
			ReadServiceCentreNumberL(iSession.StringPool(), proxyInfo.iServiceCentreNumber);
			}
		}
	else
		{
		// Use the current bearer
		proxyInfo.iBearer = iProxyInfo.iBearer;

		if( proxyInfo.iBearer == ESMS )
			{
			// Copy the service centre number
			proxyInfo.iServiceCentreNumber	= iProxyInfo.iServiceCentreNumber.Copy();
			}
		}

	// Clean up - proxyInfo retains ownership of the RStringF
	CleanupStack::Pop(&proxyInfo.iProxyAddress);

	// Check for port number
	if( aRedirectAddress.iHasPort )
		{
		// Use the given bearer
		proxyInfo.iRemotePort = aRedirectAddress.iPort;
		}
	else
		{
		// Use the current bearer
		proxyInfo.iRemotePort = iProxyInfo.iRemotePort;
		}
	// Copy over the other info
	proxyInfo.iSecureConnection		= iProxyInfo.iSecureConnection;
	proxyInfo.iLocalPort			= iProxyInfo.iLocalPort;
	proxyInfo.iSessionType			= iProxyInfo.iSessionType;

	// Set the current proxy info to the new info.
	// Cleanup the current proxy info
	iProxyInfo.Close();

	// Set the proxy info to its new value
	iProxyInfo = proxyInfo;

	// Set the locked flag
	iProxyInfoLocked = ETrue;
	}

TProxyInfo CWspProxyInfoProvider::GetProxyInfoL() const
	{
	// Check the session properties first...
	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();
	TProxyInfo proxyInfo;

	// Need to check to see if the proxy address has been specified in the HTTP
	// session properties, otherwise need to get proxy info from the Comms
	// Database.
	THTTPHdrVal namedProxyValue;
	TBool useNamedProxy = connInfo.Property(stringPool.StringF(HTTP::EWspProxyAddress, RHTTPSession::GetTable()), namedProxyValue);
	if( useNamedProxy )
		{
		// First, get the session service type - make sure it is not CL
		proxyInfo.iSessionType = ExtractSessionTypeFromSession(connInfo, stringPool);
		if( proxyInfo.iSessionType == EWspConnectionLess )
			{
			// Don't support CL at the moment, so leave
			User::Leave(KErrNotSupported);
			}

		// Now get the proxy address...
		__ASSERT_DEBUG((namedProxyValue.Type() == THTTPHdrVal::KStrFVal), Panic(KWspPanicBadProxyAddressProperty));
		proxyInfo.iProxyAddress = namedProxyValue.StrF().Copy();

		// ...the bearer...
		proxyInfo.iBearer = ExtractBearerInfoFromSession(connInfo, stringPool);

		// ...security type...
		proxyInfo.iSecureConnection = ExtractSecurityInfoFromSession(connInfo, stringPool);

		// Get bearer specific stuff
		if( proxyInfo.iBearer == EIP )
			{
			// For IP, need local and remote ports
			proxyInfo.iLocalPort	= ExtractPortInfoFromSession(HTTP::EWspLocalPort, connInfo, stringPool);
			proxyInfo.iRemotePort	= ExtractPortInfoFromSession(HTTP::EWspRemotePort, connInfo, stringPool);

			// Check for correct default value
			if( proxyInfo.iRemotePort == 0 )
				proxyInfo.iRemotePort = DefaultRemotePort(proxyInfo.iSecureConnection, ETrue);
			}
		else
			{
			// For SMS stuff need the Service number - no default, leave if not set
			THTTPHdrVal serviceNumberValue;
			TBool hasServiceNumber = connInfo.Property(stringPool.StringF(HTTP::EWspServiceNumber, RHTTPSession::GetTable()), serviceNumberValue);
			if( hasServiceNumber )
				{
				__ASSERT_DEBUG((serviceNumberValue.Type() == THTTPHdrVal::KStrFVal), Panic(KWspPanicBadServiceNumberProperty));
				proxyInfo.iServiceCentreNumber = serviceNumberValue.StrF().Copy();
				}
			else
				User::Leave(KWspErrServiceNumberNotDefined);
			}
		}
	else
		{
		// Read proxy info from CommDB
		ReadWapAccessPointInfoL(stringPool, proxyInfo.iBearer, 
											proxyInfo.iProxyAddress, 
											proxyInfo.iSecureConnection, 
											proxyInfo.iServiceCentreNumber,
											proxyInfo.iSessionType);

		// Check the bearer
		if( proxyInfo.iBearer == EIP )
			{
			// Initialise the local and remote port values for the IP bearer
			proxyInfo.iLocalPort = 0;
			proxyInfo.iRemotePort = DefaultRemotePort(proxyInfo.iSecureConnection, ETrue);
			}
		}
	return proxyInfo;
	}

/*
 * Methods from MWspProxyInfoProvider
 */

const TDesC8& CWspProxyInfoProvider::ProxyAddress() const
	{
	return iProxyInfo.iProxyAddress.DesC();
	}

TUint16 CWspProxyInfoProvider::RemotePort() const
	{
	return iProxyInfo.iRemotePort;
	}

TUint16 CWspProxyInfoProvider::LocalPort() const
	{
	return iProxyInfo.iLocalPort;
	}

TWspBearer CWspProxyInfoProvider::Bearer() const
	{
	return iProxyInfo.iBearer;
	}

const TDesC8& CWspProxyInfoProvider::ServiceCentreNumber() const
	{
	return iProxyInfo.iServiceCentreNumber.DesC();
	}

TWspSession CWspProxyInfoProvider::WspSessionService() const
	{
	return iProxyInfo.iSessionType;
	}

TBool CWspProxyInfoProvider::SecureConnection() const
	{
	return iProxyInfo.iSecureConnection;
	}

TWspSession CWspProxyInfoProvider::ExtractSessionTypeFromSession(RHTTPConnectionInfo aConnInfo, RStringPool aStringPool)
	{
	// Check for the Wsp Session Service type, or use default value of 
	// EWspConnectionOriented.
	TWspSession session = EWspConnectionOriented;

	THTTPHdrVal sessionType;
	TBool hasSessionType = aConnInfo.Property(aStringPool.StringF(HTTP::EWspSessionType, RHTTPSession::GetTable()), sessionType);
	if( hasSessionType )
		{
		__ASSERT_DEBUG((sessionType.Type() == THTTPHdrVal::KStrFVal), Panic(KWspPanicBadWspSessionTypeProperty));
		switch (sessionType.StrF().Index(RHTTPSession::GetTable()))
			{
		case HTTP::EWspConnectionOriented:
			{
			// Nothing to do as session type already set to default
			} break;
		case HTTP::EWspConnectionLess:
			{
			session = EWspConnectionLess;
			} break;
		default:
			__ASSERT_DEBUG(ETrue, Panic(KWspPanicBadWspSessionTypeProperty));
			}
		}
	return session;
	}

TWspBearer CWspProxyInfoProvider::ExtractBearerInfoFromSession(RHTTPConnectionInfo aConnInfo, RStringPool aStringPool)
	{
	// Check for bearer property, or use default value of EIP
	TWspBearer bearer = EIP;

	THTTPHdrVal bearerValue;
	TBool hasBearer	= aConnInfo.Property(aStringPool.StringF(HTTP::EWspBearer, RHTTPSession::GetTable()), bearerValue);
	if( hasBearer )
		{
		__ASSERT_DEBUG((bearerValue.Type() == THTTPHdrVal::KStrFVal), Panic(KWspPanicBadBearerProperty));
		switch (bearerValue.StrF().Index(RHTTPSession::GetTable()))
			{
		case HTTP::EWspIp:
			{
			// Nothing to do as bearer already set to default
			} break;
		case HTTP::EWspSMS:
			{
			bearer = EWAPSMS;
			}
		default:
			__ASSERT_DEBUG(ETrue, Panic(KWspPanicBadBearerProperty));
			}
		}
	return bearer;
	}

TBool CWspProxyInfoProvider::ExtractSecurityInfoFromSession(RHTTPConnectionInfo aConnInfo, RStringPool aStringPool)
	{
	// Check for security property, or use default value of unsecured
	TBool useWtls = EFalse;

	THTTPHdrVal securityValue;
	TBool hasSecurityValue = aConnInfo.Property(aStringPool.StringF(HTTP::EWspSecurity, RHTTPSession::GetTable()), securityValue);
	if( hasSecurityValue )
		{
		__ASSERT_DEBUG((securityValue.Type() == THTTPHdrVal::KStrFVal), Panic(KWspPanicBadSecurityProperty));
		switch (securityValue.StrF().Index(RHTTPSession::GetTable()))
			{
		case HTTP::EWspUseWtls:
			{
			useWtls = ETrue;
			} break;
		case HTTP::EWspDoNotUseWtls:
			{
			// Nothing to do as security level already set to default
			} break;
		default:
			__ASSERT_DEBUG(ETrue, Panic(KWspPanicBadSecurityProperty));
			}
		}
	return useWtls;
	}

TUint16 CWspProxyInfoProvider::ExtractPortInfoFromSession(TInt aPortType, RHTTPConnectionInfo aConnInfo, RStringPool aStringPool)
	{
	// Check for the specified port property, or use the default value of 0
	TUint16 port = 0;

	THTTPHdrVal portValue;
	TBool hasPort = aConnInfo.Property(aStringPool.StringF(aPortType, RHTTPSession::GetTable()), portValue);
	if( hasPort )
		{
		__ASSERT_DEBUG((portValue.Type() == THTTPHdrVal::KTIntVal), Panic(KWspPanicBadPortProperty));
		port = STATIC_CAST(TUint16, portValue.Int());
		}
	return port;
	}

TUint16 CWspProxyInfoProvider::DefaultRemotePort(TBool aSecureConnection, TBool aCOConnection)
	{
	TUint16 port = 0;
	if( aCOConnection )
		{
		// Defaults for CO session
		if( aSecureConnection )
			port = KPortSecureCO;
		else
			port = KPortInsecureCO;
		}
	else
		{
		// Defaults for CL session
		if( aSecureConnection )
			port = KPortSecureCL;
		else
			port = KPortInsecureCL;
		}
	return port;
	}

void CWspProxyInfoProvider::ReadWapAccessPointInfoL(
												   RStringPool		aStringPool,
												   TWspBearer&		aBearer, 
												   RStringF&		aProxyAddress, 
												   TBool&			aSecureConnection, 
												   RStringF&		aServiceCentreNumber,
												   TWspSession&		aSessionServiceType
												   )
	{
    // Open the Comms DB - don't specify whether it should be a IAP or ISP
	// style DB - this doesn't matter here.
    CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeUnspecified);
    CleanupStack::PushL(db);

    // Find the ID of the WAP Access Point marked in the global settings. This
	// is the 'default' WAP AP, as marked by the user's most recent selection
	// of a bookmark in the WAP browser.
    TUint32 wapAPID;
    db->GetGlobalSettingL(TPtrC(WAP_ACCESS_POINT), wapAPID);

    // Open a view on the WAP AP Table, selecting the WAP Access Point that
	// matches wapAPID. We expect only one record to in the view as we're
	// selecting against a unique ID!
    CCommsDbTableView* apView=
        db->OpenViewMatchingUintLC(TPtrC(WAP_ACCESS_POINT)
                                   , TPtrC(COMMDB_ID), wapAPID);
    if( apView->GotoFirstRecord() != KErrNone )
		User::Leave(KWspErrAccessPointNotSetUp);

	// Read the Bearer name specified in the selected WAP AP record.
	HBufC* bearer16 = HBufC::NewL(KCommsDbSvrMaxFieldLength);
	CleanupStack::PushL(bearer16);
	TPtr bearerPtr16 = bearer16->Des();
    apView->ReadTextL(TPtrC(WAP_CURRENT_BEARER), bearerPtr16);

	// Open a view on the appropriate bearer table according to the bearer
	// name. Use the WAP AP ID to select a record in the appropriate WAP
	// bearer table (either WAP_IP_BEARER or WAP_SMS_BEARER).
	CCommsDbTableView* bearerView = NULL;
	if( bearerPtr16.Compare(TPtrC(WAP_SMS_BEARER)) == 0 ) // SMS (8-bit WAP SMS)
		{
		aBearer = EWAPSMS;
		bearerView = db->OpenViewMatchingUintLC(
								TPtrC(WAP_SMS_BEARER),		// table name
								TPtrC(WAP_ACCESS_POINT_ID),	// column to match
								wapAPID						// value to match
								);
		}
	else if( bearerPtr16.Compare(TPtrC(WAP_IP_BEARER)) == 0 ) // IPv4
		{
		aBearer = EIP;
		bearerView = db->OpenViewMatchingUintLC(
								TPtrC(WAP_IP_BEARER),		// table name
								TPtrC(WAP_ACCESS_POINT_ID),	// column to match
								wapAPID						// value to match
								);
		}
	else
		{
		// Some unknown and unsupported bearer was specified - leave.
		User::Leave(KWspErrAccessPointNotSetUp);
		}

	// Navigate to the first record matching our selection in the view. We
	// expect only one record to in the view as we don't want more than one IP
	// or SMS bearer definition per WAP AP.
	if( bearerView->GotoFirstRecord() != KErrNone )
		User::Leave(KWspErrAccessPointNotSetUp);

	// Read the Secure connection flag from the chosen bearer record.
    bearerView->ReadBoolL(TPtrC(WAP_SECURITY), aSecureConnection);

	// Read the Gateway from the WAP AP settings. It is held as a UNICODE
	// descriptor - convert this to 8-bit before returning
	// NB the Gateway field in CommDB is actually used to hold the SMS Service
	// Number when SMS is used as a bearer. However, the field type is
	// specified separately as an addressing scheme, so read that first and
	// use it for validation.
	HBufC* gateway16 = HBufC::NewL(KCommsDbSvrMaxFieldLength);
	CleanupStack::PushL(gateway16);
	TPtr gatewayPtr16 = gateway16->Des();
    bearerView->ReadTextL(TPtrC(WAP_GATEWAY_ADDRESS), gatewayPtr16);

	// Copy to 8-bits...
	HBufC8* gateway8 = HBufC8::NewL(gatewayPtr16.Length());
	CleanupStack::PushL(gateway8);
	TPtr8 gatewayPtr8 = gateway8->Des();
	gatewayPtr8.Copy(gatewayPtr16);

	if( aBearer == EIP )
		{
		// Set gateway as an IPv4 quadruplet; service number is not being used
		// so clear it.
		aProxyAddress = aStringPool.OpenFStringL(gatewayPtr8);
		aServiceCentreNumber = RStringF();
		}
	else // aBearer == EWapSMS
		{
		// Set gateway as a service number - this is the proxy address
		aProxyAddress = aStringPool.OpenFStringL(gatewayPtr8);

		// Only the SMS bearer has a Service Centre Address

		// Read the Service Centre Address from the WAP AP settings. This is
		// in the form of a phone number when SMS is being used as a bearer.
		// Note: this isn't used by HTTP to configure the WAP stack - it is
		// used by the WAP stack to configure the SMS stack. HTTP doesn't need
		// to worry about it. However, store it anyway, just in case...
		HBufC* svcCentreAddr16 = HBufC::NewL(KCommsDbSvrMaxFieldLength);
		CleanupStack::PushL(svcCentreAddr16);

		TPtr svcCentreAddrPtr16 = svcCentreAddr16->Des();
		bearerView->ReadTextL(TPtrC(WAP_SERVICE_CENTRE_ADDRESS),
														svcCentreAddrPtr16);

		// Copy to 8-bits...
		HBufC8* svcCentreAddr8 = HBufC8::NewL(svcCentreAddrPtr16.Length());
		CleanupStack::PushL(svcCentreAddr8);
		TPtr8 svcCentreAddrPtr8 = svcCentreAddr8->Des();
		svcCentreAddrPtr8.Copy(svcCentreAddrPtr16);

		aServiceCentreNumber = aStringPool.OpenFStringL(svcCentreAddrPtr8);
		CleanupStack::PopAndDestroy(2, svcCentreAddr16);	// svcCentreAddr16, svcCentreAddr8 
		}

	// Read the WSP Option - CO (Connection Oriented) or CL (Connection-less).
	// Note: this WAP browser isn't going to support a connectionless mode of
	// operation. 
	TUint32 wspOptionID = 0;
    bearerView->ReadUintL(TPtrC(WAP_WSP_OPTION), wspOptionID);
	switch( wspOptionID )
		{
	case EWapWspOptionConnectionless: // Connection-less
		{
		aSessionServiceType = EWspConnectionLess;
		} break;
	case EWapWspOptionConnectionOriented: // Connection-oriented
		{
		aSessionServiceType = EWspConnectionOriented;
		} break;
	default: // 'NULL' setting - don't know what this means... leave with
			 // a 'AP Not set up' failure
		User::Leave(KWspErrAccessPointNotSetUp);
		break;
		}
	// All done - clean up
	CleanupStack::PopAndDestroy(6, db); // gateway8, gateway16, bearerView, bearer16,
										// apView, db
    }
void CWspProxyInfoProvider::ReadServiceCentreNumberL(RStringPool aStringPool, RStringF& aServiceCentreNumber)
	{
    // Open the Comms DB - don't specify whether it should be a IAP or ISP
	// style DB - this doesn't matter here.
    CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeUnspecified);
    CleanupStack::PushL(db);

    // Find the ID of the WAP Access Point marked in the global settings. This
	// is the 'default' WAP AP, as marked by the user's most recent selection
	// of a bookmark in the WAP browser.
    TUint32 wapAPID;
    db->GetGlobalSettingL(TPtrC(WAP_ACCESS_POINT), wapAPID);

	// Open a view on the SMS Bearer table. Use the WAP AP ID to select a record
	// in the appropriate WAP bearer table.
	CCommsDbTableView* bearerView = db->OpenViewMatchingUintLC(
										   TPtrC(WAP_SMS_BEARER),		// table name
										   TPtrC(WAP_ACCESS_POINT_ID),	// column to match
										   wapAPID						// value to match
										   );
	if( bearerView == NULL )
		{
		// No SMS bearer table - leave
		User::Leave(KWspErrSMSBearerNotInCommsDB);
		}

	// Read the Service Centre Address from the WAP AP settings. This is
	// in the form of a phone number when SMS is being used as a bearer.
	// Note: this isn't used by HTTP to configure the WAP stack - it is
	// used by the WAP stack to configure the SMS stack. HTTP doesn't need
	// to worry about it. However, store it anyway, just in case...
	HBufC* svcCentreAddr16 = HBufC::NewL(KCommsDbSvrMaxFieldLength);
	CleanupStack::PushL(svcCentreAddr16);

	TPtr svcCentreAddrPtr16 = svcCentreAddr16->Des();
	bearerView->ReadTextL(TPtrC(WAP_SERVICE_CENTRE_ADDRESS), svcCentreAddrPtr16);

	// Copy to 8-bits...
	HBufC8* svcCentreAddr8 = HBufC8::NewL(svcCentreAddrPtr16.Length());
	CleanupStack::PushL(svcCentreAddr8);
	TPtr8 svcCentreAddrPtr8 = svcCentreAddr8->Des();
	svcCentreAddrPtr8.Copy(svcCentreAddrPtr16);

	aServiceCentreNumber = aStringPool.OpenFStringL(svcCentreAddrPtr8);

	// Cleanup...
	CleanupStack::PopAndDestroy(4, db);
	}
