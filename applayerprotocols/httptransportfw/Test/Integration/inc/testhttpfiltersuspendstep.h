// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains declaration of CTestHttpBufferSizeStep class
// @internalAll
// 
//

#ifndef __TEST_HTTP_FILTER_SUSPEND_STEP_H__
#define __TEST_HTTP_FILTER_SUSPEND_STEP_H__
// User Include
#include "testhttpbasestep.h"
//#include "httpdebug.h"
#include "httptestutils.h"

// Literal constant defined for identifying step name
_LIT(KTestHttpFilterSuspendStep, "TestHttpFilterSuspendStep");

/**
This is the base test step from which all other test steps are derived
to test the HTTP Library.
@internalTechnology
@test
*/
class CTestHttpFilterSuspendStep : public CTestHttpBaseStep
	{
public:
    CTestHttpFilterSuspendStep();
	virtual ~CTestHttpFilterSuspendStep();
	
private:
    void StartClientL();
    void InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod);
	
	// methods from MHTTPTransactionCallback
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	MHTTPDataSupplier* iRespBody;
	TInt iChunkSize;
	};

//
class CTestFilter1 : public MHTTPFilter 
	{
public:
	static CTestFilter1* NewL(TInt aIndex);
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle);
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle);
	TInt iIndex;
	CHTTPTestUtils *iUtils;		
	
private:
	CTestFilter1(TInt aIndex)
		{ 
		iUtils = CHTTPTestUtils::NewL(KTestHttpFilterSuspendStep());	
		iIndex = aIndex; 				
    	}
	};

//	
//
class CTestFilter : public MHTTPFilter, public CTimer
	{
public:
	static CTestFilter* NewL(TInt aIndex);
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

	virtual void MHFLoad(RHTTPSession aSession, THTTPFilterHandle);
	virtual void MHFUnload(RHTTPSession aSession, THTTPFilterHandle);
	void RunL();

	TInt iIndex;	
	RHTTPTransaction iTrans;
	CHTTPTestUtils *iTestUtils;
	
private:
	   
	CTestFilter(TInt aIndex):CTimer(CActive::EPriorityIdle)
		{ 	
    	iTestUtils = CHTTPTestUtils::NewL(KTestHttpFilterSuspendStep());	
		iIndex = aIndex; 		
		CTimer::ConstructL();
    	CActiveScheduler::Add(this);     	
    	}
	};

//	

#endif		// __TEST_HTTP_FILTER_SUSPEND_STEP_H__
