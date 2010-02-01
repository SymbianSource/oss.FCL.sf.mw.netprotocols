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
// defines test classes: CTestMessageComposer
// 
//
#if !defined __CTESTMESSAGECOMPOSER_H__
#include "ctestmessagecomposer.h"
#endif

_LIT8(KUri, "http://www.connecting.nokia.com");

CTestMessageComposer* CTestMessageComposer::NewL(MTestObserver& aObserver)
	{
	CTestMessageComposer* self = new(ELeave) CTestMessageComposer(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestMessageComposer::~CTestMessageComposer()
	{
	iHeaders.Close();
	iTransaction.Close();
	iSession.Close();
	iMessageComposer.Close();
	Cancel();
	}

CTestMessageComposer::CTestMessageComposer(MTestObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

void CTestMessageComposer::ConstructL()
	{
	iSession.OpenL();
	iStringPool = iSession.StringPool();
	iMessageComposer.OpenL(*this);
	TUriParser8 up;
	up.Parse(KUri);
	iTransaction = iSession.OpenTransactionL(up, *this, iSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable()));
	}

void CTestMessageComposer::SetHeaderL(TInt aNumberofHeaders)
	{
	switch (aNumberofHeaders)
		{
		case 1:	
			{
			// Open strings used in this test	
			//1.Accept header----  Accept: text/html; q=0.8, text/vnd.wap.wml; q=0.2; extended=value, text/*);
			RStringF textHtml = iStringPool.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
			RStringF textWml  = iStringPool.StringF(HTTP::ETextVndWapWml,RHTTPSession::GetTable());
			RStringF textAny  = iStringPool.StringF(HTTP::ETextAny,RHTTPSession::GetTable());
			RStringF extended  = iStringPool.OpenFStringL(_L8("extended"));
			CleanupClosePushL(extended);
			RStringF extendVal = iStringPool.OpenFStringL(_L8("value"));
			CleanupClosePushL(extendVal);
			iHTTPHeaders = iTransaction.Request().GetHeaderCollection();
			RStringF accept = iStringPool.StringF(HTTP::EAccept,RHTTPSession::GetTable());
			THTTPHdrVal accVal(textHtml);
			THTTPHdrVal q1(THTTPHdrVal::TQConv(0.8));
			iHTTPHeaders.SetFieldL(accept, accVal, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q1);
			accVal.SetStrF(textWml);
			q1.SetInt(THTTPHdrVal::TQConv(0.2));
			iHTTPHeaders.SetFieldL(accept, accVal, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q1);
			q1.SetStrF(extendVal);
			iHTTPHeaders.SetFieldL(accept, accVal, extended, q1);
			accVal.SetStrF(textAny);
			iHTTPHeaders.SetFieldL(accept, accVal);
			CleanupStack::PopAndDestroy(2, &extended);
		
			//2.Accept-Charset header-- Accept-Charset: iso-8859-5, unicode-1-1; q=0.8
			RStringF iso8859_5 = iStringPool.OpenFStringL(_L8("iso-8859-5"));
			CleanupClosePushL(iso8859_5);
			RStringF unicode1_1  = iStringPool.OpenFStringL(_L8("unicode-1-1"));
			CleanupClosePushL(unicode1_1);
			RStringF accChSet= iStringPool.StringF(HTTP::EAcceptCharset,RHTTPSession::GetTable());
			THTTPHdrVal accChSetVal(iso8859_5);
			iHTTPHeaders.SetFieldL(accChSet, accChSetVal);
			//
			accChSetVal.SetStrF(unicode1_1);
			THTTPHdrVal q2(THTTPHdrVal::TQConv(0.8));
			iHTTPHeaders.SetFieldL(accChSet, accChSetVal, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q2);
			CleanupStack::PopAndDestroy(2, &iso8859_5);
			
			//3.User-Agent headers-- User-Agent: CERN-LineMode/2.15 libwww/2.17b3
			// Open strings used in this test
			RStringF ua1Str = iStringPool.OpenFStringL(_L8("CERN-LineMode/2.15"));
			CleanupClosePushL(ua1Str);
			RStringF ua2Str = iStringPool.OpenFStringL(_L8("libwww/2.17b3"));
			CleanupClosePushL(ua2Str);
			//
			RStringF uaStr = iStringPool.StringF(HTTP::EUserAgent,RHTTPSession::GetTable());
			THTTPHdrVal uaVal(ua1Str);
			iHTTPHeaders.SetFieldL(uaStr, uaVal); // sets part 1
			uaVal.SetStrF(ua2Str);
			iHTTPHeaders.SetFieldL(uaStr, uaVal); // sets part 2
			CleanupStack::PopAndDestroy(2,&ua1Str);
			
			//4.Connection Header -- Connection: close, keep-alive
			RStringF otherToken = iStringPool.StringF(HTTP::EKeepAlive,RHTTPSession::GetTable());
			RStringF connStr = iStringPool.StringF(HTTP::EConnection,RHTTPSession::GetTable());
			THTTPHdrVal connVal(iStringPool.StringF(HTTP::EClose,RHTTPSession::GetTable()));
			iHTTPHeaders.SetFieldL(connStr, connVal); // sets part 1
			THTTPHdrVal keepAlive(otherToken);
			iHTTPHeaders.SetFieldL(connStr, keepAlive); // sets part 2
			
			//5.content type header-- Content-Type: text/vnd.wap.wml; charset=utf-8; transcode=yes
			RStringF textWml1   = iStringPool.StringF(HTTP::ETextVndWapWml,RHTTPSession::GetTable());
			RStringF utf8	  = iStringPool.OpenFStringL(_L8("utf-8"));
			CleanupClosePushL(utf8);
			RStringF extended1  = iStringPool.OpenFStringL(_L8("transcode"));
			CleanupClosePushL(extended1);
			RStringF extendVal1 = iStringPool.OpenFStringL(_L8("yes"));
			CleanupClosePushL(extendVal1);
			//
			RStringF contType = iStringPool.StringF(HTTP::EContentType,RHTTPSession::GetTable());
			THTTPHdrVal ctVal(textWml1);
			THTTPHdrVal chSet(utf8);
			iHTTPHeaders.SetFieldL(contType, ctVal, iStringPool.StringF(HTTP::ECharset,RHTTPSession::GetTable()), chSet);
			chSet.SetStrF(extendVal1);
			iHTTPHeaders.SetFieldL(contType, ctVal, extended1, chSet);
			CleanupStack::PopAndDestroy(3,&utf8);
			
			//6.host type header-- Host: nokia.com
			RStringF host1Str = iStringPool.OpenFStringL(_L8("www.nokia.com"));
			CleanupClosePushL(host1Str);
			RStringF host = iStringPool.StringF(HTTP::EHost,RHTTPSession::GetTable());
			THTTPHdrVal hostVal(host1Str);
			iHTTPHeaders.SetFieldL(host, hostVal);
			CleanupStack::PopAndDestroy(&host1Str);
			
			//7.upgrade type header-- Upgrade: HTTP/2.0, SHTTP/1.3
			RStringF http = iStringPool.OpenFStringL(_L8("HTTP/2.0"));
			CleanupClosePushL(http);
			RStringF shttp = iStringPool.OpenFStringL(_L8("SHTTP/1.3"));
			CleanupClosePushL(shttp);
			RStringF upgradeStr = iStringPool.StringF(HTTP::EUpgrade, RHTTPSession::GetTable());
			THTTPHdrVal val(http);
			THTTPHdrVal val2(shttp);
			
			iHTTPHeaders.SetFieldL(upgradeStr, val);
			iHTTPHeaders.SetFieldL(upgradeStr, val2);
			CleanupStack::PopAndDestroy(2);
			
			//8.Cache control -- Cache-Control: public, no-cache=\"wibble,wobble\"
			_LIT8(KPublic, "public");
			_LIT8(KNoCache, "no-cache=\"wibble,wobble\"");
			RStringF publicStr = iStringPool.OpenFStringL(KPublic);
			CleanupClosePushL(publicStr);
			RStringF noCacheStr = iStringPool.OpenFStringL(KNoCache);
			CleanupClosePushL(noCacheStr);
			RStringF cc = iStringPool.StringF(HTTP::ECacheControl,RHTTPSession::GetTable());
			iHTTPHeaders.SetFieldL(cc, THTTPHdrVal(publicStr));
			iHTTPHeaders.SetFieldL(cc, THTTPHdrVal(noCacheStr));
			CleanupStack::PopAndDestroy(2,&publicStr);
			
			//9.Authorization-- Authorization: Basic c3ltYmlhbjpmMXN5bmNtbA==
			RString basicCred = iStringPool.OpenStringL(_L8("c3ltYmlhbjpmMXN5bmNtbA=="));
			CleanupClosePushL(basicCred);
			RStringF authStr = iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable());
			THTTPHdrVal authVal(iStringPool.StringF(HTTP::EBasic,RHTTPSession::GetTable()));
			iHTTPHeaders.SetFieldL(authStr, authVal); // sets part 1
			THTTPHdrVal credVal(basicCred);
			iHTTPHeaders.SetFieldL(authStr, credVal); // sets part 2
			CleanupStack::PopAndDestroy(&basicCred);
			break;
			}
		
		case 2:	
			{
			//1. Connection: Keep-Alive\r\n
			RStringF keep = iStringPool.StringF(HTTP::EKeepAlive,RHTTPSession::GetTable());
			iHTTPHeaders = iTransaction.Request().GetHeaderCollection();
			RStringF Connection= iStringPool.StringF(HTTP::EConnection,RHTTPSession::GetTable());
			THTTPHdrVal keepalive(keep);
			iHTTPHeaders.SetFieldL(Connection,keepalive);
			
			//2.Host: 192.168.100.100\r\n
			_LIT8(KHostIp, "192.168.100.100");
			RStringF host2Str = iStringPool.OpenFStringL(KHostIp);
			CleanupClosePushL(host2Str);
			RStringF host2= iStringPool.StringF(HTTP::EHost,RHTTPSession::GetTable());
			THTTPHdrVal hostip(host2Str);
			iHTTPHeaders.SetFieldL(host2,hostip);
			CleanupStack::PopAndDestroy(&host2Str);
			//3.Accept: */*\r\n
			_LIT8(KAccept2, "*/*");
			RStringF acceptall = iStringPool.OpenFStringL(KAccept2);
			CleanupClosePushL(acceptall);
			RStringF accept2= iStringPool.StringF(HTTP::EAccept,RHTTPSession::GetTable());
			THTTPHdrVal acceptvalue(acceptall);
			iHTTPHeaders.SetFieldL(accept2,acceptvalue);
			CleanupStack::PopAndDestroy(&acceptall);
			
			//4.Accept-Charset: iso-8859-1,utf-8;q=0.7,*;q=0.7\r\n
			RStringF iso8859 = iStringPool.OpenFStringL(_L8("iso-8859-1"));
			CleanupClosePushL(iso8859);
			RStringF utf8_1  = iStringPool.OpenFStringL(_L8("utf-8"));
			CleanupClosePushL(utf8_1);
			RStringF all  = iStringPool.OpenFStringL(_L8("*"));
			CleanupClosePushL(all);
			RStringF accChSet1= iStringPool.StringF(HTTP::EAcceptCharset,RHTTPSession::GetTable());
			THTTPHdrVal accChSetVal1(iso8859);
			iHTTPHeaders.SetFieldL(accChSet1, accChSetVal1);
			accChSetVal1.SetStrF(utf8_1);
			THTTPHdrVal q3(THTTPHdrVal::TQConv(0.7));
			iHTTPHeaders.SetFieldL(accChSet1, accChSetVal1, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q3);
			accChSetVal1.SetStrF(all);
			iHTTPHeaders.SetFieldL(accChSet1, accChSetVal1, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q3);	
			CleanupStack::PopAndDestroy(3,&iso8859);
			
			//5.Accept-Encoding: gzip, deflate, x-gzip, identity; q=0.9\r\n
			RStringF gzip = iStringPool.OpenFStringL(_L8("gzip"));
			CleanupClosePushL(gzip);
			RStringF deflate  = iStringPool.OpenFStringL(_L8("deflate"));
			CleanupClosePushL(deflate);
			RStringF xgzip  = iStringPool.OpenFStringL(_L8("x-gzip"));
			CleanupClosePushL(xgzip);
			RStringF identity  = iStringPool.OpenFStringL(_L8("identity"));
			CleanupClosePushL(identity);
			RStringF acceptencoding1= iStringPool.StringF(HTTP::EAcceptEncoding,RHTTPSession::GetTable());
			THTTPHdrVal aevalue(gzip);
			iHTTPHeaders.SetFieldL(acceptencoding1, aevalue);
			aevalue.SetStrF(deflate);
			iHTTPHeaders.SetFieldL(acceptencoding1, aevalue);
			aevalue.SetStrF(xgzip);
			iHTTPHeaders.SetFieldL(acceptencoding1, aevalue);
			aevalue.SetStrF(identity);
			THTTPHdrVal q4(THTTPHdrVal::TQConv(0.9));
			iHTTPHeaders.SetFieldL(acceptencoding1, aevalue, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q4);
			CleanupStack::PopAndDestroy(4,&gzip);
			
			//6.Accept-Language: en;q=1.0,bg;q=0.5,nl;q=0.5,fr;q=0.5\r\n
			RStringF en = iStringPool.OpenFStringL(_L8("en"));
			CleanupClosePushL(en);
			RStringF bg  = iStringPool.OpenFStringL(_L8("bg"));
			CleanupClosePushL(bg);
			RStringF nl  = iStringPool.OpenFStringL(_L8("nl"));
			CleanupClosePushL(nl);
			RStringF fr  = iStringPool.OpenFStringL(_L8("fr"));
			CleanupClosePushL(fr);
			RStringF acceptlanguage1= iStringPool.StringF(HTTP::EAcceptLanguage,RHTTPSession::GetTable());
			THTTPHdrVal alvalue(en);
			THTTPHdrVal q5(THTTPHdrVal::TQConv(1.0));
			iHTTPHeaders.SetFieldL(acceptlanguage1, alvalue, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q5);
			THTTPHdrVal q6(THTTPHdrVal::TQConv(0.5));
			alvalue.SetStrF(bg);
			iHTTPHeaders.SetFieldL(acceptlanguage1, alvalue, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q6);
			alvalue.SetStrF(nl);
			iHTTPHeaders.SetFieldL(acceptlanguage1, alvalue, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q6);
			alvalue.SetStrF(fr);
			iHTTPHeaders.SetFieldL(acceptlanguage1, alvalue, iStringPool.StringF(HTTP::EQ,RHTTPSession::GetTable()), q6);
			CleanupStack::PopAndDestroy(4,&en);
		
			//7.Referer: http://192.168.100.100/il/index.html\r\n
			RStringF referer = iStringPool.OpenFStringL(_L8("http://192.168.100.100/il/index.html"));
			CleanupClosePushL(referer);
			RStringF refer= iStringPool.StringF(HTTP::EReferer,RHTTPSession::GetTable());
			THTTPHdrVal referval(referer);
			iHTTPHeaders.SetFieldL(refer,referval);
			CleanupStack::PopAndDestroy(&referer);	
			
			//8.User-Agent: Mozilla/5.0 AppleWebKit/525
			RStringF ua1Str1 = iStringPool.OpenFStringL(_L8("Mozilla/5.0"));
			CleanupClosePushL(ua1Str1);
			RStringF ua2Str2 = iStringPool.OpenFStringL(_L8("AppleWebKit/525"));
			CleanupClosePushL(ua2Str2);
			RStringF uaString = iStringPool.StringF(HTTP::EUserAgent,RHTTPSession::GetTable());
			THTTPHdrVal uaVal1(ua1Str1);
			iHTTPHeaders.SetFieldL(uaString, uaVal1); // sets part 1
			uaVal1.SetStrF(ua2Str2);
			iHTTPHeaders.SetFieldL(uaString, uaVal1); // sets part 2
			CleanupStack::PopAndDestroy(2,&ua1Str1);
			break;
			}
			
		default:
			break;	
		}
	}

void CTestMessageComposer::StartRequest()
	{
	// Self-complete to kick-off the test.
	CompleteSelf();
	}

void CTestMessageComposer::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CTestMessageComposer::DoReset()
	{
	}

/*
 *	Methods from CActive
 */

void CTestMessageComposer::RunL()
	{
	iHeaderIndex = 0;
	iFlag = ETrue;
	// Start the composer...
	iMessageComposer.MessageInfoAvailable();
	}

void CTestMessageComposer::DoCancel()
	{
	// Do nothing...
	}

TInt CTestMessageComposer::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

/*
 *	Methods from MHttpMessageComposerObserver
 */

void CTestMessageComposer::StartLineL(TPtrC8& aMethod, TPtrC8& aRequestUri, TPtrC8& aVersion)
	{
	// The RFC2616 defines the Request-Line as follows - 
	// 
	// Request-Line = Method SP Request-URI SP HTTPVersion CRLF
	//
	// Get Method...
	iRequest = iTransaction.Request();
	RStringF method = iStringPool.StringF(iRequest.Method());
	aMethod.Set(method.DesC());
	
	// Get the Request-URI...
	aRequestUri.Set(KUri);
	
	// Assume HTTP/1.1
	aVersion.Set(iStringPool.StringF(HTTP::EHttp11, RHTTPSession::GetTable()).DesC());
	}

TInt CTestMessageComposer::NextHeaderL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue)
	{
	if(iFlag)
	{
		THTTPHdrFieldIter it = iHTTPHeaders.Fields();
		iFlag = EFalse;
		while(!it.AtEnd())
			{
			// Get field current field.
			RStringF name = iStringPool.StringF(it());
			CleanupClosePushL(name);
			// Get the OTA version of the field value
			TPtrC8 value;
			iRequest.GetHeaderCollection().GetRawField(name, value);
			User::LeaveIfError(iHeaders.Append(THeaderField(name.DesC(), value)));		
			// Move onto next header field...
			++it;
			CleanupStack::PopAndDestroy(&name);
			}
	}
	TInt error = ( iHeaderIndex < iHeaders.Count() ) ? KErrNone : KErrNotFound;
	if( iHeaderIndex < iHeaders.Count() )
		{
		// Give the current header info...
		THeaderField header = iHeaders[iHeaderIndex];

		aHeaderName.Set(header.iName);
		aHeaderValue.Set(header.iValue);

		// Loop to next header...
		++iHeaderIndex;
		}
	else
		{
		// No more headers - change state
		iState = EPendingHasBody;
		}
	return error;
	}



TInt CTestMessageComposer::NextTrailerL(TPtrC8& /*aHeaderName*/, TPtrC8& /*aHeaderValue*/)
	{
	return KErrNotFound;
	}

void CTestMessageComposer::MessageComplete()
	{
	iState = EDone;
	iObserver.NotifyComplete();
	}

void CTestMessageComposer::MessageDataReadyL()
	{
	//		// Get data from the composer
	TPtrC8 data;
	iMessageComposer.GetMessageData(data);
	iMessageComposer.ReleaseMessageData();
	}

TInt CTestMessageComposer::HandleComposeError(TInt /*aError*/)
	{
	return KErrNone;
	}

MHTTPDataSupplier* CTestMessageComposer::HasBodyL()
	{
	iState = EPendingMessageComplete;
	return NULL;
	}

void CTestMessageComposer::Reserved_MHttpMessageComposerObserver()
	{
	User::Invariant();
	}

TInt CTestMessageComposer::MHFRunError(TInt /*aError*/, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return KErrNone;
	}

void CTestMessageComposer::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}

