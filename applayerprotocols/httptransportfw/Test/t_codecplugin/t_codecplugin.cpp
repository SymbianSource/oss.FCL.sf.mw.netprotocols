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
//

#include <e32test.h>
#include <ecom/ecom.h>
#include <http/framework/cheadercodecplugin.h>
#include <http/rhttpsession.h>
#include <httpstringconstants.h>


_LIT( KTestName,"Codec plugin chaining unit test" );
LOCAL_D RTest test( KTestName );


/** 
@SYMTestCaseID HTTP-CODEC-001
@SYMTestCaseDesc Test Code Chaining
@SymFssID Application-Protocols-HTTP-19.1
@SymREQ REQ 4878.5
*/ 
void TestCodecChaining1L()
{
	_LIT8( KHttpClientCodecName, "HTTP/client" );

	RStringPool	strP;
	CleanupClosePushL( strP );
	strP.OpenL( RHTTPSession::GetTable() );
	
	CHeaderCodec* codec = CHeaderCodecPlugin::NewL( KHttpClientCodecName,strP );
	CleanupStack::PushL( codec );

	//make sure client codec has loaded
	test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-CODEC-0001 Test to check whether client codec has loaded" ) );
	RStringF accept = strP.StringF(HTTP::EAccept,RHTTPSession::GetTable());//this is a client header
	test( 	codec->CanEncode(accept) );

	//make sure client codec can't encode general headers
	test.Next( _L( "Test to check the loaded codec can't encode general headers" ) );
	RStringF cachecontrol = strP.StringF( HTTP::ECacheControl, RHTTPSession::GetTable( ) );//this is a general header
	test( 	!codec->CanEncode( cachecontrol ) );


	//load the delegate codec to encode the above header and make sure its general codec
	test.Next( _L( "Load the delegate codec to encode the above header and make sure its general codec" ) );
	CHeaderCodec* codec2 = codec->FindDelegateCodecL( cachecontrol );
	CleanupStack::PushL( codec2 );
	test( codec2->CanEncode( cachecontrol ) );
	
	//make sure general codec can't encode unknown headers
	test.Next( _L( "Test to check that the eneral codec can't encode unknown headers" ) );
	RStringF extended  = strP.OpenFStringL( _L8( "extended" ) );
	CleanupClosePushL( extended );
	test( !codec2->CanEncode( extended ) );
	
	
	//load the delegate codec to encode the above unknown header and make sure its default codec
	test.Next( _L( "Test to load the delegate codec to encode the above unknown header and make sure its default codec" ) );
	CHeaderCodec* codec3 = codec2->FindDelegateCodecL( extended );
	CleanupStack::PushL( codec3 );
	test( codec3->CanEncode( extended ) );

	
	CleanupStack::PopAndDestroy( codec3 );
	CleanupStack::PopAndDestroy( &extended );
	CleanupStack::PopAndDestroy( codec2 );
	CleanupStack::PopAndDestroy( codec );
		
	strP.Close();
	CleanupStack::PopAndDestroy( &strP );
	REComSession::FinalClose();
}

/** 
@SYMTestCaseID HTTP-CODEC-002
@SYMTestCaseDesc Test Code Chaining
@SymFssID Application-Protocols-HTTP-19.1
@SymREQ REQ 4878.5
*/ 
void TestCodecChaining2L()
{
	RStringPool	strP;
	_LIT8( KHttpClientCodecName, "HTTP/general" );
	CleanupClosePushL( strP );
	strP.OpenL( RHTTPSession::GetTable() );
	
	CHeaderCodec* codec = CHeaderCodecPlugin::NewL( KHttpClientCodecName, strP );
	CleanupStack::PushL( codec );

	//make sure client codec is not loaded
	test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-CODEC-0002 Test to make sure client codec is not loaded" ) );
	RStringF accept = strP.StringF( HTTP::EAccept, RHTTPSession::GetTable() );//this is a client header
	test( !codec->CanEncode( accept ) );

	//make sure general codec has loaded
	test.Next( _L( "Test to make sure general codec has loaded" ) );
	RStringF cachecontrol = strP.StringF( HTTP::ECacheControl, RHTTPSession::GetTable() );//this is a general header
	test( 	codec->CanEncode( cachecontrol ) );

	CleanupStack::PopAndDestroy( codec );
		
	strP.Close();
	CleanupStack::PopAndDestroy( &strP );
	REComSession::FinalClose();
}

/** 
@SYMTestCaseID HTTP-CODEC-003
@SYMTestCaseDesc Test Code Chaining
@SymFssID Application-Protocols-HTTP-19.1
@SymREQ REQ 4878.5
*/ 
void TestCodecChaining3L()
{
	RStringPool	strP;
	_LIT8( KHttpClientCodecName, "HTTP" );
	CleanupClosePushL( strP );
	strP.OpenL( RHTTPSession::GetTable() );
	
	CHeaderCodec* codec = CHeaderCodecPlugin::NewL( KHttpClientCodecName, strP );
	CleanupStack::PushL( codec );

	//make sure default codec has loaded
	test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-CODEC-0003 Test to make sure default codec has loaded" ) );
	RStringF extended  = strP.OpenFStringL( _L8( "extended" ) );//unknown header
	CleanupClosePushL( extended );
	test( codec->CanEncode( extended ) );
	
	//make sure there is no delgate codec for this codec
	CHeaderCodec* codec3 = codec->FindDelegateCodecL( extended );
	test( !codec3 );

	CleanupStack::PopAndDestroy( &extended );
	CleanupStack::PopAndDestroy( codec );
		
	strP.Close( );
	CleanupStack::PopAndDestroy( &strP );
	REComSession::FinalClose( );
}

