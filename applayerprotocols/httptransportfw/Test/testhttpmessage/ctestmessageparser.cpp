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
// defines test classes: CTestMessageParser
// 
//

#include "ctestmessageparser.h"
#include <inetprottextutils.h>
#include <httpstringconstants.h>
#include <http/rhttpheaders.h>

_LIT8(KUri, "http://www.connecting.nokia.com");
_LIT8(KHeaderSeparator,	"\n");

//Various http response messages which has to be parsed
_LIT8(KMessageResponseData1, "HTTP/1.1 200 OK\r\nDate: Tue, 04 Aug 2009 10:49:50 GMT\r\nServer: Apache/2.2.11 (Unix)\r\nLast-Modified: Thu, 16 Jul 2009 12:06:00 GMT\r\nETag: 4c0f49-329-46ed17eb73a00 \r\nAccept-Ranges: bytes\r\n\r\n ");
_LIT8(KMessageResponseData2, "HTTP/1.1 200 OK\r\nDate: Tue, 04 Aug 2009 10:49:59 GMT\r\nServer: Apache/2.2.11 (Unix)\r\nLast-Modified: Thu, 16 Jul 2009 12:06:00 GMT\r\nETag: 4c134b-cbc-46ed17eb73a00 \r\nAccept-Ranges: bytes\r\nContent-Length: 3260\r\nKeep-Alive: timeout=5, max=88\r\nConnection: Keep-Alive\r\nContent-Type: image/png\r\n\r\n ");
_LIT8(KMessageResponseData3, "HTTP/1.1 200 OK\r\nDate: Tue, 04 Aug 2009 10:49:59 GMT\r\nServer: Apache\r\nConnection: Close\r\nTransfer-Encoding: chunked\r\nContent-Type: 	text/html\r\n\r\n ");
_LIT8(KMessageResponseData4, "HTTP/1.1 200 OK\r\nDate: Tue, Date: Tue, 22 Sep 2009 04:27:56 GMT\r\nServer: Apache\r\nContent-Encoding: gzip\r\nContent-Length: 6355\r\nContent-Type: text/html\r\nExpires: Tue, 22 Sep 2009 04:27:56 GMT\r\nCache-Control: 	max-age=0, no-cache\r\nPragma: no-cache\r\nConnection: close\r\nVary: Accept-Encoding\r\n\r\n ");

