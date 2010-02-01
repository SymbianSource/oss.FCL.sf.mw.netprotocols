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
// System includes
// 
//

#include <e32test.h>
#include <e32base.h>


#include <http.h>
#include "cheaderfield.h"
#include "cheaders.h"
#include "ctextmodeheadercodec.h"

#include "t_httphdr.h"


//
// Implementation of CHttpHdrTest
//

CHttpHdrTest::CHttpHdrTest()
// Constructor
	{
	// Initial timestamp is time now
	ResetTimeElapsed();
	}


CHttpHdrTest::~CHttpHdrTest()
// Destructor (virtual)
	{
	}

CHttpHdrTest* CHttpHdrTest::NewLC()
// Create a new CHttpHdrTest and place it on the clean-up stack then return it
	{
	CHttpHdrTest* me = new(ELeave) CHttpHdrTest;
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}

CHttpHdrTest* CHttpHdrTest::NewL()
// Create a new CHttpHdrTest and return it
	{
	CHttpHdrTest* me = new(ELeave) CHttpHdrTest;
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop();
	return me;
	}

void CHttpHdrTest::ConstructL()
// Second phase construction
	{
	}

void CHttpHdrTest::TestL(TBool aTestCondition)
	{
	if (!aTestCondition)
		User::Leave(KErrGeneral);
	}

const TDesC& CHttpHdrTest::TestName()
	{
	_LIT(KTestName, "HTTP Headers Test");
	return KTestName;
	}


void CHttpHdrTest::DoRunL()
	{
	iExpectedStatusCode = 0;
	RunTestsL();
	}

void CHttpHdrTest::DoCancel()
	{
	}


void CHttpHdrTest::RunTestsL()
	{
	ResetTimeElapsed();

	// Locate the string pool
	CleanupClosePushL(iStrP);
	iStrP.OpenL(RHTTPSession::GetTable());

	// Run tests on internal classes 
	CTextModeHeaderCodec* headerCodec = CTextModeHeaderCodec::NewL(iStrP);
	CleanupStack::PushL(headerCodec);
	iHdrColl = CHeaders::NewL(*headerCodec);
	CleanupStack::PushL(iHdrColl);
	TestHdrValL();
	TestBasicsL();
	TestAddSinglePartLongL();
	TestAddSinglePartQuickL();
	TestAddMultiPartL();
	TestAddParamL();
	TestRemovePartL();
	TestRemoveParamL();
	TestLookupPartL();
	TestLookupParamL();
	CleanupStack::PopAndDestroy(iHdrColl);
	CleanupStack::PopAndDestroy(headerCodec);

	// Run tests on public APIs.  A real CHeaders will be created as part of the tests
	TestHeaderCollL();
	TestHeaderCollIterL();

	// Close the string pool
	CleanupStack::PopAndDestroy(); // iStrP

	// End of tests
	DisplayTimeElapsed();
}


void CHttpHdrTest::TestHdrValL()
	{
	// Open strings needed in this test
	RStringF accStr = iStrP.OpenFStringL(_L8("Accept"));

	THTTPHdrVal v1(accStr);
	TestL(CompareStringL(v1.StrF(), _L8("Accept")));
	TestL(v1.Type() == THTTPHdrVal::KStrFVal);

	THTTPHdrVal v2(3);
	TestL(v2.Int() == 3);
	TestL(v2.Type() == THTTPHdrVal::KTIntVal);


	TDateTime date(2001, EJanuary, 1, 12, 00, 00, 00);
	THTTPHdrVal v3(date);
	TestL(CompareDate(v3.DateTime(), date));
	TestL(v3.Type() == THTTPHdrVal::KDateVal);


	THTTPHdrVal v4 = accStr;
	TestL(v4.Type() == THTTPHdrVal::KStrFVal);


	THTTPHdrVal v5 = 3;
	TestL(v5.Type() == THTTPHdrVal::KTIntVal);

	
	THTTPHdrVal v6 = date;
	TestL(v6.Type() == THTTPHdrVal::KDateVal);

	RStringF str2 = v1;
	TestL(str2 == v1.StrF());

	
	TInt ii = v2;
	TestL(ii == v2.Int());


	TDateTime dt = v3;
	TestL(CompareDate(dt, date));


	v3 = str2;
	TestL(v3.Type() == THTTPHdrVal::KStrFVal);
	TestL(str2 == v3);

	v3 = 67;
	TestL(v3.Type() == THTTPHdrVal::KTIntVal);
	TestL(67 == v3);

	v3 = date;
	TestL(v3.Type() == THTTPHdrVal::KDateVal);
	TestL(CompareDate(date, v3.DateTime()));

	// Close strings used in this test
	accStr.Close();
	}


