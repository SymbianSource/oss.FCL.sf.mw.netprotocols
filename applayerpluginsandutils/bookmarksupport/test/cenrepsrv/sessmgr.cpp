// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>
#include "sessmgr.h"
#include "srvparams.h"
#include "srvsess.h"

CSessionManager* CSessionManager::NewLC()
	{
	CSessionManager* s = new(ELeave) CSessionManager();
	CleanupStack::PushL(s);
	s->ConstructL();
	return s;
	}

CSessionManager::CSessionManager() : 
	CServer2(KServerPriority, ESharableSessions)
	{}

void CSessionManager::ConstructL()
	{
	StartL(KServerName);
	}

CSession2* CSessionManager::NewSessionL(
	const TVersion& aVersion, const RMessage2&) const
	{
	// Test if the version specifies a repository session
	const TVersion KRepositoryVersion(
		KServerMajorVersion, KServerMinorVersion, KServerBuildVersion);
 	TBool r = User::QueryVersionSupported(aVersion, KRepositoryVersion);
	if(r)
		return new(ELeave) CServerSession();

	//the version is not supported
	User::Leave(KErrNotSupported);
	return 0; //compiler, be quiet!
	}
