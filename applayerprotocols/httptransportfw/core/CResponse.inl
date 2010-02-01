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

#ifndef __CRESPONSE_INL__
#define __CRESPONSE_INL__

inline CResponse* CResponse::NewL(CTransaction* aTransaction)
	{
	CResponse* that = new (ELeave) CResponse;
	CleanupStack::PushL(that);
	that->ConstructL(aTransaction);
	CleanupStack::Pop(that);
	return that;
	}

inline CResponse::CResponse()
	{
	}

inline const TInt& CResponse::Status() const
	{
	return iStatus;
	}
inline TInt& CResponse::Status()
	{
	return iStatus;
	}
inline RStringF& CResponse::StatusString()
	{
	return iStatusString;
	}
inline const RStringF& CResponse::StatusString() const
	{
	return iStatusString;
	}
inline TVersion& CResponse::Version()
	{
	return iVersion;
	}
inline const TVersion& CResponse::Version() const
	{
	return iVersion;
	}

inline RHTTPResponse CResponse::Handle()
	{
	RHTTPResponse r;
	r.iImplementation = this;
	return r;
	}



#endif // __CRESPONSE_INL__
