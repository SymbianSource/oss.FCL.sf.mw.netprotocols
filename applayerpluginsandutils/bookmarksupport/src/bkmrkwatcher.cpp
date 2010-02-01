// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Internal class for catching notifications from bookmark repositories.
// @internalComponent
// 
//

#include "bkmrkwatcher.h"

const TUint32 KAnyId = 0x00000000; // 0 are wildcards so when used as a mask this will match anything

CBkmrkWatcher::CBkmrkWatcher(TBkmrkWatcherType aType, MBkmrkWatcherObserver& aObserver, CRepository& aRepository) 
	: CActive(EPriorityStandard), iType(aType), iObserver(aObserver), iRepository(aRepository)
	{
	CActiveScheduler::Add(this);
	}

CBkmrkWatcher::~CBkmrkWatcher()
	{
	Cancel();
	}
	
void CBkmrkWatcher::WatchL()
	{
	if (IsActive())
		{
		return;
		}
	// request to be notified of repository changes
	User::LeaveIfError(iRepository.NotifyRequest(KAnyId, KAnyId, iStatus));
	SetActive();
	}

void CBkmrkWatcher::RunL()
	{
	// re-issue the watching request
	WatchL();
	// notify observer
	iObserver.BWONotify(iType);
	}
	
void CBkmrkWatcher::DoCancel()
	{
	// cancel request to the repository
	iRepository.NotifyCancel(KAnyId, KAnyId);
	}
