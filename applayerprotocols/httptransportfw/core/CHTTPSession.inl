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

#ifndef __CHTTPSESSION_INL__
#define __CHTTPSESSION_INL__


inline CHTTPManager& CHTTPSession::Manager()
	{
	return *iManager;
	}

inline const CHTTPManager& CHTTPSession::Manager() const
	{
	return *iManager;
	}

inline RArray<THTTPFilterRegistration>& CHTTPSession::FilterQueue()
	{
	return iFilterQueue;
	}

inline const RArray<THTTPFilterRegistration>& CHTTPSession::FilterQueue() const
	{
	return iFilterQueue;
	}


inline TInt CHTTPSession::NextFilterHandle()
	{
	return ++iNextFilterHandle;
	}

inline TInt CHTTPSession::NextTransactionID()
	{
	return ++iTransCount;
	}

inline CHeaderCodec* CHTTPSession::Codec() const
	{
	if (iProtocolHandler)
		return iProtocolHandler->Codec();
	else
		return NULL;
	}


inline RHTTPConnectionInfo CHTTPSession::ConnectionInfo() const
	{
	RHTTPConnectionInfo c;
	c.iImplementation = iConnectionInfo;
	return c;
	}

inline RHTTPSession CHTTPSession::Handle()
	{
	RHTTPSession r;
	r.iImplementation = this;
	return r;
	}

inline TBool CHTTPSession::CanAddFilters() const
	{
	// No transactions or the only transaction is the hidden one being used to process session events
	TInt numTransactions = iTransactions.Count();
	return (numTransactions == 0 || (numTransactions == 1 && iTransactions[0] == iSessionEventTransaction));
	}

// Returns ETrue if transactions should block.
inline TBool CHTTPSession::IsBlocking() const
	{
	return iBlockingState != ENormal;
	}

// Indicates that a transaction has blocked.
inline void CHTTPSession::TransactionHasBlocked()
	{
	iBlockingState = EHasBlocked;
	}

// Indicates that transactions should block until further notice
inline void CHTTPSession::Block()
	{
	iBlockingState = EShouldBlock;
	}

inline TInt CHTTPSession::SessionServerCert(TCertInfo& aServerCert)
	{
	if(iProtocolHandler)
		return iProtocolHandler->SessionServerCert(aServerCert);
	else
		return KErrNotFound;
	}

inline const CCertificate*  CHTTPSession::SessionServerCert()
	{
	if(iProtocolHandler)
		return iProtocolHandler->SessionServerCert();
	else
		return NULL;
	}


inline TInt CHTTPSession::TransactionServerCert(TCertInfo& aServerCert, RHTTPTransaction aTransaction)
	{
	if(iProtocolHandler)
		return iProtocolHandler->TransactionServerCert(aServerCert, aTransaction);
	else
		return KErrNotFound;
	}

inline const CCertificate*  CHTTPSession::TransactionServerCert(RHTTPTransaction aTransaction)
	{
	if(iProtocolHandler)
		return iProtocolHandler->TransactionServerCert(aTransaction);
	else
		return NULL;
	}

inline TInt CHTTPSession::FindTransaction(CTransaction* aTransaction)
	{
	TInt count = iTransactions.Count();
	while(count > 0 )
		{
		if( aTransaction == iTransactions[count - 1] )
			{
			return count;
			}
		count--;
		}
	return KErrNotFound;	
	}

#endif // __CHTTPSESSION_INL__
