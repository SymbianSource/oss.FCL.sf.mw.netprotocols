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

#include <ecom/ecom.h>

#include "cmessageparserdriver.h"
#include "cmessagecomposerdriver.h"

_LIT(KTestHttpMessageParserTitle, "HTTP Message Test Harness");

const TInt KLogBufSize	= 128;

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
	delete iTestUtils;
	}

CTestEngine::CTestEngine()
: CBase()
	{
	}

void CTestEngine::ConstructL()
	{
	iTestUtils = CHTTPTestUtils::NewL(KTestHttpMessageParserTitle());
	iTestUtils->InitCommsL();
	}

void CTestEngine::DoOOMTest(TTestFunctionL aDoTestFunctionL)
	{
	Log(_L("Doing OOM testing - logging turned-off"));
	iOOMTesting = ETrue;

	TInt err = 1;
	TInt count = 0;

	while( err || iTestFailed )
		{
		__UHEAP_MARK;
		__UHEAP_SETFAIL(RHeap::EFailNext, count); // This is a Macro, not leaving function
		TRAPD(error, (this->*aDoTestFunctionL)());
		err = error;
		__UHEAP_MARKEND;
		++count;
		}
	__UHEAP_RESET;
	iOOMTesting = EFalse;
	Log(_L("End of OOM testing"));
	}

void CTestEngine::DoTestsL()
	{
	iTestUtils->SetSilent(EFalse);		
	TBuf<KLogBufSize> buf;
	buf.Format(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-FRAMEWORK-T_HTTPMESSAGE-0001  "));
	iTestUtils->LogIt(buf);
	iTestUtils->SetSilent(ETrue);
		
	// Run through tests...
	
	// Simple Message Test Case
	iTestUtils->StartTestL(_L("Parse Simple Message Test Case HTTP-U-2201"));
	DoParseSimpleMessageL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Simple Message Test Case HTTP-U-2202"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseSimpleMessageL);
		}
	iTestUtils->EndTest(iTestFailed);

	// No Entity Body Test Case
	iTestUtils->StartTestL(_L("Parse No Entity Body Test Case HTTP-U-2203"));
	DoParseNoEntityBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse No Entity Body Test Case HTTP-U-2204"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseNoEntityBodyL);
		}
	iTestUtils->EndTest(iTestFailed);
	

	// Non-encoded Entity Body Test Case
	iTestUtils->StartTestL(_L("Parse Non-encoded Entity Body Test Case HTTP-U-2205"));
	DoParseNonEncodedBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Non-encoded Entity Body Test Case HTTP-U-2206"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseNonEncodedBodyL);
		}
	iTestUtils->EndTest(iTestFailed);


	// Chunk-Encoded Entity Body Test Case
	iTestUtils->StartTestL(_L("Parse Chunk-Encoded Entity Body Test Case HTTP-U-2207"));
	DoParseChunkEncodedBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Chunk-Encoded Entity Body Test Case HTTP-U-2208"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseChunkEncodedBodyL);
		}
	iTestUtils->EndTest(iTestFailed);


	// Chunk-Encoded Entity Body And Trailers Test Case
	iTestUtils->StartTestL(_L("Parse Chunk-Encoded Entity Body And Trailers Test Case HTTP-U-2209"));
	DoParseChunkEncodedBodyWithTrailersL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Chunk-Encoded Entity Body And Trailers Test Case HTTP-U-2210"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseChunkEncodedBodyWithTrailersL);
		}
	iTestUtils->EndTest(iTestFailed);


	// Bad Header Line Test Case
	iTestUtils->StartTestL(_L("Parse Bad Header Line Test Case HTTP-U-2211"));
	DoParseBadLineL();
	iTestUtils->EndTest(iTestFailed);


	// Bad Empty Line Test Case
	iTestUtils->StartTestL(_L("Parse Bad Empty Line Test Case HTTP-U-2212"));
	DoParseBadEmptyLineL();
	iTestUtils->EndTest(iTestFailed);


	// Bad Header Continuation Value Test Case
	iTestUtils->StartTestL(_L("Parse Bad Header Continuation Value Test Case HTTP-U-2213"));
	DoParseBadHeaderContinuationL();
	iTestUtils->EndTest(iTestFailed);


	// Bad Chunk-Extension Test Case
	iTestUtils->StartTestL(_L("Parse Bad Chunk-Extension Test Case HTTP-U-2214"));
	DoParseBadChunkExtensionL();
	iTestUtils->EndTest(iTestFailed);


	// Bad Chunk-Data Test Case
	iTestUtils->StartTestL(_L("Parse Bad Chunk-Data Test Case HTTP-U-2215"));
	DoParseBadChunkDataL();
	iTestUtils->EndTest(iTestFailed);


	// 	Bad Chunk-Size Test Case
	iTestUtils->StartTestL(_L("Parse Bad Chunk-Size Test Case HTTP-U-2216"));
	DoParseBadChunkSizeL();
	iTestUtils->EndTest(iTestFailed);


	// Missing CRs Test Case
	iTestUtils->StartTestL(_L("Parse Missing CRs Test Case HTTP-U-2217"));
	DoParseMissingCRsL();
	iTestUtils->EndTest(iTestFailed);


	// Spurious CRs Test Case
	iTestUtils->StartTestL(_L("Parse Spurious CRs Test Case HTTP-U-2218"));
	DoParseSpuriousCRsL();
	iTestUtils->EndTest(iTestFailed);


	// Long Header Test Case
	iTestUtils->StartTestL(_L("Parse Long Header Test Case HTTP-U-2219"));
	DoParseLongHeaderL();
	iTestUtils->EndTest(iTestFailed);


	// Leading Empty Line Test Case
	iTestUtils->StartTestL(_L("Parse Leading Empty Line Test Case HTTP-U-2220"));
	DoParseLeadingEmptyLineL();
	iTestUtils->EndTest(iTestFailed);


	// Non Chunked Message Reset Test Case
	iTestUtils->StartTestL(_L("Parse Non Chunked Message Reset Test Case HTTP-U-2221"));
	DoParseNonChunkedResetL();
	iTestUtils->EndTest(iTestFailed);


	// Chunked Message Reset Test Case
	iTestUtils->StartTestL(_L("Parse Chunked Message Reset Test Case HTTP-U-2222"));
	DoParseChunkedResetL();
	iTestUtils->EndTest(iTestFailed);

	// Chunked Messages Containing Only CRs Test Case
	iTestUtils->StartTestL(_L("Parse Chunked Messages Containing Only CRs Test Case HTTP-U-2223"));
	DoParseChunkDataContainingOnlyCRsL();
	iTestUtils->EndTest(iTestFailed);

