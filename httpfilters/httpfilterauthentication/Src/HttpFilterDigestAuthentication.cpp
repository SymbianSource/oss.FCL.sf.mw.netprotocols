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
* Description:  Digest Authentication filter
*
*/



// INCLUDE FILES
#include <e32std.h>
#include <e32math.h>
#include <hash.h>
#include <http/rhttpsession.h>
#include <http/rhttptransaction.h>
#include <http/rhttpheaders.h>
#include <httpstringconstants.h>
#include <httperr.h>

// User Includes
#include "HttpFilterDigestAuthentication.h"
#include "HttpFilterAuthenticationLogger.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// Length of a digest hash when represented in hex
const TInt KHashLength = 32;
// Length of a digest hash before converting to hex.
const TInt KRawHashLength = 16;
// Length of nonce-count
const TInt KNonceCountLength = 8;

_LIT8( KMd5AlgorithmStr, "MD5" );
_LIT8( KMd5SessAlgorithmStr, "MD5-sess" );
_LIT8( KQopAuthStr, "auth" );
_LIT8( KQopAuthIntStr, "auth-int" );
_LIT8( KAuthenticationInfoStr, "Authentication-Info" );
_LIT8( KColon, ":" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PanicDigestAuthenticationFilter
// Panic
// Returns: void
// -----------------------------------------------------------------------------
//
void PanicDigestAuthenticationFilter( TInt aErr = 0 )
    {
    User::Panic( _L( "HTTP FILTER DIGEST AUTHENTICATION" ), aErr );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::CHttpFilterDigestAuthentication
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CHttpFilterDigestAuthentication::CHttpFilterDigestAuthentication( RHTTPSession* aSession, TBool aIsProxy )
    : CHttpFilterAuthentication( aSession, aIsProxy )
    {
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::ConstructL( RHTTPSession aSession )
    {
    AUTHLOGGER_ENTERFN( "ConstructL" );
    CHttpFilterAuthentication::ConstructL( aSession );
    
    iOpaqueStr = iStringPool.StringF( HTTP::EOpaque, RHTTPSession::GetTable() );
    iNonceStr = iStringPool.StringF( HTTP::ENonce, RHTTPSession::GetTable() );
    iQopStr = iStringPool.StringF( HTTP::EQop, RHTTPSession::GetTable() );
    iStaleStr = iStringPool.StringF( HTTP::EStale, RHTTPSession::GetTable() );
    
    iMd5Str = iStringPool.OpenFStringL( KMd5AlgorithmStr );
    iMd5SessStr = iStringPool.OpenFStringL( KMd5SessAlgorithmStr );
    iQopAuthStr = iStringPool.OpenFStringL( KQopAuthStr );
    iAuthInfo = iStringPool.OpenFStringL( KAuthenticationInfoStr );

    aSession.FilterCollection().AddFilterL( *this,
                                            THTTPEvent::EGotResponseHeaders,
                                            iAuthInfo,
                                            HTTPStatus::EOk,
                                            EStatusCodeHandler + 1,
                                            iStringPool.StringF( HTTP::EAuthentication, RHTTPSession::GetTable() ) );

    AUTHLOGGER_WRITE( "CMD5::NewL" );
    iMD5Calculator = CMD5::NewL();

    AUTHLOGGER_LEAVEFN( "ConstructL" );
    }

// Destructor
CHttpFilterDigestAuthentication::~CHttpFilterDigestAuthentication()
    {
    iQopAuthStr.Close();
    iMd5SessStr.Close();
    iMd5Str.Close();
    iAuthInfo.Close();

    delete iMD5Calculator;

    // just in case...
    for( TInt i = 0; i < iDCredentials.Count(); ++i )
        {
        DRemoveCredentialsFromList( 0 );
        }

    iDCredentials.Close();
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::InstallFilterL
// Two-phased constructor. This function replaces NewL.
// -----------------------------------------------------------------------------
//
CHttpFilterDigestAuthentication* CHttpFilterDigestAuthentication::InstallFilterL( TAny* aAuthenticationParams )
    {
    AUTHLOGGER_CREATE;

    __ASSERT_DEBUG( aAuthenticationParams != NULL, PanicDigestAuthenticationFilter() );
    TAuthenticationParams* authParams = REINTERPRET_CAST( TAuthenticationParams*, aAuthenticationParams );
    __ASSERT_DEBUG( authParams->iSession != NULL, PanicDigestAuthenticationFilter() );
    CHttpFilterDigestAuthentication* filter = new ( ELeave ) CHttpFilterDigestAuthentication( authParams->iSession, authParams->iIsProxy );
    CleanupStack::PushL( filter );
    filter->ConstructL( *( authParams->iSession ) );
    CleanupStack::Pop( filter );
    return filter;
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::MHFRunL
// Process a transaction event.
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::MHFRunL( RHTTPTransaction aTransaction,
        const THTTPEvent& aEvent )
    {
    switch ( aEvent.iStatus )
        {

        case THTTPEvent::ESubmit:
            {
            DSubmitL( aTransaction );
            break;
            }

        case THTTPEvent::EGotResponseHeaders:
            {
            if( aTransaction.Response().StatusCode() == HTTPStatus::EOk )
                {
                ParseAuthenticationInfoL( aTransaction );
                }
            else
                {
                DHandleTransactionL( aTransaction );
                }
            break;
            }

        default:
            {
            __ASSERT_DEBUG( EFalse, PanicDigestAuthenticationFilter() );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::ParseAuthenticationInfoL
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::ParseAuthenticationInfoL( RHTTPTransaction /*aTransaction*/ )
    {
    // TODO: parse and store nc-value
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::HandleTransactionL
// Provides a central point for all Transaction handling.
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::DHandleTransactionL( RHTTPTransaction aTransaction )
    {
    AUTHLOGGER_ENTERFN( "DHandleTransactionL" );

    // Find if we have credentials in the transaction's properties, and remove them from the list
    // (but not from the properties)
    // Find if there are WWWAuthenticate or ProxyAuthenticate headers in the request,
    // and replace the realm property with what came from the request.
    THTTPHdrVal headerVal;
    TInt cred = KErrNotFound;
    TBool doDigest = EFalse;
    TBool stale( EFalse );

    if ( aTransaction.PropertySet().Property( iRealmStr, headerVal ) )
        {
        if ( iIsProxy )
            {
            // do nothing this time
            }

        else
            {
            cred = DFindCredentials( headerVal, aTransaction.Request().URI() );
            }
        }

    RStringF wwwAuthHeader = iStringPool.StringF( iHttpStringAuthenticate, RHTTPSession::GetTable() );
    RHTTPHeaders headers( aTransaction.Response().GetHeaderCollection() );

    if ( iIsProxy )
        {
        // do nothing this time
        }

    else
        {
        TInt headerPart = DFindHeaderPartToUseL( aTransaction );

        if ( headerPart != KErrNotFound )
            {
            THTTPHdrVal authTypeParam;

            if ( headers.GetField( wwwAuthHeader, headerPart, authTypeParam ) == KErrNone )
                // we need to store realm, opaque, nonce and qop values to
                // use them when resubmitting the request
                {
                THTTPHdrVal algorithmVal;
                THTTPHdrVal nonceVal;
                THTTPHdrVal realmVal;
                THTTPHdrVal opaqueVal;
                THTTPHdrVal staleVal;
                TInt algVal( KAlgUnknown );
                TInt authVal( KQopNone );

                aTransaction.PropertySet().RemoveProperty( iNonceStr );
                aTransaction.PropertySet().RemoveProperty( iRealmStr );
                aTransaction.PropertySet().RemoveProperty( iOpaqueStr );
                aTransaction.PropertySet().RemoveProperty( iQopStr );
                aTransaction.PropertySet().RemoveProperty( iStaleStr );

                if( !headers.GetParam( wwwAuthHeader, 
                                      iStringPool.StringF( HTTP::EAlgorithm, 
                                                      RHTTPSession::GetTable() ), 
                                      algorithmVal, 
                                      headerPart ) )
                    {
                    if( !algorithmVal.Str().DesC().CompareF( iMd5Str.DesC() ) )
                        {
                        algVal = KAlgMd5;
                        }
                    else if( !algorithmVal.Str().DesC().CompareF( iMd5SessStr.DesC() ) )
                        {
                        algVal = KAlgMd5Sess;
                        }
                    }

                authVal = CheckQop( headers, wwwAuthHeader, headerPart );

                headers.GetParam( wwwAuthHeader, iNonceStr, nonceVal, headerPart );
                headers.GetParam( wwwAuthHeader, iRealmStr, realmVal, headerPart );
                headers.GetParam( wwwAuthHeader, iOpaqueStr, opaqueVal, headerPart );
                headers.GetParam( wwwAuthHeader, iStaleStr, staleVal, headerPart );

                // There's only MD5 and MD5-sess algorithm and QoP-Auth support
                // Realm and Nonce values are mandatory. W/o them digest response is meaningless.
                if( ( authVal & KQopAuth || authVal == KQopNone )
                    && nonceVal.Type() == THTTPHdrVal::KStrVal
                    && realmVal.Type() == THTTPHdrVal::KStrVal )
                    {
                    aTransaction.PropertySet().SetPropertyL( iNonceStr, nonceVal );
                    aTransaction.PropertySet().SetPropertyL( iRealmStr, realmVal );
                    // Opaque
                    if( opaqueVal.Type() == THTTPHdrVal::KStrVal )
                        {
                        aTransaction.PropertySet().SetPropertyL( iOpaqueStr, opaqueVal );
                        }

                    // Qop
                    aTransaction.PropertySet().SetPropertyL( iQopStr, authVal );
                    // Algorithm
                    aTransaction.PropertySet().SetPropertyL( iStringPool.StringF( HTTP::EAlgorithm, 
                                                              RHTTPSession::GetTable() ), 
                                                              algVal );

                    _LIT8( KTrue, "TRUE" );
                    if( staleVal.Type() == THTTPHdrVal::KStrVal 
                        && !staleVal.Str().DesC().CompareF( KTrue ) )
                        // stale is true -> nonce expired
                        {
                        AUTHLOGGER_WRITE( "Stale found" );

                        __ASSERT_DEBUG( cred != KErrNotFound, PanicDigestAuthenticationFilter() );

                        // update nonce value
                        TDCredentials& creds = iDCredentials[cred];
                        iStringPool.String( creds.iNonce ).Close();
                        creds.iNonce = nonceVal.Str().Copy();

                        // indicate to engine that no uname/pwd dialog needed
                        staleVal.SetInt( 1 );
                        aTransaction.PropertySet().SetPropertyL( iStaleStr, staleVal );
                        stale = ETrue;
                        }

                    doDigest = ETrue;
                    }
                }
            }
        }

    if ( cred != KErrNotFound && !stale )
        // if stale is true credentials were ok, 
        // only the nonce value was expired.
        {
        DRemoveCredentialsFromList( cred );
        }

    if( !doDigest )
        //try with Basis
        {
        CHttpFilterAuthentication::HandleTransactionL( aTransaction );
        }

    AUTHLOGGER_LEAVEFN( "DHandleTransactionL" );
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::SubmitL
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::DSubmitL( RHTTPTransaction aTransaction )
    {
    TInt cred = KErrNotFound;
    // Get credentials from URI, only for wwwAuthenticate

    // If there are credentials in the transaction properties, add them to the list
    DGetCredentialsFromPropertiesL( aTransaction );

    // just in case we always remove stale value from properties
    aTransaction.PropertySet().RemoveProperty( iStaleStr );

    if ( iIsProxy )
        {
        // do nothing this time
        }
    else
        {
        // If we have credentials for this URL, add them
        cred = DFindCredentialsForURI( aTransaction.Request().URI() );
        }

    if ( cred != KErrNotFound )
        {
        EncodeDigestAuthL( cred, aTransaction );
        }
    else
        // Do Basic
        {
        CHttpFilterAuthentication::SubmitL( aTransaction );
        }
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::DFindCredentials
// Search list for matching credentials.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterDigestAuthentication::DFindCredentials( const RString& aRealm,
        const TUriC8& aURI )
    {
    TInt count = iDCredentials.Count();

    for ( TInt cred = 0; cred < count; ++cred )
        {
        if ( iDCredentials[ cred ].iRealm == aRealm )
            {
            TUriParser8 parser;
            parser.Parse( iStringPool.StringF( iDCredentials[ cred ].iURI ).DesC() );

            if ( !parser.Compare( aURI, EUriHost ) &&
                 ( ( !parser.Compare( aURI, EUriPort ) )
                   || ( !parser.IsPresent( EUriPort ) &&
                        !aURI.IsPresent( EUriPort ) ) ) )
                {
                AUTHLOGGER_WRITE_FORMAT( "Found cred(3): %d", cred );
                return cred;
                }
            }
        }

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::DFindCredentialsForURI
// Searches list for credentials that can be used with this URI
// -----------------------------------------------------------------------------
//
TInt CHttpFilterDigestAuthentication::DFindCredentialsForURI( const TUriC8& aURI )
    {
    // Check if any of the stored URIs are the beginning of the URI
    // we're trying now. If they are, we can immediately attempt to
    // re-use the existing credentials.
    _LIT8 ( KSchemeEnd, "://" );
    TInt count = iDCredentials.Count();

    for ( TInt cred = 0; cred < count; ++cred )
        {
        const TPtrC8& transDes = aURI.UriDes();
        const TPtrC8& credDes =
            iStringPool.StringF( iDCredentials[ cred ].iURI ).DesC();
        TPtrC8 transNoSchemePtr( transDes );
        TPtrC8 credNoSchemePtr( credDes );

        if ( transNoSchemePtr.Find( KSchemeEnd() ) > 0 )
            {
            transNoSchemePtr.Set( transNoSchemePtr.Mid( transNoSchemePtr.Find( KSchemeEnd ) ) );
            }

        if ( credNoSchemePtr.Find( KSchemeEnd() ) > 0 )
            {
            credNoSchemePtr.Set( credNoSchemePtr.Mid( credNoSchemePtr.Find( KSchemeEnd ) ) );
            }

        if ( transNoSchemePtr.Length() >= credNoSchemePtr.Length() )
            {
            // The URI is long enough, which is a good start.

            if ( transNoSchemePtr.Left( credNoSchemePtr.Length() ).Compare( credNoSchemePtr ) == 0 )
                {
                // The descriptors match. Things are looking good. In
                // the interests of paranoia, if we haven't matched
                // the entire URI, check that the character after the
                // match is '/'.
                if ( transNoSchemePtr.Length() == credNoSchemePtr.Length() )
                    {
                    TBuf<256> temp16;

                    temp16.Copy(aURI.UriDes());


                    AUTHLOGGER_WRITE_FORMAT( "Found cred(1): %d", cred );
                    AUTHLOGGER_WRITE_FORMAT8( "uri: %S", &aURI.UriDes() );
                    return cred;
                    }

                else if ( transNoSchemePtr[ credNoSchemePtr.Length() ] == '/' )
                    {
                    AUTHLOGGER_WRITE_FORMAT( "Found cred(2): %d", cred );
                    AUTHLOGGER_WRITE_FORMAT8( "uri: %S", &aURI.UriDes() );
                    return cred;
                    }
                }
            }
        }

    AUTHLOGGER_WRITE( "cred not found" );

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::FindHeaderPartToUseL
// Find the Authentication header.
// -----------------------------------------------------------------------------
//
TInt CHttpFilterDigestAuthentication::DFindHeaderPartToUseL( RHTTPTransaction aTransaction )
    {
    // While we're here, we check that all desired fields are there.
    RStringF wwwAuthenticate = iStringPool.StringF( iHttpStringAuthenticate, RHTTPSession::GetTable() );
    TInt lastGoodDigest = KErrNotFound;
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
            case HTTP::EDigest:
                {
                if ( headers.GetParam( wwwAuthenticate, iRealmStr, hdrVal, ii ) == KErrNone )
                    {
                    lastGoodDigest = ii;
                    }
                }
                break;

            default:
                // We don't understand this, whatever it is. Ignore it.
                break;
            }
        }

    return lastGoodDigest;
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::DGetCredentialsFromPropertiesL
// Find credentials from the transaction's properties.
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::DGetCredentialsFromPropertiesL( RHTTPTransaction& aTransaction )
    {
    THTTPHdrVal usernameVal;
    THTTPHdrVal passwordVal;
    THTTPHdrVal realmVal;
    THTTPHdrVal nonceVal;
    THTTPHdrVal staleVal;
    const TUriC8& uri = aTransaction.Request().URI();
    RHTTPTransactionPropertySet propSet = aTransaction.PropertySet();

    if ( propSet.Property( iUsernameStr, usernameVal ) &&
         propSet.Property( iPasswordStr, passwordVal ) &&
         propSet.Property( iRealmStr, realmVal ) && 
         propSet.Property( iNonceStr, nonceVal ) && 
         !propSet.Property( iStaleStr, staleVal ) )
        // if stale is TRUE we don't need to update credential
        {
        TInt cred;
        RStringF uriStr;

        if ( iIsProxy )
            {
            // do nothing this time
            }
        else
            {
            TInt pushCounter( 0 );
            if ( ( cred = DFindCredentialsForURI( uri ) ) != KErrNotFound )
                {
                // Remove old credentials from the list
                DRemoveCredentialsFromList( cred );
                }

            TPtrC8 uriPathPtr( uri.UriDes() );

            if ( uriPathPtr.LocateReverse( '/' ) > 0 )
                {
                uriPathPtr.Set( uriPathPtr.Left( uriPathPtr.LocateReverse( '/' ) ) );
                }

            uriStr = iStringPool.OpenFStringL( uriPathPtr );
            CleanupClosePushL<RStringF>( uriStr );
            ++pushCounter;

            THTTPHdrVal opaqueVal;
            RString opaqueStr;

            if( !propSet.Property( iOpaqueStr, opaqueVal ) )
                // this isn't a error case
                {
                opaqueStr = iStringPool.OpenStringL( KNullDesC8 );
                CleanupClosePushL<RString>( opaqueStr );
                ++pushCounter;
                }
            else
                {
                opaqueStr = opaqueVal;
                }

            THTTPHdrVal qopVal;

            if( !propSet.Property( iQopStr, qopVal ) )
                // this isn't a error case
                {
                qopVal.SetInt( KQopNone );
                }


            THTTPHdrVal algVal;
            if( !propSet.Property( iStringPool.StringF( HTTP::EAlgorithm, 
                                               RHTTPSession::GetTable() ), algVal) )
                {
                algVal.SetInt( KAlgUnknown );
                }

            // Add credentials to the list from the transaction's properties
            DAddCredentialsToListL( usernameVal.Str().Copy(), 
                                    passwordVal.Str().Copy(),
                                    realmVal.Str().Copy(), 
                                    uriStr.Copy(), 
                                    opaqueStr.Copy(), 
                                    nonceVal.Str().Copy(),
                                    qopVal.Int(),
                                    algVal.Int()
                                    );

            // this is not the traditional way to pop and destroy elements
            // but in this case this is the simplest one.
            CleanupStack::PopAndDestroy( pushCounter ); // [opaqueVal,]uriStr
            }
        }
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::DRemoveCredentialsFromList
// Remove credentials from the list.
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::DRemoveCredentialsFromList( TInt aCredId )
    {
    AUTHLOGGER_WRITE_FORMAT( "DRemoveCredentialsFromList: %d", aCredId );

    __ASSERT_DEBUG( aCredId >= 0 && aCredId < iDCredentials.Count(), PanicDigestAuthenticationFilter() );

    TDCredentials& creds = iDCredentials[ aCredId ];
    iStringPool.String( creds.iUser ).Close();
    iStringPool.String( creds.iPassword ).Close();
    iStringPool.StringF( creds.iURI ).Close();
    iStringPool.String( creds.iRealm ).Close();
    iStringPool.String( creds.iOpaque ).Close();
    iStringPool.String( creds.iNonce ).Close();
    iDCredentials.Remove( aCredId );
    }


// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::DAddCredentialsToListL
// Store credentials in the list.
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::DAddCredentialsToListL( RString aUsernameStr,
        RString aPasswordStr,
        RString aRealmStr,
        RStringF aUriStr,
        RString aOpaque,
        RString aNonce,
        TInt aQop,
        TInt aAuthAlg )
    {
    TDCredentials newCred;
    newCred.iUser = aUsernameStr;
    newCred.iPassword = aPasswordStr;
    newCred.iRealm = aRealmStr;
    newCred.iURI = aUriStr;
    newCred.iOpaque = aOpaque;
    newCred.iNonce = aNonce;
    newCred.iQop = aQop;
    newCred.iAlgorithm = aAuthAlg;
    newCred.iNc = 0;
    User::LeaveIfError( iDCredentials.Append( newCred ) );
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::EncodeDAuthL
// Encodes digest authentication
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::EncodeDigestAuthL( TInt aCred, RHTTPTransaction aTransaction )
    {
    AUTHLOGGER_ENTERFN( "EncodeDigestAuthL" );

	// For now, only support RFC2069 format. The digest is
	// MD5(H(A1):nonce:H(A2))
	TBuf8<KHashLength> hash;
    TBuf8<KNonceCountLength> nonceCount;

    TDCredentials& cred = iDCredentials[aCred];

    THTTPHdrVal nonce( iStringPool.String( cred.iNonce ) );
    THTTPHdrVal realm( iStringPool.String( cred.iRealm ) );
    THTTPHdrVal qop;
    THTTPHdrVal uname( iStringPool.String( cred.iUser ) );
    THTTPHdrVal password( iStringPool.String( cred.iPassword ) );
    THTTPHdrVal opaque( iStringPool.String( cred.iOpaque ) );

    if( cred.iQop & KQopAuth )
        // recent release supports only qop 'auth'.
        {
        qop.SetStrF( iQopAuthStr );
        }

	THTTPHdrVal requestUri;
    // we get url from request and modify as made in chttpclienthandler.cpp
    RString uriStr = RequestUriL( aTransaction );
    CleanupClosePushL<RString>( uriStr );
    
    requestUri.SetStr( uriStr );

	HBufC8* stringToHash = NULL;
	RStringF auth = iStringPool.StringF(HTTP::EAuth,RHTTPSession::GetTable());

	TBuf8<KHashLength> cnonce;
	if ( cred.iQop != KQopNone )
		{
        GenerateCNonce( cnonce );

        // 7 is for colons
        // 8 for nc-value
        stringToHash = HBufC8::NewMaxLC( KHashLength +  // H(A1)
                                         1 +    // ":"
                                         nonce.Str().DesC().Length() + 
                                         1 +    // ":"
                                         8 +    // nc value
                                         1 +    // ":"
                                         KHashLength +  // cnonce
                                         1 +    // ":"
										 qop.StrF().DesC().Length() +
                                         1 +    // ":"
										 KHashLength ); // H(A2)
		TPtr8 stringMod = stringToHash->Des();
		stringMod.Zero();
		HA1L( cred.iAlgorithm, uname, password, realm.Str(), nonce, cnonce, stringMod );
		stringMod.Append(':');
		stringMod.Append(nonce.Str().DesC());

        // nc value
		stringMod.Append(':');
        ++cred.iNc;
        // nc value is of 8 chars length and in hexadecimal format
        nonceCount.NumFixedWidth( cred.iNc, EHex, 8 );
		stringMod.Append( nonceCount );
		stringMod.Append(':');

		stringMod.Append(cnonce);
		stringMod.Append(':');
		stringMod.Append(auth.DesC());
		stringMod.Append(':');
		HA2L(aTransaction.Request().Method(), requestUri.Str(), hash);
		stringMod.Append(hash);
		}
	else
		{
		// The response is the hash of H(A1):nonce:H(A2).
		// Crqeate a buffer for the string to convert into MD5. The
		// length is 32 characters for each of the hashes, 2
		// characters for the 2 colons, plus the length of the nonce
		stringToHash = HBufC8::NewMaxLC(nonce.Str().DesC().Length() + 
												 2 * KHashLength + 2);
		TPtr8 stringMod = stringToHash->Des();
		stringMod.Zero();
		HA1L( cred.iAlgorithm, uname, password, realm.Str(), nonce, cnonce, stringMod );
		stringMod.Append(':');
		stringMod.Append(nonce.Str().DesC());
		stringMod.Append(':');
		HA2L(aTransaction.Request().Method(), requestUri.Str(), hash);
		stringMod.Append(hash);
		}
	
	Hash(*stringToHash, hash);
	CleanupStack::PopAndDestroy(stringToHash);

	// OK. hash now contains the digest response. Set up the header.
	RHTTPHeaders requestHeaders(aTransaction.Request().GetHeaderCollection());
	
	requestHeaders.RemoveField(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable())); 

	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::EUsername,RHTTPSession::GetTable()),
							 uname);
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::ERealm,RHTTPSession::GetTable()),
							 realm);
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::ENonce,RHTTPSession::GetTable()),
							 nonce);
	RString hashStr = iStringPool.OpenStringL(hash);
	CleanupClosePushL<RString>(hashStr);

	THTTPHdrVal hdrVal;
    hdrVal.SetStr(hashStr);
	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::EResponse,RHTTPSession::GetTable()),
							 hdrVal);

	requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
							 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
							 iStringPool.StringF(HTTP::EUri,RHTTPSession::GetTable()),
							 requestUri);
	CleanupStack::PopAndDestroy(&hashStr);
	if ( cred.iQop != KQopNone )
		{
        // QOP
		hdrVal.SetStrF( auth );
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::EQop,RHTTPSession::GetTable()),
								 hdrVal);
		RString cnonceString = iStringPool.OpenStringL(cnonce);
        // CNonce
		CleanupClosePushL<RString>( cnonceString );
		hdrVal.SetStr(cnonceString);
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::ECnonce,RHTTPSession::GetTable()),
								 hdrVal);
		CleanupStack::PopAndDestroy(); //cnonceString
        // Nonce-count
        RString nonceStr = iStringPool.OpenStringL( nonceCount );
        CleanupClosePushL<RString>( nonceStr );
		hdrVal.SetStr( nonceStr );
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::ENc,RHTTPSession::GetTable()),
								 hdrVal);
        CleanupStack::PopAndDestroy(); // nonceStr
		}
	
	if(  opaque.Str().DesC().Length() )
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::EOpaque,RHTTPSession::GetTable()),
								 opaque );	

    if( cred.iAlgorithm == KAlgMd5 )
        {
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::EAlgorithm,RHTTPSession::GetTable()),
								 iMd5Str );
        }
    else if( cred.iAlgorithm == KAlgMd5Sess )
        {
		requestHeaders.SetFieldL(iStringPool.StringF(HTTP::EAuthorization,RHTTPSession::GetTable()), 
								 THTTPHdrVal(iStringPool.StringF(HTTP::EDigest,RHTTPSession::GetTable())),
								 iStringPool.StringF(HTTP::EAlgorithm,RHTTPSession::GetTable()),
								 iMd5SessStr );
        }

    // set it again
    // this may be needed if auth. fails and stale is true.
    aTransaction.PropertySet().SetPropertyL( iRealmStr, realm );
    
    CleanupStack::PopAndDestroy();  // requestUri

    AUTHLOGGER_LEAVEFN( "EncodeDigestAuthL" );
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::HA1L
// Calculates H(A1)
// -----------------------------------------------------------------------------
//
TInt CHttpFilterDigestAuthentication::HA1L(int aAlgorithm,
                                           const RString& aUsername,
                                           const RString& aPW,
								           const RString& aRealm, 
                                           const RString& aNonceValue,
                                           TDesC8& aCNonce,
                                           TDes8& aResult )
	{
    TBuf8<KHashLength> hash;

	HBufC8* a1 = HBufC8::NewMaxL(aUsername.DesC().Length() + 
								 aPW.DesC().Length() + 
								 aRealm.DesC().Length() + 2 );
	TPtr8 a1Mod = a1->Des();
	a1Mod.Zero();
	a1Mod.Append( aUsername.DesC() );
	a1Mod.Append( KColon );
	a1Mod.Append( aRealm.DesC() );
	a1Mod.Append( KColon );
	a1Mod.Append( aPW.DesC() );

	Hash( *a1, hash );

    delete a1;
    a1 = NULL;

    if( aAlgorithm == KAlgMd5Sess )
        {
	    a1 = HBufC8::NewMaxL( KHashLength +
                               1 +  // ":"
                               aNonceValue.DesC().Length() +
                               1 +  // ":"
							   aCNonce.Length() );
	    TPtr8 a1Mod = a1->Des();
	    a1Mod.Zero();
        a1Mod.Append( hash );
        a1Mod.Append( KColon );
        a1Mod.Append( aNonceValue.DesC() );
        a1Mod.Append( KColon );
        a1Mod.Append( aCNonce );

	    Hash(*a1, aResult);

	    delete a1;
        a1 = NULL;
        }
    else
        {
        aResult.Copy( hash );
        }

	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::HA2L
// Calculates H(A2)
// -----------------------------------------------------------------------------
//
TInt CHttpFilterDigestAuthentication::HA2L(const RStringF& aMethod, 
								           const RString& aRequestUri,
								           TDes8& aResult)
	{
	// In the auth qop, a2 is Method:digest-uri-value
	// Digest-uri-value. We don't support auth-int qop
	// Allocate enough space for the method, the URI and the colon.
	TPtrC8 requestUri = aRequestUri.DesC();
	TPtrC8 method = aMethod.DesC();
	HBufC8* a2 = HBufC8::NewMaxLC(requestUri.Length() + method.Length() + 1);
	TPtr8 a2Mod = a2->Des();
	a2Mod.Zero();
	a2Mod.Append(method);
	a2Mod.Append(':');
	a2Mod.Append(requestUri);

	Hash(*a2, aResult);

	CleanupStack::PopAndDestroy(a2);

	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::Hash
// Calculates hash value
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::Hash(const TDesC8& aMessage, TDes8& aHash)
	{
	// Calculate the 128 bit (16 byte) hash
	iMD5Calculator->Reset();
	TPtrC8 hash = iMD5Calculator->Hash( aMessage );
	// Now print it as a 32 byte hex number
	aHash.Zero();
	_LIT8(formatStr, "%02x");
	for (TInt ii = 0; ii < KRawHashLength; ii++)
		{
		TBuf8<2> scratch;
		scratch.Zero();
		scratch.Format( formatStr, hash[ii] );
		aHash.Append( scratch );
		}
	}

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::GenerateCNonce
// Generate unique cnonce value.
// -----------------------------------------------------------------------------
//
void CHttpFilterDigestAuthentication::GenerateCNonce(TDes8& aNonce)
	{
	// 'Inspired by' CObexAuthenticator

	// The purpose of the client nonce is to protect against 'chosen
	// plaintext' attacks where a hostile server tricks the client
	// into supplying a password using a specific server nonce that
	// allows an (as yet undiscovered) flaw in MD5 to recover the
	// password. As such the only important thing about client nonces
	// is that they change and aren't predictable. See section 4.9 of
	// RFC2616

	TTime time;
	time.UniversalTime();
	TInt64 randomNumber = Math::Rand(iSeed);
	randomNumber <<= 32;
	randomNumber += Math::Rand(iSeed);
	TBuf8<33> key;
	key.Zero();
	key.AppendNum(time.Int64(), EHex);
	key.Append(_L8(":"));
	key.AppendNum(randomNumber, EHex);
	
	Hash(key, aNonce);
	}

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::RequestUriLC
// Returns requested URI in form that can be used in uri field.
// This code comes from 
// Application-protocols/http/httpclient/chttpclienthandler.cpp.
// -----------------------------------------------------------------------------
//
RString CHttpFilterDigestAuthentication::RequestUriL( RHTTPTransaction& aTransaction )
    {
    TBool useProxy( EFalse );
    THTTPHdrVal useProxyHdr;
    RStringF proxyUsage = iStringPool.StringF( HTTP::EProxyUsage, RHTTPSession::GetTable() );
    RHTTPRequest request = aTransaction.Request();
    const TUriC8& uri = request.URI();
    RStringF scheme = iStringPool.OpenFStringL( uri.Extract( EUriScheme ) );
    CleanupClosePushL<RStringF>( scheme );
    CUri8* uriToUse = CUri8::NewLC( uri );

    if( iSession->ConnectionInfo().Property( proxyUsage, useProxyHdr ) )
        {
        __ASSERT_DEBUG( useProxyHdr.Type() == THTTPHdrVal::KStrFVal, 
                        PanicDigestAuthenticationFilter(-1/*THttpClientPanic::EInvalidProxySetting*/) );

	    // Is a proxy being used?
        useProxy = ( useProxyHdr.StrF().Index(RHTTPSession::GetTable()) == HTTP::EUseProxy );
        }

	if( !useProxy || (useProxy && scheme.Index(RHTTPSession::GetTable()) == HTTP::EHTTPS ))
		{
		// Not going via a proxy - need to remove the scheme and authority parts
		uriToUse->RemoveComponentL( EUriScheme );
		uriToUse->RemoveComponentL( EUriHost );	// this also removes the userinfo + port
		}

    RString uriStr = iStringPool.OpenStringL( uriToUse->Uri().UriDes()  );

    CleanupStack::PopAndDestroy( 2 );  // schem, uriToUse

    return uriStr;    
    }

// -----------------------------------------------------------------------------
// CHttpFilterDigestAuthentication::CheckQop
//
// -----------------------------------------------------------------------------
//
TInt CHttpFilterDigestAuthentication::CheckQop( RHTTPHeaders& aHeaders,
                                                RStringF& aWwwAuthHeader,
                                                TInt aHeaderPart )
    {
    THTTPHdrVal qopVal;
    TInt retVal( KQopNone );
    
    if( !aHeaders.GetParam( aWwwAuthHeader, iQopStr, qopVal, aHeaderPart ) )
        {
        TPtrC8 qopBuf( qopVal.Str().DesC() );
        TPtrC8 qopValue;
        TInt comma;

        do
            {
            comma = qopBuf.Locate( ',' );
            
            if( comma != KErrNotFound )
                {
                qopValue.Set( qopBuf.Left(comma) );
                }
            else
                {
                qopValue.Set( qopBuf );
                }

            if( !qopValue.CompareF(iQopAuthStr.DesC()) )
                {
                retVal |= KQopAuth;
                }
            else if( !qopValue.CompareF(KQopAuthIntStr) )
                {
                retVal |= KQopAuthInt;
                }
            
            qopBuf.Set( qopBuf.Right(qopBuf.Length()-comma-1));
            }while( comma != KErrNotFound );
        }
        
    return retVal;
    }
