/*
* Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/


// INCLUDE FILES
	// System includes
// #include <cookie.h>
// #include <cookiemanagerclient.h>
#include <e32std.h>
#include <uri8.h>

	// User includes
#include "CookieFilter.h"
#include "CookieLogger.h"
#include "CookieFilterPanic.h"

// CONSTANTS
const TInt KCookieFilterStackPosition = MHTTPFilter::EStatusCodeHandler - 10;
_LIT8(KVersionValue, "$Version=1");
_LIT8(KEmpty, "");
_LIT8( KCookieUsage, "CookiesEnabled" );
_LIT8( KAppUid, "Appuid" );
_LIT8( KHttpConnectMethod, "CONNECT");
_LIT8( KSecureHttpScheme, "https://");
const TInt KSecureHttpSchemeLength = 8;

// ---------------------------------------------------------
// CCookieFilter::InstallFilterL
// ---------------------------------------------------------
//
CHttpCookieFilter* CCookieFilter::InstallFilterL( TAny* aFilterParams )
	{
    CLOG( ( EFilter, 0,
				_L( "-> CCookieFilter::InstallFilterL" ) ) );

    RHTTPSession* session = REINTERPRET_CAST( RHTTPSession*, aFilterParams );

	CCookieFilter* filter = new (ELeave) CCookieFilter( *session );

	CleanupStack::PushL( filter );

	filter->ConstructL();

	CleanupStack::Pop();	// filter

    CLOG( ( EFilter, 0,
				_L( "<- CCookieFilter::InstallFilterL" ) ) );

	return filter;
	}


// ---------------------------------------------------------
// CCookieFilter::CCookieFilter
// ---------------------------------------------------------
//
CCookieFilter::CCookieFilter( RHTTPSession aSession )
: iStringPool( aSession.StringPool() ), iSession( aSession ),
iCookieManager( iStringPool ), iStringTable( RHTTPSession::GetTable() )
	{
    CLOG( ( EFilter, 0, _L( "" ) ) );
    CLOG( ( EFilter, 0, _L( "*****************" ) ) );
    CLOG( ( EFilter, 0,
				_L( "CCookieFilter::CCookieFilter" ) ) );
	}


// ---------------------------------------------------------
// CCookieFilter::ConstructL
// ---------------------------------------------------------
//
void CCookieFilter::ConstructL()
	{
    CLOG( ( EFilter, 0,
				_L( "-> CCookieFilter::ConstructL" ) ) );

	User::LeaveIfError( iCookieManager.Connect() );

	iSession.FilterCollection().AddFilterL
					( *this,
					THTTPEvent::EGotResponseHeaders,
					RStringF(),
					KAnyStatusCode,
					KCookieFilterStackPosition,
					iStringPool.StringF( HTTP::ECookieFilter, iStringTable) );

	iSession.FilterCollection().AddFilterL
					( *this,
					THTTPEvent::ESubmit,
					RStringF(),
					KAnyStatusCode,
					KCookieFilterStackPosition,
					iStringPool.StringF( HTTP::ECookieFilter, iStringTable) );

    iCookieUsage = iStringPool.OpenFStringL( KCookieUsage );

    CLOG( ( EFilter, 0,
				_L( "<- CCookieFilter::ConstructL" ) ) );
	}



// ---------------------------------------------------------
// CCookieFilter::~CCookieFilter
// ---------------------------------------------------------
//
CCookieFilter::~CCookieFilter()
	{
    iCookieUsage.Close();

	// If we've been destroyed from the cleanup stack during creation
	// of the object, it might still be loaded. So check. (Normaly the
	// delete is initiated by the 'delete this' in MHFUnload)
	if ( iLoadCount > 0 )
		{
		// As we're already in a destructor, MHFUnload must not delete us again
		iLoadCount = -1;
		iSession.FilterCollection().RemoveFilter
				( iStringPool.StringF( HTTP::ECookieFilter, iStringTable ) );
		}

	iCookieManager.Close();

    CLOG( ( EFilter, 0,
				_L( "~CCookieFilter::CCookieFilter" ) ) );
    CLOG( ( EFilter, 0, _L( "*****************" ) ) );
	}

// ---------------------------------------------------------
// CCookieFilter::MHFLoad
// ---------------------------------------------------------
//
void CCookieFilter::MHFLoad(RHTTPSession /*aSession*/,
							THTTPFilterHandle /*aHandle*/)
	{
    CLOG( ( EFilter, 0,
				_L( "-> CCookieFilter::MHFLoad" ) ) );

	++iLoadCount;

    CLOG( ( EFilter, 0,
				_L( "<- CCookieFilter::MHFLoad" ) ) );
	}

