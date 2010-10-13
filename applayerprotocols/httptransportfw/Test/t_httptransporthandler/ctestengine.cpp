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

#include "ctestengine.h"

#include <chttptransportlayer.h>
#include <msocketfactory.h>
#include <ecom/ecom.h>
#include<sslerr.h>
#include <http/rhttpconnectioninfo.h>

#include "csocketlistenerdriver.h"
#include "csocketconnectordriver.h"

// 'this' used in base member initializer list, The 'this' pointer being used is a base class pointer.
#pragma warning( disable : 4355 )

_LIT(KTestHttpdTransportHandlerTitle, "HTTP Daemon Transport Handler Unit Test Harness");

const TInt KLogBufSize	= 128;
const TInt KPortNumber	= 80;

//propagated error code from CSocketConnector.cpp file. not defined prior in any of the files.
const TInt KErrConnectError=(-5130);

GLDEF_D TBool	gLongHostName;
GLDEF_D TBool 	gReaderShutdown;
GLDEF_D TBool 	gInvalidListenPort;
GLDEF_D TBool 	gInvalidConnectPort; 
GLDEF_D TBool	gInvalidSecureHost;
GLDEF_D TBool 	gWriterShutdown;
GLDEF_D TBool 	gReqCertificate;
GLDEF_D TBool   gListenAbort;

