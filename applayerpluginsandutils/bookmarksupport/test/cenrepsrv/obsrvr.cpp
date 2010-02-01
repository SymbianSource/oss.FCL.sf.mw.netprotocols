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

#include "obsrvr.h"
#include "panic.h"

CObservable::CObservable() :
	iNotificationState(ETrue)
	{
	}
	
CObservable::~CObservable()
	{
	iObservers.Close();
	}

TInt CObservable::AddObserverL(MObserver& aObserver)
	{
	User::LeaveIfError(iObservers.Append(&aObserver));
	return iObservers.Count();
	}

TInt CObservable::RemoveObserver(MObserver& aObserver)
	{
	TInt i = iObservers.Find(&aObserver);
	__ASSERT_ALWAYS(i>=0, Panic(ERepositoryNotFound));
	iObservers.Remove(i);
	return iObservers.Count();
	}

void CObservable::Notify(TUint32 aVal) const
	{
	if(iNotificationState)
		{
		for(TInt i=iObservers.Count()-1; i>=0; --i)
			{
			iObservers[i]->Notify(aVal);
			}
		}
	}
