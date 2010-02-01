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


// System includes
//
#include <wsp/mwspcosessioninvoker.h>
#include <wsp/mwspcomethodinvoker.h>
#include <http/mhttpdatasupplier.h>
#include <uri8.h>

// Local includes
//


// Class signature
//
#include "T_WspTrHndStub.h"
#include "cstubwsptransporthandler.h"

_LIT(KTestWspTrhndStub, "T_WSPTRHNDSTUB");

// Constants used in this class
//
_LIT8(KRequestUri, "http://wap.symbian.com/cgi-bin/");
_LIT8(KRequestHeader1, "SomeHeaderData");
_LIT8(KRequestHeader2, "SomeMoreHeaderData");
_LIT8(KRequestBody1, "SomeBodyData");
_LIT8(KRequestBody2, "SomeMoreBodyData");

// Implementation of class 'CWspTrHndStubTest'
//

CWspTrHndStubTest* CWspTrHndStubTest::NewLC()
	{
	CWspTrHndStubTest* test = new (ELeave) CWspTrHndStubTest;
	CleanupStack::PushL(test);
	test->ConstructL();
	return test;
	}

CWspTrHndStubTest::~CWspTrHndStubTest()
	{
	iStringPool.Close();
	delete iTrHnd;
	delete iDriver;
	}

void CWspTrHndStubTest::DoRunL()
	{
	iDriver->Start();
	CActiveScheduler::Start();
	}

const TDesC& CWspTrHndStubTest::TestName()
	{
	_LIT(KHeaderTestName,"CWspTrHndStubTest");
	return KHeaderTestName;
	}

void CWspTrHndStubTest::DoCancel()
	{
	}
	
void CWspTrHndStubTest::Done()
	{
	CActiveScheduler::Stop();
	}
	
const MWspCapabilityViewer& CWspTrHndStubTest::ClientCapabilities() const
	{
	User::Panic(KTestWspTrhndStub, KErrNotSupported);
	return (MWspCapabilityViewer&)(*(MWspCapabilityViewer*)1);
	}

MWspCapabilitySetter& CWspTrHndStubTest::ServerCapabilities() const
	{
	User::Panic(KTestWspTrhndStub, KErrNotSupported);
	return (MWspCapabilitySetter&)(*(MWspCapabilitySetter*)1);
	}

const TDesC8& CWspTrHndStubTest::ProxyAddress() const
	{
	_LIT8(KAddr, "10.29.23.5");
	return KAddr();
	}

TUint16 CWspTrHndStubTest::LocalPort() const
	{
	return 0;
	}

TUint16 CWspTrHndStubTest::RemotePort() const
	{
	return 0;
	}

TWspBearer CWspTrHndStubTest::Bearer() const
	{
	return EAny;
	}

TWspSession CWspTrHndStubTest::WspSessionService() const
	{
	return EWspConnectionOriented;
	}

const TDesC8& CWspTrHndStubTest::ServiceCentreNumber() const
	{
	return KNullDesC8();
	}

TBool CWspTrHndStubTest::SecureConnection() const
	{
	return EFalse;
	}

const TDesC8& CWspTrHndStubTest::ClientHeaders() const
	{
	return KNullDesC8();
	}

void CWspTrHndStubTest::SetServerHeadersL(const TDesC8& /*aBuffer*/)
	{
	}

CWspTrHndStubTest::CWspTrHndStubTest()
	{
	}

void CWspTrHndStubTest::ConstructL()
	{
	// Create the stringpool
	iStringPool.OpenL(RHTTPSession::GetTable());

	// Create a driver object to run through a connect/disconnect test
	iDriver = CWspTrHndTestDriver::NewL(*this, iStringPool);

	// Write the location of the ini file for the stub to pick up.  The filename of the actual
	// ini file to be used is written as the contents of the link file.
	_LIT(KTrHndIniLinkFile, "wsptrhndstub_link.ini");
	_LIT(KTrHndIniLinkDir,  "C:\\");
	RFs fileSrvHnd;
	User::LeaveIfError(fileSrvHnd.Connect());
	TParse parsedLinkFile;
	parsedLinkFile.Set(KTrHndIniLinkDir, &(KTrHndIniLinkFile()), NULL);
	RFile linkFileHnd;
	User::LeaveIfError( linkFileHnd.Replace(fileSrvHnd, parsedLinkFile.FullName(),
											EFileWrite|EFileShareExclusive) );
	_LIT8(KTrHndIniFile, "offline_stub_tests.ini");
	linkFileHnd.Write(KTrHndIniFile());
	linkFileHnd.Close();
	
	// Create the transport handler - stub version
	iTrHnd = CStubWspTransportHandler::InterfaceByPassNewL(iStringPool, NULL, *iDriver, *this, *this, *this);

	// Link the driver to the transport handler
	iDriver->SetTransportHandler(*iTrHnd);
	}