CTestEngine* CTestEngine::NewL()
	{
	CTestEngine* self = new (ELeave) CTestEngine();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestEngine::~CTestEngine()
	{
	iConnection.Close();
	iSocketServer.Close();

	delete iTestUtils;
	}

CTestEngine::CTestEngine()
: CBase(), iTransportLayerParams(*this), iUseConnection(ETrue)
	{
	}

void CTestEngine::ConstructL()
	{
	iTestUtils = CHTTPTestUtils::NewL(KTestHttpdTransportHandlerTitle());
	iTestUtils->InitCommsL();

	// Create and start a comms connection
	User::LeaveIfError(iSocketServer.Connect());
	User::LeaveIfError(iConnection.Open(iSocketServer));
	User::LeaveIfError(iConnection.Start());
	}

void CTestEngine::DoOOMTest(TTestFunctionL aDoTestFunctionL)
	{
	TInt err = 1;
	TInt count = 0;

	iOOMTesting = ETrue;
	while( err || iDoingTest )
		{
		__UHEAP_MARK;
		__UHEAP_SETFAIL(RHeap::EFailNext, count); // This is a Macro, not leaving function
		TRAP(err, (this->*aDoTestFunctionL)());
		REComSession::FinalClose();
		__UHEAP_MARKEND;
		++count;
		}
	__UHEAP_RESET;
	iOOMTesting = EFalse;
	}

void CTestEngine::DoTestsL()
	{
	iTestUtils->SetSilent(EFalse);		
	TBuf<KLogBufSize> buf;
	buf.Format(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-FRAMEWORK-T_HTTPTRANSPORTHANDLER-0001  "));
	iTestUtils->LogIt(buf);
	iTestUtils->SetSilent(ETrue);
	
	// Run through tests...

	// Don't use the existing comms connection
	iUseConnection = EFalse;

	// Basic test case
	iTestUtils->StartTestL(_L("Basic Test Case HTTP-U-2051"));
	DoBasicTestCaseL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing on Basic test case
	iTestUtils->StartTestL(_L("OOM Testing of Basic Test Case HTTP-U-2052"));
	DoOOMTest(&CTestEngine::DoBasicTestCaseL);
	iTestUtils->EndTest(iTestFailed);

	// Reset so that the existing comms connection is used from this point
	iUseConnection = ETrue;

	// Do Multiple Connections test case
	iTestUtils->StartTestL(_L("Multiple Connections Test Case HTTP-U-2053"));
	DoMultipleConnectionsTestCaseL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing Multiple Connections test case
	User::After(1000);
	iTestUtils->StartTestL(_L("OOM Testing of Multiple Connections Test Case HTTP-U-2054"));
	DoOOMTest(&CTestEngine::DoMultipleConnectionsTestCaseL);
	iTestUtils->EndTest(iTestFailed);

	// Do Connect Control test case
	iTestUtils->StartTestL(_L("Connect Control Test Case HTTP-U-2055"));
	DoConnectControlTestCaseL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing Connect Control test case
	iTestUtils->StartTestL(_L("OOM Testing of Connect Control Test Case HTTP-U-2056"));
	DoOOMTest(&CTestEngine::DoConnectControlTestCaseL);
	iTestUtils->EndTest(iTestFailed);

	// Run secure test
	iTestUtils->StartTestL(_L("Basic Secure Test Case"));
	DoBasicSecureTestCaseL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing of secure test case
	iTestUtils->StartTestL(_L("OOM Testing of Basic Secure Test Case"));
	DoOOMTest(&CTestEngine::DoBasicSecureTestCaseL);
	iTestUtils->EndTest(iTestFailed);


#if defined (_LOGGING) 
	// test case - to test invalid LongHostName 
	gLongHostName = ETrue;
	iTestUtils->StartTestL(_L("Invalid Long Host Name Test Case "));
	DoBasicTestCaseL();
	iTestUtils->EndTest(iTestFailed);
	gLongHostName = EFalse;
#endif	

	// Run Multiple secure test
	iTestUtils->StartTestL(_L("Multiple Secure Test Case"));
	DoMultipleSecureConnectionsTestCaseL();
	iTestUtils->EndTest(iTestFailed);

	//Test case - to test improper shutdown of SocketReader
	gReaderShutdown = ETrue;
	iTestUtils->StartTestL(_L(" Invalid Reader Shutdown Test Case "));
	DoBasicTestCaseL();
	iTestUtils->EndTest(iTestFailed);
	gReaderShutdown = EFalse;
	
	//Test case - to test socket listen on invalid local port
	gInvalidListenPort = ETrue;
	iTestUtils->StartTestL(_L("Testing Multiple Listen on Invalid Port"));
	DoMultipleConnectionsTestCaseL();
	iTestUtils->EndTest(iTestFailed);	
	gInvalidListenPort = EFalse;
	
	//Test case - to test Basic secure socket connection with non-secure port
	gLongHostName = EFalse;
	gInvalidConnectPort = ETrue;
	iTestUtils->StartTestL(_L("Basic Invalid Secure Host Name Test Case "));
	DoBasicSecureTestCaseL();
	iTestUtils->EndTest(iTestFailed);
	gInvalidConnectPort = EFalse;	
	
	//Test case - to test improper shutdown of SocketWriter
	gInvalidSecureHost = ETrue;
	gWriterShutdown=ETrue;
	iTestUtils->StartTestL(_L(" Invalid Writer Shutdown Test Case "));
	DoMultipleSecureConnectionsTestCaseL();
	iTestUtils->EndTest(iTestFailed);	

	//Test case - to test Multiple secure socket connection with non-secure port
	gInvalidSecureHost = ETrue;
	gWriterShutdown=EFalse;
	iTestUtils->StartTestL(_L(" Multiple Invalid Secure Host Name Test Case "));
	DoMultipleSecureConnectionsTestCaseL();
	iTestUtils->EndTest(iTestFailed);
	gInvalidSecureHost = EFalse;
	
	//Test case - Requests Server Certificate on Writer for Non-Secure Connection
	gReqCertificate = ETrue;
	iTestUtils->StartTestL(_L("Check Server Certificate for Non-Secure Test case"));
	DoBasicTestCaseL();
	iTestUtils->EndTest(iTestFailed);
	gReqCertificate = EFalse;	
	
	//Test case - Requests Server Certificate on Writer for Secure Connection	
	gReqCertificate = ETrue;
	iTestUtils->StartTestL(_L("Check Server Certificate for Secure Test case"));
	DoBasicSecureTestCaseL();
	iTestUtils->EndTest(iTestFailed);
	gReqCertificate = EFalse;	
		
	gListenAbort=ETrue;
	iTestUtils->StartTestL(_L("Improper Socket Listen, Abort Test Case"));
	DoListenAbortTestL();
	iTestUtils->EndTest(iTestFailed);
	gListenAbort=EFalse;
	
	}

void CTestEngine::DoBasicTestCaseL()
	{
	// Do basic test case...

	do
		{
		// Create the http transport layer
		CHttpTransportLayer* transportLayer = CHttpTransportLayer::NewL(_L8("TCP"), iTransportLayerParams);
		CleanupStack::PushL(transportLayer);

		// Create the socket listener driver - single connect listener.
		CSocketListenerDriver* socketListenerDriver = CSocketListenerDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketListenerDriver);
		socketListenerDriver->SetSingleListenL();

		socketListenerDriver->Start();

		// Create the socket connector driver - single connect connector
		CSocketConnectorDriver* socketConnectorDriver = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketConnectorDriver);
		socketConnectorDriver->SetSingleConnectL();

		socketConnectorDriver->Start();
			
		StartTesting();

		CleanupStack::PopAndDestroy(3, transportLayer);
		} while( iResetTest );
	}

