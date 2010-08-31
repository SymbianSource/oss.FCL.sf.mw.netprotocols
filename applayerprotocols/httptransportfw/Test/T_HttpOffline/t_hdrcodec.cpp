// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Implementation of CHttpHdrCodecTest
// 
//

#include <inetprottextutils.h>
#include "t_hdrcodec.h"

_LIT8(KFieldSeparator, "\n");


void CHttpHdrCodecTest::DoRunL()
	{
	iExpectedStatusCode = 0;
	// insert test code here, allow it to leave if the test fails
	RunTestsL();
	}
	
TInt CHttpHdrCodecTest::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpHdrCodecTest::DoCancel()
	{
	}
	
const TDesC& CHttpHdrCodecTest::TestName()
	{
	_LIT(KHeaderTestName,"CHttpHdrCodecTest");
	return KHeaderTestName;
	}

CHttpHdrCodecTest::CHttpHdrCodecTest()
// Constructor
	{
	// Initial timestamp is time now
	ResetTimeElapsed();
	}

CHttpHdrCodecTest::~CHttpHdrCodecTest()
// Destructor (virtual)
	{
	}

CHttpHdrCodecTest* CHttpHdrCodecTest::NewLC()
// Create a new CHttpHdrCodecTest and place it on the clean-up stack then return it
	{
	CHttpHdrCodecTest* me = new(ELeave) CHttpHdrCodecTest;
	CleanupStack::PushL(me);
	return me;
	}

CHttpHdrCodecTest* CHttpHdrCodecTest::NewL()
// Create a new CHttpHdrCodecTest and return it
	{
	CHttpHdrCodecTest* me = new(ELeave) CHttpHdrCodecTest;
	return me;
	}

void CHttpHdrCodecTest::RunTestsL()
	{
	// start the session
	iSession.OpenL();
	CleanupClosePushL(iSession);
	// Locate the string pool
	iStrP = iSession.StringPool();

	_LIT8(KTxtHTTPProt, "HTTP/TCP");
	iProtHnd = CProtocolHandler::NewL(KTxtHTTPProt(), iSession);
	CleanupStack::PushL(iProtHnd);

	iEngine->Console().Printf(_L("\nStart of header codec tests"));
	ResetTimeElapsed();

	// Run tests on parsed -> OTA data conversions, for each header implemented
	TestDecodeEncodeCacheControlL();
	TestEncodeAcceptL();
	TestEncodeAcceptCharsetL();
	TestEncodeAuthorizationL();
	TestEncodeConnectionL();
	TestEncodeContentLengthL();
	TestEncodeContentTypeL();
	TestEncodeHostL();
	TestEncodeTransferEncodingL();
	TestEncodeUserAgentL();
	TestEncodeDateL();
	TestEncodeLocaleIndependentDateL();
	TestEncodeUpgradeL();

	TestEncodeCookieL();
	TestEncodeCacheControlL();

	// Run tests on OTA data -> parsed conversions, for each header implemented
	TestDecodeConnectionL();
	TestDecodeContentLengthL();
	TestDecodeContentTypeL();
	TestDecodeContentTypeWithMultipleParametersL();
	TestDecodeContentTypeMissingParameterL();
	TestDecodeServerL();
	TestDecodeDateL();
	TestDecodeTransferEncodingL();
	TestDecodeWWWAuthenticateL();
	TestDecodeUpgradeL();

	TestDecodeSetCookieL();
	TestDecodeSetCookie2L();
	TestDecodeSetCookie3L();
	TestDecodeSetCookie4L();
	//test for DEF080697: Cookies: When sending a request to an origin server, Browser is not sending the
	TestEncodeCookiePortL();


	TestDecodeCacheControlL();

	// Do some round trip encode->decode and check the original input is the same as the final output
	TestRoundTripConnectionL();
	TestRoundTripContentLengthL();
	TestRoundTripContentLength2L();
	TestRoundTripContentTypeL();
	TestRoundTripTransferEncodingL();
	
	TestDuplicateHeaderResponsesL();
	
	//test for DEF078684: Authorization header nonce count is sent incorrectly
	TestEncodeAuthorization1L();

	//test for INC118854:Invalid parsing of HTTP headers when value contains a ";" character 
	TestDecodeContentDispositionL();

	TestGetCustomFieldValueL();
	
	TestGetCustomFieldNullValueL();

	TestGetCustomFieldNoValueL();
	
	// test for PDEF138861: Trying to access header field part at Index < 0
	TestInvalidHeaderFieldPartL ();
	
	// End of tests
	DisplayTimeElapsed();
	iEngine->Console().Printf(_L("\nEnd of header codec tests"));
	CleanupStack::PopAndDestroy(2,&iSession);
	}

void CHttpHdrCodecTest::TestDuplicateHeaderResponsesL()
	{
	// Test Headers that are handled by textmode codec
	
	// Response with 2 connection type headers
	TInt numParts=2; 
	RStringF header = iStrP.StringF(HTTP::EConnection, RHTTPSession::GetTable());
	_LIT8(KConnectionRawHeader1, "close\nkeep-alive");
	TestDuplicateHeaderDecodingL(header,KConnectionRawHeader1, numParts);
	
	_LIT8(KConnectionRawHeader2, "close, keep-alive");
	TestDuplicateHeaderDecodingL(header,KConnectionRawHeader2, numParts);
	
	// Response with 2 content type headers
	numParts=1;
	header = iStrP.StringF(HTTP::EContentType, RHTTPSession::GetTable());
	_LIT8(KContentTypeRawHeader1, "text/html\ntext/html");
	TestDuplicateHeaderDecodingL(header, KContentTypeRawHeader1,numParts);
					
	// Response with 2 different date headers 
	// This also checks Expires and Last-Modified as they use the same generic date parsing in
	// the codec
	header = iStrP.StringF(HTTP::EDate, RHTTPSession::GetTable());
	_LIT8(KDateRawHeader1, "Tue, 15 Nov 1994 08:12:31 GMT\n Wed, 16 Nov 1994 09:00:30 GMT");
	TestDuplicateHeaderDecodingL(header, KDateRawHeader1, numParts);					

    // Response with 3 different content length headers
	// This also checks Age as that uses the same generic number parsing
	header = iStrP.StringF(HTTP::EContentLength, RHTTPSession::GetTable());
	_LIT8(KContentLengthRawHeader1, "10\n200\n3000");
	TestDuplicateHeaderDecodingL(header, KContentLengthRawHeader1, numParts);					

    
    // Response with 2 different transfer-encodings
    numParts=2;
    header = iStrP.StringF(HTTP::ETransferEncoding, RHTTPSession::GetTable());
    _LIT8(KTransferEncodingRawHeader1, "chunked\nchunked2");
    TestDuplicateHeaderDecodingL(header, KTransferEncodingRawHeader1, numParts);
    
    // Response with 2 different WWWAuthenticate headers
    header = iStrP.StringF(HTTP::EWWWAuthenticate, RHTTPSession::GetTable());
    _LIT8(KWWWAuthenticateRawHeader1, "basic realm=castle\nbasic realm=magic");
    TestDuplicateHeaderDecodingL(header, KWWWAuthenticateRawHeader1, numParts);
	
	// Response with 2 different SetCookie headers
	// Also tests SetCookie2 as that goes through same codec
	header = iStrP.StringF(HTTP::ESetCookie, RHTTPSession::GetTable());
    _LIT8(KSetCookieRawHeader1, "cookie=chocolate\ncookie=ginger");
    TestDuplicateHeaderDecodingL(header, KSetCookieRawHeader1, numParts);
	
    // Response with 2 cache control headers
    header = iStrP.StringF(HTTP::ECacheControl, RHTTPSession::GetTable());
    _LIT8(KCacheControlRawHeader1, "max-age=100\nno-transform");
    TestDuplicateHeaderDecodingL(header, KCacheControlRawHeader1, numParts);
    
    
    header = iStrP.StringF(HTTP::ECacheControl, RHTTPSession::GetTable());
    _LIT8(KCacheControlRawHeader2, "max-age=100, no-transform");
    TestDuplicateHeaderDecodingL(header, KCacheControlRawHeader2, numParts);
	
    
	// Response with 2 pragma headers
	// This also tests vary and content-langugage which both use same generic parser
	header = iStrP.StringF(HTTP::EPragma, RHTTPSession::GetTable());
    _LIT8(KPragmaRawHeader1, "pragma1\npragma2");
    TestDuplicateHeaderDecodingL(header, KPragmaRawHeader1, numParts);
    
    
    // Test some headers that will use the default codec
    numParts=2;
    // Response with 2 ETag headers
    header = iStrP.StringF(HTTP::EETag, RHTTPSession::GetTable());
    _LIT8(KETagRawHeader1, "\"xxxyz\"\n\"xyyz\"");
    TestDuplicateHeaderDecodingL(header, KETagRawHeader1, numParts);
	
	header = iStrP.StringF(HTTP::EETag, RHTTPSession::GetTable());
    _LIT8(KETagRawHeader2, "\"xxxyz\"\n\"xyyz\"   ");
    TestDuplicateHeaderDecodingL(header, KETagRawHeader2, numParts);
	
	
	
	// Default codec with one header
	numParts=1;
    header = iStrP.StringF(HTTP::EETag, RHTTPSession::GetTable());
    _LIT8(KETagRawHeader3, "\"xxxyz\"");
    TestDuplicateHeaderDecodingL(header, KETagRawHeader3, numParts);
    
	iEngine->Utils().LogIt ( _L ("\nTestDuplicateHeaderResponsesL test success."));
	}


void CHttpHdrCodecTest::TestDuplicateHeaderDecodingL(RStringF aHeader,
												 const TDesC8& aRawResponseHeader,
												 TInt aExpectedNumberParts)
	/* This is to test decoding of multiple response headers
	   HTTP Framework deals with these by joining them together using \n as a seperator
	*/
	{
	RHTTPHeaders headers = GetHeadersLC();
	headers.SetRawFieldL(aHeader, aRawResponseHeader, KFieldSeparator);
	
	// Check parsing of raw header. Framework should only return first header if there are duplicates
	// Don't bother checking multiple values on one line.
	TPtrC8 rawHeader(KNullDesC8);
	User::LeaveIfError(headers.GetRawField(aHeader,rawHeader));
	TInt firstNewLinePosition = aRawResponseHeader.Locate('\n');
	if (firstNewLinePosition != KErrNotFound)
		{
		TPtrC8 expectedRawResponse(rawHeader.Left(firstNewLinePosition));
		if (rawHeader.CompareF(expectedRawResponse) != 0)
			{
			TBuf < 1024 > rawHeader16;
			TBuf < 1024 > expectedRawResponse16;
			rawHeader16.Copy ( rawHeader );
			expectedRawResponse16.Copy ( expectedRawResponse );
			
			iEngine->Utils().LogIt ( _L("Raw header comparison failed -> %S != %S"), &rawHeader16, &expectedRawResponse16 );
			User::Leave(KErrArgument);
			}
		}

	
	// Check parsing when converting through codec
	TInt numberParts = headers.FieldPartsL(aHeader);
	if (numberParts != aExpectedNumberParts)
		{
		iEngine->Utils().LogIt ( _L("\nNumber parts differ from expected. -> %d != %d"), numberParts, aExpectedNumberParts );
		User::Leave(KErrArgument);
		}
	
	
	ReleaseHeaders();
	
	iEngine->Utils().LogIt ( _L ("\nTestDuplicateHeaderDecodingL test success."));
	}


void CHttpHdrCodecTest::TestDecodeEncodeCacheControlL()
	{
	_LIT8(KCacheControlOTAVal, "private, max-age=360, must-revalidate");
	RStringF cacheControl = iStrP.StringF(HTTP::ECacheControl,RHTTPSession::GetTable());
	RStringF maxAge = iStrP.StringF(HTTP::EMaxAge,RHTTPSession::GetTable());
	RHTTPHeaders hdr = GetHeadersLC();
	hdr.SetRawFieldL(cacheControl, KCacheControlOTAVal, KFieldSeparator);
	if(hdr.FieldPartsL(cacheControl) != 3)
	    {
        iEngine->Utils().LogIt(_L("\nCache-Control OTA comparison  failed -> [parts != 3]"));
        User::Leave(KErrCorrupt);
	    }
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr.GetField(cacheControl,0,hVal));
	if(hVal.StrF().DesC().Compare(_L8("private")) != 0)
	    {
	    iEngine->Utils().LogIt(_L("\nCache-Control OTA comparison  failed -> [private]"));
	    User::Leave(KErrCorrupt);
	    }
	THTTPHdrVal paramVal;
	User::LeaveIfError(hdr.GetParam(cacheControl, maxAge, paramVal, 1));
	if(paramVal.Int() != 360)
	    {
	    iEngine->Utils().LogIt(_L("\nCache-Control OTA comparison  failed -> [300]"));
	    User::Leave(KErrCorrupt);
	    }
	THTTPHdrVal hVal2;
	User::LeaveIfError(hdr.GetField(cacheControl,2,hVal2));
    if(hVal2.StrF().DesC().Compare(_L8("must-revalidate")) != 0)
        {
        iEngine->Utils().LogIt(_L("\nmust-revalidate OTA comparison  failed -> [private, max-age=300, must-revalidate]"));
        User::Leave(KErrCorrupt);
        }
	
	TPtrC8 rawHeader;
	hdr.GetRawField(cacheControl, rawHeader);
	if(rawHeader.CompareF(KCacheControlOTAVal) != 0)
	    {
	    iEngine->Utils().LogIt(_L("\nmust-revalidate OTA comparison  failed -> [300]"));
	    User::Leave(KErrCorrupt);
	    }
	
	ReleaseHeaders();
	
	iEngine->Utils().LogIt ( _L ("\nTestDecodeEncodeCacheControlL test success."));
	}

void CHttpHdrCodecTest::TestEncodeAcceptL()
	{
	// the result we expect and that we will validate
	_LIT8(KAcceptOTAVal, "text/html; q=0.8, text/vnd.wap.wml; q=0.2; extended=value, text/*");

	// Open strings used in this test
	RStringF textHtml = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF textWml  = iStrP.StringF(HTTP::ETextVndWapWml,RHTTPSession::GetTable());
	RStringF textAny  = iStrP.StringF(HTTP::ETextAny,RHTTPSession::GetTable());

	RStringF extended  = iStrP.OpenFStringL(_L8("extended"));
	CleanupClosePushL(extended);
	RStringF extendVal = iStrP.OpenFStringL(_L8("value"));
	CleanupClosePushL(extendVal);
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Accept' header"));
	
	RHTTPHeaders hdr = 	GetHeadersLC();

	// Set up the parsed header
	RStringF accept = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	THTTPHdrVal accVal(textHtml);
	THTTPHdrVal q(THTTPHdrVal::TQConv(0.8));
	hdr.SetFieldL(accept, accVal, iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable()), q);
	accVal.SetStrF(textWml);
	q.SetInt(THTTPHdrVal::TQConv(0.2));
	hdr.SetFieldL(accept, accVal, iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable()), q);
	q.SetStrF(extendVal);
	hdr.SetFieldL(accept, accVal, extended, q);
	accVal.SetStrF(textAny);
	hdr.SetFieldL(accept, accVal);

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(accept, ota);
	if (ota.Compare(KAcceptOTAVal()) != 0)
		{		
		TBuf<512> ota16;
		ota16.Copy (ota);		
		iEngine->Utils().LogIt ( _L("\nAccept OTA comparison failed -> %S"), &ota16 );
		User::Leave(KErrArgument);		
		}
		

	// Close strings used in this test
	ReleaseHeaders();
	CleanupStack::PopAndDestroy(2, &extended);
	
	iEngine->Utils().LogIt ( _L ("\nTestEncodeAcceptL test success."));
	}

