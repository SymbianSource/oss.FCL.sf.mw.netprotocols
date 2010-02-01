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

#include <e32base.h>
#include <http.h>
#include <escapeutils.h>

#include "testwebbrowser.h"
#include "httpexampleutils.h"


const TInt KMaxURISize = 256;
const TInt KMaxFilePathLength = 512;
const TInt KHttpSchemeLength = 7;

_LIT ( KDirPath, "c:\\TestWebBrowser\\");
_LIT ( KDefaultFilePath, "index.html" );
_LIT ( KQuit, "quit" );
_LIT ( KSelectOption, " Select an option \n\n" );
_LIT ( KPossibleSelectionsText, " 1 Download URL \n 2 Set Proxy\n 3 Quit \n" );
_LIT ( KPossibleSelections,"123" );
_LIT ( KEnterUrl, "Enter URL: " );
_LIT ( KEnterProxy, "Enter Proxy Address: " );	
		
// Invalid characters that can appear in a URL. These values must be replaced
// with a valid character to generate the full file path.
_LIT ( KInvalidChars, "<>:\"|*?");

const TUint KReplaceChar = '_';

void ChangePathSeparator( TDes& aDesPtr, TUint aPathSeperatorFrom, TUint aPathSeperatorTo )
	{
	for( TInt offset = aDesPtr.Length() - 1; offset >= 0; offset-- )
		{
		if ( aDesPtr[offset] == aPathSeperatorFrom )
			{	
				aDesPtr[offset] = TUint16( aPathSeperatorTo );
			}
		}
	}

CTestWebBrowser::CTestWebBrowser ( CHttpExampleUtils& aUtils )
: CActive ( EPriorityStandard ),
iTestUtils ( aUtils ),
iCurrentState ( EMainMenu )
	{
	//Add this active object to the active schedeler
	CActiveScheduler::Add(this);
	__ASSERT_DEBUG( !IsActive(),User::Panic(_L("Reader"), KErrCorrupt) );
	
	// Wait for request to be handled.
	IssueAndCompleteRequest();		
	}


CTestWebBrowser::~CTestWebBrowser ()
	{
	// Close the HTTP session.
	iHttpSession.Close();
	
	iTransArray.ResetAndDestroy ();
	iTransArray.Close ();
	
	delete iBaseUri;
	}
	
CTestWebBrowser* CTestWebBrowser::NewLC ( CHttpExampleUtils& aUtils )	
	{
	CTestWebBrowser* me = new (ELeave) CTestWebBrowser ( aUtils );
	CleanupStack::PushL ( me );
	me->ConstructL ();
	return me;
	}

void CTestWebBrowser::ConstructL ()
	{
	// Open a HTTP session.
	iHttpSession.OpenL ();
	
	// Install the callback into the session.
	InstallAuthenticationL ( iHttpSession );
	
	iBaseUri = HBufC8::NewL ( KMaxURISize );
	}

CTestWebBrowser* CTestWebBrowser::NewL ( CHttpExampleUtils& aUtils )	
	{
	CTestWebBrowser* me = CTestWebBrowser::NewLC ( aUtils );
	CleanupStack::Pop ( me );
	return me;
	}

/**
	Set HTTP proxy information for the session.
	
	@param aProxyAddr [in] Proxy address to set for the session
 */	
void CTestWebBrowser::SetProxyL ( const TDesC8& aProxyAddr )
	{
	
	RHTTPConnectionInfo connInfo = iHttpSession.ConnectionInfo();
	
	// Set the proxy usage property.
	THTTPHdrVal proxyUsage( iHttpSession.StringPool().StringF( HTTP::EUseProxy, RHTTPSession::GetTable() ) );
	connInfo.SetPropertyL( iHttpSession.StringPool().StringF( HTTP::EProxyUsage, RHTTPSession::GetTable() ), proxyUsage );
	
	RStringF proxyAddr = iHttpSession.StringPool ().OpenFStringL ( aProxyAddr );
	CleanupClosePushL ( proxyAddr );
	
	// Set the proxy address property.
	THTTPHdrVal proxyAddrHdr( proxyAddr );
    connInfo.SetPropertyL( iHttpSession.StringPool().StringF( HTTP::EProxyAddress, RHTTPSession::GetTable() ), proxyAddrHdr );
    
    CleanupStack::PopAndDestroy (); // Pop and destroy proxyAddr
	return;		
	}

