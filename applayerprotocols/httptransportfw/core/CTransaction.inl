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

#ifndef __CTRANSACTION_INL__
#define __CTRANSACTION_INL__

// These are only called in 1 place (from RHTTPTransaction) so they
// may as well be inlined
inline CTransaction::CTransaction(MHTTPTransactionCallback& aCallback, 
								  CHTTPSession& aSession)
		: CActive(KTransactionActivePriority),
		  iCallback(aCallback),
		  iSession(aSession),
		  iEventQueue(12)
	{
    iLink.iPrev = &iLink;
    iLink.iNext = &iLink;	
	};

inline CTransaction* CTransaction::NewL(MHTTPTransactionCallback& aCallback, 
										const TUriC8& aURI,
										CHTTPSession& aSession,
										RStringF aMethod)
	{
	CTransaction* that = new (ELeave) CTransaction(aCallback, aSession);
	CleanupClosePushL(*that);
	that->ConstructL(aURI, aMethod);
	CleanupStack::Pop(that);
	return that;
	};

inline RHTTPSession CTransaction::Session() const
	{
	return iSession.Handle();
	};

inline CHeaderCodec& CTransaction::Codec() const
	{
	return *iSession.Codec();
	}

inline CRequest& CTransaction::Request() const
	{
	return *iRequest;
	}
inline CResponse& CTransaction::Response() const
	{
	return *iResponse;
	}

inline TInt CTransaction::Id() const
	{
	return iId;
	}

inline RHTTPTransactionPropertySet CTransaction::PropertySet() const
	{
	return iPropertySet;
	}


inline CTransaction* CTransaction::Implementation(RHTTPTransaction aHandle)
	{
	return aHandle.iImplementation;
	}

inline MHTTPTransactionCallback& CTransaction::Callback() const
	{
	return iCallback;
	}

inline void CTransaction::Unblock()
	{
	if (iStatus.Int() == EBlocked || iStatus.Int() == ESuspended)
		{
		Activate(EFilter);
		}
	}

inline RHTTPTransaction CTransaction::Handle()
	{
	RHTTPTransaction handle;
	handle.iImplementation = this;
	return handle;
	}

inline TBool CTransaction::SyncMode() const
	{
	return iProcessingSyncEvent;
	}

inline void CTransaction::Fail(THTTPFilterHandle aStart)
	{
	CancelTransaction(aStart);
	if (iStatus.Int() == EExiting)
		return;
	SynchronousSendEvent(THTTPEvent::EUnrecoverableError, 
						 THTTPEvent::EIncoming,
						 aStart);
	if (iStatus.Int() == EExiting)
		return;
	SynchronousSendEvent(THTTPEvent::EFailed, 
						 THTTPEvent::EIncoming,
						 aStart);
	if (iStatus.Int() == EExiting)
		return;
	}

inline TInt CTransaction::ServerCert(TCertInfo& aServerCert)
	{
	return iSession.TransactionServerCert(aServerCert, this->Handle());
	}

inline const CCertificate*   CTransaction::ServerCert()
	{
	return iSession.TransactionServerCert( this->Handle());
	}

inline	TBool CTransaction::IsValid()
	{
	TInt err = iSession.FindTransaction(this);
	if(err != KErrNotFound)
		return ETrue;
	return EFalse;
	}
	
inline CProtTransaction* CTransaction::ProtocolTransaction()
    {
    return iProtocolTransaction;
    }
    
inline void CTransaction::SetProtocolTransaction(CProtTransaction* aTrans)
    {
    iProtocolTransaction = aTrans;
    }

inline void CTransaction::NextFilter()
    {
    if (EventAvailable())
        {
        if (CurrentEvent().iDirection == THTTPEvent::EOutgoing)
            iNextFilter--;
        else
            iNextFilter++;
        }
    }

inline const TEventRegistration& CTransaction::CurrentEvent() const
    {
    if (iProcessingSyncEvent)
        return iCurrentSyncEvent;
    else
        return iEventQueue[0];
    }

inline TBool CTransaction::EventAvailable() const
    {
    if (iProcessingSyncEvent)
        return ETrue;
    else
        return (iEventQueue.Count() != 0);
    }

inline void CTransaction::RemoveEvent()
    {
    if (!iProcessingSyncEvent && iEventQueue.Count())
        {
        iEventQueue.Remove(0);
        }
    else if (iProcessingSyncEvent)
        iProcessingSyncEvent = EFalse;
    }
#endif // __CTRANSACTION__INL__