//--------------------- Composing Tests -------------------------

	iTestUtils->StartTestL(_L("Compose Simple Message Test Case HTTP-U-2225"));
	DoComposeSimpleMessageL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Compose Simple Message Test Case HTTP-U-2226"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoComposeSimpleMessageL);
		}
	iTestUtils->EndTest(iTestFailed);


	// No Entity Body Test Case
	iTestUtils->StartTestL(_L("Compose No Entity Body Test Case HTTP-U-2227"));
	DoComposeNoEntityBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Compose No Entity Body Test Case HTTP-U-2228"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoComposeNoEntityBodyL);
		}
	iTestUtils->EndTest(iTestFailed);
	
	// Non-encoded Entity Body Test Case
	iTestUtils->StartTestL(_L("Compose Non-encoded Entity Body Test Case HTTP-U-2229"));
	DoComposeNonEncodedBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Compose Non-encoded Entity Body Test Case HTTP-U-2230"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoComposeNonEncodedBodyL);
		}
	iTestUtils->EndTest(iTestFailed);


	// Chunk-Encoded Entity Body Test Case
	iTestUtils->StartTestL(_L("Compose Chunk-Encoded Entity Body Test Case HTTP-U-2231"));
	DoComposeChunkEncodedBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Compose Chunk-Encoded Entity Body Test Case HTTP-U-2232"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoComposeChunkEncodedBodyL);
		}
	iTestUtils->EndTest(iTestFailed);


	// Chunk-Encoded Entity Body And Trailers Test Case
	iTestUtils->StartTestL(_L("Compose Chunk-Encoded Entity Body And Trailers Test Case HTTP-U-2233"));
	DoComposeChunkEncodedBodyWithTrailersL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Compose Chunk-Encoded Entity Body And Trailers Test Case HTTP-U-2234"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoComposeChunkEncodedBodyWithTrailersL);
		}
	iTestUtils->EndTest(iTestFailed);

	// Bad Entity Body - too much data
	iTestUtils->StartTestL(_L("Compose Bad Entity Body With Too Much Data Test Case HTTP-U-2235"));
	DoComposeTooMuchEntityBodyDataL();
	iTestUtils->EndTest(iTestFailed);

	// Bad Entity Body - too little data
	iTestUtils->StartTestL(_L("Compose Bad Entity Body With Too Little Data Test Case HTTP-U-2236"));
	DoComposeTooLittleEntityBodyDataL();
	iTestUtils->EndTest(iTestFailed);

	// Long Header - too little data
	iTestUtils->StartTestL(_L("Compose Long Header Test Case HTTP-U-2237"));
	DoComposeLongHeaderL();
	iTestUtils->EndTest(iTestFailed);


	// Non-encoded Entity Body Reset Test Case
	iTestUtils->StartTestL(_L("Compose Non-encoded Entity Body Reset Test Case HTTP-U-2238"));
	DoComposeNonEncodedBodyResetL();
	iTestUtils->EndTest(iTestFailed);

	// Chunk-Encoded Entity Body And Trailers Reset Test Case
	iTestUtils->StartTestL(_L("Compose Chunk-Encoded Entity Body And Trailers Reset Test Case HTTP-U-2239"));
	DoComposeChunkEncodedBodyWithTrailersResetL();
	iTestUtils->EndTest(iTestFailed);

