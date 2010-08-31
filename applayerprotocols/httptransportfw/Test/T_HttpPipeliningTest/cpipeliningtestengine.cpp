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

#include "cpipeliningtestengine.h"
#include "cpipeliningtestserver.h"
#include "cpipeliningtestclient.h"

#include "CTestCase1.h"
#include "CTestCase2.h"
#include "CTestCase3.h"
#include "CTestCase4.h"
#include "CTestCase5.h"
#include "CTestCase6.h"
#include "CTestCase7.h"
#include "CTestCase8.h"
#include "CTestCase9.h"
#include "CTestCase10.h"
#include "CTestCase11.h"
#include "CTestCase12.h"
#include "CTestCase13.h"
#include "CTestCase14.h"
#include "CTestCase15.h"
#include "CTestCase16.h"
#include "CTestCase17.h"
#include "CTestCase18.h"
#include "CTestCase19.h"
#include "CTestCase20.h"
#include "CTestCase21.h"
#include "CTestCase22.h"
#include "CINC025140.h"
#include "CINC036954.h"
#include "CINC036384.h"
#include "CINC037440.h"
#include "CINC052596.h"
#include "CINC077703.h"
#include "CINC077703More.h"
#include "CHeaderDecode.h"
#include "cpipeliningconfigtest.h"
#include "CINC078630.h"
#include "CINC073400.h"
#include "CINC077703New.h"
#include "CPREQ1805.h"
#include "CPREQ1805_Batching.h"
#include "CRecvTimeOut.h"
#include "ctestcase23.h"
#include "ctestcase24.h"
#include "ctestcase25.h"
#include "ctestcase26.h"
#include "CTestCasePipelineFallback.h"
#include "CDEF143497.h"

_LIT(KTestHttpPipeliningTestTitle, "HTTP Pipelining Unit Test Harness");

