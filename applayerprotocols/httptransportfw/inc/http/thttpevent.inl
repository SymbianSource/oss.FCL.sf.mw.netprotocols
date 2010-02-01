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


inline THTTPEvent::THTTPEvent(TInt aStatus, TUint aUID)
: iStatus(aStatus), iUID(aUID), iIsSessionEventFlag(EFalse)
	{
	}

inline THTTPEvent::THTTPEvent(TStandardEvent aStatus)
: iStatus(aStatus), iUID(KHTTPUid), iIsSessionEventFlag(EFalse)
	{
	}

inline THTTPEvent::THTTPEvent(TTransactionEvent aStatus)
: iStatus(aStatus), iUID(KHTTPUid), iIsSessionEventFlag(EFalse)
	{
	}

inline THTTPEvent& THTTPEvent::operator=(TStandardEvent aStatus)
	{
	iUID = KHTTPUid;
	iStatus = aStatus;
	return *this;
	}

inline THTTPEvent& THTTPEvent::operator=(TTransactionEvent aStatus)
	{
	iUID = KHTTPUid;
	iStatus = aStatus;
	return *this;
	}

inline TBool THTTPEvent::operator==(THTTPEvent aThat) const
	{
	return (iStatus == aThat.iStatus && iUID == aThat.iUID);
	}

inline TBool THTTPEvent::operator!=(THTTPEvent aThat) const
	{
	return !(*this == aThat);
	}

inline TBool THTTPEvent::operator==(TStandardEvent aStatus) const
	{
	return (iStatus == aStatus && iUID == KHTTPUid);
	}

inline TBool THTTPEvent::operator!=(TStandardEvent aStatus) const
	{
	return !(*this == aStatus);
	}

inline TBool THTTPEvent::operator==(TTransactionEvent aStatus) const
	{
	return (iStatus == aStatus && iUID == KHTTPUid);
	}

inline TBool THTTPEvent::operator!=(TTransactionEvent aStatus) const
	{
	return !(*this == aStatus);
	}

inline TBool THTTPEvent::IsSessionEvent() const
	{
	return iIsSessionEventFlag;
	}

inline THTTPSessionEvent::THTTPSessionEvent(TInt aStatus, TUint aUID)
: THTTPEvent(aStatus, aUID)
	{
	iIsSessionEventFlag = ETrue;
	}

inline THTTPSessionEvent::THTTPSessionEvent(TStandardEvent aStatus)
: THTTPEvent(aStatus)
	{
	iIsSessionEventFlag = ETrue;
	}

inline THTTPSessionEvent::THTTPSessionEvent(TSessionEvent aStatus)
: THTTPEvent(aStatus)
	{
	iIsSessionEventFlag = ETrue;
	}

inline THTTPSessionEvent& THTTPSessionEvent::operator=(TSessionEvent aStatus)
	{
	iUID = KHTTPUid;
	iStatus = aStatus;
	return *this;
	}

inline TBool THTTPSessionEvent::operator==(TSessionEvent aStatus) const
	{
	return (iStatus == aStatus && iUID == KHTTPUid);
	}

inline TBool THTTPSessionEvent::operator!=(TSessionEvent aStatus) const
	{
	return !(*this == aStatus);
	}
