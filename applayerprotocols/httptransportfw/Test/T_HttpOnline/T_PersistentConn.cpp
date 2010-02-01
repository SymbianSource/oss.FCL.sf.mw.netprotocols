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
// defines test class: CTextModeTestPersistentConn
// 
//

#include "T_PersistentConn.h"


// Define this to do tests using real SyncML data
// #define _TEST_WITH_SYNCML_DATA_


// Constants for use in this file
_LIT8(KTestUrlFormat, "http://%S%S"); // <hostname><path> (where path must start with a / character)

#if defined (_TEST_WITH_SYNCML_DATA_)

	_LIT8(KHttpPostBodyChunk1a, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SyncML xmlns='SYNCML:SYNCML1.0'><SyncHdr><VerDTD>1.0</VerDTD><VerProto>SyncML/1.0</VerProto><SessionID>1</SessionID>");
	_LIT8(KHttpPostBodyChunk1b, "<MsgID>1</MsgID><Target><LocURI>/servlet/fde.sync.SyncML</LocURI></Target><Source><LocURI>ABCDEFG</LocURI></Source><Cred><Meta>Basic</Meta><Data>c3ltYmlhbj");
	_LIT8(KHttpPostBodyChunk1c, "pmMXN5bmNtbAA=</Data></Cred></SyncHdr><SyncBody><Alert><CmdID>1</CmdID><Data>200</Data><Item><Target><LocURI>CONTACTS</LocURI></Target><Source><LocURI>C:\\D");
	_LIT8(KHttpPostBodyChunk1d, "ocuments\\contacts</LocURI></Source></Item></Alert><Final/></SyncBody></SyncML>");

	_LIT8(KHttpPostBodyChunk2a, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SyncML xmlns='SYNCML:SYNCML1.0'><SyncHdr><VerDTD>1.0</VerDTD><VerProto>SyncML/1.0</VerProto><SessionID>1</SessionID>");
	_LIT8(KHttpPostBodyChunk2b, "<MsgID>2</MsgID><Target><LocURI>/servlet/fde.sync.SyncML</LocURI></Target><Source><LocURI>ABCDEFG</LocURI></Source><Cred><Meta>Basic</Meta><Data>c3ltYmlhbj");
	_LIT8(KHttpPostBodyChunk2c, "pmMXN5bmNtbAA=</Data></Cred></SyncHdr><SyncBody><Status><CmdID>2</CmdID><MsgRef>1</MsgRef><CmdRef>0</CmdRef><Cmd>SyncHdr</Cmd><Data>200</Data><Item><Target");
	_LIT8(KHttpPostBodyChunk2d, "><LocURI>ABCDEFG</LocURI></Target><Source><LocURI>/servlet/fde.sync.SyncML</LocURI></Source></Item></Status><Status><CmdID>3</CmdID><MsgRef>1</MsgRef><CmdR");
	_LIT8(KHttpPostBodyChunk2e, "ef>3</CmdRef><Cmd>Alert</Cmd><Data>200</Data><Item><Target><LocURI>C:\\Documents\\contacts</LocURI></Target><Source><LocURI>CONTACTS</LocURI></Source></Item>");
	_LIT8(KHttpPostBodyChunk2f, "</Status><Sync><CmdID>1</CmdID><Target><LocURI>CONTACTS</LocURI></Target><Source><LocURI>C:\\Documents\\contacts</LocURI></Source><Meta></Meta><Add><CmdID>2<");
	_LIT8(KHttpPostBodyChunk2g, "/CmdID><Item><Source><LocURI>3</LocURI></Source><Data>BEGIN:VCARD\r\nVERSION:2.1\r\nREV:20010117T160038Z\r\nUID:a1d1c81d196960-e058c63d190a18-3\r\nN:;test;");
	_LIT8(KHttpPostBodyChunk2h, ";;\r\nEND:VCARD\r\n</Data></Item></Add></Sync><Final/></SyncBody></SyncML>");

#else

	_LIT8(KHttpPostBodyChunk1, "abc=def&");
	_LIT8(KHttpPostBodyChunk2, "ghi=jkl&");
	_LIT8(KHttpPostBodyChunk3, "mno=pqr&");
	_LIT8(KHttpPostBodyChunk4, "stu=vwx&");
	_LIT8(KHttpPostBodyChunk5, "y=z");


#endif

// CTextModeTestPersistentConn-----------------------------------

CTextModeTestPersistentConn* CTextModeTestPersistentConn::NewL(const TDesC& aHostName, const TDesC& aPath, TInt aNumTrans)
	{
	CTextModeTestPersistentConn* me = new(ELeave)CTextModeTestPersistentConn(aNumTrans);
	CleanupStack::PushL(me);
	me->ConstructL(aHostName, aPath);
	CleanupStack::Pop();
	return me;
	}

CTextModeTestPersistentConn::~CTextModeTestPersistentConn()
	{
	delete iTestUrl;
	}

void CTextModeTestPersistentConn::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// HTTP response headers have been received
		//iEngine->Utils().LogIt(_L("<Got response headers>"));
		DumpRespHeaders(aTransaction);
		} break;
	case THTTPEvent::ESubmit:
		{
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Some (more) body data has been received (in the HTTP response)
		//iEngine->Utils().LogIt(_L("<Got a chunk of data>"));
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
		iEngine->Utils().LogIt(_L("<Transaction NOT validated successfully>"));
		iFailureError = KErrTestFailed;
		CActiveScheduler::Stop();
		} break;
	default:
		{
		iEngine->Utils().LogIt(_L("<unrecognised event> %d"),aEvent.iStatus);
		if (aEvent.iStatus < 0)
			{
			iFailureError = aEvent.iStatus;
			CActiveScheduler::Stop();
			}
		} 
		break;
		}
	}

