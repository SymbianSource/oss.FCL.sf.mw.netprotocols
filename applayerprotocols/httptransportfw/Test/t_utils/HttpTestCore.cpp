// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
    
#include "HttpTestCore.h"

_LIT(KTextFailure,"Test failed with error %d.\n");
_LIT(KTextFailedTest,"TEST %S FAILED\n");
_LIT(KTextPassedTest,"TEST %S PASSED\n");
_LIT(KTextInterruptTest, "TEST %S INTERRUPTED\n");

// Dummy POST body provided in test base should a specific test not want to generate its own
_LIT8(KTestPostBody, "a=10\r\n");

// format for output of data/time values
_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");


// Test harness requirements:
// Uses an active object state machine to run tests.


//##ModelId=3A76DDA701A2
EXPORT_C void CTestScheduler::Error(TInt anError) const
	{
	_LIT(KTestPanic,"CTestScheduler RunL leave");
	User::Panic(KTestPanic,anError);
	}

// CHttpTestEngine

EXPORT_C void CHttpTestEngine::SetCurrentStatusCode(TInt aStatusCode)
	{
	iCurrentStatusCode=aStatusCode;
	}


void CHttpTestEngine::DoCancel()
	{
	// cancel the current test
	if (iCurrentTest)
		iCurrentTest->Cancel();
	}

void CHttpTestEngine::RunL()
	{
	switch (iState)
		{
	case EIdle:
		{
		TRAPD(err,RunNextTestL())
		if (err!=KErrNone)
			{
			Console().Printf(KTextFailure, err);
			}
		}
		break;
	case ERunningTest:
	case ESingleTestCaseFailed:
		break;
	case EShuttingDown:
		CActiveScheduler::Stop();
		break;
		}
	}


EXPORT_C void CHttpTestEngine::PressAnyKey()
	{
	if (iDetectKeyPress)
		iDetectKeyPress->Cancel();

	iUtils->PressAnyKey();
	
	if (iDetectKeyPress)
		iDetectKeyPress->RequestKey();

	}

void CHttpTestEngine::TestInteractionDetectedL(TTestInteraction aTestInteraction)
	{
	if (aTestInteraction == EStopCurrentTest)
		{
		iCurrentTest->Cancel();
		TestCompleted(KErrTestInterrupted);
		}
	}


/**
 * Static factory function for a new test engine
 */
EXPORT_C CHttpTestEngine* CHttpTestEngine::NewL(const TDesC& aTestTitle, TBool aSilent)
	{
	CHttpTestEngine* self = new(ELeave) CHttpTestEngine(aSilent);
	CleanupStack::PushL(self);
	self->ConstructL(aTestTitle);
	CleanupStack::Pop(self); 
	return self;
	}

CHttpTestEngine::CHttpTestEngine(TBool aSilent)
	: CActive(CActive::EPriorityUserInput), iSilent(aSilent)
	{
	}

/**
 * Constructor for the test engine
 * Creates a console and sets itself active
 */
void CHttpTestEngine::ConstructL(const TDesC& aTestTitle)
	{
	iUtils = CHTTPTestUtils::NewL(aTestTitle);
	iUtils->SetSilent(iSilent);
	iDetectKeyPress = CDetectKeyPress::NewLC(*(iUtils->Test().Console()),*this);
	CleanupStack::Pop(iDetectKeyPress);
	CActiveScheduler::Add(this);
	SetActive();
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);
	}

/** 
 * Destructor for CHttpTestEngine
 * Cancels any outstanding requests and deletes member variables
 */
EXPORT_C CHttpTestEngine::~CHttpTestEngine()
	{
	Cancel();
	iTestSet.Close();
	iTestResults.Close();
	delete iDetectKeyPress;
	delete iUtils;
	}

/** 
 * Configure the set of tests to be done in this run of the test engine.
 */
EXPORT_C void CHttpTestEngine::ConfigureTestSet(RPointerArray<CHttpTestBase> aTestSet)
	{
	iTestSet = aTestSet;
	}

/**
 * This should be called by tests to indicate that they have 
 * completed and whether they were sucessful or not 
 */
EXPORT_C void CHttpTestEngine::TestCompleted(TInt aResult)
	{
	// Log test result.  Override silence to ensure something gets displayed
	TBool wasSilent = iUtils->IsSilent();
	iUtils->SetSilent(EFalse);
	// An expected status code of 0 is used if the test doesn't care about a status code to pass
	if (aResult == 0) 
		{
		iUtils->LogIt(KTextPassedTest, &(iCurrentTest->TestName()));
		iTestResults.Append(ETrue);
		}
	else if (aResult == KErrTestInterrupted)
		{
		iUtils->LogIt(KTextInterruptTest, &(iCurrentTest->TestName()));
		iTestResults.Append(EFalse);
		}
	else
		{
		iUtils->LogIt(KTextFailedTest, &(iCurrentTest->TestName()));
		iTestResults.Append(EFalse);
		}
	iUtils->SetSilent(wasSilent);

	// Inform test utils that this test is over
	iUtils->EndTest(aResult);

	// Switch state so the engine can point at the next test, then set the engine active again 
	iState=EIdle;
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);
	SetActive();
	}