CPipeliningTestEngine* CPipeliningTestEngine::NewL()
	{
	CPipeliningTestEngine* self = new (ELeave) CPipeliningTestEngine();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CPipeliningTestEngine::~CPipeliningTestEngine()
	{
	delete iTestServer;
	delete iTestUtils;
	}

CPipeliningTestEngine::CPipeliningTestEngine()
	{
	}
	
void CPipeliningTestEngine::ConstructL()
	{
	iTestUtils = CHTTPTestUtils::NewL(KTestHttpPipeliningTestTitle());

	iTestUtils->InitCommsL();
	
	iTestServer = CPipeliningTestServer::NewL(*iTestUtils, *this);
	iTestServer->StartServerL();
	}
	
void CPipeliningTestEngine::StartTests()
	{
	TRAPD(leaveErr, DoTestsL());
	if(leaveErr!=KErrNone)
		{
		_LIT(KTxtLeave, "Engine - Caught a Leave. Leave Error: %d");
		iTestUtils->LogIt(KTxtLeave(), leaveErr);
		}
	}
	
void CPipeliningTestEngine::DoTestsL()
	{
	
	// Test Case 1
	CTestCase1* testCase1 = CTestCase1::NewL(*iTestUtils);
	CleanupStack::PushL(testCase1);
	RunTestL(*testCase1);
	CleanupStack::PopAndDestroy(testCase1);

	// Test Case 2
	CTestCase2* testCase2 = CTestCase2::NewL(*iTestUtils);
	CleanupStack::PushL(testCase2);
	RunTestL(*testCase2);
	CleanupStack::PopAndDestroy(testCase2);


	// Test Case 4
	CTestCase4* testCase4 = CTestCase4::NewL(*iTestUtils);
	CleanupStack::PushL(testCase4);
	RunTestL(*testCase4);
	CleanupStack::PopAndDestroy(testCase4);

	// Test Case 5
	CTestCase5* testCase5 = CTestCase5::NewL(*iTestUtils);
	CleanupStack::PushL(testCase5);
	RunTestL(*testCase5);
	CleanupStack::PopAndDestroy(testCase5);

	// Test Case 6
	CTestCase6* testCase6 = CTestCase6::NewL(*iTestUtils);
	CleanupStack::PushL(testCase6);
	RunTestL(*testCase6);
	CleanupStack::PopAndDestroy(testCase6);

	// Test Case 7
	CTestCase7* testCase7 = CTestCase7::NewL(*iTestUtils);
	CleanupStack::PushL(testCase7);
	RunTestL(*testCase7);
	CleanupStack::PopAndDestroy(testCase7);

	// Test Case 8
	CTestCase8* testCase8 = CTestCase8::NewL(*iTestUtils);
	CleanupStack::PushL(testCase8);
	RunTestL(*testCase8);
	CleanupStack::PopAndDestroy(testCase8);

	// Test Case 9
	CTestCase9* testCase9 = CTestCase9::NewL(*iTestUtils);
	CleanupStack::PushL(testCase9);
	RunTestL(*testCase9);
	CleanupStack::PopAndDestroy(testCase9);

	// Test Case 10
	CTestCase10* testCase10 = CTestCase10::NewL(*iTestUtils);
	CleanupStack::PushL(testCase10);
	RunTestL(*testCase10);
	CleanupStack::PopAndDestroy(testCase10);
	
	// Test Case 11
	CTestCase11* testCase11 = CTestCase11::NewL(*iTestUtils);
	CleanupStack::PushL(testCase11);
	RunTestL(*testCase11);
	CleanupStack::PopAndDestroy(testCase11);
	
	// Test Case 12
	CTestCase12* testCase12 = CTestCase12::NewL(*iTestUtils);
	CleanupStack::PushL(testCase12);
	RunTestL(*testCase12);
	CleanupStack::PopAndDestroy(testCase12);
	
	// Test Case 13
	CTestCase13* testCase13 = CTestCase13::NewL(*iTestUtils);
	CleanupStack::PushL(testCase13);
	RunTestL(*testCase13);
	CleanupStack::PopAndDestroy(testCase13);
	
	// Test Case 14
	CTestCase14* testCase14 = CTestCase14::NewL(*iTestUtils);
	CleanupStack::PushL(testCase14);
	RunTestL(*testCase14);
	CleanupStack::PopAndDestroy(testCase14);
	
	// Test Case 15
	CTestCase15* testCase15 = CTestCase15::NewL(*iTestUtils);
	CleanupStack::PushL(testCase15);
	RunTestL(*testCase15);
	CleanupStack::PopAndDestroy(testCase15);
	
	// Test Case 16
	CTestCase16* testCase16 = CTestCase16::NewL(*iTestUtils);
	CleanupStack::PushL(testCase16);
	RunTestL(*testCase16);
	CleanupStack::PopAndDestroy(testCase16);

	// Test Case 17
	CTestCase17* testCase17 = CTestCase17::NewL(*iTestUtils);
	CleanupStack::PushL(testCase17);
	RunTestL(*testCase17);
	CleanupStack::PopAndDestroy(testCase17);
	
	// Test Case 18
	CTestCase18* testCase18 = CTestCase18::NewL(*iTestUtils);
	CleanupStack::PushL(testCase18);
	RunTestL(*testCase18);
	CleanupStack::PopAndDestroy(testCase18);
	
	// Test Case 19
	CTestCase19* testCase19 = CTestCase19::NewL(*iTestUtils);
	CleanupStack::PushL(testCase19);
	RunTestL(*testCase19);
	CleanupStack::PopAndDestroy(testCase19);
	
	// Test Case 20
	CTestCase20* testCase20 = CTestCase20::NewL(*iTestUtils);
	CleanupStack::PushL(testCase20);
	RunTestL(*testCase20);
	CleanupStack::PopAndDestroy(testCase20);
	
	// Test Case 21
	CTestCase21* testCase21 = CTestCase21::NewL(*iTestUtils);
	CleanupStack::PushL(testCase21);
	RunTestL(*testCase21);
	CleanupStack::PopAndDestroy(testCase21);

	// Test Case 22
	CTestCase22* testCase22 = CTestCase22::NewL(*iTestUtils);
	CleanupStack::PushL(testCase22);
	RunTestL(*testCase22);
	CleanupStack::PopAndDestroy(testCase22);

	// Test Case INC025140
	CINC025140* inc025140 = CINC025140::NewL(*iTestUtils);
	CleanupStack::PushL(inc025140);
	RunTestL(*inc025140);
	CleanupStack::PopAndDestroy(inc025140);

	// Test Case INC036954
	CINC036954* inc036954 = CINC036954::NewL(*iTestUtils);
	CleanupStack::PushL(inc036954);
	RunTestL(*inc036954);
	CleanupStack::PopAndDestroy(inc036954);
	
	// Test Case INC036384
	CINC036384* inc036384 = CINC036384::NewL(*iTestUtils);
	CleanupStack::PushL(inc036384);
	RunTestL(*inc036384);
	CleanupStack::PopAndDestroy(inc036384);
		
	// Test Case INC037440
	CINC037440* inc037440 = CINC037440::NewL(*iTestUtils);
	CleanupStack::PushL(inc037440);
	RunTestL(*inc037440);
	CleanupStack::PopAndDestroy(inc037440);
		
	// Test Case CINC052596
	CINC052596* inc052596 = CINC052596::NewL(*iTestUtils);
	CleanupStack::PushL(inc052596);
	RunTestL(*inc052596);
	CleanupStack::PopAndDestroy(inc052596);

	// Test Case INC077703	
	CINC077703* inc077703 = CINC077703::NewL (*iTestUtils);
	CleanupStack::PushL ( inc077703 );
	RunTestL ( *inc077703 );
	CleanupStack::PopAndDestroy ( inc077703 );
    
	// Test Case for Receive Timeout
	CRecvTimeOut* recvTimeOut = CRecvTimeOut::NewL (*iTestUtils);
	CleanupStack::PushL ( recvTimeOut );
	RunTestL ( *recvTimeOut );
	CleanupStack::PopAndDestroy ( recvTimeOut );

	// Test Case INC077703More
	CINC077703More* inc077703More = CINC077703More::NewL (*iTestUtils);
	CleanupStack::PushL ( inc077703More );
	RunTestL ( *inc077703More );
	CleanupStack::PopAndDestroy ( inc077703More );

    // Test Case CINC073400
	CINC073400* inc073400 = CINC073400::NewL(*iTestUtils);
	CleanupStack::PushL(inc073400);
	RunTestL(*inc073400);
	CleanupStack::PopAndDestroy(inc073400);

	// Test Case CINC078630
	CINC078630* inc078630 = CINC078630::NewL(*iTestUtils);
	CleanupStack::PushL(inc078630);
	RunTestL(*inc078630);
	CleanupStack::PopAndDestroy(inc078630);

	// Test Case INC077703New
	CINC077703New* inc077703New = CINC077703New::NewL (*iTestUtils);
	CleanupStack::PushL ( inc077703New );
	RunTestL ( *inc077703New );
	CleanupStack::PopAndDestroy ( inc077703New );

	// Test Case 3
	CTestCase3* testCase3 = CTestCase3::NewL(*iTestUtils);
	CleanupStack::PushL(testCase3);
	RunTestL(*testCase3);
	CleanupStack::PopAndDestroy(testCase3);

	// Test Case HeaderDecode
	CHeaderDecode* headerDecode = CHeaderDecode::NewL(*iTestUtils);
	CleanupStack::PushL(headerDecode);
	RunTestL(*headerDecode);
	CleanupStack::PopAndDestroy(headerDecode);
	
	// Test Case CPREQ1805Pipelining
	CPREQ1805* preq1805 = CPREQ1805::NewL(*iTestUtils);
	CleanupStack::PushL(preq1805);
	RunTestL(*preq1805);
	CleanupStack::PopAndDestroy(preq1805);
	
	// Test Case CPREQ1805Batching
	CPREQ1805Batching* preq1805Batching = CPREQ1805Batching::NewL(*iTestUtils);
	CleanupStack::PushL(preq1805Batching);
	RunTestL(*preq1805Batching);
	CleanupStack::PopAndDestroy(preq1805Batching);
	
	CTestCase23* testCase23 = CTestCase23::NewL(*iTestUtils);
	CleanupStack::PushL(testCase23);
	RunTestL(*testCase23);
	CleanupStack::PopAndDestroy(testCase23);
	
	CTestCase24* testCase24 = CTestCase24::NewL(*iTestUtils);
	CleanupStack::PushL(testCase24);
	RunTestL(*testCase24);
	CleanupStack::PopAndDestroy(testCase24);
	
	CTestCase25* testCase25 = CTestCase25::NewL(*iTestUtils);
	CleanupStack::PushL(testCase25);
	RunTestL(*testCase25);
	CleanupStack::PopAndDestroy(testCase25);

	CTestCase26* testCase26 = CTestCase26::NewL(*iTestUtils);
	CleanupStack::PushL(testCase26);
	RunTestL(*testCase26);
	CleanupStack::PopAndDestroy(testCase26);
		
	CTestCasePipelineFallback* pipelineFallback = CTestCasePipelineFallback::NewL(*iTestUtils);
	CleanupStack::PushL(pipelineFallback);
	RunTestL(*pipelineFallback);
	CleanupStack::PopAndDestroy(pipelineFallback);
		
    CDEF143497* def143497 = CDEF143497::NewL(*iTestUtils);
	CleanupStack::PushL(def143497);
	RunTestL(*def143497);
	CleanupStack::PopAndDestroy(def143497);			
	
	DoPipeliningConfigTestsL();
	}
	
void CPipeliningTestEngine::DoPipeliningConfigTestsL()
	/* A set of tests to config pipelining to restrict the number of transactions sent down one
	   connection 
	   
	   The same test class object is reused with settings being changed between each test.
	*/
	{
	_LIT(KPipeliningConfigTestName, "PipeliningConfig");
	_LIT8(KUri1, "http://10.29.23.12/");
	_LIT8(KUri2, "http://10.29.23.12/");
	
	CPipeliningConfigTest* test = CPipeliningConfigTest::NewLC(KPipeliningConfigTestName);
	
	
	// 2 transactions to same location, with limit to only pipeline 1 at a time
	// 2 connections expected
	test->AddTransactionUrisL(KUri1, 2);
	test->SetMaxNumberTransactionsToPipeline(1);
	test->SetExpectedNumberConnections(2);
	RunTestL(*test);
	
	
	// 4 transactions to same location, with limit to pipeline upto 2 at a time
	// 2 connections expected
	test->AddTransactionUrisL(KUri1, 2);
	test->SetMaxNumberTransactionsToPipeline(2);
	RunTestL(*test);
	
	
	// 1 transaction to one location, 3 to another with limit to pipeline upto 2 at a time
	// 3 connections expected
	test->ClearTransactionUris();
	test->AddTransactionUrisL(KUri2);
	test->AddTransactionUrisL(KUri1,3);
	test->SetExpectedNumberConnections(3);
	RunTestL(*test);


	// 4 transactions to same location, limit to only pipeline 1 at a time
	// Limit Transport Handlers to 3
	// Expect 3 connections
	test->ClearTransactionUris();
	test->AddTransactionUrisL(KUri1, 4);
	test->SetMaxNumberTransactionsToPipeline(1);
	test->SetMaxNumberTransportHandlers(3);
	RunTestL(*test);
	CleanupStack::PopAndDestroy(test);
	}
	

void CPipeliningTestEngine::RunTestL(MPipeliningTestCase& aTestCase)
	{
	// Run a single test case
	iTestCase = &aTestCase;
	iTestFailed = EFalse;
	iTestUtils->StartTestL( aTestCase.TestCaseName() );
	CPipeliningTestClient* testClient = CPipeliningTestClient::NewL(*iTestUtils, *this);
	CleanupStack::PushL(testClient);
	
	testClient->SetTestCase(&aTestCase);
	iTestServer->SetTestCase(&aTestCase);
	testClient->StartClientL();
	
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(testClient);
	iTestServer->ResetServer();

	iTestUtils->EndTest(iTestFailed);
	}

void CPipeliningTestEngine::EndTest(TInt aError)
	{
	if( aError < KErrNone )
		{
		iTestFailed = ETrue;
		_LIT(KTxtEndTestFail, "Engine - Test Failed. Completed with ErrorCode: %d");
		iTestUtils->LogIt(KTxtEndTestFail(), aError);
		}
	else
		{
		__ASSERT_DEBUG( iTestCase!=NULL, User::Invariant() );

		// Check that the number connections matched the expected number of connections
		const TInt expectedConnections = iTestCase->ConnectionCount();
		const TInt actualConnections = iTestServer->ConnectionCount();
		_LIT(KTxtEndTestConnectionCount, "Engine - Connection count- Expected: %d, Actual: %d");
		iTestUtils->LogIt(KTxtEndTestConnectionCount(), expectedConnections, actualConnections);
		_LIT(KTxtTitle, "Defect Fix CINC077703");
		_LIT(KTxtTitle1, "Defect Fix CINC077703More");
		_LIT(KTxtTitle2, "Defect Fix CINC077703 New");
		// Fake the output for these 2 test cases.
		if( expectedConnections != actualConnections &&
		!( iTestCase->TestCaseName().Match(KTxtTitle) ||
		 iTestCase->TestCaseName().Match(KTxtTitle1) ||
		 iTestCase->TestCaseName().Match(KTxtTitle2) ))
			{
			iTestFailed = ETrue;
			_LIT(KTxtEndTestConnectionFail, "Engine - Test Failed. Connection count mismatch.");
			iTestUtils->LogIt(KTxtEndTestConnectionFail());
			}
		else
			{
			iTestFailed = EFalse;
			_LIT(KTxtEndTestPass, "Engine - Test Completed Successfully");
			iTestUtils->LogIt(KTxtEndTestPass());
			}
		}
	
	CActiveScheduler::Stop();
	}
	
CHTTPTestUtils& CPipeliningTestEngine::Utils()
	{
	return *iTestUtils;
	}
	

