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

#ifndef SRVSESS_H
#define SRVSESS_H

#include "sessmgr.h"
#include "sessnotf.h"
#include "srvrepos.h"
#include "clientrequest.h"
#include "srvPerf.h"

class CSessionManager;

class CServerSession : public CSession2
	{
public:
	~CServerSession();

private:
	enum { KDontComplete=1 };
	typedef TInt (CServerSession::*TServerFunc)(const TClientRequest&);

	TInt GeneralOperationsL(const TClientRequest& aMessage, TServerFunc aFunc);
	TInt ReadOperationsL(const TClientRequest& aMessage, TServerFunc aFunc);
	TInt WriteOperationsL(const TClientRequest& aMessage, TServerFunc aFunc);
	TInt ResetOperationsL(const TClientRequest& aMessage, TServerFunc aFunc);

	TInt InitialiseL(const TClientRequest& aMessage);
	TInt CreateIntL(const TClientRequest& aMessage);
	TInt CreateRealL(const TClientRequest& aMessage);
	TInt CreateStringL(const TClientRequest& aMessage);
	TInt DeleteL(const TClientRequest& aMessage);
	TInt DeleteRangeL(const TClientRequest& aMessage);
	TInt GetIntL(const TClientRequest& aMessage);
	TInt SetIntL(const TClientRequest& aMessage);
	TInt GetRealL(const TClientRequest& aMessage);
	TInt SetRealL(const TClientRequest& aMessage);
	TInt GetStringL(const TClientRequest& aMessage);
	TInt SetStringL(const TClientRequest& aMessage);
	TInt FindL(const TClientRequest& aMessage);
	TInt FindEqIntL(const TClientRequest& aMessage);
	TInt FindEqRealL(const TClientRequest& aMessage);
	TInt FindEqStringL(const TClientRequest& aMessage);
	TInt FindNeqIntL(const TClientRequest& aMessage);
	TInt FindNeqRealL(const TClientRequest& aMessage);
	TInt FindNeqStringL(const TClientRequest& aMessage);
	TInt GetFindResultL(const TClientRequest& aMessage);
	TInt NotifyRequestCheck(const TClientRequest& aMessage);
	TInt NotifyRequest(const TClientRequest& aMessage);
	TInt NotifyCancel(const TClientRequest& aMessage);
	TInt NotifyCancelAll(const TClientRequest& aMessage);
	TInt GroupNotifyRequest(const TClientRequest& aMessage);
	TInt GroupNotifyCancel(const TClientRequest& aMessage);
	TInt ResetL(const TClientRequest& aMessage);
	TInt ResetAllL(const TClientRequest& aMessage);
	TInt TransactionStart(const TClientRequest& aMessage);
	TInt TransactionCancel(const TClientRequest& aMessage);
	TInt TransactionCommitL(const TClientRequest& aMessage);
	TInt MoveL(const TClientRequest& aMessage) ;
	TInt TransactionStateL(const TClientRequest& aMessage);
	TInt TransactionFail(const TClientRequest& aMessage);

private:
	virtual void ServiceError(const RMessage2 &aMessage, TInt aError);
	virtual void ServiceL(const RMessage2& aMessage);
	template <class T> TInt FindValueL(const TClientRequest& aMessage, const T& aVal, TComparison aComparison=EEqual);
	inline CSessionManager& Server();	
	TInt CheckPolicy(const TClientRequest& msg,const TSecurityPolicy& aPolicy,
					const char *aDiagnostic);

#if defined(__CENTREP_SERVER_PERFTEST__) || defined(__CENTREP_SERVER_MEMTEST__) || defined(__CENTREP_SERVER_CACHETEST__)
	TInt GetSetParameters(const TClientRequest& aMessage);
#endif

#ifdef __CENTREP_SERVER_PERFTEST__
	void DoEventStart();
	void DoEventEnd(TUint aEventId);
#endif

private:
	CServerRepository iRepository;
	CSessionNotifier iNotifier;
	TBool iInitialised;
	RArray<TUint32> iFindResult;

#ifdef __CENTREP_SERVER_PERFTEST__
	TUint32 iStartTick;
	TUint32 iRepositoryUid;
	TCentRepPerfTest iPerfData;
#endif
	};

#endif // SRVSESS_H
