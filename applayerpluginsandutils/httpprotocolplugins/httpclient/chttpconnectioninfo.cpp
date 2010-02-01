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

#include "chttpconnectioninfo.h"

CHttpConnectionInfo* CHttpConnectionInfo::NewL(RStringPool aStringPool, const TDesC8& aHost, TUint16 aPort)
	{
	CHttpConnectionInfo* self = new (ELeave) CHttpConnectionInfo(aStringPool);
	CleanupStack::PushL(self);
	self->ConstructL(aHost, aPort);
	CleanupStack::Pop(self);
	return self;
	}

CHttpConnectionInfo::~CHttpConnectionInfo()
	{
	iHost.Close();
	}	
	
	
void CHttpConnectionInfo::ConstructL(const TDesC8& aHost, TUint16 aPort)
	{
	iHost = iStringPool.OpenFStringL(aHost);
	iPort = aPort;
	}
	

TBool CHttpConnectionInfo::HostAndPortMatches(const CHttpConnectionInfo& aConnectionInfo) const
	{
	if( iHost != aConnectionInfo.iHost ||
		iPort != aConnectionInfo.iPort
		)
		return EFalse;
	return ETrue;
	}