void CHttpHdrTest::TestBasicsL()
	{
	// Open strings needed in this test
	RStringF accStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());

	CHeaderField* hdr = CHeaderField::NewL(accStr, *iHdrColl);
	delete hdr;
	//
	CHeaderFieldPart* part = CHeaderFieldPart::NewL(THTTPHdrVal(textHtmlStr));
	delete part;
	//
	THTTPHdrVal::TQConv q(0.2);
	CHeaderFieldParam* param = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	delete param;
	}


void CHttpHdrTest::TestAddSinglePartLongL()
	{
	// Open strings needed in this test
	RStringF accStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF expiresStr = iStrP.StringF(HTTP::EExpires,RHTTPSession::GetTable());
	RStringF maxForwardsStr = iStrP.StringF(HTTP::EMaxForwards,RHTTPSession::GetTable());

	CHeaderFieldPart* headerPart = NULL;
	//
	//    Accept: text/html
	//
	CHeaderField* accept = CHeaderField::NewL(accStr, *iHdrColl);
	CleanupStack::PushL(accept);
	headerPart = CHeaderFieldPart::NewL(THTTPHdrVal(textHtmlStr));
	CleanupStack::PushL(headerPart);
	accept->AddPartL(headerPart); // string part added; ownership given to header
	CleanupStack::Pop(headerPart);
	CleanupStack::PopAndDestroy(accept);
	//
	//	  Expires: Mon, 15 Jan 2001 10:30:25 GMT
	//
	CHeaderField* expires = CHeaderField::NewL(expiresStr, *iHdrColl);
	CleanupStack::PushL(expires);
	headerPart = CHeaderFieldPart::NewL(THTTPHdrVal(TDateTime(2001, EJanuary, 15, 10, 30, 25, 0)));
	CleanupStack::PushL(headerPart);
	expires->AddPartL(headerPart); // date/time added; ownership given to header
	CleanupStack::Pop(headerPart);
	CleanupStack::PopAndDestroy(expires); 
	//
	//	  Max-Forwards: 3
	//
	CHeaderField* maxfwds = CHeaderField::NewL(maxForwardsStr, *iHdrColl);
	CleanupStack::PushL(maxfwds);
	headerPart = CHeaderFieldPart::NewL(3);
	CleanupStack::PushL(headerPart);
	maxfwds->AddPartL(headerPart); // integer part added; ownership given to header
	CleanupStack::Pop(headerPart);
	CleanupStack::PopAndDestroy(maxfwds);
	}