void CTestEngine::DoMultipleConnectionsTestCaseL()
	{
	// Do multiple connections test case...

	do
		{
		// Create the http transport layer
		CHttpTransportLayer* transportLayer = CHttpTransportLayer::NewL(_L8("TCP"), iTransportLayerParams);
		CleanupStack::PushL(transportLayer);

		// Create the socket listener driver - double connect listener.
		CSocketListenerDriver* socketListenerDriver = CSocketListenerDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketListenerDriver);
		socketListenerDriver->SetDoubleListenL();

		socketListenerDriver->Start();

		// Create the socket connector driver#1 - single connect connector
		CSocketConnectorDriver* socketConnectorDriver1 = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketConnectorDriver1);
		socketConnectorDriver1->SetSingleConnectL();

		socketConnectorDriver1->Start();
			
		// Create the socket connector driver#2 - single connect connector
		CSocketConnectorDriver* socketConnectorDriver2 = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketConnectorDriver2);
		socketConnectorDriver2->SetSingleConnectL();

		socketConnectorDriver2->Start();

		StartTesting();

		CleanupStack::PopAndDestroy(4, transportLayer);
		} while( iResetTest );
	}

void CTestEngine::DoConnectControlTestCaseL()
	{
	// Do connect control test case...

	do
		{
		// Create the http transport layer
		CHttpTransportLayer* transportLayer = CHttpTransportLayer::NewL(_L8("TCP"), iTransportLayerParams);
		CleanupStack::PushL(transportLayer);

		// Create the socket listener driver - single connect listener.
		CSocketListenerDriver* socketListenerDriver = CSocketListenerDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketListenerDriver);
		socketListenerDriver->SetSingleListenL();

		socketListenerDriver->Start();

		// Create the socket connector driver - double connect connector
		CSocketConnectorDriver* socketConnectorDriver = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketConnectorDriver);
		socketConnectorDriver->SetDoubleConnectL();

		socketConnectorDriver->Start();
			
		StartTesting();

		CleanupStack::PopAndDestroy(3, transportLayer);
		} while( iResetTest );
	}

void CTestEngine::DoBasicSecureTestCaseL()
	{
	// Do basic test case...

	do
		{
		// Create the http transport layer
		CHttpTransportLayer* transportLayer = CHttpTransportLayer::NewL(_L8("TCP"), iTransportLayerParams);
		CleanupStack::PushL(transportLayer);

		// Create the socket connector driver - single connect connector
		CSocketConnectorDriver* socketConnectorDriver = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketConnectorDriver);
		socketConnectorDriver->SetSingleSecureConnectL();

		socketConnectorDriver->Start();

		StartTesting();		

		CleanupStack::PopAndDestroy(2, transportLayer);
		} while( iResetTest );
	}

