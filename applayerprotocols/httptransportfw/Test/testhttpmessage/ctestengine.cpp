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
// defines test classes: CTestEngine
// 
//

#include "ctestengine.h"
#if !defined __CTESTMESSAGECOMPOSER_H__
#include "ctestmessagecomposer.h"
#endif
#if !defined CTESTMESSAGEPARSER_H_
#include "ctestmessageparser.h"
#endif

#define KTestRequestHeader _L8("HTTP MESSAGE PERFORMANCE LOGGER\n\n\n")

_LIT(KFilePath, "c:\\logs\\http\\");
_LIT(KFileName, "c:\\logs\\http\\msg_peformance_logger.txt");
_LIT(KTestHttpMessageComposer,"Composing HTTP Request Message \n \n \n");
_LIT(KTestHttpMessageParser,"Parsing HTTP Response Message \n \n \n");
_LIT(KTestHttpMessagePeformanceTitle, "HTTP Message Performance Test Harness");
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

const TDesC& CTestEngine::TestName()
		{
		_LIT(KTestName, "HTTP message Test");
		return KTestName;
		}
CTestEngine::CTestEngine()
	{
	iStartTime = 0;
	iEndTime = 0;
	}

void CTestEngine::ConstructL()
	{
	iTestUtils = CHTTPTestUtils::NewL(KTestHttpMessagePeformanceTitle());
	iTestUtils->InitCommsL();
	}

void CTestEngine::DoRunL()
	{

	}
void CTestEngine::DoCancel()
	{
	}
void CTestEngine::RunTestsL()
	{
	//Tests for composer
	iTestUtils->StartTestL(_L("Test 1 for composition of http message having multiple Headers with no body"));
	TestComposeMessageL(1);
	iTestUtils->EndTest(iTestFailed);
	iTestUtils->StartTestL(_L("Test 2 for composition of http message having 8 Headers with no body"));
	TestComposeMessageL(2);
	iTestUtils->EndTest(iTestFailed);
	//Tests for parser
	iTestUtils->StartTestL(_L("Test 1 for http message parser"));
	//Message to be parsed : "HTTP/1.1 200 OK\r\nDate: Tue, 04 Aug 2009 10:49:50 GMT\r\nServer: Apache/2.2.11 (Unix)\r\nLast-Modified: Thu, 16 Jul 2009 12:06:00 GMT\r\nETag: 4c0f49-329-46ed17eb73a00 \r\nAccept-Ranges: bytes\r\n\r\n "
	TestParseMessageL(1);
	iTestUtils->EndTest(iTestFailed);
	iTestUtils->StartTestL(_L("Test 2 for http message parser"));
	//Message to be parsed : "HTTP/1.1 200 OK\r\nDate: Tue, 04 Aug 2009 10:49:59 GMT\r\nServer: Apache/2.2.11 (Unix)\r\nLast-Modified: Thu, 16 Jul 2009 12:06:00 GMT\r\nETag: 4c134b-cbc-46ed17eb73a00 \r\nAccept-Ranges: bytes\r\nContent-Length: 3260\r\nKeep-Alive: timeout=5, max=88\r\nConnection: Keep-Alive\r\nContent-Type: image/png\r\n\r\n "
	TestParseMessageL(2);
	iTestUtils->EndTest(iTestFailed);
	iTestUtils->StartTestL(_L("Test 3 for http message parser"));
	//Message to be parsed : "HTTP/1.1 200 OK\r\nDate: Tue, 04 Aug 2009 10:49:59 GMT\r\nServer: Apache\r\nConnection: Close\r\nTransfer-Encoding: chunked\r\nContent-Type: text/html\r\n\r\n "
	TestParseMessageL(3);
	iTestUtils->EndTest(iTestFailed);
	iTestUtils->StartTestL(_L("Test 4 for http message parser"));
	//Message to be parsed : "HTTP/1.1 200 OK\r\nDate: Tue, Date: Tue, 22 Sep 2009 04:27:56 GMT\r\nServer: Apache\r\nContent-Encoding: gzip\r\nContent-Length: 6355\r\nContent-Type: text/html\r\nExpires: Tue, 22 Sep 2009 04:27:56 GMT\r\nCache-Control: 	max-age=0, no-cache\r\nPragma: no-cache\r\nConnection: close\r\nVary: Accept-Encoding\r\n\r\n "
	TestParseMessageL(4);
	iTestUtils->EndTest(iTestFailed);
	}
	
void CTestEngine::TestComposeMessageL(TInt aNumberofHeaders)
	{
	iTestFailed	= EFalse;
	CTestMessageComposer* messagecomposer = CTestMessageComposer::NewL(*this);
	CleanupStack::PushL(messagecomposer);
	messagecomposer->SetHeaderL(aNumberofHeaders);
	StartTimer();
	messagecomposer->StartRequest();
	CActiveScheduler::Start();
	EndTimer(KTestHttpMessageComposer);
	CleanupStack::PopAndDestroy(messagecomposer);
	}

void CTestEngine::TestParseMessageL(TInt aMessageType)
	{
	CTestMessageParser* messageparser = CTestMessageParser::NewL(*this);
	CleanupStack::PushL(messageparser);
	StartTimer();
	messageparser->StartRequest(aMessageType);
	CActiveScheduler::Start();
	EndTimer(KTestHttpMessageParser);
	CleanupStack::PopAndDestroy(messageparser);
	}
	
	void CTestEngine::NotifyError(TInt /*aError*/)
	{
	}

void CTestEngine::NotifyStart()
	{
	}

void CTestEngine::NotifyComplete()
	{
	CActiveScheduler::Stop();	
	}

void CTestEngine::LogIt(const TDesC& aComment)
	{
	TInt err = iFs.Connect();
	if(err == KErrNone)
		{
		iFs.MkDirAll(KFilePath);
		err = iFile.Open(iFs, KFileName, EFileWrite);
		if(err == KErrNotFound)
			{
			iFile.Create(iFs, KFileName, EFileWrite);
			iFile.Write(KTestRequestHeader);
			}
		TFileText file;
		file.Set(iFile);
		file.Write(aComment);
		iFile.Close();
		iFs.Close();
		}
	else	
		{
		User::InfoPrint(_L("Cannot write to file"));
		}
	}

	EXPORT_C void CTestEngine::StartTimer()
			{
			iStartTime = User::FastCounter();
			}
	
	EXPORT_C void CTestEngine::EndTimer(const TDesC& aComment)
			{
			iEndTime = User::FastCounter();
			iTimeDifference = iEndTime-iStartTime;	
			_LIT(KTimeDiff, ",%ld microseconds\n");
			RBuf myBuf;
			myBuf.Create (aComment.Length()+64);
			myBuf.Append (aComment );
			myBuf.AppendFormat(KTimeDiff, iTimeDifference.Int64());
		    LogIt(myBuf);
			myBuf.Close();
			iStartTime = 0;
			iEndTime = 0;			
			}

	
