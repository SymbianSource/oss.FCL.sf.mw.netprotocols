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




#include "t_respparse.h"



#include <http/framework/mrxdataobserver.h>


#if !defined HTTP_H
#include <http.h>
#endif




//
// Global declarations
//

enum {
	KErrNotEnoughStatusCodes = -10000,
	KErrTooManyStatusCodes	 = -10001,
	KErrStatusCodeMismatch	 = -10002
	};

const TInt KLineBuffSize = 256;
const TInt KSmallLineBuffSize = 16;

// sample response for first test - a single complete set of headers.
_LIT8(KTest1Response, "HTTP/12.34 200 OK\r\nDate: Mon, 29 Jan 2001 11:11:12 GMT\r\nServer: Apache/1.3.12 (Win32)\r\nLast-Modified: Mon, 08 Jan 2001 17:28:36 GMT\r\nETag: \"0-3524-3a59f8c4\"\r\nAccept-Ranges: bytes\r\nContent-Length: 13604\r\nContent-Type: text/html\r\n\r\n");

// sample response for second test - a status line split across two buffers.
_LIT8(KTest2Response1, "HTTP/1.0 304 Not Mod");
_LIT8(KTest2Response2, "ified\r\nDate: Mon, ");
_LIT8(KTest2Response3, "29 Jan 2001 11:11:12");
_LIT8(KTest2Response4, " GMT\r\n");

// sample responses for third test - split across several buffers + a simple response
_LIT8(KTest3Response1, "HTTP/1.1 200 OK\r\nServer: Microsoft-IIS/5.0\r\nDate: Mon, 29 Jan 2001 13:13:21 GMT\r\nPIC");
_LIT8(KTest3Response2, "S-Label: (PICS-1.0 \"http://www.rsac.org/ratingsv01.html\" l by \"dwightg@fool.com\" on \"");
_LIT8(KTest3Response3, "2000.02.14T10:29-0500\" exp \"2030.02.01T12:00-0500\" r (v 0 s 0 n 0 l 0))\r\nContent-Leng");
_LIT8(KTest3Response4, "th: 22136\r\nContent-Type: text/html\r\nExpires: Tue, 01 Apr 1980 05:00:00 GMT\r\nCache-co");
_LIT8(KTest3Response5, "ntrol: private\r\n\r\n<HTML>\r\n</HTML>\r\n");

// sample responses for fourth test - full response, non-chunked, split over several buffers
_LIT8(KTest4Response1, "HTTP/1.1 404 Not Found\r\nDate: Tue, 30 Jan 2001 12:27:58 GMT\r\nServer: Apache/1.3.12 (Wi");
_LIT8(KTest4Response2, "n32)\r\nContent-Type: text/html; charset=iso-8859-1\r\nContent-Length: 280\r\n\r\n<!DOCTYP");
_LIT8(KTest4Response3, "E HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n<HTML><HEAD>\r\n<TITLE>404 Not Found</TITL");
_LIT8(KTest4Response4, "E\r\n</HEAD><BODY>\r\n<H1>Not Found</H1>\r\nThe requested URL /dd was not found on this se");
_LIT8(KTest4Response5, "rver.<P><HR><ADDRESS>Apache/1.3.12 Server at lon-paulha.INTRA Port 80</ADDRESS></BODY></HT");
_LIT8(KTest4Response6, "ML>\r\n");

// sample responses for fifth test - chunked response
_LIT8(KTest5Response1, "HTTP/1.1 404 Not Found\r\nDate: Wed, 16 May 2001 12:01:44 GMT\r\nServer: Apache/1.3.");
_LIT8(KTest5Response2, "12 (Win32)\r\nTransfer-Encoding: chunked\r\nContent-Type: text/html; charset=iso-885");
_LIT8(KTest5Response3, "9-1\r\n\r\n113\r\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n<HTML><HEAD>\n<TIT");
_LIT8(KTest5Response4, "LE>404 Not Found</TITLE>\n</HEAD><BODY>\n<H1>Not Found</H1>\nThe requested URL /sdf");
_LIT8(KTest5Response5, "sdfsf was not found on this server\n<P>\n<HR>\n<ADDRESS>Apache/1.3.12 Server at lon");
_LIT8(KTest5Response6, "-paulha.INTRA Port 80</ADDRESS>\n</BODY></HTML>\r\r\n0\r\nPostChunkedBodyHeader: some value\r\n");