// ---------------------------------------------------------
// CCookieFilter::MHFUnload
// ---------------------------------------------------------
//
void CCookieFilter::MHFUnload( RHTTPSession /*aSession*/,
							  THTTPFilterHandle /*aHandle*/ )
	{
    CLOG( ( EFilter, 0,
				_L( "<-> CCookieFilter::MHFUnload" ) ) );

    if ( --iLoadCount )
		{
		return;
		}

	delete this;
	}

// ---------------------------------------------------------
// CCookieFilter::MHFRunL
// ---------------------------------------------------------
//
void CCookieFilter::MHFRunL( RHTTPTransaction aTransaction, 
							const THTTPEvent& aEvent )
	{
    CLOG( ( EFilter, 0,
				_L( "-> CCookieFilter::MHFRunL" ) ) );

    switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders :
			{
			HandleResponseHeadersL( aTransaction );

			break;
			}
		case THTTPEvent::ESubmit :
			{
			HandleRequestHeadersL( aTransaction );

			break;
			}
		default :
			{
			TCookieFilterPanic::Panic
								( TCookieFilterPanic::EInvalidFilterState );
			}
		}

    CLOG( ( EFilter, 0,
				_L( "<- CCookieFilter::MHFRunL" ) ) );
	}

// ---------------------------------------------------------
// CCookieFilter::MHFRunError
// ---------------------------------------------------------
//
TInt CCookieFilter::MHFRunError( TInt /*aError*/, 
								RHTTPTransaction /*aTransaction*/,
								const THTTPEvent& /*aEvent*/ )
	{
    CLOG( ( EFilter, 0,
				_L( "<-> CCookieFilter::MHFRunError" ) ) );
	// If anything left, we've run out of memory or something
	// similarly catastrophic has gone wrong.

	// The transaction should never fail just because there was something wrong
	// with cookie management
//	aTransaction.Fail();

	return KErrNone;
	}

// ---------------------------------------------------------
// CCookieFilter::HandleRequestHeadersL
// ---------------------------------------------------------
//
void CCookieFilter::HandleRequestHeadersL( RHTTPTransaction aTransaction )
	{
    CLOG( ( EFilter, 0,
				_L( "-> CCookieFilter::HandleRequestHeadersL" ) ) );

    if( !IsCookieUsageEnabledL( aTransaction ) )
        {
        return;
        }

    RHTTPHeaders reqHeaders = aTransaction.Request().GetHeaderCollection();
	reqHeaders.RemoveField( iStringPool.StringF( HTTP::ECookie,
												iStringTable ) );
	reqHeaders.RemoveField( iStringPool.StringF( HTTP::ECookie2,
												iStringTable ) );

	TBool cookie2Reqd = EFalse;
	RPointerArray<CCookie> cookies(20);
    HBufC8* requestUriBuf( NULL );
	TPtrC8 requestUri( aTransaction.Request().URI().UriDes() );
    TPtrC8 requestMethod( aTransaction.Request().Method().DesC() );
    if( (requestMethod.CompareF(KHttpConnectMethod) == 0) && (requestUri.Left(KSecureHttpSchemeLength).CompareF(KSecureHttpScheme) == 0))
        {
        // if we are performing an HTTP CONNECT to create tunnel for original https:// request, we
        // should not include secure cookies in this HTTP request since they will be in clear text.
        // to ensure that Cookie manager does not add these into cookie array, change scheme so
        // it appears as non-secure transaction
        requestUriBuf = requestUri.Alloc();
        CleanupStack::PushL(requestUriBuf);        
        TPtr8 requestUriPtr = requestUriBuf->Des();
        requestUriPtr.Delete(4, 1);  // remove char in pos 4 to change https:// to http:// 
        requestUri.Set(requestUriPtr);
        }
	TBool ret;
	RStringF appuid = iStringPool.OpenFStringL( KAppUid );
	THTTPHdrVal hdrVal;
	TUint32 appuidValue (0);
	
	ret = aTransaction.PropertySet().Property(appuid,hdrVal);
	if(ret)
	    {
    	appuidValue = hdrVal.Int();    	       
	    }
	//iCookieManager.SetAppUidL(appuidValue); 
    appuid.Close();
    
    iCookieManager.GetCookiesL( requestUri, cookies, cookie2Reqd,appuidValue );
    if( requestUriBuf )
        {
        CleanupStack::PopAndDestroy( requestUriBuf );
        }

	TInt numCookies = cookies.Count();
	for ( TInt ii = 0; ii < numCookies; ++ii )
        {
		cookies[ii]->AddToRequestL( reqHeaders, ii );
        }

	cookies.ResetAndDestroy();	// delete cookies
	cookies.Close();

	if ( cookie2Reqd )
		{
		// Add a Cookie2 header.  Note: SetRawFieldL is being used instead of
        // SetFieldL because RHTTPHeaders doesn't know how to deal with
        // the Cookie2 header field.
		reqHeaders.SetRawFieldL( iStringPool.StringF( 
                                 HTTP::ECookie2,
							     iStringTable ), 
                                 KVersionValue(),
                                 KEmpty() );
		}

    CLOG( ( EFilter, 0,
				_L( "<- CCookieFilter::HandleRequestHeadersL" ) ) );
	}



