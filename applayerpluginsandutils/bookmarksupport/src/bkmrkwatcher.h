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

#ifndef __BKMRKWATCHER_H__
#define __BKMRKWATCHER_H__

#include <centralrepository.h>

enum TBkmrkWatcherType
	{
	EBkmrkWatcherDatabase,
	EBkmrkWatcherFolder,
	EBkmrkWatcherBookmark,
	EBkmrkWatcherIcon
	};

/**
The interface for observers of the repository watchers. The repository watchers are active objects that 
get notified when changes occur in the central repository. The watcher sends a notification to its 
observer through this interface
*/
class MBkmrkWatcherObserver
	{
public:
	virtual void BWONotify(TBkmrkWatcherType aType) = 0;
	};


/**
This watcher class is responsible for propagating notifications from the central repository back to the
bookmark database. 
*/
class CBkmrkWatcher: public CActive
	{
public:
    CBkmrkWatcher(TBkmrkWatcherType aType, MBkmrkWatcherObserver& aObserver, CRepository& aRepository);
    ~CBkmrkWatcher();
    void WatchL();
protected:
    void RunL();
    void DoCancel();
private:
	TBkmrkWatcherType		iType;
	MBkmrkWatcherObserver&	iObserver;
	CRepository&			iRepository;
    };

#endif //__BKMRKWATCHER_H__