TInt CTextModeTestPersistentConn::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	iEngine->Utils().LogIt(_L("\nMHFRunL left: error code %d on transaction ID=%d for event %d\n"), aError, aTransaction.Id(), aEvent.iStatus);
	return KErrNone;
	}

TBool CTextModeTestPersistentConn::GetNextDataPart(TPtrC8& aDataChunk)
	{
	iRetVal = EFalse;
#if defined (_TEST_WITH_SYNCML_DATA_)
	switch (iTestIdx)
		{
	case 0:
		{
		switch (iReqBodyChunkCount)
			{
		case 0:
			{
			aDataChunk.Set(KHttpPostBodyChunk1a());
			} break;
		case 1:
			{
			aDataChunk.Set(KHttpPostBodyChunk1b());
			} break;
		case 2:
			{
			aDataChunk.Set(KHttpPostBodyChunk1c());
			} break;
		case 3:
			{
			aDataChunk.Set(KHttpPostBodyChunk1d());
			iRetVal = ETrue;
			} break;
			}
		} break;
	case 1:
		{
		switch (iReqBodyChunkCount)
			{
		case 0:
			{
			aDataChunk.Set(KHttpPostBodyChunk2a());
			} break;
		case 1:
			{
			aDataChunk.Set(KHttpPostBodyChunk2b());
			} break;
		case 2:
			{
			aDataChunk.Set(KHttpPostBodyChunk2c());
			} break;
		case 3:
			{
			aDataChunk.Set(KHttpPostBodyChunk2d());
			} break;
		case 4:
			{
			aDataChunk.Set(KHttpPostBodyChunk2e());
			} break;
		case 5:
			{
			aDataChunk.Set(KHttpPostBodyChunk2f());
			} break;
		case 6:
			{
			aDataChunk.Set(KHttpPostBodyChunk2g());
			} break;
		case 7:
			{
			aDataChunk.Set(KHttpPostBodyChunk2h());
			iRetVal = ETrue;
			} break;
			}
		} break;
		}
#else
	switch (iReqBodyChunkCount)
		{
	case 0:
		{
		aDataChunk.Set(KHttpPostBodyChunk1());
		} break;
	case 1:
		{
		aDataChunk.Set(KHttpPostBodyChunk2());
		} break;
	case 2:
		{
		aDataChunk.Set(KHttpPostBodyChunk3());
		} break;
	case 3:
		{
		aDataChunk.Set(KHttpPostBodyChunk4());
		} break;
	case 4:
		{
		aDataChunk.Set(KHttpPostBodyChunk5());
		iRetVal = ETrue;
		} break;
		}
#endif

	iReqBodyChunkCount++;
	return iRetVal;
	}

void CTextModeTestPersistentConn::ReleaseData()
	{
	if( !iRetVal )
		{
		// More data to send - notify framework.
		TRAPD(err,iTransaction.NotifyNewRequestBodyPartL());
		
		if (err != KErrNone)
			iTransaction.Fail();
		}
	}

