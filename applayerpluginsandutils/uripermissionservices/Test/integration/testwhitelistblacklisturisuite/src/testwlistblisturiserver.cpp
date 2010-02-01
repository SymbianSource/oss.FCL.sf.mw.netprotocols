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
// Description:
// Contributors:
//
// Description:
// TESTWHITELISTBLACKLISTURISERVER.CPP
// Contains implementation of CTestWListBListUriServer class
// @internalAll
// 
//

// User Includes
// Test Server
#include "testwlistblisturiserver.h"

// Test steps
#include "testwlistblisturibasestep.h"
#include "testadduristep.h"
#include "testremoveuristep.h"
#include "testmatchuristep.h"
#include "testmatchuritypestep.h"
#include "testupdateurilisttypestep.h"
#include "testupdateurifavnamestep.h"
#include "testurilisttypestep.h"
#include "testuricountstep.h"
#include "testwithoutdbconnectionstep.h"
#include "testcopypreconfigxmlstep.h"
#include "testremovedbstep.h"
#include "testtldpolicystep.h"

#if (!defined EKA2)
// The system-wide unique name for the test-server
_LIT(KServerName, "TestWListBListUriServer");
#endif

/**
Static factory constructor. Creates and returns instance of the test server
@internalTechnology
@test
@return		A pointer to the newly created CTestWListBListUriServer object
*/
CTestWListBListUriServer*  CTestWListBListUriServer::NewL()
	{
	// Construct the server
	CTestWListBListUriServer* server = new(ELeave) CTestWListBListUriServer();
	CleanupStack::PushL(server);

	// CServer base class call
	// Name the server using the system-wide unique string
	// Clients use this to create server sessions.
	server->StartL(server->ServerName());

	CleanupStack::Pop(server);
	return server;
	}


#if (!defined EKA2)
/**
Creates the Active Scheduler, then creates the test-server, synchronises the
thread with the client and then enters the active scheduler.

This is EKA1 version of MainL(). Uses sempahore to sync with client
as Rendezvous calls are not available
*/
LOCAL_C void MainL()
	{
	// Create and install the active scheduler.
	CActiveScheduler* sched = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(sched);
	CActiveScheduler::Install(sched);

	// Create the server inside trap harness
	CTestWListBListUriServer *server = NULL;
	TRAPD(err, server = CTestWListBListUriServer::NewL());
	if (!err)
		{
		CleanupStack::PushL(server);
		RSemaphore sem;

		// The client API of TestExecute will already have created the
		// semaphore and will be waiting on it.
		User::LeaveIfError(sem.OpenGlobal(KServerName));

		CleanupStack::Pop(server);

		// Signal the client
		sem.Signal();
		sem.Close();

		// Enter the active scheduler
		sched->Start();
		}
	CleanupStack::PopAndDestroy(); // sched
	delete server;
	
	}
#else
/**
EKA2 version of MainL()
Uses the new Rendezvous call isntead of the older semaphore.
*/
LOCAL_C void MainL()
	{
	// For platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTestWListBListUriServer* server = NULL;

	// Create the test-server
	TRAPD(err, server = CTestWListBListUriServer::NewL());

	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}
#endif		// #if (!defined EKA2)


#if (defined __WINS__ && !defined EKA2)
/**
DLL entry-point for EKA1 emulator builds.
*/
GLDEF_C TInt E32Dll(enum TDllReason /*aDllReason*/)
	{
	return KErrNone;
	}
#else
/**
Exe entry point code, for EKA1 hardware and EKA2 builds.
*/
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup *cleanup = CTrapCleanup::New();
	if (cleanup == NULL)
		{
		return KErrNoMemory;
		}
	
	TRAPD(err, MainL());
	__ASSERT_ALWAYS(!err,User::Panic(KTxt,err));
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
#endif		// #if (defined __WINS__ && !defined EKA2)