// ---------------------------------------------------------
// CCookieFilter::HandleResponseHeadersL
// ---------------------------------------------------------
//
void CCookieFilter::HandleResponseHeadersL( RHTTPTransaction aTransaction )
	{
    CLOG( ( EFilter, 0,
				_L( "-> CCookieFilter::HandleResponseHeadersL" ) ) );

    if( !IsCookieUsageEnabledL( aTransaction ) )
        {
        return;
        }

    RHTTPHeaders responseHeaders = 
							aTransaction.Response().GetHeaderCollection();

    LogHeaders( responseHeaders );

	const TUriC8& requestUri = aTransaction.Request().URI();

	RStringF cookieHeaderFieldName = iStringPool.StringF( HTTP::ESetCookie2, 
                                                          iStringTable );
	TInt numCookies = responseHeaders.FieldPartsL( cookieHeaderFieldName );
	if ( !numCookies ) 	// if there are no Set-Cookie2 (RFC2965) cookies
		{
		// then check if there are Set-Cookie (Netscape) cookies
		cookieHeaderFieldName = iStringPool.StringF( HTTP::ESetCookie, 
                                                     iStringTable );
		numCookies = responseHeaders.FieldPartsL( cookieHeaderFieldName );
		}

	for ( TInt ii = 0; ii < numCookies; ++ii )
        {
		CCookie* cookie = CCookie::NewL( responseHeaders,
										ii,
										cookieHeaderFieldName,
										iStringPool,
										requestUri );
		CleanupStack::PushL( cookie );
		
		TBool ret;    	
    	RStringF appuid = iStringPool.OpenFStringL(KAppUid);
    	THTTPHdrVal hdrVal;    	
    	TUint32 appuidValue (0);
    	
    	ret = aTransaction.PropertySet().Property(appuid,hdrVal);
    	if(ret)
    	    {
    	    appuidValue = hdrVal.Int();                                     
    	    }	
        //iCookieManager.SetAppUidL(appuidValue);    
        appuid.Close();       
        
		User::LeaveIfError( iCookieManager.StoreCookie( *cookie,
														requestUri,appuidValue ) );

		CleanupStack::PopAndDestroy();	// cookie
        }

    CLOG( ( EFilter, 0,
				_L( "<- CCookieFilter::HandleResponseHeadersL" ) ) );
	}