void CTestEngine::DoMultipleSecureConnectionsTestCaseL()
	{
	// Do multiple secure connections test case...
	do
		{
		// Create the http transport layer
		CHttpTransportLayer* transportLayer = CHttpTransportLayer::NewL(_L8("TCP"), iTransportLayerParams);
		CleanupStack::PushL(transportLayer);

		// Create the socket connector driver#1 - single connect connector
		CSocketConnectorDriver* socketConnectorDriver1 = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketConnectorDriver1);
		socketConnectorDriver1->SetSingleSecureConnectL();

		socketConnectorDriver1->Start();

		
		// Create the socket connector driver#2 - single connect connector
		CSocketConnectorDriver* socketConnectorDriver2 = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
		CleanupStack::PushL(socketConnectorDriver2);
		socketConnectorDriver2->SetSingleSecureConnectL();

		socketConnectorDriver2->Start();

		StartTesting();		

		CleanupStack::PopAndDestroy(3, transportLayer);
		} while( iResetTest );
	}

void CTestEngine::DoListenAbortTestL()
	{		
	// Create the http transport layer
	CHttpTransportLayer* transportLayer = CHttpTransportLayer::NewL(_L8("TCP"), iTransportLayerParams);
	CleanupStack::PushL(transportLayer);		

	// Create the socket listener driver
	CSocketListenerDriver* socketListenerDriver = CSocketListenerDriver::NewL(*this, transportLayer->SocketFactory());
	CleanupStack::PushL(socketListenerDriver);
	
	transportLayer->SocketFactory().ListenL(*socketListenerDriver, KPortNumber);
		
	// Create the socket connector driver
	CSocketConnectorDriver* socketConnectorDriver = CSocketConnectorDriver::NewL(*this, transportLayer->SocketFactory());
	CleanupStack::PushL(socketConnectorDriver);
	transportLayer->SocketFactory().ConnectL (*socketConnectorDriver, _L8("127.0.0.1"), KPortNumber);	
		
	transportLayer->SocketFactory().StopListen();
	
	socketConnectorDriver->SetSingleConnectL();
	socketConnectorDriver->Start();
	
	StartTesting();
		
	CleanupStack::PopAndDestroy(3, transportLayer);	
	
	}

void CTestEngine::StartTesting()
	{
	iTestCount	= 0;
	iTestFailed	= EFalse;
	iDoingTest	= ETrue;
	iResetTest	= EFalse;
	CActiveScheduler::Start();
	}

void CTestEngine::StopTesting()
	{
	CActiveScheduler::Stop();
	iDoingTest = EFalse;
	}

void CTestEngine::ResetTest()
	{
	StopTesting();
	iResetTest = ETrue;
	}

/*
 *	Methods from MDriverObserver
 */

