// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "rexplicithttpsession.h"

#include <commdbconnpref.h>

#include <cdblen.h>
#include <metadatabase.h>
#include <commsdattypesv1_1.h>
using namespace CommsDat;


_LIT(KSlashChar, "\\");
const TInt32 KMaxLenColonNumberStr = (1 + 10);	// 1 Char for ':' and 10 Chars for TUint32 string

/**
A default RExplicitHTTPSession constructor.
*/
EXPORT_C RExplicitHTTPSession::RExplicitHTTPSession()
	{
	}


/**
This is a wrapper for the session handle RHTTPSession.
It opens a session and sets up the proxies and the network settings using the provided IAP.
It also starts a connection using the connection preference settings in CommDb.
This function leaves with an appropriate error code if the open failed.

@param aUri This is used to retrieve the proxy record. If no proxy is found for the given URI,
			then no proxy will be used by default.
@param aIapNumber This carries the IAP. If this is non-zero, it will be set as a 
				  connction preference and the user will not be prompted for an IAP.
@param aStartConnection If set to ETrue, a new connection is started. This by default is set to ETrue.
*/
EXPORT_C void RExplicitHTTPSession::OpenL(const TUriC8& aUri, TUint32 aIapNumber, TBool aStartConnection)
	{
	BindConnectionL(aIapNumber, aStartConnection);
	
	/*
	 Finally, get the proxy settings from the comms database. Note that if 
	 there is a problem with retrieving this info, we will use no proxy by 
	 default.
	 The proxy which is retrieved is based on the scheme used by the URI
	 to be opened. eg if the URI to be opened is https://myhost.com/...
	 then the HTTPS proxy will be used
	*/
	
	HBufC8* proxy = NULL;
	
	TBool useProxy = EFalse;
	
	TRAPD(err, useProxy = UseProxyL(aUri.Extract(EUriScheme), proxy));
	
	if (useProxy && !err)
		{
		RStringPool strPool = iHTTPSession.StringPool();
		RHTTPConnectionInfo connInfo = iHTTPSession.ConnectionInfo();
		
		CleanupStack::PushL(proxy);
		RStringF proxyAddr = strPool.OpenFStringL(*proxy);
		CleanupClosePushL(proxyAddr);
		
		THTTPHdrVal proxyUsage(strPool.StringF(HTTP::EUseProxy,
			                   RHTTPSession::GetTable()));
			                   
		connInfo.SetPropertyL(strPool.StringF(HTTP::EProxyUsage,
			                  RHTTPSession::GetTable()), proxyUsage);
			                  
		connInfo.SetPropertyL(strPool.StringF(HTTP::EProxyAddress,
			                  RHTTPSession::GetTable()), proxyAddr);
			                  
		CleanupStack::PopAndDestroy(2, proxy);	// pop proxyAddr and proxy
		}				
	}


/**
This API can be used to start an RConnection. It uses the connection preferences
set while opening the handle to RExplicitHTTPSession to start the RConnection.

@param aStatus On return, the status of the request, e.g. KErrNone, KErrAlreadyExists.
*/
EXPORT_C void RExplicitHTTPSession::StartConnection(TRequestStatus& aStatus)
	{
	if (iIap)
		{
		TCommDbConnPref connectionPref;
		connectionPref.SetIapId(iIap);
		connectionPref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
		iConnection.Start(connectionPref, aStatus);
		}
	else
		{
		iConnection.Start(aStatus);
		}
	}


/**
This API is used to close the RConnection.
*/
EXPORT_C void RExplicitHTTPSession::CancelStart()
	{
	iConnection.Close();
	iConnection.Open(iSocketServ);
	}


/**
Closes the handle to RExplicitHTTPSession. It also closes the handle to RHTTPSession, the RConnection
and the RSocketServ.
*/
EXPORT_C void RExplicitHTTPSession::Close()
	{
	iHTTPSession.Close();
	iConnection.Close();
	iSocketServ.Close();
	}


/**
Returns a handle to the RHTTPSession.

@return RHTTPSession
*/
EXPORT_C RHTTPSession& RExplicitHTTPSession::HTTPSession()
	{
	return iHTTPSession;
	}

