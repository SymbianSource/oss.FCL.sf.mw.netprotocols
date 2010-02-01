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

#include "browsertransaction.h"
#include "httpexampleutils.h"
#include "htmlhandler.h"

#include <escapeutils.h>

_LIT8(KHtmlParserDataType, "text/html");
_LIT8(KXmlParserDataType, "text/xml");


_LIT8(KUserAgent, "Test web browser/1.0");

CBrowserTransaction::CBrowserTransaction ( RHTTPSession& aSession, 
										CHttpExampleUtils& aTestUtils,
										MBrowserTransactionObserver* aObserver,
										TBool aParseHtml /* = EFalse */ )
: iSession ( aSession ),
	iTransObserver ( aObserver ),
	iTestUtils ( aTestUtils ),
	iParseHtml ( aParseHtml ),
	iSavingBody ( EFalse )
	{
	
	}

CBrowserTransaction::~CBrowserTransaction ()
	{
	iFileServer.Close();
	delete iFilePath;
	delete iHtmlHandler;
	}

CBrowserTransaction* CBrowserTransaction::NewLC (	RHTTPSession& aSession, 
												CHttpExampleUtils& aTestUtils,
												MBrowserTransactionObserver* aObserver,
												const TDesC& aFilePath,
												TBool aParseHtml /* = EFalse */ )
	{
	CBrowserTransaction* me = new( ELeave ) CBrowserTransaction ( aSession, aTestUtils, aObserver, aParseHtml );
	CleanupStack::PushL ( me );
	me->ConstructL ( aFilePath );
	return me;
	}

void CBrowserTransaction::ConstructL ( const TDesC& aFilePath )
	{
	User::LeaveIfError ( iFileServer.Connect () );
	iFilePath = aFilePath.AllocL ();
	}

CBrowserTransaction* CBrowserTransaction::NewL ( RHTTPSession& aSession, 
											   CHttpExampleUtils& aTestUtils,
											   MBrowserTransactionObserver* aObserver,
											   const TDesC& aFilePath,
											   TBool aParseHtml /* = EFalse */ )
	{
	CBrowserTransaction* me = CBrowserTransaction::NewLC ( aSession, aTestUtils, aObserver, aFilePath, aParseHtml );
	CleanupStack::Pop ( me );
	return me;
	}

/**
	Open a new transaction and set the Date header field.
	
	@param aUri [in] Request URI.
 */	
void CBrowserTransaction::CreateTransactionL ( const TDesC8& aUri )
	{
	
	TUriParser8 uri; 
	uri.Parse ( aUri );	
	RStringF get = iSession.StringPool().StringF ( HTTP::EGET, RHTTPSession::GetTable() );	
	// Open a new transaction.
	iTransaction = iSession.OpenTransactionL ( uri, *this, get );	

	// Add current date header
	TTime time;
	time.UniversalTime();
	THTTPHdrVal hdrVal( time.DateTime() );
	RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
	hdr.SetFieldL( iSession.StringPool().StringF( HTTP::EDate, RHTTPSession::GetTable() ), hdrVal );	

	RStringF valStr = iSession.StringPool().OpenFStringL(KUserAgent);
	THTTPHdrVal val(valStr);
	hdr.SetFieldL(iSession.StringPool().StringF( HTTP::EUserAgent, RHTTPSession::GetTable() ), val);
	valStr.Close();

	// Set the accept header.
	SetAcceptHdrL ();
	}

/**
	Submit the transaction.		
 */	
void CBrowserTransaction::StartTransactionL ()
	{
	// Submit the transaction
	iTransaction.SubmitL ();	
	}

/**
   Extract content type header from the HTTP response headers. 
 */