// sample response for sixth test - a single buffer containing a HEAD response from Apache
_LIT8(KTest6Response1, "HTTP/1.1 200 OK\r\nDate: Mon, 14 May 2001 15:50:36 GMT\r\nServer: Apache/1.3.12 (Win32)\r\nLast-Modified: Mon, 12 Mar 2001 12:57:37 GMT\r\nETag: \"0-44b-3aacc7c1\"\r\nAccept-Ranges: bytes\r\nContent-Length: 1099\r\nContent-Type: text/html\r\n");

// sample response for seventh test - a single buffer containing a HEAD response from IIS
_LIT8(KTest7Response1, "HTTP/1.1 200 OK\r\nServer: Microsoft-IIS/5.0\r\nContent-Location: http://www.symbian.com/index.html\r\nDate: Mon, 14 May 2001 15:55:01 GMT\r\nContent-Type: text/html\r\nAccept-Ranges: bytes\r\nLast-Modified: Mon, 14 May 2001 12:47:18 GMT\r\nETag: \"b4935c274dcc01:1023\"\r\nContent-Length: 18076\r\n\r\n");

// sample responses for eighth test - entire response in one buffer; test will iterate through it in increasing buffer sizes
_LIT8(KTest8Response, "HTTP/1.1 404 Not Found\r\nDate: Wed, 16 May 2001 12:01:44 GMT\r\nServer: Apache/1.3.12 (Win32)\r\nTransfer-Encoding: chunked\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n113\r\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n<HTML><HEAD>\n<TITLE>404 Not Found</TITLE>\n</HEAD><BODY>\n<H1>Not Found</H1>\nThe requested URL /sdfsdfsf was not found on this server\n<P>\n<HR>\n<ADDRESS>Apache/1.3.12 Server at lon-paulha.INTRA Port 80</ADDRESS>\n</BODY></HTML>\r\r\n0\r\nPostChunkedBodyHeader: some value\r\n");
const TInt KFirstBodyPos = 172; // first byte 'H' is pos 0; first body byte '<' is pos 172
const TInt KBodySize = 0x113; // see the chunk header
const TInt KLastBodyPos = KFirstBodyPos + KBodySize - 1;


_LIT8(KTest9Response, "HTTP/1.1 200 Ok\r\nContent-Type:\r\n text/vnd.wap.wml\r\nContent-Length: 253\r\n\r\n");
_LIT8(KTest10Response, "HTTP/1.1 200 Ok\r\nContent-Type:text/vnd.wap.wml\r\nContent-Length: 253\r\n\r\n");

_LIT8(KTest11Response, "HTTP/1.1 200 Ok\r\nContent-Type: text/vnd.wap.wml\r\nConnection\r\nContent-Length: 253\r\n\r\n");

//
// Implementation of CResponseParserTestBase - base class for all the response parser tests
//

CResponseParserTestBase::CResponseParserTestBase() 
// Constructor
	{
	}


CResponseParserTestBase::~CResponseParserTestBase()
// Destructor (virtual)
	{
	delete iLineBuffer;
	}

void CResponseParserTestBase::ConstructL(TInt aLineBufferSize)
// Second phase construction
	{
	iLineBuffer = HBufC8::NewL(aLineBufferSize);
	iParse.SetLineBuffer(iLineBuffer->Des());
	}


