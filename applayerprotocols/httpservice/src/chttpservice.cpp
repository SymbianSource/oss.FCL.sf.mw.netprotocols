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

#include "chttpservice.h"
#include "httpclientutils.h"
#include "chttpclientauthentication.h" 
#include "mhttpserviceauthentication.h" 
#include "chttpnetworkconnectioninfo.h"
const TInt KMaxNoOfConnections = 6;
const TInt KMaxTransToPipeline = 5;

/**
 * The default 2-phase constructor to create a CHttpService instance
 * @publishedAll
 * @prototype  
 */
EXPORT_C CHttpService* CHttpService::NewL()
	{
    CHttpService* self = new(ELeave) CHttpService;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;	
	}
/**
 * Destructor
 * @publishedAll
 * @prototype  
 */	

EXPORT_C CHttpService::~CHttpService()
	{
	if(iHttpServiceStruct)
	    {
	iHttpServiceStruct->iHttpSession.Close();
	    }
	delete iHttpServiceStruct;
	}

/**
 * Destructor of the inner class CHttpServiceStruct
 */ 
CHttpService::CHttpServiceStruct::~CHttpServiceStruct()
    {
    delete iHttpClientAuthentication;
    }

/**
 * Retrieves the equivalent string for a give string ID from the 
 * HTTP string pool
 * 
 * @param @aStringId String ID
 * @param aPtr  String
 * 
 * @return KErrNotFound if the string ID is not found in the HTTP string pool
 *          otherwise KErrNone for success
 *          
 * @publishedAll
 * @prototype 
 */
EXPORT_C TInt CHttpService::String(TInt aStringId, TPtrC8& aPtr)
    {
    RStringF str = iHttpServiceStruct->iHttpSession.StringPool().StringF(aStringId, RHTTPSession::GetTable());
    aPtr.Set(str.DesC());
    return KErrNone;
    }
/**
 * Set the proxy that applies to all HTTP client transactions that "this"
 * CHttpService instance creates
 * 
 * @param aProxyAddress Proxy address. [for ex;  http://myproxy.org:8081]
 * @return KErrNoMemory if the proxy information cannot be set other KErrNone for success.
 * 
 */

EXPORT_C TInt CHttpService::SetProxy(const TDesC8& aProxyAddress)
	{
	RHTTPSession sess = iHttpServiceStruct->iHttpSession;
	RStringPool sp = sess.StringPool();
	RHTTPConnectionInfo connInfo = sess.ConnectionInfo();
	RStringF property  = sp.StringF(HTTP::EProxyUsage, RHTTPSession::GetTable());
	RStringF useProxy = sp.StringF(HTTP::EUseProxy, RHTTPSession::GetTable());
	TInt err = connInfo.SetProperty(property, useProxy);
	if(err == KErrNone)
		{
		RStringF str;
		err = HttpClientUtils::OpenStringF(aProxyAddress, sp, str);
		if(err == KErrNone)
		    {
		    err = connInfo.SetProperty(sp.StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()), str);
		    str.Close();
		    }
		}		
	return err;	
	}

/**
 * Returns the proxy address that is set
 * 
 * @return Proxy address otherwise KNullDesC8 if the proxy information
 *         is not set.
 *         
 * @publishedAll
 * @prototype  
 */

EXPORT_C const TDesC8& CHttpService::ProxyAddress() const
	{
	RHTTPSession sess = iHttpServiceStruct->iHttpSession;
	RStringPool sp = sess.StringPool();
	RHTTPConnectionInfo connInfo = sess.ConnectionInfo();
	THTTPHdrVal useProxy;
	if(connInfo.Property(sp.StringF(HTTP::EProxyUsage, RHTTPSession::GetTable()), useProxy))
		{
		THTTPHdrVal proxyAddress;
		if(connInfo.Property(sp.StringF(HTTP::EProxyAddress, RHTTPSession::GetTable()), proxyAddress))
			{
			return proxyAddress.StrF().DesC();			
			}
		}
	return KNullDesC8();
	}

/**
 * Set the maxuimum number of TCP connections[socket] CHttpService instance that can activated 
 * at any time. The default no. of connections that the CHttpService instance uses is 6.
 * 
 * @param aValue aValue No. of connections
 * 
 * @publishedAll
 * @prototype  
 */

EXPORT_C void CHttpService::SetMaxConnections(TInt aValue)
	{
	RHTTPSession sess = iHttpServiceStruct->iHttpSession;
	RStringPool sp = sess.StringPool();
	RHTTPConnectionInfo connInfo = sess.ConnectionInfo();
	
	connInfo.SetProperty(sp.StringF(HTTP::EMaxNumTransportHandlers, RHTTPSession::GetTable()), THTTPHdrVal(aValue));		
	}

/**
 * Returns the maximum no. of TCP connections that is set.
 * 
 */

EXPORT_C TInt CHttpService::MaxConnections() const
	{
	RHTTPSession sess = iHttpServiceStruct->iHttpSession;
	RStringPool sp = sess.StringPool();
	RHTTPConnectionInfo connInfo = sess.ConnectionInfo();
	THTTPHdrVal val;
	if(connInfo.Property(sp.StringF(HTTP::EMaxNumTransportHandlers, RHTTPSession::GetTable()), val))
		{
		if(val.Type() == THTTPHdrVal::KTIntVal)
			return val.Int();
		}
	return 0;
	}
	
/**
 * Sets the maximum number of transactions to be pipelined.
 * @param aValue - number of transactions
 *
 * @publishedAll
 * @prototype 
 */ 