void CBrowserTransaction::ExtractContentTypeL ( RHTTPResponse& aResponse, RStringF& aContentTypeValue )
	{	
	RHTTPHeaders hdr = aResponse.GetHeaderCollection();

	RStringF contentType = iSession.StringPool ().StringF ( HTTP::EContentType, RHTTPSession::GetTable() );
	
	THTTPHdrVal fieldVal;
	TInt ret = hdr.GetField ( contentType, 0, fieldVal );
	if ( ret != KErrNone )
		{
		User::Leave ( KErrNotFound );
		}
		
	aContentTypeValue = fieldVal.StrF ();	
	}

/**
   Load the parser plugin. text/html or text/xml type.
 */
void CBrowserTransaction::LoadPluginL ( TMimeType aType )
	{		
	// Load the corresponding plugin based on the type. If it is of "xml" type 
	// load xml plugin else load html plugin.		
	iHtmlHandler = CHtmlHandler::NewL ( *iTransObserver, ( aType == eXml ) ? KXmlParserDataType() : KHtmlParserDataType() );			
	}

void CBrowserTransaction::SetAcceptHdrL ()
	{	 
	// Set the following HTTP Accept header.
	// text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,*/*;q=0.5	
	RHTTPHeaders hdr = iTransaction.Request ().GetHeaderCollection();
	RStringPool stringPool = iSession.StringPool ();
	
	RStringF textXml = stringPool.OpenFStringL ( _L8 ( "text/xml" ) );
	CleanupClosePushL ( textXml );
	
	RStringF appXml = stringPool.OpenFStringL ( _L8 ( "application/xml" ) );
	CleanupClosePushL ( appXml );
	
	RStringF appXhtmlXml = stringPool.OpenFStringL ( _L8 ( "application/xhtml+xml" ) );
	CleanupClosePushL ( appXhtmlXml );
	
	RStringF textHtml = stringPool.OpenFStringL ( _L8 ( "text/html" ) );
	CleanupClosePushL ( textHtml );
			
	RStringF any = stringPool.OpenFStringL ( _L8 ( "*/*" ) );
	CleanupClosePushL ( any );
	
	THTTPHdrVal hdrVal ( textXml );
	hdr.SetFieldL ( stringPool.StringF ( HTTP::EAccept, RHTTPSession::GetTable () ), hdrVal );
	
	hdrVal.SetStrF ( appXml );
	hdr.SetFieldL ( stringPool.StringF ( HTTP::EAccept, RHTTPSession::GetTable () ), hdrVal );
	
	hdrVal.SetStrF ( appXhtmlXml );
	hdr.SetFieldL ( stringPool.StringF ( HTTP::EAccept, RHTTPSession::GetTable () ), hdrVal );
	
	hdrVal.SetStrF ( textHtml );
	THTTPHdrVal q;
	q.SetInt( THTTPHdrVal::TQConv( 0.9 ) );
	
	hdr.SetFieldL ( stringPool.StringF ( HTTP::EAccept, RHTTPSession::GetTable () ), hdrVal, 
					stringPool.StringF ( HTTP::EQ, RHTTPSession::GetTable() ), q );

	q.SetInt ( THTTPHdrVal::TQConv( 0.5 ) );
	hdrVal.SetStrF ( any );
	hdr.SetFieldL ( stringPool.StringF ( HTTP::EAccept, RHTTPSession::GetTable () ), hdrVal, 
					stringPool.StringF ( HTTP::EQ, RHTTPSession::GetTable() ), q );
	
	CleanupStack::PopAndDestroy ( 5 );	// Destroy textXml, appXml, appXhtmlXml, textHtml & any.		
	}

			
