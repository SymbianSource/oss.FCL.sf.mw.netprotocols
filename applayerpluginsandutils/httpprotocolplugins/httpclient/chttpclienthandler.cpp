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

#include "chttpclienthandler.h"

#include <http.h>
#include <http/framework/crxdata.h>
#include <http/framework/csecuritypolicy.h>
#include <inetprottextutils.h>
#include <uriutilscommon.h>
#include <uriutils.h>
#include <authority8.h>
#include <in_sock.h>
#include <securesocket.h>
#include <x509certext.h>

#include "chttptransportlayer.h"
#include "chttpclientfilter.h"
#include "chttpconnectfilter.h"
#include "chttpclientheadercodec.h"
#include "chttpclienttransaction.h"
#include "chttpconnectioninfo.h"
#include "chttpconnectionmanager.h"
#include "chttprequestcomposer.h"
#include "chttpresponseparser.h"
#include "thttpclientpanic.h"

const TInt KHttpDefaultPort			= 80;
const TInt KHttpDefaultSecurePort	= 443;
const TInt KHttpDefaultProxyPort	= 8080;
const TInt KMaxConnectionManagers	= 4;
const TInt KDefaultBatchingBufSize	= 1400;
const TInt KDefaultMaxNumberTransactionsToPipeline = KMaxTInt;
const TInt KDefaultBufferSize		= 32*1024;


_LIT8(KSecureHttpScheme, "https");
_LIT8(KHttpClientCodecName, "HTTP/client");

_LIT8(KUAProfile, "x-wap-profile");

const TUint KIPv6HostOpenBrace		= '[';
const TUint KIPv6HostCloseBrace		= ']';
class InetProtTextUtils;

