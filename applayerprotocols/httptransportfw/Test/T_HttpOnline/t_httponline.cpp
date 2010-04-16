// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Framework includes
#if !defined(__HTTPTESTUTILS_H__)
#include "httptestutils.h"
#endif

#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

#if !defined(__TESTSCRIPTS_H__)
#include "TestScripts.h"
#endif

// Individual test class includes
#if !defined (__CTEXTMODETESTCANCELSINGLETRANS_H__)
#include "T_CancelSingleTrans.h"
#endif
#if !defined (__CTEXTMODETESTPERSISTENTCONN_H__)
#include "T_PersistentConn.h"
#endif
#if !defined (__CTEXTMODETESTCANCELMULTITRANS_H__)
#include "T_CancelMultiTrans.h"
#endif
#if !defined (__CTEXTMODETESTPOSTBODYCHUNKS_H__)
#include "T_PostBodyChunks.h"
#endif
#if !defined (__CTEXTMODETESTMULTITRANS_H__)
#include "T_MultiTrans.h"
#endif
#if !defined (__CTEXTMODETESTMULTITRANSCONCURRENT_H__)
#include "T_MultiTransConcurrent.h"
#endif
#if !defined (__CTEXTMODETESTMULTITRANSMALFORMEDSTATUSLINE_H__)
#include "T_MultiTransMalformedStatusLine.h"
#endif
#if !defined (__CTEXTMODETESTRESPCHUNKSIZE_H__)
#include "T_RespChunkSize.h"
#endif
#if !defined (__CTEXTMODETESTPROXIES_H__)
#include "T_Proxies.h"
#endif

#include "T_Cookies.h"
#include "DEF079378.h"
#include "CINC102380.h"
#include "INC094209.h"
#include "CINC097492.h"
#include "CINC097492_2.h"
#include "CINC097492_3.h"
#include "CINC097492_4.h"
#include "CINC110203.h"
#include "T_UAProf.h"
#include "T_SocketShutdownMode.h"
#include "cinc112633.h"
#include "cinc112633_2.h"
#include "CPDEF143929.h"

#include "csrvaddrval.h"
#include "CINC082448.h"
#include "cinc105767.h"
#include "CR1130.h"
#include "t_100continuetest.h"
#include "cinc120672.h"
#include "crecvbufsizetest1.h"
#include "crecvbufsizetest2.h"


// Constants used in this file
enum TPanic
	{
	EFailedToCreateEngine,
	EFailedToReadIniFile
	};

// Name of test harness
_LIT(KHttpOnlineTestTitle, "HTTP Online Unit Tests");

// Location of INI file that defines the test cases to be run
_LIT(KHttpOnlineTestCompName, 	"online");
_LIT(KHttpOnlineTestScriptName, "UnitTests.ini");

// Labels for resources in the INI file
_LIT(KRunTestItemName, 						"RunTest");
_LIT(KDefaultSection, 						"Defaults");
_LIT(KSilenceItemName, 						"Silent");
_LIT(KLeakTestCount, 						"LeakTestCount");
_LIT(KFirstLeakIteration, 					"FirstLeakIteration");

// Test names, used as section headings in the .ini file, and any 
// resource names associated with the test
_LIT(KTestPostBodyChunks,					"PostBodyChunks");
//

_LIT(KTestMultiTrans,						"MultiTrans");
_LIT(KTestMultiTransConcurrent,				"MultiTransConcurrent");
_LIT(KTestMultiTransMalformedStatusLine,	"MultiTransMalformedStatusLine");

// used to specify the number of transactions to run in parallel
_LIT(KResNumTrans,							"NumTrans"); 
//
_LIT(KTestPersistentConn,					"PersistentConn");
_LIT(KResHostName,							"HostName");
_LIT(KResPath,								"Path");
//
_LIT(KTestProxies,							"Proxies");
_LIT(KResNumSubTests,						"NumSubTests");
_LIT(KResProxyAddr,							"ProxyAddr");
//
_LIT(KResSecureHostName,					"SecureHostName");
//
_LIT(KTestRespChunkSize,					"RespChunkSize");
_LIT(KResMinChunkSize,						"MinChunkSize");
_LIT(KResMaxChunkSize,						"MaxChunkSize");
_LIT(KResChunkSizeStep,						"ChunkSizeStep");
_LIT(KResProxyPort,							"ProxyPort");
//
_LIT(KTestCancelSingleTrans,				"CancelSingleTrans");
_LIT(KResCancelDelay,						"CancelDelay");
//
_LIT(KTestCancelMultiTrans,					"CancelMultiTrans");
_LIT(KTestCookies,							"Cookies");
//
_LIT(KTestUAProf,							"UAProf");
//
_LIT(KTestSocketShutdownMode,				"SocketShutdownMode");
_LIT(KTestSocketShutdownModeImmediate, 		"ImmediateShutdown");
_LIT(KTextSocketShutdownModeInterrupt, 		"Interrupt");
_LIT(KTestAuthentication, 					"Authentication");
_LIT(KTestReconnectAfterConnectionDrop, 	"ReconnectAfterConnectionDrop");
_LIT(KTestINC094209,						"INC094209");
_LIT(KTestCINC102380,						"INC102380");
_LIT(KTestINC097492,						"INC097492");
_LIT(KTestCINC105767,						"INC105767");
_LIT(KTestCR1130,							"CR1130");
_LIT(KTestINC112633,						"INC112633");
_LIT(KTestINC110203,						"INC110203");
_LIT(KTest100Continue,						"100-ContinueTest");
_LIT(KTestCINC120672_1,						"INC120672_1");
_LIT(KTestCINC120672_2,						"INC120672_2");
_LIT(KSettingsIniCompName,					"settings");
_LIT(KSettingsIniName,						"settings.ini");
_LIT(KRecvBufSizeTest1,						"RecvBufSizeTest1");
_LIT(KRecvBufSizeTest2,						"RecvBufSizeTest2");
_LIT(KTestCPDEF143929,						"PDEF143929");


