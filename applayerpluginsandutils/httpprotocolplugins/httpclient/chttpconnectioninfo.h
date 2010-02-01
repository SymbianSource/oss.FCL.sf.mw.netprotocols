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


#ifndef __CHTTPCONNECTIONINFO_H__
#define __CHTTPCONNECTIONINFO_H__

#include <e32base.h>
#include <stringpool.h>

class CHttpConnectionInfo : public CBase
	{
public:	

	enum THttpPersistent
		{
		EPersistent			= 0,
		ENonPersistent
		};

public:

	static CHttpConnectionInfo* NewL(RStringPool aStringPool, const TDesC8& aHost, TUint16 aPort);
	virtual ~CHttpConnectionInfo();
	
	inline void SetSecureState(TBool aIsSecure);
	inline void SetPersistentState(THttpPersistent aState);
	
	inline TBool IsSecure() const;
	inline TBool IsNonPersistent() const;

	inline const TDesC8& Host() const;
	inline TUint16 Port() const;
	
	TBool HostAndPortMatches(const CHttpConnectionInfo& aConnectionInfo) const;
	
private:

	inline CHttpConnectionInfo(RStringPool aStringPool);
	void ConstructL(const TDesC8& aHost, TUint16 aPort);
	
private:
    TUint16         iPort;
    TBool           iIsSecure;
    TBool           iIsNonPersistent;    	
	RStringPool		iStringPool;
	RStringF		iHost;	
	};

inline CHttpConnectionInfo::CHttpConnectionInfo(RStringPool aStringPool)
: iStringPool(aStringPool)
    {
    }

inline void CHttpConnectionInfo::SetSecureState(TBool aIsSecure)
    {
    iIsSecure = aIsSecure;
    }

inline void CHttpConnectionInfo::SetPersistentState(THttpPersistent aState)
    {
    iIsNonPersistent = (aState == ENonPersistent);
    }

inline TBool CHttpConnectionInfo::IsSecure() const
    {
    return iIsSecure;
    }
    
inline TBool CHttpConnectionInfo::IsNonPersistent() const
    {
    return iIsNonPersistent;
    }
    
inline const TDesC8& CHttpConnectionInfo::Host() const
    {
    return iHost.DesC();
    }
    
inline TUint16 CHttpConnectionInfo::Port() const
    {
    return iPort;
    }

#endif	// __CHTTPCONNECTIONINFO_H__