void RExplicitHTTPSession::BindConnectionL(TUint32 aIap, TBool aStartConnection)
	{
	iIap = aIap;
	// Open the socket server to create the connection
	User::LeaveIfError(iSocketServ.Connect());
	User::LeaveIfError(iConnection.Open(iSocketServ));

	// we may wish to do an async connection start.
	if (aStartConnection)
		{
		// If the IAP number is non-zero, we have an explicit IAP to use
		if (iIap)
			{
			TCommDbConnPref connectionPref;
			connectionPref.SetIapId(iIap);
			connectionPref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
			User::LeaveIfError(iConnection.Start(connectionPref));
			}
		else
			{
			User::LeaveIfError(iConnection.Start());
			}
		}

	// Now open the real HTTP session and bind this session with the
	// socket server/connection
	iHTTPSession.OpenL();
	RStringPool strPool = iHTTPSession.StringPool();
	
	RHTTPConnectionInfo connInfo = iHTTPSession.ConnectionInfo();
	
	connInfo.SetPropertyL(strPool.StringF(HTTP::EHttpSocketServ, 
						  RHTTPSession::GetTable()), THTTPHdrVal(iSocketServ.Handle()));
						  
	connInfo.SetPropertyL(strPool.StringF(HTTP::EHttpSocketConnection, 
						  RHTTPSession::GetTable()), 
		                  THTTPHdrVal(reinterpret_cast<TInt>(&iConnection)));
	}

TBool RExplicitHTTPSession::UseProxyL(const TDesC8& aScheme, HBufC8*& aProxyServer)
	{
	TBool useProxy = EFalse;

	// Get the service id and service type from the connection
	TBuf<KCommsDbSvrMaxFieldLength> field;

	field = TPtrC(IAP);
	field.Append(KSlashChar);
	field.Append(TPtrC(IAP_SERVICE));

	TUint32 serviceId;
	User::LeaveIfError(iConnection.GetIntSetting(field, serviceId));

	field = TPtrC(IAP);
	field.Append(KSlashChar);
	field.Append(TPtrC(IAP_SERVICE_TYPE));

	TBuf<KCommsDbSvrMaxFieldLength> serviceType;
	
	User::LeaveIfError(iConnection.GetDesSetting(field, serviceType));

	// Now we've got the comms database, from the serviceId and serviceType,
	// get the proxy record
	
	// use commsDat API

 	// Create CommmsDat seesion using latest version of commsdat
 	CMDBSession* session = CMDBSession::NewLC(CMDBSession::LatestVersion());
	//CMDBSession* session = CMDBSession::NewLC(KCommsDatVersion);
	
	// ***************************************
	// Create the CMDBRecordSet for the search
	// ***************************************
	CMDBRecordSet<CCDProxiesRecord>* pProxiesRecordSet = new (ELeave)CMDBRecordSet<CCDProxiesRecord>(KCDTIdProxiesRecord);
	CleanupStack::PushL(pProxiesRecordSet);

	CCDProxiesRecord* primingRecord = static_cast <CCDProxiesRecord*> (CCDRecordBase::RecordFactoryL(KCDTIdProxiesRecord));
	CleanupStack::PushL(primingRecord);

	// ************************************
	// build priming record for the search
	// ************************************
	primingRecord->iServiceType.SetMaxLengthL(serviceType.Length());
	primingRecord->iServiceType = serviceType;
	
	primingRecord->iService = serviceId;
	primingRecord->iUseProxyServer = ETrue;

	// Create 16-bit copy and set ProtocolName
	HBufC16* protocolName16 = HBufC16::NewLC(aScheme.Length());
	TPtr16 ptr(protocolName16->Des());
	ptr.Copy(aScheme);
	primingRecord->iProtocolName.SetMaxLengthL(ptr.Length());
	primingRecord->iProtocolName = ptr;
	CleanupStack::PopAndDestroy(protocolName16);
	
	pProxiesRecordSet->iRecords.AppendL(primingRecord);
	CleanupStack::Pop(primingRecord);
	
	// *****************************
	// Search for the priming record
	// *****************************
	if (pProxiesRecordSet->FindL(*session))
		{
			// Proxy is located so copy settings to aProxyServer
			TPtrC serverName(static_cast <CCDProxiesRecord*> (pProxiesRecordSet->iRecords[0])->iServerName);

			// Create the 8-bit version allowing extra characters for port number
			HBufC8* proxyServer = HBufC8::NewLC(serverName.Length() + KMaxLenColonNumberStr);

			TPtr8 ptr(proxyServer->Des());
			ptr.Copy(serverName);

			ptr.Append(':');
			ptr.AppendNum(static_cast <CCDProxiesRecord*> (pProxiesRecordSet->iRecords[0])->iPortNumber);
			useProxy = ETrue;
			CleanupStack::Pop(proxyServer);
			aProxyServer = proxyServer;
		}
	else
		{
			// do nothing
			// useProxy = FALSE
		}

	CleanupStack::PopAndDestroy(pProxiesRecordSet);
	CleanupStack::PopAndDestroy(session);

	return useProxy;
	}
