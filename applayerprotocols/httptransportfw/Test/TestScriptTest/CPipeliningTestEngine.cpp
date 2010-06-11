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

#include "httptestutils.h"
#include "CPipeliningTestEngine.h"
#include "CPipeliningTestServer.h"
#include "CPipeliningTestClient.h"

#include "CPipeliningTestParams.h"
#include "CPipeliningTestCase.h"
#include "CPipeliningConfigTest.h"

#include "ctestcaselocalandremotehost.h"

#if !defined(__TESTSCRIPTS_H__)
#include "TestScripts.h"
#endif


enum TPanic
	{
	EFailedToCreateEngine,
	EFailedToReadIniFile
	};
	

_LIT(KHttpPipeliningTestCompName, 	"pipelining");
_LIT(KHttpPipeliningTestScriptName, "pipelining.ini");

_LIT(KRunTestItemName, 				"RunTest");

//TestCase
_LIT(KTestCaseName,					"TestCaseName");
_LIT(KTestBatchingEnabled,			"Batching");
_LIT(KTestPipeliningEnabled,		"Pipelining");
_LIT(KTestBufferSize,				"BufferSize");
_LIT(KTestConnectionCount,			"ConnectionCount");
_LIT(KTestTransactionCount,			"TotalTransactionCount");
_LIT(KTestTransactionArr,			"TransactionList");
_LIT(KRawRequest,					"RawRequest");
_LIT(KRawResponse,					"RawResponse");
_LIT(KExpectedRequest,				".ExpectedRequests");
_LIT(KExpectedResponse,				".ExpectedResponses");
_LIT(KTestUri,						"Uri");
_LIT(KTestConn,						"Conn");
_LIT(KError,						"Error");
_LIT(KTransType,					"TransType");
_LIT(KHeaderName,					"Header");
_LIT(KHeaderVal,					".Val");
_LIT(KTestReqMethod, 				"Method");


_LIT(KTestHttpPipeliningTestTitle, "HTTP Pipelining Unit Test Harness");
_LIT(KUriLog, "TestCaseParams: Uri[%d] = %S");
_LIT(KRequestLog, "TestCaseParams: RawRequest [%d] = %S");
_LIT(KResponseLog, "TestCaseParams: RawResponse [%d] = %S");
_LIT(KHeaderLog, "TestCaseParams: Header [%d] = %S");
_LIT(KHeaderValLog, "TestCaseParams: Header [%d] : Value [%d] = %S");


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
	
void CPipeliningTestEngine::StartTestsL(TDesC& secName)
	{	
	TRAPD ( leaveErr,DoTestsL(secName));
	if ( leaveErr != KErrNone )
		{
		_LIT(KTxtLeave, "Engine - Caught a Leave. Leave Error: %d");
		iTestUtils->LogIt(KTxtLeave(), leaveErr);
		_LIT(KTxtFailed, "**** TEST FAILED ****");
		iTestUtils->LogIt(KTxtFailed);
		User::LeaveIfError(leaveErr);
		}
	}