EXPORT_C void CHttpService::SetMaxTransactionsToPipeline(TInt aValue)
	{
	RHTTPSession sess = iHttpServiceStruct->iHttpSession;
	RStringPool sp = sess.StringPool();
	RHTTPConnectionInfo connInfo = sess.ConnectionInfo();
	
	connInfo.SetProperty(sp.StringF(HTTP::EMaxNumTransactionsToPipeline, RHTTPSession::GetTable()), THTTPHdrVal(aValue));				
	}

/**
 * Returns the maximum number of transactions to be pipelined.
 * 
 * @publishedAll
 * @prototype 
 */ 
EXPORT_C TInt CHttpService::MaxTransactionsToPipeline() const
	{
	RHTTPSession sess = iHttpServiceStruct->iHttpSession;
	RStringPool sp = sess.StringPool();
	RHTTPConnectionInfo connInfo = sess.ConnectionInfo();
	THTTPHdrVal val;
	if(connInfo.Property(sp.StringF(HTTP::EMaxNumTransactionsToPipeline, RHTTPSession::GetTable()), val))
		{
		if(val.Type() == THTTPHdrVal::KTIntVal)
			return val.Int();
		}
	return 0;		
	}

/**
 * This method facilitates to set the header once in the CHttpService instance that is applicable
 * for all HTTP client transactions. [For ex; User-Agent header] 
 * 
 * @param aStringId - Pre-defined String ID in the HTTP string pool
 * @param aHeaderValue - Value for the header 
 * 
 * @publishedAll
 * @prototype 
 */
EXPORT_C TInt CHttpService::AddRequestHeader(TInt aStringId, const THttpHeaderValueVariant& aHeaderValue)
	{
	RHTTPHeaders sessHeaders = iHttpServiceStruct->iSessionHeaders;
    RStringPool sp = iHttpServiceStruct->iHttpSession.StringPool();
    THTTPHdrVal val = HttpClientUtils::CopyHttpHdrVal(aHeaderValue, sp);
    TInt err = sessHeaders.SetField(sp.StringF(aStringId, RHTTPSession::GetTable()), val);
    HttpClientUtils::CloseString(val);
    return err;
	}

/**
 * This method facilitates to set the custom HTTP header once in the CHttpService instance that is 
 * applicable for all HTTP client transactions. [For ex; User-Agent header] 
 * 
 * @param aHeaderName - Custom header name
 * @param aHeaderValue - Value for the header 
 * 
 * @publishedAll
 * @prototype 
 */
EXPORT_C TInt CHttpService::AddCustomRequestHeader(const TDesC8& aHeaderName, const TDesC8& aHeaderValue)
	{
	_LIT8(KFieldSeparator, "\n");
	RStringPool sp = iHttpServiceStruct->iHttpSession.StringPool();
	RStringF str;
	if(HttpClientUtils::OpenStringF(aHeaderName, sp, str) != KErrNone)
        {
        return KErrNoMemory;
        }
	TInt err = iHttpServiceStruct->iSessionHeaders.SetRawField(str, aHeaderValue, KFieldSeparator);
	str.Close();
	return err;
	}

/**
 * This method facilitates to set to construct the authentication credentials to be passed by the application
 * The derived class from MHTTPServiceAuthentication  will be called from the framework, if authentication is required.
 * @param aCallback - Derived class from MHTTPServiceAuthentication
 *
 * @publishedAll
 * @prototype 
 */
EXPORT_C TInt CHttpService::SetAuthentication(MHTTPServiceAuthentication* aCallback)
    {
    TInt error = KErrGeneral;
    iHttpServiceStruct->iHttpClientAuthentication = CHttpClientAuthentication::New(iHttpServiceStruct->iHttpSession, aCallback);
    if(iHttpServiceStruct->iHttpClientAuthentication)
        error = KErrNone;
    return error;
    }

/**
 * Constructor
 */
CHttpService::CHttpService()
: iHttpServiceStruct(NULL)
	{
	}
	
void CHttpService::ConstructL()
	{
    iHttpServiceStruct = new(ELeave) CHttpServiceStruct();
    iHttpServiceStruct->iHttpSession.OpenL();
    iHttpServiceStruct->iSessionHeaders = iHttpServiceStruct->iHttpSession.RequestSessionHeadersL();
    SetMaxConnections(KMaxNoOfConnections);
    SetMaxTransactionsToPipeline(KMaxTransToPipeline);    
	}

/**
 * returns the CHttpNetworkConnection instance created by the framework.
 * The class can be used to set the connection properties.
 *
 * @publishedAll
 * @prototype 
 */
EXPORT_C CHttpNetworkConnection* CHttpService::HttpNetworkConnection()
    {
    CHttpNetworkConnection *httpNetworkConn = CHttpNetworkConnection::New();
    httpNetworkConn->SetHttpService(this);
    return httpNetworkConn;
    }

void CHttpService::RemoveUnwantedFilters()
    {
    THTTPFilterRegistration filterInfo;
    RStringPool stringPool = iHttpServiceStruct->iHttpSession.StringPool();

    RHTTPFilterCollection filterArray = iHttpServiceStruct->iHttpSession.FilterCollection();
    THTTPFilterIterator iter = filterArray.Query();
    const TStringTable& st = RHTTPSession::GetTable();
  
    iter.First();

    while (!iter.AtEnd())
        {
        // Get next filter registration info
        filterInfo = iter();
        RStringF filterName = stringPool.StringF(filterInfo.iName);
        switch(filterName.Index(st))
            {
            //dont remove these filters
            case HTTP::EClient:
            case HTTP::EProtocolHandler:
            case HTTP::EValidation:
            case HTTP::ERedirect:
            case HTTP::EHttpConnectFilter:
                break;
            //anything other than above, remove.    
            default:
                filterArray.RemoveFilter(filterName);
             }
        ++iter;
        }
    }