void CHttpHdrTest::TestAddSinglePartQuickL()
	{
	RStringF accStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF expiresStr = iStrP.StringF(HTTP::EExpires,RHTTPSession::GetTable());
	RStringF maxForwardsStr = iStrP.StringF(HTTP::EMaxForwards,RHTTPSession::GetTable());

	//
	//    Accept: text/html
	//
	// creates a single stringtoken part 
	CHeaderField* accept = CHeaderField::NewL(accStr, *iHdrColl, THTTPHdrVal(textHtmlStr));
	CleanupStack::PushL(accept);
	//
	//	  Expires: Mon, 15 Jan 2001 10:30:25 GMT
	//
	// creates a single date/time part, using an implicit case
	CHeaderField* expires = CHeaderField::NewL(expiresStr, *iHdrColl, TDateTime(2001, EJanuary, 15, 10, 30, 25, 0));
	CleanupStack::PushL(expires);
	//
	//	  Max-Forwards: 3
	//
	// creates a single integer part, using an implicit case
	CHeaderField* maxfwds = CHeaderField::NewL(maxForwardsStr, *iHdrColl, 3);
	CleanupStack::PushL(maxfwds);

	// Now just destroy that lot
	CleanupStack::PopAndDestroy(3);
	}


void CHttpHdrTest::TestAddMultiPartL()
	{
	// Open strings needed in this test
	RStringF accChSetStr = iStrP.StringF(HTTP::EAcceptCharset,RHTTPSession::GetTable());
	RStringF usAsciiStr = iStrP.OpenFStringL(_L8("us-ascii"));
	CleanupClosePushL(usAsciiStr);
	RStringF utf8Str = iStrP.OpenFStringL(_L8("utf-8"));
	CleanupClosePushL(utf8Str);

	RStringF accLangStr = iStrP.StringF(HTTP::EAcceptLanguage,RHTTPSession::GetTable());
	RStringF enGbStr = iStrP.OpenFStringL(_L8("en-gb"));
	CleanupClosePushL(enGbStr);
	RStringF frStr = iStrP.OpenFStringL(_L8("fr"));
	CleanupClosePushL(frStr);

	//
	//	  Accept-Charset: us-ascii, utf-8
	//
	CHeaderField* accChSet = CHeaderField::NewL(accChSetStr, *iHdrColl);
	CleanupStack::PushL(accChSet);

	// create first part ; ownership transferred

	CHeaderFieldPart* headerPart = CHeaderFieldPart::NewL(THTTPHdrVal(usAsciiStr));
	CleanupStack::PushL(headerPart);
	accChSet->AddPartL(headerPart);
	CleanupStack::Pop(headerPart);

    // create second part ; ownership transferred
	headerPart = CHeaderFieldPart::NewL(THTTPHdrVal(utf8Str));
	CleanupStack::PushL(headerPart);
	accChSet->AddPartL(headerPart);
	CleanupStack::Pop(headerPart);



	//	  Accept-Language: en-gb. fr
	//
	// quick method to get first part
	CHeaderField* accLang = CHeaderField::NewL(accLangStr, *iHdrColl, THTTPHdrVal(enGbStr));
	CleanupStack::PushL(accLang);

	// create second part ; ownership transferred
	headerPart = CHeaderFieldPart::NewL(THTTPHdrVal(frStr));
	CleanupStack::PushL(headerPart);
	accLang->AddPartL(headerPart);
	CleanupStack::Pop(headerPart);


	// Now just destroy that lot
	CleanupStack::PopAndDestroy(6);
	}


void CHttpHdrTest::TestAddParamL()
	{
	// Open strings needed in this test
	RStringF accStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF textPlainStr = iStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF anyAnyStr = iStrP.StringF(HTTP::EAnyAny,RHTTPSession::GetTable());
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());

	//
	//	  Accept: text/html; q=0.8, text/plain; q=0.2, */*
	//
	CHeaderField* accept = CHeaderField::NewL(accStr, *iHdrColl);
	CleanupStack::PushL(accept);
	//
	CHeaderFieldPart* htmlPt = CHeaderFieldPart::NewL(THTTPHdrVal(textHtmlStr));
	CleanupStack::PushL(htmlPt);
	THTTPHdrVal::TQConv q(0.8);

	CHeaderFieldParam* headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam); // now owned by htmlPt
	accept->AddPartL(htmlPt);
	CleanupStack::Pop(); // htmlPt - now owned by the header
	//
	CHeaderFieldPart* plainPt = CHeaderFieldPart::NewL(THTTPHdrVal(textPlainStr));
	CleanupStack::PushL(plainPt);
	headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam); // now owned by htmlPt
	accept->AddPartL(plainPt);
	CleanupStack::Pop(); // plainPt - now owned by the header
	//
	accept->AddPartL(CHeaderFieldPart::NewL(THTTPHdrVal(anyAnyStr)));

	// Now just destroy that lot
	CleanupStack::PopAndDestroy(); // accept
	}