void CResponseParserTestBase::DoSingleParseLoopL(const TDesC8& aRespData, RArray<TInt> aExpectedStatusCodes,
												 TBool aValidateErrorCodes, TInt& aCodeIdx)
	{
	iParse.SetRespBuffer(aRespData);
	TBool done = EFalse;
	TInt stat = KErrNone;
	while (!done)
		{
		TBool statIsStatusCode = EFalse;
		TBool statIsErrorCode  = EFalse;
		stat = iParse.ParseMoreData();
		// Log the parse results
		switch (stat)
			{
			case ECompleteNoError:
				{
				iEngine->Utils().LogIt(_L("<no error>"));
				} break;
			case EEmptyLine:
				{
				iEngine->Utils().LogIt(_L("Found empty line"));
				statIsStatusCode = ETrue;
				} break;
			case EFoundStatusLine:
				{
				DumpStatusLineL();
				statIsStatusCode = ETrue;
				} break;
			case EFoundHeaderField:
			case EFoundHeaderFieldContinuation:
				{
				DumpHeaderL();
				statIsStatusCode = ETrue;
				} break;
			case EFoundBody:
				{
				DumpBody();
				statIsStatusCode = ETrue;
				} break;
			case EFoundChunkHeader:
				{
				iEngine->Utils().LogIt(_L("Found a chunk header"));
				statIsStatusCode = ETrue;
				} break;
			case ENoData:
				{
				iEngine->Utils().LogIt(_L("Response buffer exhausted with no carry over"));
				statIsErrorCode = ETrue;
				} break;
			case EPartialData:
				{
				iEngine->Utils().LogIt(_L("Response buffer exhausted with partial data carried over"));
				statIsErrorCode = ETrue;
				} break;
			case EParseError:
				{
				iEngine->Utils().LogIt(_L("Parsing error"));
				statIsErrorCode = ETrue;
				} break;
			default:
				{
				HandleUnexpectedStatusCodeL(stat);
				// Consider it handled if we haven't left by now
				} break;
			}

		// Validate the status code, and the error code too if set up to do so. However don't validate overrun errors.
		if ((statIsStatusCode) || (aValidateErrorCodes && (stat != ELineBuffOverrun)))
			{
			// Check we've been supplied enough status codes to validate
			if (aCodeIdx == aExpectedStatusCodes.Count())
				{
				iEngine->Utils().LogIt(_L("Not enough status codes provided (%d checked already)."), aCodeIdx);
				User::Leave(KErrNotEnoughStatusCodes);
				}

			if ((stat != aExpectedStatusCodes[aCodeIdx]) && (stat != ELineBuffOverrun))
				{
				iEngine->Utils().LogIt(_L("Failed on status code #%d (expected %d; got %d)"),
										aCodeIdx, aExpectedStatusCodes[aCodeIdx], stat);
				User::Leave(KErrStatusCodeMismatch);
				}
			aCodeIdx++;
			}

		done = statIsErrorCode;
		}
	}

void CResponseParserTestBase::DumpStatusLineL()
	{
	TInt majVer, minVer;
	TPtrC8 desc;
	TInt status = iParse.StatusLine(majVer, minVer, desc);
	iEngine->Utils().LogIt(_L("Found status line: code is %d, HTTP version is %d.%d"),status, majVer, minVer);
	DumpString8L(_L("Status description is: %S"), desc);
	}

void CResponseParserTestBase::DumpHeaderL()
	{
	TPtrC8 name, value;
	iParse.Header(name, value);
	DumpString8L(_L("Found header field name : %S"), name);
	DumpString8L(_L("Found header field value: %S"), value);
	}

void CResponseParserTestBase::DumpBody()
	{
	TPtrC8 bodyData;
	iParse.Body(bodyData);
	iEngine->Utils().LogIt(_L("Found body data :\n"));
	iEngine->Utils().DumpData(bodyData, ETrue);
	}

void CResponseParserTestBase::DumpString8L(const TDesC& aFmt, const TDesC8& aString8)
	{
	// Convert string to unicode
	HBufC* string16 = HBufC::NewL(aString8.Length());
	CleanupStack::PushL(string16);
	TPtr string16ptr = string16->Des();
	string16ptr.Copy(aString8.Left(KMaxFileName));
	iEngine->Utils().LogIt(aFmt, &string16ptr);
	CleanupStack::PopAndDestroy(string16);
	}

void CResponseParserTestBase::HandleUnexpectedStatusCodeL(TInt aStat)
	{
	iEngine->Utils().LogIt(_L("Unrecognised status code %d\n\n"), aStat);
	}


//
// Test class CRespParsTestSingleBuff:  a simple test case that contains an entire response in a single buffer

CRespParsTestSingleBuff::~CRespParsTestSingleBuff()
	{
	}