// Implementation of class 'CWspTrHndStubTest'
//

CWspTrHndTestDriver* CWspTrHndTestDriver::NewL(CWspTrHndStubTest& aOwner, RStringPool aStringPool)
	{
	CWspTrHndTestDriver* test = new (ELeave) CWspTrHndTestDriver(aOwner, aStringPool);
	CleanupStack::PushL(test);
	test->ConstructL();
	CleanupStack::Pop(test);
	return test;
	}

CWspTrHndTestDriver::~CWspTrHndTestDriver()
	{
	}

void CWspTrHndTestDriver::Start()
	{
	iState = EInit;
	CompleteSelf();
	}

void CWspTrHndTestDriver::RunL()
	{
	switch (iState)
		{
	case EInit:
		{
		// Send out the connect request
		DoConnectReq();
		} break;
	case EConnecting:
		{
		// Shouldn't get run in this state
		__ASSERT_DEBUG(ETrue, User::Invariant());
		} break;
	case EConnected:
		{
		// Make a method invoke request
		DoMethodInvokeReq();
		} break;
	case EInvoking:
		{
		// Make a method invoke data request
		DoMethodInvokeDataReq();
		} break;
	case EInvokingMore:
		{
		// Shouldn't get run in this state
		__ASSERT_DEBUG(ETrue, User::Invariant());
		} break;
	case EReplying:
		{
		} break;
	case EReplyingMore:
		{
		} break;
	case EReplyComplete:
		{
		// Send out the disconnect request
		DoDisconnectReq();
		} break;
	case EDisconnecting:
		{
		// Shouldn't get run in this state
		__ASSERT_DEBUG(ETrue, User::Invariant());
		} break;
	case EDisconnected:
		{
		// Test complete - inform the owning test object
		iOwner.Done();
		} break;
	default:
		// Shouldn't get run in this state
		__ASSERT_DEBUG(ETrue, User::Invariant());
		break;
		}
	}

void CWspTrHndTestDriver::DoCancel()
	{
	}

TInt CWspTrHndTestDriver::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

CWspTrHndTestDriver::CWspTrHndTestDriver(CWspTrHndStubTest& aOwner, RStringPool aStringPool)
	: CActive(CActive::EPriorityStandard), iOwner(aOwner), iStringPool(aStringPool)
	{
	CActiveScheduler::Add(this);
	}

void CWspTrHndTestDriver::ConstructL()
	{
	}

void CWspTrHndTestDriver::CompleteSelf()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat, KErrNone);
	SetActive();
	}

void CWspTrHndTestDriver::SetTransportHandler(CWspTransportHandler& aTrHnd)
	{
	iTrHnd = &aTrHnd;
	}

void CWspTrHndTestDriver::DoConnectReq()
	{
	MWspCOSessionInvoker& inv = iTrHnd->COSessionInvoker();
	inv.ConnectReq();
	iState = EConnecting;
	}

void CWspTrHndTestDriver::DoMethodInvokeReq()
	{
	MWspCOMethodInvoker& inv = iTrHnd->COTransactionInvoker();
	RStringF method = iStringPool.StringF(HTTP::EPOST,RHTTPSession::GetTable());
	TUriParser8 uri;
	uri.Parse(KRequestUri());

	inv.MethodInvokeReq(*this, method, uri, KRequestHeader1(), KRequestBody1(), ETrue);
	iState = EInvoking;
	}

void CWspTrHndTestDriver::DoMethodInvokeDataReq()
	{
	MWspCOMethodInvoker& inv = iTrHnd->COTransactionInvoker();
	inv.MethodInvokeDataReq(*this, KRequestBody2(), KRequestHeader2(), EFalse);
	iState = EInvokingMore;
	}

void CWspTrHndTestDriver::DoDisconnectReq()
	{
	MWspCOSessionInvoker& inv = iTrHnd->COSessionInvoker();
	inv.DisconnectReq(EUserReq); // disconnect reason of '1' whatever that means
	iState = EDisconnecting;
	}

void CWspTrHndTestDriver::ConnectCnf()
	{
	iState = EConnected;
	CompleteSelf();
	}