const TInt KMaxHeapSize = 0x100000;
const TInt KMaxIntDesLen = 10;				// Des length required for displaying max 32 bit int
_LIT(KTestThreadName,						"TestThread");
_LIT(KRtestFailureLabel, 					"RTEST: FAILED due to a %S %d Panic ");
_LIT(KPanicOnLeave,							"Leave DoTestL()");// Panic if DeTestL() does a Leave().

/**
	This function is called the E32Main function to configure and start the
	execution of the tests.

	@leave	Leaves with a standard error, Failed to create engine, Failed to
			read ini file
*/
void DoTestsL()
	{
	// The following two lines create a semaphore which removes the security dialog
	// displayed when an untrusted certificate is received from a SSL server
	RSemaphore semaphore;
	semaphore.CreateGlobal(_L("T_AUTOSSL"), 0);

	TBool runTestDefault = EFalse;
	CTestScheduler* as = new(ELeave) CTestScheduler;
	CleanupStack::PushL(as);
	if (!as)
		{
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
		}
	CActiveScheduler::Install(as); // Install active scheduler
	
	// Initialise Comms, required in a minimal console environment
	CHTTPTestUtils::InitCommsL();

	// Force a connection to be held open keep NT RAS connected...
	CHTTPTestUtils::HoldOpenConnectionL();
	
	// Create the test engine
	CHttpTestEngine* engine=NULL;
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpOnlineTestTitle(), EFalse));
	if (err != KErrNone)
		{
		User::Panic(_L("T_HTTPONLINE"),EFailedToCreateEngine);
		}
	CleanupStack::PushL(engine);

	engine->Utils().LogIt(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-FRAMEWORK-T_HTTPONLINE-0001 "));
	// Open the INI file to initialise the tests we're going to run
	CScriptFile* iniFile = NULL;
	TRAP(err, iniFile = CScriptFile::NewL(engine->Utils(), KHttpOnlineTestCompName, KHttpOnlineTestScriptName()));
	if (err != KErrNone)
		{
		User::Panic(_L("T_HTTPONLINE"),EFailedToReadIniFile);
		}
	CleanupStack::PushL(iniFile);

	// Open the settings.ini file which has the server IP/Name that would
	// be used by the tests.
	CScriptFile* iniSettingsFile = NULL;
	TRAP(err, iniSettingsFile = CScriptFile::NewL(engine->Utils(), KSettingsIniCompName, KSettingsIniName()));
	if (err != KErrNone)
		{
		User::Panic(KHttpOnlineTestCompName(),EFailedToReadIniFile);
		}
	CleanupStack::PushL(iniSettingsFile);

	// Read the default silent setting for the tests
	engine->Utils().SetSilent(iniFile->ItemValue(KDefaultSection, KSilenceItemName, EFalse));

	// Create the tests we're going to run.  Ownership is transferred to the 
	// test engine
	RPointerArray<CHttpTestBase> onlineTests;
	CleanupClosePushL(onlineTests);

	// Create an array of sections from the script / ini file.
	CArrayPtrFlat<CScriptSection>& sections=iniFile->Sections();
	//create the  tests for iteration2 and  append them in the array
	TInt secCount = sections.Count();
	TInt count= secCount;
	CScriptSection* section = NULL;
	
	for (TInt i=0;i<count;i++)
		{
		//create an array of tests to be sent to the engine
		section = sections[i];
		_LIT(KZero, "0");
		if (section->SectionName().CompareF(KDefaultSection) == 0)
			{
			if (iniFile->Section(i).ItemL(KSilenceItemName).Value().CompareF(KZero) == 0)
				{
   				engine->SetSilent(EFalse);
				}
 			else
				{
				engine->SetSilent(ETrue);
				}
 			if (iniFile->Section(i).ItemL(KRunTestItemName).Value().CompareF(KZero) != 0)
				{
				runTestDefault = ETrue;
				}
			}

		// Single POST test; body data provided in timed chunks
		if ( section->SectionName().CompareF(KTestPostBodyChunks) == 0)
			{
			TBool runTest = runTestDefault;			
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
				{
				CTextModeTestPostBodyChunks* postChunksTest = CTextModeTestPostBodyChunks::NewL(iniSettingsFile);
				CleanupStack::PushL(postChunksTest);
				onlineTests.Append(postChunksTest);
				
				TInt numLeakTests = iniFile->ItemValue(KTestPostBodyChunks(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(KTestPostBodyChunks(), KFirstLeakIteration(), 0);
				postChunksTest->SetLeakTests(numLeakTests, firstLeakIter);
				CleanupStack::Pop(postChunksTest);
				}
			}

		// Parallel transactions test  
		if ( section->SectionName().CompareF(KTestMultiTrans) == 0)
			{
			TBool runTest = runTestDefault;			
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
				{
				CTextModeTestMultiTrans* multiTransTest;
				TInt numTrans = iniFile->ItemValue(KTestMultiTrans(), KResNumTrans(), 8); // 8 transactions is default
				
				// Retrieve the host name from script / ini file.
				_LIT(KDefaultHostName, "WapTestIP");
				const TDesC& hostName = iniFile->ItemValue(KTestMultiTrans(), KResHostName(), KDefaultHostName());

				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(hostName, iniSettingsFile);
				CleanupStack::PushL(newUrl16);
				TPtr newUrlPtr16 = newUrl16->Des();

				TInt num;
				for (num = 1; num <= numTrans; num++)
					{
					multiTransTest = CTextModeTestMultiTrans::NewL(newUrlPtr16, num);
					CleanupStack::PushL(multiTransTest);
					onlineTests.Append(multiTransTest);

					TInt numLeakTests = iniFile->ItemValue(KTestMultiTrans(), KLeakTestCount(), 0);
					TInt firstLeakIter = iniFile->ItemValue(KTestMultiTrans(), KFirstLeakIteration(), 0);
					multiTransTest->SetLeakTests(numLeakTests, firstLeakIter);
					}

				CleanupStack::Pop(num-1);
				CleanupStack::PopAndDestroy(newUrl16);
				}
			}
			
 		// Parallel transactions test  
 		if ( section->SectionName().CompareF(KTestMultiTransConcurrent) == 0)
 			{
 			TBool runTest = runTestDefault;			
 			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
  				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
  			if (runTest)
 				{
 				CTextModeTestMultiTransConcurrent* multiTransTest;
 				TInt numTrans = iniFile->ItemValue(KTestMultiTrans(), KResNumTrans(), 8); // 8 transactions is default

				_LIT(KDefaultHostName, "WapTestIP");
				const TDesC& hostName = iniFile->ItemValue(KTestMultiTransConcurrent(), KResHostName(), KDefaultHostName());

				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(hostName, iniSettingsFile);
				CleanupStack::PushL(newUrl16);
				TPtr newUrlPtr16 = newUrl16->Des();

				multiTransTest = CTextModeTestMultiTransConcurrent::NewL(newUrlPtr16, numTrans);

				CleanupStack::PopAndDestroy(newUrl16);

 				CleanupStack::PushL(multiTransTest);
 				onlineTests.Append(multiTransTest);
 
 				TInt numLeakTests = iniFile->ItemValue(KTestMultiTrans(), KLeakTestCount(), 0);
 				TInt firstLeakIter = iniFile->ItemValue(KTestMultiTrans(), KFirstLeakIteration(), 0);
 				multiTransTest->SetLeakTests(numLeakTests, firstLeakIter);

				CleanupStack::Pop(multiTransTest);
 				}
 			}
   
 		// Parallel transactions test  
 		if ( section->SectionName().CompareF(KTestMultiTransMalformedStatusLine) == 0)
 			{
 			TBool runTest = runTestDefault;			
 			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
  			if (itemPtr.CompareF(KRunTestItemName)==0)
  				{
  				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
  				}
  			if (runTest)
 				{
 				CTextModeTestMultiTransMalformedStatusLine* multiTransTest;
 				TInt numTrans = iniFile->ItemValue(KTestMultiTransMalformedStatusLine(), KResNumTrans(), 1);

 				// The host here is a special test server that will always 
				// serve malform status lines
				_LIT(KDefaultHostName, "http://JigsawTestIP:9002");
				const TDesC& hostName = iniFile->ItemValue(KTestMultiTransMalformedStatusLine(), KResHostName(), KDefaultHostName());
				
				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(hostName, iniSettingsFile);
				CleanupStack::PushL(newUrl16);
				TPtr newUrlPtr16 = newUrl16->Des();

				TInt num;
 				for (num = 1; num <= numTrans; num++)
 					{
					multiTransTest = CTextModeTestMultiTransMalformedStatusLine::NewL(newUrlPtr16, num);
					CleanupStack::PushL(multiTransTest);
 					onlineTests.Append(multiTransTest);
 
 					TInt numLeakTests = iniFile->ItemValue(KTestMultiTrans(), KLeakTestCount(), 0);
 					TInt firstLeakIter = iniFile->ItemValue(KTestMultiTrans(), KFirstLeakIteration(), 0);
 					multiTransTest->SetLeakTests(numLeakTests, firstLeakIter);
 					}

				CleanupStack::Pop(num-1);
				CleanupStack::PopAndDestroy(newUrl16);
 				}
 			}

		// Do the persistent connection test five times, each with one 
		// additional transaction
		if ( section->SectionName().CompareF(KTestPersistentConn) == 0)
			{
			TBool runTest = runTestDefault;			
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
				{
				CTextModeTestPersistentConn* persistConnTest;
				TInt numTrans = iniFile->ItemValue(KTestPersistentConn(), KResNumTrans(), 5); // 5 transactions is default

				_LIT(KDefaultHostName, "WapTestIP");
				_LIT(KDefaultPath, "/perl/dumpform.pl");
				const TDesC& hostName = iniFile->ItemValue(KTestPersistentConn(), KResHostName(), KDefaultHostName());
				const TDesC& path = iniFile->ItemValue(KTestPersistentConn(), KResPath(), KDefaultPath());

				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(hostName, iniSettingsFile);
				CleanupStack::PushL(newUrl16);
				TPtr newUrlPtr16 = newUrl16->Des();

				TInt num;
				for (num = 1; num <= numTrans; num++)
					{
					persistConnTest = CTextModeTestPersistentConn::NewL(newUrlPtr16, path, num);
					CleanupStack::PushL(persistConnTest);
					onlineTests.Append(persistConnTest);

					TInt numLeakTests = iniFile->ItemValue(KTestPersistentConn(), KLeakTestCount(), 0);
					TInt firstLeakIter = iniFile->ItemValue(KTestPersistentConn(), KFirstLeakIteration(), 0);
					persistConnTest->SetLeakTests(numLeakTests, firstLeakIter);
					}

				CleanupStack::Pop(num-1);
				CleanupStack::PopAndDestroy(newUrl16);
				}
			}

		// Proxies test
		if ( section->SectionName().CompareF(KTestProxies) == 0)
			{
			TBool runTest = runTestDefault;			
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
				{
				CTextModeTestProxies* proxiesTest;
				TInt numSubTests = iniFile->ItemValue(KTestProxies(), KResNumSubTests(), 10); // 10 sub-tests is default

				_LIT(KDefaultProxy, "WapTestIP:9003");
				_LIT(KDefaultHostName, "WapTestIP");
				const TDesC& proxy = iniFile->ItemValue(KTestProxies(), KResProxyAddr(), KDefaultProxy());
				const TDesC& host = iniFile->ItemValue(KTestProxies(), KResHostName(), KDefaultHostName());

				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(host, iniSettingsFile);
				CleanupStack::PushL(newUrl16);
				TPtr16 newUrlPtr16 = newUrl16->Des();

				HBufC16* proxy16 = TSrvAddrVal::ReplaceHostNameL(proxy, iniSettingsFile);
				CleanupStack::PushL(proxy16);
				TPtr16 proxyPtr16 = proxy16->Des();

				TBuf8<32> proxy8;
				proxy8.Copy(proxyPtr16);
				TBuf8<32> host8;
				host8.Copy(newUrlPtr16);

				TInt num;
				for (num = 1; num <= numSubTests; num++)
					{
					proxiesTest = CTextModeTestProxies::NewL(num, proxy8, host8);
					CleanupStack::PushL(proxiesTest);
					onlineTests.Append(proxiesTest);
					
					TInt numLeakTests = iniFile->ItemValue(KTestProxies(), KLeakTestCount(), 0);
					TInt firstLeakIter = iniFile->ItemValue(KTestProxies(), KFirstLeakIteration(), 0);
					proxiesTest->SetLeakTests(numLeakTests, firstLeakIter);
					}

				CleanupStack::Pop(num-1);
				CleanupStack::PopAndDestroy(2, newUrl16); // and proxy16
				}
			}


		// Do the response chunk size test 32 times (to cover all possible 
		// chunk boundaries in the header) 
		if ( section->SectionName().CompareF(KTestRespChunkSize) == 0)
			{
			TBool runTest = runTestDefault;			
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
				{
				CTextModeTestRespChunkSize* respChunkSizeTest;
				TInt min = iniFile->ItemValue(KTestRespChunkSize(), KResMinChunkSize(), 8); // minimum size of 8 bytes
				TInt max = iniFile->ItemValue(KTestRespChunkSize(), KResMaxChunkSize(), 256); // maximum size of 256 bytes
				// stepping up by 8 bytes each time
				TInt step = iniFile->ItemValue(KTestRespChunkSize(), KResChunkSizeStep(), 8);
				// port number the dummy proxy will listen on
				TInt port = iniFile->ItemValue(KTestRespChunkSize(), KResProxyPort(), 8000); 

				TInt num;
				TInt count=0;
				for (num = min; num <= max; num+=step)
					{
					++count;
					respChunkSizeTest = CTextModeTestRespChunkSize::NewL(port, num, iniSettingsFile);
					CleanupStack::PushL(respChunkSizeTest);
					TInt numLeakTests = iniFile->ItemValue(KTestRespChunkSize(), KLeakTestCount(), 0);
					TInt firstLeakIter = iniFile->ItemValue(KTestRespChunkSize(), KFirstLeakIteration(), 0);
					respChunkSizeTest->SetLeakTests(numLeakTests, firstLeakIter);
					onlineTests.Append(respChunkSizeTest);
					}

				CleanupStack::Pop(count);
				}
			}

		// Do the single trans cancellation test fifty times at increasing
		// intervals 
		if ( section->SectionName().CompareF(KTestCancelSingleTrans) == 0)
			{
			TBool runTest = runTestDefault;			
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
 				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
 				}
 			if (runTest)
				{
				CTextModeTestCancelSingleTrans* singleCancelTest;

				_LIT(KDefaultHostName, "WapTestIP");
				const TDesC& hostName = iniFile->ItemValue(KTestCancelSingleTrans(), KResHostName(), KDefaultHostName());

				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(hostName, iniSettingsFile);
				CleanupStack::PushL(newUrl16);
				TPtr16 newUrlPtr16 = newUrl16->Des();

				TInt numSubTests = iniFile->ItemValue(KTestCancelSingleTrans(), KResNumSubTests(), 50); // 50 sub-tests is default
				TInt delay = iniFile->ItemValue(KTestCancelSingleTrans(), KResCancelDelay(), 50000); // 50ms is default

				TInt num;
				for (num = 1; num <= numSubTests; num++)
					{
					singleCancelTest = CTextModeTestCancelSingleTrans::NewL(newUrlPtr16, num, num*delay, EFalse);
					CleanupStack::PushL(singleCancelTest);
					TInt numLeakTests = iniFile->ItemValue(KTestCancelSingleTrans(), KLeakTestCount(), 0);
					TInt firstLeakIter = iniFile->ItemValue(KTestCancelSingleTrans(), KFirstLeakIteration(), 0);
					singleCancelTest->SetLeakTests(numLeakTests, firstLeakIter);
					onlineTests.Append(singleCancelTest);
					}

				CleanupStack::Pop(num-1);
				CleanupStack::PopAndDestroy(newUrl16);

				// repeat the test using a secure host
				_LIT(KDefaultSecureHostName, "SecureServerIP");
				const TDesC& secureHostName = iniFile->ItemValue(KTestCancelSingleTrans(), KResSecureHostName(), KDefaultSecureHostName());
				
				HBufC16* secNewUrl16 = TSrvAddrVal::ReplaceHostNameL(secureHostName, iniSettingsFile);
				CleanupStack::PushL(secNewUrl16);
				TPtr16 secNewUrlPtr16 = secNewUrl16->Des();

				TInt secureNum;
				for (secureNum = 1; secureNum <= numSubTests; secureNum++)
					{
					singleCancelTest = CTextModeTestCancelSingleTrans::NewL(secNewUrlPtr16, secureNum, secureNum*delay, ETrue);
					CleanupStack::PushL(singleCancelTest);
					TInt numLeakTests = iniFile->ItemValue(KTestCancelSingleTrans(), KLeakTestCount(), 0);
					TInt firstLeakIter = iniFile->ItemValue(KTestCancelSingleTrans(), KFirstLeakIteration(), 0);
					singleCancelTest->SetLeakTests(numLeakTests, firstLeakIter);
					onlineTests.Append(singleCancelTest);
					}

				CleanupStack::Pop(secureNum-1);
				CleanupStack::PopAndDestroy(secNewUrl16);
				}
			}

		// Multi-transaction cancellation tests. Each test set adds one more
		// transaction to the group which is set off simultaneously at the 
		// start. The middle loop modifies which test in the test set is to be 
		// cancelled.  The inner loop increases the time interval for the 
		// cancellation
		if ( section->SectionName().CompareF(KTestCancelMultiTrans) == 0)
			{
			TBool runTest = runTestDefault;
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
				{
				TInt count = 0;
				CTextModeTestCancelMultiTrans* multiCancelTest;

				_LIT(KDefaultHostName, "WapTestIP");
				const TDesC& hostName = iniFile->ItemValue(KTestCancelMultiTrans(), KResHostName(), KDefaultHostName());
				
				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(hostName, iniSettingsFile);
				CleanupStack::PushL(newUrl16);
				TPtr16 newUrlPtr16 = newUrl16->Des();

				TInt numTrans = iniFile->ItemValue(KTestCancelMultiTrans(), KResNumTrans(), 12); // 12 transactions is default
				TInt numSubTests = iniFile->ItemValue(KTestCancelMultiTrans(), KResNumSubTests(), 2); // 2 sub-tests per iteration
				TInt delay = iniFile->ItemValue(KTestCancelMultiTrans(), KResCancelDelay(), 200000); // 0.2s is default
				for (TInt numTransIdx = 1; numTransIdx <= numTrans; numTransIdx++)
					for (TInt transIdxToCancel = 0; transIdxToCancel <= numTransIdx - 1; transIdxToCancel++)
						for (TInt testNum = 1; testNum <= numSubTests; testNum++) // increases the delay before cancelling
							{
							++count;
							multiCancelTest = CTextModeTestCancelMultiTrans::NewL(newUrlPtr16, numTransIdx, testNum, testNum*delay, transIdxToCancel);
							CleanupStack::PushL(multiCancelTest);
							TInt numLeakTests = iniFile->ItemValue(KTestCancelMultiTrans(), KLeakTestCount(), 0);
							TInt firstLeakIter = iniFile->ItemValue(KTestCancelMultiTrans(), KFirstLeakIteration(), 0);
							multiCancelTest->SetLeakTests(numLeakTests, firstLeakIter);
							onlineTests.Append(multiCancelTest);
							engine->Utils().LogIt(_L("%d "), count);
							}

				CleanupStack::Pop(count);
				CleanupStack::PopAndDestroy(newUrl16);
				}
			}

		// Cookies testing
		if ( section->SectionName().CompareF(KTestCookies) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCookies(), KResNumSubTests(), 12); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CTextModeTestCookies* cookiesTest = CTextModeTestCookies::NewL(index + 1, iniSettingsFile); //Tests are 1-13
					CleanupStack::PushL(cookiesTest);
					onlineTests.Append(cookiesTest);
					}
				CleanupStack::Pop(index);
				}
			}

		// UAProf testing
		if ( section->SectionName().CompareF(KTestUAProf) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				CTestUAProf* uaprofTest;

				_LIT(KDefaultProxy, "WapGatewayAddress");
				_LIT(KDefaultHostName, "http://WapTestIP/wap/wstiot/welcome.wml");
				const TDesC& proxy = iniFile->ItemValue(KTestUAProf(), KResProxyAddr(), KDefaultProxy()); // Internal test WAP G/W
				const TDesC& host = iniFile->ItemValue(KTestUAProf(), KResHostName(), KDefaultHostName());
				
				HBufC16* newUrl16 = TSrvAddrVal::ReplaceHostNameL(host, iniSettingsFile);
				CleanupStack::PushL(newUrl16);

				HBufC16* proxy16 = TSrvAddrVal::ReplaceHostNameL(proxy, iniSettingsFile);
				CleanupStack::PushL(proxy16);

				TBuf8<32> proxy8;
				proxy8.Copy(newUrl16->Des());
				TBuf8<32> host8;
				host8.Copy(proxy16->Des());

				const TInt numSubTests = iniFile->ItemValue(KTestUAProf(), KResNumSubTests(), 6); // 6 sub-tests is default

				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					uaprofTest = CTestUAProf::NewL((index + 1), proxy8, host8); //Tests are 1-6
					CleanupStack::PushL(uaprofTest);

					TInt numLeakTests = iniFile->ItemValue(KTestUAProf(), KLeakTestCount(), 0);
					TInt firstLeakIter = iniFile->ItemValue(KTestUAProf(), KFirstLeakIteration(), 0);
					uaprofTest->SetLeakTests(numLeakTests, firstLeakIter);
					onlineTests.Append(uaprofTest);
					}

				CleanupStack::Pop(index);
				CleanupStack::PopAndDestroy(2, newUrl16); // and proxy16
				}
			}
			
		// Socket Shutdown Mode test  
		if ( section->SectionName().CompareF(KTestSocketShutdownMode) == 0)
			{
			TBool runTest = runTestDefault;			
			TPtrC itemPtr(iniFile->Section(i).Item(0).Item());
 			if (itemPtr.CompareF(KRunTestItemName)==0)
				{
 				runTest = !(iniFile->Section(i).Item(0).Value().CompareF(KZero) == 0);
				}
 			if (runTest)
				{
				CSocketShutdownModeTest* socketShutdownModeTest;
				TBool interrupt = iniFile->ItemValue(KTestSocketShutdownMode(), KTextSocketShutdownModeInterrupt(), EFalse);
				TBool immediateShutDown = iniFile->ItemValue(KTestSocketShutdownMode(), KTestSocketShutdownModeImmediate(), ETrue);
				
				socketShutdownModeTest = CSocketShutdownModeTest::NewL(immediateShutDown, interrupt, iniSettingsFile);
				CleanupStack::PushL(socketShutdownModeTest);
				onlineTests.Append(socketShutdownModeTest);

				TInt numLeakTests = iniFile->ItemValue(KTestMultiTrans(), KLeakTestCount(), 0);
				TInt firstLeakIter = iniFile->ItemValue(KTestMultiTrans(), KFirstLeakIteration(), 0);
				socketShutdownModeTest->SetLeakTests(numLeakTests, firstLeakIter);
				CleanupStack::Pop(socketShutdownModeTest);
				}
			}

		// Authentication test
		if ( section->SectionName().CompareF(KTestAuthentication) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestAuthentication(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CDEF079378* authTest = CDEF079378::NewL(index + 1, iniSettingsFile); //Tests are 1-13
					CleanupStack::PushL(authTest);
					onlineTests.Append(authTest);
					}
				CleanupStack::Pop(index);
				}
			}
		
		// Reconnect after a connection drop
		if ( section->SectionName().CompareF ( KTestReconnectAfterConnectionDrop ) == 0 )
			{
			TBool runTest = iniFile->Section(i).ItemValue( KTestReconnectAfterConnectionDrop(), runTestDefault );
			if (  runTest )				
				{
				CINC082448* reconnectTest = CINC082448::NewL( 1, iniSettingsFile ); 
				CleanupStack::PushL( reconnectTest );
				onlineTests.AppendL( reconnectTest );
				CleanupStack::Pop ( reconnectTest );
				}
			}

		// INC094209 test
		if ( section->SectionName().CompareF(KTestINC094209) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC094209(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC094209* incTest = CINC094209::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}

		// INC110203 test
		if ( section->SectionName().CompareF(KTestINC110203) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC110203(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC110203* incTest = CINC110203::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
 		
		// INC102380 test
		if ( section->SectionName().CompareF(KTestCINC102380) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCINC102380(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC102380* incTest = CINC102380::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
		// CR1130
		if ( section->SectionName().CompareF(KTestCR1130) == 0)
 			{
 			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
  			if (runTest)
 				{
 				const TInt numSubTests = iniFile->ItemValue(KTestCR1130(), KResNumSubTests(), 1); 
 				TInt index;
 				for(index = 0; index < numSubTests; ++index)
 					{
 					CR1130* incTest = CR1130::NewL(index + 1, iniSettingsFile); 
 					CleanupStack::PushL(incTest);
 					onlineTests.Append(incTest);
 					}
 				CleanupStack::Pop(index);
 				}
 			}
		
		// INC112633
		if ( section->SectionName().CompareF(KTestINC112633) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC112633(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC112633* incTest = CINC112633::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
		if ( section->SectionName().CompareF(KTestINC112633) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC112633(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC112633_2* incTest = CINC112633_2::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}	
				CleanupStack::Pop(index);
				}
			}
		//INC120672_1 test
		if ( section->SectionName().CompareF(KTestCINC120672_1) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCINC120672_1(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC120672_1* incTest = CINC120672_1::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}	
			
        //INC120672_2 test
		if ( section->SectionName().CompareF(KTestCINC120672_2) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCINC120672_2(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC120672_2* incTest = CINC120672_2::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}

		// INC097492 test.
		// An apache server needs to be configured before enabling this test.
		if ( section->SectionName().CompareF(KTestINC097492) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC097492(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC097492* incTest = CINC097492::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
         	 }			
		if ( section->SectionName().CompareF(KTestINC097492) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC097492(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC097492_2* incTest = CINC097492_2::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
		if ( section->SectionName().CompareF(KTestINC097492) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC097492(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC097492_3* incTest = CINC097492_3::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}			

		if ( section->SectionName().CompareF(KTestINC097492) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestINC097492(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC097492_4* incTest = CINC097492_4::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
		// INC105767 test
		if ( section->SectionName().CompareF(KTestCINC105767) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCINC105767(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CINC105767* incTest = CINC105767::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
			
		// CPDEF143929   test
		if ( section->SectionName().CompareF(KTestCPDEF143929) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCPDEF143929(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CPDEF143929* incTest = CPDEF143929::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
			// 100-Continue tests
		if ( section->SectionName().CompareF(KTest100Continue) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTest100Continue(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CTestClient* incTest = CTestClient::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}	

		// 100-Continue tests
		if ( section->SectionName().CompareF(KTest100Continue) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTest100Continue(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CTestClient_2* incTest = CTestClient_2::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}	

		// 100-Continue tests
		if ( section->SectionName().CompareF(KTest100Continue) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KTest100Continue(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCINC105767(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CTestClient_3* incTest = CTestClient_3::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}	

		// 100-Continue tests
		if ( section->SectionName().CompareF(KTest100Continue) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KTest100Continue(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTestCINC105767(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CTestClient_4* incTest = CTestClient_4::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
			}
	
		// 100-Continue test with timeout properties set on transaction.
		if ( section->SectionName().CompareF(KTest100Continue) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KTest100Continue(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CTestClient_5* incTest = CTestClient_5::NewL(index + 1, iniSettingsFile); 
					CleanupStack::PushL(incTest);
					onlineTests.Append(incTest);
					}
				CleanupStack::Pop(index);
				}
 			}

		if ( section->SectionName().CompareF(KRecvBufSizeTest1) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KRecvBufSizeTest1(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CRecvBufSizeTest1* recvBufSizeTest = CRecvBufSizeTest1::NewL(1, iniSettingsFile);
					CleanupStack::PushL(recvBufSizeTest);
					onlineTests.AppendL(recvBufSizeTest);
					}
				CleanupStack::Pop(index);
				}
			}
			
		if ( section->SectionName().CompareF(KRecvBufSizeTest2) == 0)
			{
			TBool runTest = iniFile->Section(i).ItemValue(KRunTestItemName(), runTestDefault);
 			if (runTest)
				{
				const TInt numSubTests = iniFile->ItemValue(KRecvBufSizeTest2(), KResNumSubTests(), 1); 
				TInt index;
				for(index = 0; index < numSubTests; ++index)
					{
					CRecvBufSizeTest2* recvBufSizeTest = CRecvBufSizeTest2::NewL(1, iniSettingsFile);
					CleanupStack::PushL(recvBufSizeTest);
					onlineTests.AppendL(recvBufSizeTest);
					}
				CleanupStack::Pop(index);
				}
			}

		} 

	// Set the test set based on the menu choices
	engine->ConfigureTestSet(onlineTests);

	// Pop the tests which are now owned by the test engine
	CleanupStack::Pop(&onlineTests); // onlineTests, the array itself

	// All the action happens within the main event loop.
	CActiveScheduler::Start();
	engine->Utils().LogIt(_L("\nEnd of tests"));

	TInt pass, fail;
	engine->TestSummary(pass, fail);
	engine->Utils().LogIt(_L("\nTest Summary: %d passed, %d failed.\n"), pass, fail);
	engine->PressAnyKey();
	
	// Release the held open connection
	CHTTPTestUtils::ReleaseConnectionL();	

	CleanupStack::PopAndDestroy(4, as); // inifile, engine and iniSettingsFile
	}
	
TInt TestEntryPoint(TAny* /*aParam*/)
	{
	__UHEAP_MARK;
	
	CTrapCleanup* theCleanup = CTrapCleanup::New(); // Install exception handler	
	
	// Run all the tests
	TRAPD(err,DoTestsL());
	if (err!=KErrNone)
		User::Panic(KPanicOnLeave,err);
	
	delete theCleanup;
	
	__UHEAP_MARKEND;
	User::Heap().Check();
    return(KErrNone);	
	}
		
void LogOnPanicL(RThread& aTestThread)	
	{
	CTestScheduler* as = new(ELeave) CTestScheduler;
	CleanupStack::PushL(as);
	if (!as)
		{
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
		}
	CActiveScheduler::Install(as); // Install active scheduler
		
	// Initialise Comms, required in a minimal console environment
	CHTTPTestUtils::InitCommsL();

	// Force a connection to be held open keep NT RAS connected...
	CHTTPTestUtils::HoldOpenConnectionL();
	
	// Create the test engine
	CHttpTestEngine* engine=NULL;
	TRAPD(err,engine = CHttpTestEngine::NewL(KHttpOnlineTestTitle(), EFalse));
	if (err != KErrNone)
		{
		User::Panic(_L("T_HTTPONLINE"),EFailedToCreateEngine);
		}
	CleanupStack::PushL(engine);
	
	// Create log message
	TExitCategoryName panicName = aTestThread.ExitCategory();
	TInt panicNumber = aTestThread.ExitReason();
	HBufC* logMessage = HBufC::NewLC(KRtestFailureLabel().Length() + 
								   panicName.Length() +	
								   KMaxIntDesLen);
	TPtr logMessagePtr = logMessage->Des();
	logMessagePtr.Format(KRtestFailureLabel, &panicName, panicNumber);
			
	engine->Utils().LogIt(logMessagePtr);
	
	// Panic here so that nothing else is written to the log file
	User::Panic(panicName, panicNumber);
	
	// The code below provided for completenes, but should never be reached.
	
	// Release the held open connection
	CHTTPTestUtils::ReleaseConnectionL();	
	
	CleanupStack::PopAndDestroy(3, as); //as, engine, logMessage
	}
		
// main loop
//
GLDEF_C TInt E32Main()
    {  
	__UHEAP_MARK;  
	CTrapCleanup* theCleanup = CTrapCleanup::New(); // Install exception handler  
	
	// Create a new thread for running tests in
	RThread testThread;
	TInt err = testThread.Create(KTestThreadName, TestEntryPoint, KDefaultStackSize,  
							 KMinHeapSize, KMaxHeapSize, NULL);
	if(err == KErrNone)
		{
		testThread.Resume();	
		}
	else
		{
		return err;
		}
	
	// Kick off test thread and wait for it to exit
	TRequestStatus tStat;
	testThread.Logon(tStat);
	User::WaitForRequest(tStat);
	
	// Log if paniced
	if(testThread.ExitType() == EExitPanic)
		{
		TRAP_IGNORE(LogOnPanicL(testThread));	
		}
	
	testThread.Close();
	delete theCleanup;		
	__UHEAP_MARKEND;		
	return KErrNone;
	}
