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
// System includes
// 
//


#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

#if !defined HTTP_H
#include <http.h>
#endif

#include <http/framework/cprotocolhandler.h>

//
// Global declarations
//

// Throughout:  iStrP is a handle to the HTTP string table;
//
// Strings are represented as handles to HTTP strings;
//  i.e.   RHttpString str;
//
// String table lookup (i.e. getting the string handle from some descriptor) is done as so:
//
//  RHttpString str = iStrP(_L("My String"));
//
// using an operator() overload in the string table class.  Perhaps that could be macro'd in itself.
//
// Furthermore, strings which are 'very well known' and therefore certainly expected to be in ROM can be
// converted to HTTP string handles using enumerations:
//
//  RHttpString str = iStrP.OpenFStringL(KHttpAccept); // guaranteed to give the same handle as:
//  RHttpString str = iStrP.OpenFStringL(KHttpAccept);


class CProtocolHandler;

//
// Definition of CHttpHdrCodecTest
//

class CHttpHdrCodecTest : public CHttpTestBase, public MHTTPTransactionCallback
	{
public:
	virtual ~CHttpHdrCodecTest();

	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();

	static CHttpHdrCodecTest* NewLC();
	static CHttpHdrCodecTest* NewL();
	void RunTestsL();

	// from MHTTPTransactionCallback 
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

protected:
	CHttpHdrCodecTest();
private:
	void ResetTimeElapsed();
	void DisplayTimeElapsed();
	//
	void TestDecodeEncodeCacheControlL();
	void TestEncodeAcceptL();
	void TestEncodeAcceptCharsetL();
	void TestEncodeAuthorizationL();
	void TestEncodeConnectionL();
	void TestEncodeContentLengthL();
	void TestEncodeContentTypeL();
	void TestEncodeHostL();
	void TestEncodeTransferEncodingL();
	void TestEncodeUserAgentL();
	void TestEncodeDateL();
	void TestEncodeCookieL();
	void TestEncodeCacheControlL();
	void TestEncodeLocaleIndependentDateL();
	void TestEncodeUpgradeL();
	//
	void TestDecodeConnectionL();
	void TestDecodeContentLengthL();
	void TestDecodeContentTypeL();
	void TestDecodeContentTypeWithMultipleParametersL();
	void TestDecodeContentTypeMissingParameterL();
	void TestDecodeDateL();
	void TestDecodeServerL();
	void TestDecodeTransferEncodingL();
	void TestDecodeWWWAuthenticateL();
	void TestDecodeSetCookieL();
	void TestDecodeSetCookie2L();
	void TestDecodeSetCookie3L();
	void TestDecodeSetCookie4L();
	void TestEncodeCookiePortL();
	void TestDecodeCacheControlL();
	void TestDecodeUpgradeL();
	//
	void TestRoundTripConnectionL();
	void TestRoundTripContentLengthL();
	void TestRoundTripContentLength2L();
	void TestRoundTripContentTypeL();
	void TestRoundTripTransferEncodingL();
	//
	void TestDuplicateHeaderResponsesL();
	//
	void TestDuplicateHeaderDecodingL(RStringF aHeader,
									  const TDesC8& aRawResponseHeader,
									  TInt aExpectedNumberParts);

	//test for DEF078684: Authorization header nonce count is sent incorrectly
	void TestEncodeAuthorization1L();
	
	//test for INC118854:Invalid parsing of HTTP headers when value contains a ";" character
	void TestDecodeContentDispositionL();

	//a header field whose value is folded into multiple lines.
	void TestGetCustomFieldValueL();

	//a header field whose value is NULL.
	void TestGetCustomFieldNullValueL();

	//a header field with no value [\r\n].
	void TestGetCustomFieldNoValueL();
	
	// test for PDEF138861: Trying to access header field part at Index < 0
	void TestInvalidHeaderFieldPartL ();
	
	//
	TBool CompareStringL(RStringF aStr, const TDesC8& aDes);
	TBool CompareStringL(RString aStr, const TDesC8& aDes);
	TBool CompareDate(TDateTime aDate1, TDateTime aDate2);
	TBool CompareValue(TInt aVal1, TInt aVal2);

	RHTTPHeaders GetHeadersLC();
	void ReleaseHeaders();
private:
	TTime iLastTimeStamp;
	RStringPool iStrP;
	CHeaders* iHdrColl;
	CProtocolHandler* iProtHnd;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction; // only used to create headers
	};


 
