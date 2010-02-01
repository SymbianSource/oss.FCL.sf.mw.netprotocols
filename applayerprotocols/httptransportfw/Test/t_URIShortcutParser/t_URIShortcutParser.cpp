// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// t_URIShortcutParserr.cpp
// 
//

#include <e32test.h>
#include <ecom/ecom.h>
#include "turishortcutparser8.h"
#include <Uri8.h>


_LIT( KTestName,"UriShortcutParser Unit Tests" );
LOCAL_D RTest test( KTestName );

typedef void ( *TTestFunctionL ) ();


void DoOOMTest( TTestFunctionL aDoTestFunctionL )
	{
    test.Printf( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0001 Doing OOM testing" ) );
	TInt err = 1;
	TInt count = 0;

	while( err )
		{
		__UHEAP_MARK;
		__UHEAP_SETFAIL( RHeap::EFailNext, count ); // This is a Macro, not leaving function
		TRAPD( error, ( aDoTestFunctionL )() );
		err = error;
		__UHEAP_MARKEND;
		++count;
		}
    __UHEAP_RESET;
    test.Printf( _L( "End of OOM testing" ) );
	}
	
void TestOOMLocalhostL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0002 Testing localhost - small letters" ) );
    theResult = parser.CreateUriL( _L8( "localhost" ) );
    test( theResult->Uri().UriDes().Compare( _L8( "http://localhost" ) ) == 0 );
    delete theResult;
        
    theResult = NULL;
    test.Next( _L( "Testing localhost - CAPITAL LETTERS " ) );
    theResult = parser.CreateUriL( _L8( "LOCALHOST" ) );
    test( theResult->Uri().UriDes().Compare( _L8( "http://LOCALHOST" ) ) == 0 );
    delete theResult;

    theResult = NULL;
    test.Next( _L( "Testing localhost - mIX OF SMALL and cAPITAL LEtters" ) );
    theResult = parser.CreateUriL( _L8( "LOCALhost" ) );
    test( theResult->Uri().UriDes().Compare( _L8( "http://LOCALhost" ) ) == 0 );
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - with path"));
    theResult = parser.CreateUriL(_L8("localhost/index.html"));
    test(theResult->Uri().UriDes().Compare(_L8("http://localhost/index.html"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - with scheme"));
    theResult = parser.CreateUriL(_L8("http://localhost"));
    test(theResult->Uri().UriDes().Compare(_L8("http://localhost"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - with scheme and path"));
    theResult = parser.CreateUriL(_L8("http://localhost/index.html"));
    test(theResult->Uri().UriDes().Compare(_L8("http://localhost/index.html"))==0);
    delete theResult;
    
    }

void TestLocalhostL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0003 Testing localhost - small letters"));
    TRAP(err, theResult = parser.CreateUriL(_L8("localhost")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://localhost"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - CAPITAL LETTERS "));
    TRAP(err, theResult = parser.CreateUriL(_L8("LOCALHOST")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://LOCALHOST"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - mIX OF SMALL and cAPITAL LEtters"));
    TRAP(err, theResult = parser.CreateUriL(_L8("LOCALhost")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://LOCALhost"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - with path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("localhost/index.html")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://localhost/index.html"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - with scheme"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://localhost")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://localhost"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing localhost - with scheme and path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://localhost/index.html")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://localhost/index.html"))==0);
    delete theResult;
    
    }
void TestOOMOneComponentL()
    {
    
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0004 Testing One Component Hostname"));
    theResult = parser.CreateUriL(_L8("testpage"));
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage.com"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with path"));
    theResult = parser.CreateUriL(_L8("testpage/index"));
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage/index"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with scheme"));
    theResult = parser.CreateUriL(_L8("http://testpage"));
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with scheme and path"));
    theResult = parser.CreateUriL(_L8("http://testpage/index.html"));
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage/index.html"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with scheme (CAPITAL LETTERS) and path"));
    theResult = parser.CreateUriL(_L8("HTTP://testpage/index.html"));
    test(theResult->Uri().UriDes().Compare(_L8("HTTP://testpage/index.html"))==0);
    delete theResult;
    
    }

void TestOneComponentL()
    {
    
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0005 Testing One Component Hostname"));
    TRAP(err, theResult = parser.CreateUriL(_L8("testpage")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage.com"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("testpage/index")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage/index"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with scheme"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://testpage")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with scheme and path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://testpage/index.html")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage/index.html"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing One Component Hostname - one component with scheme (CAPITAL LETTERS) and path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("HTTP://testpage/index.html")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("HTTP://testpage/index.html"))==0);
    delete theResult;
    
    }
    
void TestTwoComponentsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0006 Testing Two Components Hostname"));
    TRAP(err, theResult = parser.CreateUriL(_L8("testpage.com")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname1"));
    TRAP(err, theResult = parser.CreateUriL(_L8("www.testpage")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname - with path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("testpage.com/index.htm")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com/index.htm"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname - with scheme"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://testpage.com")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname - with scheme and path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://testpage.com/index")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com/index"))==0);
    delete theResult;
    
    }
    
void TestOOMTwoComponentsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0007 Testing Two Components Hostname"));
    theResult = parser.CreateUriL(_L8("testpage.com"));
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname1"));
    theResult = parser.CreateUriL(_L8("www.testpage"));
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname - with path"));
    theResult = parser.CreateUriL(_L8("testpage.com/index.htm"));
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com/index.htm"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname - with scheme"));
    theResult = parser.CreateUriL(_L8("http://testpage.com"));
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing Two Components Hostname - with scheme and path"));
    theResult = parser.CreateUriL(_L8("http://testpage.com/index"));
    test(theResult->Uri().UriDes().Compare(_L8("http://testpage.com/index"))==0);
    delete theResult;
    
    }

void TestThreeComponentsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0008 Testing Three Components Hostname"));
    TRAP(err, theResult = parser.CreateUriL(_L8("www.testpage.com")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage.com"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Three Components Hostname - with path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("www.testpage.com/index.html")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage.com/index.html"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Three Components Hostname - with caps"));
    TRAP(err, theResult = parser.CreateUriL(_L8("WWW.testpage.COM")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://WWW.testpage.COM"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Three Components Hostname - with trailing slash"));
    TRAP(err, theResult = parser.CreateUriL(_L8("WWW.testpage.COM/")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://WWW.testpage.COM/"))==0);
    delete theResult;
    
    }

void TestOOMThreeComponentsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0009 Testing Three Components Hostname"));
    theResult = parser.CreateUriL(_L8("www.testpage.com"));
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage.com"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Three Components Hostname - with path"));
    theResult = parser.CreateUriL(_L8("www.testpage.com/index.html"));
    test(theResult->Uri().UriDes().Compare(_L8("http://www.testpage.com/index.html"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Three Components Hostname - with caps"));
    theResult = parser.CreateUriL(_L8("WWW.testpage.COM"));
    test(theResult->Uri().UriDes().Compare(_L8("http://WWW.testpage.COM"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Three Components Hostname - with trailing slash"));
    theResult = parser.CreateUriL(_L8("WWW.testpage.COM/"));
    test(theResult->Uri().UriDes().Compare(_L8("http://WWW.testpage.COM/"))==0);
    delete theResult;
    
    }

void TestIPV4HostL()
    {
    
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0010 Testing IPv4 Address"));
    TRAP(err, theResult = parser.CreateUriL(_L8("192.168.202.96")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing IPv4 Address - with path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("192.168.202.96/index")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96/index"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing IPv4 Address - with scheme"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://192.168.202.96")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing IPv4 Address - with scheme and path"));
    TRAP(err, theResult = parser.CreateUriL(_L8("http://192.168.202.96/path")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96/path"))==0);
    delete theResult;
    
    }

void TestOOMIPV4HostL()
    {
    
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0011 Testing IPv4 Address"));
    theResult = parser.CreateUriL(_L8("192.168.202.96"));
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing IPv4 Address - with path"));
    theResult = parser.CreateUriL(_L8("192.168.202.96/index"));
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96/index"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing IPv4 Address - with scheme"));
    theResult = parser.CreateUriL(_L8("http://192.168.202.96"));
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing IPv4 Address - with scheme and path"));
    theResult = parser.CreateUriL(_L8("http://192.168.202.96/path"));
    test(theResult->Uri().UriDes().Compare(_L8("http://192.168.202.96/path"))==0);
    delete theResult;
    
    }

void TestIPV6HostL()
    {
    
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0012 Testing IPV6 Address - IPv6"));
    TRAP(err, theResult = parser.CreateUriL(_L8("[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing IPV6 Address - IPv6 with scheme"));
    TRAP(err, theResult = parser.CreateUriL(_L8("nntp://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("nntp://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing IPV6 Address - Mix of IPv4 and IPv6"));
    TRAP(err, theResult = parser.CreateUriL(_L8("[FEC0::1111:0:5EFE:131.107.129.8]")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://[FEC0::1111:0:5EFE:131.107.129.8]"))==0);
    delete theResult;
    
    }
    
void TestOOMIPV6HostL()
    {
    
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0013 Testing IPV6 Address - IPv6"));
    theResult = parser.CreateUriL(_L8("[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]"));
    test(theResult->Uri().UriDes().Compare(_L8("http://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing IPV6 Address - IPv6 with scheme"));
    theResult = parser.CreateUriL(_L8("nntp://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]"));
    test(theResult->Uri().UriDes().Compare(_L8("nntp://[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]"))==0);
    delete theResult;

    theResult = NULL;
    test.Next(_L("Testing IPV6 Address - Mix of IPv4 and IPv6"));
    theResult = parser.CreateUriL(_L8("[FEC0::1111:0:5EFE:131.107.129.8]"));
    test(theResult->Uri().UriDes().Compare(_L8("http://[FEC0::1111:0:5EFE:131.107.129.8]"))==0);
    delete theResult;
    
    }

void TestInvalidInputsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0014 Testing Invalid Input - empty"));
    TRAP(err, theResult = parser.CreateUriL(_L8("")));
    test(err != KErrNone);

    test.Next(_L("Testing Invalid Input - space"));
    TRAP(err, theResult = parser.CreateUriL(_L8(" ")));
    test(err != KErrNone);
    
    test.Next(_L("Testing Invalid Input - invalid chars"));
    TRAP(err, theResult = parser.CreateUriL(_L8("{~!~}")));
    test(err != KErrNone);
    test(theResult == NULL);
    }
    

void TestLengthyInputsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0015 Testing Lengthy Input - lengthy host name"));
    TRAP(err, theResult = parser.CreateUriL(_L8("www.mypage.longstringasdfsafasfsafdasfasfdslongstringasdfsafasfsafdasfasfds.co.in")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://www.mypage.longstringasdfsafasfsafdasfasfdslongstringasdfsafasfsafdasfasfds.co.in"))==0);
    delete theResult;
    }
    
void TestOOMLengthyInputsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0016 Testing Lengthy Input - lengthy host name"));
    theResult = parser.CreateUriL(_L8("www.mypage.longstringasdfsafasfsafdasfasfdslongstringasdfsafasfsafdasfasfds.co.in"));
    test(theResult->Uri().UriDes().Compare(_L8("http://www.mypage.longstringasdfsafasfsafdasfasfdslongstringasdfsafasfsafdasfasfds.co.in"))==0);
    delete theResult;
    }
    
void TestVariousSchemesL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    theResult = NULL;
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0017 Testing Various Schemes - gopher - before calling function SetCheckUncommonSchemes()"));
    TRAP(err, theResult = parser.CreateUriL(_L8("gopher.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://gopher.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    parser.SetCheckUncommonSchemes(ETrue);
    test.Next(_L("Testing Various Schemes - gopher - after calling function SetCheckUncommonSchemes(ETrue)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("gopher.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("gopher://gopher.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - ftp - after calling function SetCheckUncommonSchemes(ETrue)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("ftp.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("ftp://ftp.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - nntp - after calling function SetCheckUncommonSchemes(ETrue)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("nntp.mypage.co.in")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("nntp://nntp.mypage.co.in"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - nntp - after calling function SetCheckUncommonSchemes(ETrue)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("news.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("nntp://news.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - telnet - after calling function SetCheckUncommonSchemes(ETrue)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("telnet.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("telnet://telnet.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - prospero - after calling function SetCheckUncommonSchemes(ETrue)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("prospero.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("prospero://prospero.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - wais - after calling function SetCheckUncommonSchemes(ETrue)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("wais.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("wais://wais.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    parser.SetCheckUncommonSchemes(EFalse);
    test.Next(_L("Testing Various Schemes - wais - after calling function SetCheckUncommonSchemes(EFalse)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("wais.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://wais.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - wais - after calling function SetCheckUncommonSchemes(EFalse)"));
    TRAP(err, theResult = parser.CreateUriL(_L8("ftp.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("ftp://ftp.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - unknown scheme"));
    TRAP(err, theResult = parser.CreateUriL(_L8("unknown.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://unknown.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    _LIT8(KScheme,"unknown");
    test.Next(_L("Testing Various Schemes - change default scheme"));
    parser.SetDefaultScheme(KScheme());
    TRAP(err, theResult = parser.CreateUriL(_L8("unknown.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("unknown://unknown.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - test unknown"));
    TRAP(err, theResult = parser.CreateUriL(_L8("abc.mypage.co.uk")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("unknown://abc.mypage.co.uk"))==0);
    delete theResult;
    
    }
    
void TestOOMVariousSchemesL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    theResult = NULL;
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0018 Testing Various Schemes - gopher - before calling function SetCheckUncommonSchemes()"));
    theResult = parser.CreateUriL(_L8("gopher.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("http://gopher.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    parser.SetCheckUncommonSchemes(ETrue);
    test.Next(_L("Testing Various Schemes - gopher - after calling function SetCheckUncommonSchemes(ETrue)"));
    theResult = parser.CreateUriL(_L8("gopher.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("gopher://gopher.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - ftp - after calling function SetCheckUncommonSchemes(ETrue)"));
    theResult = parser.CreateUriL(_L8("ftp.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("ftp://ftp.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - nntp - after calling function SetCheckUncommonSchemes(ETrue)"));
    theResult = parser.CreateUriL(_L8("nntp.mypage.co.in"));
    test(theResult->Uri().UriDes().Compare(_L8("nntp://nntp.mypage.co.in"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - nntp - after calling function SetCheckUncommonSchemes(ETrue)"));
    theResult = parser.CreateUriL(_L8("news.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("nntp://news.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - telnet - after calling function SetCheckUncommonSchemes(ETrue)"));
    theResult = parser.CreateUriL(_L8("telnet.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("telnet://telnet.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - prospero - after calling function SetCheckUncommonSchemes(ETrue)"));
    theResult = parser.CreateUriL(_L8("prospero.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("prospero://prospero.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - wais - after calling function SetCheckUncommonSchemes(ETrue)"));
    theResult = parser.CreateUriL(_L8("wais.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("wais://wais.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    parser.SetCheckUncommonSchemes(EFalse);
    test.Next(_L("Testing Various Schemes - wais - after calling function SetCheckUncommonSchemes(EFalse)"));
    theResult = parser.CreateUriL(_L8("wais.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("http://wais.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - wais - after calling function SetCheckUncommonSchemes(EFalse)"));
    theResult = parser.CreateUriL(_L8("ftp.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("ftp://ftp.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - unknown scheme"));
    theResult = parser.CreateUriL(_L8("unknown.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("http://unknown.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    _LIT8(KScheme,"unknown");
    test.Next(_L("Testing Various Schemes - change default scheme"));
    parser.SetDefaultScheme(KScheme());
    theResult = parser.CreateUriL(_L8("unknown.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("unknown://unknown.mypage.co.uk"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Various Schemes - test unknown"));
    theResult = parser.CreateUriL(_L8("abc.mypage.co.uk"));
    test(theResult->Uri().UriDes().Compare(_L8("unknown://abc.mypage.co.uk"))==0);
    delete theResult;
    
    }    
    
void TestHierarchicalComponentsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    TInt err;
    
    theResult = NULL;
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0019 Testing Hierarchical Components - ?"));
    TRAP(err, theResult = parser.CreateUriL(_L8("example?foo=bar")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://example?foo=bar"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Hierarchical Components - /"));
    TRAP(err, theResult = parser.CreateUriL(_L8("example/index.html")));
    test(err == KErrNone);
    test(theResult->Uri().UriDes().Compare(_L8("http://example/index.html"))==0);
    delete theResult;
    
    }
    
void TestOOMHierarchicalComponentsL()
    {
    TUriShortcutParser8 parser;
    CUri8* theResult = NULL;
    
    theResult = NULL;
    test.Next(_L("@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-PARSER-0020 Testing Hierarchical Components - ?"));
    theResult = parser.CreateUriL(_L8("example?foo=bar"));
    test(theResult->Uri().UriDes().Compare(_L8("http://example?foo=bar"))==0);
    delete theResult;
    
    theResult = NULL;
    test.Next(_L("Testing Hierarchical Components - /"));
    theResult = parser.CreateUriL(_L8("example/index.html"));
    test(theResult->Uri().UriDes().Compare(_L8("http://example/index.html"))==0);
    delete theResult;
    
    }
    
    
GLDEF_C void DoTestsL()
	{
	TestLocalhostL();
	DoOOMTest(TestOOMLocalhostL);
	
	TestOneComponentL();
	DoOOMTest(TestOOMOneComponentL);
	
	TestTwoComponentsL();
	DoOOMTest(TestOOMTwoComponentsL);
	
	TestThreeComponentsL();
	DoOOMTest(TestOOMThreeComponentsL);
	
	TestIPV4HostL();
	DoOOMTest(TestOOMIPV4HostL);

	TestIPV6HostL();
	DoOOMTest(TestOOMIPV6HostL);
	
	TestInvalidInputsL();
	
	TestLengthyInputsL();
	DoOOMTest(TestOOMLengthyInputsL);

	TestVariousSchemesL();
	DoOOMTest(TestOOMVariousSchemesL);
	
	TestHierarchicalComponentsL();
	DoOOMTest(TestOOMHierarchicalComponentsL);
	
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* tc=CTrapCleanup::New();
	test.Start(_L("Starting unit tests"));
	TRAPD(err,DoTestsL());
	test(err==KErrNone);
	test.End();
	test.Close();
	delete tc;
	
	__UHEAP_MARKEND;
	
	return KErrNone;
	}