CRespParsTestSingleBuff* CRespParsTestSingleBuff::NewLC()
// Create a new CRespParsTestSingleBuff and place it on the clean-up stack then return it
	{
	CRespParsTestSingleBuff* me = new(ELeave) CRespParsTestSingleBuff;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestSingleBuff* CRespParsTestSingleBuff::NewL()
	{
	CRespParsTestSingleBuff* me = new(ELeave) CRespParsTestSingleBuff;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestSingleBuff::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;



	// reset for a new response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	
	
	// Response 9
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine)); // HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content Type
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderFieldContinuation)); 
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest9Response, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);

	iParse.Reset();
	
	// Response 10
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine)); // HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content Type
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	codeIdx = 0;
	DoSingleParseLoopL(KTest10Response, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);

	iParse.Reset();

	// Response 11
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine)); // HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content Type
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	codeIdx = 0;
	DoSingleParseLoopL(KTest11Response, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);

	iParse.Reset();	




	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine)); // HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Last-Modified
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Etag
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Accept-Ranges
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	codeIdx = 0;
	DoSingleParseLoopL(KTest1Response, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	

	}

void CRespParsTestSingleBuff::DoCancel()
	{
	}

const TDesC& CRespParsTestSingleBuff::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Single Buffer");
	return KTestName;
	}

CRespParsTestSingleBuff::CRespParsTestSingleBuff()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestSingleBuff::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KLineBuffSize);
	}


//
// Test class CRespParsTestSplitStatusLine:  a simple test case that contains an entire response in a single buffer

CRespParsTestSplitStatusLine::~CRespParsTestSplitStatusLine()
	{
	}

CRespParsTestSplitStatusLine* CRespParsTestSplitStatusLine::NewLC()
// Create a new CRespParsTestSplitStatusLine and place it on the clean-up stack then return it
	{
	CRespParsTestSplitStatusLine* me = new(ELeave) CRespParsTestSplitStatusLine;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestSplitStatusLine* CRespParsTestSplitStatusLine::NewL()
	{
	CRespParsTestSplitStatusLine* me = new(ELeave) CRespParsTestSplitStatusLine;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestSplitStatusLine::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 304
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest2Response1, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest2Response2, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest2Response3, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest2Response4, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestSplitStatusLine::DoCancel()
	{
	}

const TDesC& CRespParsTestSplitStatusLine::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Split Status Line");
	return KTestName;
	}

CRespParsTestSplitStatusLine::CRespParsTestSplitStatusLine()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestSplitStatusLine::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KLineBuffSize);
	}


//
// Test class CRespParsTestMultipleBuffers:  a simple test case that contains an entire response in a single buffer

CRespParsTestMultipleBuffers::~CRespParsTestMultipleBuffers()
	{
	}

CRespParsTestMultipleBuffers* CRespParsTestMultipleBuffers::NewLC()
// Create a new CRespParsTestMultipleBuffers and place it on the clean-up stack then return it
	{
	CRespParsTestMultipleBuffers* me = new(ELeave) CRespParsTestMultipleBuffers;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestMultipleBuffers* CRespParsTestMultipleBuffers::NewL()
	{
	CRespParsTestMultipleBuffers* me = new(ELeave) CRespParsTestMultipleBuffers;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestMultipleBuffers::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// PICS-Label
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Expires
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Cache-Control
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body chunk 1
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest3Response1, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response2, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response3, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response4, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response5, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestMultipleBuffers::DoCancel()
	{
	}

const TDesC& CRespParsTestMultipleBuffers::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Multiple Buffers");
	return KTestName;
	}

CRespParsTestMultipleBuffers::CRespParsTestMultipleBuffers()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestMultipleBuffers::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KLineBuffSize);
	}


//
// Test class CRespParsTestMultiBodyNormal:  a simple test case that contains an entire response in a single buffer

CRespParsTestMultiBodyNormal::~CRespParsTestMultiBodyNormal()
	{
	}

CRespParsTestMultiBodyNormal* CRespParsTestMultiBodyNormal::NewLC()
// Create a new CRespParsTestMultiBodyNormal and place it on the clean-up stack then return it
	{
	CRespParsTestMultiBodyNormal* me = new(ELeave) CRespParsTestMultiBodyNormal;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestMultiBodyNormal* CRespParsTestMultiBodyNormal::NewL()
	{
	CRespParsTestMultiBodyNormal* me = new(ELeave) CRespParsTestMultiBodyNormal;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestMultiBodyNormal::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 404
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body piece 1
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body piece 2
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body piece 3
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body piece 4
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body piece 5
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest4Response1, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest4Response2, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest4Response3, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest4Response4, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest4Response5, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest4Response6, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestMultiBodyNormal::DoCancel()
	{
	}

const TDesC& CRespParsTestMultiBodyNormal::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Multi Buffered Normal Body");
	return KTestName;
	}

CRespParsTestMultiBodyNormal::CRespParsTestMultiBodyNormal()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestMultiBodyNormal::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KLineBuffSize);
	}