void CHttpHdrCodecTest::TestEncodeAcceptCharsetL()
	{
	// the result we expect and that we will validate
	_LIT8(KAccChSetOTAVal, "iso-8859-5, unicode-1-1; q=0.8");

	// Open strings used in this test
	RStringF iso8859_5 = iStrP.OpenFStringL(_L8("iso-8859-5"));
	CleanupClosePushL(iso8859_5);
	RStringF unicode1_1  = iStrP.OpenFStringL(_L8("unicode-1-1"));
	CleanupClosePushL(unicode1_1);

	iEngine->Console().Printf(_L("\nTest encoding 'Accept-Charset' header"));
	RHTTPHeaders hdr = GetHeadersLC();
	
	// Set up the parsed header
	RStringF accChSet= iStrP.StringF(HTTP::EAcceptCharset,RHTTPSession::GetTable());
	THTTPHdrVal accChSetVal(iso8859_5);
	hdr.SetFieldL(accChSet, accChSetVal);
	//
	accChSetVal.SetStrF(unicode1_1);
	THTTPHdrVal q(THTTPHdrVal::TQConv(0.8));
	hdr.SetFieldL(accChSet, accChSetVal, iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable()), q);

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(accChSet, ota);
	if (ota.Compare(KAccChSetOTAVal()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);		
		iEngine->Utils().LogIt ( _L("\nComparison of charset field failed -> %S"), &ota16 );	
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();

	CleanupStack::PopAndDestroy(2, &iso8859_5);
	iEngine->Utils().LogIt ( _L ("\nTestEncodeAcceptCharsetL test success."));
	}

//test for DEF078684: Authorization header nonce count is sent incorrectly
void CHttpHdrCodecTest::TestEncodeAuthorization1L()
{
	// the result we expect and that we will validate
	_LIT8(KAuthorizationOTAVal, "Digest nc=00000001, algorithm=MD5, nonce=\"999999\"");
	
	// Open strings used in this test
	RString ncVal = iStrP.OpenStringL(_L8("00000001"));
	CleanupClosePushL(ncVal);

	RString algVal = iStrP.OpenStringL(_L8("MD5"));
	CleanupClosePushL(algVal);

	RString nonceVal = iStrP.OpenStringL(_L8("999999"));
	CleanupClosePushL(nonceVal);
	
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Authorization' header"));
	RHTTPHeaders hdr = GetHeadersLC();

	RStringF authStr = iStrP.StringF(HTTP::EAuthorization,RHTTPSession::GetTable());
	THTTPHdrVal authVal(iStrP.StringF(HTTP::EDigest,RHTTPSession::GetTable()));
	THTTPHdrVal hdrNcVal(ncVal);

	RStringF ncStr = iStrP.StringF(HTTP::ENc,RHTTPSession::GetTable());
	hdr.SetFieldL(authStr, authVal,ncStr,hdrNcVal);

	THTTPHdrVal hdralgVal(algVal);
	RStringF algStr = iStrP.StringF(HTTP::EAlgorithm,RHTTPSession::GetTable());
	hdr.SetFieldL(authStr, authVal,algStr,hdralgVal);

	THTTPHdrVal hdrnonceVal(nonceVal);
	RStringF nonceStr = iStrP.StringF(HTTP::ENonce,RHTTPSession::GetTable());
	hdr.SetFieldL(authStr, authVal,nonceStr,hdrnonceVal);
	
	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(authStr, ota);
	if (ota.Compare(KAuthorizationOTAVal()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);
		iEngine->Utils().LogIt ( _L("\nComparison of Authorization field failed -> %S"), &ota16 );	
		User::Leave(KErrArgument);
		}

	ReleaseHeaders();
	
	CleanupStack::PopAndDestroy(&nonceVal);
	CleanupStack::PopAndDestroy(&algVal);
	CleanupStack::PopAndDestroy(&ncVal);
	iEngine->Utils().LogIt ( _L ("\nTestEncodeAuthorizationL test success."));
}
void CHttpHdrCodecTest::TestEncodeAuthorizationL()
	{
	// the result we expect and that we will validate
	_LIT8(KAuthorizationOTAVal, "Basic c3ltYmlhbjpmMXN5bmNtbA==");

	// Open strings used in this test
	RString basicCred = iStrP.OpenStringL(_L8("c3ltYmlhbjpmMXN5bmNtbA=="));
	CleanupClosePushL(basicCred);
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Authorization' header"));
	RHTTPHeaders hdr = GetHeadersLC();

	RStringF authStr = iStrP.StringF(HTTP::EAuthorization,RHTTPSession::GetTable());
	THTTPHdrVal authVal(iStrP.StringF(HTTP::EBasic,RHTTPSession::GetTable()));
	hdr.SetFieldL(authStr, authVal); // sets part 1
	THTTPHdrVal credVal(basicCred);
	hdr.SetFieldL(authStr, credVal); // sets part 2

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(authStr, ota);
	if (ota.Compare(KAuthorizationOTAVal()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);
		iEngine->Utils().LogIt ( _L("\nComparison of Authorization field failed -> %S"), &ota16 );	
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();
	
	CleanupStack::PopAndDestroy(&basicCred);
	iEngine->Utils().LogIt ( _L ("\nTestEncodeAuthorizationL test success."));
	}

void CHttpHdrCodecTest::TestEncodeConnectionL()
	{
	// the result we expect and that we will validate
	_LIT8(KConnectionOTAVal, "close, keep-alive");

	// Open strings used in this test
	RStringF otherToken = iStrP.StringF(HTTP::EKeepAlive,RHTTPSession::GetTable());
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Connection' header"));
	RHTTPHeaders hdr = GetHeadersLC();
	RStringF connStr = iStrP.StringF(HTTP::EConnection,RHTTPSession::GetTable());
	THTTPHdrVal connVal(iStrP.StringF(HTTP::EClose,RHTTPSession::GetTable()));
	hdr.SetFieldL(connStr, connVal); // sets part 1
	THTTPHdrVal keepAlive(otherToken);
	hdr.SetFieldL(connStr, keepAlive); // sets part 2

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(connStr, ota);
	if (ota.CompareF(KConnectionOTAVal()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);
		iEngine->Utils().LogIt ( _L("\nComparison of connection field failed -> %S"), &ota16 );	
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();
	
	iEngine->Utils().LogIt ( _L ("\nTestEncodeConnectionL test success.\n"));
	}

void CHttpHdrCodecTest::TestEncodeContentLengthL()
	{
	// the result we expect and that we will validate
	_LIT8(KContentLengthOTAVal, "12345");
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Content-Length' header"));
	RHTTPHeaders hdr = GetHeadersLC();
	RStringF lengthStr = iStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable());
	THTTPHdrVal lengthVal(12345);
	hdr.SetFieldL(lengthStr, lengthVal); // sets part 1

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(lengthStr, ota);
	if (ota.Compare(KContentLengthOTAVal()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);
		iEngine->Utils().LogIt ( _L("\nComparison of content length field value failed. -> %S"), &ota16 );	
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestEncodeContentLengthL test success."));
	}

void CHttpHdrCodecTest::TestEncodeContentTypeL()
	{
	// the result we expect and that we will validate
	_LIT8(KContentTypeOTAVal1, "text/plain");
	_LIT8(KContentTypeOTAVal2, "text/html; charset=us-ascii");
	_LIT8(KContentTypeOTAVal3, "text/vnd.wap.wml; charset=utf-8; transcode=yes");

	// Open strings used in this test
	RStringF textHtml  = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF textWml   = iStrP.StringF(HTTP::ETextVndWapWml,RHTTPSession::GetTable());
	RStringF textPlain = iStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());

	RStringF usAscii   = iStrP.OpenFStringL(_L8("us-ascii"));
	CleanupClosePushL(usAscii);
	RStringF utf8	  = iStrP.OpenFStringL(_L8("utf-8"));
	CleanupClosePushL(utf8);
	RStringF extended  = iStrP.OpenFStringL(_L8("transcode"));
	CleanupClosePushL(extended);
	RStringF extendVal = iStrP.OpenFStringL(_L8("yes"));
	CleanupClosePushL(extendVal);
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Content-Type' header (no params)"));
	RHTTPHeaders hdr = GetHeadersLC();

	// Set up the parsed header
	RStringF contType = iStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable());
	THTTPHdrVal ctVal(textPlain);
	hdr.SetFieldL(contType, ctVal);

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(contType, ota);
	if (ota.Compare(KContentTypeOTAVal1()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);
		iEngine->Utils().LogIt ( _L("\nComparison of content type (no params) field failed. -> %S"), &ota16);	
		User::Leave(KErrArgument);
		}
		
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Content-Type' header (with charset parameter)"));
	hdr.RemoveField(contType);
	ctVal.SetStrF(textHtml);
	THTTPHdrVal chSet(usAscii);
	hdr.SetFieldL(contType, ctVal, iStrP.StringF(HTTP::ECharset,RHTTPSession::GetTable()), chSet);
	hdr.GetRawField(contType, ota);
	if (ota.Compare(KContentTypeOTAVal2()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);
		iEngine->Utils().LogIt ( _L("Comparison of content type (with charset parameter) failed -> %S"), &ota16 );	
		User::Leave(KErrArgument);
		}
		
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Content-Type' header (with two parameters)"));
	hdr.RemoveField(contType);
	ctVal.SetStrF(textWml);
	chSet.SetStrF(utf8);
	hdr.SetFieldL(contType, ctVal, iStrP.StringF(HTTP::ECharset,RHTTPSession::GetTable()), chSet);
	chSet.SetStrF(extendVal);
	hdr.SetFieldL(contType, ctVal, extended, chSet);
	hdr.GetRawField(contType, ota);
	if (ota.Compare(KContentTypeOTAVal3()) != 0)
		{
		TBuf<512> ota16;
		ota16.Copy (ota);
		iEngine->Utils().LogIt ( _L("Comparison of content type (with two parameters) field failed -> %S"), &ota16 );			
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();
	CleanupStack::PopAndDestroy(4,&usAscii);
	iEngine->Utils().LogIt ( _L ("\nTestEncodeContentTypeL test success."));
	}

void CHttpHdrCodecTest::TestEncodeHostL()
	{
	_LIT8(KHostOTAVal1, "www.symbian.com"); // ota host header
	_LIT8(KHostOTAVal2, "www.psion.com:8080"); // ota host header

	// Open strings used in this test
	RStringF host1Str = iStrP.OpenFStringL(_L8("www.symbian.com"));
	CleanupClosePushL(host1Str);
	RStringF host2Str = iStrP.OpenFStringL(_L8("www.psion.com"));
	CleanupClosePushL(host2Str);
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Host' header (no port)"));
	RHTTPHeaders hdr = GetHeadersLC();
	RStringF host = iStrP.StringF(HTTP::EHost,RHTTPSession::GetTable());
	THTTPHdrVal hostVal(host1Str);
	hdr.SetFieldL(host, hostVal);

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(host, ota);
	if (ota.Compare(KHostOTAVal1()) != 0)
		{
		TBuf < 512 > ota16;
		ota16.Copy ( ota );
		
		iEngine->Utils().LogIt ( _L("\nComparison of host field failed. -> %S"), &ota16 );		
		User::Leave(KErrArgument);	
		}
	
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Host' header (with port parameter)"));
	hdr.RemoveField(host);
	RStringF port = iStrP.StringF(HTTP::EPort,RHTTPSession::GetTable());
	hostVal.SetStrF(host2Str);
	THTTPHdrVal portNum(8080);
	hdr.SetFieldL(host, hostVal, port, portNum);

	// now get the OTA data
	hdr.GetRawField(host, ota);
	if (ota.Compare(KHostOTAVal2()) != 0)
		{
		TBuf < 512 > ota16;
		ota16.Copy ( ota );
		iEngine->Utils().LogIt ( _L("\nComparison of host (with port parameter) field failed -> %S"), &ota16 );
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();

	CleanupStack::PopAndDestroy(2,&host1Str);
	iEngine->Utils().LogIt(_L("\nTestEncodeHostL test success."));
	}

void CHttpHdrCodecTest::TestEncodeTransferEncodingL()
	{
	// the result we expect and that we will validate
	_LIT8(KTransferEncodingOTAVal, "chunked, another");

	// Open strings used in this test
	RStringF anotherToken = iStrP.OpenFStringL(_L8("another"));
	CleanupClosePushL(anotherToken);
	//
	iEngine->Console().Printf(_L("\nTest encoding 'Transfer-Encoding' header"));
	RHTTPHeaders hdr = GetHeadersLC();
	RStringF trEncStr = iStrP.StringF(HTTP::ETransferEncoding,RHTTPSession::GetTable());
	THTTPHdrVal trEncVal(iStrP.StringF(HTTP::EChunked,RHTTPSession::GetTable()));
	hdr.SetFieldL(trEncStr, trEncVal); // sets part 1
	trEncVal.SetStrF(anotherToken);
	hdr.SetFieldL(trEncStr, trEncVal); // sets part 2

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(trEncStr, ota);
	if(ota.Compare(KTransferEncodingOTAVal()) != 0)
		{
		TBuf < 512 > ota16;
		ota16.Copy ( ota );
		iEngine->Utils().LogIt ( _L("\nComparison of Transfer-Encoding field failed. -> %S"), &ota16 );
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();

	CleanupStack::PopAndDestroy(&anotherToken);
	iEngine->Utils().LogIt ( _L ("\nTestEncodeTransferEncodingL test success."));
	}

void CHttpHdrCodecTest::TestEncodeUserAgentL()
	{
 	// the result we expect and that we will validate
	_LIT8(KUserAgentOTAVal, "CERN-LineMode/2.15 libwww/2.17b3");

	// Open strings used in this test
	RStringF ua1Str = iStrP.OpenFStringL(_L8("CERN-LineMode/2.15"));
	CleanupClosePushL(ua1Str);
	RStringF ua2Str = iStrP.OpenFStringL(_L8("libwww/2.17b3"));
	CleanupClosePushL(ua2Str);
	//
	iEngine->Console().Printf(_L("\nTest encoding 'User-Agent' header"));
	RHTTPHeaders hdr = GetHeadersLC();
	RStringF uaStr = iStrP.StringF(HTTP::EUserAgent,RHTTPSession::GetTable());
	THTTPHdrVal uaVal(ua1Str);
	hdr.SetFieldL(uaStr, uaVal); // sets part 1
	uaVal.SetStrF(ua2Str);
	hdr.SetFieldL(uaStr, uaVal); // sets part 2

	// now get the OTA data
	TPtrC8 ota;
	hdr.GetRawField(uaStr, ota);
	if(ota.Compare(KUserAgentOTAVal()) != 0)
		{
		TBuf < 512 > ota16;
		ota16.Copy ( ota );
		iEngine->Utils().LogIt ( _L("\nComparison of User-Agent field failed. -> %S"), &ota16 );
		User::Leave(KErrArgument);
		}
		
	
	ReleaseHeaders();

	CleanupStack::PopAndDestroy(2,&ua1Str);
	iEngine->Utils().LogIt ( _L ("\nTestEncodeUserAgentL test success."));
	}

void CHttpHdrCodecTest::TestEncodeCookieL()
	{
	_LIT8(KEncodedCookieHeader1, "CUSTOMER=WILE_E_COYOTE");
	_LIT8(KEncodedCookieHeader2, "CUSTOMER=WILE_E_COYOTE; PART_NUMBER=ROCKETLAUNCHER_0001");
	_LIT8(KEncodedCookieHeader3, "CUSTOMER=WILE_E_COYOTE; PART_NUMBER=ROCKETLAUNCHER_0001; SHIPPING=FEDEX");
	_LIT8(KEncodedCookieHeader4, "$VERSION=1; CUSTOMER=WILE_E_COYOTE; $PATH=/");
	
	_LIT8(KName1, "CUSTOMER");
	_LIT8(KValue1, "WILE_E_COYOTE");
	_LIT8(KName2, "PART_NUMBER");
	_LIT8(KValue2, "ROCKETLAUNCHER_0001");
	_LIT8(KName3, "SHIPPING");
	_LIT8(KValue3, "FEDEX");
	_LIT8(KVersion, "1");
	_LIT8(KPath, "/");

	RString name1 = iStrP.OpenStringL(KName1);
	CleanupClosePushL(name1);
	RString value1 = iStrP.OpenStringL(KValue1);
	CleanupClosePushL(value1);
	RString name2 = iStrP.OpenStringL(KName2);
	CleanupClosePushL(name2);
	RString value2 = iStrP.OpenStringL(KValue2);
	CleanupClosePushL(value2);	
	RString name3 = iStrP.OpenStringL(KName3);
	CleanupClosePushL(name3);	
	RString value3 = iStrP.OpenStringL(KValue3);
	CleanupClosePushL(value3);
	RStringF version = iStrP.OpenFStringL(KVersion);
	CleanupClosePushL(version);
	RStringF path = iStrP.OpenFStringL(KPath);
	CleanupClosePushL(path);

	RStringF cookie = iStrP.StringF(HTTP::ECookie, RHTTPSession::GetTable());
	RStringF cookieName = iStrP.StringF(HTTP::ECookieName, RHTTPSession::GetTable());
	RStringF cookieValue = iStrP.StringF(HTTP::ECookieValue, RHTTPSession::GetTable());
	RStringF cookieVersion = iStrP.StringF(HTTP::EVersion, RHTTPSession::GetTable());
	RStringF cookiePath = iStrP.StringF(HTTP::EPath, RHTTPSession::GetTable());

	TPtrC8 rawHeader;
	RHTTPHeaders hdr = GetHeadersLC();

	THTTPHdrVal hVal;
	hdr.SetFieldL(cookie, THTTPHdrVal());
	hVal.SetStr(name1);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value1);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);

	hdr.GetRawField(cookie, rawHeader);
	if (rawHeader.CompareF(KEncodedCookieHeader1) !=0)
		{
		TBuf < 1024> rawHeader16;
		rawHeader16.Copy ( rawHeader );
		
		iEngine->Utils().LogIt ( _L("\nComparison of cookie header1 failed. -> %S"), &rawHeader16);		
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();

	hdr = GetHeadersLC();
	hdr.SetFieldL(cookie, THTTPHdrVal());
	hVal.SetStr(name1);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value1);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);
	hVal.SetStr(name2);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value2);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);

	hdr.GetRawField(cookie, rawHeader);
	if (rawHeader.CompareF(KEncodedCookieHeader2) !=0)
		{
		TBuf < 1024> rawHeader16;
		rawHeader16.Copy ( rawHeader );

		iEngine->Utils().LogIt ( _L("\nComparison of cookie header2 failed. -> %S"), &rawHeader16);
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();

	hdr = GetHeadersLC();
	hdr.SetFieldL(cookie, THTTPHdrVal());
	hVal.SetStr(name1);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value1);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);
	hVal.SetStr(name2);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value2);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);
	hVal.SetStr(name3);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value3);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);

	hdr.GetRawField(cookie, rawHeader);
	if (rawHeader.CompareF(KEncodedCookieHeader3) !=0)
		{
		TBuf < 1024> rawHeader16;
		rawHeader16.Copy ( rawHeader );
		
		iEngine->Utils().LogIt ( _L("\nComparison of cookie header3 failed %S"), &rawHeader16 );
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();

	hdr = GetHeadersLC();
	hdr.SetFieldL(cookie, THTTPHdrVal());
	hVal.SetStr(name1);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value1);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);
	hVal.SetStr(name2);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value2);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);
	hVal.SetStr(name3);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value3);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);

	hdr.GetRawField(cookie, rawHeader);
	if (rawHeader.CompareF(KEncodedCookieHeader3) !=0)
		{
		TBuf < 1024> rawHeader16;
		rawHeader16.Copy ( rawHeader );
		
		iEngine->Utils().LogIt ( _L("\nComparison of cookie header4 failed %S"), &rawHeader16 );
		User::Leave(KErrArgument);
		}	
	ReleaseHeaders();

	hdr = GetHeadersLC();
	hdr.SetFieldL(cookie, THTTPHdrVal());
	hVal.SetStrF(version);
	hdr.SetParamL(cookie, cookieVersion, hVal, 0);
	hVal.SetStr(name1);
	hdr.SetParamL(cookie, cookieName, hVal, 0);
	hVal.SetStr(value1);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);
	hVal.SetStrF(path);
	hdr.SetParamL(cookie, cookiePath, hVal, 0);
	
	hdr.GetRawField(cookie, rawHeader);
	if (rawHeader.CompareF(KEncodedCookieHeader4) !=0)
		{
		TBuf < 1024> rawHeader16;
		rawHeader16.Copy ( rawHeader );
		
		iEngine->Utils().LogIt ( _L("\nComparison of cookie header5 failed  -> %S"), &rawHeader16);
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();
	
	CleanupStack::PopAndDestroy(8,&name1);
	iEngine->Utils().LogIt ( _L ("\nTestEncodeCookieL test success."));
	}