//--------------------- Unknown Body Length Tests -------------------------
	// Run all the above parser tests again using a HTTP 1.0 style response of unknown body length
	iUseUnknownBodyLength = ETrue;
	
	// Simple Message Test Case
	iTestUtils->StartTestL(_L("Parse Simple Message Test Case HTTP-U-2201"));
	DoParseSimpleMessageL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Simple Message Test Case HTTP-U-2202"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseSimpleMessageL);
		}
	iTestUtils->EndTest(iTestFailed);

	// No Entity Body Test Case
	iTestUtils->StartTestL(_L("Parse No Entity Body Test Case HTTP-U-2203"));
	DoParseNoEntityBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse No Entity Body Test Case HTTP-U-2204"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseNoEntityBodyL);
		}
	iTestUtils->EndTest(iTestFailed);
	

	// Non-encoded Entity Body Test Case
	iTestUtils->StartTestL(_L("Parse Non-encoded Entity Body Test Case HTTP-U-2205"));
	DoParseNonEncodedBodyL();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Non-encoded Entity Body Test Case HTTP-U-2206"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseNonEncodedBodyL);
		}
	iTestUtils->EndTest(iTestFailed);

	// Bad Header Line Test Case
	iTestUtils->StartTestL(_L("Parse Bad Header Line Test Case HTTP-U-2211"));
	DoParseBadLineL();
	iTestUtils->EndTest(iTestFailed);


	// Bad Header Continuation Value Test Case
	iTestUtils->StartTestL(_L("Parse Bad Header Continuation Value Test Case HTTP-U-2213"));
	DoParseBadHeaderContinuationL();
	iTestUtils->EndTest(iTestFailed);


	// Long Header Test Case
	iTestUtils->StartTestL(_L("Parse Long Header Test Case HTTP-U-2219"));
	DoParseLongHeaderL();
	iTestUtils->EndTest(iTestFailed);


	// Leading Empty Line Test Case
	iTestUtils->StartTestL(_L("Parse Leading Empty Line Test Case HTTP-U-2220"));
	DoParseLeadingEmptyLineL();
	iTestUtils->EndTest(iTestFailed);

	iUseUnknownBodyLength = EFalse;
	

    // Raw Bad Response - field containing "<sp>:<sp> 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2240"));
	DoParseRawBadResponseTest1L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2241"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest1L);
		}
	iTestUtils->EndTest(iTestFailed);
    
	// Raw Bad Response - field containing "<sp>:<sp>bytes" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2242"));
	DoParseRawBadResponseTest2L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2243"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest2L);
		}
	iTestUtils->EndTest(iTestFailed);

	// Raw Bad Response - field containing "Accept-Ranges:<sp>" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2244"));
	DoParseRawBadResponseTest3L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2245"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest3L);
		}
	iTestUtils->EndTest(iTestFailed);

	// Raw Bad Response - field containing "<sp>:bytes" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2246"));
	DoParseRawBadResponseTest4L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2247"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest4L);
		}
	iTestUtils->EndTest(iTestFailed);

	// Raw Bad Response - field containing "<sp>:" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2248"));
	DoParseRawBadResponseTest5L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2249"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest5L);
		}
	iTestUtils->EndTest(iTestFailed);

	// Raw Bad Response - field containing "Accept-Ranges:" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2250"));
	DoParseRawBadResponseTest6L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2251"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest6L);
		}
	iTestUtils->EndTest(iTestFailed);

	// Raw Bad Response - field containing "<sp>::" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2252"));
	DoParseRawBadResponseTest7L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2253"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest7L);
		}
	iTestUtils->EndTest(iTestFailed);

	// Raw Bad Response - field containing "<sp>::!!" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2254"));
	DoParseRawBadResponseTest8L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2255"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest8L);
		}
	iTestUtils->EndTest(iTestFailed);

	// Raw Bad Response - field containing "<sp>: :!!" 
	iTestUtils->StartTestL(_L("Parse Raw Bad Response Test Case HTTP-U-2256"));
	DoParseRawBadResponseTest9L();
	iTestUtils->EndTest(iTestFailed);

	// Do OOM testing 
	iTestUtils->StartTestL(_L("OOM Testing of Parse Raw Bad Response Test Case HTTP-U-2257"));
	if( !iTestFailed )
		{
		DoOOMTest(&CTestEngine::DoParseRawBadResponseTest9L);
		}
	iTestUtils->EndTest(iTestFailed);

	}
void CTestEngine::DoParseRawBadResponseTest1L()
	{
	// Simple message test case - field containing "<sp>:<sp>". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n : \r\n\r\nhello!");
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6 :");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	
	}