//
// Test class CRespParsTestChunkedResponse:  a simple test case that contains an entire response in a single buffer

CRespParsTestChunkedResponse::~CRespParsTestChunkedResponse()
	{
	}

CRespParsTestChunkedResponse* CRespParsTestChunkedResponse::NewLC()
// Create a new CRespParsTestChunkedResponse and place it on the clean-up stack then return it
	{
	CRespParsTestChunkedResponse* me = new(ELeave) CRespParsTestChunkedResponse;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestChunkedResponse* CRespParsTestChunkedResponse::NewL()
	{
	CRespParsTestChunkedResponse* me = new(ELeave) CRespParsTestChunkedResponse;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestChunkedResponse::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 404
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Transfer-Encoding
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(EFoundChunkHeader));// start of first response body chunk
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body chunk 1, piece 1
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body chunk 1, piece 2
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body chunk 1, piece 3
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body chunk 1, piece 4
	User::LeaveIfError(expectedStatCodes.Append(EFoundChunkHeader));// indicates last response body chunk
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// PostChunkedHeader
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest5Response1, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest5Response2, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest5Response3, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest5Response4, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest5Response5, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest5Response6, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestChunkedResponse::DoCancel()
	{
	}

const TDesC& CRespParsTestChunkedResponse::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Chunked Response");
	return KTestName;
	}

CRespParsTestChunkedResponse::CRespParsTestChunkedResponse()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestChunkedResponse::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KLineBuffSize);
	}



//
// Test class CRespParsTestApacheHeadResponse:  a test case that mimics Apache's response to a HEAD request

CRespParsTestApacheHeadResponse::~CRespParsTestApacheHeadResponse()
	{
	}

CRespParsTestApacheHeadResponse* CRespParsTestApacheHeadResponse::NewLC()
	{
	CRespParsTestApacheHeadResponse* me = new(ELeave) CRespParsTestApacheHeadResponse;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestApacheHeadResponse* CRespParsTestApacheHeadResponse::NewL()
	{
	CRespParsTestApacheHeadResponse* me = new(ELeave) CRespParsTestApacheHeadResponse;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestApacheHeadResponse::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Last-Modified
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Etag
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Accept-Ranges
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest6Response1, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestApacheHeadResponse::DoCancel()
	{
	}

const TDesC& CRespParsTestApacheHeadResponse::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Apache HEAD Response");
	return KTestName;
	}

CRespParsTestApacheHeadResponse::CRespParsTestApacheHeadResponse()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestApacheHeadResponse::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KLineBuffSize);
	}


//
// Test class CRespParsTestIISHeadResponse:  a test case that mimics IIS's response to a HEAD request

CRespParsTestIISHeadResponse::~CRespParsTestIISHeadResponse()
	{
	}

CRespParsTestIISHeadResponse* CRespParsTestIISHeadResponse::NewLC()
	{
	CRespParsTestIISHeadResponse* me = new(ELeave) CRespParsTestIISHeadResponse;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestIISHeadResponse* CRespParsTestIISHeadResponse::NewL()
	{
	CRespParsTestIISHeadResponse* me = new(ELeave) CRespParsTestIISHeadResponse;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestIISHeadResponse::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Location
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Accept-Ranges
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Last-Modified
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Etag
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest7Response1, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestIISHeadResponse::DoCancel()
	{
	}

const TDesC& CRespParsTestIISHeadResponse::TestName()
	{
	_LIT(KTestName,"Response Parser: Test IIS HEAD Response");
	return KTestName;
	}

CRespParsTestIISHeadResponse::CRespParsTestIISHeadResponse()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestIISHeadResponse::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KLineBuffSize);
	}



//
// Test class CRespParsTestLineBuffOverrun:  a test case that handles line buffer overruns by allocating a larger buffer

CRespParsTestLineBuffOverrun::~CRespParsTestLineBuffOverrun()
	{
	}

