/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Authentication filter
*
*/



// INCLUDE FILES
#include <e32std.h>
#include <e32math.h>
#include <uri8.h>
#include <http/rhttpsession.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <httpstringconstants.h>
#include <tconvbase64.h>

// User Includes
#include "HttpFilterAuthentication.h"
#include "httpfiltercommonstringsext.h"
#include "httperr.h"
#include "HttpFilterAuthenticationLogger.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PanicAuthenticationFilter
// Panic
// Returns: void
// -----------------------------------------------------------------------------
//
void PanicAuthenticationFilter( TInt aErr = 0 )
    {
    User::Panic( _L( "HTTP FILTER AUTHENTICATION" ), aErr );
    }


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::CHttpFilterAuthentication
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CHttpFilterAuthentication::CHttpFilterAuthentication( RHTTPSession* aSession, TBool aIsProxy )
    {
    __ASSERT_DEBUG( aSession != NULL, PanicAuthenticationFilter() );
    iIsProxy = aIsProxy;
    iSession = aSession;
    iStringPool = ( *aSession ).StringPool();

   
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::ConstructL( RHTTPSession aSession )
    {

    // Register the filter. We only care about 401 status codes where
    // there's a WWW-Authenticate header (which the RFC says MUST be
    // present). If we don't fire (e.g. due to a missing header) then
    // the transaction will fail if we ignore it, which is what we
    // want.

    // Register for submit events, to extract credentials from the URI
    // and to pre-supply credentials for basic
    // authentication. Registered as EStatusCodeHandler+1 so that it
    // isn't offered submit events originating from the 401 handler
    // (which is harmless but slow and confusing)
    
     iSession->StringPool().OpenL( HttpFilterCommonStringsExt::GetTable() );

    if ( iIsProxy )
        {
        iHttpStringAuthorization = HTTP::EProxyAuthorization;
        iHttpStringAuthenticate = HTTP::EProxyAuthenticate;
        iStatusCode = HTTPStatus::EProxyAuthenticationRequired;   // 407
        iRealmStr = iStringPool.StringF( HttpFilterCommonStringsExt::EProxyRealm, HttpFilterCommonStringsExt::GetTable() );
        iUsernameStr = iStringPool.StringF( HttpFilterCommonStringsExt::EProxyUsername, HttpFilterCommonStringsExt::GetTable() );
        iPasswordStr = iStringPool.StringF( HttpFilterCommonStringsExt::EProxyPassword, HttpFilterCommonStringsExt::GetTable() );
        }

    else
        {
        iHttpStringAuthorization = HTTP::EAuthorization;
        iHttpStringAuthenticate = HTTP::EWWWAuthenticate;
        iStatusCode = HTTPStatus::EUnauthorized;       // 401
        iRealmStr = iStringPool.StringF( HTTP::ERealm, RHTTPSession::GetTable() );
        iUsernameStr = iStringPool.StringF( HTTP::EUsername, RHTTPSession::GetTable() );
        iPasswordStr = iStringPool.StringF( HTTP::EPassword, RHTTPSession::GetTable() );
        }
        
    aSession.FilterCollection().AddFilterL( *this,
                                            THTTPEvent::ESubmit,
                                            RStringF(),
                                            KAnyStatusCode,
                                            EStatusCodeHandler + 1,
                                            iStringPool.StringF( HTTP::EAuthentication, RHTTPSession::GetTable() ) );

    // And for 401 status codes, for obvious reasons.
    aSession.FilterCollection().AddFilterL( *this,
                                            THTTPEvent::EGotResponseHeaders,
                                            iStringPool.StringF( iHttpStringAuthenticate, RHTTPSession::GetTable() ),
                                            iStatusCode,
                                            EStatusCodeHandler,
                                            iStringPool.StringF( HTTP::EAuthentication, RHTTPSession::GetTable() ) );

    }

// Destructor
CHttpFilterAuthentication::~CHttpFilterAuthentication()
    {
    TInt credCount = iCredentials.Count();

    for ( TInt ii = 0; ii < credCount; ++ii )
        {
        TCredentials& cred = iCredentials[ ii ];

        // Close all owned strings now while we can get a string pool handle.
        iStringPool.String( cred.iUser ).Close();
        iStringPool.String( cred.iPassword ).Close();
        iStringPool.StringF( cred.iURI ).Close();
        iStringPool.String( cred.iRealm ).Close();
        }

    iCredentials.Close();
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::InstallFilterL
// Two-phased constructor. This function replaces NewL.
// -----------------------------------------------------------------------------
//
CHttpFilterAuthentication* CHttpFilterAuthentication::InstallFilterL( TAny* aAuthenticationParams )
    {
    __ASSERT_DEBUG( aAuthenticationParams != NULL, PanicAuthenticationFilter() );
    TAuthenticationParams* authParams = REINTERPRET_CAST( TAuthenticationParams*, aAuthenticationParams );
    __ASSERT_DEBUG( authParams->iSession != NULL, PanicAuthenticationFilter() );
    CHttpFilterAuthentication* filter = new ( ELeave ) CHttpFilterAuthentication( authParams->iSession, authParams->iIsProxy );
    CleanupStack::PushL( filter );
    filter->ConstructL( *( authParams->iSession ) );
    CleanupStack::Pop( filter );
    return filter;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::MHFLoad
// Called when the filter is being added to the session's filter queue.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::MHFLoad( RHTTPSession, THTTPFilterHandle )
    {
    ++iLoadCount;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::MHFUnload
// Called when the filter is being removed from a session's filter queue.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::MHFUnload( RHTTPSession /*aSession*/, THTTPFilterHandle /*aFilterHandler*/ )
    {
    __ASSERT_DEBUG( iLoadCount >= 0, PanicAuthenticationFilter() );

    if ( --iLoadCount )
        {
        return ;
        }

    delete this;
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::MHFRunL
// Process a transaction event.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::MHFRunL( RHTTPTransaction aTransaction,
        const THTTPEvent& aEvent )
    {
    switch ( aEvent.iStatus )
        {

        case THTTPEvent::ESubmit:
            {
            SubmitL( aTransaction );
            break;
            }

        case THTTPEvent::EGotResponseHeaders:
            {
            HandleTransactionL( aTransaction );
            break;
            }

        default:
            {
            __ASSERT_DEBUG( EFalse, PanicAuthenticationFilter() );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::MHFRunError
// Process an error that occured while processing the transaction.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::MHFRunError( TInt /*aError*/,
        RHTTPTransaction /*aTransaction*/,
        const THTTPEvent& /*aEvent*/ )
    {
    // aTransaction.Fail(); This causes a crash in Symbian code
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::MHFSessionRunL
// Process a session event.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::MHFSessionRunL( const THTTPSessionEvent& /*aEvent*/ )
    {}

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::MHFSessionRunError
// Called when MHFRunL leaves from a session event.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::MHFSessionRunError( TInt aError, const THTTPSessionEvent& /*aEvent*/ )
    {
    return aError;
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::HandleTransactionL
// Provides a central point for all Tranaction handling.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::HandleTransactionL( RHTTPTransaction aTransaction )
    {
    AUTHLOGGER_ENTERFN( "CHttpFilterAuthentication::HandleTransactionL" );

    // Find if we have credentials in the transaction's properties, and remove them from the list
    // (but not from the properties)
    // Find if there are WWWAuthenticate or ProxyAuthenticate headers in the request,
    // and replace the realm property with what came from the request.
    THTTPHdrVal realmVal;
    TInt cred = KErrNotFound;

    if ( aTransaction.PropertySet().Property( iRealmStr, realmVal ) )
        {
        if ( iIsProxy )
            {
            cred = FindCredentialsForRealm( realmVal );
            }

        else
            {
            cred = FindCredentials( realmVal, aTransaction.Request().URI() );
            }
        }

    if ( cred != KErrNotFound )
        {
        RemoveCredentialsFromList( cred );
        }

    RStringF wwwAuthHeader = iStringPool.StringF( iHttpStringAuthenticate, RHTTPSession::GetTable() );
    RHTTPHeaders headers( aTransaction.Response().GetHeaderCollection() );

    if ( iIsProxy )
        {
        AddProxyRealmToTransactionL( aTransaction );
        }

    else
        {
        TInt headerPart = FindHeaderPartToUseL( aTransaction );

        if ( headerPart == KErrNotFound )
            {
			aTransaction.Cancel();
			aTransaction.SendEventL(KErrHttpDecodeUnknownAuthScheme, THTTPEvent::EIncoming, THTTPFilterHandle::ECurrentFilter);
			return;
            }

        THTTPHdrVal authTypeParam;

        if ( headers.GetField( wwwAuthHeader, headerPart, authTypeParam ) == KErrNone &&
             headers.GetParam( wwwAuthHeader, iRealmStr, realmVal, headerPart ) == KErrNone )
            {
            TInt cred = FindCredentialsForRealm( realmVal.Str(), EFalse );

            if( cred != KErrNotFound )
                // We have already met with this realm. Check that it came from the same host.
                {
                const TUriC8& uri = aTransaction.Request().URI();
                TUriParser8 uriCred;

                uriCred.Parse( iStringPool.StringF(iCredentials[cred].iURI).DesC() );

                if( !uriCred.Extract( EUriHost ).CompareF( uri.Extract( EUriHost ) ) )
                    // the host and the realm are the same as previously
                    // we most probably authenticated to this server
                    // use the previously sent credentials
                    {
                    aTransaction.PropertySet().RemoveProperty( iRealmStr );
                    aTransaction.PropertySet().RemoveProperty( iUsernameStr );
                    aTransaction.PropertySet().RemoveProperty( iPasswordStr );

                    aTransaction.PropertySet().SetPropertyL( iRealmStr, realmVal );

                    SetPropertyL( aTransaction, iUsernameStr, iStringPool.String( iCredentials[ cred ].iUser).DesC() );
                    SetPropertyL( aTransaction, iPasswordStr, iStringPool.String( iCredentials[ cred ].iPassword).DesC() );

                    // 'stale' is used in digest authentication only. But we can
                    // take its advantage here as well. Because we are working
                    // in the same protection space as before thus we know 
                    // the uname/pwd -> don't need to ask it again from user,
                    // or show dialog the authentication failed.
                    SetPropertyL( aTransaction, 
                                  iSession->StringPool().StringF( HTTP::EStale, 
                                                                  RHTTPSession::GetTable() ),
                                  KNullDesC8 );
                    }
                }
            else
                {
                aTransaction.PropertySet().RemoveProperty( iRealmStr );
                aTransaction.PropertySet().SetPropertyL( iRealmStr, realmVal );

                SetBasicProperty( aTransaction );
                }
            }
        }
    AUTHLOGGER_LEAVEFN( "CHttpFilterAuthentication::HandleTransactionL" );
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::SubmitL
// Function to handle Submit events. We need to remove any username and password from the URI.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::SubmitL( RHTTPTransaction aTransaction )
    {
    // From RFC 2617:
    // A client SHOULD assume that all paths at or deeper than the depth of
    // the last symbolic element in the path field of the Request-URI also
    // are within the protection space specified by the Basic realm value of
    // the current challenge. A client MAY preemptively send the
    // corresponding Authorization header with requests for resources in
    // that space without receipt of another challenge from the server.

    // First time the transaction is submitted, we check if the credentials are appended to the url,
    // remove it from the url and add it to the transaction's properties.
    // Then we check if there are credentials in the transaction's properties,
    // and add it to the credentials list.
    // Last thing is to lookup the list for credentials for:
    //     WWWAuthenticate: match the URI.
    //     ProxyAuthenticate: match the realm.
    TInt cred = KErrNotFound;
    // Get credentials from URI, only for wwwAuthenticate

    // we don't need this property anymore
    aTransaction.PropertySet().RemoveProperty( iStringPool.StringF( HTTP::EBasic,
                                               RHTTPSession::GetTable() ) );

    if ( !iIsProxy )
        {
        GetCredentialsFromURIL( aTransaction );
        }

    // If there are credentials in the transaction properties, add them to the list
    GetCredentialsFromPropertiesL( aTransaction );

    if ( iIsProxy )
        {
        THTTPHdrVal realmVal;

        if ( aTransaction.PropertySet().Property( iRealmStr, realmVal ) )
            {
            cred = FindCredentialsForRealm( realmVal.Str() );
            }

        else
            {
            cred = FindCredentialsForProxy();
            }
        }

    else
        {
        // If we have credentials for this URL, add them
        cred = FindCredentialsForURI( aTransaction.Request().URI() );
        }

    if ( cred != KErrNotFound )
        {
        EncodeBasicAuthL( iStringPool.String( iCredentials[ cred ].iUser ),
                          iStringPool.String( iCredentials[ cred ].iPassword ),
                          aTransaction );

        if( !iIsProxy )
            {
            SetPropertyL( aTransaction, iUsernameStr, iStringPool.String( iCredentials[ cred ].iUser ).DesC() );
            SetPropertyL( aTransaction, iPasswordStr, iStringPool.String( iCredentials[ cred ].iPassword ).DesC() );
            SetPropertyL( aTransaction, iRealmStr, iStringPool.String( iCredentials[ cred ].iRealm ).DesC() );
            }
        }
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindCredentials
// Search list for matching credentials.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindCredentials( const RString& aRealm,
        const TUriC8& aURI )
    {
    TInt count = iCredentials.Count();

    for ( TInt cred = 0; cred < count; ++cred )
        {
        if ( iCredentials[ cred ].iRealm == aRealm )
            {
            TUriParser8 parser;
            parser.Parse( iStringPool.StringF( iCredentials[ cred ].iURI ).DesC() );

            if ( !parser.Compare(aURI, EUriScheme) &&
                 !parser.Compare( aURI, EUriHost ) &&
                 ( ( !parser.Compare( aURI, EUriPort ) )
                   || ( !parser.IsPresent( EUriPort ) &&
                        !aURI.IsPresent( EUriPort ) ) ) )
                {
                return cred;
                }
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindCredentialsForURI
// Searches list for credentials that can be used with this URI
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindCredentialsForURI( const TUriC8& aURI )
    {
    // Check if any of the stored URIs are the beginning of the URI
	// we're trying now. If they are, we can immediately attempt to
	// re-use the existing credentials.
	TInt count = iCredentials.Count();
	for (TInt cred = 0; cred < count; ++cred)
		{
		const TDesC8& transDes = aURI.UriDes();
		const TDesC8& credDes = 
			iStringPool.StringF(iCredentials[cred].iURI).DesC();
		if (transDes.Length() >= credDes.Length())
			{
			// The URI is long enough, which is a good start.
			if (transDes.Left(credDes.Length()).Compare(credDes) == 0)
				{
				// The descriptors match. Things are looking good. In
				// the interests of paranoia, if we haven't matched
				// the entire URI, check that the character after the
				// match is '/'.
				if (transDes.Length() == credDes.Length())
                {
					return cred;
                }
				else if (transDes[credDes.Length()] == '/')
                {
					return cred;
                }
				}
			}
		}
	return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindCredentialsForRealm
// Search list for matching credentials for a given realm.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindCredentialsForRealm( const RString& aRealm,
                                                         TBool aCheckProxy )
    {
    RStringF proxyName;
    TInt count = iCredentials.Count();

    if( aCheckProxy )
        {
        TBool ret = GetProxyName( proxyName );

        if ( ret )
            {
            for ( TInt cred = 0; cred < count; ++cred )
                {
                if ( iCredentials[ cred ].iRealm == aRealm && iCredentials[ cred ].iURI == proxyName )
                    {
                    return cred;
                    }
                }
            }
        }
    else
        {
        for ( TInt cred = 0; cred < count; ++cred )
            {
            if ( iCredentials[ cred ].iRealm == aRealm )
                {
                return cred;
                }
            }
        }
    AUTHLOGGER_LEAVEFN( "CHttpFilterAuthentication::FindCredentialsForRealm" );
    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindCredentialsForProxy
// Search list for matching credentials based on proxy used.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindCredentialsForProxy()
    {
    RStringF proxyName;
    TInt cred = KErrNotFound;
    TInt count = iCredentials.Count();
    TBool ret = GetProxyName( proxyName );

    if ( ret )
        {
        for ( cred = 0; cred < count; ++cred )
            {
            if ( iCredentials[ cred ].iURI == proxyName )
                {
                return cred;
                }
            }
        }

    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::EncodeBasicAuthL
// Set basic auth header.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::EncodeBasicAuthL( const RString& aUsername,
        const RString& aPW,
        RHTTPTransaction& aTransaction )
    {
    AUTHLOGGER_ENTERFN( "CHttpFilterAuthentication::EncodeBasicAuthL" );

    // Standard, plain-text HTTP - Base 64 the name and password
    TBase64 codec;
    HBufC8* nameAndPW = HBufC8::NewMaxLC( aUsername.DesC().Length() + aPW.DesC().Length() + 1 );
    _LIT8( KBasicAuthFormatString, "%S:%S" );
    nameAndPW->Des().Format( KBasicAuthFormatString, &aUsername.DesC(), &aPW.DesC() );

    // Conservatively allocate a buffer twice as big as the unencoded
    // buffer for the encoded string.
    HBufC8* encoded = HBufC8::NewMaxLC( nameAndPW->Des().Length() * 2 );
    TPtr8 encodedPtr( encoded->Des() );
    codec.Encode( *nameAndPW, encodedPtr );
    HBufC8* authHeaderField = HBufC8::NewLC( encodedPtr.Length() + 1 +
                              iStringPool.StringF( HTTP::EBasic, RHTTPSession::GetTable() ).DesC().Length() );
    authHeaderField->Des().Copy( iStringPool.StringF( HTTP::EBasic, RHTTPSession::GetTable() ).DesC() );
    _LIT8( KSpace, " " );
    authHeaderField->Des().Append( KSpace );
    authHeaderField->Des().Append( encoded->Des() );

    RString encodedString = iStringPool.OpenStringL( *authHeaderField );
    CleanupStack::PopAndDestroy( 3 ); // authHeaderField, encoded, nameAndPW

    CleanupClosePushL( encodedString );

    RHTTPHeaders requestHeaders( aTransaction.Request().GetHeaderCollection() );

    requestHeaders.RemoveField( iStringPool.StringF( iHttpStringAuthorization, RHTTPSession::GetTable() ) );

    requestHeaders.SetFieldL( iStringPool.StringF( iHttpStringAuthorization, RHTTPSession::GetTable() ),
                              THTTPHdrVal( encodedString ) );

    CleanupStack::PopAndDestroy(); // encodedString
    AUTHLOGGER_LEAVEFN( "CHttpFilterAuthentication::EncodeBasicAuthL" );
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindHeaderPartToUseL
// Find the Authentication header.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindHeaderPartToUseL( RHTTPTransaction aTransaction )
    {
    AUTHLOGGER_ENTERFN( "CHttpFilterAuthentication::FindHeaderPartToUseL" );

    // There may be several different authentication fields. We need
    // to chose the strongest one we understnad. Currently, we only
    // support 2, Basic and Digest, and Digest is the strongest
    // assuming there is a qop that we can accept. Therefore, we keep
    // track of the last good basic one we found, and return the
    // moment we find an acceptable digest one.

    // While we're here, we check that all desired fields are there.

    RStringF wwwAuthenticate = iStringPool.StringF( iHttpStringAuthenticate, RHTTPSession::GetTable() );
    TInt lastGoodBasic = KErrNotFound;
    RHTTPHeaders headers = aTransaction.Response().GetHeaderCollection();
    const TInt parts = headers.FieldPartsL( wwwAuthenticate );

    for ( TInt ii = 0; ii < parts; ii++ )
        {
        THTTPHdrVal fieldVal; // The name of the current field.
        THTTPHdrVal hdrVal; //A scratch hdrVal
				// coverity [check_return]
				// coverity [unchecked_value]
        headers.GetField( wwwAuthenticate, ii, fieldVal );

        switch ( fieldVal.StrF().Index( RHTTPSession::GetTable() ) )
            {

            case HTTP::EBasic:

            if ( headers.GetParam( wwwAuthenticate, iRealmStr, hdrVal, ii ) == KErrNone )
                lastGoodBasic = ii;

            break;

            default:
            // We don't understand this, whatever it is. Ignore it.
            break;
            }
        }
    AUTHLOGGER_LEAVEFN( "CHttpFilterAuthentication::FindHeaderPartToUseL" );
    return lastGoodBasic;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::GetCredentialsFromURIL
// Find credentials from the transaction's URI.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::GetCredentialsFromURIL( RHTTPTransaction& aTransaction )
    {
    AUTHLOGGER_ENTERFN( "CHttpFilterAuthentication::GetCredentialsFromURIL" );

    const TUriC8 & uri = aTransaction.Request().URI();

    if ( uri.IsPresent( EUriUserinfo ) )
        {
        const TPtrC8 userInfo = uri.Extract( EUriUserinfo );
        TInt colonPos = userInfo.Locate( ':' );

        if ( colonPos == KErrNotFound )
            colonPos = userInfo.Length();

        if ( colonPos )
            {
            // We have a username
            TPtrC8 username = userInfo.Left( colonPos );
            SetPropertyL( aTransaction, iUsernameStr, username );
            }

        if ( colonPos != userInfo.Length() )
            {
            // We have a password
            TPtrC8 password = userInfo.Mid( colonPos + 1 );
            SetPropertyL( aTransaction, iPasswordStr, password );
            }

        if ( colonPos && uri.IsPresent( EUriHost ) )
            {
            // Get the realm only if there is a username
            TPtrC8 realm = uri.Extract( EUriHost );
            SetPropertyL( aTransaction, iRealmStr, realm );
            }

        // Now remove the username and password from the URI
        CUri8* newUri = CUri8::NewLC( uri );

        newUri->RemoveComponentL( EUriUserinfo );

        aTransaction.Request().SetURIL( newUri->Uri() );

        CleanupStack::PopAndDestroy( newUri ); // newUri
        }
    AUTHLOGGER_LEAVEFN( "CHttpFilterAuthentication::GetCredentialsFromURIL" );
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::GetCredentialsFromPropertiesL
// Find credentials from the transaction's properties.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::GetCredentialsFromPropertiesL( RHTTPTransaction& aTransaction )
    {
    AUTHLOGGER_ENTERFN( "CHttpFilterAuthentication::GetCredentialsFromPropertiesL" );

    THTTPHdrVal usernameVal;
    THTTPHdrVal passwordVal;
    THTTPHdrVal realmVal;
    const TUriC8& uri = aTransaction.Request().URI();
    RHTTPTransactionPropertySet propSet = aTransaction.PropertySet();

    if ( propSet.Property( iUsernameStr, usernameVal ) &&
         propSet.Property( iPasswordStr, passwordVal ) &&
         propSet.Property( iRealmStr, realmVal ) )
        {
        TInt cred;
        RStringF uriStr;

        if ( iIsProxy )
            {
            if ( ( cred = FindCredentialsForRealm( realmVal ) ) != KErrNotFound )
                {
                // Remove old credentials from the list
                RemoveCredentialsFromList( cred );
                }

            if ( GetProxyName( uriStr ) )
                {
                // Add credentials to the list from the transaction's properties
                AddCredentialsToListL( usernameVal.Str().Copy(), passwordVal.Str().Copy(),
                                       realmVal.Str().Copy(), uriStr.Copy() );
                }
            }

        else
            {
            if ( ( cred = FindCredentialsForURI( uri ) ) != KErrNotFound )
                {
                // Remove old credentials from the list
                RemoveCredentialsFromList( cred );
                }

            TPtrC8 uriPathPtr( uri.UriDes() );

            if ( uriPathPtr.LocateReverse( '/' ) > 0 )
                {
                uriPathPtr.Set( uriPathPtr.Left( uriPathPtr.LocateReverse( '/' ) ) );
                }

            uriStr = iStringPool.OpenFStringL( uriPathPtr );
            // Add credentials to the list from the transaction's properties
            AddCredentialsToListL( usernameVal.Str().Copy(), passwordVal.Str().Copy(),
                                   realmVal.Str().Copy(), uriStr );
            }
        }
    AUTHLOGGER_LEAVEFN( "CHttpFilterAuthentication::GetCredentialsFromPropertiesL" );
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::RemoveCredentialsFromList
// Remove credentials from the list.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::RemoveCredentialsFromList( TInt aCredId )
    {
    __ASSERT_DEBUG( aCredId >= 0 && aCredId < iCredentials.Count(), PanicAuthenticationFilter() );

    TCredentials& creds = iCredentials[ aCredId ];
    iStringPool.String( creds.iUser ).Close();
    iStringPool.String( creds.iPassword ).Close();
    iStringPool.StringF( creds.iURI ).Close();
    iStringPool.String( creds.iRealm ).Close();
    iCredentials.Remove( aCredId );
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::AddCredentialsToListL
// Store credentials in the list.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::AddCredentialsToListL( RString aUsernameStr,
        RString aPasswordStr,
        RString aRealmStr,
        RStringF aUriStr )
    {
    TCredentials newCred;
    newCred.iUser = aUsernameStr;
    newCred.iPassword = aPasswordStr;
    newCred.iRealm = aRealmStr;
    newCred.iURI = aUriStr;
    newCred.iID = iNextID++;
    User::LeaveIfError( iCredentials.Append( newCred ) );
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::TrimLWSLeft
// Remove white spaces from the left.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::TrimLWSLeft( TPtrC8& aPtr )
    {
    TInt i;
    _LIT8( KSpaceNewLineTab, " \r\n\t" );
    TInt len = aPtr.Length();

    for ( i = 0; i < len; i++ )
        {
        if ( KSpaceNewLineTab().Locate( aPtr[ i ] ) == KErrNotFound )
            {
            break;
            }
        }

    if ( i == len )
        {
        aPtr.Set( NULL, 0 );
        }

    else
        {
        aPtr.Set( aPtr.Right( len - i ) );
        }
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::TrimLWSRight
// Remove white spaces from the right.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::TrimLWSRight( TPtrC8& aPtr )
    {
    TInt i;
    _LIT8( KSpaceNewLineTab, " \r\n\t" );
    TInt len = aPtr.Length();

    for ( i = len - 1; i >= 0; i-- )
        {
        if ( KSpaceNewLineTab().Locate( aPtr[ i ] ) == KErrNotFound )
            {
            break;
            }
        }

    if ( i == -1 )
        {
        aPtr.Set( NULL, 0 );
        }

    else
        {
        aPtr.Set( aPtr.Left( i + 1 ) );
        }
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindCharNotInQoutes
// Find a character in a buffer, but not in a quoted string.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindCharNotInQoutes( TPtrC8& aPtr, TUint8 aCharToFind )
    {
    TInt len = aPtr.Length();
    TInt i;

    for ( i = 0; i < len; i++ )
        {
        if ( aPtr[ i ] == aCharToFind )
            {
            return i;
            }

        if ( aPtr[ i ] == '"' )
            {
            for ( ; i < len && i != '"'; i++ )

                ;
            }
        }

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindCharNotInQoutes
// Find a substring in a buffer, but not in a quoted string.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindCharNotInQoutes( TPtrC8& aPtr, const TPtrC8& aCharsToFind )
    {
    TInt len = aPtr.Length();
    TInt i;

    for ( i = 0; i < len; i++ )
        {
        if ( aCharsToFind.Locate( aPtr[ i ] ) != KErrNotFound )
            return i;

        if ( aPtr[ i ] == '"' )
            {
            for ( ; i < len && aPtr[ i ] != '"'; i++ )

                ;
            }
        }

    return KErrNotFound;
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::FindNextTokenInHeaderFieldL
// Find a next token in the header.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterAuthentication::FindNextTokenInHeaderFieldL( TPtrC8& aHeaderPtr, TPtrC8& aTokenPtr )
    {
    _LIT8( KCommaSpaceNewLineTab, ", \r\n\t" );
    aTokenPtr.Set( aHeaderPtr );
    TrimLWSLeft( aTokenPtr ); // Skip leading spaces
    TInt leadingSpaces = aHeaderPtr.Length() - aTokenPtr.Length();

    if ( aTokenPtr.Length() == 0 )
        return KErrNotFound;

    TInt i = FindCharNotInQoutes( aTokenPtr, KCommaSpaceNewLineTab() ); // Find separator after leading spaces

    if ( i == KErrNotFound )
        {
        i = aHeaderPtr.Length();
        }

    else
        {
        aTokenPtr.Set( aTokenPtr.Left( i ) );
        i += leadingSpaces; // Add the number of leading spaces
        i++; // Advance beyond separator
        }

    return i;
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::AddProxyRealmToTransactionL
// Find the proxy realm in the response header and add it to the transaction's properties.
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::AddProxyRealmToTransactionL( RHTTPTransaction aTransaction )
    {
    AUTHLOGGER_ENTERFN( "CHttpFilterAuthentication::AddProxyRealmToTransactionL" );

    RStringF authenticateStr = iStringPool.StringF( HTTP::EProxyAuthenticate, RHTTPSession::GetTable() );
    RHTTPHeaders headers = aTransaction.Response().GetHeaderCollection();
    const TInt parts = headers.FieldPartsL( authenticateStr );
    TInt i;
    TInt offset = 0;
    TInt equalOffset = 0;
    TBool found = EFalse;
    THTTPHdrVal fieldVal;
    TPtrC8 headerPtr;
    TPtrC8 tokenPtr;
    TPtrC8 partTokenPtr;
    RStringF tokenStr;
    RString realmStr;

    for ( i = 0; i < parts; i++ )
        {
        if ( headers.GetField( authenticateStr, i, fieldVal ) == KErrNotFound )
            {
            continue;
            }

        headerPtr.Set( fieldVal.StrF().DesC() );

        while ( ( offset = FindNextTokenInHeaderFieldL( headerPtr, tokenPtr ) ) != KErrNotFound )     // Get next token
            {
            headerPtr.Set( headerPtr.Right( headerPtr.Length() - offset ) ); // Advance headerPtr
            tokenStr = iStringPool.OpenFStringL( tokenPtr );

            switch ( tokenStr.Index( RHTTPSession::GetTable() ) )     // Is it Basic?
                {

                case HTTP::EBasic:
                    {
                    found = ETrue;
                    break;
                    }

                default:
                    {
                    found = EFalse;
                    break;
                    }

                }
            tokenStr.Close();

            if ( !found )
                {
                continue;
                }

            offset = FindNextTokenInHeaderFieldL( headerPtr, tokenPtr ); // Find next token
            headerPtr.Set( headerPtr.Right( headerPtr.Length() - offset ) ); // Advance headerPtr

            equalOffset = FindCharNotInQoutes( tokenPtr, '=' );

            if ( equalOffset < 1 )     // Not the first character
                {
                continue;
                }

            partTokenPtr.Set( tokenPtr.Left( equalOffset ) );
            tokenStr = iStringPool.OpenFStringL( partTokenPtr );

            switch ( tokenStr.Index( RHTTPSession::GetTable() ) )     // Is it realm?
                {

                case HTTP::ERealm:
                    {
                    found = ETrue;
                    break;
                    }

                default:
                    {
                    found = EFalse;
                    break;
                    }

                }
            tokenStr.Close();

            if ( !found )
                {
                continue;
                }

            partTokenPtr.Set( tokenPtr.Right( tokenPtr.Length() - equalOffset - 1 ) );

            if ( partTokenPtr.Length() > 0 )
                {
                realmStr = iStringPool.OpenStringL( partTokenPtr );
                CleanupClosePushL( realmStr );
                aTransaction.PropertySet().RemoveProperty( iRealmStr );
                aTransaction.PropertySet().SetPropertyL( iRealmStr, realmStr );
                CleanupStack::PopAndDestroy(); // realmStr

                SetBasicProperty( aTransaction );
                }

            }
        }
    AUTHLOGGER_LEAVEFN( "CHttpFilterAuthentication::AddProxyRealmToTransactionL" );
    }


// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::GetProxyName
// Get proxy name from the HTTP session.
// -----------------------------------------------------------------------------
//
TBool CHttpFilterAuthentication::GetProxyName( RStringF &aProxyName )
    {
    RHTTPConnectionInfo connInfo = iSession->ConnectionInfo();
    THTTPHdrVal proxyAddrVal;


    TBool ret = connInfo.Property( iStringPool.StringF( HTTP::EProxyAddress,
                                   RHTTPSession::GetTable() ), proxyAddrVal );

    if ( ret )
        aProxyName = proxyAddrVal.StrF();

    return ret;
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::SetPropertyL
// Set property of the transaction
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::SetPropertyL( RHTTPTransaction& aTransaction,
                                              RStringF aPropertyName, 
                                              const TDesC8& aToken )
    {
    RString tokenStr = iStringPool.OpenStringL( aToken );
    THTTPHdrVal tokenVal = tokenStr;
    CleanupClosePushL( tokenStr );
    aTransaction.PropertySet().SetPropertyL( aPropertyName, tokenVal );
    CleanupStack::PopAndDestroy(); // tokenStr
    }

// -----------------------------------------------------------------------------
// CHttpFilterAuthentication::SetBasicProperty()
// Set 'Basic' in property set indicating that this is a
// basic authentication
// -----------------------------------------------------------------------------
//
void CHttpFilterAuthentication::SetBasicProperty( RHTTPTransaction aTransaction )
    {
    // the value is dummy. The property needs to be there only
    THTTPHdrVal value( 1 ); 
    aTransaction.PropertySet().RemoveProperty( 
    iStringPool.StringF( HTTP::EBasic, 
                         RHTTPSession::GetTable() ) );
    aTransaction.PropertySet().SetPropertyL( 
    iStringPool.StringF( HTTP::EBasic, 
                         RHTTPSession::GetTable() ), 
                         value );
    }

//  End of File

