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
// defines test class: CTextModeTestRespChunkSize
// 
//

#include "T_RespChunkSize.h"

// User include
#include "csrvaddrval.h"

// Constants used in CTextModeTestRespChunkSize test
_LIT8(KHttpSyncmlUrl, "http://WapTestIP/perl/dumpform.pl?req=req1.txt&resp=resp1.txt");

// It is noticed that trying to condence the below code to 80 lines has caused
// some problems with running the test harness.
_LIT8(KHttpSyncmlReqBody, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SyncML xmlns='SYNCML:SYNCML1.0'><SyncHdr><VerDTD>1.0</VerDTD><VerProto>SyncML/1.0</VerProto><SessionID>1</SessionID><MsgID>1</MsgID><Target><LocURI>servlet/fde.sync.SyncML</LocURI></Target><Source><LocURI>ABCDEFG</LocURI></Source><Cred><Meta><Type xmlns='syncml:metinf'>syncml:auth-basic</Type><Format xmlns='syncml:metinf'>b64</Format></Meta><Data>c3ltYmlhbjpmMXN5bmNtbAA=</Data></Cred></SyncHdr><SyncBody><Alert><CmdID>1</CmdID><Data>200</Data><Item><Target><LocURI>CALENDAR</LocURI></Target><Source><LocURI>C:\\Documents\\Agenda</LocURI></Source><Meta><Anchor xmlns='syncml:metinf'><Last xmlns='syncml:metinf'>20010320T105418Z</Last><Next xmlns='syncml:metinf'>20010320T145358Z</Next></Anchor></Meta></Item></Alert><Final/></SyncBody></SyncML>");
_LIT8(KHttpSyncmlUserAgent, "EPOC SyncML Client");
_LIT8(KHttpSyncmlContentType, "application/vnd.syncml-xml");
_LIT8(KProxyAddr, "127.0.0.1:8000");

const TInt KHttpSyncmlContentLength = 783;

// CTextModeTestRespChunkSize-----------------------------------
/**
	This function is called to create a new instance of the class 
	CTextModeTestCookies

	@param aDummyProxyPort	This variable holds a dummy non existing proxy 
							port
	@param aRespChunkSize	This variable holds the response chunk size
	@param aIniSettingsFile	The script file that holds the sections
							from which the value has to be retrieved
	@leave 					KErrNoMemory if there is insufficient memory to allocate for the object
*/
CTextModeTestRespChunkSize* CTextModeTestRespChunkSize::NewL(TInt aDummyProxyPort, TInt aRespChunkSize, CScriptFile* aIniSettingsFile)
	{
	CTextModeTestRespChunkSize* self = new(ELeave) CTextModeTestRespChunkSize(aDummyProxyPort, aRespChunkSize, aIniSettingsFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
	Destructor which deletes dummy server
*/
CTextModeTestRespChunkSize::~CTextModeTestRespChunkSize()
	{
	delete iDummyServer;
	}

void CTextModeTestRespChunkSize::MHFRunL(RHTTPTransaction aTransaction,
					 const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received
			DumpRespHeaders(aTransaction);
			} break;
		case THTTPEvent::ESubmit:
			{
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Some (more) body data has been received (in the HTTP response)
			DumpResponseBody(aTransaction);
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iEngine->Utils().LogIt(_L("<Transaction Complete>"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			iEngine->Utils().LogIt(_L("<Transaction validated successfully>"));
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EFailed:
			{
			iEngine->Utils().LogIt(
				_L("<Transaction NOT validated successfully>"));
			iFailureError = KErrTestFailed;
			CActiveScheduler::Stop();
			} break;
		default:
			{
			iEngine->Utils().LogIt(_L("<unrecognised event> %d")
				,aEvent.iStatus);
			if (aEvent.iStatus < 0)
				{
				iFailureError = aEvent.iStatus;
				CActiveScheduler::Stop();
				}
			} 
			break;
		}
	}

TInt CTextModeTestRespChunkSize::MHFRunError(TInt aError, 
											 RHTTPTransaction aTransaction,
					 const THTTPEvent& aEvent)
	{
	iEngine->Utils().LogIt(
	  _L("\nMHFRunL left: error code %d on transaction ID=%d for event %d\n"), 
	  aError, aTransaction.Id(), aEvent.iStatus);
	return KErrNone;
	}

TBool CTextModeTestRespChunkSize::GetNextDataPart(TPtrC8& aDataChunk)
	{
	aDataChunk.Set(KHttpSyncmlReqBody());
	return ETrue;
	}

void CTextModeTestRespChunkSize::ReleaseData()
	{
	// do nothing
	}

TInt CTextModeTestRespChunkSize::OverallDataSize()
	{
	__ASSERT_DEBUG(KHttpSyncmlContentLength == KHttpSyncmlReqBody().Length(), 
		User::Invariant());
	return KHttpSyncmlReqBody().Length();
	}

const TDesC& CTextModeTestRespChunkSize::TestName()
	{
	_LIT(KHeaderTestName,"CTextModeTestRespChunkSize");
	return KHeaderTestName;
	}

void CTextModeTestRespChunkSize::DoRunL()
/**
	This function is called to initiate the tests.
	It may leave with a system wide error code.
*/
	{
	iSession.OpenL();
	CleanupClosePushL(iSession);
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();

	// Open the HTTP session, with a proxy specified to be the dummy server
	RStringPool strP = iSession.StringPool();
	THTTPHdrVal proxyUsage(strP.StringF(HTTP::EUseProxy, RHTTPSession::GetTable()));
	connInfo.SetPropertyL(strP.StringF(HTTP::EProxyUsage, RHTTPSession::GetTable()), proxyUsage);
	RStringF proxy = strP.OpenFStringL(KProxyAddr());
	CleanupClosePushL(proxy);
	THTTPHdrVal proxyAddr(proxy);
	connInfo.SetPropertyL(strP.StringF(HTTP::EProxyAddress, RHTTPSession::GetTable()), proxyAddr);
	CleanupStack::PopAndDestroy(&proxy);

	// Open a POST transactions, specifying this object as the request 
	// body data supplier
	TUriParser8 up;

	// Replace the hostname in the url
	HBufC8* newUrl8 = TSrvAddrVal::ReplaceHostNameL(KHttpSyncmlUrl(), iIniSettingsFile);
	CleanupStack::PushL(newUrl8);
	TPtr8 newUrlPtr8 = newUrl8->Des();

	up.Parse(newUrlPtr8);

	iTransaction = iSession.OpenTransactionL(up, *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
	
	CleanupStack::PopAndDestroy(newUrl8);

	RHTTPRequest rq = iTransaction.Request();
	rq.SetBody(*this);
	RHTTPHeaders hdr = rq.GetHeaderCollection();
	THTTPHdrVal length(KHttpSyncmlContentLength);
	hdr.SetFieldL(strP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()), length);
	RStringF contType = strP.OpenFStringL(KHttpSyncmlContentType());
	CleanupClosePushL(contType);
	hdr.SetFieldL(strP.StringF(HTTP::EContentType,RHTTPSession::GetTable()), THTTPHdrVal(contType));
	RStringF userAgent = strP.OpenFStringL(KHttpSyncmlUserAgent());
	CleanupClosePushL(userAgent);
	hdr.SetFieldL(strP.StringF(HTTP::EUserAgent,RHTTPSession::GetTable()), THTTPHdrVal(userAgent));

	// Submit the transaction and start the proxy
	iTransaction.SubmitL();
	iDummyServer->ListenL();
	
	// Go!
	iFailureError = KErrNone;
	CActiveScheduler::Start();

	// check if we are making a leave.  If leaving then destroy the memory allocated
	if (iFailureError != KErrNone)
		{
		CleanupStack::PopAndDestroy(3, &iSession); // close userAgent, contType and iSession;
		User::LeaveIfError(iFailureError);
		}

	// Get the completion code and inform the engine.  Anything other than 
	// HTTP/200 status is a failure
	// for this test.
	iEngine->SetCurrentStatusCode(iTransaction.Response().StatusCode());
	iExpectedStatusCode = 200;

	// Close anything opened
	CleanupStack::PopAndDestroy(3, &iSession); // close userAgent, contType and iSession;
	}

TInt CTextModeTestRespChunkSize::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CTextModeTestRespChunkSize::DoCancel()
	{
	}

CTextModeTestRespChunkSize::CTextModeTestRespChunkSize(TInt aDummyProxyPort, 
										TInt aRespChunkSize, 
										CScriptFile* aIniSettingsFile)
	: iDummyProxyPort(aDummyProxyPort), iRespChunkSize(aRespChunkSize), 
	  iIniSettingsFile(aIniSettingsFile)
	{
	}

/**
	Create a dummy HTTP server which will listen on port 8000.
*/
void CTextModeTestRespChunkSize::ConstructL()
	{
	iDummyServer = CDummyServer::NewL(iDummyProxyPort, iRespChunkSize);
	}