// ---------------------------------------------------------
// CCookieFilter::LogHeaders()
// ---------------------------------------------------------
//
void CCookieFilter::LogHeaders( RHTTPHeaders LOG_ONLY( aHeaders ) )
    {
#ifdef __TEST_COOKIE_LOG__
    _LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");

    CLOG(( EFilter, 1, _L("Headers:") ));

    TInt i;
    TInt fieldParts;
    TInt ignoreErr;
    RStringPool strP = iStringPool;
    THTTPHdrFieldIter it = aHeaders.Fields();

    while ( !it.AtEnd() )
        {
        RStringTokenF fieldName = it();
        RStringF fieldNameStr = strP.StringF (fieldName );
        THTTPHdrVal fieldVal;
        fieldParts = 0; // For the case if next the call fails.
        TRAP( ignoreErr, fieldParts =  aHeaders.FieldPartsL( fieldNameStr ) );
        for ( i = 0; i < fieldParts; i++ )
            {
            if ( aHeaders.GetField( fieldNameStr, i, fieldVal ) == KErrNone )
                {
                const TDesC8& fieldNameDesC = fieldNameStr.DesC();
                switch ( fieldVal.Type() )
                    {
                    case THTTPHdrVal::KTIntVal:
                        {
                        CLOG(( EFilter, 1, _L8("  <%S> (%d)"), \
                            &fieldNameDesC, fieldVal.Int() ));
                        break;
                        }

                    case THTTPHdrVal::KStrFVal:
                        {
                        RStringF fieldValStr = strP.StringF( fieldVal.StrF() );
                        const TDesC8& fieldValDesC = fieldValStr.DesC();
                        CLOG(( EFilter, 1, _L8("  <%S> <%S>"), \
                            &fieldNameDesC, &fieldValDesC ));
                        }
                        break;

                    case THTTPHdrVal::KStrVal:
                        {
                        RString fieldValStr = strP.String( fieldVal.Str() );
                        const TDesC8& fieldValDesC = fieldValStr.DesC();
                        CLOG(( EFilter, 1, _L8("  <%S> <%S>"), \
                            &fieldNameDesC, &fieldValDesC ));
                        }
                        break;

                    case THTTPHdrVal::KDateVal:
                        {
                        TDateTime date = fieldVal.DateTime();
                        TBuf<40> dateTimeString;
                        TTime t( date );
                        TRAP( ignoreErr, \
                            t.FormatL( dateTimeString, KDateFormat ) );
                        TBuf8<40> dateTimeString8;
                        dateTimeString8.Copy( dateTimeString );
                        CLOG(( EFilter, 1, _L8("  <%S> <%S>"), \
                            &fieldNameDesC, &dateTimeString8 ));
                        } 
                        break;

                    default:
                        CLOG(( EFilter, 1, \
                            _L8("  <%S> unrecognised value type(%d)"), \
                            &fieldNameDesC, fieldVal.Type() ));
                        break;
                    }

                // Display realm for WWW-Authenticate header.
                RStringF wwwAuth = strP.StringF
                    ( HTTP::EWWWAuthenticate, RHTTPSession::GetTable() );
                if ( fieldNameStr == wwwAuth )
                    {
                    // check the auth scheme is 'basic'
                    RStringF basic = strP.StringF
                        ( HTTP::EBasic, RHTTPSession::GetTable() );
                    RStringF realm = strP.StringF
                        ( HTTP::ERealm, RHTTPSession::GetTable() );
                    THTTPHdrVal realmVal;
                    if ( ( fieldVal.StrF() == basic ) && 
                        ( !aHeaders.GetParam( wwwAuth, realm, realmVal ) ) )
                        {
                        RStringF realmValStr = strP.StringF( realmVal.StrF() );
                        CLOG(( EFilter, 1, _L8("    Realm<%S>"), \
                            &realmValStr ));
                        }
                    }
                }
            }
        ++it;
        }
#endif /* def __TEST_COOKIE_LOG__ */
    }

// ---------------------------------------------------------
// CCookieFilter::IsCookieUsageEnabledL()
// ---------------------------------------------------------
//
TBool CCookieFilter::IsCookieUsageEnabledL( RHTTPTransaction aTransaction )
    {
    THTTPHdrVal cookieVal;
    TBool enabled( ETrue );

    if( aTransaction.PropertySet().Property( iCookieUsage, cookieVal ) )
        {
        enabled = cookieVal.Int();
        }

    return enabled;
    }

// End of file
