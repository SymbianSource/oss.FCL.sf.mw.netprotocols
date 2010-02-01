// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <es_sock.h>
#include <mconnectionprefsprovider.h>
#include <chttptransportlayer.h>

#include "httptestutils.h"
#include "mdriverobserver.h"

const TInt KDefaultBufferSize		= 6*1024;

/**	@class		CTestEngine
	The CTestEngine class performs the testing. It runs through the test cases
	defined in the HTTP Daemon Transport Handler Unit Test Specification. The
	results of the tests are logged to the appropriate summary file. More verbose
	logging is placed in the non-summary file.
	@since		8.0
*/
class CTestEngine : public CBase,
					public MDriverObserver,
					public MConnectionPrefsProvider
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
	void DoTestsL();

/** @fn			Log(const TDesC& aComment)
	@see		MTestObserver
*/
	virtual void Log(const TDesC& aComment);

/**	@fn			DoListenAbortTestL()
	Starts the Listen and aborts it.
*/
	void DoListenAbortTestL();
	
private:	// methods from MDriverObserver

/**	@fn			NotifyError(TInt aError)
	@see		MTestObserver
*/
	virtual void NotifyError(TInt aError);

/**	@fn			NotifyStart()
	@see		MTestObserver
*/
	virtual void NotifyStart();

/**	@fn			NotifyComplete()
	@see		MTestObserver
*/
	virtual void NotifyComplete();


private:	// methods from MCommsConnectionProvider

/**	@fn			SupplyCommsConnection(TInt& aSocketServerHandle, RConnection*& aConnectionPtr)
	@see		MCommsConnectionProvider
*/
	virtual TBool SupplyCommsConnection(RConnection*& aConnectionPtr);

/**	@fn			SetCommsConnection(TInt aSocketServerHandle, RConnection* aConnectionPtr)
	@see		MCommsConnectionProvider
*/
	virtual void SetCommsConnectionL(RConnection* aConnectionPtr);

/**	@fn			GetSecurityPrefs(TBool aDialogPrompt, MSecurityPolicy* aSecurityPolicy)
	@see		MCommsConnectionProvider	
*/
	virtual void GetSecurityPrefs(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy);

/**	@fn			ImmediateSocketShutdown()
	@see		MCommsConnectionProvider	
*/
	virtual TBool ImmediateSocketShutdown();

	virtual TBool SupplySocketServerHandle ( TInt& aSocketServerHandle );
	
	virtual void SetSocketServerHandleL ( TInt aSocketServerHandle );


	virtual TInt SessionId();
	TInt GetRecvBufferSize();

private:	// typedefs

/**	@typedef	TTestFunctionL
	A typedef defining a pointer to a test function.
	@since		8.0
*/
	typedef void (CTestEngine::*TTestFunctionL) ();

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

	void StartTesting();

	void StopTesting();

	void ResetTest();

/**	@fn			DoBasicTestCaseL()
	Performs the Basic test case, as described in the Unit Test specification.
	@since		8.0
*/
	void DoBasicTestCaseL();

/**	@fn			DoMultipleConnectionsTestCaseL()
	Performs the Multiple Connections test case, as described in the Unit Test
	specification.
	@since		8.0
*/
	void DoMultipleConnectionsTestCaseL();

/**	@fn			DoConnectControlTestCaseL()
	Performs the Connect Control test case, as described in the Unit Test 
	specification.
	@since		8.0
*/
	void DoConnectControlTestCaseL();

/**	@fn			DoBasicSecureTestCaseL()
	Performs the Basic secure test case.
	@since		8.0
*/
	void DoBasicSecureTestCaseL();

/**	@fn			DoOOMTest(TTestFunctionL aDoTestFunctionL)
	Does OOM testing on the test case specified. Will continue to re-start the
	specified test case until either the test case function does not leave and
	the test passes. If a test case does not pass under standard conditions, 
	then OOM testing of that test case should not be done.
	@since		8.0
	@param		aDoTestFunctionL	The function of the test case to be run.
*/
	void DoOOMTest(TTestFunctionL aDoTestFunctionL);

/**	@fn			DoMultipleSecureConnectionsTestCaseL()
	Performs the Multiple Secure Connections test case
*/
	void DoMultipleSecureConnectionsTestCaseL();

private:	// attributes

/**	The test utils object - does all logging.
*/
	CHTTPTestUtils*		iTestUtils;

/**	The number of tests running.
*/
	TInt				iTestCount;

/**	A flag indicating whether the current test has failed.
*/
	TBool				iTestFailed;

	TBool				iDoingTest;

	TBool				iOOMTesting;

	TBool				iResetTest;

/**	The construction parameters required for the transport layer.
*/
	THttpTransportConstructionParams iTransportLayerParams;

/**	A flag to indicate whether to use the existing comms connection.
*/
	TBool				iUseConnection;

/**	The socket server.
*/
	RSocketServ			iSocketServer;

/**	The comms connection.
*/
	RConnection			iConnection;
	};

#endif	// __CTESTENGINE_H__