/** 
 * Run the next test
 * The switch statement lists all tests to be run
 * 
 */
void CHttpTestEngine::RunNextTestL()
	{
	// delete the test previously run
	
	delete iCurrentTest;
	iCurrentTest=NULL;

	// Advance to the next test

	iTestIndex++;
	if (iTestIndex <= iTestSet.Count())
		{
		// still going...
		iCurrentTest = iTestSet[iTestIndex - 1];
		iCurrentTest->SetEngine(this);
 		iUtils->StartTestL(iCurrentTest->TestName());
		iState = ERunningTest;
		iCurrentTest->BeginTest();
		}
	else
		{
		// finished testing - set status to shut down and complete our own request
		SetPriority(EPriorityIdle);
		iState = EShuttingDown;
		int a,b;
		TestSummary(a,b);
		TRequestStatus* stat = &iStatus;
		User::RequestComplete(stat,KErrNone);
		SetActive();
		return;
		}
	}

/**
 * return a reference to the console used by the test harness
 */
EXPORT_C CConsoleBase& CHttpTestEngine::Console() const 
	{
	return *(iUtils->Test().Console());
	}

/**
 * return a reference to the test harness utilities
 */
EXPORT_C CHTTPTestUtils& CHttpTestEngine::Utils() const 
	{
	return *iUtils;
	}


EXPORT_C void CHttpTestEngine::TestSummary(TInt& aNumPasses, TInt& aNumFailures) const
	{
	aNumPasses = 0;
	aNumFailures = 0;
	for (TInt ii = 0; ii < iTestResults.Count(); ii++)
		iTestResults[ii]?(aNumPasses++):(aNumFailures++);
	}


//-----CHttpTestBase------------------------------------------------------------------

EXPORT_C CHttpTestBase::CHttpTestBase() : CActive(CActive::EPriorityStandard) 
	{
	iExpectedStatusCode= 200;
	iTestFail=0;
	}

EXPORT_C  TInt CHttpTestBase::ExpectedStatusCode()
	{
	return iExpectedStatusCode;
	}

EXPORT_C void CHttpTestBase::BeginTest()
	{
	CActiveScheduler::Add(this);
	SetActive();
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);
	}

EXPORT_C void CHttpTestBase::RunL()
	{
	// Either do leak tests or regular tests
	TInt err = KErrNone;
	if (iLeakTests > 0)
		{
		TRAP(err,DoLeakTestsL());
		}
	else
		{
		TRAP(err,DoRunL());
		}

	switch(err)
		{
	case KErrNone:
		{
		iEngine->Utils().LogIt(_L("\nTest case finished"));
		if (!iEngine->IsSilent())
			iEngine->PressAnyKey();
		}
		break;
	default:// any error
		{
		iEngine->Utils().LogIt(_L("Test case failed (caught a Leave) ... skipping to the next test case"));
		}
		break;
		}
	iEngine->TestCompleted(err);
	}


void CHttpTestBase::DoLeakTestsL()
	{
	for (TInt ii = iFirstLeakIteration; ii < iLeakTests; ++ii)
		{
		iEngine->Utils().LogIt(_L("Memory Leak Testing on allocation %d\n"), ii);

		TInt err = KErrNoMemory;
		
		__UHEAP_MARK;
		__UHEAP_FAILNEXT(ii);
		TRAP(err,DoRunL());
		__UHEAP_MARKEND;
		User::Heap().Check();
		__UHEAP_RESET;

		// Break out if the test passes successfully; allow only memory failure or test failure codes to proceed.
		if (err == KErrNone)
			return;
		else if (err != KErrNoMemory && err != KErrTestFailed)
			User::Leave(err);
		}
	}
	
EXPORT_C TInt CHttpTestBase::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;	
	}

EXPORT_C void CHttpTestBase::CompleteOwnRequest()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);	
	if (!IsActive())
		SetActive();
	}


//-----CHttpTestTransBase------------------------------------------------------------------

EXPORT_C CHttpTestTransBase::CHttpTestTransBase() : CHttpTestBase() 
	{
	}

EXPORT_C TBool CHttpTestTransBase::GetNextDataPart(TPtrC8& aDataChunk)
	{
	//add a body 
	aDataChunk.Set(KTestPostBody);
	return ETrue;
	}

EXPORT_C void CHttpTestTransBase::ReleaseData() 
	{
	}
	
EXPORT_C TInt CHttpTestTransBase::OverallDataSize()
	{
	return KTestPostBody().Length();
	}