void CHttpHdrCodecTest::TestDecodeSetCookieL()
	{
	RStringF setCookie = iStrP.StringF(HTTP::ESetCookie,RHTTPSession::GetTable());

	RStringF cookieName = iStrP.StringF(HTTP::ECookieName,RHTTPSession::GetTable());
	RStringF cookieValue= iStrP.StringF(HTTP::ECookieValue,RHTTPSession::GetTable());
	RStringF cookiePath= iStrP.StringF(HTTP::EPath,RHTTPSession::GetTable());
	RStringF cookieExpires= iStrP.StringF(HTTP::EExpires,RHTTPSession::GetTable());

	THTTPHdrVal hdrVal;
	RHTTPHeaders hdr = GetHeadersLC();
	_LIT8(KEncodedCookie1, "CUSTOMER=WILE_E_COYOTE; path=/; expires=Wednesday, 09-Nov-99 23");
	_LIT8(KCookie1Name, "CUSTOMER");
	_LIT8(KCookie1Value, "WILE_E_COYOTE"); 
	_LIT8(KCookie1Path, "/"); 
	_LIT8(KCookie1Expires, "Wednesday, 09-Nov-99 23");

	RString cookie1Name = iStrP.OpenStringL(KCookie1Name);
	CleanupClosePushL(cookie1Name);
	RString cookie1Value = iStrP.OpenStringL(KCookie1Value);
	CleanupClosePushL(cookie1Value);
	RStringF cookie1Path = iStrP.OpenFStringL(KCookie1Path);
	CleanupClosePushL(cookie1Path);
	RStringF cookie1Expires = iStrP.OpenFStringL(KCookie1Expires);
	CleanupClosePushL(cookie1Expires);

	hdr.SetRawFieldL(setCookie, KEncodedCookie1, KFieldSeparator); 
	User::LeaveIfError(hdr.GetParam(setCookie, cookieName, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie1Name))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Name failed. -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.Str() == cookie1Name );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieValue, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie1Value))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Value failed. -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.Str() == cookie1Value );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieExpires, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie1Expires))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Expires failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie1Expires );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookiePath, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie1Path))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Path failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie1Path );
		User::Leave(KErrArgument);
		}
		

	CleanupStack::PopAndDestroy(4,&cookie1Name); // cookie1Expires, cookie1Path, cookie1Value, cookie1Name
	ReleaseHeaders();

	hdr = GetHeadersLC();
	_LIT8(KEncodedCookie2, "PART_NUMBER=ROCKETLAUNCHER_0001; path=/");
	_LIT8(KCookie2Name, "PART_NUMBER");
	_LIT8(KCookie2Value, "ROCKETLAUNCHER_0001");
	_LIT8(KCookie2Path, "/");	

	RString cookie2Name = iStrP.OpenStringL(KCookie2Name);
	CleanupClosePushL(cookie2Name);
	RString cookie2Value = iStrP.OpenStringL(KCookie2Value);
	CleanupClosePushL(cookie2Value);
	RStringF cookie2Path = iStrP.OpenFStringL(KCookie2Path);
	CleanupClosePushL(cookie2Path);

	hdr.SetRawFieldL(setCookie, KEncodedCookie2, KFieldSeparator); 
	User::LeaveIfError(hdr.GetParam(setCookie, cookieName, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie2Name))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie2Name failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.Str() == cookie2Name );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieValue, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie2Value))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie2Value failed  -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.Str() == cookie2Value );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookiePath, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie2Path))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie2Path failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie2Path );
		User::Leave(KErrArgument);	
		}
	

	CleanupStack::PopAndDestroy(3, &cookie2Name); // cookie2Path, cookie2Value, cookie2Name;
	ReleaseHeaders();
	
	hdr = GetHeadersLC();	
	_LIT8(KEncodedCookie3, " SHIPPING=FEDEX; path=/foo");
	_LIT8(KCookie3Name, "SHIPPING");
	_LIT8(KCookie3Value, "FEDEX");
	_LIT8(KCookie3Path, "/foo");

	RString cookie3Name = iStrP.OpenStringL(KCookie3Name);
	CleanupClosePushL(cookie3Name);
	RString cookie3Value = iStrP.OpenStringL(KCookie3Value);
	CleanupClosePushL(cookie3Value);
	RStringF cookie3Path = iStrP.OpenFStringL(KCookie3Path);
	CleanupClosePushL(cookie3Path);
	hdr.SetRawFieldL(setCookie, KEncodedCookie3, KFieldSeparator); 
	
	User::LeaveIfError(hdr.GetParam(setCookie, cookieName, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie3Name))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie3Name failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie3Name );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieValue, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie3Value))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie3Value failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie3Value );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookiePath, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie3Path))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie3Path failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie3Path );
		User::Leave(KErrArgument);
		}
		

	CleanupStack::PopAndDestroy(3,&cookie3Name); //cookie3Path, cookie3Value, cookie3Name
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeSetCookieL test success."));
	}

void CHttpHdrCodecTest::TestDecodeSetCookie2L()
	{
	RStringF setCookie = iStrP.StringF(HTTP::ESetCookie2, RHTTPSession::GetTable());

	RStringF cookieName = iStrP.StringF(HTTP::ECookieName, RHTTPSession::GetTable());
	RStringF cookieValue= iStrP.StringF(HTTP::ECookieValue, RHTTPSession::GetTable());
	RStringF cookiePath= iStrP.StringF(HTTP::EPath, RHTTPSession::GetTable());
	RStringF cookieMaxAge= iStrP.StringF(HTTP::EMaxAge, RHTTPSession::GetTable());
	RStringF cookiePort= iStrP.StringF(HTTP::ECookiePort, RHTTPSession::GetTable());
	RStringF cookieComment= iStrP.StringF(HTTP::EComment, RHTTPSession::GetTable());
	RStringF cookieCommentURL= iStrP.StringF(HTTP::ECommentURL, RHTTPSession::GetTable());
	RStringF cookieSecure= iStrP.StringF(HTTP::ESecure, RHTTPSession::GetTable());
	RStringF cookieDiscard= iStrP.StringF(HTTP::EDiscard, RHTTPSession::GetTable());

	THTTPHdrVal hdrVal;
	RHTTPHeaders hdr = GetHeadersLC();
	_LIT8(KEncodedCookie1, 
		"CUSTOMER=WILE_E_COYOTE; path=/; max-age=0; port=\"80,8000\"; comment=This is a cookie; commentURL=http://www.cookie.com; secure; discard");
	_LIT8(KCookie1Name, "CUSTOMER");
	_LIT8(KCookie1Value, "WILE_E_COYOTE"); 
	_LIT8(KCookie1Path, "/"); 
	_LIT8(KCookie1MaxAge, "0");
	_LIT8(KCookie1Port, "\"80,8000\"");
	_LIT8(KCookie1Comment, "This is a cookie");
	_LIT8(KCookie1CommentURL, "http://www.cookie.com");

	RString cookie1Name = iStrP.OpenStringL(KCookie1Name);
	CleanupClosePushL(cookie1Name);
	RString cookie1Value = iStrP.OpenStringL(KCookie1Value);
	CleanupClosePushL(cookie1Value);
	RStringF cookie1Path = iStrP.OpenFStringL(KCookie1Path);
	CleanupClosePushL(cookie1Path);
	RStringF cookie1MaxAge = iStrP.OpenFStringL(KCookie1MaxAge);
	CleanupClosePushL(cookie1MaxAge);
	RStringF cookie1Port = iStrP.OpenFStringL(KCookie1Port);
	CleanupClosePushL(cookie1Port);
	RStringF cookie1Comment = iStrP.OpenFStringL(KCookie1Comment);
	CleanupClosePushL(cookie1Comment);
	RStringF cookie1CommentURL = iStrP.OpenFStringL(KCookie1CommentURL);
	CleanupClosePushL(cookie1CommentURL);

	hdr.SetRawFieldL(setCookie, KEncodedCookie1, KFieldSeparator); 
	User::LeaveIfError(hdr.GetParam(setCookie, cookieName, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie1Name))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Name & hdr type failed  -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie1Name );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieValue, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie1Value))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Value & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie1Value );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieMaxAge, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie1MaxAge))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1MaxAge & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie1MaxAge );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookiePort, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie1Port))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Port & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie1Port );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookiePath, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie1Path))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Path & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie1Path );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieComment, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie1Comment))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Comment & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie1Comment );
		User::Leave(KErrArgument);		
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieCommentURL, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie1CommentURL))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1CommentURL & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie1CommentURL );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieSecure, hdrVal));
	User::LeaveIfError(hdr.GetParam(setCookie, cookieDiscard, hdrVal));

	CleanupStack::PopAndDestroy(7,&cookie1Name); // cookie1CommentURL, cookie1Comment, cookie1Port, cookie1MaxAge, cookie1Path, cookie1Value, cookie1Name
	ReleaseHeaders();

	hdr = GetHeadersLC();
	_LIT8(KEncodedCookie2, "PART_NUMBER=ROCKETLAUNCHER_0001; path=/");
	_LIT8(KCookie2Name, "PART_NUMBER");
	_LIT8(KCookie2Value, "ROCKETLAUNCHER_0001");
	_LIT8(KCookie2Path, "/");	

	RString cookie2Name = iStrP.OpenStringL(KCookie2Name);
	CleanupClosePushL(cookie2Name);
	RString cookie2Value = iStrP.OpenStringL(KCookie2Value);
	CleanupClosePushL(cookie2Value);
	RStringF cookie2Path = iStrP.OpenFStringL(KCookie2Path);
	CleanupClosePushL(cookie2Path);

	hdr.SetRawFieldL(setCookie, KEncodedCookie2, KFieldSeparator); 
	User::LeaveIfError(hdr.GetParam(setCookie, cookieName, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie2Name))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie2Name & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie2Name );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieValue, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie2Value))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie2Value & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie2Value );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookiePath, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie2Path))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie2Path & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie2Path );
		User::Leave(KErrArgument);
		}
		

	CleanupStack::PopAndDestroy(3, &cookie2Name); // cookie2Path, cookie2Value, cookie2Name;
	ReleaseHeaders();
	
	hdr = GetHeadersLC();	
	_LIT8(KEncodedCookie3, " SHIPPING=FEDEX; path=/foo");
	_LIT8(KCookie3Name, "SHIPPING");
	_LIT8(KCookie3Value, "FEDEX");
	_LIT8(KCookie3Path, "/foo");

	RString cookie3Name = iStrP.OpenStringL(KCookie3Name);
	CleanupClosePushL(cookie3Name);
	RString cookie3Value = iStrP.OpenStringL(KCookie3Value);
	CleanupClosePushL(cookie3Value);
	RStringF cookie3Path = iStrP.OpenFStringL(KCookie3Path);
	CleanupClosePushL(cookie3Path);
	hdr.SetRawFieldL(setCookie, KEncodedCookie3, KFieldSeparator); 
	
	User::LeaveIfError(hdr.GetParam(setCookie, cookieName, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie3Name))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie3Name & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie3Name );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieValue, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie3Value))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie3Value & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrVal, hdrVal.Str() == cookie3Value );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookiePath, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookie3Path))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie3Path & hdr type failed -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookie3Path );
		User::Leave(KErrArgument);
		}
		

	CleanupStack::PopAndDestroy(3,&cookie3Name); //cookie3Path, cookie3Value, cookie3Name
	ReleaseHeaders();
	
	iEngine->Utils().LogIt ( _L ("\nTestDecodeSetCookie2L test success."));
	}

