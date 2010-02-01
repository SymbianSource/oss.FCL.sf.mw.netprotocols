// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// System Include
// 
//

/**
 @file
 @internalTechnology 
*/

// User Includes
// Test Server
#include "TestScriptTestServer.h"
#include "ctesthttpclientstep.h"
// Test steps

_LIT(KTestScriptHTTPClientAPIStep, "httpserviceapiteststep");
_LIT(KServerName, "httpserviceapitest");

/**
Static factory constructor. Creates and returns instance of the test server
@internalTechnology
@test
@return		A pointer to the newly created CTestScriptTestServer object
*/
CTestScriptTestServer*  CTestScriptTestServer::NewL()
	{
	// Construct the server
	CTestScriptTestServer* server = new(ELeave) CTestScriptTestServer();
	CleanupStack::PushL(server);

	// CServer base class call
	// Name the server using the system-wide unique string
	// Clients use this to create server sessions.
	server->StartL(KServerName);

	CleanupStack::Pop(server);
	return server;
	}

/**
Destructor. Closes the arrays.
@internalTechnology
@test
*/
CTestScriptTestServer::~CTestScriptTestServer()
	{
	
	}


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
	CTestScriptTestServer* server = NULL;

	// Create the test-server
	TRAPD(err, server = CTestScriptTestServer::NewL());

	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
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

CTestStep* CTestScriptTestServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;

	if (aStepName == KTestScriptHTTPClientAPIStep)
		{
		testStep = new CTestHttpClientStep;
		}
	
	return testStep;
	}