void CPipeliningTestEngine::DoTestsL(TDesC& secName)
	{
	if((secName.Compare(_L("CONFIG_TEST1") ) == 0) || (secName.Compare(_L("CONFIG_TEST2") ) == 0) || (secName.Compare(_L("CONFIG_TEST3") ) == 0) || (secName.Compare(_L("CONFIG_TEST4") ) == 0))
		{
		DoPipeliningConfigTestsL(secName);
		}
	else
		{
		// Open the INI file to initialise the tests we're going to run
		CScriptFile* iniFile = NULL;
		TInt err = KErrNone;
		
		TRAP(err, iniFile = CScriptFile::NewL(*iTestUtils, KHttpPipeliningTestCompName(), KHttpPipeliningTestScriptName()));
		if (err != KErrNone)
			{
			User::Leave(KErrNoMemory);
			}
		
		CleanupStack::PushL(iniFile);
				
		// Create an array of sections from the script / ini file.
		CArrayPtrFlat<CScriptSection>& sections=iniFile->Sections();

		//create the  tests for iteration2 and  append them in the array
		TInt secCount = sections.Count();
		TInt count = secCount;
		CScriptSection* section = NULL;

		for ( TInt iter = 0 ; iter < count ; ++iter)
			{
			//create an array of tests to be sent to the engine
			section = sections[iter];
			
			// TestCase
			if ( section->SectionName().CompareF(secName) == 0)
				{
				TPtrC itemPtr(iniFile->Section(iter).Item(0).Item());
				TInt itemFieldCount= iniFile->Section(iter).Items().Count();

				TInt runTest = 0;
				runTest = iniFile->ItemValue(iniFile->Section(iter).SectionName(), KRunTestItemName(), 0);
				TPtrC runTestPtr = iniFile->Section(iter).ItemL(KRunTestItemName()).Value();
				TLex  ptrToIntLex(runTestPtr);
				ptrToIntLex.Val(runTest);
			
				if ( runTest == 1 )
					{
					CPipeliningTestParams* testCaseParams = CPipeliningTestParams::NewL(*iTestUtils);
					CleanupStack::PushL(testCaseParams);
							
					TBuf<4> intStr;
			
					//Urichecking strings initialisation
					HBufC* uriCheckingStringBuf = HBufC::NewL(10); 
					CleanupStack::PushL(uriCheckingStringBuf);
					TInt uriCtr = 1;
					(uriCheckingStringBuf->Des()).Copy(KTestUri());					
					intStr.Format(_L( "%d"), uriCtr++);
					(uriCheckingStringBuf->Des()).Append(intStr);
					
					//RawRequest 
					HBufC* rawRequestCheckingStringBuf = HBufC::NewL(24);
					CleanupStack::PushL(rawRequestCheckingStringBuf);
					TInt rawReqCtr = 1;
					(rawRequestCheckingStringBuf->Des()).Copy(KRawRequest());					
					intStr.Format(_L( "%d"), rawReqCtr++);
					(rawRequestCheckingStringBuf->Des()).Append(intStr);
					
						
					//RawResponse
					HBufC* rawResponseCheckingStringBuf = HBufC::NewL(24);
					CleanupStack::PushL(rawResponseCheckingStringBuf);
					TInt rawRespCtr = 1;
					(rawResponseCheckingStringBuf->Des()).Copy(KRawResponse());					
					intStr.Format(_L( "%d"), rawRespCtr++);
					(rawResponseCheckingStringBuf->Des()).Append(intStr);
					
					
					//ConnRequest & ConnResponse
					HBufC* connRequestArrStr = HBufC::NewL(32);
					CleanupStack::PushL(connRequestArrStr);
					HBufC* connResponseArrStr = HBufC::NewL(34);
					CleanupStack::PushL(connResponseArrStr);
					TInt connReqCtr = 1;
					TInt connRespCtr = 1;
					(connRequestArrStr->Des()).Copy(KTestConn());
					(connResponseArrStr->Des()).Copy(KTestConn());
					intStr.Format(_L( "%d"), connReqCtr++);
					(connRequestArrStr->Des()).Append(intStr);
					intStr.Format(_L( "%d"), connRespCtr++);
					(connResponseArrStr->Des()).Append(intStr);
					(connRequestArrStr->Des()).Append(KExpectedRequest());
					(connResponseArrStr->Des()).Append(KExpectedResponse());
					
					//TransType
					HBufC* transTypeStringBuf = HBufC::NewL(22);
					CleanupStack::PushL(transTypeStringBuf);
					TInt transTypeCtr = 2;
					(transTypeStringBuf->Des()).Copy(KTransType());					
					intStr.Format(_L( "%d"), transTypeCtr++);
					(transTypeStringBuf->Des()).Append(intStr);		
					
					//Header2Process
					HBufC* hdrName = HBufC::NewL(22);
					CleanupStack::PushL(hdrName);
					TInt hdrNameCtr = 1;
					(hdrName->Des()).Copy(KHeaderName());					
					intStr.Format(_L( "%d"), hdrNameCtr++);
					(hdrName->Des()).Append(intStr);	
					
					//ValueOfHeader	
					HBufC* hdrVal = HBufC::NewL(22);
					CleanupStack::PushL(hdrVal);
					HBufC* hdrValNext = HBufC::NewL(22);
					CleanupStack::PushL(hdrValNext);
					TInt hdrValCtr = 1;
					(hdrVal->Des()).Copy(KHeaderName());
					(hdrValNext->Des()).Copy(KHeaderName());					
					intStr.Format(_L( "%d"), (hdrNameCtr - 1));
					(hdrVal->Des()).Append(intStr);
					intStr.Format(_L( "%d"), (hdrNameCtr));
					(hdrValNext->Des()).Append(intStr);				
					(hdrVal->Des()).Append(KHeaderVal());
					(hdrValNext->Des()).Append(KHeaderVal());
					intStr.Format(_L( "%d"), hdrValCtr++);
					(hdrVal->Des()).Append(intStr);
					(hdrValNext->Des()).Append(intStr);
					
					for (TInt fctr=0; fctr < itemFieldCount ; ++fctr)
						{
						//Get the field and value				
						TPtrC itemFieldPtr(iniFile->Section(iter).Item(fctr).Item());
						TPtrC itemValuePtr(iniFile->Section(iter).Item(fctr).Value());
																
						//which member of testcase param to set?
						if (itemFieldPtr.Compare(KRunTestItemName()) == 0)
							{
							//RunTest
							testCaseParams->SetRunTest(itemValuePtr);
							TInt tempInt = 1;
							iTestUtils->LogIt(_L("TestCaseParams: RunTest= %d"), tempInt);
							}
						
						else if (itemFieldPtr.Compare(KTestPipeliningEnabled()) == 0)
							{
							//Pipelining Enabled
							testCaseParams->EnablePipelining(itemValuePtr);
							iTestUtils->LogIt(_L("TestCaseParams: PipeliningEnabled= %d"),testCaseParams->PipeliningEnabled());	
							}
						else if (itemFieldPtr.Compare(KTestBatchingEnabled()) == 0)			
							{
							//Batching Enabled
							testCaseParams->EnableBatching( itemValuePtr );
							iTestUtils->LogIt(_L("TestCaseParams: BatchingEnabled= %d"),testCaseParams->BatchingEnabled());	
							}
						else if (itemFieldPtr.Compare(KTestBufferSize()) == 0)				
							{
							//Buffer size
							testCaseParams->SetBufferSize(itemValuePtr);
							iTestUtils->LogIt(_L("TestCaseParams: BufferSize= %d"),testCaseParams->BufferSize());
							}
						else if (itemFieldPtr.Compare(KTestTransactionCount()) == 0)				
							{
							//TransactionCount
							testCaseParams->SetTotalTransactionCount(itemValuePtr);
							iTestUtils->LogIt(_L("TestCaseParams: TotalTransactionCount= %d"),testCaseParams->TotalTransactionCount());
							}		
						else if (itemFieldPtr.Compare(KTestConnectionCount()) == 0)			
							{
							//ConnectionCount
							testCaseParams->SetConnectionCount(itemValuePtr);
							iTestUtils->LogIt(_L("TestCaseParams: ConnectionCount= %d"),testCaseParams->ConnectionCount());
							}
						else if (itemFieldPtr.Compare(KError()) == 0)			
							{
							//Error
							testCaseParams->SetErrorL(itemValuePtr);
							iTestUtils->LogIt(_L("TestCaseParams: Error= %d"),testCaseParams->Error());
							}
						else if (itemFieldPtr.Compare(KTestCaseName()) == 0)				
							{
							//TestCaseName
							testCaseParams->SetTestCaseNameL(itemValuePtr);
							TPtrC tcName = testCaseParams->TestCaseName();
							iTestUtils->LogIt(_L("TestCaseParams: TestCaseName= %S"),&tcName);
							}
						else if (itemFieldPtr.Compare(KTestTransactionArr()) == 0)
							{
							//TransArr
							testCaseParams->SetSequenceArrL(itemValuePtr, ETransactionArray);
							iTestUtils->LogIt(_L("TestCaseParams: SequenceArr= %S"), &itemValuePtr);
							
							}
						else if(itemFieldPtr.Compare(KTestReqMethod()) == 0)
							{
							//Request Method
							testCaseParams->SetReqMethodL(itemValuePtr);
							TPtrC reqMethod = testCaseParams->ReqMethod();
							iTestUtils->LogIt(_L("TestCaseParams: RequestMethod= %S"),&reqMethod );
							}
						else if ((itemFieldPtr.Compare(KTestUri()) == 0) || ((itemFieldPtr.Compare(*uriCheckingStringBuf)) == 0))
							{
							//Uri
							testCaseParams->SetUriL(uriCtr-2, itemValuePtr);
							TPtrC8 uri = (testCaseParams->UriL(uriCtr-2));
							
							//for logging
							HBufC* uri16 = HBufC::NewL(uri.Length()*2);
							CleanupStack::PushL(uri16);
							(uri16->Des()).Copy(uri);
							TPtrC uriPtr = (uri16->Des());					
							iTestUtils->LogIt(KUriLog(), uriCtr-2 , &uriPtr);
							CleanupStack::PopAndDestroy(uri16);
							
							//ready for checking next uri
							(uriCheckingStringBuf->Des()).Copy(KTestUri());					
							intStr.Format(_L( "%d"), uriCtr++);
							(uriCheckingStringBuf->Des()).Append(intStr);
							}

						else if (itemFieldPtr.Compare(*rawRequestCheckingStringBuf) == 0)
							{
							//RawRequest
							testCaseParams->SetRawRequestL(rawReqCtr-2, itemValuePtr);
							TPtrC8 request = testCaseParams->RawRequest( rawReqCtr-2 );
							
							//for logging
							HBufC* req16 = HBufC::NewL(request.Size() * 2);
							CleanupStack::PushL(req16);
							req16->Des().Copy(request);
							TPtrC req16Ptr = req16->Des();
							iTestUtils->LogIt(KRequestLog(),rawReqCtr-2,  &req16Ptr);
							CleanupStack::PopAndDestroy(req16);
							
							//ready for checking next request
							(rawRequestCheckingStringBuf->Des()).Copy(KRawRequest());					
							intStr.Format(_L( "%d"), rawReqCtr++);
							(rawRequestCheckingStringBuf->Des()).Append(intStr);
							}

						else if (itemFieldPtr.Compare(*rawResponseCheckingStringBuf) == 0)
							{
							//RawRequest
							testCaseParams->SetRawResponseL(rawRespCtr-2, itemValuePtr);
							TPtrC8 response = testCaseParams->RawResponse( rawRespCtr-2 );
							
							//for logging
							HBufC* res16 = HBufC::NewL(response.Size() * 2);
							CleanupStack::PushL(res16);
							res16->Des().Copy(response);
							TPtrC res16Ptr = res16->Des();
							iTestUtils->LogIt(KResponseLog(),rawRespCtr-2,  &res16Ptr);
							CleanupStack::PopAndDestroy(res16);
							
							//ready for checking next response
							(rawResponseCheckingStringBuf->Des()).Copy(KRawResponse());					
							intStr.Format(_L( "%d"), rawRespCtr++);
							(rawResponseCheckingStringBuf->Des()).Append(intStr);
							}

						else if (itemFieldPtr.Compare(*connRequestArrStr) == 0)
							{
							//ConnReqArr
							testCaseParams->SetSequenceArrL(itemValuePtr, ERequestArray);
							TPtrC connRequestArrStrPtr = connRequestArrStr->Des();
							iTestUtils->LogIt(_L("TestCaseParams: %S= %S"),&connRequestArrStrPtr, &itemValuePtr);
							
							//ready for checking next connReqArr
							(connRequestArrStr->Des()).Copy(KTestConn());
							intStr.Format(_L( "%d"), connReqCtr++);
							(connRequestArrStr->Des()).Append(intStr);
							(connRequestArrStr->Des()).Append(KExpectedRequest());
							}
							
						else if (itemFieldPtr.Compare(*connResponseArrStr) == 0)
							{
							//ConnReqArr
							testCaseParams->SetSequenceArrL(itemValuePtr, EResponseArray);
							TPtrC connResponseArrStrPtr = connResponseArrStr->Des();
							iTestUtils->LogIt(_L("TestCaseParams: %S= %S"),&connResponseArrStrPtr, &itemValuePtr);
							
							//ready for checking next connReqArr
							(connResponseArrStr->Des()).Copy(KTestConn());
							intStr.Format(_L( "%d"), connRespCtr++);
							(connResponseArrStr->Des()).Append(intStr);
							(connResponseArrStr->Des()).Append(KExpectedResponse());
							}
						
						else if (itemFieldPtr.Compare(*transTypeStringBuf) == 0)
							{
							//TransType
							TInt returnInt = testCaseParams->SetTransactionTypeL(( transTypeCtr - 1 ), itemValuePtr);
							TPtrC transTypeStringBufPtr = transTypeStringBuf->Des();
							iTestUtils->LogIt(_L("TestCaseParams: %S= %S"), &transTypeStringBufPtr, &itemValuePtr);
							
							TransInfo transTypeInfo = testCaseParams->TransactionType((transTypeCtr - 1));
							
							//ready for checking next transType
							(transTypeStringBuf->Des()).Copy(KTransType());					
							intStr.Format(_L( "%d"), transTypeCtr++);
							(transTypeStringBuf->Des()).Append(intStr);
							}
						
						else if (itemFieldPtr.Compare(*hdrName) == 0)				
							{
							//HeaderName
							testCaseParams->SetHeaderToProcessL(hdrNameCtr-1, itemValuePtr);
							TPtrC8 hdr2Process = testCaseParams->HeaderToProcess(hdrNameCtr-2);
							
							//for logging
							HBufC *hdr16 = HBufC::NewLC(hdr2Process.Size() * 2);
							hdr16->Des().Copy(hdr2Process);
							TPtrC hdr16Ptr = hdr16->Des();
							iTestUtils->LogIt(KHeaderLog(),hdrNameCtr-1,  &hdr16Ptr);
							CleanupStack::PopAndDestroy(hdr16);
							
							//Get ready to look for next header
							//ready for checking next response
							(hdrName->Des()).Copy(KHeaderName());					
							intStr.Format(_L( "%d"), hdrNameCtr++);
							(hdrName->Des()).Append(intStr);
							}
							
						else if ((itemFieldPtr.Compare(*hdrVal) == 0) || (itemFieldPtr.Compare(*hdrValNext) == 0))				
							{
							
							if ((itemFieldPtr.Compare(*hdrValNext) == 0))
								{
								(hdrValNext->Des()).Copy(KHeaderName());					
								intStr.Format(_L( "%d"), hdrNameCtr - 1);
								(hdrValNext->Des()).Append(intStr);
								(hdrValNext->Des()).Append(KHeaderVal());
								intStr.Format(_L("%d"), 1);
								(hdrValNext->Des()).Append(intStr);	
								
								hdrValCtr = 2;
								}
					
							//Header Value Name
							testCaseParams->SetHdrValL(hdrNameCtr-2, hdrValCtr-1, itemValuePtr);
							TPtrC8 hdrValName = testCaseParams->HeaderVal(hdrNameCtr-2, hdrValCtr-1);
							
							//for logging
							HBufC* hdrVal16 = HBufC::NewL(hdrValName.Size() * 2);
							CleanupStack::PushL(hdrVal16);
							hdrVal16->Des().Copy(hdrValName);
							TPtrC hdrVal16Ptr = hdrVal16->Des();
							iTestUtils->LogIt(KHeaderValLog(),hdrNameCtr-2, hdrValCtr-2,  &hdrVal16Ptr);
							CleanupStack::PopAndDestroy(hdrVal16);
							
							//Get ready to look for next header
							//ready for checking next response
							(hdrVal->Des()).Copy(KHeaderName());
							intStr.Format(_L( "%d"), hdrNameCtr - 2);	
							(hdrVal->Des()).Append(intStr);						
							(hdrVal->Des()).Append(KHeaderVal());					
							intStr.Format(_L( "%d"), hdrValCtr++);
							(hdrVal->Des()).Append(intStr);			
							}
							
						
						}//should have read all available params and values for a single test case

						CleanupStack::PopAndDestroy( hdrValNext );
						
						CleanupStack::PopAndDestroy( hdrVal );
						
						CleanupStack::PopAndDestroy( hdrName );
						
						CleanupStack::PopAndDestroy( transTypeStringBuf );
						
						CleanupStack::PopAndDestroy( connResponseArrStr );
						
						CleanupStack::PopAndDestroy( connRequestArrStr );
						
						CleanupStack::PopAndDestroy( rawResponseCheckingStringBuf );			
						
						CleanupStack::PopAndDestroy( rawRequestCheckingStringBuf);
						
						CleanupStack::PopAndDestroy( uriCheckingStringBuf );
				
						
						//Valid parameters? Need to fill up Validate & the switch handles
						TRAPD(err, testCaseParams->ValidateL());
						if (err == KErrNone)
							{
							CPipeliningTestCase* testCase1 = CPipeliningTestCase::NewL(*iTestUtils, *testCaseParams);
							CleanupStack::PushL(testCase1);
							RunTestL(secName, *testCase1);
							CleanupStack::PopAndDestroy(testCase1);	
							}
						else
							{
							TPtrC testCaseNamePtr(testCaseParams->TestCaseName()) ;
							switch (err)
								{
								case KErrGeneral :
									iTestUtils->LogIt(_L("TestCaseParams->ValidateL(): %S : Invalid parameters"), &testCaseNamePtr );
									break;							
								default:
									iTestUtils->LogIt(_L("TestCaseParams->ValidateL(): %S : Invalid parameters"), &testCaseNamePtr);
									break;
								}
							}
						
						
						testCaseParams->SetErrorL(_L("KErrNone"));
						CleanupStack::Pop( testCaseParams );
						delete testCaseParams;
						}
				break;
				}
				
			}
		CleanupStack::PopAndDestroy(iniFile);	
		}
	}

