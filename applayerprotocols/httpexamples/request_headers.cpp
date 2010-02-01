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

// eg.s of forming request headers. Each request header handled by HTTP is listed along with RFC refereces to
// show the allowed syntax.
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



// RFC2616, section 14.1
// Accept         = "Accept" ":"
//                    #( media-range [ accept-params ] )
//   media-range    = ( "*/*"
//                    | ( type "/" "*" )
//                    | ( type "/" subtype )
//                    ) *( ";" parameter )
//   accept-params  = ";" "q" "=" qvalue *( accept-extension )
//   accept-extension = ";" token [ "=" ( token | quoted-string ) ]
//
void AcceptExample(RHTTPRequest aReq)
	{
	RHTTPHeaders hdr = aReq.GetHeaderCollection();

	// Setting a single media range with no parameter, e.g. Accept: text/html
	RStringF textHtml = iStrTb.OpenFStringL(_L8("text/html"));
	THTTPHdrVal accVal(textHtml);
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal);
	textHtml.Close();

	// Setting several media ranges with no parameters, e.g. Accept: text/html; text/vnd.wap.wml
	RStringF textHtml = iStrTb.OpenFStringL(_L8("text/html"));
	RStringF textWml  = iStrTb.OpenFStringL(_L8("text/vnd.wap.wml"));
	THTTPHdrVal accVal(textHtml);
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal);
	accVal.SetStrtextWml);
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal);
	textHtml.Close();

	// Setting a media range with a 'q' parameter,  Accept: text/html; q=0.8
	RStringF textHtml  = iStrTb.OpenFStringL(_L8("text/html"));
	THTTPHdrVal accVal(textHtml);
	THTTPHdrVal q(THTTPHdrVal::TQConv(0.8));
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal, iStrTb.String(HTTP::EQ,RHTTPSession::GetTable()), q);
	textHtml.Close();

	// Using an accept extension,  Accept: text/html; extended=value
	RStringF textHtml  = iStrTb.OpenFStringL(_L8("text/html"));
	RStringF extended  = iStrTb.OpenFStringL(_L8("extended"));
	RStringF extendVal = iStrTb.OpenFStringL(_L8("value"));
	THTTPHdrVal accVal(textHtml);
	THTTPHdrVal extVal(extendVal);
	hdr.SetFieldL(iStrTb.String(HTTP::EAccept,RHTTPSession::GetTable()), accVal, extended, extVal);
	textHtml.Close();
	extended.Close();
	extendVal.Close();
	}


// RFC2616, section 14.8;  RFC2617 
// Authorization  = "Authorization" ":" credentials
// credentials = auth-scheme #auth-param
// credentials = "Basic" basic-credentials
// basic-credentials = base64-user-pass
// base64-user-pass  = <base64 [4] encoding of user-pass, except not limited to 76 char/line>
// user-pass   = userid ":" password
// userid      = *<TEXT excluding ":">
// password    = *TEXT
void AuthorizationBasicExample(RHTTPRequest aReq)
	{
	RHTTPHeaders hdr = aReq.GetHeaderCollection();

	// Setting an authorization credential, e.g. Authorization: Basic c3ltYmlhbjpmMXN5bmNtbA==
	RStringF basicCred = iStrTb.OpenFStringL(_L8("c3ltYmlhbjpmMXN5bmNtbA=="));
	THTTPHdrVal authVal(iStrTb.String(HTTP::EBasic,RHTTPSession::GetTable()));
	hdr.SetFieldL(iStrTb.String(HTTP::EAuthorization,RHTTPSession::GetTable()), authVal); // sets part 1 'Basic'
	authVal.SetStrbasicCred);
	hdr.SetFieldL(iStrTb.String(HTTP::EAuthorization,RHTTPSession::GetTable()), authVal); // sets part 2 'c3ltYmlhbjpmMXN5bmNtbA'
	basicCred.Close();
	}


// RFC2616, section 14.13
// Content-Length    = "Content-Length" ":" 1*DIGIT
void ContentLengthExample(RHTTPRequest aReq)
	{
	RHTTPHeaders hdr = aReq.GetHeaderCollection();

	// Setting Content-Length header, e.g. Content-Length: 12345
	THTTPHdrVal lenVal(12345);
	hdr.SetFieldL(iStrTb.String(HTTP::EContentLength, lenVal);
	}


// RFC2616, section 14.23
// Host = "Host" ":" host [ ":" port ] ; Section 3.2.2
void HostExample(RHTTPRequest aReq)
	{
	RHTTPHeaders hdr = aReq.GetHeaderCollection();

	// Setting host header, e.g. Host: www.symbian.com
	RStringF domain = iStrTb.OpenFStringL(_L8("www.symbian.com"));
	THTTPHdrVal domVal(domain);
	hdr.SetFieldL(iStrTb.String(HTTP::EHost, domVal);
	domain.Close();

	// Setting a host header with a specific port number, e.g. Host: www.symbian.com:80
	RStringF domain = iStrTb.OpenFStringL(_L8("www.symbian.com"));
	THTTPHdrVal domVal(domain);
	THTTPHdrVal portParam(80);
	// A string constant HTTP::EPort will be used to identify the parameter
	hdr.SetFieldL(iStrTb.String(HTTP::EHost, domVal, EHTTPPort, portParam);
	domain.Close();
	}
