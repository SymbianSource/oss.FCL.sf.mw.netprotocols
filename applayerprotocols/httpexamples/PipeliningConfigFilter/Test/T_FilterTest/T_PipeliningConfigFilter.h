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

#ifndef __T_PIPELININGCONFIGFILTER_H__
#define __T_PIPELININGCONFIGFILTER_H__

#include <e32base.h>
#include <http/mhttpfilter.h>
#include <http/rhttpsession.h>
#include <stringpool.h>


class CPipeliningConfigFilterTest : public CBase, 
						  			public MHTTPFilter
	{
public:
	static CPipeliningConfigFilterTest* NewL(RHTTPSession aSession, TInt aNumberOfTransactions, TBool aPipelinedTransactions);
	virtual ~CPipeliningConfigFilterTest();
	TBool GetTestResult();


private: // methods from MHTTPFilterBase
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);


private: // methods from MHTTPFilter
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle aHandle);
	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle aHandle);
	
	CPipeliningConfigFilterTest(TInt aNumberOfTransactions, TBool aPipelinedTransactions);
	void ConstructL(RHTTPSession aSession);
	void CheckForPipeliningSupport(RHTTPTransaction aTransaction);


private:
	TInt			iLoadCount;
	RStringPool		iStringPool;
	RHTTPSession	iSession;
	TInt			iNumberOfTransactions;
	TBool			iPipelinedTransactions;
	TBool			iTestFailed;
	RStringF		iFilterName;
	TInt			iCurrentTransactionNumber;
	};
	
#endif // __T_PIPELININGCONFIGFILTER_H__
