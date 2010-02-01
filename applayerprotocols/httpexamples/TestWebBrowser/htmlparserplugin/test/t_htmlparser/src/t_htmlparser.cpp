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
#include <xml/xmlparsererrors.h>
#include <xml/parserfeature.h>
#include <xml/parser.h>

#include "contenthandlers.h"
#include "unzip.h"

using namespace Xml;
//_LIT ( KHtmlInputFile, "c:\\system\\data\\html\\htmldoc.html" );
//_LIT ( KHtmlRefFile, "c:\\system\\data\\html\\htmldoc.ref" );
_LIT ( KXhtmlInputFile, "c:\\system\\data\\html\\XHMTLdoc.xhtml" );
_LIT ( KXhtmlRefFile, "c:\\system\\data\\html\\XHXMTLdoc.ref" );
_LIT ( KInvalidTestFile, "c:\\system\\data\\html\\invalid.html" );
_LIT(KSimpleTestFile,    "c:\\system\\data\\html\\simple.html");
_LIT(KJunkTestFile,    "c:\\system\\data\\html\\Junk.html");


RTest test(_L("HTML parser tests"));  // must be called test, as the e32test macros rely on this

_LIT8(KParserDataType, "text/html");

_LIT8 ( KInput1, "<html><head><ul><li>foo<li>bar</ul></head></html>" );
_LIT8 ( KOutput1,"<?xml encoding=\"UTF-8\"?>\n<html><head><ul><li>foo</li><li>bar</li></ul></head></html>" );
_LIT8 ( KInput2, "<html><head><ul><li>foo<ul><li>one<li>two</ul><li>bar</ul></head></html>" );
_LIT8 ( KOutput2,"<?xml encoding=\"UTF-8\"?>\n<html><head><ul><li>foo<ul><li>one</li><li>two</li></ul></li><li>bar</li></ul></head></html>" );
_LIT8 ( KInput3, "<html><head><td nowrap><br></td></head></html>");
_LIT8 ( KOutput3, "<?xml encoding=\"UTF-8\"?>\n<html><head><td nowrap=\"nowrap\"><br /></td></head></html>");
_LIT8 ( KInput4, "<html><head><td valign=top nowrap width=25%><br></td></head></html>");
_LIT8 ( KOutput4, "<?xml encoding=\"UTF-8\"?>\n<html><head><td valign=\"top\" nowrap=\"nowrap\" width=\"25%\"><br /></td></head></html>");
_LIT8 ( KInput5,"<html><head><ul><li>foo</li><li>bar</li></ul></head></html>" );
_LIT8 ( KOutput5,"<?xml encoding=\"UTF-8\"?>\n<html><head><ul><li>foo</li><li>bar</li></ul></head></html>" );
_LIT8 ( KInput6, "<html><head><br><td nowrap></td></head></html>" );
_LIT8 ( KOutput6, "<?xml encoding=\"UTF-8\"?>\n<html><head><br /><td nowrap=\"nowrap\"></td></head></html>" );
_LIT8 ( KInput7, "<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"></head></html><br><br>");
_LIT8 ( KOutput7, "<?xml encoding=\"UTF-8\"?>\n<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\" /></head></html>");


// Task: Parses the HTML/XHTML file passed as argument. A comparison
// between the output in Epocwind.out and file content can be used to
// that the parser is doing what it should.
LOCAL_C void BasicParsingTestL ( const TDesC& aFileName, const TDesC& aRefFile )
	{
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile result;
	TFileName filename;
	User::LeaveIfError( result.Temp( fs, _L("C:\\"), filename, EFileWrite ) );
	CleanupClosePushL( result );

	TRebuildingContentHandler contentHandler( result );
	
	CParser* parser = CParser::NewLC(KParserDataType, contentHandler);
	
	contentHandler.SetStringPoolL( parser->StringPool () );
	
	ParseL ( *parser, fs, aFileName );
	test(contentHandler.iError==KErrNone);

	CleanupStack::PopAndDestroy(2);
	
	RFile refFile;
	User::LeaveIfError( refFile.Open( fs, aRefFile, EFileRead ) );
	CleanupClosePushL( refFile );
	User::LeaveIfError( result.Open( fs, filename, EFileRead ) );
	CleanupClosePushL( result );

	TInt resultSize;
	User::LeaveIfError( result.Size( resultSize ) );
	TInt refSize;
	User::LeaveIfError( refFile.Size( refSize ) );
	test( resultSize == refSize );

	TBuf8<256> refBuf;
	TBuf8<256> resultBuf;

	while( refFile.Read(refBuf)==KErrNone && result.Read(resultBuf) == KErrNone && refBuf.Length() > 0 )
			test(refBuf==resultBuf);

	test(refBuf.Length()==0 && resultBuf.Length()==0);

	CleanupStack::PopAndDestroy(2);
	fs.Delete(filename);
	CleanupStack::PopAndDestroy(&fs);

	return;				
	}

// Handle invalid HTML document that has a tag mismatch.
LOCAL_C void InvalidTestL ( )
	{
	TSimpleContentHandler contentHandler;
	
	CParser* parser = CParser::NewLC(KParserDataType, contentHandler);
	contentHandler.SetStringPoolL ( parser->StringPool() );
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	ParseL(*parser, fs, KInvalidTestFile());
	if(contentHandler.iError!=EXmlTagMismatch)
		User::Leave(contentHandler.iError);  // For OOM testing

	CleanupStack::PopAndDestroy(2);		
	}

// Handle special test case for HTML document. The input and output will be passed as 
// parameters
LOCAL_C void TestHTMLCasesL ( const TDesC8& aInput, const TDesC8& aOutput )
	{	
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile result;
	TFileName filename;
	User::LeaveIfError( result.Temp( fs, _L("C:\\"), filename, EFileWrite ) );
	CleanupClosePushL( result );

	TRebuildingContentHandler contentHandler( result );
	CParser* parser = CParser::NewLC( KParserDataType, contentHandler );
	
	contentHandler.SetStringPoolL ( parser->StringPool() );
	
	parser->ParseBeginL ();
	parser->ParseL ( aInput );
	parser->ParseEndL ();
	test(contentHandler.iError==KErrNone);
	CleanupStack::PopAndDestroy ( 2 );
	
	User::LeaveIfError( result.Open( fs, filename, EFileRead ) );
	CleanupClosePushL( result );

	TInt resultSize;
	User::LeaveIfError( result.Size( resultSize ) );
	test( resultSize == aOutput.Length() );

	TBuf8<256> resultBuf;
	while( result.Read(resultBuf) == KErrNone && resultBuf.Length () > 0 )
		{		
		test(!resultBuf.Compare ( aOutput ));			
		}
		
	test( resultBuf.Length()==0 );
	CleanupStack::PopAndDestroy(1);
	fs.Delete(filename);
	CleanupStack::PopAndDestroy(&fs);
	return;
	}

// Test case for junk HTML document. 
LOCAL_C void TestJunkHTMLL ()
	{
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile result;
	TFileName filename;
	User::LeaveIfError( result.Temp( fs, _L("C:\\"), filename, EFileWrite ) );
	CleanupClosePushL( result );

	TRebuildingContentHandler contentHandler( result );
	
	CParser* parser = CParser::NewLC(KParserDataType, contentHandler);
	
	contentHandler.SetStringPoolL ( parser->StringPool() );
	
	ParseL ( *parser, fs, KJunkTestFile() );
	test(contentHandler.iError==KErrNone);
	
	CleanupStack::PopAndDestroy( 2 );
	
	User::LeaveIfError( result.Open( fs, filename, EFileRead ) );
	CleanupClosePushL( result );
	TInt resultSize;
	User::LeaveIfError( result.Size( resultSize ) );
	test ( resultSize == 0 );
	
	CleanupStack::PopAndDestroy(1);
	fs.Delete(filename);
	CleanupStack::PopAndDestroy(&fs);

	}

// Checks that the parser copes with a leave from a content handler callback
LOCAL_C void CallbackLeaveTestL()
	{
	TSimpleContentHandler contentHandler;
	contentHandler.iLeaveOnStartElement = ETrue;
	
	
	CParser* parser = CParser::NewLC(KParserDataType, contentHandler);

	contentHandler.SetStringPoolL ( parser->StringPool() );
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	TRAPD(err, ParseL(*parser, fs, KSimpleTestFile()));
	
	User::LeaveIfError(err); // For OOM testing

	test(contentHandler.iNumElements==1);
	test(err==TSimpleContentHandler::KExpectedLeaveCode);

	contentHandler.iLeaveOnStartElement = EFalse;
	ParseL(*parser, fs, KSimpleTestFile());
	test(contentHandler.iNumElements==5);

	CleanupStack::PopAndDestroy(2);
	}


LOCAL_C void OomTest(void (*testFuncL)())
	{	
	TInt error;
	TInt count = 0;
	do
		{
		User::__DbgSetAllocFail(RHeap::EUser, RHeap::EFailNext, ++count);
		User::__DbgMarkStart(RHeap::EUser);
		TRAP(error, (testFuncL)());
		User::__DbgMarkEnd(RHeap::EUser, 0);
		} while(error == KErrNoMemory);

	_LIT(KTestFailed, "Out of memory test failure on iteration %d\n");
	__ASSERT_ALWAYS(error==KErrNone, test.Panic(error, KTestFailed, count));

	User::__DbgSetAllocFail(RHeap::EUser, RHeap::ENone, 1);
	}

	
LOCAL_C void RunTestsL ()
	{
	test.Title();
/*
	test.Start(_L("Unziping test html files"));
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	CUnzip* unzip = CUnzip::NewLC(fs, KZipTestFile);
	unzip->ExtractL(KHtmlTestDir);
	CleanupStack::PopAndDestroy(unzip);

*/	
//	test.Start( _L( "Starting basic HTML parsing test" ) );
 //	BasicParsingTestL( KHtmlInputFile, KHtmlRefFile );	
  	
  	test.Next( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-BOOKMARKS-T_HTMLPARSER-0001 Starting basic XHTML parsing test" ) );
 	BasicParsingTestL( KXhtmlInputFile, KXhtmlRefFile );	
 	
	test.Next( _L( "Invalid document handling" ) );
	InvalidTestL ();
	
	test.Next( _L( "Junk HTML document handling" ) );
	TestJunkHTMLL ();
	
	test.Next( _L( "Testing HTML cases --- Optional tags." ) );
	TestHTMLCasesL ( KInput1 (), KOutput1 () );
	TestHTMLCasesL ( KInput2 (), KOutput2 () );
	TestHTMLCasesL ( KInput5 (), KOutput5 () );
	TestHTMLCasesL ( KInput6 (), KOutput6 () );
	
	test.Next ( _L("Testing HTML cases --- Attributes with no attribute values" ) );
	TestHTMLCasesL ( KInput3 (), KOutput3 () );
	TestHTMLCasesL ( KInput4 (), KOutput4 () );
	
	test.Next ( _L ("Test case: Content after </html> will be ignored"));
	TestHTMLCasesL ( KInput7 (), KOutput7 () );
		
	test.Next(_L("Leave from callback"));
	CallbackLeaveTestL ();
	
	test.Next(_L("Out-of-Memory testing"));	
	OomTest(CallbackLeaveTestL);
	OomTest( InvalidTestL );
	test.Next(_L("***Finished.***"));

	test.End();

	test.Close();

	REComSession::FinalClose();
	return;
	}
	
TInt E32Main()
	{	
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if( !cleanup )
		return KErrNoMemory;

	TRAPD( err, RunTestsL() );
	if ( err != KErrNone )
		User::Panic( _L(" Testing failed: "), err );

	delete cleanup;
	__UHEAP_MARKEND;

	return 0;
	}