void CHttpHdrCodecTest::TestDecodeSetCookie3L()
	{
	// Directly testing a defect fix INC036055. Parsing a string with three quotes.
	// Testing ExtractNextTokenFromList which can be used for extracting cookies.

	_LIT8(KSeperator, "\\n");
	_LIT8(KCookieBuf, "quote1\"quote2\"quote3\"");

	TBuf8<0x100> buf(KCookieBuf());
	TPtrC8 ptr = buf.Ptr();
	TPtrC8 token;

	User::LeaveIfError(InetProtTextUtils::ExtractNextTokenFromList(ptr, token, KSeperator));
	iEngine->Utils().LogIt ( _L ("\nTestDecodeSetCookie3L test success."));
	}

// INC070131
void CHttpHdrCodecTest::TestDecodeSetCookie4L()
	{
	RStringF setCookie = iStrP.StringF(HTTP::ESetCookie,RHTTPSession::GetTable());

	RStringF cookieName = iStrP.StringF(HTTP::ECookieName,RHTTPSession::GetTable());
	RStringF cookieValue= iStrP.StringF(HTTP::ECookieValue,RHTTPSession::GetTable());
	RStringF cookiePath= iStrP.StringF(HTTP::EPath,RHTTPSession::GetTable());
	RStringF cookieExpires= iStrP.StringF(HTTP::EExpires,RHTTPSession::GetTable());
	RStringF cookieDomain= iStrP.StringF(HTTP::EDomain,RHTTPSession::GetTable());

	THTTPHdrVal hdrVal;
	RHTTPHeaders hdr = GetHeadersLC();
	_LIT8(KEncodedCookie1, "id=33263187046453257 bb=141A11twQw_\"4totrK4ow| adv=; Domain=.bluestreak.com; expires=Friday 02-Oct-2015 05:50:28 GMT; path=/;");
	_LIT8(KCookie1Name, "id");
	_LIT8(KCookie1Value, "33263187046453257 bb=141A11twQw_\"4totrK4ow| adv="); 
	_LIT8(KCookieDomain, ".bluestreak.com");
	_LIT8(KCookieExpires, "Friday 02-Oct-2015 05:50:28 GMT");
	_LIT8(KCookiePath, "/"); 
	
	RString cookie1Name = iStrP.OpenStringL(KCookie1Name);
	CleanupClosePushL(cookie1Name);
	RString cookie1Value = iStrP.OpenStringL(KCookie1Value);
	CleanupClosePushL(cookie1Value);
	RStringF cookieExpiresVal = iStrP.OpenFStringL(KCookieExpires);
	CleanupClosePushL(cookieExpiresVal);
	RStringF cookieDomainVal = iStrP.OpenFStringL(KCookieDomain);
	CleanupClosePushL(cookieDomainVal);
	RStringF cookiePathVal = iStrP.OpenFStringL(KCookiePath);
	CleanupClosePushL(cookiePathVal);


	hdr.SetRawFieldL(setCookie, KEncodedCookie1, KFieldSeparator); 
	User::LeaveIfError(hdr.GetParam(setCookie, cookieName, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie1Name))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Name failed. -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.Str() == cookie1Name );
		User::Leave(KErrArgument);
		}
		
	User::LeaveIfError(hdr.GetParam(setCookie, cookieValue, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrVal && hdrVal.Str() == cookie1Value))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookie1Value failed. -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.Str() == cookie1Value );
		User::Leave(KErrArgument);
		}

	User::LeaveIfError(hdr.GetParam(setCookie, cookieExpires, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookieExpiresVal))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookieExpires failed. -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookieExpiresVal );
		User::Leave(KErrArgument);
		}

	User::LeaveIfError(hdr.GetParam(setCookie, cookieDomain, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookieDomainVal))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookieExpires failed. -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookieDomainVal );
		User::Leave(KErrArgument);
		}

	User::LeaveIfError(hdr.GetParam(setCookie, cookiePath, hdrVal));
	if (!(hdrVal.Type() == THTTPHdrVal::KStrFVal && hdrVal.StrF() == cookiePathVal))
		{
		iEngine->Utils().LogIt ( _L("\nComparison of cookieExpires failed. -> %d,%d"), hdrVal.Type() == THTTPHdrVal::KStrFVal, hdrVal.StrF() == cookiePathVal );
		User::Leave(KErrArgument);
		}

	CleanupStack::PopAndDestroy( 5, &cookie1Name );
	ReleaseHeaders();	
	iEngine->Utils().LogIt ( _L ("\nTestDecodeSetCookie4L test success."));
	}

//test for DEF080697: Cookies: When sending a request to an origin server, Browser is not sending the	
void CHttpHdrCodecTest::TestEncodeCookiePortL()
	{
	RHTTPHeaders hdr = GetHeadersLC();
	_LIT8(KEncodedCookie5, "$Version=112; CUSTOMER=WILE_E_COYOTE; $Path=/; $Port=\"80,8000\"");
	_LIT8(KName1, "CUSTOMER");
	_LIT8(KValue1, "WILE_E_COYOTE");
	_LIT8(KCookiePort, "80,8000");
	_LIT8(KVersion, "112");
	_LIT8(KPath, "/");

	RString name1 = iStrP.OpenStringL(KName1);
	CleanupClosePushL(name1);
	RString value1 = iStrP.OpenStringL(KValue1);
	CleanupClosePushL(value1);
	RStringF version = iStrP.OpenFStringL(KVersion);
	CleanupClosePushL(version);
	RStringF cookiePort1 = iStrP.OpenFStringL(KCookiePort);
	CleanupClosePushL(cookiePort1);
	RStringF path = iStrP.OpenFStringL(KPath);
	CleanupClosePushL(path);


	RStringF cookie = iStrP.StringF(HTTP::ECookie, RHTTPSession::GetTable());
	RStringF cookieName = iStrP.StringF(HTTP::ECookieName, RHTTPSession::GetTable());
	RStringF cookieValue = iStrP.StringF(HTTP::ECookieValue, RHTTPSession::GetTable());
	RStringF cookieVersion = iStrP.StringF(HTTP::EVersion, RHTTPSession::GetTable());
	RStringF cookiePort = iStrP.StringF(HTTP::ECookiePort, RHTTPSession::GetTable());
	RStringF cookiePath = iStrP.StringF(HTTP::EPath, RHTTPSession::GetTable());


	THTTPHdrVal hVal;
	hdr.SetFieldL(cookie, THTTPHdrVal());
	
	hVal.SetStrF(version);
	hdr.SetParamL(cookie, cookieVersion, hVal, 0);
	
	hVal.SetStr(name1);
	hdr.SetParamL(cookie, cookieName, hVal, 0);

	hVal.SetStr(value1);
	hdr.SetParamL(cookie, cookieValue, hVal, 0);
	
	hVal.SetStrF(path);
	hdr.SetParamL(cookie, cookiePath, hVal, 0);

	hVal.SetStrF(cookiePort1);
	hdr.SetParamL(cookie, cookiePort, hVal, 0);
	
	TPtrC8 ota;
	hdr.GetRawField(cookie, ota);
	
	if(ota.Compare(KEncodedCookie5) != 0)
	{
	TBuf < 1024> rawHeader16;
	rawHeader16.Copy ( ota );

	iEngine->Utils().LogIt ( _L("\nComparison of Encode Cookie Port failed. -> %S"), &rawHeader16 );
	User::Leave(KErrArgument);
	}

	CleanupStack::PopAndDestroy(5, &name1);
	ReleaseHeaders();
	}
	
