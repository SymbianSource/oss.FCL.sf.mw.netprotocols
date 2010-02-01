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

#include "sessnotf.h"

CSessionNotifier::CSessionNotifier() :
	iIdReportingOn(ETrue), iRequests(KArrayGranularity, _FOFF(SRequest, id))
	{
	}

CSessionNotifier::~CSessionNotifier()
	{
	CancelAllRequests();
	}

void CSessionNotifier::Notify(TUint32 aId)
	{
	SRequest s;
	s.id = aId;

	// Check if iIdReportingOn is True. If  it's false return KUnspecifiedKey, otherwise return
	// the individual key. One exception to this is where the individual key is KRequestPending.
	// To prevent problems in AO's using key notifications KUnspecifiedKey will be returned to
	// notify for changes to this key.
	TUint32 reportedId = (iIdReportingOn  && (aId != static_cast<TUint32>(KRequestPending))) ? aId : KUnspecifiedKey;
	
	TInt i = iRequests.FindInUnsignedKeyOrder(s);
	if(i>=0)
		{
		iRequests[i].msg.Complete(reportedId);
		iRequests.Remove(i);
		}

	for(i=iGroupRequests.Count()-1; i>=0; i--)
		{
		SGroupRequest& s = iGroupRequests[i];
		if((aId & s.idMask)==(s.partialId & s.idMask))
			{
			s.msg.Complete(reportedId);
			iGroupRequests.Remove(i);
			}
		}
	}

TInt CSessionNotifier::AddRequest(TUint32 aId, TClientRequest aMessage)
	{
	SRequest s;
	s.id = aId;
	s.msg = aMessage;

	return iRequests.InsertInUnsignedKeyOrder(s);
	}

TInt CSessionNotifier::AddRequest(TUint32 aPartialId, TUint32 aIdMask, TClientRequest aMessage)
	{
	SGroupRequest s;
	s.partialId = aPartialId;;
	s.idMask = aIdMask;
	s.msg = aMessage;

	return iGroupRequests.Append(s);
	}

TInt CSessionNotifier::CancelRequest(TUint32 aId)
	{
	SRequest s;
	s.id = aId;
	
	TInt r = iRequests.FindInUnsignedKeyOrder(s);
	if(r>=0)
		{
		iRequests[r].msg.Complete(KUnspecifiedKey);
		iRequests.Remove(r);
		}

	return r;
	}

TInt CSessionNotifier::CancelRequest(TUint32 aPartialId, TUint32 aIdMask)
	{
	TInt n = 0;
	for(TInt i=iGroupRequests.Count()-1; i>=0; i--)
		{
		SGroupRequest& s = iGroupRequests[i];
		if(aPartialId==s.partialId && aIdMask==s.idMask)
			{
			n++;
			s.msg.Complete(KUnspecifiedKey);
			iGroupRequests.Remove(i);
			}
		}

	return n>0 ? KErrNone : KErrNotFound;
	}

TInt CSessionNotifier::CancelAllRequests()
	{
	TInt n = iRequests.Count();
	while(n-->0)
		iRequests[n].msg.Complete(KUnspecifiedKey);

	iRequests.Reset();

	n = iGroupRequests.Count();
	while(n-->0)
		iGroupRequests[n].msg.Complete(KUnspecifiedKey);

	iGroupRequests.Reset();

	return KErrNone;
	}

void CSessionNotifier::IdReportingOn()
	{
	iIdReportingOn = ETrue;
	}

void CSessionNotifier::IdReportingOff()
	{
	iIdReportingOn = EFalse;
	}
