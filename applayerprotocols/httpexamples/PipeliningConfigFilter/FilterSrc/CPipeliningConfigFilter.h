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

#ifndef		__CPIPELININGCONFIGFILTER_H__
#define		__CPIPELININGCONFIGFILTER_H__

#include <e32base.h>
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <http/cecomfilter.h>

class CDomainList;

class CPipeliningConfigFilter : public CEComFilter, public MHTTPFilter
	{
public:
	static CEComFilter* InstallFilterL(TAny* aSession);	
	~CPipeliningConfigFilter();

public:	// Methods from MHTTPFilterBase
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

public:	// Methods from MHTTPFilter
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);

private:
	CPipeliningConfigFilter();
	void ConstructL(RHTTPSession aSession);
	
	void ProcessSubmitTransactionL(RHTTPTransaction& aTransaction);
	void DisablePipeliningL(RHTTPTransaction& aTransaction);
	void ProcessRulesL(RHTTPTransaction& aTransaction);
	void AddDomainToListL(RHTTPTransaction& aTransaction);
	
	// The rules
	TBool IsHTTP11(RHTTPTransaction& aTransaction);
	TBool IsDIGESTAuthenticationL(RHTTPTransaction& aTransaction);

private:
	RHTTPSession iSession;
	RStringF iFilterName;
	TInt iLoadCount;
	RStringPool iStringPool;
	CDomainList* iDomainList;
	};

#endif // 	__CPIPELININGCONFIGFILTER_H__
