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

#ifndef OBSRVR_H
#define OBSRVR_H

#include <e32base.h>

class MObserver
	{
public:
	virtual void Notify(TUint32 aId) = 0;
	};

class CObservable : public CBase
	{
public:
	CObservable();
	~CObservable();

	TInt AddObserverL(MObserver& aObserver);
	TInt RemoveObserver(MObserver& aObserver);
	void Notify(TUint32 aVal) const;
	
protected:
	TBool iNotificationState;//Notification can be made if iNotificationState is non-zero, 
	                         //otherwise the notification is disabled

private:
	RPointerArray<MObserver> iObservers;
	TInt iRefCount;
	};

#endif // OBSRVR_H
