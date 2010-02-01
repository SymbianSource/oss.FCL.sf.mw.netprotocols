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

#ifndef SESSNOTF_H
#define SESSNOTF_H

#include <e32base.h>
#include "obsrvr.h"
#include <centralrepository.h>
#include "clientrequest.h"

using namespace NCentralRepositoryConstants;

class CSessionNotifier : public CBase, public MObserver
	{
public:
	CSessionNotifier();
	~CSessionNotifier();

	TInt AddRequest(TUint32 aId, TClientRequest aMessage);
	TInt AddRequest(TUint32 aPartialId, TUint32 aIdMask, TClientRequest aMessage);
	TInt CancelRequest(TUint32 aId);
	TInt CancelRequest(TUint32 aPartialId, TUint32 aIdMask);
	TInt CancelAllRequests();

	void IdReportingOn();
	void IdReportingOff();

private:
	void Notify(TUint32 aId);

private:
	TBool iIdReportingOn;

	struct SRequest
		{
		TUint32 id;
		TClientRequest msg;
		};
	RArray<SRequest> iRequests;

	struct SGroupRequest
		{
		TUint32 partialId;
		TUint32 idMask;
		TClientRequest msg;
		};
	RArray<SGroupRequest> iGroupRequests;

	enum { KArrayGranularity=1 };
	};

#endif // SESSNOTF_H
