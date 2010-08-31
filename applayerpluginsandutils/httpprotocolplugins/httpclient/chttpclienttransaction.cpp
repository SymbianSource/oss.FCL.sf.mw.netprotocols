// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <http.h>
#include "chttpclienttransaction.h"
#include "chttprequestcomposer.h"
#include "chttpresponseparser.h"
#include "chttpconnectionmanager.h"

CHttpClientTransaction* CHttpClientTransaction::NewL(RHTTPTransaction aTransaction)
	{
	CHttpClientTransaction* self = new (ELeave) CHttpClientTransaction(aTransaction);
	return self;	
	}
	
CHttpClientTransaction::~CHttpClientTransaction()
	{
	}

CHttpClientTransaction::CHttpClientTransaction(RHTTPTransaction aTransaction)
: CProtTransaction(aTransaction), iStringTable(RHTTPSession::GetTable())
	{
	}
	
void CHttpClientTransaction::SetConnectionManager(CHttpConnectionManager& aConnectionManager)
	{
	__ASSERT_DEBUG( iConnectionManager == NULL, User::Invariant() );
	
	// A connection manager has been supplied - add this object to it.	
	iConnectionManager = &aConnectionManager;
	}

CHttpConnectionManager* CHttpClientTransaction::ConnectionManager() const
	{
	return iConnectionManager;
	}

void CHttpClientTransaction::RemoveConnectionManager()
	{
	iConnectionManager = NULL;
	}
	
/*
 *	Methods from CProtTransaction
 */

void CHttpClientTransaction::CreateTxDataL()
	{
	__ASSERT_DEBUG( iConnectionManager != NULL, User::Invariant() );

	delete iTxData;
	iTxData = NULL;
	iTxData = CHttpRequestComposer::NewL(*this, *iConnectionManager);
	}
	
void CHttpClientTransaction::CreateRxDataL(MRxDataObserver& aObserver)
	{
	__ASSERT_DEBUG( iConnectionManager != NULL, User::Invariant() );

	delete iRxData;
	iRxData = NULL;
	iRxData = CHttpResponseParser::NewL(*this, aObserver, *iConnectionManager);
	}
	
void CHttpClientTransaction::IncRetryCount ()
	{
	++iRetryCount;		
	}

TBool CHttpClientTransaction::RetryNeeded ()
	{
	const TInt KMaxRetryCount = 3;	
	return ( ( iTransactionState != ECancelled ) && 
			( iTransactionState != ECompleted ) && 
			( iRetryCount < KMaxRetryCount) );
	}
	
TBool CHttpClientTransaction::NeedDisconnectNotification () const
    {
    TBool needDisconnectNotification = EFalse;
	RHTTPTransaction myTransaction = Transaction (); 
	RHTTPSession mySession = myTransaction.Session ();
	RStringPool myStringPool ( mySession.StringPool () );
	RStringF notifyOnDisconnect = myStringPool.StringF( HTTP::ENotifyOnDisconnect, iStringTable );

	RHTTPPropertySet transactionProperties = myTransaction.PropertySet();
	RHTTPPropertySet sessionProperties = mySession.ConnectionInfo();
    
	THTTPHdrVal hdrVal;
	
	// Check the transaction properties for the disconnect notification
	// If it is set then it should be used.
	if ( transactionProperties.Property ( notifyOnDisconnect, hdrVal ) || sessionProperties.Property ( notifyOnDisconnect, hdrVal ) )	
		{
		__ASSERT_DEBUG( hdrVal.Type() == THTTPHdrVal::KStrFVal, User::Invariant() );
		
		needDisconnectNotification = ( hdrVal.StrF().Index(iStringTable) == HTTP::EEnableDisconnectNotification );
		}
	return needDisconnectNotification;	
	}
