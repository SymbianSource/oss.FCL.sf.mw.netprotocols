// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Author:	Franco Bellu
// this file Contains the following classes: ,	CHttpTestBase, CHttpTestEngine
// 
//

#ifndef __HTTPTESTCORE_H__
#define __HTTPTESTCORE_H__

#include <e32std.h>
#include <e32base.h>
#include <e32cons.h>
#include <f32file.h>
#include <http.h>
#include <uri8.h>
#include "httptestutils.h"
#include "t_detectkey.h" 



const TInt KErrTestInterrupted = -7398;


// An error code that is considered success by the heap failure
// tests. It's used in situations where the heap failures will cause
// the test to deviate from the 'normal' course.
const TInt KErrTestFailed = -7399;


class CDetectKeyPress;

//##ModelId=3C4C4C40025F
class CTestScheduler : public CActiveScheduler
	{
	//##ModelId=3C4C4C40026A
	IMPORT_C virtual void Error(TInt anError) const;
	};

class CHttpTestEngine;


/**
 * This class represents a general test case to be run.It provides 
 * common functionalities for the derived testcases
 */

//##ModelId=3C4C4C410260
class CHttpTestBase : public CActive
	{
public:
	//##ModelId=3C4C4C410315
	IMPORT_C TInt ExpectedStatusCode();
	IMPORT_C CHttpTestBase();
	//##ModelId=3C4C4C410314
	IMPORT_C void BeginTest();
	//##ModelId=3C4C4C410300
	inline void SetEngine(CHttpTestEngine* aEngine);
	//##ModelId=3C4C4C4102EE
	inline void SetLeakTests(TInt aNumberOfLeakTests, TInt aFirstLeakIteration = 0);
	//##ModelId=3C4C4C4102ED
	virtual const TDesC& TestName()=0;
protected:
	//##ModelId=3C4C4C4102EC
	IMPORT_C virtual void RunL();
	//##ModelId=3C4C4C4102E2
	IMPORT_C virtual TInt RunError(TInt aErr);
	//##ModelId=3C4C4C4102D9
	IMPORT_C void CompleteOwnRequest();
	//##ModelId=3C4C4C4102D8
	virtual void DoRunL()=0;

private:
	//##ModelId=3C4C4C4102CE
	void DoLeakTestsL();

protected:
	//##ModelId=3C4C4C4102C4
	CHttpTestEngine* iEngine;
	//##ModelId=3C4C4C4102B2
	RStringPool iMyStrP ; // all the tests need it to use strings
	//##ModelId=3C4C4C4102A8
	RStringPool iIpAddr ; // every test opening transaction needs it
	//##ModelId=3C4C4C41029C
	TInt iExpectedStatusCode;
	//##ModelId=3C4C4C410288
	TBool iTestFail;	// then it will leave
	//##ModelId=3C4C4C41027E
	TInt iLeakTests;	// the number of leak tests to do
	//##ModelId=3C4C4C41026D
	TInt iFirstLeakIteration;	// the iteration number of the first leak test to do
	};

/**
 * This class represents a transaction-based test case to be run.
 */

//##ModelId=3C4C4C440070
class CHttpTestTransBase : public CHttpTestBase, public MHTTPDataSupplier
	{
public:
	IMPORT_C CHttpTestTransBase();
	//##ModelId=3C4C4C4400C2
	IMPORT_C virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	//##ModelId=3C4C4C4400B8
	IMPORT_C virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	//##ModelId=3C4C4C4400B7
	IMPORT_C virtual TInt Reset() ;

protected:
	//##ModelId=3C4C4C4400AE
	IMPORT_C virtual TBool GetNextDataPart(TPtrC8& aDataChunk);
	//##ModelId=3C4C4C4400AD
	IMPORT_C void ReleaseData() ;
	//##ModelId=3C4C4C4400AC
	IMPORT_C TInt OverallDataSize();
	//##ModelId=3C4C4C44009A
	IMPORT_C void DumpResponseBody(RHTTPTransaction& aTrans);
	//##ModelId=3C4C4C440098
	IMPORT_C void DumpRespHeaders(RHTTPTransaction& aTrans);
protected:
	//##ModelId=3C4C4C440090
	RHTTPSession iSession;
	//##ModelId=3C4C4C440084
	RHTTPTransaction iTransaction;
	};