void CPipeliningTestEngine::RunTestL(const TDesC& aSecName, MPipeliningTestCase& aTestCase)
	{
	// Run a single test case
	iTestCase = &aTestCase;
	iTestFailed = EFalse;
	iTestUtils->StartTestL( aTestCase.TestCaseName() );
	__TESTNAME(aTestCase.TestCaseName());
	
	CPipeliningTestClient* testClient = NULL;
	if(aSecName.FindF(_L("TSWDEFECT")) ==0)
	    {
	    RDebug::Print(_L("TSWDEFECT"));
	    _LIT(KTestCaseLocalAndRemoteHost, "TSWDEFECT1 JKUN-7VVGB6");
	    if(aTestCase.TestCaseName().CompareF(KTestCaseLocalAndRemoteHost) == 0)
	        {
	        testClient = CTestCaseLocalAndRemoteHost::NewL(*iTestUtils, *this);
	        CleanupStack::PushL(testClient);
	        }
	    else
	        {
	        User::Invariant();
	        }	     
	    }
	else
	    {
	    testClient = CPipeliningTestClient::NewL(*iTestUtils, *this);   
	    CleanupStack::PushL(testClient);
	    }	
		
	testClient->SetTestCase(&aTestCase);
	iTestServer->SetTestCase(&aTestCase);
	testClient->StartClientL();
	
	CActiveScheduler::Start();
	iTestServer->ResetServer();
    RDebug::Print(_L("After reseting the server: Last error: %d"), iEngineLastError);
	User::LeaveIfError( iEngineLastError );

	CleanupStack::PopAndDestroy(testClient);
    RDebug::Print(_L("Deleting Test client"));
	iTestUtils->EndTest(iTestFailed);
	}