void CHttpHdrTest::TestRemovePartL()
	{
	// Open strings needed in this test
	RStringF accStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF textPlainStr = iStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF anyAnyStr = iStrP.StringF(HTTP::EAnyAny,RHTTPSession::GetTable());
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());

	//
	//	  Accept: text/html; q=0.8, text/plain; q=0.2, */*
	//
	CHeaderField* accept = CHeaderField::NewL(accStr, *iHdrColl);
	CleanupStack::PushL(accept);
	//
	CHeaderFieldPart* htmlPt = CHeaderFieldPart::NewL(THTTPHdrVal(textHtmlStr));
	CleanupStack::PushL(htmlPt);
	THTTPHdrVal::TQConv q(0.2);

	CHeaderFieldParam* headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	accept->AddPartL(htmlPt);
	CleanupStack::Pop(htmlPt); // htmlPt - now owned by the header
	//
	CHeaderFieldPart* plainPt = CHeaderFieldPart::NewL(THTTPHdrVal(textPlainStr));
	CleanupStack::PushL(plainPt);
	headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	CleanupStack::PushL(headerParam);
	plainPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	accept->AddPartL(plainPt);
	CleanupStack::Pop(plainPt); // plainPt - now owned by the header
	//

	CHeaderFieldPart* headerPart = CHeaderFieldPart::NewL(anyAnyStr);
	CleanupStack::PushL(headerPart);
	accept->AddPartL(headerPart);
	CleanupStack::Pop(headerPart);

	// now remove the parts
	TestL(accept->RemovePartL(0) == KErrNone); // removes htmlPt
	TestL(accept->RemovePartL(1) == KErrNone); // removes '*/*' part...

	// Attempting to remove a non-existent part should fail somehow
	TestL(accept->RemovePartL(2) == KErrNotFound);

	// remove the final part
	TestL(accept->RemovePartL(0) == KErrNone); // removes plainPt

	// Now just destroy that lot
	CleanupStack::PopAndDestroy(); // accept

	}

void CHttpHdrTest::TestRemoveParamL()
	{
	// Open strings needed in this test
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());

	//
	CHeaderFieldPart* htmlPt = CHeaderFieldPart::NewL(THTTPHdrVal(textHtmlStr));
	CleanupStack::PushL(htmlPt);
	THTTPHdrVal::TQConv q(0.2);

	CHeaderFieldParam* headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	//
	// now remove the params
	CHeaderFieldParam* qP = htmlPt->Parameter(qStr);
	htmlPt->RemoveParam(qP);

	// Attempting to remove a non-existent parameter should fail somehow
	TestL(htmlPt->RemoveParam(qP) == KErrNotFound);

	// Now just destroy that lot
	CleanupStack::PopAndDestroy(htmlPt); 
	}