/**
	Get the credentials from the user.
	
	@param aURI [in] The URI being requested
	@param aRealm [out] The realm being requested
	@param aAuthenticationType [out] The type of authentication.
	@param aUsername [out] User name
	@param aPassword [out] Pass word
	
	@return TBool ETrue if credentials being returned else EFalse
 */		
TBool CTestWebBrowser::GetCredentialsL( const TUriC8& aURI, 
										RString aRealm, 
									   	RStringF aAuthenticationType,
									   	RString& aUsername, 
									   	RString& aPassword )
	{
	// Convert to 16 bit to display
	HBufC* uriDesBuf = HBufC::NewLC( aURI.UriDes().Length() );
	TPtr uriDesPtr( uriDesBuf->Des() );
	uriDesPtr.Copy( aURI.UriDes() );

	HBufC* uriRealmBuf = HBufC::NewLC( aRealm.DesC().Length() );
	TPtr uriRealmPtr( uriRealmBuf->Des() );
	uriRealmPtr.Copy( aRealm.DesC() );
	
	HBufC* uriAuthenticationType = HBufC::NewLC ( aAuthenticationType.DesC().Length() );
	TPtr uriAuthenticationPtr( uriAuthenticationType->Des() );
	uriAuthenticationPtr.Copy ( aAuthenticationType.DesC() );
	
	
	// Prompt user for input
	iTestUtils.Test().Printf ( _L( "Enter credentials for URL %S, realm %S\n"), &uriDesPtr, &uriRealmPtr );
	iTestUtils.Test().Printf ( _L("Using %S authentication\n"), &uriAuthenticationPtr );
	CleanupStack::PopAndDestroy ( 3 ); // Pop and destroy uriDesBuf, uriRealmBuf and uriAuthenticationType
	
	HBufC* userDetails16 = HBufC::NewLC ( KMaxUserEntrySize );
	HBufC8* userDetails8 = HBufC8::NewLC ( KMaxUserEntrySize );
	TPtr userDetailsPtr16( userDetails16->Des() );
	TPtr8 userDetailsPtr8( userDetails8->Des() );
	
	iTestUtils.GetAnEntry ( _L( "Username (or QUIT to give up): " ), userDetailsPtr16 );
	TBool set = EFalse;
	if ( userDetailsPtr16.CompareF ( KQuit ) )
		{
		userDetailsPtr8.Copy ( userDetailsPtr16 );
		aUsername = aRealm.Pool().OpenStringL ( userDetailsPtr8 );
		iTestUtils.GetAnEntry( _L( "Password: " ), userDetailsPtr16 );
		userDetailsPtr8.Copy ( userDetailsPtr16 );
		aPassword = aRealm.Pool().OpenStringL ( userDetailsPtr8 );
		set = ETrue;		
		}
	CleanupStack::PopAndDestroy ( 2 ); // Pop and destroy userDetails16 & userDetails8
	return set;	
	}

/**
	Create a new transaction. Generate the full uri from a partial uri.
	Also, generates the file path where the contents to be saved. Adds the transaction
	object into array.
	
	@param aUri [in] URI to download.
	@param aParseHtml [in] Parse the downloaded HTML content. Default value: EFalse. ETrue value to parse 
	the HTML content. Typically the value will be set to ETrue only for the main page.
 */
void CTestWebBrowser::CreateTransactionL ( const TDesC8& aUri, TBool aParseHtml /* = EFalse */ )
	{	
	TUriParser8 baseUri;	
	baseUri.Parse ( iBaseUri->Des() );
	
	TUriParser8 refUri;
	refUri.Parse ( aUri );

	// Resolve uri
	CUri8* uriPtr = CUri8::ResolveL( baseUri, refUri );	
	CleanupStack::PushL ( uriPtr );
	
	// Generate the file path from the full http uri.	
	HBufC* filePath = GenerateFilePathL ( uriPtr->Uri() );
	CleanupStack::PushL ( filePath );
	
	// Create a new HTTP transaction object.
	CBrowserTransaction* newTrans = CBrowserTransaction::NewL ( iHttpSession, iTestUtils, this, *filePath, aParseHtml );
	CleanupStack::PopAndDestroy ( filePath );
	CleanupStack::PushL ( newTrans );
	iTransArray.AppendL ( newTrans );
	
	// Open and submit the transaction.
	newTrans->CreateTransactionL ( uriPtr->Uri().UriDes() );
	newTrans->StartTransactionL ();
	
	CleanupStack::Pop ( newTrans ); 
	CleanupStack::PopAndDestroy ( uriPtr );
	return;		
	}