void CTestEngine::NotifyError(TInt aError)
	{
	if( !iTestFailed && iDoingTest )
		{
		iTestFailed = ETrue;

		// Are we doing OOM testing?
		if( iOOMTesting && aError == KErrNoMemory )
			{
			// This is not an error.
			TBuf<KLogBufSize> buf;
			buf.Format(_L("--> OOM Testing : alloc failure : %d"), aError);
			Log(buf);

			// Don't stop testing, but stop scheduler.
			CActiveScheduler::Stop();
			}
		else if( aError == KErrInUse )
			{
			TBuf<KLogBufSize> buf;
			buf.Format(_L("--> Listen port still not shutdown : %d - re-start test..."), aError);
			Log(buf);

			ResetTest();
			}
		else if( aError == KErrNotReady )
			{
			TBuf<KLogBufSize> buf;
			buf.Format(_L("--> Comms connection has timed-out : %d - re-start it..."), aError);
			Log(buf);

			aError = iConnection.Start();
			if( aError == KErrNone )
				{
				buf.Format(_L("--> Comms connection has re-started - re-start test..."));
				Log(buf);
				ResetTest();
				}
			else
				{
				buf.Format(_L("--> Could not start comms connection : %d - stop test"), aError);
				Log(buf);
				StopTesting();
				}
			}
		else if( gLongHostName && aError == KErrConnectError ) 
			{
				//connection to invalid long host cannot be established hence error is expected
				//so no use in testing further.
				iTestFailed = EFalse;
				StopTesting();
			}
		else if (gReaderShutdown && (aError == KErrEof || aError == KErrNone))
			{//Making reader shutdown in the middle of Data-recieving, error is expected.
			//so no use in testing further.
			iTestFailed = EFalse;
			StopTesting();
			}
		else if( gInvalidListenPort && aError == KErrCouldNotConnect )
			{//Listening to invalid port, connection cannot be established hence error is expected
			//so no use in testing further.
			iTestFailed = EFalse;
			StopTesting();
			}
		else if (gInvalidConnectPort && aError == KErrSSLAlertUnexpectedMessage)			
			{//Trying to connect non-secure host on a Secure Port, error is expected.
			//so no use in testing further.
			iTestFailed = EFalse;
			StopTesting();
			}
		else if (gInvalidSecureHost && gWriterShutdown && aError == KErrNone)			
			{//Trying to connect invalid secure host and making immediate shutdown of the WriterStream, error is expected.
			//so no use in testing further.
			iTestFailed = EFalse;
			StopTesting();
			}			
		else if (gInvalidSecureHost && !gWriterShutdown && aError == KErrSSLAlertIllegalParameter)
			{//Trying to connect invalid secure host on a Secure Port, error is expected.
			//so no use in testing further.
			iTestFailed = EFalse;
			StopTesting();
			}
		else if (gListenAbort && aError==KErrCancel)
			{//Making Listen to stop abruptly, error is expected. 
			//so no use in testing further.
			iTestFailed = EFalse;
			StopTesting();	
			}				
		else
			{
			// Log the failure
			TBuf<KLogBufSize> buf;
			buf.Format(_L("--> Test error : %d"), aError);
			Log(buf);

			// Stop the scheduler and the test.
			StopTesting();
			}
		}
	}

void CTestEngine::NotifyStart()
	{
	++iTestCount;
	}

void CTestEngine::NotifyComplete()
	{
	--iTestCount;
	if( iTestCount == 0 )
		{
		// Log success
		Log(_L("Test completed"));

		// All tests have completed - stop the scheduler
		StopTesting();
		}
	}

void CTestEngine::Log(const TDesC& aComment)
	{
	// Do not log this to summary file - go silent, but remember current state.
	TBool isSilent = iTestUtils->IsSilent();
	iTestUtils->SetSilent(ETrue);

	// Log the comment...
	iTestUtils->LogIt(aComment);

	// Go back to original state.
	iTestUtils->SetSilent(isSilent);
	}

TBool CTestEngine::SupplyCommsConnection(RConnection*& aConnectionPtr)
	{
	// If transport layer requests a comms connection, provide one if test is configured to reuse one
	if(iUseConnection)
		{
		aConnectionPtr = &iConnection;
		return ETrue;
		}
	return EFalse;
	}

void CTestEngine::SetCommsConnectionL(RConnection* /*aConnectionPtr*/)
	{
	// Dont't need to do anything here
	}

void CTestEngine::GetSecurityPrefs(TBool& aDialogPrompt, MSecurityPolicy*& aSecurityPolicy)
	{
	if(gInvalidSecureHost && !gWriterShutdown)
		aDialogPrompt = EFalse;
	else
		aDialogPrompt = ETrue;
	
	aSecurityPolicy = NULL;
	}

TBool CTestEngine::ImmediateSocketShutdown()
	{
	if (iUseConnection)
		return ETrue;
	else
		return EFalse;
	}


TInt CTestEngine::SessionId()
	{
	return KErrNotSupported;
	}

TBool CTestEngine::SupplySocketServerHandle ( TInt& aSocketServerHandle )
	{
	if(iUseConnection)
		{
		aSocketServerHandle = iSocketServer.Handle();
		return ETrue;
		}
	return EFalse;	
	}
	
void CTestEngine::SetSocketServerHandleL ( TInt /*aSocketServerHandle*/ )	
	{	
	// do nothing
	}	

TInt CTestEngine::GetRecvBufferSize()
	{
	return KDefaultBufferSize;
	}