CRespParsTestLineBuffOverrun* CRespParsTestLineBuffOverrun::NewLC()
	{
	CRespParsTestLineBuffOverrun* me = new(ELeave) CRespParsTestLineBuffOverrun();
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestLineBuffOverrun* CRespParsTestLineBuffOverrun::NewL()
	{
	CRespParsTestLineBuffOverrun* me = new(ELeave) CRespParsTestLineBuffOverrun();
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestLineBuffOverrun::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 200
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// PICS-Label
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Length
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Expires
	User::LeaveIfError(expectedStatCodes.Append(EPartialData));		// ...
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Cache-Control
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body chunk 1
	User::LeaveIfError(expectedStatCodes.Append(ENoData));			// <end of input buffer>
	//
	TInt codeIdx = 0;
	DoSingleParseLoopL(KTest3Response1, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response2, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response3, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response4, expectedStatCodes, ETrue, codeIdx);
	DoSingleParseLoopL(KTest3Response5, expectedStatCodes, ETrue, codeIdx);
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestLineBuffOverrun::DoCancel()
	{
	}

const TDesC& CRespParsTestLineBuffOverrun::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Handling of Line Buffer Overruns");
	return KTestName;
	}

CRespParsTestLineBuffOverrun::CRespParsTestLineBuffOverrun()
	: CResponseParserTestBase()
	{
	}

void CRespParsTestLineBuffOverrun::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KSmallLineBuffSize);
	}

void CRespParsTestLineBuffOverrun::HandleUnexpectedStatusCodeL(TInt aStat)
	{
	// Is the status code ELineBuffOverrun? -- if so, attempt to increase the size of the line buffer
	if (aStat == ELineBuffOverrun)
		{
		// Get the amount by which we would overrun and extend the line buffer
		TInt overrun = iParse.GetLineBufferOverrunSize();
		TInt maxLength = iLineBuffer->Des().MaxLength();
		HBufC8* largerBuffer = iLineBuffer->ReAlloc(maxLength + overrun);
		if (!largerBuffer)
			 {
			 // Allocation failed - leave. The old buffer is unchanged.
			 User::Leave(KErrNoMemory);
			 }

		// Switch buffers. The old one was removed by ReAlloc.
		iLineBuffer = largerBuffer;
		iParse.SetLineBuffer(iLineBuffer->Des());
		}
	else
		CResponseParserTestBase::HandleUnexpectedStatusCodeL(aStat);
	}


//
// Test class CRespParsTestCyclingRespBuffer:  a test case that handles line buffer overruns by allocating a larger buffer

CRespParsTestCyclingRespBuffer::~CRespParsTestCyclingRespBuffer()
	{
	}