/**
	To create a new transaction. This function is a calllback called from 
	the CHTMLHandler when it finds a new URI to be downloaded.
	
	@param aUri [in] URI to download.
 */	
void CTestWebBrowser::OnTransactionCreateL ( const TDesC8& aUri, TBool aParseHtml )
	{
	CreateTransactionL ( aUri, aParseHtml );
	return;		
	}

/**
	Delete the transaction. Removes the transaction object from the transaction array
	
	@param aTrans [out] Transaction object to close.
 */	
void CTestWebBrowser::OnTransactionClose ( CBrowserTransaction* aTrans ) 
	{	
	// Find the transaction object to be deleted and remove it from the 
	// transaction	array.
	TInt numTrans = iTransArray.Count ();
	for ( TInt i = 0; i < numTrans; ++i )
		{
		if ( iTransArray[ i ] == aTrans )			
			{
			delete aTrans;
			iTransArray.Remove ( i );
			break;
			}
		}	
	
	if ( iTransArray.Count () == 0 )
		{
		// All transactions are completed. 
		iCurrentState = ETransactionCompleted;
		IssueAndCompleteRequest ();
		}
	return;		
	}

/**
	Issues a new request if the active object has no outstanding request.
	
 */	
void CTestWebBrowser::IssueAndCompleteRequest()
	{
	if ( !IsActive() )		
		{
		TRequestStatus* status = &iStatus;
		User::RequestComplete( status, KErrNone );
		SetActive();
		}
	}
	
void CTestWebBrowser::RunL ()
	{
	
	switch ( iCurrentState )
		{
		case EMainMenu:
			iTestUtils.Test().Printf ( _L ( "Main menu") );
			ExecuteMainMenuL();
			break;
		
		case EEnterUrl:
			AcceptUrlL ();
			break;
		
		case ESelectProxy:
			AcceptProxyL ();
			break;
		
		case EStop:		
			CActiveScheduler::Stop();
			break;
		
		case EStartTransaction:
			{		
			CreateTransactionL ( iBaseUri->Des(), ETrue );	
			}
			break;
		
		case ETransactionStarted:
			{
			iTestUtils.Test().Printf ( _L ( "Transaction started. Please wait...") );			
			}
			break;
		
		case ETransactionCompleted:
			{
			OnTransactionCompleted();
			}
			break;				
			
		default:
			break;
		}
	}

void CTestWebBrowser::DoCancel ()
	{
	// Do nothing...		
	}

/**
	Checks whether a uri is valid or not. Will check whether the uri contains
	scheme, host and path.
	
	@param aUri [in] URI to validate. Only scheme and host presence is validated.
	
	@return TBool ETrue if the URI is successfully validated else EFalse.
 */	
TBool CTestWebBrowser::ValidateUri ( const TDesC8& aUri )
	{
	TUriParser8 uriParser;
	if ( uriParser.Parse ( aUri ) != KErrNone )
		{
		return EFalse;			
		}
	
	// check for scheme
	if ( !uriParser.IsPresent ( EUriScheme ) )
		{
		return EFalse;				
		}
	
	// check for host
	if ( !uriParser.IsPresent ( EUriHost) )
		{
		return EFalse;			
		}
	return ETrue;				
	}

/**
	Generate the full file path based on the aUri.
	
	@param aUri [in] Full URI to generate the full file path.
	
	@return HBufC* Holds the full file path.
 */	
