// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains implementation of CTestBookmarksServer class
// System Include
// 
//

/**
 @file
 @internalTechnology 
*/

// User Includes
// Test Server
#include "TestBookmarksServer.h"

// Test steps
#include "TestBookmarksBaseStep.h"
#include "TestCreateBookmarkItemsStep.h"
#include "TestOpenItemStep.h"
#include "TestSetPropertyStep.h"
#include "TestGetPropertyStep.h"
#include "TestDeleteFolderStep.h"
#include "TestDeleteItemsInFolderStep.h"
#include "TestCustomPropertyStep.h"
#include "TestVersionStep.h"
#include "TestUpdateVisitedStep.h"


#if (!defined EKA2)
// The system-wide unique name for the test-server
_LIT(KServerName, "TestBookmarksServer");
#endif

/**
Static factory constructor. Creates and returns instance of the test server
@internalTechnology
@test
@return		A pointer to the newly created CTestBookmarksServer object
*/
CTestBookmarksServer*  CTestBookmarksServer::NewL()
	{
	// Construct the server
	CTestBookmarksServer* server = new(ELeave) CTestBookmarksServer();
	CleanupStack::PushL(server);

	// CServer base class call
	// Name the server using the system-wide unique string
	// Clients use this to create server sessions.
	server->StartL(server->ServerName());

	CleanupStack::Pop(server);
	return server;
	}

/**
Destructor. Closes the arrays.
@internalTechnology
@test
*/
CTestBookmarksServer::~CTestBookmarksServer()
	{
	iIds.Reset();
	iIds.Close();
	iTitles.ResetAndDestroy();
	iTitles.Close();
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
	CActiveScheduler* sched = NULL;
	sched = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(sched);
	CActiveScheduler::Install(sched);

	// Create the server inside trap harness
	CTestBookmarksServer *server = NULL;
	TRAPD(err, server = CTestBookmarksServer::NewL());
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
	CleanupStack::Pop(sched);
	delete server;
	delete sched;
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
	CActiveScheduler* sched = NULL;
	sched = new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTestBookmarksServer* server = NULL;

	// Create the test-server
	TRAPD(err, server = CTestBookmarksServer::NewL());

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
	TInt err = KErrNone;
	TRAP(err, MainL());
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
CTestStep* CTestBookmarksServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep *testStep = NULL;

	if (aStepName == KTestBookmarksBaseStep)
		{
		testStep = new (ELeave) CTestBookmarksBaseStep(*this);
		}
	if (aStepName == KTestCreateBookmarkItemsStep)
		{
		testStep = new (ELeave) CTestCreateBookmarkItemsStep(*this);
		}
	else if (aStepName == KTestOpenItemStep)
		{
		testStep = new (ELeave) CTestOpenItemStep(*this);
		}
	else if (aStepName == KTestSetPropertyStep)
		{
		testStep = new (ELeave) CTestSetPropertyStep(*this);
		}
	else if (aStepName == KTestGetPropertyStep)
		{
		testStep = new (ELeave) CTestGetPropertyStep(*this);
		}
	else if (aStepName == KTestDeleteFolderStep)
		{
		testStep = new (ELeave) CTestDeleteFolderStep(*this);
		}
	else if (aStepName == KTestDeleteItemsInFolderStep)
		{
		testStep = new (ELeave) CTestDeleteItemsInFolderStep(*this);
		}
	else if (aStepName == KTestCustomPropertyStep)
		{
		testStep = new (ELeave) CTestCustomPropertyStep(*this);
		}
	else if (aStepName == KTestVersionStep)
		{
		testStep = new (ELeave) CTestVersionStep(*this);
		}
	else if (aStepName == KTestUpdateVisitedStep)
		{
		testStep = new (ELeave) CTestUpdateVisitedStep(*this);
		}
	return testStep;
	}
/**
Converts a string to TBool
@internalTechnology
@test
@return		The boolean equivalent
@param		Descriptor containing the string to be converted
*/
TBool CTestBookmarksServer::GetBool(const TPtrC& aString)
	{
	_LIT(KTrue, "true");
	return (aString.Compare(KTrue) == KErrNone) ? ETrue : EFalse;
	}

/**
Converts a string to TUint32
@internalTechnology
@test
@return		The TUint32 equivalent
@param		Descriptor containing the value to be converted
*/
TUint32	CTestBookmarksServer::GetTUint32(const TPtrC& aValue)
	{
	TUint32 tUint32;
	TLex lex(aValue);
	lex.Val(tUint32, EHex);
	return tUint32;
	}

/**
Converts a string to TInt
@internalTechnology
@test
@return		The TInt equivalent
@param		Descriptor containing the value to be converted
*/
TInt CTestBookmarksServer::GetTInt(const TPtrC& aValue)
	{
	TInt tInt;
	TLex lex(aValue);
	lex.Val(tInt);
	return tInt;
	}

/**
Converts a string to TReal
@internalTechnology
@test
@return		The TReal equivalent
@param		Descriptor containing the value to be converted
*/
TReal CTestBookmarksServer::GetTReal(const TPtrC& aValue)
	{
	TReal tReal;
	TLex lex(aValue);
	lex.Val(tReal);
	return tReal;
	}

/**
Returns server name based on the EKA version
@internalTechnology
@test
@return		Descriptor containing the servername
*/
const TPtrC CTestBookmarksServer::ServerName()
	{
#if (!defined EKA2)
	return KServerName();
#else
	// The exe name can be either TestBookmarksServer or
	// TestBookmarksServer_Cap based on whether the normal
	// or the security tests are being run. So decide the server
	// name during runtime
	TParsePtrC serverName(RProcess().FileName());
	return serverName.Name();
#endif
	}