void CPipeliningTestEngine::DoPipeliningConfigTestsL(TDesC& secName)
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
	if( secName.Compare(_L("CONFIG_TEST1")) == 0 )
		{
		RunTestL(KNullDesC(), *test);
		}
	
	// 4 transactions to same location, with limit to pipeline upto 2 at a time
	// 2 connections expected
	test->AddTransactionUrisL(KUri1, 2);
	test->SetMaxNumberTransactionsToPipeline(2);
	if( secName.Compare(_L("CONFIG_TEST2")) == 0 )
		{
		RunTestL(KNullDesC(), *test);
		}

	// 1 transaction to one location, 3 to another with limit to pipeline upto 2 at a time
	// 3 connections expected
	test->ClearTransactionUris();
	test->AddTransactionUrisL(KUri2);
	test->AddTransactionUrisL(KUri1,3);
	test->SetExpectedNumberConnections(3);
	if( secName.Compare(_L("CONFIG_TEST3")) == 0 )
		{
		RunTestL(KNullDesC(), *test);
		}
	
	// 4 transactions to same location, limit to only pipeline 1 at a time
	// Limit Transport Handlers to 3
	// Expect 3 connections
	test->ClearTransactionUris();
	test->AddTransactionUrisL(KUri1, 4);
	test->SetMaxNumberTransactionsToPipeline(1);
	test->SetMaxNumberTransportHandlers(3);
	if( secName.Compare(_L("CONFIG_TEST4")) == 0 )
		{
		RunTestL(KNullDesC(), *test);
		}
			
	CleanupStack::PopAndDestroy(test);
	}
	