HBufC* CTestWebBrowser::GenerateFilePathL ( const TUriC8& aUri )
	{
	
	HBufC* filePath = HBufC::NewLC ( KMaxFilePathLength );	
	TPtr filePtr ( filePath->Des() );	
	
	// Append the base path
	filePtr.Append ( KDirPath ); 
	
	// Get the full uri.
	HBufC* completeUri = aUri.DisplayFormL();
	CleanupStack::PushL ( completeUri );
	
	TPtr ptr = completeUri->Des();
	// Chop off the scheme. ( http:// )
	ptr.Delete ( 0, KHttpSchemeLength );
	
	// Reverse the slashes.
	ChangePathSeparator ( ptr, '/', '\\' );
	
	// Replace invalid characters with '_'. Invalid characters: <>:"|* 	
	TPtrC ptrInvalidChars ( KInvalidChars );
	for ( TInt i = 0; i < KInvalidChars().Length(); ++i )
		{
		ChangePathSeparator ( ptr, ptrInvalidChars[i], KReplaceChar );					
		}
	
	// Append the URI file path.	
	filePtr.Append ( ptr );		
	CleanupStack::PopAndDestroy ( completeUri );
	
	HBufC* uriTail = aUri.GetFileNameL ( EUriFileNameTail );
	CleanupStack::PushL ( uriTail );	
	
	TBool fileExtPresent = uriTail->Des().Length() != 0;
	
	CleanupStack::PopAndDestroy ( uriTail );
			
	// Append a '\\' at the end of the completeUri if not present.
	TInt len = filePtr.Length();
	
	if ( ( !fileExtPresent ) && ( len != 0 ) && ( filePtr[ len-1 ] != '\\') )
		{
		filePtr.Append ( '\\' );		
		}

	if ( !fileExtPresent )
		{
		// No file name present in the uri. Set the file name as index.html.		
		filePtr.Append ( KDefaultFilePath() );
		}	

	CleanupStack::Pop ( filePath );
	iTestUtils.LogIt ( _L("File Path: %S"), &filePtr );
	return filePath;
	}
		
void CTestWebBrowser::ExecuteMainMenuL ()
	{
	iTestUtils.Test().Console()->ClearScreen ();

	iTestUtils.Test().Printf(KSelectOption);		
	
	
	TInt selection = iTestUtils.GetSelection ( KPossibleSelectionsText, KPossibleSelections );	
	
	switch ( selection )
		{
		case EDownloadURL:
		iCurrentState = EEnterUrl;
		break;
		
		case ESetProxy:
		iCurrentState = ESelectProxy;
		break;
		
		case EQuit:
		iCurrentState = EStop;
		break;
		
		default:
		break;
		}
		
	IssueAndCompleteRequest ();
	}

/**
	Accept the url from the user and validate.
 */	
void CTestWebBrowser::AcceptUrlL ()
	{

	HBufC* uri = HBufC::NewLC ( KMaxURISize );
	TPtr ptr( uri->Des() );
	iTestUtils.GetAnEntry ( KEnterUrl, ptr );
	if ( iBaseUri->Des().MaxLength() < ptr.Length() )
		{
		iBaseUri = 	iBaseUri->ReAllocL ( ptr.Length() );
		}
		
	iBaseUri->Des().Copy ( ptr );
	
	if ( ValidateUri( iBaseUri->Des() ) )
		{
		iCurrentState = EStartTransaction;
		}
	else
		{
		iTestUtils.Test().Printf ( _L("Invalid entry.") );
		iTestUtils.PressAnyKey ();			
		iCurrentState = EMainMenu;
		}	
	CleanupStack::PopAndDestroy ( uri );
	
	IssueAndCompleteRequest ();		
	}

/**
	Accept proxy information from the user.
 */	
void CTestWebBrowser::AcceptProxyL ()
	{	
	HBufC* proxy16 = HBufC::NewLC ( 128 )		;
	TPtr ptr( proxy16->Des() );
	iTestUtils.GetAnEntry ( KEnterProxy, ptr );
	
	HBufC8* proxy8 = HBufC8::NewLC ( ptr.Length() );
	proxy8->Des().Copy ( ptr );
	
	TRAPD ( err, SetProxyL ( proxy8->Des() ) );	
	
	if ( err != KErrNone )
		{
		iTestUtils.Test().Printf ( _L ( "Setting of proxy address failed." ) );
		}
	
	iCurrentState = EMainMenu;
	IssueAndCompleteRequest ();
	}

/**
	Set the state to EMainMenu and issues a request to be processed by the RunL.	
 */	
void CTestWebBrowser::OnTransactionCompleted ()
	{
	iTestUtils.Test().Printf ( _L ("Transaction completed.") );
	iTestUtils.PressAnyKey();
	iCurrentState = EMainMenu;			
	IssueAndCompleteRequest ();
	}