void CHttpHdrTest::TestLookupPartL()
	{
	// Open strings needed in this test
	RStringF accStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF textPlainStr = iStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF anyAnyStr = iStrP.StringF(HTTP::EAnyAny,RHTTPSession::GetTable());
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());

	//
	//	  Accept: text/html; q=0.8, text/plain; q=0.2, */*
	//
	CHeaderField* accept = CHeaderField::NewL(accStr, *iHdrColl);
	CleanupStack::PushL(accept);
	//
	CHeaderFieldPart* htmlPt = CHeaderFieldPart::NewL(THTTPHdrVal(textHtmlStr));
	CleanupStack::PushL(htmlPt);
	THTTPHdrVal::TQConv q(0.8);
	CHeaderFieldParam* headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	accept->AddPartL(htmlPt);
	CleanupStack::Pop(htmlPt); // now owned by the header
	//

	CHeaderFieldPart* plainPt = CHeaderFieldPart::NewL(THTTPHdrVal(textPlainStr));
	CleanupStack::PushL(plainPt);
	THTTPHdrVal::TQConv q2(0.2);
	headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q2));
	CleanupStack::PushL(headerParam);
	plainPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	accept->AddPartL(plainPt);
	CleanupStack::Pop(plainPt); // now owned by the header
	//
	CHeaderFieldPart* headerPart = CHeaderFieldPart::NewL(THTTPHdrVal(anyAnyStr));
	CleanupStack::PushL(headerPart);
	accept->AddPartL(headerPart);
	CleanupStack::Pop(headerPart);

	// now lookup the parts by index and check they are correct
	CHeaderFieldPart* pt = accept->PartL(0);
	TestL(pt == htmlPt);
	pt = accept->PartL(1);
	TestL(pt == plainPt);
	pt = accept->PartL(2);
	THTTPHdrVal val = pt->Value();
	TestL(val.Type() == THTTPHdrVal::KStrFVal);
	TestL(val.StrF().Index(RHTTPSession::GetTable()) == anyAnyStr.Index(RHTTPSession::GetTable()));	

	// Now just destroy that lot
	CleanupStack::PopAndDestroy(accept);
	}

void CHttpHdrTest::TestLookupParamL()
	{
	// Open strings needed in this test
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());
	RStringF textHtmlStr = iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());

	RStringF user1Str = iStrP.OpenFStringL(_L8("user1"));
	CleanupClosePushL(user1Str);
	RStringF value1Str = iStrP.OpenFStringL(_L8("value1"));
	CleanupClosePushL(value1Str);
	RStringF user2Str = iStrP.OpenFStringL(_L8("user2"));
	CleanupClosePushL(user2Str);
	RStringF value2Str = iStrP.OpenFStringL(_L8("value2"));
	CleanupClosePushL(value2Str);

	//
	CHeaderFieldPart* htmlPt = CHeaderFieldPart::NewL(THTTPHdrVal(textHtmlStr));
	CleanupStack::PushL(htmlPt);
	THTTPHdrVal::TQConv q(0.2);

	CHeaderFieldParam* headerParam = CHeaderFieldParam::NewL(qStr, THTTPHdrVal(q));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	
	headerParam = CHeaderFieldParam::NewL(user1Str, THTTPHdrVal(value1Str));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	
	headerParam = CHeaderFieldParam::NewL(user2Str, THTTPHdrVal(value2Str));
	CleanupStack::PushL(headerParam);
	htmlPt->AddParamL(headerParam);
	CleanupStack::Pop(headerParam);
	//
	// now lookup the params
	CHeaderFieldParam* qP = htmlPt->Parameter(qStr);
	RStringF qPstr = qP->Name();
	TestL(qPstr.Index(RHTTPSession::GetTable()) == qStr.Index(RHTTPSession::GetTable()));
	//
	qP = htmlPt->Parameter(user1Str);
	qPstr = qP->Name();
	TestL(qPstr.Index(RHTTPSession::GetTable()) == user1Str.Index(RHTTPSession::GetTable()));
	//
	qP = htmlPt->Parameter(user2Str);
	qPstr = qP->Name();

	TestL(qPstr.Index(RHTTPSession::GetTable()) == user2Str.Index(RHTTPSession::GetTable()));

	// Now just destroy that lot
	CleanupStack::PopAndDestroy(5); 
	}