void CPipeliningTestEngine::EndTest(TInt aError)
	{
	if( aError < KErrNone )
		{
		iEngineLastError = aError;
		iTestFailed = ETrue;
		_LIT(KTxtEndTestFail, "Engine - Test Failed. Completed with ErrorCode: %d");
		iTestUtils->LogIt(KTxtEndTestFail(), aError);
		}
	else
		{
		__ASSERT_DEBUG( iTestCase != NULL, User::Invariant() );
		
		// Check that the number connections matched the expected number of connections
		const TInt expectedConnections = iTestCase->ConnectionCount();
		const TInt actualConnections = iTestServer->ConnectionCount();
		_LIT(KTxtEndTestConnectionCount, "Engine - Connection count- Expected: %d, Actual: %d");
		iTestUtils->LogIt(KTxtEndTestConnectionCount(), expectedConnections, actualConnections);
		//Actual Connection is 3, this is to support changes made as part of defect: INC099971
		_LIT(KTestCaseLocalAndRemoteHost, "TSWDEFECT1 JKUN-7VVGB6");
		if((iTestCase->TestCaseName().CompareF(KTestCaseLocalAndRemoteHost) != 0) && actualConnections < expectedConnections  && !(expectedConnections == 1 && actualConnections == 3))
			{
			iEngineLastError = KErrGeneral;
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