void CHttpHdrCodecTest::TestEncodeCacheControlL()
	{
	_LIT8(KEncodedCacheControlHeader, "public, no-cache=\"wibble,wobble\"");
	_LIT8(KPublic, "public");
	_LIT8(KNoCache, "no-cache=\"wibble,wobble\"");
	RStringF publicStr = iStrP.OpenFStringL(KPublic);
	CleanupClosePushL(publicStr);
	RStringF noCacheStr = iStrP.OpenFStringL(KNoCache);
	CleanupClosePushL(noCacheStr);
	
	TPtrC8 rawHeader;
	RHTTPHeaders hdr = GetHeadersLC();
	RStringF cc = iStrP.StringF(HTTP::ECacheControl,RHTTPSession::GetTable());
	hdr.SetFieldL(cc, THTTPHdrVal(publicStr));
	hdr.SetFieldL(cc, THTTPHdrVal(noCacheStr));
	hdr.GetRawField(cc, rawHeader);
	if(rawHeader.Compare(KEncodedCacheControlHeader) != 0)
		{
		TBuf < 1024> rawHeader16;
		rawHeader16.Copy ( rawHeader );
	
		iEngine->Utils().LogIt ( _L("\nComparison of Cache Control header failed. -> %S"), &rawHeader16 );
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();

	CleanupStack::PopAndDestroy(2,&publicStr);
	
	iEngine->Utils().LogIt ( _L ("\nTestEncodeCacheControlL test success."));
	}	


void CHttpHdrCodecTest::TestEncodeDateL()
	{
 	// the result we expect and that we will validate
//	_LIT8(KDateVal1, "Sun, 06 Nov 2001 08:49:37 -0700"); // ota date header, rfc1123 format


//	ReleaseHeaders();
	}


void CHttpHdrCodecTest::TestEncodeLocaleIndependentDateL()
	{
	
	iEngine->Console().Printf(_L("\nTest encoding 'Custom Date' header for Locale independence"));
	RHTTPHeaders hdr = GetHeadersLC();

	_LIT8(KRefDate,			"Wed, 08 Jun 1977 12:00:00 GMT");
	
	TLocale locale;

	TDateTime refDate(1977, EJune, 7, 12, 00, 00, 0); // note offset of the date value by 1
	THTTPHdrVal refDateHdrVal(refDate);
	THTTPHdrVal returnedDateHdrVal;
	TPtrC8 rawDateJP;
	TPtrC8 rawDateUSA;
	TPtrC8 rawDateEU;
	
	//Sets the capability to change the date format
	RProcess process;
	TSecurityInfo info(process);
 	info.iCaps.AddCapability(ECapabilityWriteDeviceData);

	//Japanese Locale Settings	
	locale.SetDateFormat(EDateJapanese);
	locale.Set();		

	RStringF japDateRStr = iStrP.OpenFStringL(_L8("JapanLocaleDate"));
	CleanupClosePushL(japDateRStr);
	hdr.SetFieldL(japDateRStr,refDateHdrVal);
	
	//Check if it encodes in the correct format
	hdr.GetRawField(japDateRStr, rawDateJP);
	if (rawDateJP.Compare(KRefDate()))
		User::Leave(KErrCorrupt);
	else
		iEngine->Console().Printf(_L("Japanese, "));
	CleanupStack::PopAndDestroy(&japDateRStr);	


	//American Locale Settings
	locale.SetDateFormat(EDateAmerican);
	locale.Set();		

	RStringF usaDateRStr= iStrP.OpenFStringL(_L8("USALocaleDate"));
	CleanupClosePushL(usaDateRStr);
	hdr.SetFieldL(usaDateRStr,refDateHdrVal);
	
	
	hdr.GetRawField(usaDateRStr, rawDateUSA);
	if (rawDateUSA.Compare(KRefDate()))
		User::Leave(KErrCorrupt);
	else
		iEngine->Console().Printf(_L("\nUSA, "));
	CleanupStack::PopAndDestroy(&usaDateRStr);	


	//European Locale Settings
	locale.SetDateFormat(EDateEuropean);
	locale.Set();		


	RStringF euDateRStr= iStrP.OpenFStringL(_L8("EULocaleDate"));
	CleanupClosePushL(euDateRStr);
	hdr.SetFieldL(euDateRStr,refDateHdrVal);

	hdr.GetRawField(euDateRStr, rawDateEU);
	if (rawDateEU.Compare(KRefDate()))
		User::Leave(KErrCorrupt);
	else
		iEngine->Console().Printf(_L("\nEU formats independent\t"));
	CleanupStack::PopAndDestroy(&euDateRStr);	


	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestEncodeLocaleIndependentDateL test success"));
	}




void CHttpHdrCodecTest::TestDecodeConnectionL()
	{
	_LIT8(KConnectionVal1, "close, value1, value2,, value3  ,  ,lastvalue"); // ota connection header

	// Open strings used in this test
	RStringF connectionStr = iStrP.StringF(HTTP::EConnection,RHTTPSession::GetTable());
	RStringF pt1Str = iStrP.StringF(HTTP::EClose,RHTTPSession::GetTable());

	RStringF pt2Str = iStrP.OpenFStringL(_L8("value1"));
	CleanupClosePushL(pt2Str);
	RStringF pt3Str = iStrP.OpenFStringL(_L8("value2"));
	CleanupClosePushL(pt3Str);
	RStringF pt4Str = iStrP.OpenFStringL(_L8(""));
	CleanupClosePushL(pt4Str);
	RStringF pt5Str = iStrP.OpenFStringL(_L8("value3"));
	CleanupClosePushL(pt5Str);
	RStringF pt6Str = iStrP.OpenFStringL(_L8(""));
	CleanupClosePushL(pt6Str);
	RStringF pt7Str = iStrP.OpenFStringL(_L8("lastvalue"));
	CleanupClosePushL(pt7Str);

	iEngine->Console().Printf(_L("\nTest decoding 'Connection' header"));
	RHTTPHeaders hdr = GetHeadersLC();

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	hdr.SetRawFieldL(connectionStr, KConnectionVal1(), KFieldSeparator);

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr.GetField(connectionStr,0,hVal));

	iEngine->Console().Printf(_L("\ncheck for correct number of parts"));
	if(hdr.FieldPartsL(connectionStr) != 7)
		{		
		iEngine->Utils().LogIt ( _L("\nComparison of Connection header failed.") );
		User::Leave(KErrArgument);
		}
		

	iEngine->Console().Printf(_L("\ndecode and check each header part"));
	THTTPHdrVal vPt1;  
	User::LeaveIfError(hdr.GetField(connectionStr, 0, vPt1));
	THTTPHdrVal vPt2; 
	User::LeaveIfError(hdr.GetField(connectionStr, 1, vPt2));
	
	THTTPHdrVal vPt3; 
	User::LeaveIfError(hdr.GetField(connectionStr, 2, vPt3));
	
	THTTPHdrVal vPt4; 
	User::LeaveIfError(hdr.GetField(connectionStr, 3, vPt4));
	
	THTTPHdrVal vPt5; 
	User::LeaveIfError(hdr.GetField(connectionStr, 4, vPt5));
	
	THTTPHdrVal vPt6; 
	User::LeaveIfError(hdr.GetField(connectionStr, 5, vPt6));
	
	THTTPHdrVal vPt7; User::LeaveIfError(hdr.GetField(connectionStr, 6, vPt7));
	
	if( (vPt1.StrF() != pt1Str) || (vPt2.StrF() != pt2Str) || (vPt3.StrF() != pt3Str)
		|| (vPt4.StrF() != pt4Str) || (vPt5.StrF() != pt5Str) || (vPt6.StrF() != pt6Str)
		|| (vPt7.StrF() != pt7Str))
		{
		iEngine->Utils().LogIt ( _L("\nHeader part checking failed. ") );
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();
	CleanupStack::PopAndDestroy(6,&pt2Str);
	iEngine->Utils().LogIt ( _L ("\nTestDecodeConnectionL test success."));
	}

void CHttpHdrCodecTest::TestDecodeContentLengthL()
	{
	_LIT8(KContentLengthVal1, "123456"); // ota content length header
	_LIT8(KContentLengthVal2, "123456.5");
	_LIT8(KContentLengthVal3, "0.5");

	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Content-Length' header"));

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	RStringF contLenStr = iStrP.OpenFStringL(_L8("Content-Length"));
	hdr.SetRawFieldL(contLenStr, KContentLengthVal1(), KFieldSeparator);

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr.GetField(contLenStr,0,hVal));

	iEngine->Console().Printf(_L("\ndecode header and check"));
	THTTPHdrVal hdrVal;
	User::LeaveIfError(hdr.GetField(contLenStr,0,hdrVal));
	if (hdr.FieldPartsL(contLenStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nHeader contains no Content-Length header.") );
		User::Leave(KErrArgument);
		}
		 

	THTTPHdrVal vPt1;
	User::LeaveIfError(hdr.GetField(contLenStr, 0,vPt1));
	if (vPt1.Int() != 123456)
		{
		iEngine->Utils().LogIt ( _L("\nContent length value is not 123456 -> %d"), vPt1.Int() );
		User::Leave(KErrArgument);
		}
	
	hdr.RemoveAllFields();
	hdr.SetRawFieldL(contLenStr, KContentLengthVal2(), KFieldSeparator);
	
	iEngine->Console().Printf(_L("\ncheck for header presence"));
	User::LeaveIfError(hdr.GetField(contLenStr,0,hVal));
	
	iEngine->Console().Printf(_L("\ndecode header and check"));
	User::LeaveIfError(hdr.GetField(contLenStr,0,hdrVal));
	if (hdr.FieldPartsL(contLenStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nHeader contains no Content-Length header.") );
		User::Leave(KErrArgument);
		}
			 
	User::LeaveIfError(hdr.GetField(contLenStr, 0,vPt1));
	if (vPt1.Int() != 123456)
		{
		iEngine->Utils().LogIt ( _L("\nContent length value is not 123456 -> %d"), vPt1.Int() );
		User::Leave(KErrArgument);
		}
	
	
	hdr.RemoveAllFields();
	hdr.SetRawFieldL(contLenStr, KContentLengthVal3(), KFieldSeparator);
	
	iEngine->Console().Printf(_L("\ncheck for header presence"));
	User::LeaveIfError(hdr.GetField(contLenStr,0,hVal));
	
	iEngine->Console().Printf(_L("\ndecode header and check"));
	User::LeaveIfError(hdr.GetField(contLenStr,0,hdrVal));
	if (hdr.FieldPartsL(contLenStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nHeader contains no Content-Length header.") );
		User::Leave(KErrArgument);
		}
		 
	User::LeaveIfError(hdr.GetField(contLenStr, 0,vPt1));
	if (vPt1.Int() != 0)
		{
		iEngine->Utils().LogIt ( _L("\nContent length value is not 0 -> %d"), vPt1.Int() );
		User::Leave(KErrArgument);
		}

	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeContentLengthL test success."));
	}

void CHttpHdrCodecTest::TestDecodeContentTypeL()
	{
	_LIT8(KContentTypeVal1, "text/html; charset=\"iso-8859-1\""); // ota content type header

	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Content-Type' header"));
	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	RStringF contTypeStr = iStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable());
	hdr.SetRawFieldL(contTypeStr, KContentTypeVal1(), KFieldSeparator);

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr.GetField(contTypeStr,0,hVal));

	_LIT8(KMediaType, "text/html");
	_LIT8(KCharsetQuoted, "iso-8859-1");
	iEngine->Console().Printf(_L("\ndecode header and check"));
	THTTPHdrVal hdrVal;
	User::LeaveIfError(hdr.GetField(contTypeStr,0,hdrVal));
	
	if(hdr.FieldPartsL(contTypeStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nHeader contains no Content-Type header.") );
		User::Leave(KErrArgument);
		}
		

	if (!CompareStringL(hdrVal.StrF(), KMediaType()))
		{
		iEngine->Utils().LogIt ( _L("\nHeader not contains media type ( text/html) value.") );
		User::Leave(KErrArgument);
		}
		

	RStringF charsetStr = iStrP.StringF(HTTP::ECharset,RHTTPSession::GetTable());
 	THTTPHdrVal paramVal;

	User::LeaveIfError(hdr.GetParam(contTypeStr, charsetStr, paramVal));
	if (!CompareStringL(paramVal.StrF(), KCharsetQuoted()))
		{
		iEngine->Utils().LogIt ( _L("Header not contains the charset value") );
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();
	
	iEngine->Utils().LogIt ( _L ("\nTestDecodeContentTypeL test success."));
	}

void CHttpHdrCodecTest::TestDecodeContentTypeWithMultipleParametersL()
	{
	// ota content type header
	_LIT8(KContentTypeVal1, "image/jpeg; fwd=\"no\"; ringtone=\"yes\""); 

	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Content-Type with multiple params' header"));
	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	RStringF contTypeStr = iStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable());
	hdr.SetRawFieldL(contTypeStr, KContentTypeVal1(), KFieldSeparator);

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr.GetField(contTypeStr,0,hVal));

	_LIT8(KMediaType, "image/jpeg");
	iEngine->Console().Printf(_L("\ndecode header and check"));
	THTTPHdrVal hdrVal;
	User::LeaveIfError(hdr.GetField(contTypeStr,0,hdrVal));
	
	if(hdr.FieldPartsL(contTypeStr) != 1)
		User::Leave(KErrArgument);

	if (!CompareStringL(hdrVal.StrF(), KMediaType()))
		User::Leave(KErrArgument);

	// Check 1st parameter
	_LIT8(KFwdValue, "no");
	_LIT8( KFwdString, "fwd"); 
	RStringF fwdString = iStrP.OpenFStringL(KFwdString);

 	THTTPHdrVal paramVal;
	User::LeaveIfError(hdr.GetParam(contTypeStr, fwdString, paramVal));
	if (!CompareStringL(paramVal.StrF(), KFwdValue()))
		User::Leave(KErrArgument);
	fwdString.Close();

	// Check 2nd parameter
	_LIT8(KRingtoneValue, "yes");
	_LIT8( KRingtoneString, "ringtone"); 
	RStringF ringtoneString = iStrP.OpenFStringL(KRingtoneString);

 	THTTPHdrVal paramVal2;
	User::LeaveIfError(hdr.GetParam(contTypeStr, ringtoneString, paramVal2));
	if (!CompareStringL(paramVal2.StrF(), KRingtoneValue()))
		User::Leave(KErrArgument);
	ringtoneString.Close();

	ReleaseHeaders();
	}

// Test behaviour of ContentType decoding is when a parameter value is missing.
void CHttpHdrCodecTest::TestDecodeContentTypeMissingParameterL()
	{
	_LIT8(KContentTypeVal1, "text/html; charset="); // ota content type header with a parameter value missing

	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Content-Type' header with a parameter missing"));
	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	RStringF contTypeStr = iStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable());
	hdr.SetRawFieldL(contTypeStr, KContentTypeVal1(), KFieldSeparator);

	iEngine->Console().Printf(_L("\ncheck for header presence"));
	THTTPHdrVal hVal;
	User::LeaveIfError(hdr.GetField(contTypeStr,0,hVal));

	_LIT8(KMediaType, "text/html");
	_LIT8(KCharsetQuoted, "");
	iEngine->Console().Printf(_L("\ndecode header and check"));
	THTTPHdrVal hdrVal;
	User::LeaveIfError(hdr.GetField(contTypeStr,0,hdrVal));
	
	if(hdr.FieldPartsL(contTypeStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nHeader contains no Content-Type header ( when a parameter value is missing )."));
		User::Leave(KErrArgument);
		}
		

	if (!CompareStringL(hdrVal.StrF(), KMediaType()))
		{
		iEngine->Utils().LogIt ( _L("\nHeader contains no media type ( when a parameter value is missing ) ") );
		User::Leave(KErrArgument);
		}
		

	RStringF charsetStr = iStrP.StringF(HTTP::ECharset,RHTTPSession::GetTable());
 	THTTPHdrVal paramVal;

	User::LeaveIfError(hdr.GetParam(contTypeStr, charsetStr, paramVal));
	if (!CompareStringL(paramVal.StrF(), KCharsetQuoted()))
		{
		iEngine->Utils().LogIt ( _L("\nHeader contains no charset value ( when a parameter value is missing ) ") );
		User::Leave(KErrArgument);
		}
		

	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeContentTypeMissingParameterL test success."));
	}

void CHttpHdrCodecTest::TestDecodeDateL()
	{
	_LIT8(KDateVal1, "Sun, 06 Nov 2001 08:49:37 GMT"); // ota date header, rfc1123 format
	_LIT8(KDateVal2, "Sunday, 06-Nov-01 08:49:37 GMT"); // ota date header, rfc850 format
	_LIT8(KDateVal3, "Sun Nov  6 08:49:37 2001"); // ota date header, ansi asctime format


	_LIT8(KDateVal4, "Mon, 07 Nov 2001 08:49:37 +1100"); // rfc1123 format with offset
	_LIT8(KDateVal5, "Tue, 08 Nov 2001 08:49:37 PST"); // rfc1123 format timezone
	_LIT8(KDateVal6, "Wed, 09 Nov 2001 08:49:37 ABC"); // rfc1123 format bogus timezone
	_LIT8(KDateVal7, "Thu, 10 Nov 2001 08:49:37"); // rfc1123 format - no timezone
	_LIT8(KDateVal8, "Fri, 10 Nov 2001 08:49:37 GMT"); // ota date header, rfc1123 format
	_LIT8(KDateVal9, "Sunday, 06-Nov-01 08:49:37 -0200"); // rfc850 format with offset

	RHTTPHeaders hdr = GetHeadersLC();
	THTTPHdrVal dummy;
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Date' header"));
	RStringF dateStr = iStrP.StringF(HTTP::EDate,RHTTPSession::GetTable());
	TDateTime refDate(2001, ENovember, 5, 8, 49, 37, 0); // note offset of the date value by 1
	//Changed the Values due to Fix in InetProtUtils for Defect PDEF126657.
	TDateTime refDate4(2001, ENovember, 5, 21, 49, 37, 0); // note offset of the date value by 1
	TDateTime refDate5(2001, ENovember, 7, 16, 49, 37, 0); // note offset of the date value by 1
	TDateTime refDate9(2001, ENovember, 5, 10, 49, 37, 0); // note offset of the date value by 1
	//
	iEngine->Console().Printf(_L("\nDate format 1: add OTA data to header collection"));
	hdr.SetRawFieldL(dateStr, KDateVal1(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 1: check for header presence"));
	User::LeaveIfError(hdr.GetField(dateStr,0,dummy));
		

	iEngine->Console().Printf(_L("\nDate format 1: decode header and check"));
	if (hdr.FieldPartsL(dateStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 1: checking failed.") );		
		User::Leave(KErrArgument);
		}
		

	THTTPHdrVal datePt1;
	User::LeaveIfError(hdr.GetField(dateStr, 0,datePt1));
	if (!CompareDate(datePt1.DateTime(), refDate))
		{
		iEngine->Utils().LogIt ( _L("\nDate format 1 comparison failed.") );	
		User::Leave(KErrArgument);
		}
		
	//
	iEngine->Console().Printf(_L("\nDate format 2: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal2(), KFieldSeparator);
	iEngine->Console().Printf(_L("\nDate format 2: check for header presence"));
	
	User::LeaveIfError(hdr.GetField(dateStr,0,dummy));
	
	iEngine->Console().Printf(_L("\nDate format 2: decode header and check"));
	if (hdr.FieldPartsL(dateStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 2: checking failed" ));		
		User::Leave(KErrArgument);
		}
		

	THTTPHdrVal datePt2;
	User::LeaveIfError(hdr.GetField(dateStr, 0, datePt2));
	if (!CompareDate(datePt2.DateTime(), refDate))
		{
		iEngine->Utils().LogIt ( _L("\nDate format 2 comparison failed.") );	
		User::Leave(KErrArgument);
		}
		
	//
	iEngine->Console().Printf(_L("\nDate format 3: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal3(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 3: check for header presence"));
	iEngine->Console().Printf(_L("\nDate format 3: decode header and check"));
	THTTPHdrVal hdrVal;
	User::LeaveIfError(hdr.GetField(dateStr,0,hdrVal));
	if (hdr.FieldPartsL(dateStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 3: checking failed" ) );	
		User::Leave(KErrArgument);
		}
		

	THTTPHdrVal datePt3;
	User::LeaveIfError(hdr.GetField(dateStr, 0,datePt3));
	if (!CompareDate(datePt3.DateTime(), refDate))
		{
		iEngine->Utils().LogIt ( _L("\nDate format 3 comparison failed") );	
		User::Leave(KErrArgument);
		}
		
	
	//
	iEngine->Console().Printf(_L("\nDate format 4: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal4(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 4: check for header presence"));
	User::LeaveIfError(hdr.GetField(dateStr,0,dummy));

	iEngine->Console().Printf(_L("\nDate format 4: decode header and check"));
	if (hdr.FieldPartsL(dateStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 4: checking failed"));	
		User::Leave(KErrArgument);		
		}
		

	THTTPHdrVal datePt4;
	User::LeaveIfError(hdr.GetField(dateStr, 0,datePt4));
	if (!CompareDate(datePt4.DateTime(), refDate4))
		{
		iEngine->Utils().LogIt ( _L("\nDate format 4 comparison failed") );	
		User::Leave(KErrArgument);
		}
		

	//
	iEngine->Console().Printf(_L("\nDate format 5: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal5(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 5: check for header presence"));
	User::LeaveIfError(hdr.GetField(dateStr,0,dummy));

	iEngine->Console().Printf(_L("\nDate format 5: decode header and check"));
	if (hdr.FieldPartsL(dateStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 5: checking failed "));	
		User::Leave(KErrArgument);
		}
		

	THTTPHdrVal datePt5;
	User::LeaveIfError(hdr.GetField(dateStr, 0,datePt5));
	if (!CompareDate(datePt5.DateTime(), refDate5))
		{
		iEngine->Utils().LogIt ( _L("\nDate format 5 comparison failed") );	
		User::Leave(KErrArgument);
		}
		

	//
	iEngine->Console().Printf(_L("\nDate format 6: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal6(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 6: check for header presence"));
	TInt err = hdr.GetField(dateStr,0,dummy);
	if(err !=KErrCorrupt)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 6: not a corrupt header.") );	
		User::Leave(KErrArgument);
		}
		

	//
	iEngine->Console().Printf(_L("\nDate format 7: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal7(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 7: check for header presence"));
	err = hdr.GetField(dateStr,0,dummy);
	if(err !=KErrCorrupt)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 7: not a corrupt header") );	
		User::Leave(KErrArgument);
		}
		

	//
	iEngine->Console().Printf(_L("\nDate format 8: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal8(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 8: check for header presence"));
	User::LeaveIfError(hdr.GetField(dateStr,0,dummy));

	iEngine->Console().Printf(_L("\nDate format 8: decode header and check"));
	if (hdr.FieldPartsL(dateStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 8: checking failed"));	
		User::Leave(KErrArgument);
		}
		
	//
	iEngine->Console().Printf(_L("\nDate format 9: add OTA data to header collection"));
	hdr.RemoveField(dateStr);
	hdr.SetRawFieldL(dateStr, KDateVal9(), KFieldSeparator);

	iEngine->Console().Printf(_L("\nDate format 9: check for header presence"));
	User::LeaveIfError(hdr.GetField(dateStr,0,dummy));

	iEngine->Console().Printf(_L("\nDate format 9: decode header and check"));
	if (hdr.FieldPartsL(dateStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("\nDate format 9: checking failed "));	
		User::Leave(KErrArgument);
		}
		

	THTTPHdrVal datePt9;
	User::LeaveIfError(hdr.GetField(dateStr, 0,datePt9));
	if (!CompareDate(datePt9.DateTime(), refDate9))
		{
		iEngine->Utils().LogIt ( _L("\nDate format 8 comparison failed.") );	
		User::Leave(KErrArgument);
		}
		
	
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeDateL test success."));
	}

void CHttpHdrCodecTest::TestDecodeServerL()
	{
	_LIT8(KServerVal1, "CERN/3.0 libwww/2.17"); // ota server header

	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Server' header"));
	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	RStringF serverStr = iStrP.StringF(HTTP::EServer,RHTTPSession::GetTable());
	hdr.SetRawFieldL(serverStr, KServerVal1(), KFieldSeparator);

	iEngine->Console().Printf(_L("\ndecode header and check"));
	THTTPHdrVal hdrVal;
	User::LeaveIfError(hdr.GetField(serverStr,0,hdrVal));
	if (hdr.FieldPartsL(serverStr) != 1)
		{
		iEngine->Utils().LogIt ( _L("Server header not present.") );	
		User::Leave(KErrArgument);
		}
		

	THTTPHdrVal servPt;
	User::LeaveIfError(hdr.GetField(serverStr, 0,servPt));
	if(!CompareStringL(servPt.StrF(), KServerVal1()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of server header value failed.") );	
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeServerL test success."));
	}

void CHttpHdrCodecTest::TestDecodeTransferEncodingL()
	{
	_LIT8(KTransferEncodingVal1, "chunked, extension1;param1=value1, extension2"); // ota transfer-encoding header
	_LIT8(KTransferEncodingVal2, "identity, extension3;param3=value3, extension4"); // ota transfer-encoding header

	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Transfer-Encoding' header"));

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	RStringF xferEncStr = iStrP.StringF(HTTP::ETransferEncoding,RHTTPSession::GetTable());
	hdr.SetRawFieldL(xferEncStr, KTransferEncodingVal1(), KFieldSeparator);
	hdr.SetRawFieldL(xferEncStr, KTransferEncodingVal2(), KFieldSeparator);

	_LIT8(KPart1Val, "chunked"); 
	_LIT8(KPart2Val, "extension1"); 
	_LIT8(KPart3Val, "extension2"); 
	_LIT8(KPart4Val, "identity"); 
	_LIT8(KPart5Val, "extension3"); 
	_LIT8(KPart6Val, "extension4"); 
	_LIT8(KParam1Val, "value1"); 
	_LIT8(KParam3Val, "value3");
	iEngine->Console().Printf(_L("\ndecode header and check"));
	if (hdr.FieldPartsL(xferEncStr) != 6)
		{
		iEngine->Utils().LogIt ( _L("Transfer-Encoding check failed.") );	
		User::Leave(KErrArgument);
		}
		

	THTTPHdrVal pt1Val;
	User::LeaveIfError(hdr.GetField(xferEncStr, 0,pt1Val));
	if (!CompareStringL(pt1Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("Compairson of part1 value failed.") );	
		User::Leave(KErrArgument);
		}
		
	
	THTTPHdrVal pt2Val;
	User::LeaveIfError(hdr.GetField(xferEncStr, 1,pt2Val));
	if (!CompareStringL(pt2Val.StrF(), KPart2Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of part2 value failed."));	
		User::Leave(KErrArgument);
		}
		
	
	THTTPHdrVal pt3Val;
	User::LeaveIfError(hdr.GetField(xferEncStr, 2,pt3Val));
	if (!CompareStringL(pt3Val.StrF(), KPart3Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of part3 value failed.") );	
		User::Leave(KErrArgument);
		}
		

	RStringF param1Str = iStrP.OpenFStringL(_L8("param1"));
	THTTPHdrVal param1Val;
	if (hdr.GetParam(xferEncStr, param1Str, param1Val, 1)) // part 1 is the extension1 part
		{
		iEngine->Utils().LogIt ( _L("param1 value is not present.") );	
		User::Leave(KErrArgument);
		}
		
	
	if (!CompareStringL(param1Val.StrF(), KParam1Val()))
		{
		iEngine->Utils().LogIt ( _L("Param1 value is not matching.") );	
		User::Leave(KErrArgument);
		}
		

	param1Str.Close();


	THTTPHdrVal pt4Val;
	User::LeaveIfError(hdr.GetField(xferEncStr, 3,pt4Val));
	if (!CompareStringL(pt4Val.StrF(), KPart4Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of part4 value (identity) is failed."));	
		User::Leave(KErrArgument);
		}
		
	
	THTTPHdrVal pt5Val;
	User::LeaveIfError(hdr.GetField(xferEncStr, 4,pt5Val));
	if (!CompareStringL(pt5Val.StrF(), KPart5Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of part5 value (extension3) is failed") );	
		User::Leave(KErrArgument);
		}
		
	
	THTTPHdrVal pt6Val;
	User::LeaveIfError(hdr.GetField(xferEncStr, 5,pt6Val));
	if (!CompareStringL(pt6Val.StrF(), KPart6Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of part6 value (extension4) is failed") );	
		User::Leave(KErrArgument);
		}
		

	RStringF param3Str = iStrP.OpenFStringL(_L8("param3"));
	THTTPHdrVal param3Val;
	if (hdr.GetParam(xferEncStr, param3Str, param3Val, 4)) // part 4 is the extension3 part
		{
		iEngine->Utils().LogIt ( _L("param3 value is missing.") );	
		User::Leave(KErrArgument);
		}
		
	
	if (!CompareStringL(param3Val.StrF(), KParam3Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of part3 value is failed.") );	
		User::Leave(KErrArgument);
		}
		

	param3Str.Close();

	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeTransferEncodingL test success."));
	}

void CHttpHdrCodecTest::TestDecodeWWWAuthenticateL()
	{
	_LIT8(KWWWAuthenticateVal1, "Basic realm=\"basicrealm\", Digest realm=\"digestrealm\""); // ota WWW-Authenticate header
	_LIT8(KWWWAuthenticateVal2, "Digest realm=\"digestrealm2\""); 
	_LIT8(KWWWAuthenticateVal3, "NTLM TlRMTVNTUAABAAAAA7IAAAoACgApAAAACQAJACAAAABMSUdIVENJVFlVUlNBLU1JTk9S==");
	_LIT8(KWWWAuthenticateVal4, "NTLM TlRMTVNTUAABAAAAA7IAAAoACgApAAAACQAJACAAAABMSUdIVENJVFlVUlNBLU1JTk9S=");
	_LIT8(KWWWAuthenticateVal5, "NTLM"); 
	_LIT8(KWWWAuthenticateVal6, "NTLM TlRMTVNTUAABAAAAA7IAAAoACgApAAAACQAJACAAAABMSUdIVENJVFlVUlNBLU1JTk9S");

	RHTTPHeaders hdr= GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'WWW-Authenticate' header"));

	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	RStringF wwwAuthStr = iStrP.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());
	hdr.SetRawFieldL(wwwAuthStr, KWWWAuthenticateVal1(), KFieldSeparator);
	hdr.SetRawFieldL(wwwAuthStr, KWWWAuthenticateVal2(), KFieldSeparator);
	
	hdr.SetRawFieldL(wwwAuthStr, KWWWAuthenticateVal3(), KFieldSeparator);
	hdr.SetRawFieldL(wwwAuthStr, KWWWAuthenticateVal4(), KFieldSeparator);
	hdr.SetRawFieldL(wwwAuthStr, KWWWAuthenticateVal5(), KFieldSeparator);
	hdr.SetRawFieldL(wwwAuthStr, KWWWAuthenticateVal6(), KFieldSeparator);
	hdr.SetRawFieldL(wwwAuthStr, KWWWAuthenticateVal2(), KFieldSeparator);

	_LIT8(KPart1Val, "Basic"); 
	_LIT8(KParam1, "realm"); 
	_LIT8(KParam1Val, "basicrealm"); 
	_LIT8(KPart2Val, "Digest"); 
	_LIT8(KParam2, "realm"); 
	_LIT8(KParam2Val, "digestrealm"); 
	_LIT8(KPart3Val, "Digest"); 
	_LIT8(KParam3, "realm"); 
	_LIT8(KParam3Val, "digestrealm2");
	_LIT8(KPart4689Val, "NTLM");
	_LIT8(KPart5Val, "TlRMTVNTUAABAAAAA7IAAAoACgApAAAACQAJACAAAABMSUdIVENJVFlVUlNBLU1JTk9S==");
	_LIT8(KPart7Val, "TlRMTVNTUAABAAAAA7IAAAoACgApAAAACQAJACAAAABMSUdIVENJVFlVUlNBLU1JTk9S=");
	_LIT8(KPart10Val, "TlRMTVNTUAABAAAAA7IAAAoACgApAAAACQAJACAAAABMSUdIVENJVFlVUlNBLU1JTk9S");
 
	
	iEngine->Console().Printf(_L("\ndecode header and check"));
	TInt parts = hdr.FieldPartsL(wwwAuthStr);
	if (hdr.FieldPartsL(wwwAuthStr) != 11)
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate header is not present.") );	
		User::Leave(KErrArgument);
		}
		

	// check first part
	THTTPHdrVal pt1Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 0,pt1Val)); 
	if(!CompareStringL(pt1Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part1 checking failed") );	
		User::Leave(KErrArgument);
		}
		
	
	RStringF param1Str = iStrP.OpenFStringL(KParam1());
	THTTPHdrVal param1Val;
	if (hdr.GetParam(wwwAuthStr, param1Str, param1Val, 0))
		{
		iEngine->Utils().LogIt ( _L("Cannot get param1 value.") );	
		User::Leave(KErrArgument);
		}
		
	
	if (!CompareStringL(param1Val.Str(), KParam1Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of param1 value is failed.") );	
		User::Leave(KErrArgument);
		}
		
	param1Str.Close();

	// check second part
	THTTPHdrVal pt2Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 1,pt2Val));
	if(!CompareStringL(pt2Val.StrF(), KPart2Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part2 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal param2Val;
	RStringF param2Str = iStrP.OpenFStringL(KParam2());
	if (hdr.GetParam(wwwAuthStr, param2Str, param2Val, 1))
		{
		iEngine->Utils().LogIt ( _L("Cannot get param2 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param2Val.Str(), KParam2Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of param2 value is failed.") );	
		User::Leave(KErrArgument);
		}
		
	param2Str.Close();

	// check third part
	THTTPHdrVal pt3Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 2,pt3Val));
	if(!CompareStringL(pt3Val.StrF(), KPart3Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part3 checking failed."));	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal param3Val;
	RStringF param3Str = iStrP.OpenFStringL(KParam3());
	if (hdr.GetParam(wwwAuthStr, param3Str, param3Val, 2))
		{
		iEngine->Utils().LogIt ( _L("Cannot get param3 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param3Val.Str(), KParam3Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of param3 value is failed.") );	
		User::Leave(KErrArgument);
		}
		
	param3Str.Close();
	
	THTTPHdrVal pt4Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 3, pt4Val));
	if(!CompareStringL(pt4Val.StrF(), KPart4689Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part4 checking failed."));	
		User::Leave(KErrArgument);
		}

	THTTPHdrVal pt5Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 4, pt5Val));
	if(!CompareStringL(pt5Val.StrF(), KPart5Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part5 checking failed."));	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal pt6Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 5, pt6Val));
	if(!CompareStringL(pt6Val.StrF(), KPart4689Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part6 checking failed."));	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal pt7Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 6, pt7Val));
	if(!CompareStringL(pt7Val.StrF(), KPart7Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part7 checking failed."));	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal pt8Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 7, pt8Val));
	if(!CompareStringL(pt8Val.StrF(), KPart4689Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part8 checking failed."));	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal pt9Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 8, pt9Val));
	if(!CompareStringL(pt9Val.StrF(), KPart4689Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part9 checking failed."));	
		User::Leave(KErrArgument);
		}

	THTTPHdrVal pt10Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 9, pt10Val));
	if(!CompareStringL(pt10Val.StrF(), KPart10Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part10 checking failed."));	
		User::Leave(KErrArgument);
		}
		
		
	THTTPHdrVal pt11Val;
	User::LeaveIfError(hdr.GetField(wwwAuthStr, 10,pt11Val));
	if(!CompareStringL(pt11Val.StrF(), KPart2Val()))
		{
		iEngine->Utils().LogIt ( _L("WWW-Authenticate - part11 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal param11Val;
	RStringF param11Str = iStrP.OpenFStringL(KParam2());
	if (hdr.GetParam(wwwAuthStr, param11Str, param11Val, 1))
		{
		iEngine->Utils().LogIt ( _L("Cannot get param11 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param11Val.Str(), KParam2Val()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of param11 value is failed.") );	
		User::Leave(KErrArgument);
		}
		
	param11Str.Close();

	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeWWWAuthenticateL test success."));
	}

void CHttpHdrCodecTest::TestDecodeCacheControlL()
	{
	
	_LIT8(KCacheControlVal1, "private,no-cache,must-revalidate,max-age=100");
	_LIT8(KCacheControlVal2, "no-cache,max-stale=50");
		
    _LIT8(KCacheControl1, "private");
	_LIT8(KCacheControl2, "no-cache");
	_LIT8(KCacheControl3, "must-revalidate");
	_LIT8(KCacheControlParam1, "max-age");
	_LIT8(KCacheControlParam2, "max-stale");
	
	TInt maxAgeVal = 100;
	TInt maxStaleVal = 50;
	THTTPHdrVal maxAgeHdrVal(maxAgeVal);
	THTTPHdrVal maxStaleHdrVal(maxStaleVal);	

	RHTTPHeaders hdr = GetHeadersLC();

	RStringF cacheControlStr = iStrP.StringF(HTTP::ECacheControl,RHTTPSession::GetTable());
	hdr.SetRawFieldL(cacheControlStr, KCacheControlVal1, KFieldSeparator);
	hdr.SetRawFieldL(cacheControlStr, KCacheControlVal2, KFieldSeparator);
		
	if (hdr.FieldPartsL(cacheControlStr) != 6)
		{
		iEngine->Utils().LogIt ( _L("Cache control header doesn't have 6 parts.") );	
		User::Leave(KErrArgument);
		}
		
	THTTPHdrVal val;
	User::LeaveIfError(hdr.GetField(cacheControlStr, 0,val));
	if(!CompareStringL(val.StrF(), KCacheControl1()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of cache control1 value is failed.") );	
		User::Leave(KErrArgument);
		}		
	
	User::LeaveIfError(hdr.GetField(cacheControlStr, 1,val));
	if(!CompareStringL(val.StrF(), KCacheControl2()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of cache control2 value is failed.") );	
		User::Leave(KErrArgument);
		}
	
	User::LeaveIfError(hdr.GetField(cacheControlStr, 2,val));
	if(!CompareStringL(val.StrF(), KCacheControl3()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of cache control3 value is failed.") );	
		User::Leave(KErrArgument);
		}
		
	RStringF param1Str = iStrP.OpenFStringL(KCacheControlParam1());
	CleanupClosePushL(param1Str);
	THTTPHdrVal param1Val;
	if(hdr.GetParam(cacheControlStr, param1Str, param1Val, 3))
		{
		iEngine->Utils().LogIt ( _L("\ncacheControlStr - Cannot get param value.") );	
		User::Leave(KErrArgument);
		}
	if(!CompareValue(param1Val.Int(),maxAgeVal))
	    {
	    iEngine->Utils().LogIt ( _L("Comparison of cache control paramvalue is failed.") );		
	    User::Leave(KErrArgument);
	    }
	CleanupStack::PopAndDestroy(&param1Str);
	
			
	User::LeaveIfError(hdr.GetField(cacheControlStr, 4,val));
	if(!CompareStringL(val.StrF(), KCacheControl2()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of cache control5 value is failed."));	
		User::Leave(KErrArgument);
		}
	
	RStringF param2Str = iStrP.OpenFStringL(KCacheControlParam2());
	CleanupClosePushL(param2Str);	
	THTTPHdrVal param2Val;
	if(hdr.GetParam(cacheControlStr, param2Str, param2Val,5))
		{
		iEngine->Utils().LogIt ( _L("\ncacheControlStr - Cannot get param value.") );	
		User::Leave(KErrArgument);
		}
	if(!CompareValue(param2Val.Int(),maxStaleVal))
	    {
	    iEngine->Utils().LogIt ( _L("Comparison of cache control paramvalue is failed.") );		
	    User::Leave(KErrArgument);
	    }
	CleanupStack::PopAndDestroy(&param2Str);
	
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeCacheControlL test success."));
	}

void CHttpHdrCodecTest::TestEncodeUpgradeL()
	{
	_LIT8(KUpgradeOTAVal1, "HTTP/2.0, SHTTP/1.3");
		
	iEngine->Console().Printf(_L("\nTest encoding 'Upgrade' header"));
	RHTTPHeaders hdr = GetHeadersLC();
		
	RStringF http = iStrP.OpenFStringL(_L8("HTTP/2.0"));
	CleanupClosePushL(http);
	RStringF shttp = iStrP.OpenFStringL(_L8("SHTTP/1.3"));
	CleanupClosePushL(shttp);
		
	RStringF upgradeStr = iStrP.StringF(HTTP::EUpgrade, RHTTPSession::GetTable());
	
	THTTPHdrVal val(http);
	THTTPHdrVal val2(shttp);
	
	hdr.SetFieldL(upgradeStr, val);
	hdr.SetFieldL(upgradeStr, val2);
		
	TPtrC8 ota;
	hdr.GetRawField(upgradeStr, ota);
	if(ota.Compare(KUpgradeOTAVal1) != 0)
		{
		TBuf<512> temp;
		temp.Copy(ota);
		iEngine->Utils().LogIt(_L("\nComparison of Upgrade field value failed: %S"), &temp);	
		User::Leave(KErrArgument);
		}
	
	CleanupStack::PopAndDestroy(2);//Pop and destroy http and shttp objects
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestEncodeUpgradeL test success."));
	}

void CHttpHdrCodecTest::TestDecodeUpgradeL()
	{
	_LIT8(KUpgradeHdrVal, "HTTP/2.0, SHTTP/1.3" );
	_LIT8(KHttpVal, "HTTP/2.0");
	_LIT8(KSHttpVal, "SHTTP/1.3");

	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest decoding 'Upgrade' header"));
	iEngine->Console().Printf(_L("\nadd OTA data to header collection"));
	
	RStringF upgradeStr = iStrP.StringF(HTTP::EUpgrade,RHTTPSession::GetTable());
	
	hdr.SetRawFieldL(upgradeStr, KUpgradeHdrVal(), KFieldSeparator);	

	iEngine->Console().Printf(_L("\ndecode header and check"));
	
	if (hdr.FieldPartsL(upgradeStr) != 2)//Check if the Upgrade header has 2 values
		{
		iEngine->Utils().LogIt ( _L("Upgrade header not present.") );	
		User::Leave(KErrArgument);
		}
	
	//Get the HTTP/2.0
	THTTPHdrVal upgradePt;
	User::LeaveIfError(hdr.GetField(upgradeStr, 0,upgradePt));
	
	if(!CompareStringL(upgradePt.StrF(), KHttpVal()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of Upgrade header value failed.") );	
		User::Leave(KErrArgument);
		}
	
	//Get the SHTTP/1.3
	THTTPHdrVal upgradePt2;
	User::LeaveIfError(hdr.GetField(upgradeStr, 1, upgradePt2));
	
	if(!CompareStringL(upgradePt2.StrF(), KSHttpVal()))
		{
		iEngine->Utils().LogIt ( _L("Comparison of Upgrade header value failed.") );	
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeUpgradeL test success."));
	}

void CHttpHdrCodecTest::TestRoundTripConnectionL()
	{
	}

void CHttpHdrCodecTest::TestRoundTripContentLengthL()
	{
	_LIT8(KContentLengthOTAVal, "25234");
	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest round-tripping 'Content-Length' header"));
	RStringF contLenStr = iStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable());
	hdr.SetRawFieldL(contLenStr, KContentLengthOTAVal(), KFieldSeparator);
	
	// Force a decode by getting the 1st part of the value
	THTTPHdrVal dummy;
	User::LeaveIfError(hdr.GetField(contLenStr,0,dummy));

	// Force a re-encode by getting the OTA data
	TPtrC8 ota;
	User::LeaveIfError(hdr.GetRawField(contLenStr, ota));

	// This should look identical
	if (ota.Compare(KContentLengthOTAVal()) != 0)
		{
		iEngine->Utils().LogIt ( _L("Content-Length field value is not matching.") );	
		User::Leave(KErrArgument);
		}
		
	//
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestRoundTripContentLengthL test success."));
	}


void CHttpHdrCodecTest::TestRoundTripContentLength2L()
	{
	HBufC8* onesBuf = HBufC8::NewLC(4096);
	TPtr8 ones = onesBuf->Des();
	
	ones.AppendFill('1',4096);
	_LIT8(KKnownToBeBad, "2147483649");

	const TInt KNumberOfOnes[] = {9, 32, 255, 256, 4096};

	for(TInt i=0;i<6;i++) 
		{
		TPtrC8 use;
		if(i<5) 
			{
			ones.SetLength(KNumberOfOnes[i]);
			use.Set(ones);
			}
		else
			{
			use.Set(KKnownToBeBad);
			}
		RHTTPHeaders hdr = GetHeadersLC();
		//
		iEngine->Console().Printf(_L("\nTest round-tripping 'Content-Length' header"));
		RStringF contLenStr = iStrP.StringF(HTTP::EContentLength,RHTTPSession::GetTable());
		hdr.SetRawFieldL(contLenStr, use , KFieldSeparator);
		
		// Force a decode by getting the 1st part of the value, if possible. 
		THTTPHdrVal dummy;
		hdr.GetField(contLenStr,0,dummy); // ignore errors

		// Force a re-encode by getting the OTA data
		TPtrC8 ota;
		User::LeaveIfError(hdr.GetRawField(contLenStr, ota));
		

		// This should look identical
		if (ota.Compare(use) != 0)
			{
			iEngine->Utils().LogIt ( _L("Round-tripping Content-Length field is not matching.") );	
			User::Leave(KErrArgument);	
			}
		
		//
		ReleaseHeaders();
		}
		
	CleanupStack::PopAndDestroy(onesBuf);
	iEngine->Utils().LogIt ( _L ("\nTestRoundTripContentLength2L test success."));
	}

void CHttpHdrCodecTest::TestRoundTripContentTypeL()
	{
	_LIT8(KContentTypeOTAVal1, "text/plain");
	_LIT8(KContentTypeOTAVal2, "text/html; charset=us-ascii");
	_LIT8(KContentTypeOTAVal3, "text/vnd.wap/wml; charset=utf-8; wmlversion=12");
	RHTTPHeaders hdr = GetHeadersLC();
	//
	iEngine->Console().Printf(_L("\nTest round-tripping 'Content-Type' header"));
	RStringF contTypeStr = iStrP.StringF(HTTP::EContentType,RHTTPSession::GetTable());
	hdr.SetRawFieldL(contTypeStr, KContentTypeOTAVal1(), KFieldSeparator);
	
	// Force a decode by getting the 1st part of the value
	THTTPHdrVal dummy;
	User::LeaveIfError(hdr.GetField(contTypeStr,0,dummy));

	// Force a re-encode by getting the OTA data
	TPtrC8 ota;
	User::LeaveIfError(hdr.GetRawField(contTypeStr, ota));

	// This should look identical
	if (ota.Compare(KContentTypeOTAVal1()) != 0)
		{
		iEngine->Utils().LogIt ( _L("Content-Type value1 is not matching.") );	
		User::Leave(KErrArgument);
		}
		

	// Now do the charset parameter test
	hdr.RemoveField(contTypeStr);
	hdr.SetRawFieldL(contTypeStr, KContentTypeOTAVal2(), KFieldSeparator);
	User::LeaveIfError(hdr.GetField(contTypeStr,0,dummy));
	User::LeaveIfError(hdr.GetRawField(contTypeStr, ota));
	if (ota.Compare(KContentTypeOTAVal2()) != 0)
		{
		iEngine->Utils().LogIt ( _L("Content-Type value2 is not matching.") );	
		User::Leave(KErrArgument);
		}
		

	// Now do the two parameter test
	hdr.RemoveField(contTypeStr);
	hdr.SetRawFieldL(contTypeStr, KContentTypeOTAVal3(), KFieldSeparator);
	User::LeaveIfError(hdr.GetField(contTypeStr,0,dummy));
	User::LeaveIfError(hdr.GetRawField(contTypeStr, ota));
	if (ota.Compare(KContentTypeOTAVal3()) != 0)
		{
		iEngine->Utils().LogIt ( _L("Content-Type value3 is not matching.") );	
		User::Leave(KErrArgument);
		}
		
	//
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestRoundTripContentTypeL test success."));
	}

void CHttpHdrCodecTest::TestDecodeContentDispositionL()
{
    iEngine->Utils().LogIt ( _L ("\nTesting decode functionality of Content-Disposition."));

	_LIT8(KContentDisposition, "attachment;filename=Starshine");
	_LIT8(KContentDisposition1, "attachment;filename=Starshine;");
	_LIT8(KContentDisposition2, "attachment;filename=\"Starshine\"");
	_LIT8(KContentDisposition3, "attachment;filename=\"Starshine\";");
	_LIT8(KContentDisposition4, "attachment;filename=\"Star;shine\"");
	_LIT8(KContentDisposition5, "attachment;filename=\"Star;shine\";");
	_LIT8(KContentDisposition6, "attachment;filename=\"Star;shine\";date=\"25/06/1983\"");
	_LIT8(KContentDisposition7, "attachment;filename=\"Star;shine\";date=\"25-06-1983\";");
	_LIT8(KContentDisposition8, "attachment;filename=Starshine;date=\"25-06-1983\"");
	_LIT8(KContentDisposition9, "attachment;filename=Starshine;date=25-06-1983");

	
	RHTTPHeaders hdr= GetHeadersLC();
	
	RStringF contentDisposition = iStrP.StringF(HTTP::EContentDisposition,RHTTPSession::GetTable());
	
	_LIT8(KPart1Val, "attachment"); 
	_LIT8(KParam1, "filename");
	_LIT8(KParam2, "date"); 
	_LIT8(KParam1Val, "Starshine"); 
	_LIT8(KParam2Val, "Star;shine");
	_LIT8(KParam3Val, "25/06/1983");
	_LIT8(KParam4Val, "25-06-1983");


    // First Case.
	hdr.SetRawFieldL(contentDisposition, KContentDisposition(), KFieldSeparator);
	iEngine->Utils().LogIt ( _L ("\nCase 1:- Content-Disposition: attachment;filename=Starshine"));	
	THTTPHdrVal pt1Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt1Val)); 
	if(!CompareStringL(pt1Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
		User::Leave(KErrArgument);
		}
		
	RStringF param1Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param1Str);
	THTTPHdrVal param1Val;
	if (hdr.GetParam(contentDisposition, param1Str, param1Val, 0))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param1Val.StrF(), KParam1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	CleanupStack::PopAndDestroy(&param1Str);
	hdr.RemoveField( contentDisposition );

    // Second Case.
	iEngine->Utils().LogIt ( _L ("\nCase 2:- Content-Disposition: attachment;filename=Starshine;"));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition1(), KFieldSeparator);
	THTTPHdrVal pt2Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt2Val)); 
	if(!CompareStringL(pt2Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
		User::Leave(KErrArgument);
		}
		
	RStringF param2Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param2Str);
	THTTPHdrVal param2Val;
	if (hdr.GetParam(contentDisposition, param2Str, param2Val, 0))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param2Val.StrF(), KParam1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	CleanupStack::PopAndDestroy(&param2Str);
	hdr.RemoveField( contentDisposition );
	
    // Third Case.
	iEngine->Utils().LogIt ( _L ("\nCase 3:- Content-Disposition: attachment;filename=\"Starshine\""));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition2(), KFieldSeparator);
	THTTPHdrVal pt3Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt3Val)); 
	if(!CompareStringL(pt3Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
		User::Leave(KErrArgument);
		}
		
	RStringF param3Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param3Str);
	THTTPHdrVal param3Val;
	if (hdr.GetParam(contentDisposition, param3Str, param3Val, 0))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param3Val.StrF(), KParam1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	CleanupStack::PopAndDestroy(&param3Str);
	hdr.RemoveField( contentDisposition );

    // Fourth Case.	
	iEngine->Utils().LogIt ( _L ("\nCase 4:- Content-Disposition: attachment;filename=\"Starshine\";"));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition3(), KFieldSeparator);
	THTTPHdrVal pt4Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt4Val)); 
	if(!CompareStringL(pt4Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
		User::Leave(KErrArgument);
		}
		
	RStringF param4Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param4Str);
	THTTPHdrVal param4Val;
	if (hdr.GetParam(contentDisposition, param4Str, param4Val, 0))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param4Val.StrF(), KParam1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	CleanupStack::PopAndDestroy(&param4Str);
	hdr.RemoveField( contentDisposition );

    // Fifth Case.	
	iEngine->Utils().LogIt ( _L ("\nCase 5:- Content-Disposition: attachment;filename=\"Star;shine\""));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition4(), KFieldSeparator);
	THTTPHdrVal pt5Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt5Val)); 
	if(!CompareStringL(pt5Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
		User::Leave(KErrArgument);
		}
		
	RStringF param5Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param5Str);
	THTTPHdrVal param5Val;
	if (hdr.GetParam(contentDisposition, param5Str, param5Val, 0))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param5Val.StrF(), KParam2Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	CleanupStack::PopAndDestroy(&param5Str);
	hdr.RemoveField( contentDisposition );

    // Sixth Case.	
	iEngine->Utils().LogIt ( _L ("\nCase 6:- Content-Disposition: attachment;filename=\"Star;shine\";"));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition5(), KFieldSeparator);
	THTTPHdrVal pt6Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt6Val)); 
	if(!CompareStringL(pt6Val.StrF(), KPart1Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
		User::Leave(KErrArgument);
		}
		
	RStringF param6Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param6Str);
	THTTPHdrVal param6Val;
	if (hdr.GetParam(contentDisposition, param6Str, param6Val, 0))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
		User::Leave(KErrArgument);
		}
		
	if (!CompareStringL(param6Val.StrF(), KParam2Val()))
		{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
		User::Leave(KErrArgument);
		}
		
	CleanupStack::PopAndDestroy(&param6Str);
	hdr.RemoveField( contentDisposition );

	// Seventh Case.	
	iEngine->Utils().LogIt ( _L ("\nCase 7:- Content-Disposition: attachment;filename=\"Star;shine\";date=\"25/06/1983\""));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition6(), KFieldSeparator);
	THTTPHdrVal pt7Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt7Val)); 
	if(!CompareStringL(pt7Val.StrF(), KPart1Val()))
	{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
		User::Leave(KErrArgument);
	}
	
	RStringF param7Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param7Str);
	THTTPHdrVal param7Val;
	if (hdr.GetParam(contentDisposition, param7Str, param7Val, 0))
	{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
		User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param7Val.StrF(), KParam2Val()))
	{
		iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
		User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param7Str);
	
	RStringF param8Str = iStrP.OpenFStringL(KParam2());
	CleanupClosePushL(param8Str);
	THTTPHdrVal param8Val;
	if (hdr.GetParam(contentDisposition, param8Str, param8Val, 0))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param2 value.") );	
	User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param8Val.StrF(), KParam3Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - param2 checking failed.") );	
	User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param8Str);
	
	hdr.RemoveField( contentDisposition );
	
	
	// Eight Case.	
	iEngine->Utils().LogIt ( _L ("\nCase 8:- Content-Disposition: attachment;filename=\"Star;shine\";date=\"25-06-1983\";"));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition7(), KFieldSeparator);
	THTTPHdrVal pt8Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt8Val)); 
	if(!CompareStringL(pt8Val.StrF(), KPart1Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
	User::Leave(KErrArgument);
	}
	
	RStringF param9Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param9Str);
	THTTPHdrVal param9Val;
	if (hdr.GetParam(contentDisposition, param9Str, param9Val, 0))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
	User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param9Val.StrF(), KParam2Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
	User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param9Str);
	
	RStringF param10Str = iStrP.OpenFStringL(KParam2());
	CleanupClosePushL(param10Str);
	THTTPHdrVal param10Val;
	if (hdr.GetParam(contentDisposition, param10Str, param10Val, 0))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param2 value.") );	
	User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param10Val.StrF(), KParam4Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - param2 checking failed.") );	
	User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param10Str);
	hdr.RemoveField( contentDisposition );
	
	// Ninth case
	iEngine->Utils().LogIt ( _L ("\nCase 9:- Content-Disposition: attachment;filename=Starshine;date=\"25-06-1983\""));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition8(), KFieldSeparator);
	THTTPHdrVal pt9Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt9Val)); 
	if(!CompareStringL(pt9Val.StrF(), KPart1Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
	User::Leave(KErrArgument);
	}
	
	RStringF param11Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param11Str);
	THTTPHdrVal param11Val;
	if (hdr.GetParam(contentDisposition, param11Str, param11Val, 0))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
	User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param11Val.StrF(), KParam1Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
	User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param11Str);
	
	RStringF param12Str = iStrP.OpenFStringL(KParam2());
	CleanupClosePushL(param12Str);
	THTTPHdrVal param12Val;
	if (hdr.GetParam(contentDisposition, param12Str, param12Val, 0))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param2 value.") );	
	User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param12Val.StrF(), KParam4Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - param2 checking failed.") );	
	User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param12Str);
	hdr.RemoveField( contentDisposition );
	//Tenth case
	iEngine->Utils().LogIt ( _L ("\nCase 10:- Content-Disposition: attachment;filename=Starshine;date=\"25-06-1983\""));	
	hdr.SetRawFieldL(contentDisposition, KContentDisposition9(), KFieldSeparator);
	THTTPHdrVal pt10Val;
	User::LeaveIfError(hdr.GetField(contentDisposition, 0,pt10Val)); 
	if(!CompareStringL(pt10Val.StrF(), KPart1Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - part1 checking failed") );	
	User::Leave(KErrArgument);
	}
	
	RStringF param13Str = iStrP.OpenFStringL(KParam1());
	CleanupClosePushL(param13Str);
	THTTPHdrVal param13Val;
	if (hdr.GetParam(contentDisposition, param13Str, param13Val, 0))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param1 value.") );	
	User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param13Val.StrF(), KParam1Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - param1 checking failed.") );	
	User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param13Str);
	
	RStringF param14Str = iStrP.OpenFStringL(KParam2());
	CleanupClosePushL(param14Str);
	THTTPHdrVal param14Val;
	if (hdr.GetParam(contentDisposition, param14Str, param14Val, 0))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - Cannot get param2 value.") );	
	User::Leave(KErrArgument);
	}
	
	if (!CompareStringL(param14Val.StrF(), KParam4Val()))
	{
	iEngine->Utils().LogIt ( _L("\nContent-Disposition - param2 checking failed.") );	
	User::Leave(KErrArgument);
	}
	
	CleanupStack::PopAndDestroy(&param14Str);
	hdr.RemoveField( contentDisposition );
	
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTestDecodeContentDispositionL test success."));
    }