void CHttpHdrTest::TestHeaderCollL()
	{
	// Open strings needed in this test

	RStringF userAgentStr = iStrP.StringF(HTTP::EUserAgent,RHTTPSession::GetTable());
	RStringF textPlainStr = iStrP.StringF(HTTP::ETextPlain,RHTTPSession::GetTable());
	RStringF qStr = iStrP.StringF(HTTP::EQ,RHTTPSession::GetTable());
	RStringF accStr = iStrP.StringF(HTTP::EAccept,RHTTPSession::GetTable());
	RStringF textHtmlStr = 	iStrP.StringF(HTTP::ETextHtml,RHTTPSession::GetTable());
	RStringF wmlStr = 	iStrP.StringF(HTTP::ETextVndWapWml,RHTTPSession::GetTable());
	RStringF symbGTWapStr = iStrP.OpenFStringL(_L8("SymbGTWap"));
	CleanupClosePushL(symbGTWapStr);

	//

	CTextModeHeaderCodec* headerCodec = CTextModeHeaderCodec::NewL(iStrP);
	CleanupStack::PushL(headerCodec);
	CHeaders* headers =  CHeaders::NewL(*headerCodec);
	CleanupStack::PushL(headers);
	RHTTPHeaders hdr = headers->Handle();
	
	THTTPHdrVal uaStr(symbGTWapStr);
	hdr.SetFieldL(userAgentStr, uaStr);

	THTTPHdrVal dummy;
	TestL(hdr.GetField(userAgentStr,0,dummy) == KErrNone);
	
	hdr.RemoveField(userAgentStr);

	TestL(hdr.GetField(userAgentStr,0,dummy) == KErrNotFound);

	THTTPHdrVal htmlStr(textHtmlStr);
	THTTPHdrVal q(THTTPHdrVal::TQConv(0.5));
	hdr.SetFieldL(accStr, htmlStr, qStr, q);
	
	THTTPHdrVal wml(wmlStr);
	THTTPHdrVal q2(THTTPHdrVal::TQConv(0.3));
	hdr.SetFieldL(accStr, wml, qStr, q2);

	THTTPHdrVal plainStr(textPlainStr);
	THTTPHdrVal q3(THTTPHdrVal::TQConv(0.1));
	hdr.SetFieldL(accStr, plainStr, qStr, q3);
	TestL(hdr.FieldPartsL(accStr) == 3);

	THTTPHdrVal v1;
	hdr.GetField(accStr,0,v1);
	TestL(v1.Type() == THTTPHdrVal::KStrFVal);
	TestL(v1.StrF().Index(RHTTPSession::GetTable()) == textHtmlStr.Index(RHTTPSession::GetTable()));
	THTTPHdrVal v2;
	TestL(!hdr.GetParam(accStr, qStr, v2));
	hdr.GetField(accStr, 1,v2);
	TestL(v1.Type() == THTTPHdrVal::KStrFVal);

	TestL(v2.StrF().Index(RHTTPSession::GetTable()) == wmlStr.Index(RHTTPSession::GetTable()));
	THTTPHdrVal v3;
	TestL(!hdr.GetParam(accStr, qStr, v3, 1));
	hdr.GetField(accStr, 2,v3);
	TestL(v3.Type() == THTTPHdrVal::KStrFVal);
	TestL(v3.StrF().Index(RHTTPSession::GetTable()) == textPlainStr.Index(RHTTPSession::GetTable()));
	TestL(!hdr.GetParam(accStr, qStr, v3, 2));

	CleanupStack::PopAndDestroy(3); 
	}