void CTestEngine::DoParseRawBadResponseTest2L()
	{
	// Simple message test case - field containing "<sp>:<sp>bytes". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n : bytes\r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6 : bytes");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
void CTestEngine::DoParseRawBadResponseTest3L()
	{
	// Simple message test case - field containing "Accept-Ranges:<sp>". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\nAccept-Ranges: \r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6");
	_LIT8(KHeaderName2, "Accept-Ranges"); _LIT8(KHeaderValue2, "");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
    messageParser->SetHeaderL(KHeaderName2(),KHeaderValue2());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
void CTestEngine::DoParseRawBadResponseTest4L()
	{
	// Simple message test case - field containing "<sp>:bytes". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n :bytes\r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6 :bytes");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
void CTestEngine::DoParseRawBadResponseTest5L()
	{
	// Simple message test case - field containing "<sp>:". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n :\r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6 :");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
void CTestEngine::DoParseRawBadResponseTest6L()
	{
	// Simple message test case - field containing "Accept-Ranges:". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\nAccept-Ranges:\r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6");
	_LIT8(KHeaderName2, "Accept-Ranges"); _LIT8(KHeaderValue2, "");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
    messageParser->SetHeaderL(KHeaderName2(),KHeaderValue2());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
void CTestEngine::DoParseRawBadResponseTest7L()
	{
	// Simple message test case - field containing "<sp>::". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n ::\r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6 ::");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
void CTestEngine::DoParseRawBadResponseTest8L()
	{
	// Simple message test case - field containing "<sp>::!!". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n ::!!\r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6 ::!!");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
	
void CTestEngine::DoParseRawBadResponseTest9L()
	{
	// Simple message test case - field containing "<sp>: :!!". 

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n : :!!\r\n\r\nhello!");
	
	_LIT8(KStartLine,	"HTTP/1.1 200 OK");
	_LIT8(KHeaderName1,	"Content-Length");			_LIT8(KHeaderValue1,"6 : :!!");
	_LIT8(KEntityBody,	"hello!");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}
	

void CTestEngine::DoParseSimpleMessageL()
	{
	// Simple message test case - just a start-line. no headers or entity body.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData,	"HTTP/1.1 100 Continue\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 100 Continue");
	_LIT8(KEntityBody,	"");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseNoEntityBodyL()
	{
	// Message with no entity body - just headers.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 204 No Content\r\nX-Wap-Push-Status: 400 Accepted\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 204 No Content");
	_LIT8(KHeaderName1,	"X-Wap-Push-Status");		_LIT8(KHeaderValue1,	"400 Accepted");
	_LIT8(KEntityBody, "");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseNonEncodedBodyL()
	{
	// Message with headers and non-encoded entity body.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length: 35\r\nServer: CERN/3.0\r\n libwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Content-Length");			_LIT8(KHeaderValue2,	"35");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KEntityBody, "This is the data that is being sent");
	TBool isChunked		= EFalse;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseChunkEncodedBodyL()
	{
	// Message with headers and chunk-encoded entity body.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nServer: CERN/3.0\r\n\tlibwww/2.17\r\n\r\n4 \r\nThis\r\n0a\r\n is the da\r\n15 ; myextension = \"ten\"\r\nta that is being sent\r\n000\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0\tlibwww/2.17");
	_LIT8(KEntityBody,	"This is the data that is being sent");
	TBool isChunked		= ETrue;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseChunkEncodedBodyWithTrailersL()
	{
	// Message with headers, chunk-encoded entity body and trailers.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nTrailer: Server, ETag\r\n\r\n4 \r\nThis\r\n0a\r\n is the da\r\n15 ; myextension = \"ten\"\r\nta that is being sent\r\n 0 \r\nServer: CERN/3.0\r\n libwww/2.17\r\nETag: \"dave\"\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Trailer");					_LIT8(KHeaderValue3,	"Server, ETag");
	_LIT8(KEntityBody,	"This is the data that is being sent");
	TBool isChunked		= ETrue;
	_LIT8(KTrailerName1,"Server");				_LIT8(KTrailerValue1,	"CERN/3.0 libwww/2.17");
	_LIT8(KTrailerName2,"ETag");					_LIT8(KTrailerValue2,	"\"dave\"");
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	messageParser->SetTrailerL(KTrailerName1(), KTrailerValue1());
	messageParser->SetTrailerL(KTrailerName2(), KTrailerValue2());

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseBadLineL()
	{
	// Message with a bad header line - malformed CRLF; missing LF

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\rContent-Length: 35\r\nServer: CERN/3.0\r\n libwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plainContent-Length: 35");
	_LIT8(KHeaderName2,	"Server");					_LIT8(KHeaderValue2,	"CERN/3.0 libwww/2.17");
	_LIT8(KEntityBody, "This is the data that is being sent");
	TBool isChunked		= EFalse;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseBadEmptyLineL()
	{
	// Message with a bad empty line - malformed CRLF; missing LF

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nServer: CERN/3.0\r\n\tlibwww/2.17\r\n\r4 \r\nThis\r\n0a\r\n is the da\r\n15 ; myextension = \"ten\"\r\nta that is being sent\r\n000\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0\tlibwww/2.17");
	_LIT8(KEntityBody,	"");
	TBool isChunked		= ETrue;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseBadHeaderContinuationL()
	{
	// Message with bad header - malformed header continuation; missing space/tab.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length: 35\r\nServer: CERN/3.0\r\nlibwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Content-Length");			_LIT8(KHeaderValue2,	"35");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0");
	_LIT8(KEntityBody, "This is the data that is being sent");
	TBool isChunked		= EFalse;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseBadChunkDataL()
	{
	// Chunked message with error in chunk-data - too much data.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nServer: CERN/3.0\r\n\tlibwww/2.17\r\n\r\n4 \r\nThis\r\n0a\r\n is the da\r\n12 ; myextension = \"ten\"\r\nta that is being sent\r\n000\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0\tlibwww/2.17");
	//_LIT8(KEntityBody,	"This is the data that is being sent");
	_LIT8(KEntityBody,	"This is the data that is being s");
	TBool isChunked		= ETrue;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrCorrupt);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseBadChunkExtensionL()
	{
	// Chunked message with error in chunk-size - bad chunk-extension

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nServer: CERN/3.0\r\n libwww/2.17\r\n\r\n4 \r\nThis\r\n0a\r\n is the da\r\n15 myextension = \"ten\"\r\nta that is being sent\r\n000\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KEntityBody,	"This is the data that is being sent");
	TBool isChunked		= ETrue;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseBadChunkSizeL()
	{
	// Chunked message with errors in the chunk-size - an empty line where a 
	// chunk-size should be (ignored) followed by the chunk-data, treated as a
	// chunk-size and ignoring the error (can't convert to numeric value)

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nServer: CERN/3.0\r\n libwww/2.17\r\n\r\n4 \r\nThis\r\n\r\n is the da\r\n15 ;myextension = \"ten\"\r\nta that is being sent\r\n000\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KEntityBody,	"This");
	TBool isChunked		= ETrue;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);	

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseMissingCRsL()
	{
	// Message with headers, chunk-encoded entity body and trailers. 
	// NOTE - CRs are missing from line terminators

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\nContent-Type: text/plain\nTransfer-Encoding: chunked\nTrailer: Server, ETag\n\n4 \nThis\n0a\n is the da\n15 ; myextension = \"ten\"\nta that is being sent\n 0 \nServer: CERN/3.0\n libwww/2.17\nETag: \"dave\"\n\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Trailer");					_LIT8(KHeaderValue3,	"Server, ETag");
	_LIT8(KEntityBody,	"This is the data that is being sent");
	TBool isChunked		= ETrue;
	_LIT8(KTrailerName1,"Server");				_LIT8(KTrailerValue1,	"CERN/3.0 libwww/2.17");
	_LIT8(KTrailerName2,"ETag");					_LIT8(KTrailerValue2,	"\"dave\"");
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	messageParser->SetTrailerL(KTrailerName1(), KTrailerValue1());
	messageParser->SetTrailerL(KTrailerName2(), KTrailerValue2());

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseSpuriousCRsL()
	{
	// Message with headers, chunk-encoded entity body and trailers. 
	// NOTE - there are spurious CRs throughout the message that should be ignored.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\r\r\r\r\nContent\r-Type:\rtext/plain\r\nTransfer-Encoding\r:\r \rchunked\r\r\r\n\rTrailer: Server,\rETag\r\n\r\r\r\r\n\r\r4\r\nThis\n0a\r\n is the da\r\n15 ; myextension = \"ten\"\r\nta that is being sent\r\n0\r\nServer: CERN/3.0\r\n\r libwww/2.17\r\nETag:\r \"d\rave\"\r\n\r\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Trailer");					_LIT8(KHeaderValue3,	"Server,ETag");
	_LIT8(KEntityBody,	"This is the data that is being sent");
	TBool isChunked		= ETrue;
	_LIT8(KTrailerName1,"Server");				_LIT8(KTrailerValue1,	"CERN/3.0 libwww/2.17");
	_LIT8(KTrailerName2,"ETag");					_LIT8(KTrailerValue2,	"\"dave\"");
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	messageParser->SetTrailerL(KTrailerName1(), KTrailerValue1());
	messageParser->SetTrailerL(KTrailerName2(), KTrailerValue2());

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseLongHeaderL()
	{
	// Message with no entity body - just headers.
	// Header is very long - should excercise the realloc functionality

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 204 No Content\r\nLong-Header: AVeryLongHeaderIndeedFullOfRubbishAndNotReallyHavingAnyMeaningAtAllInFactItJustNeedToBeOver128BytesInLengthAndThatIsReallyItOkJustALIttlMoreAndThatIsAll\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 204 No Content");
	_LIT8(KHeaderName1,	"Long-Header");		_LIT8(KHeaderValue1,	"AVeryLongHeaderIndeedFullOfRubbishAndNotReallyHavingAnyMeaningAtAllInFactItJustNeedToBeOver128BytesInLengthAndThatIsReallyItOkJustALIttlMoreAndThatIsAll");
	_LIT8(KEntityBody, "");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseLeadingEmptyLineL()
	{
	// Simple message test case - just a start-line. no headers or entity body.
	// There is a leading empty lines that should just be ignored.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData,	"\r\n\r\nHTTP/1.1 100 Continue\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 100 Continue");
	_LIT8(KEntityBody,	"");
	TBool isChunked		= EFalse;

	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	
	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseNonChunkedResetL()
	{
	// Message with headers and non-encoded entity body.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length: 35\r\nServer: CERN/3.0\r\n libwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Content-Length");			_LIT8(KHeaderValue2,	"35");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KEntityBody, "This is the data that is being sent");
	TBool isChunked		= EFalse;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone, ETrue);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseChunkedResetL()
	{
	// Message with headers, chunk-encoded entity body and trailers.

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nTrailer: Server, ETag\r\n\r\n4 \r\nThis\r\n0a\r\n is the da\r\n15 ; myextension = \"ten\"\r\nta that is being sent\r\n 0 \r\nServer: CERN/3.0\r\n libwww/2.17\r\nETag: \"dave\"\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");		_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Trailer");					_LIT8(KHeaderValue3,	"Server, ETag");
	_LIT8(KEntityBody,	"This is the data that is being sent");
	TBool isChunked		= ETrue;
	_LIT8(KTrailerName1,"Server");				_LIT8(KTrailerValue1,	"CERN/3.0 libwww/2.17");
	_LIT8(KTrailerName2,"ETag");					_LIT8(KTrailerValue2,	"\"dave\"");
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);
	messageParser->SetTrailerL(KTrailerName1(), KTrailerValue1());
	messageParser->SetTrailerL(KTrailerName2(), KTrailerValue2());

	messageParser->Start(KErrNone, ETrue);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoParseChunkDataContainingOnlyCRsL()
	{
	// Message with headers and chunk-encoded entity body.
	// Message body has chunks containing only CRs

	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	// Create the http message parser driver
	CMessageParserDriver* messageParser = CMessageParserDriver::NewL(*this);
	CleanupStack::PushL(messageParser);

	_LIT8(KMessageData, "HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nServer: CERN/3.0\r\n\tlibwww/2.17\r\n\r\n4 \r\nThis\r\n0a\r\n is the da\r\n0d ; myextension = \"ten\"\r\nta that is be\r\n1\r\n\r\r\n5\r\ning s\r\n2\r\n\r\r\r\n3\r\nent\r\n000\r\n\r\n");
	_LIT8(KStartLine,	"HTTP/1.1 200 Ok");
	_LIT8(KHeaderName1,	"Content-Type");	_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");	_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Server");		_LIT8(KHeaderValue3,	"CERN/3.0\tlibwww/2.17");
	_LIT8(KEntityBody,	"This is the data that is be\ring s\r\rent");
	TBool isChunked		= ETrue;
	
	messageParser->SetMessageData(KMessageData());
	messageParser->SetStartLine(KStartLine());
	messageParser->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageParser->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageParser->SetHeaderL(KHeaderName3(), KHeaderValue3());
	messageParser->SetBodyData(KEntityBody(), isChunked, iUseUnknownBodyLength);

	messageParser->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageParser);
	}

void CTestEngine::DoComposeSimpleMessageL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 100 Continue\r\n\r\n");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"100");
	_LIT8(KStartLine3,	"Continue");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Start the composer...
	messageComposer->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeNoEntityBodyL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 204 No Content\r\nX-Wap-Push-Status: 400 Accepted\r\n\r\n");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"204");
	_LIT8(KStartLine3,	"No Content");
	_LIT8(KHeaderName1,	"X-Wap-Push-Status");		_LIT8(KHeaderValue1,	"400 Accepted");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());

	// Start the composer...
	messageComposer->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeNonEncodedBodyL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length: 35\r\nServer: CERN/3.0 libwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"200");
	_LIT8(KStartLine3,	"Ok");
	_LIT8(KHeaderName1,	"Content-Type");		_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Content-Length");		_LIT8(KHeaderValue2,	"35");
	_LIT8(KHeaderName3,	"Server");				_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KBodyPart1,		"This is the da");
	_LIT8(KBodyPart2,		"");
	_LIT8(KBodyPart3,		"ta that is being sent");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageComposer->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageComposer->SetHeaderL(KHeaderName3(), KHeaderValue3());

	// Set body...
	messageComposer->SetBodyDataL(KBodyPart1());
	messageComposer->SetBodyDataL(KBodyPart2());
	messageComposer->SetBodyDataL(KBodyPart3());
	messageComposer->SetBodySize(35);


	// Start the composer...
	messageComposer->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeChunkEncodedBodyL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nServer: CERN/3.0 libwww/2.17\r\n\r\n4\r\nThis\r\nA\r\n is the da\r\n15\r\nta that is being sent\r\n0\r\n\r\n");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"200");
	_LIT8(KStartLine3,	"Ok");
	_LIT8(KHeaderName1,	"Content-Type");		_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");	_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Server");				_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KBodyPart1,		"This");
	_LIT8(KBodyPart2,		" is the da");
	_LIT8(KBodyPart3,		"");
	_LIT8(KBodyPart4,		"ta that is being sent");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageComposer->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageComposer->SetHeaderL(KHeaderName3(), KHeaderValue3());

	// Set body...
	messageComposer->SetBodyDataL(KBodyPart1());
	messageComposer->SetBodyDataL(KBodyPart2());
	messageComposer->SetBodyDataL(KBodyPart3());
	messageComposer->SetBodyDataL(KBodyPart4());
	messageComposer->SetBodySize(KErrNotFound);

	// Start the composer...
	messageComposer->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeChunkEncodedBodyWithTrailersL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nTrailer: Server, ETag\r\n\r\n4\r\nThis\r\nA\r\n is the da\r\n15\r\nta that is being sent\r\n0\r\nServer: CERN/3.0 libwww/2.17\r\nETag: \"dave\"\r\n\r\n");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"200");
	_LIT8(KStartLine3,	"Ok");
	_LIT8(KHeaderName1,	"Content-Type");		_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");	_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Trailer");				_LIT8(KHeaderValue3,	"Server, ETag");
	_LIT8(KTrailerName1,	"Server");			_LIT8(KTrailerValue1,	"CERN/3.0 libwww/2.17");
	_LIT8(KTrailerName2,	"ETag");			_LIT8(KTrailerValue2,	"\"dave\"");
	_LIT8(KBodyPart1,		"This");
	_LIT8(KBodyPart2,		" is the da");
	_LIT8(KBodyPart3,		"ta that is being sent");
	_LIT8(KBodyPart4,		"");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageComposer->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageComposer->SetHeaderL(KHeaderName3(), KHeaderValue3());

	// Set body...
	messageComposer->SetBodyDataL(KBodyPart1());
	messageComposer->SetBodyDataL(KBodyPart2());
	messageComposer->SetBodyDataL(KBodyPart3());
	messageComposer->SetBodyDataL(KBodyPart4());
	messageComposer->SetBodySize(KErrNotFound);

	// Set trailers...
	messageComposer->SetTrailerL(KTrailerName1(), KTrailerValue1());
	messageComposer->SetTrailerL(KTrailerName2(), KTrailerValue2());
	
	// Start the composer...
	messageComposer->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeTooMuchEntityBodyDataL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length: 35\r\nServer: CERN/3.0 libwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine1,	"HTTP/1.1");	
	_LIT8(KStartLine2,	"200");	
	_LIT8(KStartLine3,	"Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Content-Length");			_LIT8(KHeaderValue2,	"35");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KBodyPart1,		"This is the da");
	_LIT8(KBodyPart2,		"ta that is being sent");
	_LIT8(KBodyPart3,		"extra data");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageComposer->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageComposer->SetHeaderL(KHeaderName3(), KHeaderValue3());

	// Set body...
	messageComposer->SetBodyDataL(KBodyPart1());
	messageComposer->SetBodyDataL(KBodyPart2());
	messageComposer->SetBodyDataL(KBodyPart3());
	messageComposer->SetBodySize(35);

	// Start the composer...
	messageComposer->Start(KErrCorrupt);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeTooLittleEntityBodyDataL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length: 35\r\nServer: CERN/3.0 libwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"200");
	_LIT8(KStartLine3,	"Ok");
	_LIT8(KHeaderName1,	"Content-Type");			_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Content-Length");			_LIT8(KHeaderValue2,	"35");
	_LIT8(KHeaderName3,	"Server");					_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KBodyPart1,	"This is the da");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageComposer->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageComposer->SetHeaderL(KHeaderName3(), KHeaderValue3());

	// Set body...
	messageComposer->SetBodyDataL(KBodyPart1());
	messageComposer->SetBodySize(35);

	// Start the composer...
	messageComposer->Start(KErrCorrupt);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeLongHeaderL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 204 No Content\r\nLong-Header: AVeryLongHeaderIndeedFullOfRubbishAndNotReallyHavingAnyMeaningAtAllInFactItJustNeedToBeOver128BytesInLengthAndThatIsReallyItOkJustALIttlMoreAndThatIsAll\r\n\r\n");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"204");
	_LIT8(KStartLine3,	"No Content");
	_LIT8(KHeaderName1,	"Long-Header");		_LIT8(KHeaderValue1,	"AVeryLongHeaderIndeedFullOfRubbishAndNotReallyHavingAnyMeaningAtAllInFactItJustNeedToBeOver128BytesInLengthAndThatIsReallyItOkJustALIttlMoreAndThatIsAll");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());

	// Start the composer...
	messageComposer->Start(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeNonEncodedBodyResetL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nContent-Length: 35\r\nServer: CERN/3.0 libwww/2.17\r\n\r\nThis is the data that is being sent");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"200");
	_LIT8(KStartLine3,	"Ok");
	_LIT8(KHeaderName1,	"Content-Type");		_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Content-Length");		_LIT8(KHeaderValue2,	"35");
	_LIT8(KHeaderName3,	"Server");				_LIT8(KHeaderValue3,	"CERN/3.0 libwww/2.17");
	_LIT8(KBodyPart1,		"This is the da");
	_LIT8(KBodyPart2,		"");
	_LIT8(KBodyPart3,		"ta that is being sent");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageComposer->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageComposer->SetHeaderL(KHeaderName3(), KHeaderValue3());

	// Set body...
	messageComposer->SetBodyDataL(KBodyPart1());
	messageComposer->SetBodyDataL(KBodyPart2());
	messageComposer->SetBodyDataL(KBodyPart3());
	messageComposer->SetBodySize(35);


	// Start the composer...
	messageComposer->Start(KErrNone, ETrue);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

void CTestEngine::DoComposeChunkEncodedBodyWithTrailersResetL()
	{
	// Reset test flags.
	iTestCount	= 0;
	iTestFailed	= EFalse;

	_LIT8(KMessageData,	"HTTP/1.1 200 Ok\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\nTrailer: Server, ETag\r\n\r\n4\r\nThis\r\nA\r\n is the da\r\n15\r\nta that is being sent\r\n0\r\nServer: CERN/3.0 libwww/2.17\r\nETag: \"dave\"\r\n\r\n");
	_LIT8(KStartLine1,	"HTTP/1.1");
	_LIT8(KStartLine2,	"200");
	_LIT8(KStartLine3,	"Ok");
	_LIT8(KHeaderName1,	"Content-Type");		_LIT8(KHeaderValue1,	"text/plain");
	_LIT8(KHeaderName2,	"Transfer-Encoding");	_LIT8(KHeaderValue2,	"chunked");
	_LIT8(KHeaderName3,	"Trailer");				_LIT8(KHeaderValue3,	"Server, ETag");
	_LIT8(KTrailerName1,	"Server");			_LIT8(KTrailerValue1,	"CERN/3.0 libwww/2.17");
	_LIT8(KTrailerName2,	"ETag");			_LIT8(KTrailerValue2,	"\"dave\"");
	_LIT8(KBodyPart1,		"This");
	_LIT8(KBodyPart2,		" is the da");
	_LIT8(KBodyPart3,		"ta that is being sent");
	_LIT8(KBodyPart4,		"");

	// Create the http message parser driver
	CMessageComposerDriver* messageComposer = CMessageComposerDriver::NewL(*this);
	CleanupStack::PushL(messageComposer);

	// Set the message data...
	messageComposer->SetMessageData(KMessageData());

	// Set the start-line
	messageComposer->SetStartLine(KStartLine1(), KStartLine2(), KStartLine3());

	// Set the headers...
	messageComposer->SetHeaderL(KHeaderName1(), KHeaderValue1());
	messageComposer->SetHeaderL(KHeaderName2(), KHeaderValue2());
	messageComposer->SetHeaderL(KHeaderName3(), KHeaderValue3());

	// Set body...
	messageComposer->SetBodyDataL(KBodyPart1());
	messageComposer->SetBodyDataL(KBodyPart2());
	messageComposer->SetBodyDataL(KBodyPart3());
	messageComposer->SetBodyDataL(KBodyPart4());
	messageComposer->SetBodySize(KErrNotFound);

	// Set trailers...
	messageComposer->SetTrailerL(KTrailerName1(), KTrailerValue1());
	messageComposer->SetTrailerL(KTrailerName2(), KTrailerValue2());
	
	// Start the composer...
	messageComposer->Start(KErrNone, ETrue);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(messageComposer);
	}

/*
 *	Methods from MDriverObserver
 */

void CTestEngine::NotifyError(TInt aError)
	{
	if( !iTestFailed )
		{
		// This is the first failure - stop the scheduler
		iTestFailed = ETrue;

		// Log the failure - do not log this to summary file - go silent
		iTestUtils->SetSilent(ETrue);

		// Log the comment...
		TBuf<KLogBufSize> comment;
		comment.Format(_L("Test error %d"), aError);
		iTestUtils->LogIt(comment);

		// Go back to non-silent
		iTestUtils->SetSilent(EFalse);

		// Stop the scheduler
		CActiveScheduler::Stop();
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
		// Log success - do not log this to summary file - go silent
		iTestUtils->SetSilent(ETrue);

		// Log the comment...
		iTestUtils->LogIt(_L("Test completed"));

		// Go back to non-silent
		iTestUtils->SetSilent(EFalse);

		// All tests have completed - stop the scheduler
		CActiveScheduler::Stop();
		}
	}

void CTestEngine::Log(const TDesC& aCmt)
	{
	// Only log if not doing OOM testing
	if( !iOOMTesting )
		{
		// Do not log this to summary file - go silent
		iTestUtils->SetSilent(ETrue);

		// Log the comment...
		iTestUtils->LogIt(aCmt);

		// Go back to non-silent
		iTestUtils->SetSilent(EFalse);
		}
	}

void CTestEngine::Dump(const TDesC8& aData)
	{
	// Only log if not doing OOM testing
	if( !iOOMTesting )
		{
		// Do not log this to summary file - go silent
		iTestUtils->SetSilent(ETrue);

		// Do the data dump...
		iTestUtils->DumpData(aData, ETrue);

		// Go back to non-silent
		iTestUtils->SetSilent(EFalse);
		}
	}