void CHttpHdrCodecTest::TestGetCustomFieldValueL()
	{
	iEngine->Utils().LogIt ( _L ("\nTesting custom header value. The value is folded into multiple lines."));
	
	_LIT8(KCustomField, "foldedField");
	_LIT8(KCustomVal, "first line\r\nsecond line\r\r\nthird line\r\n");
	
	RHTTPHeaders hdr= GetHeadersLC();
	RStringF customField = iStrP.OpenFStringL(KCustomField());
	CleanupClosePushL(customField);
	RStringF customVal = iStrP.OpenFStringL(KCustomVal());
	CleanupClosePushL(customVal);
	
	THTTPHdrVal val(customVal);
	
	hdr.SetFieldL(customField, val);
	TPtrC8 rawValue;
	hdr.GetRawField(customField, rawValue);
	
	CleanupStack::PopAndDestroy(&customVal);
	CleanupStack::PopAndDestroy(&customField);
	
	TPtrC8 hdrVal(KCustomVal());
	hdrVal.Set(hdrVal.Left(hdrVal.Length() - 1));
	if(rawValue.Compare(hdrVal) != 0)
		{
		iEngine->Utils().LogIt ( _L("\nTesting custom header value failed.") );	
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTesting custom header value was successful."));
	}

void CHttpHdrCodecTest::TestGetCustomFieldNullValueL()
	{
	iEngine->Utils().LogIt ( _L ("\nTesting custom header value as NULL"));
	
	_LIT8(KCustomField, "X-Oma-Drm-Separate-Delivery");
	
	RHTTPHeaders hdr= GetHeadersLC();
	RStringF customField = iStrP.OpenFStringL(KCustomField());
	CleanupClosePushL(customField);
	RStringF customVal = iStrP.OpenFStringL(KNullDesC8);
	CleanupClosePushL(customVal);
	
	THTTPHdrVal val(customVal);
	
	hdr.SetFieldL(customField, val);
	TPtrC8 rawValue;
	hdr.GetRawField(customField, rawValue);
	
	CleanupStack::PopAndDestroy(&customVal);
	CleanupStack::PopAndDestroy(&customField);
	
	TPtrC8 hdrVal(KNullDesC8);
	if(rawValue.Compare(hdrVal) != 0)
		{
		iEngine->Utils().LogIt ( _L("\nTesting custom header value failed.") );	
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTesting custom header value was successful."));
	}

void CHttpHdrCodecTest::TestGetCustomFieldNoValueL()
	{
	iEngine->Utils().LogIt ( _L ("\nTesting custom header value as [\r\n]"));
	
	_LIT8(KCustomField, "X-Oma-Drm-Separate-Delivery");
	
	RHTTPHeaders hdr= GetHeadersLC();
	RStringF customField = iStrP.OpenFStringL(KCustomField());
	CleanupClosePushL(customField);
	RStringF customVal = iStrP.OpenFStringL(KNullDesC8);
	CleanupClosePushL(customVal);
	
	hdr.SetRawFieldL(customField, KNullDesC8, KFieldSeparator);
	TPtrC8 rawValue;
	hdr.GetRawField(customField, rawValue);
	
	CleanupStack::PopAndDestroy(&customVal);
	CleanupStack::PopAndDestroy(&customField);
	
	TPtrC8 hdrVal(KNullDesC8);
	if(rawValue.Compare(hdrVal) != 0)
		{
		iEngine->Utils().LogIt ( _L("\nTesting custom header value failed.") );	
		User::Leave(KErrArgument);
		}
		
	ReleaseHeaders();
	iEngine->Utils().LogIt ( _L ("\nTesting custom header value was successful."));
	}

void CHttpHdrCodecTest::TestInvalidHeaderFieldPartL ()
	{
	iEngine->Utils().LogIt ( _L ("\nTestInvalidHeaderFieldPartL test started."));
	iEngine->Console().Printf(_L("\nTest 'Accept'(for example) header by adding one/more field parts and query for non-existing field part"));
	iEngine->Utils().LogIt ( _L ("\nCreating Accept Header as Accept: text/html; q=0.8, text/vnd.wap.wml; q=0.2; extended=value, text/*"));

	// Open strings used in this test
	RStringF textHtml = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF textWml  = iStrP.StringF(HTTP::ETextVndWapWml,RHTTPSession::GetTable());
	RStringF textAny  = iStrP.StringF(HTTP::ETextAny,RHTTPSession::GetTable());

	RStringF extended  = iStrP.OpenFStringL(_L8("extended"));
	CleanupClosePushL(extended);
	RStringF extendVal = iStrP.OpenFStringL(_L8("value"));
	CleanupClosePushL(extendVal);
	//
	
	RHTTPHeaders hdr = 	GetHeadersLC();

	// Set up the parsed header
	RStringF accept = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	THTTPHdrVal accVal(textHtml);
	THTTPHdrVal q(THTTPHdrVal::TQConv(0.8));
	hdr.SetFieldL(accept, accVal, iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable()), q);
	accVal.SetStrF(textWml);
	q.SetInt(THTTPHdrVal::TQConv(0.2));
	hdr.SetFieldL(accept, accVal, iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable()), q);
	q.SetStrF(extendVal);
	hdr.SetFieldL(accept, accVal, extended, q);
	accVal.SetStrF(textAny);
	hdr.SetFieldL(accept, accVal);	
	// now get the OTA data
	THTTPHdrVal paramVal;
	TInt err = hdr.GetParam(accept, iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable()), paramVal, -1);
	
	if ( err != KErrNotFound )
		{
		iEngine->Utils().LogIt ( _L("\nTestInvalidHeaderFieldPartL failed, expected error: KErrNotFound, result error [%d]"), err );
		User::Leave(KErrArgument);
		}
	
	// Close strings used in this test
	ReleaseHeaders();
	CleanupStack::PopAndDestroy(2, &extended);
	
	iEngine->Utils().LogIt ( _L ("\nTestInvalidHeaderFieldPartL test success."));

	}

