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

#ifndef __CTESTENGINE_H__
#define __CTESTENGINE_H__

#include <e32base.h>
#include <f32file.h>
#include <http/framework/cprottransaction.h>
#if !defined __MTESTOBSERVER_H__
#include "mtestobserver.h"
#endif
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif
/**	@class		CTestEngine
	@todo
	@since		8.0
*/
class CTestEngine : public CHttpTestBase,public MTestObserver
					
	{
public:		// methods

/**	@fn			NewL()
	Factory constructor.
	@since		8.0
	@return		A pointer to a fully initialised object.
*/
	static CTestEngine* NewL();

/**	@fn			~CTestEngine
	Destructor.
	@since		8.0
*/
	virtual ~CTestEngine();

/**	@fn			DoTestsL()
	Starts the test run.
	@since		8.0
*/
//	void DoTestsL();
	

public:
	const TDesC& TestName();
	void RunTestsL();
	void DoRunL();
	void DoCancel();
public:	// methods from CProtTransaction

//	virtual void CreateTxDataL();
//	virtual void CreateRxDataL(MRxDataObserver& aObserver);
		
private:	// methods

/**	@fn			CTestEngine()
	Constructor.
	@since		8.0
*/
	CTestEngine();

/**	@fn			ConstructL()
	Second phase constructor.
	@since		8.0
*/
	void ConstructL();

// Test 
	void TestComposeMessageL(TInt aNumberofHeaders);
	void TestParseMessageL(TInt aMessageType);
	
	
private:	// methods from MTestObserver

	virtual void NotifyError(TInt aError);
	virtual void NotifyStart();
	void NotifyComplete();

	//logger related
private:
	IMPORT_C void StartTimer();
	IMPORT_C void EndTimer(const TDesC&);
	void LogIt(const TDesC& aComment);
	
private:	// attributes

	TInt				iTestCount;
	TBool				iTestFailed;
	//Timer related
	RFs iFs;
	RFile iFile;
	TUint32 iStartTime;
	TUint32 iEndTime;
	TTimeIntervalMicroSeconds iTimeDifference ;
	CHTTPTestUtils*		iTestUtils;
	};

#endif	// __CTESTENGINE_H__