CRespParsTestCyclingRespBuffer* CRespParsTestCyclingRespBuffer::NewLC(TInt aMinRespBuffSize, TInt aRespBuffInc)
	{
	CRespParsTestCyclingRespBuffer* me = new(ELeave) CRespParsTestCyclingRespBuffer(aMinRespBuffSize, aRespBuffInc);
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CRespParsTestCyclingRespBuffer* CRespParsTestCyclingRespBuffer::NewL(TInt aMinRespBuffSize, TInt aRespBuffInc)
	{
	CRespParsTestCyclingRespBuffer* me = new(ELeave) CRespParsTestCyclingRespBuffer(aMinRespBuffSize, aRespBuffInc);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CRespParsTestCyclingRespBuffer::DoRunL()
	{
	// This type of test is not based on an HTTP status code...
	iExpectedStatusCode = 0;

	for (TInt size = iMinRespBuffSize; size < KTest8Response().Length(); size += iRespBuffInc)
		{
		iEngine->Utils().LogIt(_L("Cycling with response buffers of size %d bytes."), size);
		TRAPD(err, DoTestL(size));
		if (err == KErrStatusCodeMismatch)
			{
			TBool silence = iEngine->Utils().IsSilent();
			iEngine->Utils().SetSilent(EFalse);
			iEngine->Utils().LogIt(_L("Had a parse mis-match with response buffers of size %d bytes."), size);
			iEngine->Utils().SetSilent(silence);
			}
		else
			User::LeaveIfError(err);
		}
	}

void CRespParsTestCyclingRespBuffer::DoTestL(TInt aRespBuffSize)
	{

	// set the first response
	iParse.Reset();
	RArray<TInt> expectedStatCodes;
	CleanupClosePushL(expectedStatCodes);
	//
	// Work out how many chunks the body will arrive in
	TInt numBodyChunks = 0;
	if ((KFirstBodyPos % aRespBuffSize) != 0)
		numBodyChunks++; // start of body is in a partial chunk
	if (((KLastBodyPos + 1) % aRespBuffSize) != 0)
		numBodyChunks++; // end of body is in a partial chunk
	TInt bodyBytesInWholeChunks = KBodySize - (aRespBuffSize - (((KFirstBodyPos - 1) % aRespBuffSize) + 1)) - ((KLastBodyPos + 1) % aRespBuffSize);
	numBodyChunks += bodyBytesInWholeChunks/aRespBuffSize;

	// Note: no EPartialData, ENoData codes validated since they'll fall in different places for
	// each response buffer size used,
	User::LeaveIfError(expectedStatCodes.Append(EFoundStatusLine));	// HTTP 404
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Date
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Server
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Transfer-Encoding
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// Content-Type
	User::LeaveIfError(expectedStatCodes.Append(EEmptyLine));		// separates header and body
	User::LeaveIfError(expectedStatCodes.Append(EFoundChunkHeader));// start of first response body chunk
	for (TInt ii = 0; ii < numBodyChunks; ii++)
		User::LeaveIfError(expectedStatCodes.Append(EFoundBody));		// body chunk 1, piece 1
	User::LeaveIfError(expectedStatCodes.Append(EFoundChunkHeader));// indicates last response body chunk
	User::LeaveIfError(expectedStatCodes.Append(EFoundHeaderField));// PostChunkedHeader
	//
	TInt codeIdx = 0;
	TInt respBuffOffset = 0;
	TInt remaining = KTest8Response().Length();
	while (remaining > 0)
		{
		if (remaining >= aRespBuffSize)
			{
			const TDesC8& respBuff = KTest8Response().Mid(respBuffOffset, aRespBuffSize);
			DoSingleParseLoopL(respBuff, expectedStatCodes, EFalse, codeIdx); // don't validate error codes
			}
		else
			{
			// last time round, the response buffer will most likely be an incomplete one
			const TDesC8& respBuff = KTest8Response().Mid(respBuffOffset);
			DoSingleParseLoopL(respBuff, expectedStatCodes, EFalse, codeIdx); // don't validate error codes
			}
		respBuffOffset += aRespBuffSize;
		remaining -= aRespBuffSize;
		}
	//
	if (codeIdx < expectedStatCodes.Count())
		User::Leave(KErrTooManyStatusCodes);
	CleanupStack::PopAndDestroy(&expectedStatCodes);
	}

void CRespParsTestCyclingRespBuffer::DoCancel()
	{
	}

const TDesC& CRespParsTestCyclingRespBuffer::TestName()
	{
	_LIT(KTestName,"Response Parser: Test Cycling Response Buffer");
	return KTestName;
	}

CRespParsTestCyclingRespBuffer::CRespParsTestCyclingRespBuffer(TInt aMinRespBuffSize, TInt aRespBuffInc)
	: CResponseParserTestBase(), iMinRespBuffSize(aMinRespBuffSize), iRespBuffInc(aRespBuffInc)
	{
	}

void CRespParsTestCyclingRespBuffer::ConstructL()
	{
	CResponseParserTestBase::ConstructL(KSmallLineBuffSize);
	}

void CRespParsTestCyclingRespBuffer::HandleUnexpectedStatusCodeL(TInt aStat)
	{
	// Is the status code ELineBuffOverrun? -- if so, attempt to increase the size of the line buffer
	if (aStat == ELineBuffOverrun)
		{
		// Get the amount by which we would overrun and extend the line buffer
		TInt overrun = iParse.GetLineBufferOverrunSize();
		TInt maxLength = iLineBuffer->Des().MaxLength();
		HBufC8* largerBuffer = iLineBuffer->ReAlloc(maxLength + overrun);
		if (!largerBuffer)
			 {
			 // Allocation failed - leave. The old buffer is unchanged.
			 User::Leave(KErrNoMemory);
			 }

		// Switch buffers. The old one was removed by ReAlloc.
		iLineBuffer = largerBuffer;
		iParse.SetLineBuffer(iLineBuffer->Des());
		}
	else
		CResponseParserTestBase::HandleUnexpectedStatusCodeL(aStat);
	}