/** 
@SYMTestCaseID HTTP-CODEC-004
@SYMTestCaseDesc Test loading an unknown protocol
@SymFssID Application-Protocols-HTTP-19.1
@SymREQ REQ 4878.5
*/ 
void TestLoadUnknownProtocolL( )
{
	RStringPool	strP;
	_LIT8( KHttpClientCodecName, "FTP/general/client" );
	CleanupClosePushL( strP );
	strP.OpenL( RHTTPSession::GetTable() );
	
	//make sure it leaves with KErrNotFound if we try to load unknown protocol
	test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-CODEC-0004 Test to make sure it leaves with KErrNotFound if we try to load unknown protocol" ) );
	TRAPD( err, CHeaderCodecPlugin::NewL( KHttpClientCodecName, strP ) );
	test( err == KErrNotFound );
		
	strP.Close( );
	CleanupStack::PopAndDestroy( &strP );
	REComSession::FinalClose( );
}

/** 
@SYMTestCaseID HTTP-CODEC-005
@SYMTestCaseDesc Test loading an unknown Codec
@SymFssID Application-Protocols-HTTP-19.1
@SymREQ REQ 4878.5
*/ 
void TestLoadUnknownCodecL()
{
	RStringPool	strP;
	_LIT8( KHttpClientCodecName, "HTTP/client/Delta/WebDav" );
	CleanupClosePushL( strP );
	strP.OpenL( RHTTPSession::GetTable() );
	
	//make sure it loads only known codecs and ignores the unknown codecs
	test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-CODEC-0005 Test to make sure make sure it loads only known codecs and ignores the unknown codecs" ) );
	CHeaderCodec* codec = CHeaderCodecPlugin::NewL( KHttpClientCodecName, strP );
	CleanupStack::PushL( codec );
	RStringF cachecontrol = strP.StringF( HTTP::EAccept, RHTTPSession::GetTable() );//client header
	test( codec->CanEncode( cachecontrol ) );
	CleanupStack::PopAndDestroy( codec );

	strP.Close( );
	CleanupStack::PopAndDestroy( &strP );
	REComSession::FinalClose( );
}

/** 
@SYMTestCaseID HTTP-CODEC-006
@SYMTestCaseDesc Test loading an different Codecs
@SymFssID Application-Protocols-HTTP-19.1
@SymREQ REQ 4878.5
*/ 
void TestLoadDifferentCodecsL()
{
	RStringPool	strP;
	_LIT8( KHttpCodecName, "HTTP/client/server" );
	CleanupClosePushL( strP );
	strP.OpenL( RHTTPSession::GetTable() );
	
	//make sure it loads the server codec
	test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-HTTP-CODEC-0006 Test to make sure it loads the server codec" ) );
	CHeaderCodec* codec = CHeaderCodecPlugin::NewL( KHttpCodecName, strP );
	CleanupStack::PushL( codec );
	RStringF authenticate = strP.StringF( HTTP::EWWWAuthenticate, RHTTPSession::GetTable() );//client header
	test( codec->CanEncode( authenticate ) );
	
	//load the delegate codec and make sure its client codec
	CHeaderCodec* codec1 = codec->FindDelegateCodecL( authenticate );;
	CleanupStack::PushL( codec1 );
	test.Next( _L( "Test to check whether client codec has loaded" ) );
	RStringF accept = strP.StringF(HTTP::EAccept,RHTTPSession::GetTable());//this is a client header
	test( 	codec1->CanEncode(accept) );

	//load the delegate codec and make sure its general codec
	RStringF cachecontrol = strP.StringF( HTTP::ECacheControl, RHTTPSession::GetTable( ) );//this is a general header
	test.Next( _L( "load the delegate codec and make sure its general codec" ) );
	CHeaderCodec* codec2 = codec1->FindDelegateCodecL( cachecontrol );
	CleanupStack::PushL( codec2 );
	test( codec2->CanEncode( cachecontrol ) );
	
	//load the delegate codec and make sure its default codec
	RStringF extended  = strP.OpenFStringL( _L8( "extended" ) );//unknown header
	CleanupClosePushL( extended );
	test.Next( _L( "Test to load the delegate codec and make sure its default codec" ) );
	CHeaderCodec* codec3 = codec2->FindDelegateCodecL( extended );
	CleanupStack::PushL( codec3 );
	test( codec3->CanEncode( extended ) );
	
	CleanupStack::PopAndDestroy( codec3 );
	CleanupStack::PopAndDestroy( &extended );
	CleanupStack::PopAndDestroy( codec2 );
	CleanupStack::PopAndDestroy( codec1 );
	
	CleanupStack::PopAndDestroy( codec );
	strP.Close( );
	CleanupStack::PopAndDestroy( &strP );
	REComSession::FinalClose( );
	
}

GLDEF_C void DoTestsL()
	{
	TestCodecChaining1L( );
	TestCodecChaining2L( );
	TestCodecChaining3L( );
	TestLoadUnknownProtocolL( );
	TestLoadUnknownCodecL( );
	TestLoadDifferentCodecsL( );
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* tc = CTrapCleanup::New( );
	test.Start( _L( "Starting codec plugin chaining unit test" ) );
	TRAPD( err, DoTestsL( ) );
	test( err == KErrNone );
	test.End( );
	test.Close( );
	delete tc;
	__UHEAP_MARKEND;
	
	return KErrNone;
	}