void CHttpHdrCodecTest::TestRoundTripTransferEncodingL()
	{
	}

TBool CHttpHdrCodecTest::CompareStringL(RStringF aStrTk, const TDesC8& aDes)
	{
	RStringF desStr = iStrP.OpenFStringL(aDes);
	TBool retVal = (desStr == aStrTk);
	desStr.Close();
	return retVal;
	}
TBool CHttpHdrCodecTest::CompareStringL(RString aStrTk, const TDesC8& aDes)
	{
	RString desStr = iStrP.OpenStringL(aDes);
	TBool retVal = (desStr == aStrTk);
	desStr.Close();
	return retVal;
	}

TBool CHttpHdrCodecTest::CompareValue(TInt aVal1, TInt aVal2)
	{
	return (aVal1 == aVal2);
    }

TBool CHttpHdrCodecTest::CompareDate(TDateTime aDate1, TDateTime aDate2)
	{
	return ((aDate1.Year() == aDate2.Year()) &&
			(aDate1.Month() == aDate2.Month()) &&
			(aDate1.Day() == aDate2.Day()) &&
			(aDate1.Hour() == aDate2.Hour()) &&
			(aDate1.Minute() == aDate2.Minute()) &&
			(aDate1.Second() == aDate2.Second()) &&
			(aDate1.MicroSecond() == aDate2.MicroSecond()));
	}


