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
#include <e32std.h>
#include <http/rhttpheaders.h>
#include <http/mhttpfiltercreationcallback.h>
#include <httperr.h>
#include <httpstringconstants.h>
#include <http/rhttpsession.h>// Class signature

// Local includes
#include "CHTTPManager.h"
#include "CTransaction.h"
#include "CHTTPSession.h"


EXPORT_C void RHTTPSession::OpenL()
	{
	__ASSERT_DEBUG(iImplementation == 0, 
				   HTTPPanic::Panic(HTTPPanic::ESessionAlreadyOpen ));

	iImplementation = CHTTPSession::NewL();
	}

EXPORT_C void RHTTPSession::OpenL(const TDesC8& aProtocol)
	{
	__ASSERT_DEBUG(iImplementation == 0, 
				   HTTPPanic::Panic(HTTPPanic::ESessionAlreadyOpen ));

	iImplementation = CHTTPSession::NewL(aProtocol);
	}

EXPORT_C void RHTTPSession::OpenL(const TDesC8& aProtocol, MHTTPFilterCreationCallback* aSessionCallback)
	{
	__ASSERT_DEBUG(iImplementation == 0, 
				   HTTPPanic::Panic(HTTPPanic::ESessionAlreadyOpen ));

	iImplementation = CHTTPSession::NewL(aProtocol, aSessionCallback);
	CleanupClosePushL(*this);

	// If the session callback was supplied, call the callback function passing the filter iterator
	if( aSessionCallback != NULL )
		{
		aSessionCallback->ConfigureSessionFiltersL(iImplementation->FilterConfigIterator());
		}
	CleanupStack::Pop(this);
	}

EXPORT_C void RHTTPSession::ListAvailableProtocolsL(RPointerArray<HBufC8>& aProtocolArray)
	{
	CHTTPSession::ListAvailableProtocolsL(aProtocolArray);
	}

EXPORT_C void RHTTPSession::Close()
	{
	delete iImplementation;
	iImplementation = NULL;
	}


EXPORT_C void RHTTPSession::SetSessionEventCallback(MHTTPSessionEventCallback* aSessionEventCallback)
	{
	iImplementation->SetSessionEventCallback(aSessionEventCallback);
	}

EXPORT_C void RHTTPSession::SendSessionEventL(THTTPSessionEvent aStatus, THTTPSessionEvent::TDirection aDirection, 
							 THTTPFilterHandle aStart)
	{
	iImplementation->SendSessionEventL(aStatus, aDirection, aStart);
	}

EXPORT_C void RHTTPSession::FailSessionEvent(THTTPFilterHandle aStart)
	{
	iImplementation->FailSessionEvent(aStart);
	}

EXPORT_C RStringPool RHTTPSession::StringPool() const
	{
	
	return iImplementation->Manager().StringPool();
	}

EXPORT_C RHTTPConnectionInfo RHTTPSession::ConnectionInfo() const
	{
	return iImplementation->ConnectionInfo();
	}

EXPORT_C RHTTPTransaction RHTTPSession::OpenTransactionL(const TUriC8& aURI, 
									 MHTTPTransactionCallback& aCallback, 
									 RStringF aMethod)
	{
	CTransaction* implementation = CTransaction::NewL(aCallback, aURI,
													  *iImplementation,
													  aMethod);
	return implementation->Handle();
	};

EXPORT_C RHTTPHeaders RHTTPSession::RequestSessionHeadersL()
	{
	return iImplementation->RequestSessionHeadersL();
	}

EXPORT_C RHTTPHeaders RHTTPSession::ResponseSessionHeadersL()
	{
	return iImplementation->ResponseSessionHeadersL();
	}


EXPORT_C const TStringTable& RHTTPSession::GetTable()
	{
	return HTTP::Table;
	}

EXPORT_C TInt RHTTPSession::ServerCert(TCertInfo& aServerCert)
	{
	return iImplementation->SessionServerCert(aServerCert);
	}

EXPORT_C const CCertificate* RHTTPSession::ServerCert()
	{
	return iImplementation->SessionServerCert();
	}	

EXPORT_C void RHTTPSession::ConnectL()
	{
	iImplementation->SendSessionEventL(THTTPSessionEvent::EConnect, THTTPSessionEvent::EOutgoing);
	}

EXPORT_C void RHTTPSession::DisconnectL()
	{
	iImplementation->SendSessionEventL(THTTPSessionEvent::EDisconnect, THTTPSessionEvent::EOutgoing);
	}

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS	
EXPORT_C void RHTTPSession::SetupDefaultProxyFromCommsDatL()
	{
	iImplementation->SetupDefaultProxyFromCommsDatL();
	}
#endif	//SYMBIAN_ENABLE_SPLIT_HEADERS

EXPORT_C void RHTTPSession::SetupHttpDataOptimiser(MHttpDataOptimiser& aHttpOptimiser)
 	{
 	iImplementation->SetupHttpDataOptimiser (aHttpOptimiser);
 	}
 
EXPORT_C MHttpDataOptimiser* RHTTPSession::HttpDataOptimiser()
 	{
 	return iImplementation->HttpDataOptimiser ();
 	}

