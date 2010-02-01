/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the client interface of the cookie server.
*
*/


// INCLUDE FILES
	// System includes
#include <e32std.h>
// #include <thttphdrval.h>

	// User includes
#include "cookie.h"
#include "cookieIPC.h"
#include "CookieCommonConstants.h"
#include "cookielogger.h"
#include "cookiemanagerclient.h"
#include "CookieManagerServer.h"
#include "CookieManagerStart.h"
#include "CookieServerDef.h"

// CONSTANTS

// TBD : do we have to set limits to the number of cookies at all?
// Possible answer : as we store cookies in an RPointerArray that has a
// restriction on the number of elements (max. 640 can be stored in an RArray
// or RPointerArray), we have to bother with it.
// TODO :
//	- change this value
//	- use always when appropriate
//	- share in a common header file
// const TInt KMaxNumberOfCookies = 256;

// ---------------------------------------------------------
// RCookieManager::RCookieManager
// ---------------------------------------------------------
//
EXPORT_C RCookieManager::RCookieManager( RStringPool aStringPool )
: iStringPool( aStringPool ), iCookiePacker( iStringPool )
    {
    CLOG(( EClient, 0, _L(" ") ));
    CLOG(( EClient, 0, _L("*****************") ));
    CLOG(( EClient, 0, _L("RCookieManager::RCookieManager") ));
    }

// ---------------------------------------------------------
// RCookieManager::ClearCookies
// ---------------------------------------------------------
//
EXPORT_C TInt RCookieManager::ClearCookies( TInt& aDeletedCookies )
    {
    CLOG(( EClient, 0, _L("-> RCookieManager::ClearCookies") ));

    TPckg<TInt> pkgCount( aDeletedCookies );
    TInt ret( SendReceive( EClearAllCookies, TIpcArgs( &pkgCount ) ) );

    CLOG(( EClient, 0, 
		_L("<- RCookieManager::ClearCookies cleared %d cookies, errcode%d"),
		aDeletedCookies, ret ) );

    return ret;
    }

// ---------------------------------------------------------
// RCookieManager::Connect
// ---------------------------------------------------------
//
EXPORT_C TInt RCookieManager::Connect()
    {
    CLOG( ( EClientConnect, 0, _L( "-> RCookieManager::Connect" ) ) );

    TInt error = KErrNone;
    RProcess server;
    error = server.Create( KCookieServerExe, TPtr( NULL, 0 ),
                           TUidType( KNullUid, KNullUid, KCookieServerExeUid ) );
    CLOG( ( EClientConnect, 0,
          _L( "RCookieManager::Connect after process creation, err %d" ),
          error ) );

    if( !error )
        {
        TRequestStatus status;
        server.Rendezvous( status );
        if( status != KRequestPending )
            {
            CLOG( (EClientConnect, 0, _L("RCookieManager::Connect pending status error.") ) );
            server.Kill( 0 );
            }
        else
            {
            CLOG( (EClientConnect, 0, _L("RCookieManager::Connect pending resume server.") ) );
            server.Resume( );
            }
        User::WaitForRequest( status );
        error = ( server.ExitType() == EExitPanic ) ? KErrGeneral : status.Int();
        server.Close();
        }
    CLOG( ( EClientConnect, 0, _L( "Creating server session" ) ) );
    error = CreateSession( KCookieServerName, Version() );
    CLOG( ( EClientConnect, 0, _L( "Server session created, errcode%d" ), error ) );

    CLOG( ( EClientConnect, 0, _L( "<- RCookieManager::Connect" ) ) );
    return error;
    }

// ---------------------------------------------------------
// RCookieManager::GetCookies
// ---------------------------------------------------------
//
TInt RCookieManager::DoGetCookies( TDes8& aBuffer ) const
	{
	return SendReceive( EGetCookies, TIpcArgs( &aBuffer ) );
	}

// ---------------------------------------------------------
// RCookieManager::GetCookieSize
// ---------------------------------------------------------
//
TInt RCookieManager::DoGetCookieSize( const TDesC8& aRequestUri,
									  TPckg<TInt>& aPkgSize ) const
	{
    return SendReceive( EGetCookieSize, TIpcArgs( aRequestUri.Length(), 
                                                  &aRequestUri, &aPkgSize ) );
	}