void CBrowserTransaction::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received. 
			RHTTPResponse resp = aTransaction.Response();
			TInt status = resp.StatusCode();
			if ( iParseHtml )
				{
				RStringF contentTypeValue;
				ExtractContentTypeL ( resp, contentTypeValue );
				TMimeType type = GetMimeType ( contentTypeValue );
				if ( type != eUnknown )
					{
					TRAPD ( err, LoadPluginL ( type ) );
					if ( err != KErrNone )
						{
						iTestUtils.Test().Printf(_L("\nUnable to load the parser plugin.\n"));
						User::Leave ( KErrNotFound );					
						}										
					else
						{
						iTestUtils.Test().Printf(_L("\nPlugin loaded.\n"));							
						}
					}
				else
					{
					iParseHtml = EFalse;						
					}
				}

			if ( resp.HasBody() && ( status >= 200 ) && ( status < 300 ) && ( status != 204 ) )
				{
				iFileServer.MkDirAll( iFilePath->Des() );
				User::LeaveIfError ( iRespBodyFile.Replace ( iFileServer, iFilePath->Des(), EFileWrite|EFileShareExclusive ) );
				iSavingBody = ETrue;
				}
			} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Get the body data supplier
			iRespBody = aTransaction.Response().Body();
			// Append to the output file.
			TPtrC8 bodyData;
			TBool lastChunk = iRespBody->GetNextDataPart( bodyData );

			if ( iParseHtml )
				{
				iHtmlHandler->ParseHtmlContentL( bodyData );				
				}
				
			if ( iSavingBody )
				{
				iRespBodyFile.Write( bodyData );								
				}

						
			if ( lastChunk )
				{
				if ( iParseHtml )
					{
					iHtmlHandler->ParseEndL ();							
					}
				if ( iSavingBody )
					{
					iRespBodyFile.Close();				
					}
				}

			// Done with that bit of body data
			iRespBody->ReleaseData();
			
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			iTestUtils.Test().Printf(_L("\nTransaction Complete\n"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			iTestUtils.Test().Printf(_L("Transaction Successful\n"));
			aTransaction.Close();
			iTransObserver->OnTransactionClose ( this );
			} break;
		case THTTPEvent::EFailed:
			{
			iTestUtils.Test().Printf(_L("Transaction Failed\n"));
			aTransaction.Close();
			iTransObserver->OnTransactionClose ( this );
			} break;
		case THTTPEvent::ERedirectedPermanently:
			{
			iTestUtils.Test().Printf(_L("Permanent Redirection\n"));
			} break;
		case THTTPEvent::ERedirectedTemporarily:
			{
			iTestUtils.Test().Printf(_L("Temporary Redirection\n"));
			} break;
		case THTTPEvent::ERedirectRequiresConfirmation:
 			{
			// 301(Moved Permanently), 302(Found) or 307(Temporary Redirect) status is received 
			// from a transaction and hence ERedirectRequiresConfirmation is sent by filter
			// client has opted to close the transaction
			iTestUtils.Test().Printf(_L("Redirect requires confirmation\n"));
 			aTransaction.Close();
			iTransObserver->OnTransactionClose ( this );
 			} break;
		default:
			{
			iTestUtils.Test().Printf(_L("<unrecognised event: %d>\n"), aEvent.iStatus);
			// close off the transaction if it's an error
			if (aEvent.iStatus < 0)
				{
				aTransaction.Close();
				iTransObserver->OnTransactionClose ( this );
				}
			} break;
		}
	}

TInt CBrowserTransaction::MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	iTestUtils.Test().Printf(_L("MHFRunError fired with error code %d\n"), aError);
	iTestUtils.PressAnyKey ();

	return KErrNone;
	}


CBrowserTransaction::TMimeType CBrowserTransaction::GetMimeType ( const RStringF& aType )
	{
	// Find the type has "xml" or "html". HTTP header will be set to 
	// text/xml,application/xml,application/xhtml+xml,text/html	
	TMimeType type ( eUnknown );
	TPtrC8 ptrType ( aType.DesC() );
	
	if ( ptrType.FindF ( _L8 ("xml") ) != KErrNotFound )
		{
		type = eXml;
		}

	if ( ptrType.FindF ( _L8 ("html") ) != KErrNotFound )
		{
		type = eHtml;
		}	
	return type;
	}
