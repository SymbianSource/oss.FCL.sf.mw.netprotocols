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

#ifndef __CHTTPSERVICE_H__
#define __CHTTPSERVICE_H__

#include <e32std.h>
#include <http.h>
#include <in_sock.h>
#include "httpheaderiter.h"
class CHttpNetworkConnection;
/**
 * CHttpService represent a handle to the HTTP service instance for a set of client 
 * HTTP transactions[a request and its equivalent response]. The application can 
 * specify connection preferences[proxy, max. no of connections allowed in a single
 * HTTP service instance], pipelining, setting request headers that applies to all 
 * transactions that the CHttpService holds etc...
 * 
 * @publishedAll
 * @prototype 
 */
class CHttpClientAuthentication;
class MHTTPServiceAuthentication; 
class CHttpService : public CBase
	{
	friend class CHttpClientTransactionImpl;	
	friend class CHttpNetworkConnection;
	public:
	
	IMPORT_C static CHttpService* NewL();
	IMPORT_C ~CHttpService();
	
	IMPORT_C TInt String(TInt aStringId, TPtrC8& aPtr);
	
	IMPORT_C TInt SetProxy(const TDesC8& aProxyAddress);
	IMPORT_C const TDesC8& ProxyAddress() const;
	
	IMPORT_C void SetMaxConnections(TInt aValue);
	IMPORT_C TInt MaxConnections() const;
	
	IMPORT_C void SetMaxTransactionsToPipeline(TInt aValue);
	IMPORT_C TInt MaxTransactionsToPipeline() const;
	
	IMPORT_C TInt AddRequestHeader(TInt aHeaderId, const THttpHeaderValueVariant& aHeaderValue);		
	IMPORT_C TInt AddCustomRequestHeader(const TDesC8& aHeaderName, const TDesC8& aHeaderValue);
	
	IMPORT_C TInt SetAuthentication(MHTTPServiceAuthentication* aCallback);
	
	IMPORT_C CHttpNetworkConnection* HttpNetworkConnection();
	
	private:
	CHttpService();
	void ConstructL();
	void RemoveUnwantedFilters();
	
	NONSHARABLE_CLASS(CHttpServiceStruct) : public CBase
		{
		public:
		    ~CHttpServiceStruct();
		RHTTPSession                  iHttpSession;
		RHTTPHeaders                  iSessionHeaders; 
		CHttpClientAuthentication*    iHttpClientAuthentication;
		};
	private:
	   RHTTPSession Session()
	       {
	       return iHttpServiceStruct->iHttpSession;
	       }
	private:
	
	
	   CHttpServiceStruct*         iHttpServiceStruct; // Implementation struct
	};

#endif // __CHTTPSERVICE_H__