CHttpClientHandler* CHttpClientHandler::NewL(TAny* aSession)
	{
	RHTTPSession* httpSession = REINTERPRET_CAST(RHTTPSession*, aSession);
	CHttpClientHandler* self = new (ELeave) CHttpClientHandler(*httpSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CHttpClientHandler::~CHttpClientHandler()
	{
	iProxyAddress.Close();
	iConnectionManagers.ResetAndDestroy();
	delete iTransportLayer;
	delete iPipelineFallback;
	__FLOG_CLOSE;
	}
	
CHttpClientHandler::CHttpClientHandler(RHTTPSession aSession)
: CProtocolHandler(aSession),
iStringTable(RHTTPSession::GetTable())
	{
	__FLOG_OPEN("http", "httpclienthandler.txt");
	__FLOG(_T8("HTTP Client Protocol Handler Log"));
	}
	
void CHttpClientHandler::ConstructL()
	{	
	CProtocolHandler::ConstructL(iSession);
	
	CHttpClientFilter* clientFilter = CHttpClientFilter::NewL(iSession);
	CleanupStack::PushL ( clientFilter );
	CHttpConnectFilter::NewL(iSession);
	CleanupStack::Pop (); // clientFilter

	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();
	THTTPHdrVal value;
	TBool hasValue = connInfo.Property(stringPool.StringF(HTTP::ESessionClosing, iStringTable), value);
	if( hasValue && value.Type() == THTTPHdrVal::KTIntVal )  
		{
		iSessionClosingPtr = reinterpret_cast<TBool*>(value.Int());
		}
	else
		{
		__FLOG_0(_T8("!! Session Closing Flag NOT set"));
		User::Leave(KErrNotFound);
		}
	iPipelineFallback = CHttpPipelineFallback::NewL();	
	}

CHttpConnectionInfo* CHttpClientHandler::PrepareTransactionL(RHTTPTransaction aTrans, TBool& aNeedTunnel, TBool& aCanPipeline)
	{
    // Check that filters are not added headers that are not needed for CONNECT method.
    // Remove the unwanted headers. But do only if the property has been set for the 
    // strict CONNECT method.
    EnsureStrictConnectMethodHeaders (aTrans);
	
	// To prepare the transaction need to create the Request-URI, set the Host
	// header and establish the connection info.
	RStringPool stringPool = iSession.StringPool();
	RHTTPRequest request = aTrans.Request();

	SetupProxyInformation(aTrans);
	
	// Store the method index from string table
	TInt methodIndex = request.Method().Index(iStringTable); 
	// Check for https scheme.
	const TUriC8& uri = request.URI();
	// secure doesn't count if it's a connect request	
	TBool secure = methodIndex != HTTP::ECONNECT &&
		(uri.Extract(EUriScheme).CompareF(KSecureHttpScheme) == 0);
	// Create the Request-URI for this transaction...
	TInt port = (secure) ? KHttpDefaultSecurePort : KHttpDefaultPort;
	TPtrC8 host;
	CreateRequestUriL(methodIndex, aTrans, host, port);

	// If the host name is empty leave as the URI is not complete - a CONNECT
	// method request has an empty host as it will go to the proxy.
	if( host.Length() == 0 )
		{
		__FLOG_0(_T8("!! Invalid uri"));
		__FLOG_2(_T8("-> Trans %d, Con %d : missing host!"), aTrans.Id(),GetConnectionID(aTrans));

		User::Leave(KErrHttpInvalidUri);
		}

	// Set the Host header...
	TBool isHttp10 = SetHostHeaderL(aTrans, host, port);

	// Create the connection info. Is a proxy being used?
	if( iUseProxy )
		{
		// Need a tunnel if request is going via a proxy and request is using 
		// the https scheme
		if( secure )
			{
			__FLOG_4(_T8("Trans %d, Con %d : tunnel required to host %S, port %d"), aTrans.Id(),GetConnectionID(aTrans), &host, port);

			// This request needs a tunnel to be established to the specified 
			// host/port - add the ETunnel property to the transaction.
			aNeedTunnel = ETrue;
			AddTunnelInfoL(aTrans, host, port);
			}

		// Need to obtain the proxy info.
		const TDesC8& proxy = iProxyAddress.DesC();
		TAuthorityParser8 auth;
		auth.Parse(proxy);

		host.Set(auth.Extract(EAuthorityHost));

		// Read the proxy port if present
		port = KHttpDefaultProxyPort; // Initialise with default proxy port
		if( auth.IsPresent(EAuthorityPort) )
			{
			TPtrC8 portDesc = auth.Extract(EAuthorityPort);
			TInt error = InetProtTextUtils::ConvertDescriptorToInt(portDesc, port);
			if( error != KUriUtilsErrEmptyData )
				{
				// An empty port component is allowed - just ignore it.
				User::LeaveIfError(error);
				}
			}
		}
		
	// Is a non-persistent connection required? The connection is non-persistent
	// if either it is a HTTP/1.0 request or it is a HTTP/1.1 request and it has
	// a Connection: close header.
	RHTTPHeaders headers = request.GetHeaderCollection();
	CHttpConnectionInfo::THttpPersistent persistentState = isHttp10 ? 
														   CHttpConnectionInfo::ENonPersistent : 
														   CHttpConnectionInfo::EPersistent;
	THTTPHdrVal connVal;
	RStringF connStr = stringPool.StringF(HTTP::EConnection, iStringTable);
	if( headers.GetField(connStr,0,connVal) == KErrNone )
		{
		__ASSERT_DEBUG( !isHttp10, THttpClientPanic::Panic(THttpClientPanic::EInvalidHeaderForHTTP10) ); // no connection headers are allowed in HTTP/1.0
		
		if( (connVal.Type() == THTTPHdrVal::KStrFVal) && 
			(connVal.StrF() == stringPool.StringF(HTTP::EClose, iStringTable)) )
			{
			// Client has specified a Connection: close header - non-persistent
			// connection.
			persistentState = CHttpConnectionInfo::ENonPersistent;
			}
		}
	
	// Create the connection info object
	CHttpConnectionInfo* connectionInfo = CHttpConnectionInfo::NewL(stringPool, host, static_cast<TUint16>(port));
	connectionInfo->SetSecureState(secure);
	connectionInfo->SetPersistentState(persistentState);

	// Check to see if this transaction can be pipelined.
	switch(methodIndex)
		{
	case HTTP::EGET:
		{
		// Can only pipeline non-HTTP/1.0 requests...
		if( !isHttp10 )
			{
			// These methods can be pipelined but only if pipelining has not been 
			// specifically disabled for the transaction.
            aCanPipeline = (CheckPipelineSupport(aTrans) && !iPipelineFallback->NeedPipelineFallback(host));
            __FLOG_4(_T8("Pipelining --- enabled: %d Support: %d Fallback: %d  Host %S"), aCanPipeline, CheckPipelineSupport(aTrans), !iPipelineFallback->NeedPipelineFallback(host), &host);
			break;
			}
		// Allow the HTTP/1.0 requests to drop through to default case.
		}
	default:
		// All other methods are not to be pipelined.
		aCanPipeline = EFalse;
		break;
		};

	return connectionInfo;
	} 

void CHttpClientHandler::CreateRequestUriL(TInt aMethodIndex, RHTTPTransaction aTrans, TPtrC8& aHost, TInt& aPort)
	{
	RStringPool stringPool = iSession.StringPool();
	RHTTPRequest request = aTrans.Request();
	const TUriC8& uri = request.URI();
	TBool secure = EFalse;
	
	RStringF scheme = stringPool.OpenFStringL(uri.Extract(EUriScheme));
	CleanupClosePushL(scheme);
	if( aMethodIndex == HTTP::ECONNECT )
		{
		__ASSERT_DEBUG( iUseProxy, User::Invariant() );
		aHost.Set(uri.Extract(EUriHost));
		}
	else if( uri.IsPresent(EUriHost) )
		{
		// The request URI is absolute - check the scheme is http or https
		secure = uri.IsPresent(EUriScheme) && (scheme.Index(iStringTable) == HTTP::EHTTPS);
		if( !uri.IsPresent(EUriScheme) || !secure &&
			scheme.Index(iStringTable) != HTTP::EHTTP)
			{
			__FLOG_0(_T8("!! Invalid uri."));
			__FLOG_3(_T8("-> Trans %d, Con %d: %S scheme is not supported!"), aTrans.Id(),GetConnectionID(aTrans), &scheme.DesC());

			User::Leave(KErrHttpInvalidUri);
			}

		// Get the host and port info from it.
		aHost.Set(uri.Extract(EUriHost));

		// Check to see if a port has been specified	
		if( uri.IsPresent(EUriPort) )
			{
			TPtrC8 port = uri.Extract(EUriPort);
			TInt error = InetProtTextUtils::ConvertDescriptorToInt(port, aPort);
			if( error != KUriUtilsErrEmptyData )
				{
				// An empty port component is allowed - just ignore it.
				User::LeaveIfError(error);
				}
			}
		}
	else
		{
		// The request URI is relative - update the request URI to be absolute.
		// The client MUST have supplied a Host header.
		RHTTPHeaders headers = request.GetHeaderCollection();
		RStringF hostStr = stringPool.StringF(HTTP::EHost, iStringTable);
		THTTPHdrVal hostVal;

		if( headers.GetField(hostStr, 0, hostVal) == KErrNotFound )
			{
			// No Host header - do not know which host to connect to.
			User::Leave(KErrHttpGeneralHeaderMissingHost);
			}

		__ASSERT_DEBUG( hostVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );

		// Set the host output argument
		aHost.Set(hostVal.StrF().DesC());

		// Create the absolute uri
		CUri8* absoluteUri = CUri8::NewLC(uri);

		RStringF httpStr = stringPool.StringF(HTTP::EHTTP, iStringTable);
		absoluteUri->SetComponentL(httpStr.DesC(), EUriScheme);
		absoluteUri->SetComponentL(aHost, EUriHost);

		// Check for port info in the Host header
		RStringF portStr = stringPool.StringF(HTTP::EPort, iStringTable);
		THTTPHdrVal portVal;
		TBool hasPort = (headers.GetParam(hostStr, portStr, portVal) == KErrNone);
		if( hasPort && portVal.Type() == THTTPHdrVal::KTIntVal )
			{
			// Set the port output argument
			aPort = portVal.Int();

			// Convert the int value to its descriptor format
			HBufC8* portBuf = NULL;
			InetProtTextUtils::ConvertIntToDescriptorL(aPort, portBuf);
			CleanupStack::PushL(portBuf);

			absoluteUri->SetComponentL(*portBuf, EUriPort);
			CleanupStack::PopAndDestroy(portBuf);
			}

		// Now set the absolute URI back into the transaction
		request.SetURIL(absoluteUri->Uri());
		CleanupStack::PopAndDestroy(absoluteUri);
		}

	// Create the Request-URI based on the request URI. 
	// NOTE - might have changed from earlier

	const TUriC8 reqURI = request.URI();
	// CONNECT uses authority while everything else uses absoluteURI or absolutePath
	// use absolutePath for direct connections and tunnels, and absoluteURI for proxy
	// note: OPTIONS uses "*" as well, but that should still work
	CAuthority8* authToUse=NULL;
	CUri8* uriToUse = NULL;
	if( aMethodIndex == HTTP::ECONNECT)
		{
		authToUse = CAuthority8::NewLC();
		authToUse->SetComponentL(reqURI.Extract(EUriUserinfo),EAuthorityUserinfo);
		authToUse->SetComponentL(reqURI.Extract(EUriHost),EAuthorityHost);
		authToUse->SetComponentL(reqURI.Extract(EUriPort),EAuthorityPort);	
		}
	else 
		{
		uriToUse = CUri8::NewLC(reqURI);
		if( !iUseProxy || secure ) // if (useproxy and not secure) or (not useproxy) = not useproxy or secure
			{// Not going via a proxy - need to remove the scheme and authority parts
			uriToUse->RemoveComponentL(EUriScheme);
			uriToUse->RemoveComponentL(EUriHost);	// this also removes the userinfo + port
			}
		}
	// Set the Request-URI for the request
	RString uriStr = stringPool.OpenStringL(
		authToUse ? authToUse->Authority().AuthorityDes() : uriToUse->Uri().UriDes());
	CleanupClosePushL(uriStr);
	THTTPHdrVal uriVal(uriStr);
	aTrans.PropertySet().SetPropertyL(stringPool.StringF(HTTP::EUri, iStringTable), uriVal);
	CleanupStack::PopAndDestroy(3, &scheme); // uriToUse or authToUse, uriStr 
	}

TBool CHttpClientHandler::SetHostHeaderL(RHTTPTransaction aTrans, const TDesC8& aHost, TInt aPort)
	{
		__START_PERFORMANCE_LOGGER();
	// Set the Host header only if the request is not an HTTP/1.0 request.
	RStringPool stringPool = iSession.StringPool();
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	THTTPHdrVal httpVersion;
	TBool isHttp10 = EFalse;
	if( connInfo.Property(stringPool.StringF(HTTP::EHTTPVersion,iStringTable), httpVersion) )
		{
		__ASSERT_DEBUG( httpVersion.Type() == THTTPHdrVal::KStrFVal, THttpClientPanic::Panic(THttpClientPanic::EInvalidHeaderValueType) );
		isHttp10 = (httpVersion.StrF() == stringPool.StringF(HTTP::EHttp10, iStringTable));
		}

	// Regardless of proxy settings a Host header based on the URI authority must be added unless
	// this is an HTTP/1.0 request in which case a Host header should not exist.
	RHTTPRequest request = aTrans.Request();
	RHTTPHeaders headers = request.GetHeaderCollection();
	RStringF hostStr = stringPool.StringF(HTTP::EHost, iStringTable);
	if( !isHttp10 )
		{
		// If the Host header does not already exist, add it!
		THTTPHdrVal hostValue;
		if( headers.GetField(hostStr, 0, hostValue) == KErrNotFound )
			{				
			// Set the Host header...
			RStringF hostValStr;

			// Check if its a literal IPV6 address
			UriUtils::TUriHostType aHostsType = UriUtils::HostType( aHost );
			if ( ( aHostsType != UriUtils::ETextHost ) && ( aHostsType != UriUtils::EIPv4Host ) ) // is an IPv6 or other future protocol address
				{

				HBufC8* ipv6LiteralHost = HBufC8::NewLC( aHost.Length() + 2 ); // add 2 for the braces
			
				TPtr8 ipv6LiteralHostPtr = ipv6LiteralHost->Des();	
				
				ipv6LiteralHostPtr.Append( KIPv6HostOpenBrace );
				ipv6LiteralHostPtr.Append( aHost );
				ipv6LiteralHostPtr.Append( KIPv6HostCloseBrace );

				hostValStr = stringPool.OpenFStringL( ipv6LiteralHostPtr );
				CleanupStack::PopAndDestroy( ipv6LiteralHost );
				
				}
			else
				{
				hostValStr = stringPool.OpenFStringL( aHost );
				}
			
			CleanupClosePushL(hostValStr);

	
			THTTPHdrVal hostVal(hostValStr);	
			headers.SetFieldL(hostStr, hostVal);
			CleanupStack::PopAndDestroy(&hostValStr);
			
			// Also set the port number if Host header is not empty and a port 
			// number is not the default.
			if( aPort != KHttpDefaultPort && aPort != KHttpDefaultSecurePort )
				{
				THTTPHdrVal portVal(aPort);
				RStringF portStr = stringPool.StringF(HTTP::EPort,iStringTable);
				headers.SetParamL(hostStr, portStr, portVal, 0);
				}
			}
		// else the Host header already exists, so do nothing
		}
	else // This is an HTTP/1.0 request
		headers.RemoveField(hostStr);
		
	__END_PERFORMANCE_LOGGER(_L(",CHttpClientHandler::SetHostHeaderL()"));	
	return isHttp10;
	}

void CHttpClientHandler::AddTunnelInfoL(RHTTPTransaction aTrans, const TDesC8& aHost, TInt aPort)
	{
	// Convert the port number into a descriptor...
	HBufC8* port = NULL;
	InetProtTextUtils::ConvertIntToDescriptorL(aPort, port);
	CleanupStack::PushL(port);

	// Create the Request-URI for the CONNECT request - it is in the 'authority'
	// form, as described in RFC2616, section 5.1.2.
	CAuthority8* authority = CAuthority8::NewLC();
	
	authority->SetComponentL(aHost, EAuthorityHost);
	authority->SetComponentL(*port, EAuthorityPort);

	// Set this as the ETunnel property in the transaction.
	RStringPool stringPool = iSession.StringPool();
	RStringF tunnelStr = stringPool.OpenFStringL(authority->Authority().AuthorityDes());
	CleanupClosePushL(tunnelStr);
	THTTPHdrVal tunnelVal(tunnelStr);
	aTrans.PropertySet().SetPropertyL(stringPool.StringF(HTTP::ETunnel, iStringTable), tunnelVal);
	CleanupStack::PopAndDestroy(3, port); // delete authority and close tunnelStr
	}

TBool CHttpClientHandler::SelectConnectionManagerL(const CHttpConnectionInfo& aConnectionInfo, RHTTPTransaction aTrans, TBool aCanPipeline, CHttpConnectionManager *&aManager )
	{
	// Selecting a connection manager depends on whether the transaction can be
	// pipelined. If so, then the connection manager of choice would be the one
	// that is connected to the correct location and busy with other transactions.
	// If there is no connection manager that fits this criteria, then the 
	// selection process follows that for a transaction that cannot be pipelined.

	// The order of preference for selecting a connection manager - 
	// 1) A manager that is connected to the correct location and is available.
	//	  This is the backup-choice for a transaction that can be pipelined.
	// 2) A manager that is not connected to anywhere.
	// 3) Create a new manager if the limit has not been reached.
	// 4) Use a manager that is connected to a different host (but not being 
	//    used) and will therefore need disconnecting and reconnecting.
	
	CHttpConnectionManager* backupChoice = NULL;
	CHttpConnectionManager* secondChoice = NULL;
	CHttpConnectionManager* fourthChoice = NULL;
    CHttpConnectionManager* managerConnecting = NULL;	
	TBool newConnection = ETrue;
    TInt numConnectionsToSingleServer = 0;
    
	TBool connectMethod = aTrans.Request().Method().Index(iStringTable) == HTTP::ECONNECT;
	
	const TInt numConnMan = iConnectionManagers.Count();

	for( TInt ii=0; (ii < numConnMan && aManager == NULL ); ++ii )
		{
		CHttpConnectionManager::TConnectionStatus status = iConnectionManagers[ii]->Status();
		switch( status )
			{
		case CHttpConnectionManager::ENotConnected:
			{
			// This is a pretty good option because it just needs a connection 
			// to be established
			secondChoice = iConnectionManagers[ii];
			} break;
		case CHttpConnectionManager::EConnectedAndAvailable:
			{
			// This is the ideal situation if the location matches, otherwise it
			// becomes the fourth choice
			const CHttpConnectionInfo& connectionInfo = iConnectionManagers[ii]->ConnectionInfo();
			if( connectionInfo.HostAndPortMatches(aConnectionInfo) )
				{
				if(!connectMethod)
				     {
	                ++numConnectionsToSingleServer;					
				     if( aCanPipeline )
					     {
						  // This is the backup-choice
						  backupChoice = iConnectionManagers[ii];
						 }
				     else
					    {
						// This is the one!
						aManager = iConnectionManagers[ii];
						newConnection = EFalse;
					    }
				     }
				 }
			else
				{
				// Non-matching connection info - fourth choice.
				fourthChoice = iConnectionManagers[ii];
				}
			} break;
		case CHttpConnectionManager::EConnectedAndBusy:
			{
			// This is the ideal choice if the pipelining can done and the 
			// location and secure status match.
			const CHttpConnectionInfo& connectionInfo = iConnectionManagers[ii]->ConnectionInfo();
            TBool hostAndPortMatches = connectionInfo.HostAndPortMatches(aConnectionInfo);
            if(hostAndPortMatches)
                {
                ++numConnectionsToSingleServer;
                }
			if( !connectMethod && aCanPipeline && 
			    hostAndPortMatches &&
				connectionInfo.IsSecure() == aConnectionInfo.IsSecure() )
				{
				// This is the one!
				aManager = iConnectionManagers[ii];
				newConnection = EFalse;
				}
			} break;
		case CHttpConnectionManager::EConnectedNotAvailable:
			{
			// Do nothing - continue the search...
			} break;
	     case CHttpConnectionManager::EConnectingNotAvailable:
	         {
	         const CHttpConnectionInfo& connectionInfo = iConnectionManagers[ii]->ConnectionInfo();
             TBool hostAndPortMatches = connectionInfo.HostAndPortMatches(aConnectionInfo);
	         if(hostAndPortMatches)
	             {
	             ++numConnectionsToSingleServer;
	             }
             if (aCanPipeline &&  hostAndPortMatches && connectionInfo.IsSecure() == aConnectionInfo.IsSecure())
	           {
	           // We are connecting. So do not initiate another connection.
	           // We will be able to send the request via the same connection.
	           managerConnecting = iConnectionManagers[ii];
	           }
	         }
	         break;			
		default:
			// There are no other transport handler states - should not reach here
			User::Invariant();
			break;
			}
		}

	if( aManager == NULL)
		{
		if( aCanPipeline && backupChoice != NULL )
			{
			// Use the connection manager that is connected to the correct 
			// location	but not busy.
			aManager = backupChoice;
			newConnection = EFalse;
			}
		else if( secondChoice != NULL )	
			{
			// Use the second choice connection manager - idle one.
			aManager = secondChoice;
			}
		else if( numConnMan < MaxNumConnectionManagers() )
			{
			// Have not reached the max number of connection managers and so 
			// can create a new connection manager - check for a transport layer
			if( iTransportLayer == NULL )
				{
				// Create the transport layer
				_LIT8(KTcpProtocol, "TCP");
				THttpTransportConstructionParams params = THttpTransportConstructionParams(*this);
				
				RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
				RStringPool stringPool = iSession.StringPool();
				THTTPHdrVal valPriority;
				RStringF strConnMan = stringPool.StringF(HTTP::ETranspHndlrPriority , iStringTable);
				params.iPriority = EFalse;
				if( connInfo.Property(strConnMan, valPriority) )
					{
					if(valPriority== stringPool.StringF(HTTP::EEnableTranspHndlrPriority , iStringTable))
						params.iPriority = ETrue;
					}				
				
				iTransportLayer = CHttpTransportLayer::NewL(KTcpProtocol, params);
				}
			// Check we are doing an optimal pipelining. Read the property value only
			// if we haven't created atleast one connection manager yet
			if(numConnMan == 0)
			    {			  
                RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
                RStringPool stringPool = iSession.StringPool();
			    __ASSERT_DEBUG(!managerConnecting, User::Invariant());
			    THTTPHdrVal optimalPipelineValue;
			    RStringF strOptimalPipeline = stringPool.StringF(HTTP::EHttpOptimalPipelining, iStringTable);
			    if(connInfo.Property(strOptimalPipeline, optimalPipelineValue))
			        {
			        iEnableOptimalPipeline = (optimalPipelineValue == stringPool.StringF(HTTP::EHttpEnableOptimalPipelining, iStringTable)); 
			        }
			    }
			if(managerConnecting)
			    {
                newConnection = EFalse;			    
			    }
			else
			    {			    
	            // Create the new connection manager
	            const TInt maxNumberTransactionsToPipeline = MaxNumTransactionsToPipeline(); 
	            aManager = CHttpConnectionManager::NewL(iTransportLayer->SocketFactory(), *this, *iPipelineFallback, maxNumberTransactionsToPipeline, iEnableOptimalPipeline);
	            CleanupStack::PushL(aManager);

			__RecordConnectionManagerCreationL();


#if defined (_DEBUG) && defined (_LOGGING)
	            aManager->__logger__ = this->__logger__;
#endif

	            // Append to the store
	            User::LeaveIfError(iConnectionManagers.Append(aManager));
	            CleanupStack::Pop(aManager);			    
			    }
			}
		else if( fourthChoice != NULL )	
			{
			// As a last resort reuse one that is connected to the wrong host but 
			// not being used.
			aManager = fourthChoice;
			}
		}
	return newConnection;
	}

CHttpConnectionManager* CHttpClientHandler::SelectTunnelConnectionL(const CHttpConnectionInfo& aConnectionInfo, RHTTPTransaction aTrans, TBool aCanPipeline)
	{
	// Look for connection manager that is a tunnel connection via appropriate
	// proxy to appropriate host. If the transaction can be pipelined, then the
	// first choice is a connection manager that is connected and busy. 
	RStringPool stringPool = iSession.StringPool();
	THTTPHdrVal hostVal;
#ifdef _DEBUG
	TBool found = 
#endif
	aTrans.PropertySet().Property(stringPool.StringF(HTTP::ETunnel, iStringTable), hostVal);

	__ASSERT_DEBUG( found, User::Invariant() );
	__ASSERT_DEBUG( hostVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );

	RStringF host = hostVal.StrF();
	TBool notifyCreateTunnel = ETrue;	
	CHttpConnectionManager* manager = NULL;
	CHttpConnectionManager* backupChoice = NULL;
	
	const TInt numConnMan = iConnectionManagers.Count();

	for( TInt ii=0; (ii < numConnMan && manager == NULL ); ++ii )
		{
		CHttpConnectionManager::TConnectionStatus status = iConnectionManagers[ii]->Status();
		switch( status )
			{
		case CHttpConnectionManager::ENotConnected:
		case CHttpConnectionManager::EConnectedNotAvailable:
		case CHttpConnectionManager::EConnectingNotAvailable:
			{
			// Do nothing - continue search.
			} break;
		case CHttpConnectionManager::EConnectedAndAvailable:
			{
			const CHttpConnectionInfo& connectionInfo = iConnectionManagers[ii]->ConnectionInfo();
			if( connectionInfo.HostAndPortMatches(aConnectionInfo) && 
				iConnectionManagers[ii]->TunnelMatches(host) )
				{
				// Location (ie the proxy) and tunnel host match - this is the 
				// backup choice if pipelining allowed, otherwise it is the one.
				if( aCanPipeline )
					{
					// This is the backup choice...
					backupChoice = iConnectionManagers[ii];
					}
				else
					{
					// This is the one! There is no need to create a tunnel.
					manager = iConnectionManagers[ii];
					notifyCreateTunnel = EFalse;
					}
				}
			} break;
		case CHttpConnectionManager::EConnectedAndBusy:
			{
			const CHttpConnectionInfo& connectionInfo = iConnectionManagers[ii]->ConnectionInfo();
			if( aCanPipeline &&
				connectionInfo.HostAndPortMatches(aConnectionInfo) && 
				iConnectionManagers[ii]->TunnelMatches(host) )
				{
				// Location (ie the proxy) and tunnel host match and pipelining
				// is allowed - this is the one! There is no need to create a 
				// tunnel.
				manager = iConnectionManagers[ii];
				notifyCreateTunnel = EFalse;
				}
			} break;
		default:
			// There are no other transport handler states - should not reach here
			User::Invariant();
			break;
			}
		}

	if( manager == NULL && backupChoice != NULL )
		{
		// Use the back-up choice - now no need to create a tunnel.
		manager = backupChoice;
		notifyCreateTunnel = EFalse;
		}
#if defined (_DEBUG) && defined (_LOGGING)
	if( manager != NULL )
		{
		__FLOG_1(_T8("!! Tunnel to %S available"), &host.DesC());
		__FLOG_6(
				_T8("-> Trans %d, Con %d : can service via host %S, remote port %d (secure : %d, nonpersistent : %d)"),
				aTrans.Id(),
				GetConnectionID(aTrans),
				&manager->ConnectionInfo().Host(), 
				manager->ConnectionInfo().Port(), 
				manager->ConnectionInfo().IsSecure(), 
				manager->ConnectionInfo().IsNonPersistent()						
				);
		}
#endif
	if( notifyCreateTunnel )
		{
		__FLOG_1(_T8("!! No tunnel to %S"), &host.DesC());
		__FLOG_2(_T8("-> Trans %d, Con %d : cannot service until tunnel established"), aTrans.Id(),GetConnectionID(aTrans));

		// Notify client (or filter) that a tunnel needs to be established
		// before this transaction can be serviced.
		aTrans.SendEventL(
						 THTTPEvent::ENeedTunnel, 
						 THTTPEvent::EIncoming,
						 THTTPFilterHandle(THTTPFilterHandle::EProtocolHandler)
						 );
		}
	return manager;
	}


/*
*   The below function selects correct NTLM connection manger. Connection manager is associate with a socket.
*   NTLM protocol needs all authentication messages Negotiate(->),Challenge(<-) and Authorise(->) packets
*   to be sent on the same port.  In this function, the connection manager is identified by NTLM id which is set
*   when CHttpResponseParser when 401 unauthorised message is received from ntlm enabled server.
*   The same iNtlmConnId will be maintained until authentication is successful. 
*/
CHttpConnectionManager* CHttpClientHandler::SelectNtlmConnectionL(const CHttpConnectionInfo& aConnectionInfo,RHTTPTransaction aTrans)
    {
    
    TInt ntlmConnId;
    CHttpConnectionManager* fourthChoice=NULL;
    _LIT8( KNtlmId, "NTLMConnId" );
    RStringPool stringPool = aTrans.Session().StringPool();
    RStringF ntlmId = stringPool.OpenFStringL( KNtlmId );
    THTTPHdrVal value;
    
    if (aTrans.PropertySet().Property(ntlmId,value))
        {
        ntlmConnId = value.Int();
        for (TInt i=0;i<iConnectionManagers.Count();i++)
            {
            const CHttpConnectionInfo& connectionInfo = iConnectionManagers[i]->ConnectionInfo();
            if( connectionInfo.HostAndPortMatches(aConnectionInfo) )
                {
                if (iConnectionManagers[i]->GetNtlmConnId() ==  ntlmConnId)
                    {
                    ntlmId.Close();
                    return iConnectionManagers[i];
                    }
                }
            else
                {
                fourthChoice = iConnectionManagers[i];
                }
            }
        }
    ntlmId.Close();
    return fourthChoice;
    }


TInt CHttpClientHandler::MaxNumConnectionManagers() const
	{
	// Has this value been previously cached?
	if( iMaxNumConnectionManagers == 0 )
		{
		// No. Use this default should the property not be set
		iMaxNumConnectionManagers = KMaxConnectionManagers;

		// Check session properties
		RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
		RStringPool stringPool = iSession.StringPool();
		THTTPHdrVal maxConnMan;
		RStringF strConnMan = stringPool.StringF(HTTP::EMaxNumTransportHandlers, iStringTable);
		if( connInfo.Property(strConnMan, maxConnMan) )
			{
			if( maxConnMan.Type() == THTTPHdrVal::KTIntVal )
				iMaxNumConnectionManagers = maxConnMan.Int();
			}
		}
	return iMaxNumConnectionManagers;
	}
	
void CHttpClientHandler::SetupProxyInformation(RHTTPTransaction aTrans)
	{
	// Assume a direct connection unless the properties specifically indicate 
	// that a proxy should be used.
	iUseProxy = EFalse;
	iProxyAddress.Close();
	
	RStringPool stringPool(iSession.StringPool());
	THTTPHdrVal useProxy;
	THTTPHdrVal address;
	RStringF proxyUsage = stringPool.StringF(HTTP::EProxyUsage, iStringTable);
	RStringF proxyAddress = stringPool.StringF(HTTP::EProxyAddress, iStringTable);

	RHTTPPropertySet transactionProperties = aTrans.PropertySet();
	RHTTPPropertySet sessionProperties = iSession.ConnectionInfo();

	// First check the transaction properties for proxy info. If the transaction
	// has its own proxy info set then this should be used, including the fact 
	// that a proxy should not be used. Otherwise check the session properties 
	// for proxy info.
	if( transactionProperties.Property(proxyUsage, useProxy) )
		{
		__ASSERT_DEBUG( useProxy.Type() == THTTPHdrVal::KStrFVal, THttpClientPanic::Panic(THttpClientPanic::EInvalidProxySetting) );

		// The transaction has proxy info set...
		iUseProxy = (useProxy.StrF().Index(iStringTable) == HTTP::EUseProxy);
	
		if( iUseProxy )
			{
			if( transactionProperties.Property(proxyAddress, address) )
				{
				__ASSERT_DEBUG( address.Type() == THTTPHdrVal::KStrFVal, THttpClientPanic::Panic(THttpClientPanic::EInvalidProxySetting) );

				iProxyAddress = address.StrF().Copy();
				}
			else
				{
				// It is invalid to specify using a proxy and not set a proxy
				// address!
				THttpClientPanic::Panic(THttpClientPanic::EInvalidProxySetting);
				}
			}	
		}
	else if( sessionProperties.Property(proxyUsage, useProxy) )
		{
		__ASSERT_DEBUG( useProxy.Type() == THTTPHdrVal::KStrFVal, THttpClientPanic::Panic(THttpClientPanic::EInvalidProxySetting) );

		// The session has proxy info set...
		iUseProxy = (useProxy.StrF().Index(iStringTable) == HTTP::EUseProxy);
		
		if( iUseProxy )
			{
			if( sessionProperties.Property(proxyAddress, address) )
				{
				__ASSERT_DEBUG( address.Type() == THTTPHdrVal::KStrFVal, THttpClientPanic::Panic(THttpClientPanic::EInvalidProxySetting) );
				
				iProxyAddress = address.StrF().Copy();
				}
			else
				{
				// It is invalid to specify using a proxy and not set a proxy
				// address!
				THttpClientPanic::Panic(THttpClientPanic::EInvalidProxySetting);
				}
			}
		}
	}

TBool CHttpClientHandler::CheckPipelineSupport(RHTTPTransaction aTrans)
	{
	// Check to see if the pipelining support has been disabled/enabled for this
	// transaction.
	RStringPool stringPool = iSession.StringPool();
	RStringF pipeline = stringPool.StringF(HTTP::EHttpPipelining, iStringTable);
	THTTPHdrVal value;
	TBool canPipeline = ETrue;

	if(	aTrans.PropertySet().Property(pipeline, value) )
		{
		__ASSERT_DEBUG( value.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );

		canPipeline = (value.StrF().Index(iStringTable) != HTTP::EDisablePipelining);
		}
	return canPipeline;
	}

/*
 *	Methods from CProtocolHandler
 */

TInt CHttpClientHandler::SessionServerCert(TCertInfo& /*aServerCert*/)
	{
	return KErrNotSupported;
	}


TInt CHttpClientHandler::TransactionServerCert(TCertInfo& aServerCert, RHTTPTransaction aTransaction)
	{
	TInt error = KErrNotFound;
	const CX509Certificate* cert = static_cast<const CX509Certificate*>(TransactionServerCert(aTransaction));
	if(cert) 
		{
		TRAPD( failed, GetCertInfoL(*cert, aServerCert));

		// pass back the leaving system error.
		error = (failed < 0 ) ? failed : KErrNone;
		}
	return error;
	}

void CHttpClientHandler::CreateCodecL()
	{
	iCodec = CHeaderCodecPlugin::NewL( KHttpClientCodecName, iSession.StringPool());
	}

CProtTransaction* CHttpClientHandler::CreateProtTransactionL(RHTTPTransaction aTransaction)
	{
	// Create the appropriate CProtTransaction object
	CHttpClientTransaction* transaction = CHttpClientTransaction::NewL(aTransaction);
	return transaction;
	}
	
TBool CHttpClientHandler::ServiceL(CProtTransaction& aTrans)
	{
		__START_PERFORMANCE_LOGGER();
	// Prepare the transaction...
	TBool needTunnel = EFalse;
	TBool canPipeline = EFalse;
	CHttpConnectionInfo* info = PrepareTransactionL(aTrans.Transaction(), needTunnel, canPipeline);
	CleanupStack::PushL(info);

	CHttpConnectionManager* manager = NULL;
	TBool isNewConnection = ETrue;
	
	if( needTunnel )
		{
		// Find a connection that is tunnelling via appropriate proxy to the
		// appropriate host.
		manager = SelectTunnelConnectionL(*info, aTrans.Transaction(), canPipeline);
		}
	else
		{
	    THTTPHdrVal value;
        _LIT8( KNtlmStateStr, "NTLMState" );
        RStringPool stringPool = iSession.StringPool();
        RStringF strF = stringPool.OpenFStringL( KNtlmStateStr );
        CleanupClosePushL(strF);
        if ( aTrans.Transaction().PropertySet().Property( strF, value ) && (value.Int() == ENtlmNegotiate || value.Int() == ENtlmAuthenticate) )
            {
            manager = SelectNtlmConnectionL(*info,aTrans.Transaction());
            if (manager == NULL)
                {
                __FLOG_0(_T8("No NTLM connection manger!!!"));
                }
            else if (value.Int() == ENtlmAuthenticate)
                {
#if defined (_DEBUG) && defined (_LOGGING)
                __FLOG_1(_T8(" NTLM AUTH msg Id %d and resetting to  -1"),manager->GetNtlmConnId());
#endif
                manager->SetNtlmConnId(KErrNotFound);//reset it
                }
            }
        else// Look for a normal connection.
            {
            isNewConnection = SelectConnectionManagerL(*info, aTrans.Transaction(), canPipeline, manager);
            }
        CleanupStack::PopAndDestroy(&strF);
		}

	if( manager != NULL )
		{
		// Pass the connection manager to the transaction - need to do this 
		// before creating tx- and rx- data objects.
		CHttpClientTransaction* trans = static_cast<CHttpClientTransaction*>(&aTrans);
		trans->SetConnectionManager(*manager);
		
		if(isNewConnection)
			{
			//Increment retry count only if it is a new connection.
			trans->IncRetryCount();
			}

		// Create the tx- and rx- objects in the transaction
		aTrans.CreateTxDataL();
		aTrans.CreateRxDataL(*this);

#if defined (_DEBUG) && defined (_LOGGING)
		CHttpRequestComposer* composer = static_cast<CHttpRequestComposer*>(&aTrans.TxData());
		CHttpResponseParser* parser = static_cast<CHttpResponseParser*>(&aTrans.RxData());

		composer->__logger__ = this->__logger__;
		parser->__logger__ = this->__logger__;
#endif

		// If the transaction cannot be pipelined, then set the connection 
		// manager to not allow pipelining. Once this transaction has completed
		// the manager will revert to allowing pipelining.
		if( !canPipeline )
			manager->DisablePipelining();
				
		// Remove connection info from cleanup stack before submiting to the 
		// connection manager - ownership is passed to the connection manager.
		CleanupStack::Pop(info);
		MHttpRequest& request = static_cast<CHttpRequestComposer&>(aTrans.TxData());
		MHttpResponse& response = static_cast<CHttpResponseParser&>(aTrans.RxData());

		manager->SubmitL(*info, request, response);
		__END_PERFORMANCE_LOGGER(_L(",CHttpClientHandler::ServiceL()"));
		return ETrue;
		}
	CleanupStack::PopAndDestroy(info);
		__END_PERFORMANCE_LOGGER(_L(",CHttpClientHandler::ServiceL()"));
	return EFalse;
	}
	
void CHttpClientHandler::ClosedTransactionHook(CProtTransaction* aTrans)
	{
	__FLOG_0(_T8("!! Closing transaction - client request"));
	__FLOG_1(_T8("-> Trans %d : closed"), aTrans->Transaction().Id());

	delete aTrans;	
	}
	
void CHttpClientHandler::CancelTransactionHook(CProtTransaction& aTransaction)
	{
	// Is this transaction still alive - check to see if it still has a 
	// connection manager.
	CHttpClientTransaction& trans = static_cast<CHttpClientTransaction&>(aTransaction);
	CHttpConnectionManager* manager = trans.ConnectionManager();

	__FLOG_0(_T8("!! Cancelling transaction - client request"));
	
	if( manager != NULL )
		{
		__FLOG_2(_T8("-> Trans %d, Con %d : still alive - cancelling its connection manager"), aTransaction.Transaction().Id(),GetConnectionID(manager));
	
		// Transaction is still alive - ask its connection manager to cancel it.
		MHttpRequest& request = static_cast<CHttpRequestComposer&>(aTransaction.TxData());
		MHttpResponse& response = static_cast<CHttpResponseParser&>(aTransaction.RxData());
		response.FlushBodyDataIfNotRead();
		manager->CancelSubmission(request, response);
		
		// Connection is now cancelled - remove the connection manager from the
		// transaction.
		trans.RemoveConnectionManager();		
		}
#if defined (_DEBUG) && defined (_LOGGING)
	else
		__FLOG_1(_T8("-> Trans %d : already finished - nothing to do"), aTransaction.Transaction().Id());
#endif
	}
	
void CHttpClientHandler::NotifyNewRequestBodyPart(CProtTransaction& aTransaction)
	{
	// Notify the transaction of more data.
	static_cast<CHttpRequestComposer&>(aTransaction.TxData()).NotifyMoreRequestBodyData();
	}
	
void CHttpClientHandler::GetInterfaceL(TUid aInterfaceId, MProtHandlerInterface*& aInterfacePtr)
	{
	switch(aInterfaceId.iUid)
		{
	case KProtHandlerTransactionServerCertUid:
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
 *	Methods from MConnectionPrefsProvider
 */

TBool CHttpClientHandler::SupplyCommsConnection( RConnection*& aConnectionPtr )
	{
	aConnectionPtr = NULL;

	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();
	THTTPHdrVal value;
	
	TBool hasValue = connInfo.Property( stringPool.StringF(HTTP::EHttpSocketConnection, iStringTable), value );
	if( hasValue && value.Type() == THTTPHdrVal::KTIntVal )
		{
		aConnectionPtr = reinterpret_cast<RConnection*>(value.Int());
		return ETrue;
		}
	return EFalse;
	}

TBool CHttpClientHandler::SupplySocketServerHandle ( TInt& aSocketServerHandle ) 
	{
	aSocketServerHandle = 0;
	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();
	THTTPHdrVal value;

	TBool hasValue = connInfo.Property(stringPool.StringF(HTTP::EHttpSocketServ, iStringTable), value);
	if( hasValue && value.Type() == THTTPHdrVal::KTIntVal )  
		{
		aSocketServerHandle = value.Int();
		return ETrue;
		}
	return EFalse;
	}

void CHttpClientHandler::SetCommsConnectionL( RConnection* aConnectionPtr )
	{
	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();
	TInt connectionPtrVal = reinterpret_cast<TInt>(aConnectionPtr);
	connInfo.SetPropertyL ( stringPool.StringF(HTTP::EHttpSocketConnection, iStringTable ), THTTPHdrVal (connectionPtrVal) );
	}
	
void CHttpClientHandler::SetSocketServerHandleL ( TInt aSocketServerHandle )
	{
	RHTTPConnectionInfo	connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();
	connInfo.SetPropertyL ( stringPool.StringF(HTTP::EHttpSocketServ, iStringTable ), THTTPHdrVal (aSocketServerHandle) );		
	}

void CHttpClientHandler::GetSecurityPrefs(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy)
	{
	// Set the security policy
	aSecurityPolicy = iSecurityPolicy;

	// Set the dialog info - check the session properties
	THTTPHdrVal value;
	RStringF secureDialog = iSession.StringPool().StringF(HTTP::ESecureDialog, iStringTable);
	TBool hasValue = iSession.ConnectionInfo().Property(secureDialog, value);
	if( hasValue && value.Type() == THTTPHdrVal::KStrFVal && 
		value.StrF().Index(iStringTable) == HTTP::EDialogNoPrompt )
		{
		// Client has requested to not be prompted
		aDialogPrompt = EFalse;
		}
	else
		{
		// The default value - the client will be prompted
		aDialogPrompt = ETrue;
		}
	}

TBool CHttpClientHandler::ImmediateSocketShutdown()
	{
	TBool immediateSocketShutdown = EFalse;
	if( *iSessionClosingPtr ) // iSessionClosingPtr cannot be NULL as its is set in ConstructL
		{
		// Session is closing down, check the session properties to check if the client has requested
		// an immediate socket shutdown
		THTTPHdrVal value;
		RStringF socketShutdownMode = iSession.StringPool().StringF(HTTP::ESocketShutdownMode, iStringTable);
		TBool hasValue = iSession.ConnectionInfo().Property(socketShutdownMode, value);
		if( hasValue && value.Type() == THTTPHdrVal::KStrFVal && 
			value.StrF().Index(iStringTable) == HTTP::ESocketShutdownImmediate )
			{
			__FLOG_0(_T8("!! Immediate socket shutdown requested by client"));
			immediateSocketShutdown = ETrue;
			}
		}

	return immediateSocketShutdown;
	}
	
TInt CHttpClientHandler::SessionId()
	{
	THTTPHdrVal value;
	TInt result = KErrNotFound;
	const TBool hasValue = iSession.ConnectionInfo().Property(iSession.StringPool().StringF(HTTP::ESessionId, iStringTable), value);
	if( hasValue && value.Type()==THTTPHdrVal::KTIntVal) // silently ignore inappropriate types
		{
		result = value.Int();
		if(result<0)
			{
			result = KErrNotFound;
			}
		}
	return result;
	}


/*
 *	Methods from MRxDataObserver
 */
 
TInt CHttpClientHandler::SetStatus(CRxData& aRxData, TInt aStatus)
	{
	// Have received a status message from an Rx data object - check the status.
	CHttpClientTransaction& protTrans = static_cast<CHttpClientTransaction&>(aRxData.ProtTrans());
	RHTTPTransaction trans = protTrans.Transaction();
	TInt err = KErrNone;
	switch( aStatus )
		{
	case THTTPEvent::EResponseComplete:
		{
		__FLOG_2(_T8("Trans %d, Con %d : transaction completed"), trans.Id(), GetConnectionID(trans));

		// The response is complete - the client has been passed all the data
		// and released it. Check to see if this was a CONNECT request.
		RStringPool stringPool = iSession.StringPool();

		TInt status = aStatus;
		if( trans.Request().Method().Index(iStringTable) == HTTP::ECONNECT )
			{
			if( HTTPStatus::IsSuccessful(trans.Response().StatusCode()) )
				{
				// A 2xx status code - tunnel has been successfully established. 
				// Mark the connection manager as tunnelled connection, providing 
				// the host to which the tunnel leads.
				THTTPHdrVal hostVal;
#ifdef _DEBUG
				TBool found = 
#endif
				trans.PropertySet().Property(stringPool.StringF(HTTP::ETunnel, iStringTable), hostVal);

				__ASSERT_DEBUG( found, User::Invariant() );
				__ASSERT_DEBUG( hostVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );

				protTrans.ConnectionManager()->TunnelConnection(hostVal.StrF());
				}
			else
				{
				status = THTTPEvent::EFailed;
				
#if defined (_DEBUG) && defined (_LOGGING)
				THTTPHdrVal hostVal;
				TBool found = trans.PropertySet().Property(stringPool.StringF(HTTP::ETunnel, iStringTable), hostVal);

				__ASSERT_DEBUG( found, User::Invariant() );
				__ASSERT_DEBUG( hostVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );

				const CHttpConnectionInfo& connectionInfo = protTrans.ConnectionManager()->ConnectionInfo();

				__FLOG_2(
						_T8("!! Tunnel failed : %d %S"),
						trans.Response().StatusCode(), 
						&trans.Response().StatusText().DesC()
						);
				__FLOG_5(
						_T8("-> Tunnel to %S on connection to host %S, remote port %d (secure : %d, nonpersistent : %d)"),
						&hostVal.StrF().DesC(), 
						&connectionInfo.Host(), 
						connectionInfo.Port(), 
						connectionInfo.IsSecure(), 
						connectionInfo.IsNonPersistent()
						);
#endif
				}
			}

		// Ensure that the connection manager is still not dealing with this 
		// request.
		MHttpRequest& request = static_cast<CHttpRequestComposer&>(protTrans.TxData());
		protTrans.ConnectionManager()->CheckRequestComplete(request);

		// The transaction has no further use for the connection manager - it 
		// can now be removed.
		protTrans.RemoveConnectionManager();
			
		// The transaction is now complete - inform the base class.
		err = TransactionCompleted(trans, status);
		} break;
	default:
		// Unknown status - do nothing, unless an error
		if( aStatus < 0 )
			{
			__FLOG_1(_T8("!! Error : %d"), aStatus);

			if( aStatus == KErrHttpPipeliningError )
				{
				__FLOG_2(_T8("-> Trans %d, Con %d : transaction was being pipelined"), trans.Id(), GetConnectionID(trans));
				__FLOG_2(_T8("-> Trans %d, Con %d : re-try without pipelining"), trans.Id(), GetConnectionID(trans));

				// Specify that this transaction should not be pipelined when 
				// it is re-submitted.
				RStringPool stringPool = iSession.StringPool();
				RStringF pipeline = stringPool.StringF(HTTP::EHttpPipelining, iStringTable);
				RStringF disable = stringPool.StringF(HTTP::EDisablePipelining, iStringTable);

				err = trans.PropertySet().SetProperty(pipeline, disable);
				if(err != KErrNone)
				    {
				    break;
				    }
				}
#if defined (_DEBUG) && defined (_LOGGING)
			else if( aStatus == KErrHttpNonPipeliningError )
				{
				__FLOG_2(_T8("-> Trans %d, Con %d : transaction was being pipelined"), trans.Id(), GetConnectionID(trans));
				__FLOG_2(_T8("-> Trans %d, Con %d : re-try with pipelining again"), trans.Id(), GetConnectionID(trans));
				}
			else
				{
				__FLOG_2(_T8("-> Trans %d, Con %d : transaction failed"), trans.Id(), GetConnectionID(trans));
				}
#endif
			// An error code has occured. As no further data will be exchanged
			// with the origin server. The connection manager can now be removed.
			protTrans.RemoveConnectionManager();

			if( aStatus == KErrEof )
				{
				__FLOG_4(_T8("-> Trans %d, Con %d : reporting %d (KErrDisconnected) instead of %d"), trans.Id(), GetConnectionID(trans), KErrDisconnected, aStatus);				

				// Convert these errors to KErrDisconnected
				aStatus = KErrDisconnected;
				}

			// Propagate the error back to the client and mark this transaction
			// as completed.
			err = TransactionCompleted(trans, aStatus);
			}
		break;
		}
	return err;
	}

void CHttpClientHandler::SetStatusL(CRxData& aRxData, TInt aStatus)
    {
    User::LeaveIfError(SetStatus(aRxData, aStatus));
    }

/*
 *	Methods from MHttpBatchingPropertiesCallback
 */

TInt CHttpClientHandler::GetMaxBatchingBufferSize()
	{
	TInt batchingBuffer = 0;
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	RStringPool stringPool = iSession.StringPool();

	THTTPHdrVal doBatching;
	RStringF batchingSetting = stringPool.StringF(HTTP::EHttpBatching, iStringTable);

	if (connInfo.Property(batchingSetting, doBatching))
		{
		TBool batchingSupported = EFalse;	// default behaviour is batching disabled

		// First of all check if batching has been enabled
		batchingSupported = (doBatching.StrF().Index(iStringTable) == HTTP::EEnableBatching);
		if (batchingSupported)
			{
			THTTPHdrVal bufferSize;
			RStringF buffer = stringPool.StringF(HTTP::EBatchingBufferSize, iStringTable);

			// If batching has been enabled, check for a client-specified buffer size to use
			if (connInfo.Property(buffer, bufferSize))
				{
				__ASSERT_DEBUG(bufferSize.Type() == THTTPHdrVal::KTIntVal, THttpClientPanic::Panic(THttpClientPanic::EInvalidBatchingSetting));
				batchingBuffer = bufferSize.Int();
				}
			// No client-specified buffer size therefore use the default value
			else
				batchingBuffer = KDefaultBatchingBufSize;
			}
		}
	return batchingBuffer;
	}


/*
 *	Methods from MProtHandlerInterface
 */

const CCertificate*  CHttpClientHandler::SessionServerCert()
	{
	return NULL;
	}
	
const CCertificate* CHttpClientHandler::TransactionServerCert(RHTTPTransaction aTransaction)
	{
	const CProtTransaction* trans = FindProtocolTransaction(aTransaction);
	if(trans != NULL)
		{
		CHttpConnectionManager* manager = static_cast<const CHttpClientTransaction*>(trans)->ConnectionManager();
		if( manager )
			{
			return manager->ServerCert();
			}
		}
	return NULL;
	}


void CHttpClientHandler::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	// Handle the event
	switch (aEvent.iStatus)
		    {
	case THTTPEvent::EGetCipherSuite:
		    {
		    GetCipherSuiteL(aTransaction);
		    }
		    break;
	case THTTPEvent::ECancelWaitFor100Continue:
		{
		const CProtTransaction* pT = FindProtocolTransaction(aTransaction);
		if(pT != NULL)
			{
			static_cast<CHttpRequestComposer&>(pT->TxData()).CancelWaitFor100Continue();
			}
		}
		break;
	default:
		   CProtocolHandler::MHFRunL(aTransaction,aEvent);
		   }
	}

void CHttpClientHandler::GetCipherSuiteL(RHTTPTransaction aTransaction)
	{
	RStringPool stringPool = iSession.StringPool();
	RHTTPTransactionPropertySet properties(aTransaction.PropertySet());
	const CProtTransaction* transaction = FindProtocolTransaction(aTransaction);
	
	if (transaction != NULL)
		{
		CHttpConnectionManager* connectionManager = static_cast<const CHttpClientTransaction*>(transaction)->ConnectionManager();
		if(connectionManager)
			{
			TBuf8<8> cipherSuite;
			TInt error = connectionManager->CipherSuite(cipherSuite);
			
			if (error == KErrNone)
				{
				RString cipherSuiteString = stringPool.OpenStringL(cipherSuite);
				THTTPHdrVal hdrValue(cipherSuiteString);
				CleanupClosePushL(cipherSuiteString);
				properties.SetPropertyL(stringPool.StringF(HTTP::ECipherSuiteValue, iStringTable), hdrValue);
				CleanupStack::PopAndDestroy(&cipherSuiteString);	
				return;
				}		
			}
		}
	// No cipher suite could be obtained so set the ECipherSuiteValue property to an empty string.
	properties.SetPropertyL(stringPool.StringF(HTTP::ECipherSuiteValue, iStringTable), THTTPHdrVal(RString()));
	}

TInt CHttpClientHandler::MaxNumTransactionsToPipeline() const
	{
	/* This is called when the first transaction is submitted. This is so filters can set the
	   session setting HTTP::EMaxNumTransactionsToPipeline
	   However it is only required to be set when the first transaction is submitted. Therefore
	   check to see if the value is already cached.
	*/
 	if (iMaxNumTransactionsToPipeline == 0)
 		{
 		iMaxNumTransactionsToPipeline = KDefaultMaxNumberTransactionsToPipeline;
 		
 		// Check session properties
		RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
		RStringPool stringPool = iSession.StringPool();
		THTTPHdrVal maxToPipeline;
		RStringF maxToPipelineString = stringPool.StringF(HTTP::EMaxNumTransactionsToPipeline, iStringTable);
		if(connInfo.Property(maxToPipelineString, maxToPipeline))
			{
			if(maxToPipeline.Type() == THTTPHdrVal::KTIntVal)
				{
				iMaxNumTransactionsToPipeline = maxToPipeline.Int();
				}
			}
		}
 	
 	return iMaxNumTransactionsToPipeline;
	}


void CHttpClientHandler::__RecordConnectionManagerCreationL()
	// In debug mode update a session property to record when each connection manager is created
	// This is used to validate the runtime behavior of pipelining use cases
	{
	#if defined (_DEBUG)
	_LIT8(KNumberConnectionManagers, "__NumConnectionManagers");
	
	RStringPool stringPool = iSession.StringPool();
	RStringF numberConnectionsString = stringPool.OpenFStringL(KNumberConnectionManagers);
	CleanupClosePushL(numberConnectionsString);
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	TInt numberConnections =0;
	
	THTTPHdrVal numberConnectionsVal;
	if (connInfo.Property(numberConnectionsString, numberConnectionsVal))
		{
		numberConnections = numberConnectionsVal.Int();
		connInfo.RemoveProperty(numberConnectionsString);
		}
	
	numberConnections++;
	numberConnectionsVal.SetInt(numberConnections);
	connInfo.SetPropertyL(numberConnectionsString, numberConnectionsVal);
	
	CleanupStack::PopAndDestroy(&numberConnectionsString);
	#endif	
	}
	
#if defined (_DEBUG)
	TInt CHttpClientHandler::GetConnectionID( const CHttpConnectionManager* aConnectionManager )
	//in debug mode it returns the connection ID, if not in the list returns 0
	{
	TInt i = iConnectionManagers.Count();
	for( ;i > 0; --i )
		{
		if( iConnectionManagers[i-1] == aConnectionManager )
			{
			break;
			}
		}
	return 	i;	
	}
	
	TInt CHttpClientHandler::GetConnectionID( const RHTTPTransaction &aTrans )
	{
	CHttpConnectionManager* manager = NULL;
	const CProtTransaction* trans = FindProtocolTransaction(aTrans);
	if(trans != NULL)
		{
		manager = static_cast< const CHttpClientTransaction* >( trans )->ConnectionManager();
		}
		
	return GetConnectionID( manager );
	}




#endif

void CHttpClientHandler::GetCertInfoL(const CX509Certificate& aSource, TCertInfo& aDest)
	{
	TInt len;

	//Fetch Fingerprint
	len = Min(aSource.Fingerprint().Length(),aDest.iFingerprint.MaxLength());
	aDest.iFingerprint.Copy(aSource.Fingerprint().Ptr(),len);

	//Fetch SerialNumber
	len = Min(aSource.SerialNumber().Length(),aDest.iSerialNo.MaxLength());
	aDest.iSerialNo.Copy(aSource.SerialNumber().Ptr(),len);

	//Fetch PublicKey
	const CSubjectPublicKeyInfo& publicKeyInfo = aSource.PublicKey();
	len = Min(publicKeyInfo.KeyData().Length(),aDest.iPublicKey.MaxLength());
	aDest.iPublicKey.Copy(publicKeyInfo.KeyData().Ptr(),len);

	//Fetch PublicKeyAlg
	aDest.iPkAlg= publicKeyInfo.AlgorithmId();

	//Fetch VersionNo
	aDest.iVersionNo = aSource.Version();

	//Fetch StartValDate
	const CValidityPeriod& validityPeriod = aSource.ValidityPeriod();
	aDest.iStartValDate = validityPeriod.Start();

	//Fetch EndValDate
	aDest.iEndValDate = validityPeriod.Finish();

	//Fetch SubjectDNInfo
	const CX500DistinguishedName& subjectName = aSource.SubjectName();
	GetDNInfo(subjectName,aDest.iSubjectDNInfo);

	//Fetch IssuerDNInfo;
	const CX500DistinguishedName& issuerName = aSource.IssuerName();
	GetDNInfo(issuerName,aDest.iIssuerDNInfo);

	// Fetch Alt Name
	aDest.iDNSName.Copy(KNullDesC);

	//fetch digest alg
	aDest.iDigAlg = aSource.SigningAlgorithm().DigestAlgorithm().Algorithm();
	const CX509CertExtension* ext = aSource.Extension(KSubjectAltName); 

	if( ext )
		{
		CX509AltNameExt* subjectAltName = CX509AltNameExt::NewLC(ext->Data());
		const CArrayPtrFlat<CX509GeneralName>& gNs = subjectAltName->AltName();
		const TInt count = gNs.Count(); 

		for( TInt i = 0; i < count; ++i )
			{ 
			const CX509GeneralName* gN = gNs.At(i);
			
			if( gN->Tag() == EX509DNSName )
				{ 
				CX509DNSName* dNS = CX509DNSName::NewLC(gN->Data());
				const TPtrC dNSValue = dNS->Name();
				aDest.iDNSName.Copy(dNSValue);
				CleanupStack::PopAndDestroy(dNS);
				break;
				} 
			}
		CleanupStack::PopAndDestroy(subjectAltName);
		} 

	}

void CHttpClientHandler::GetDNInfo(const CX500DistinguishedName& aSource, TDNInfo& aDest)
	{
	const TInt count = aSource.Count();
	for( TInt i=0; i<count; ++i )
		{
		const CX520AttributeTypeAndValue& attribute = aSource.Element(i);

		HBufC* valuePtr = NULL;
		TDes8* destination = NULL;
		TInt maxLength = 0 ;
		TBool found = ETrue;

		const TDesC& type = attribute.Type();
		if( type.Compare(KX520CountryName) == 0 )
			{
			maxLength = aDest.iCountry.MaxLength();
			destination = &aDest.iCountry;
			}
		else if( type.Compare(KX520OrganizationalUnitName) == 0 )
			{
			maxLength = aDest.iOrganizationUnit.MaxLength();
			destination = &aDest.iOrganizationUnit;
			}
		else if( type.Compare(KX520OrganizationName) == 0 )
			{
			maxLength = aDest.iOrganization.MaxLength();
			destination = &aDest.iOrganization;
			}
		else if( type.Compare(KX520CommonName) == 0 )
			{
			maxLength = aDest.iCommonName.MaxLength();
			destination = &aDest.iCommonName;
			}
		else if( type.Compare(KX520LocalityName) == 0 )
			{
			maxLength = aDest.iLocality.MaxLength();
			destination = &aDest.iLocality;
			}
		else
			{
			found = EFalse;
			}

		if( found )
			{
			TRAPD(ret, (valuePtr = attribute.ValueL()));

			if( ret != KErrNone )
				{
				// Non fatal error - just zero the descriptor
				destination->Zero();
				}
			else
				{
				TInt len = Min(valuePtr->Length(), maxLength);
				TPtrC value = TPtrC(valuePtr->Des().Ptr(), len);
				destination->Copy(value);
				delete valuePtr;
				}
			}
		}
	}

TInt CHttpClientHandler::GetRecvBufferSize()
	{
	
		iRecvBufferSize = KDefaultBufferSize;
 		// Check session properties
		RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
		RStringPool stringPool = iSession.StringPool();
		THTTPHdrVal value;
		RStringF recvBufferSize = stringPool.StringF(HTTP::ERecvBufferSize, iStringTable);
		if(connInfo.Property(recvBufferSize, value))
			{
			if(value.Type() == THTTPHdrVal::KTIntVal && value.Int() > 0)
				{
				iRecvBufferSize = value.Int();
				}
			}
	
 	return iRecvBufferSize;
	}

void CHttpClientHandler::EnsureStrictConnectMethodHeaders(RHTTPTransaction aTransaction)
    {
    // Check only for CONNECT method request
    // Strict & mandatory headers as needed by CONNECT request are
    // User-Agent, x-wap-profile, EProxyAuthorization
    
    RHTTPRequest request = aTransaction.Request();
    RHTTPSession session = aTransaction.Session();
    RStringPool sp = session.StringPool();
    if(aTransaction.Request().Method().Index(iStringTable) == HTTP::ECONNECT)
        {
        RStringF strictConnectHeaders = sp.StringF(HTTP::EStrictConnectHeaders, iStringTable);
        RHTTPPropertySet sessionProperties = session.ConnectionInfo();
        THTTPHdrVal hdrVal;
        if(sessionProperties.Property(strictConnectHeaders, hdrVal) && 
                (hdrVal.Type() == THTTPHdrVal::KStrFVal) && 
                (hdrVal.StrF().Index(iStringTable) == HTTP::EEnableStrictConnectHeaders))
            {
            RStringF profileHeader = sp.OpenFStringL(KUAProfile);
            CleanupClosePushL(profileHeader);

            
            RHTTPHeaders hdr = request.GetHeaderCollection();
            THTTPHdrFieldIter it = hdr.Fields();
            while(!it.AtEnd())
                {
                RStringTokenF hdrStr = it();
                RStringF hdrNameStr = sp.StringF(hdrStr);
                TInt hdrIndex = hdrNameStr.Index(iStringTable);
                // Check if it is a UA/User-Agent/Proxy-Auhorization Profile header
                if((hdrNameStr == profileHeader) || (hdrIndex == HTTP::EUserAgent) || (hdrIndex == HTTP::EProxyAuthorization))
                    {
                    ++it; // Keep going
                    }
                else
                    {
                    // Anything else remove the header and reset the iterator
                    hdr.RemoveField(hdrNameStr);
                    it.First(); // Not so efficient. 
                    }
                }
            CleanupStack::PopAndDestroy(&profileHeader);            
            }        
        }
    }