/**
 * This class drives CHttpTestBase-derived test objects
 * 
 */
//##ModelId=3C4C4C410007
class CHttpTestEngine : public CActive, public MTestInteractionObserver
	{
public:
	//##ModelId=3C4C4C410170
	IMPORT_C void SetCurrentStatusCode(TInt aStatusCode);
	//##ModelId=3C4C4C41015D
	IMPORT_C static CHttpTestEngine* NewL(const TDesC& aTestTitle, TBool aSilent = EFalse);
	//##ModelId=3C4C4C41015C
	IMPORT_C ~CHttpTestEngine();
	//##ModelId=3C4C4C410152
	IMPORT_C void ConfigureTestSet(RPointerArray<CHttpTestBase> aTestSet);
	//##ModelId=3C4C4C41013E
	IMPORT_C void TestCompleted(TInt aResult);
	//##ModelId=3C4C4C410135
	inline TInt TestIndex();
	//##ModelId=3C4C4C410134
	IMPORT_C CConsoleBase& Console() const;
	//##ModelId=3C4C4C41012A
	IMPORT_C CHTTPTestUtils& Utils() const;
	//##ModelId=3C4C4C410116
	IMPORT_C void TestSummary(TInt& aNumPasses, TInt& iNumFailures) const;
	//##ModelId=3C4C4C410115
	inline RArray<TBool> TestResults() const;
	//##ModelId=3C4C4C41010B
	inline TBool IsSilent() const;
	//##ModelId=3C4C4C410102
	IMPORT_C void SetSilent(TBool aSilent);
	//##ModelId=3C4C4C410101
	IMPORT_C void PressAnyKey();

	// from MTestInteractionObserver
	//##ModelId=3C4C4C4100ED
	void TestInteractionDetectedL(TTestInteraction aTestInteraction);


protected:
	//##ModelId=3C4C4C4100E3
	void ConstructL(const TDesC& aTestTitle);
	// from CActive
	//##ModelId=3C4C4C4100DA
	virtual void DoCancel();
	//##ModelId=3C4C4C4100D9
	virtual void RunL();
private:
	//##ModelId=3C4C4C4100CF
	CHttpTestEngine(TBool aSilent);
	//##ModelId=3C4C4C4100C6
	void RunNextTestL();
	
private:
	enum TState { EIdle=0, ERunningTest, EShuttingDown, ESingleTestCaseFailed};

private:
	//##ModelId=3C4C4C4100BD
	RPointerArray<CHttpTestBase>	iTestSet;
	//##ModelId=3C4C4C410095
	RArray<TBool>					iTestResults;
	//##ModelId=3C4C4C41006D
	TState							iState;
	//##ModelId=3C4C4C410063
	TInt							iTestIndex;
	//##ModelId=3C4C4C410059
	CHttpTestBase*					iCurrentTest;
	//##ModelId=3C4C4C41004D
	CConsoleBase*					iConsole;
	//##ModelId=3C4C4C410039
	CHTTPTestUtils*					iUtils;
	//##ModelId=3C4C4C410027
	TBool							iSilent;
	//##ModelId=3C4C4C41001D
	TInt							iCurrentStatusCode;
	
	//##ModelId=3C4C4C410015
	CDetectKeyPress* iDetectKeyPress;
	};


inline void CHttpTestBase::SetEngine(CHttpTestEngine* aEngine)	
	{ 
	iEngine = aEngine;
	}

inline void CHttpTestBase::SetLeakTests(TInt aNumberOfLeakTests, TInt aFirstLeakIteration)
	{
	iLeakTests = aNumberOfLeakTests;
	iFirstLeakIteration = aFirstLeakIteration;
	}


inline TInt CHttpTestEngine::TestIndex()	
	{ 
	return  iTestIndex;
	}

inline RArray<TBool> CHttpTestEngine::TestResults() const
	{
	return iTestResults;
	}

inline TBool CHttpTestEngine::IsSilent() const
	{
	return iSilent;
	}



#endif
