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

#include <http.h>

// eg.s of Header usage.
//

// Throughout:  iStrTb is a handle to the HTTP string table;
//  i.e.   RStringPool iStrTb;
//
// Strings are represented as handles to HTTP strings;
//  i.e.   RStringF str;
//
// String table lookup (i.e. getting the string handle from some descriptor) is done as so:
//
//  RStringF str = iStrTb.OpenFStringL(_L8("my string"));
//
// Strings which are 'very well known' (e.g. those in the HTTP RFC) and therefore certainly expected to be in ROM
// can be converted to HTTP string handles using enumerations:
//
//  RStringF str = iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()); // guaranteed to give the same handle as:
//  RHttpString str = iStrTb.OpenFStringL(_L8("KHttpAccept"));



// 1. Client API creation of simple headers
//	  
//    Accept: text/html
//	  Expires: Mon, 15 Jan 2001 10:30:25 GMT
//	  Location: http://www.symbian.com/
//	  Max-Forwards: 3
//
void Example1(RHTTPRequest aReq)
	{
	RHTTPHeaders hdr = aReq.GetHeaderCollection();

	RStringF textHtml = iStrTb.OpenFStringL(_L8("text/html"));
	THTTPHdrVal accVal(textHtml);
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal);
	textHtml.Close();

	TDateTime dateVal(2001, EJanuary, 14, 10, 30, 25, 0);
	THTTPHdrVal expVal(dateVal);
	hdr.SetFieldL(iStrTb.String(HTTP::EExpires,RHTTPSession::GetTable()), expVal);

	RStringF symbUrl = iStrTb.OpenFStringL(_L8("http://www.symbian.com/"));
	THTTPHdrVal locVal(symbUrl);
	hdr.SetFieldL(iStrTb.String(HTTP::ELocation,RHTTPSession::GetTable()), locVal);
	symbUrl.Close();

	THTTPHdrVal maxFwdsVal(3);
	hdr.SetFieldL(iStrTb.String(HTTP::EMaxForwards,RHTTPSession::GetTable()), maxFwdsVal);
	}


// 2. Client API creation of headers with several parts
//
//	  Accept-Charset: us-ascii, utf-8
//	  Accept-Language: en-gb. fr
//
void Example2(RHTTPRequest aReq)
	{
	RHTTPHeaders hdr = aReq.GetHeaderCollection();

	RStringF usAscii = iStrTb.OpenFStringL(_L8("us-ascii"));
	RStringF utf8    = iStrTb.OpenFStringL(_L8("utf-8"));
	THTTPHdrVal accChSetVal(usAscii);
	hdr.SetFieldL(iStrTb.String(HTTP::EAcceptCharset,RHTTPSession::GetTable()), accChSetVal); // sets the first part
	accChSetVal.SetStrutf8);
	hdr.SetFieldL(iStrTb.String(HTTP::EAcceptCharset,RHTTPSession::GetTable()), accChSetVal); // adds an additional part
	usAscii.Close();
	utf8.Close();

	RStringF enGb = iStrTb.OpenFStringL(_L8("en-gb"));
	RStringF fr   = iStrTb.OpenFStringL(_L8("fr"));
	THTTPHdrVal accLangVal(enGb);
	hdr.SetFieldL(iStrTb.String(HTTP::EAcceptLanguage,RHTTPSession::GetTable()), accLangVal);
	accLangVal.SetStrfr);
	hdr.SetFieldL(iStrTb.String(HTTP::EAcceptLanguage,RHTTPSession::GetTable()), accLangVal);
	enGb.Close();
	fr.Close();
	}


// 3. Client API creation of headers with parameters
//
//	  Accept: text/html; q=0.8, text/plain; q=0.2, */*
//
void Example3(RHTTPRequest aReq)
	{
	RHTTPHeaders hdr = aReq.GetHeaderCollection();

	RStringF textHtml  = iStrTb.OpenFStringL(_L8("text/html"));
	RStringF textPlain = iStrTb.OpenFStringL(_L8("text/plain"));
	RStringF anyAny	  = iStrTb.OpenFStringL(_L8("*/*"));
	THTTPHdrVal accVal(textHtml);
	THTTPHdrVal q(THTTPHdrVal::TQConv(0.8));
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal, iStrTb.String(HTTP::EQ,RHTTPSession::GetTable()), q);
	accVal.SetStrtextPlain);
	q.SetInt(THTTPHdrVal::TQConv(0.2));
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal, iStrTb.String(HTTP::EQ,RHTTPSession::GetTable()), q);
	accVal.SetStranyAny);
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal);
	textHtml.Close();
	textPlain.Close();
	anyAny.Close();
	}