CTestMessageParser* CTestMessageParser::NewL(MTestObserver& aObserver)
	{
	CTestMessageParser* self = new(ELeave) CTestMessageParser(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestMessageParser::~CTestMessageParser()
	{
	delete iBuffer;
	iBuffer = NULL;
	iTransaction.Close();
	iSession.Close();
	iMessageParser.Close();	
	Cancel();
	}

CTestMessageParser::CTestMessageParser(MTestObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

void CTestMessageParser::ConstructL()
	{
	iBuffer = NULL;
	iSession.OpenL();
	iStringPool = iSession.StringPool();
	iMessageParser.OpenL(*this);
	TUriParser8 up;
	up.Parse(KUri);
	iTransaction = iSession.OpenTransactionL(up, *this, iSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable()));
	}
void CTestMessageParser::StartRequest(TInt aMessageType)
	{
	iMessageType = aMessageType;
	// Self-complete to kick-off the test.
	CompleteSelf();
	}

void CTestMessageParser::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CTestMessageParser::DoReset()
	{
	// Reset the parser and update the reset state.
	}

/*
 *	Methods from CActive
 */

void CTestMessageParser::RunL()
	{
	// Reset the test case data

	// Start the parser...
	iMessageParser.ReceivedMessageData();
	}

void CTestMessageParser::DoCancel()
	{
	// Do nothing...
	}

TInt CTestMessageParser::RunError(TInt /*aError*/)
	{
	// Inform the observer of the error
	return KErrNone;
	}

/*
 *	Methods from MHttpMessageParserObserver
 */

void CTestMessageParser::GetDataPacket(TPtrC8& aData)
	{
	//_LIT8(KMessageResponseData, "HTTP/1.1 200 OK\r\n Date: Tue, 04 Aug 2009 10:49:50 GMT\r\n Server: Apache/2.2.11 (Unix)\r\n Last-Modified: Thu, 16 Jul 2009 12:06:00 GMT\r\n ETag: 4c0f49-329-46ed17eb73a00 \r\n Accept-Ranges: bytes\r\n \r\n "); 
	switch(iMessageType)
		{
		case 1:
			iBuffer = KMessageResponseData1().AllocL();
			break;
		case 2:
			iBuffer = KMessageResponseData2().AllocL();
			break;
		case 3:
			iBuffer = KMessageResponseData3().AllocL();
			break;
		case 4:
			iBuffer = KMessageResponseData4().AllocL();
			break;
		default:
			break;
		}
	aData.Set(*iBuffer);
	}

void CTestMessageParser::ReleaseDataPacket()
	{

	}

void CTestMessageParser::StartLineL(const TDesC8& aStartLine)
	{
	// The RFC2616 defines the Status-Line as follows - 
	// 
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	//
	// First extract the HTTP-Version
	TPtrC8 data = aStartLine;
	User::LeaveIfError(InetProtTextUtils::RemoveWhiteSpace(data, InetProtTextUtils::ERemoveLeft));
	// HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
	TInt consumed = data.Locate('/');
	User::LeaveIfError(consumed);
	RStringPool stringPool = iTransaction.Session().StringPool();
	TPtrC8 http = data.Left(consumed);
	if( http.CompareF(stringPool.StringF(HTTP::EHTTP, RHTTPSession::GetTable()).DesC()) != 0 )
		User::Leave(KErrCorrupt);
	// Skip past the HTTP and "/"
	data.Set(data.Mid(consumed + 1));
	// Extract the major version number
	TInt major;
	consumed = InetProtTextUtils::ConvertDescriptorToInt(data, major);
	User::LeaveIfError(consumed);
	// Skip past major version number and the "."
	data.Set(data.Mid(consumed + 1));
	// Extract the minor version number
	TInt minor;
	consumed = InetProtTextUtils::ConvertDescriptorToInt(data, minor);
	User::LeaveIfError(consumed);
	// Skip past minor version number and the SP
	data.Set(data.Mid(consumed + 1));
	// Clear any extra surrounding whitespace
	User::LeaveIfError(InetProtTextUtils::RemoveWhiteSpace(data, InetProtTextUtils::ERemoveBoth));
	// Extract the status code
	TInt status;
	consumed = InetProtTextUtils::ConvertDescriptorToInt(data, status);
	User::LeaveIfError(consumed);
	if( data.Length() > consumed )
		{
		// Skip past status code and the SP
		data.Set(data.Mid(consumed + 1));
	
		// Remaining data is the status reason - trim any leading whitespace as right side already trimmed
		User::LeaveIfError(InetProtTextUtils::RemoveWhiteSpace(data, InetProtTextUtils::ERemoveLeft));
		}
	else
		{
		// No reason phrase following the status code
		data.Set(KNullDesC8());
		}
	// Populate transaction with Status-Line info
	RStringF reason = stringPool.OpenFStringL(data);
	RHTTPResponse response = iTransaction.Response();
	TVersion version(major, minor, 0);
	response.SetVersion(version);
	response.SetStatusCode(status);
	response.SetStatusText(reason);
	reason.Close();
	}

void CTestMessageParser::HeaderL(const TDesC8& aFieldName, TDesC8& aFieldValue)
	{
	RStringPool stringPool = iTransaction.Session().StringPool();
	RStringF name = stringPool.OpenFStringL(aFieldName);
	CleanupClosePushL(name);
	iHTTPHeaders = iTransaction.Response().GetHeaderCollection();
	iHTTPHeaders.SetRawFieldL(name, aFieldValue, KHeaderSeparator);
	CleanupStack::PopAndDestroy(&name);	
	}

TInt CTestMessageParser::BodySizeL()
	{
	return ENoBody;
	}

void CTestMessageParser::BodyChunkL(const TDesC8& /*aData*/)
	{
	}

void CTestMessageParser::BodyCompleteL()
	{
	}

void CTestMessageParser::MessageCompleteL(const TPtrC8& /*aExcessData*/)
	{
	iObserver.NotifyComplete();
	}

TInt CTestMessageParser::HandleParserError(TInt /*aError*/)
	{
	return KErrNone;
	}

void CTestMessageParser::Reserved_MHttpMessageParserObserver()
	{
	User::Invariant();
	}

TInt CTestMessageParser::MHFRunError(TInt /*aError*/, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return KErrNone;
	}

void CTestMessageParser::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}