void CHttpHdrCodecTest::ResetTimeElapsed()
// Resets timestamp to time now
	{
	iLastTimeStamp.UniversalTime(); 
	}


void CHttpHdrCodecTest::DisplayTimeElapsed()
// Calculate elapsed time since last measurement, and display
	{
	TTime timeNow;
	timeNow.UniversalTime();
	TTimeIntervalMicroSeconds elapsedMicroSec =
									timeNow.MicroSecondsFrom(iLastTimeStamp);
	iLastTimeStamp = timeNow;
	iEngine->Console().Printf(
		_L("Time elapsed since last measurement is: %d ms\n"),
		elapsedMicroSec.Int64()/1000
		);
	}

RHTTPHeaders CHttpHdrCodecTest::GetHeadersLC()
	{
	_LIT8(KUri, "http://www.symbian.com");
	TUriParser8 up;
	up.Parse(KUri);
	iTransaction = iSession.OpenTransactionL(up, *this, iSession.StringPool().StringF(HTTP::EGET,RHTTPSession::GetTable()));
	CleanupClosePushL(iTransaction);
	return iTransaction.Request().GetHeaderCollection();
	}

void CHttpHdrCodecTest::ReleaseHeaders()
	{
	CleanupStack::PopAndDestroy(&iTransaction);
	}

void  CHttpHdrCodecTest::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}

TInt CHttpHdrCodecTest::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}