void CHttpHdrTest::TestHeaderCollIterL()
	{
	RStringF field1Str = iStrP.OpenFStringL(_L8("hcfield1"));
	CleanupClosePushL(field1Str);
	RStringF value1Str = iStrP.OpenFStringL(_L8("hcvalue1"));
	CleanupClosePushL(value1Str);
	RStringF field2Str = iStrP.OpenFStringL(_L8("hcfield2"));
	CleanupClosePushL(field2Str);
	RStringF value2Str = iStrP.OpenFStringL(_L8("hcvalue2"));
	CleanupClosePushL(value2Str);
	RStringF field3Str = iStrP.OpenFStringL(_L8("hcfield3"));
	CleanupClosePushL(field3Str);
	RStringF value3Str = iStrP.OpenFStringL(_L8("hcvalue3"));
	CleanupClosePushL(value3Str);
	RStringF field4Str = iStrP.OpenFStringL(_L8("hcfield4"));
	CleanupClosePushL(field4Str);
	RStringF value4Str = iStrP.OpenFStringL(_L8("hcvalue4"));
	CleanupClosePushL(value4Str);
	//

	CTextModeHeaderCodec* headerCodec = CTextModeHeaderCodec::NewL(iStrP);
	CleanupStack::PushL(headerCodec);
	CHeaders* headers = CHeaders::NewL(*headerCodec);
	CleanupStack::PushL(headers);
	RHTTPHeaders hdr = headers->Handle();

	THTTPHdrVal str1(value1Str);
	hdr.SetFieldL(field1Str, str1);
	THTTPHdrVal str2(value2Str);
	hdr.SetFieldL(field2Str, str2);
	THTTPHdrVal str3(value3Str);
	hdr.SetFieldL(field3Str, str3);
	THTTPHdrVal str4(value4Str);
	hdr.SetFieldL(field4Str, str4);

	THTTPHdrFieldIter it = hdr.Fields();
	TInt count = 0;
	while (it.AtEnd() == EFalse)
		{
		RStringTokenF nextTk = it();
		RStringF nextStr = iStrP.StringF(nextTk);
		THTTPHdrVal hVal;
		TestL(hdr.GetField(nextStr,0,hVal)==KErrNone);
		switch (count)
			{
			case 0:
				TestL(nextStr.Index(RHTTPSession::GetTable()) == field1Str.Index(RHTTPSession::GetTable()));
				break;
			case 1:
				TestL(nextStr.Index(RHTTPSession::GetTable()) == field2Str.Index(RHTTPSession::GetTable()));
				break;
			case 2:
				TestL(nextStr.Index(RHTTPSession::GetTable()) == field3Str.Index(RHTTPSession::GetTable()));
				break;
			case 3:
					TestL(nextStr.Index(RHTTPSession::GetTable()) == field4Str.Index(RHTTPSession::GetTable()));
				break;
			}
		++it;
		++count;
		}

	// Close strings used in this test
	CleanupStack::PopAndDestroy(10);
	}

TBool CHttpHdrTest::CompareStringL(RStringF aStr, const TDesC8& aDes)
	{
	RStringF desStr = iStrP.OpenFStringL(aDes);
	TBool retVal = (desStr == aStr);
	desStr.Close();
	return retVal;
	}

TBool CHttpHdrTest::CompareStringTokenL(RStringTokenF aStrTk, const TDesC8& aDes)
	{
	RStringF desStr = iStrP.OpenFStringL(aDes);
	RStringTokenF desStrTk = desStr;
	TBool retVal = (desStrTk == aStrTk);
	desStr.Close();
	return retVal;
	}

TBool CHttpHdrTest::CompareDate(TDateTime aDate1, TDateTime aDate2)
	{
	return ((aDate1.Year() == aDate2.Year()) &&
			(aDate1.Month() == aDate2.Month()) &&
			(aDate1.Day() == aDate2.Day()) &&
			(aDate1.Hour() == aDate2.Hour()) &&
			(aDate1.Minute() == aDate2.Minute()) &&
			(aDate1.Second() == aDate2.Second()) &&
			(aDate1.MicroSecond() == aDate2.MicroSecond()));
	}


void CHttpHdrTest::ResetTimeElapsed()
// Resets timestamp to time now
	{
	iLastTimeStamp.UniversalTime(); 
	}


void CHttpHdrTest::DisplayTimeElapsed()
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







	