void CWspTrHndTestDriver::ResumeCnf()
	{
	}

void CWspTrHndTestDriver::DisconnectInd(
									  TWspReason		/*aReasonCode*/, 
									  TBool				/*aRedirectSecurity*/, 
									  TWspRedirectedAddress& /*aRedirectAddresses*/, 
									  const TDesC8&		/*aErrorHeader*/, 
									  const TDesC8&		/*aErrorBody*/
									  )						  
	{
	// ignore the parameters for now...
	iState = EDisconnected;
	CompleteSelf();
	}

void CWspTrHndTestDriver::SuspendInd(TWspReason /*aReason*/)
	{
	}

void CWspTrHndTestDriver::ExceptionInd(const TDesC8& /*aExceptionData*/)
	{
	}

void CWspTrHndTestDriver::MethodInvokeCnf()
	{
	CompleteSelf();
	}

void CWspTrHndTestDriver::MethodInvokeDataCnf()
	{
	CompleteSelf();
	}

void CWspTrHndTestDriver::MethodAbortInd(TWspReason /*aReason*/)
	{
	}

void CWspTrHndTestDriver::MethodResultInd(
										TInt					aStatus, 
										const TDesC8&			aResponseHeaders, 
										MHTTPDataSupplier&		aResponseBody,
										TBool					aMoreData
										)
	{
	// Check the status
	__ASSERT_ALWAYS(aStatus == 200, User::Invariant());

	// Check the header data
	__ASSERT_ALWAYS(aResponseHeaders.Compare(_L8("SomeResponseHeaderData")) == 0, User::Invariant());

	// Check aMoreData
	__ASSERT_ALWAYS(aMoreData, User::Invariant());

	// extract the body data...
	TPtrC8 buffer;
	aResponseBody.OverallDataSize();
	aResponseBody.GetNextDataPart(buffer);
	aResponseBody.ReleaseData();
	
	// Done - remember the data suppliers for later on...
	iResponseBody = &aResponseBody;
	iState = EReplying;
	}


void CWspTrHndTestDriver::MethodResultDataInd(const TDesC8& aTrailerHeaders, TBool aMoreData)
	{
	// Check the trailer header data
	__ASSERT_ALWAYS(aTrailerHeaders.Compare(_L8("SomeMoreResponseHeaderData")) == 0, User::Invariant());

	// Check aMoreData
	__ASSERT_ALWAYS(!aMoreData, User::Invariant());

	// extract more body data...
	TPtrC8 buffer;
	iResponseBody->OverallDataSize();
	iResponseBody->GetNextDataPart(buffer);
	iResponseBody->ReleaseData();
	
	// Done
	iState = EReplyComplete;
	CompleteSelf();
	}

/*
CWspTrHndTestDriver::TBody::TBody()
	{
	iPartCount = 0;
	}

TBool CWspTrHndTestDriver::TBody::GetNextDataPart(TPtrC8& aDataPart)
	{
	switch (iPartCount)
		{
	case 0:
		aDataPart.Set(KRequestBody1());
		break;
	case 1:
		aDataPart.Set(KRequestBody2());
		break;
	default:
		User::Invariant();
		};
	++iPartCount;
	return (iPartCount == 2);
	}

void CWspTrHndTestDriver::TBody::ReleaseData()
	{
	// do nothing
	}

TInt CWspTrHndTestDriver::TBody::OverallDataSize()
	{
	return KRequestBody1().Length() + KRequestBody2().Length();
	}

TInt CWspTrHndTestDriver::TBody::Reset()
	{
	return KErrNotSupported;
	}


CWspTrHndTestDriver::THeader::THeader()
	{
	iPartCount = 0;
	}

TBool CWspTrHndTestDriver::THeader::GetNextDataPart(TPtrC8& aDataPart)
	{
	switch (iPartCount)
		{
	case 0:
		aDataPart.Set(KRequestHeader1());
		break;
	case 1:
		aDataPart.Set(KRequestHeader2());
		break;
	default:
		User::Invariant();
		};
	++iPartCount;
	return (iPartCount == 2);
	}

void CWspTrHndTestDriver::THeader::ReleaseData()
	{
	// do nothing
	}

TInt CWspTrHndTestDriver::THeader::OverallDataSize()
	{
	return KRequestHeader1().Length() + KRequestHeader2().Length();
	}

TInt CWspTrHndTestDriver::THeader::Reset()
	{
	return KErrNotSupported;
	}
*/