EXPORT_C void CHttpTestTransBase::DumpResponseBody(RHTTPTransaction& aTrans)
//dump all the body's chunks 
	{
	MHTTPDataSupplier* body = aTrans.Response().Body();
	TPtrC8 dataChunk;
	TBool isLast = body->GetNextDataPart(dataChunk);
	iEngine->Utils().DumpData(dataChunk);
	if (isLast && !iEngine->IsSilent())
		iEngine->Utils().LogIt(_L("Got last data chunk.\n"));
	body->ReleaseData();
	}

EXPORT_C void CHttpTestTransBase::DumpRespHeaders(RHTTPTransaction& aTrans)
//dump the message's headers
	{
	//dump the message's headers
	RHTTPResponse resp = aTrans.Response();
	TInt status = resp.StatusCode();
	if (!iEngine->IsSilent())
		iEngine->Utils().LogIt(_L("Status code = %d\n"), status);

	RStringPool strP = aTrans.Session().StringPool();
	RHTTPHeaders hdr = resp.GetHeaderCollection();
	THTTPHdrFieldIter it = hdr.Fields();

	TBuf<32>  fieldName16;
	TBuf<128> fieldVal16;

	while (it.AtEnd() == EFalse)
		{
		RStringTokenF fieldNameTk = it();
		RStringF fieldName = strP.StringF(fieldNameTk);
		THTTPHdrVal hVal;
		if (hdr.GetField(fieldName,0,hVal) == KErrNone)
			{
			TPtrC8 fieldNameStr(strP.StringF(fieldName).DesC());
			if (fieldNameStr.Length() > 32)
				fieldNameStr.Set(fieldNameStr.Left(32));

			fieldName16.Copy(fieldNameStr);

			THTTPHdrVal fieldVal;
			hdr.GetField(fieldName,0,fieldVal);
			switch (fieldVal.Type())
				{
				case THTTPHdrVal::KTIntVal:
					{
					iEngine->Utils().LogIt(_L("%S: %d\n"), &fieldName16, fieldVal.Int());
					} break;
				case THTTPHdrVal::KStrVal:
				case THTTPHdrVal::KStrFVal:
					{
					TPtrC8 fieldValStr(strP.StringF(fieldVal.StrF()).DesC());
					if (fieldValStr.Length() > 128)
						fieldValStr.Set(fieldValStr.Left(128));

					fieldVal16.Copy(fieldValStr);
					iEngine->Utils().LogIt(_L("%S: %S\n"), &fieldName16, &fieldVal16);
					} break;
				case THTTPHdrVal::KDateVal:
					{
					TDateTime date = fieldVal.DateTime();
					TTime t(date);
					TBuf<128> dateTimeString;
					TRAPD(err,t.FormatL(dateTimeString,KDateFormat));
					if (err == KErrNone)
						iEngine->Utils().LogIt(_L("%S: %S\n"), &fieldName16, &dateTimeString);
					} break;
				default:
					{
					iEngine->Utils().LogIt(_L("%S: <unrecognised value type>\n"), &fieldName16);
					}
				}
			}
		++it;
		}
	}

EXPORT_C TInt CHttpTestTransBase::MHFRunError(TInt aError, 
										RHTTPTransaction /*aTransaction*/,
										const THTTPEvent& /*aEvent*/)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aError);
	return KErrNone;
	}

EXPORT_C void CHttpTestTransBase::MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		iEngine->Utils().LogIt(_L("<EGotResponseHeaders>\n"));
		DumpRespHeaders(aTransaction);
		iEngine->SetCurrentStatusCode(aTransaction.Response().StatusCode());
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		iEngine->Utils().LogIt(_L("<EGotResponseBodyData received>\n"));
		// for each chunk of data received we have to empty the buffer before to be able to receive 
		MHTTPDataSupplier* body = aTransaction.Response().Body();
		body->ReleaseData();
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// The transaction's response is complete
		iEngine->Utils().LogIt(_L("<EResponseComplete received >\n"));
		} break;
	case THTTPEvent::ESucceeded:
		{
		iEngine->Utils().LogIt(_L("<ESucceeded received from the VF>\n"));
		CActiveScheduler::Stop();
		} break;
	case THTTPEvent::EFailed:
		{
		iEngine->Utils().LogIt(_L("<EFailed received from the VF>\n"));
		CActiveScheduler::Stop();
		} break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event>\n %d"),aEvent.iStatus);
		iEngine->Utils().LogIt(_L("Test Failed\n"));
		iEngine->PressAnyKey();
		CActiveScheduler::Stop();
		} 
		break;
		}
	}

EXPORT_C void CHttpTestEngine::SetSilent(TBool aSilent)
	{
	iUtils->SetSilent(aSilent);
	}

EXPORT_C  TInt CHttpTestTransBase::Reset()
	{
	return KErrNone;
	}