// ---------------------------------------------------------
// RCookieManager::DoStoreCookie
// ---------------------------------------------------------
//
TInt RCookieManager::DoStoreCookie( const TDesC8& aPackedCookie,
								   const TDesC8& aUri ) const
	{
    return SendReceive( EStoreCookie, TIpcArgs( aPackedCookie.Length(), 
                                      &aPackedCookie, aUri.Length(), &aUri ) );
	}

// ---------------------------------------------------------
// RCookieManager::GetCookiesL
// ---------------------------------------------------------
//
EXPORT_C void RCookieManager::GetCookiesL( const TDesC8& aUri,
										RPointerArray<CCookie>& aCookies,
										TBool& aCookie2Reqd )
    {
    CLOG( ( EClient, 0, _L( "-> RCookieManager::GetCookiesL" ) ) );

	aCookie2Reqd = EFalse;

	TInt size = 0;
	TPckg<TInt> pkgSize( size );
	User::LeaveIfError( DoGetCookieSize( aUri, pkgSize ) );

	if ( size )
		{
		HBufC8* buf = HBufC8::NewLC( size );

		TPtr8 des( buf->Des() );
		User::LeaveIfError( DoGetCookies( des ) );

		// it seems this is the only place where we cannot avoid leaving
		// ==> we allocate memory for cookies when we fill up the cookie array.
		iCookiePacker.UnpackCookiesFromBufferL( *buf, aCookies );

		TInt count = aCookies.Count();
		TInt i = 0;
		TBool anyCookie2( EFalse );
		TBool anyUnknownVersion( EFalse );
		for ( ; i < count; i++ )
			{
			if ( aCookies[i]->FromCookie2() )
				{
				anyCookie2 = ETrue;
				anyUnknownVersion |= aCookies[i]->IsUnknownVersion();
				}
			}

		// if there were no new-style cookies or a new version info is detected
		// then we have to send an extra cookie header indicating that we're 
		// able to process new-style cookies
		if ( !anyCookie2 || anyUnknownVersion )
			{
			aCookie2Reqd = ETrue;
			}

		CleanupStack::PopAndDestroy();	// buf
		}

    CLOG( ( EClient, 0, _L( "<- RCookieManager::GetCookiesL" ) ) );
    }

// ---------------------------------------------------------
// RCookieManager::StoreCookie
// ---------------------------------------------------------
//
EXPORT_C TInt RCookieManager::StoreCookie( const CCookie& aCookie,
								 const TUriC8& aUri )
    {
    CLOG( ( EClient, 0, _L( "-> RCookieManager::StoreCookie" ) ) );

	TInt err;

	TInt cookieSize = aCookie.Size( EFalse );
	HBufC8* buf = HBufC8::New( cookieSize );
	if ( buf )
		{
		CLOG( ( EClient, 0,
			    _L( "RCookieManager::StoreCookie, cookie size:%d" ), 
                cookieSize ) );

		TPtr8 bufDes( buf->Des() );
		err = iCookiePacker.CliPackCookie( bufDes, aCookie );
		if ( !err )
			{
			err = DoStoreCookie( *buf, aUri.UriDes() );
			}

		delete buf;
		}
	else
		{
		err = KErrNoMemory;
		}

    CLOG( ( EClient, 0,
			_L( "<- RCookieManager::StoreCookie errcode%d" ), err ) );

	return err;
    }

// ---------------------------------------------------------
// RCookieManager::Version
// ---------------------------------------------------------
//
TVersion RCookieManager::Version() const
    {
    CLOG(( EClient, 0, _L("<->RCookieManager::Version") ));

	return( TVersion( KCookieServerMajorVersionNumber,
                      KCookieServerMinorVersionNumber, 
                      KCookieServerBuildVersionNumber ));
    }
    
// ---------------------------------------------------------
// RCookieManager::SetAppUidL
// ---------------------------------------------------------
//
EXPORT_C TInt RCookieManager::SetAppUidL( const TUint32& aAppUid )
    {
    CLOG(( EClient, 0, _L("->RCookieManager::SetAppUid") ));   
    HBufC* buf = HBufC::NewLC(128);
    TPtr ptr(buf->Des());
    ptr.AppendNum(aAppUid,EHex);    
    TInt error = SendReceive(ESetAppUid,TIpcArgs(ptr.Length(),&ptr)); 
    CleanupStack::PopAndDestroy();
    CLOG(( EClient, 0, _L("<-RCookieManager::SetAppUid") ));
    return error;
    }
// End of file