TInt CTextModeTestPersistentConn::OverallDataSize()
	{
#if defined (_TEST_WITH_SYNCML_DATA_)
	switch (iTestIdx)
		{
	case 0:
		{
		return (KHttpPostBodyChunk1a().Length() + KHttpPostBodyChunk1b().Length() + KHttpPostBodyChunk1c().Length() + KHttpPostBodyChunk1d().Length());
		} break;
	case 1:
		{
		return (KHttpPostBodyChunk2a().Length() + KHttpPostBodyChunk2b().Length() + KHttpPostBodyChunk2c().Length() + KHttpPostBodyChunk2d().Length() +
			    KHttpPostBodyChunk2e().Length() + KHttpPostBodyChunk2f().Length() + KHttpPostBodyChunk2g().Length() + KHttpPostBodyChunk2h().Length());
		} break;
	default:
		return 0;
		}
#else
	return (KHttpPostBodyChunk1().Length() + KHttpPostBodyChunk2().Length() + KHttpPostBodyChunk3().Length() + KHttpPostBodyChunk4().Length() + KHttpPostBodyChunk5().Length());
#endif
	}

const TDesC& CTextModeTestPersistentConn::TestName()
	{
	_LIT(KHeaderTestName,"CTextModeTestPersistentConn");
	return KHeaderTestName;
	}

void CTextModeTestPersistentConn::DoRunL()
	{
	// Open the HTTP session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	RStringPool strP = iSession.StringPool();

	// Loop for the number of transactions for this test
	for (iTestIdx = 0; iTestIdx < iNumTrans; iTestIdx++)
		{
		// Reset the body chunk counter for the new transaction
		iReqBodyChunkCount = 0;

		// Open a POST transactions, specifying this object as the request body data supplier
		iTransaction = iSession.OpenTransactionL(iTestUrl->Uri(), *this, strP.StringF(HTTP::EPOST,RHTTPSession::GetTable()));
		CleanupClosePushL(iTransaction);
		RHTTPRequest rq = iTransaction.Request();
		rq.SetBody(*this);
		RHTTPHeaders hdr = rq.GetHeaderCollection();
		THTTPHdrVal length(OverallDataSize());
		hdr.SetFieldL(strP.StringF(HTTP::EContentLength,RHTTPSession::GetTable()), length);
		THTTPHdrVal contType(strP.StringF(HTTP::EApplicationXWwwFormUrlEncoded,RHTTPSession::GetTable()));
		hdr.SetFieldL(strP.StringF(HTTP::EContentType,RHTTPSession::GetTable()), contType);
		
		// On the last transaction, add a 'Connection: close' header
		if (iTestIdx == iNumTrans - 1)
			hdr.SetFieldL(strP.StringF(HTTP::EConnection,RHTTPSession::GetTable()), THTTPHdrVal(strP.StringF(HTTP::EClose,RHTTPSession::GetTable())));

		// Submit the transaction
		iTransaction.SubmitL();
		iFailureError = KErrNone;
		CActiveScheduler::Start();

		if (iFailureError != KErrNone)
			{
			// Done with this transaction and session
			CleanupStack::PopAndDestroy(2, &iSession); // iTransaction
			// Check for failure error codes caught in MHFRunL
			User::LeaveIfError(iFailureError);
			}

		// Get the completion code and inform the engine.  Anything other than HTTP/200 status is a failure
		// for this test.
		iEngine->SetCurrentStatusCode(iTransaction.Response().StatusCode());
		iExpectedStatusCode = 200;

		// Done with this transaction
		CleanupStack::PopAndDestroy(); // iTransaction
		}

	// Done with the session
	CleanupStack::PopAndDestroy(); // iSession
	}

TInt CTextModeTestPersistentConn::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}
	
void CTextModeTestPersistentConn::DoCancel()
	{
	}

CTextModeTestPersistentConn::CTextModeTestPersistentConn(TInt aNumTrans)
	: iNumTrans(aNumTrans)
	{
	}

void CTextModeTestPersistentConn::ConstructL(const TDesC& aHostName, const TDesC& aPath)
	{
	HBufC8* tmp1 = HBufC8::NewL(KTestUrlFormat().Length() + aHostName.Length() + aPath.Length());
	CleanupStack::PushL(tmp1);
	HBufC8* tmp2 = HBufC8::NewL(aHostName.Length());
	CleanupStack::PushL(tmp2);
	tmp2->Des().Copy(aHostName);
	HBufC8* tmp3 = HBufC8::NewL(aPath.Length());
	CleanupStack::PushL(tmp3);
	tmp3->Des().Copy(aPath);
	tmp1->Des().Format(KTestUrlFormat, tmp2, tmp3);
	TUriParser8 up;
	up.Parse(*tmp1);
	iTestUrl = CUri8::NewL(up);
	CleanupStack::PopAndDestroy(3, tmp1);
	}