#if (defined __WINS__ && !defined EKA2)
/**
For EKA1 emulator builds. This function is called when the thread is first
resumed. Has the standard thread entry siganture.
@internalTechnology
@test
@return		KErrNone if everything is fine or system-wide error if any
*/
TInt ThreadFunc (TAny* /*aParam*/)
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if (cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err, MainL());
	__ASSERT_ALWAYS(!err,User::Panic(KTxt,err));
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

/**
For EKA1 emulator builds. Creates and starts a thread for the server to run.
@internalTechnology
@test
@param		None
@return		Integer value indicating the error code.
*/
EXPORT_C TInt NewServer()
	{
	_LIT(KThread, "Thread");
	RThread thread;

	// Name the thread as "<Server-Name>Thread" making it hopefully unique
	TBuf<KMaxTestExecuteNameLength> threadName(KServerName);
	threadName.Append(KThread);

	const TInt KMaxHeapSize = 0x1000000;

	// Create the thread
	TInt err = thread.Create(threadName, ThreadFunc, KDefaultStackSize,
							 KMinHeapSize, KMaxHeapSize, NULL, EOwnerProcess
							);
	if (err != KErrNone)
		{
		return err;
		}

	// Start the thread -> effectively calls ThreadFunc
	thread.Resume();

	thread.Close();
	return KErrNone;
	}
#endif 		// #if (defined __WINS__ && !defined EKA2)


/**
Base class pure virtual
@internalTechnology
@test
@return 	Instance of the test step
@param		Descriptor containing the test-step name
*/
CTestStep* CTestWListBListUriServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep *testStep = NULL;
	TRAPD(err,testStep=CreateTestStepL(aStepName));
	if(err == KErrNone)
		return testStep;
	else
		return NULL;
	}


CTestStep* CTestWListBListUriServer::CreateTestStepL(const TDesC& aStepName)
	{
	CTestStep *testStep = NULL;
	
	if (aStepName == KTestWListBListUriBaseStep)
		{
		testStep = new (ELeave) CTestWListBListUriBaseStep();
		}
	else if (aStepName == KTestAddUriStep)
		{
		testStep = new (ELeave) CTestAddUriStep();
		}
	else if (aStepName == KTestRemoveUriStep)
		{
		testStep = new (ELeave) CTestRemoveUriStep();
		}
	else if (aStepName == KTestMatchUriStep)
		{
		testStep = new (ELeave) CTestMatchUriStep();
		}
	else if (aStepName == KTestMatchUriTypeStep)
		{
		testStep = new (ELeave) CTestMatchUriTypeStep();
		}
	else if (aStepName == KTestUpdateUriListTypeStep)
		{
		testStep = new (ELeave) CTestUpdateUriListTypeStep();
		}
	else if (aStepName == KTestUpdateUriFavNameStep)
		{
		testStep = new (ELeave) CTestUpdateUriFavNameStep();
		}
	else if (aStepName == KTestUriListTypeStep)
		{
		testStep = new (ELeave) CTestUriListTypeStep();
		}
	else if (aStepName == KTestUriCountStep)
		{
		testStep = new (ELeave) CTestUriCountStep();
		}
	else if (aStepName == KTestWithOutDBConnectionStep)
		{
		testStep = new (ELeave) CTestWithOutDBConnectionStep();
		}
	else if (aStepName == KTestCopyPreConfigXmlStep)
		{
		testStep = new (ELeave) CTestCopyPreConfigXmlStep();
		}
	else if (aStepName == KTestRemoveDBStep)
		{
		testStep = new (ELeave) CTestRemoveDBStep();
		}
	else if (aStepName == KTestTldPolicyStep)
		{
		testStep = new (ELeave) CTestTldPolicyStep();
		}
	
	return testStep;
	}

/**
Returns server name based on the EKA version
@internalTechnology
@test
@return		Descriptor containing the servername
*/
const TPtrC CTestWListBListUriServer::ServerName()
	{
#if (!defined EKA2)
	return KServerName();
#else
	// The exe name can be either TestWListBListUriServer or
	// TestWListBListUriServer_Cap based on whether the normal
	// or the security tests are being run. So decide the server
	// name during runtime
	TParsePtrC serverName(RProcess().FileName());
	return serverName.Name();
#endif
	}
