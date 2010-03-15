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
* Description:  Implementation of Cookie Manager Session.
*
*/


// INCLUDE FILES
	// System includes
// #include <thttphdrval.h>

	// User includes
#include "cookie.h"
#include "CookieArray.h"
#include "CookieCommonConstants.h"
#include "cookieipc.h"
#include "CookieLogger.h"
#include "CookieManagerServer.h"
#include "CookieManagerSession.h"

#include "CookieServerDef.h"


// ---------------------------------------------------------
// CCookieManagerSession::CCookieManagerSession
// ---------------------------------------------------------
//
CCookieManagerSession::CCookieManagerSession( CCookieManagerServer& aServer ) :
iCookieServer( aServer ),
iStringPool( iCookieServer.StringPool() ), iCookiePacker( *iStringPool ),
iGetCookieList( KCookieStandardGranularity )
	{
    CLOG( ( EServerSession, 0, _L( "" ) ) );
    CLOG( ( EServerSession, 0, _L( "*****************" ) ) );
    CLOG( ( EServerSession, 0,
					_L( "CCookieManagerSession::CCookieManagerSession" ) ) );
	}


// ---------------------------------------------------------
// CCookieManagerSession::ConstructL
// ---------------------------------------------------------
//
void CCookieManagerSession::ConstructL()
    {
    CLOG( ( EServerSession, 0,
						_L( "-> CCookieManagerSession::ConstructL" ) ) );

//	iTransientCookies = new (ELeave) CCookieArray;

    CLOG( ( EServerSession, 0,
						_L( "<- CCookieManagerSession::ConstructL" ) ) );
    }


// ---------------------------------------------------------
// CCookieManagerSession::NewL
// ---------------------------------------------------------
//
CCookieManagerSession* CCookieManagerSession::NewL
										( CCookieManagerServer& aServer )
    {
    CLOG( ( EServerSession, 0, _L( "-> CCookieManagerSession::NewL" ) ) );
	
    CCookieManagerSession* self = 
					            new (ELeave) CCookieManagerSession( aServer );

    CleanupStack::PushL( self );

    self->ConstructL();

    CleanupStack::Pop(); // self

    CLOG( ( EServerSession, 0, _L( "<- CCookieManagerSession::NewL" ) ) );

    return self;
    }


// ---------------------------------------------------------
// CCookieManagerSession::~CCookieManagerSession
// ---------------------------------------------------------
//
CCookieManagerSession::~CCookieManagerSession()
    {
//	delete iTransientCookies;

	iGetCookieList.Close();

    iCookieServer.CloseSession();

    CLOG( ( EServerSession, 0,
					_L( "CCookieManagerSession::~CCookieManagerSession ") ) );
    CLOG( ( EServerSession, 0, _L( "*****************" ) ) );
    }


// ---------------------------------------------------------
// CCookieManagerSession::ClearAllCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::ClearAllCookies( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
				_L( "-> CCookieManagerSession::ClearAllCookies" ) ) );

	TInt err = DoClearAllCookies( aMessage );

    CLOG( ( EServerSession, 0,
				_L( "-> CCookieManagerSession::ClearAllCookies" ) ) );

    return err;
    }


// ---------------------------------------------------------
// CCookieManagerSession::DoClearAllCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoClearAllCookies( const RMessage2& aMessage )
	{
    TPckg<TInt> count( iCookieServer.ClearAllCookies() );
    return aMessage.Write( 0, count );
	}


// ---------------------------------------------------------
// CCookieManagerSession::DoGetCookieSize
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoGetCookieSize( const RMessage2& aMessage )
	{
    TInt err = KErrNone;

	if ( iGetCookieList.Count() || iGetCookieListSize )
		{
		iGetCookieList.Reset();
		iGetCookieListSize = 0;
		}
	else
		{
		// read in the size of the URI in bytes
		TInt uriSize = aMessage.Int0();
		HBufC8* uriBuf = HBufC8::New( uriSize );
        if ( uriBuf ) 
            {
		    // read in the URI
		    TPtr8 uriDes( uriBuf->Des() );
		    err = aMessage.Read( 1, uriDes );
            if ( err == KErrNone )
                {
		        // fill the cookie array with the appropriate cookies :
		        // both from the server (persistent cookies) and from our local 
		        // cookie list (transient cookies)
		        err = iCookieServer.GetCookies( uriDes, iGetCookieList );
                if ( err == KErrNone )
                    {
        /*
    		        User::LeaveIfError( iTransientCookies->GetCookies( uriDes,
														        iGetCookieList ) );
        */

		            // TBD : perhaps it'd be worth passing the size as an argument
		            // in CCookieArray::GetCookies method ==> this way this 
		            // additional loop below could be removed.
		            TInt count = iGetCookieList.Count();
		            for ( TInt i = 0; i < count; i++ )
			            {
			            iGetCookieListSize += iGetCookieList[i]->Size( ETrue );
			            }

		            // writing back the result - the number of bytes to be copied
		            TPckg<TInt> pkgSize( iGetCookieListSize );
                    err = aMessage.Write( 2, pkgSize );
                    }
                }

            delete uriBuf;
            }
        else
            {
            err = KErrNoMemory;
            }
		}

    return err;
	}


// ---------------------------------------------------------
// CCookieManagerSession::DoGetCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoGetCookies( const RMessage2& aMessage )
	{
    TInt err = KErrNone;

    CLOG( ( EServerSession, 0,
				_L( "-> CCookieManagerSession::DoGetCookiesL" ) ) );

	// We do not use __ASSERT_* here, because it terminates only the client,
	// but the server-side session is still running, which may easily cause
	// serious problems
	if ( !iGetCookieList.Count() || !iGetCookieListSize )
		{
		PanicClient( aMessage, ECookieBrokenGetRequest );
		}
	else
		{
		HBufC8* buf = HBufC8::New( iGetCookieListSize );
        if ( buf )
            {
		    TUint8* bufPtr = CONST_CAST( TUint8*, buf->Ptr() );

		    TInt count = iGetCookieList.Count();
		    for ( TInt i = 0; i < count; i++ )
			    {
			    TPtr8 bufDes( bufPtr, iGetCookieList[i]->Size( ETrue ) );
			    err = iCookiePacker.SrvPackCookie ( bufDes, 
                                                    *iGetCookieList[i] );
                if ( err != KErrNone )
                    {
                    break;
                    }

			    bufPtr += bufDes.Length();
			    }

            if ( err == KErrNone )
                {
    		    buf->Des().SetLength( iGetCookieListSize );
                err = aMessage.Write( 0, *buf );
                }

            delete buf;
            }
        else
            {
            err = KErrNoMemory;
            }
		}

    CLOG( ( EServerSession, 0,
				_L( "<- CCookieManagerSession::DoGetCookiesL" ) ) );

    return err;
	}


// ---------------------------------------------------------
// CCookieManagerSession::DoStoreCookieL
// ---------------------------------------------------------
//
void CCookieManagerSession::DoStoreCookieL( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
			_L( "-> CCookieManagerSession::DoStoreCookie ") ) );

    HBufC8* packedCookieBuf = HBufC8::NewLC( aMessage.Int0() );
    TPtr8 packedCookiePtr( packedCookieBuf->Des() );
    aMessage.ReadL( 1, packedCookiePtr );
	CLOG( ( EServerSession, 0, _L("Buffer Size: %d"), aMessage.Int0() ) );
	
    // create a dummy cookie that will be initialized afterwards
    CCookie* cookie = CCookie::NewL( *iStringPool );
    CleanupStack::PushL( cookie );

    // initialize the newly allocated cookie
    // too big Cookie leaves.
    TRAPD( err, iCookiePacker.UnpackCookieL( *packedCookieBuf, *cookie ) );
    if( err == KErrNone )
        {
	    // aMessage.Int2() == request-URI length
	    HBufC8* uriBuf = HBufC8::NewLC( aMessage.Int2() );
	    TPtr8 uriPtr( uriBuf->Des() );

    	aMessage.ReadL( 3, uriPtr );

        // first need to check if it is present in the array as it must 
        // overwrite already existing cookies...
        TInt index(0);
        CCookieArray* perscookiearray = iCookieServer.CookieArray();
        if ( perscookiearray->DoesAlreadyExists( cookie, index ) )
            { // must overwrite !!!
            // but first add the new one if needed
            // just not to give a chance of beeing lost...
            // persistence of the cookie will be handled on 
            // saving all cookies to disk
            perscookiearray->MakeRoomForInsertIfNeededL(cookie, *uriBuf, index);     		
     		if(index >=0)
     			{
     			// insert cookie at valid index    
            	iCookieServer.StorePersistentCookieL( cookie, *uriBuf, index );
            	// remove the old cookie
            	perscookiearray->Remove( index + 1 );
            	}
            else
                {   // invalid index means old cookie has been deleted in the process of making room
                    // append the new cookie to the end of array
                    iCookieServer.StorePersistentCookieL( cookie, *uriBuf );
                }
            }
        else
            { // it is not in the array yet, add it now
            iCookieServer.StorePersistentCookieL( cookie, *uriBuf );
            }

	    CleanupStack::PopAndDestroy( uriBuf );
        }
    else if( err == KErrTooBig )
        {
        // cookie too big, ignore it here, nothing to do
        }
    else
        {
        // some other error happened
        User::Leave( err );
        }
    CleanupStack::Pop( cookie );	// do not destroy the cookie
    CleanupStack::PopAndDestroy( packedCookieBuf );

    CLOG(( EServerSession, 0, _L("<- CCookieManagerSession::DoStoreCookie") ));
    }


// ---------------------------------------------------------
// CCookieManagerSession::GetCookieSize
// ---------------------------------------------------------
//
TInt CCookieManagerSession::GetCookieSize( const RMessage2& aMessage )
	{
    CLOG( ( EServerSession, 0,
				_L( "-> CCookieManagerSession::GetCookieSize" ) ) );

	TInt err = DoGetCookieSize( aMessage );

	// it might happen that the array is filled with cookies and the method
	// leaves afterwards : in this case we have to reset the array.
	if ( err != KErrNone )
		{
		iGetCookieList.Reset();
		iGetCookieListSize = 0;
		}

    CLOG( ( EServerSession, 0,
				_L( "<- CCookieManagerSession::GetCookieSize, errcode%d" ),
				err ) );

	return err;
	}


// ---------------------------------------------------------
// CCookieManagerSession::GetCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::GetCookies( const RMessage2& aMessage )
	{
	CLOG( ( EServerSession, 0,
					_L("-> CCookieManagerSession::GetCookies" ) ) );

	TInt err = DoGetCookies( aMessage );

	// we have to reset the array in any case
	iGetCookieList.Reset();
	iGetCookieListSize = 0;

	CLOG( ( EServerSession, 0,
				_L( "<- CCookieManagerSession::GetCookies, errcode%d" ),
				err ) );

	return err;
	}


// ---------------------------------------------------------
// CCookieManagerSession::PanicClient
// ---------------------------------------------------------
//
void CCookieManagerSession::PanicClient( const RMessage2& aMessage, 
                                         TCookieClientPanic aPanic ) const
	{
    CLOG( ( EServerSession, 0,
					_L( "-> CCookieManagerSession::PanicClient, panic%d" ),
					aPanic ) );

    aMessage.Panic( KCookieClientPanicString, aPanic );
	}


// ---------------------------------------------------------
// CCookieManagerSession::ServiceL
// ---------------------------------------------------------
//
void CCookieManagerSession::ServiceL( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0, _L( "-> CCookieManagerSession::ServiceL" ) ) );

	TInt function = aMessage.Function();
	TInt result = KErrNone;

    // all functions called should ensure that all synchronous messages 
    // have been completed
	switch ( function )
		{
        case EStoreCookie :
            {
            if ( result == KErrNone )
                {
                result = StoreCookie( aMessage );
                }

            aMessage.Complete( result );

            break;
            }
		case EGetCookieSize :
			{
            if ( result == KErrNone )
                {
    			result = GetCookieSize( aMessage );
                }

			aMessage.Complete( result );

			break;
			}
		case EGetCookies :
			{
            if ( result == KErrNone )
                {
    			result = GetCookies( aMessage );
                }

			aMessage.Complete( result );

			break;
			}
        case EClearAllCookies :
            {
            if ( result == KErrNone )
                {
    			result = ClearAllCookies( aMessage );
                }

			aMessage.Complete( result );

            break;
            }
		case ESetAppUid:
		    {
		    if(result == KErrNone )
		        {
		        result = SetAppUidL(aMessage);    
		        }
		    aMessage.Complete(result);
		    break;
		    }
		default :
			PanicClient( aMessage, ECookieBadRequest );
			break;
		}

    CLOG( ( EServerSession, 0,
			_L( "<- CCookieManagerSession::ServiceL, func%d, result%d" ),
			function, result ) );
    }


// ---------------------------------------------------------
// CCookieManagerSession::StoreCookie
// ---------------------------------------------------------
//
TInt CCookieManagerSession::StoreCookie( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
			_L( "-> CCookieManagerSession::StoreCookie" ) ) );
    
    TInt error = KErrNone;
    TRAP( error, DoStoreCookieL( aMessage ) );

    CLOG( ( EServerSession, 0,
			_L( "<- CCookieManagerSession::StoreCookie ") ) );

    return error;
    }
    
// ---------------------------------------------------------
// CCookieManagerSession::SetAppUidL
// ---------------------------------------------------------
//
TInt CCookieManagerSession::SetAppUidL(const RMessage2& aMessage )
    {
   TInt ret(KErrNone);
   //READ FROM MESSAGE
    HBufC* packedCookieBuf = HBufC::NewLC( aMessage.Int0() );
    TPtr packedCookiePtr( packedCookieBuf->Des() );
    aMessage.ReadL( 1, packedCookiePtr );   
    TLex lex(packedCookiePtr);
    TUint32 appUid(0);
    ret = lex.Val(appUid,EHex);    

    //Extract Appuid from File Name
    TPtrC buf = iCookieServer.GetFileName();
    TInt len = buf.LocateReverse('_');
    TPtrC ptr(iCookieServer.GetFileName().Mid(len+1));
    TInt len1 = ptr.LocateReverse('.');
    TPtrC ptr1(ptr.Left(len1));
    if(!packedCookiePtr.Compare(ptr1) || (len == KErrNotFound && !appUid ))
        {
        //Already the Same File
        CleanupStack::PopAndDestroy(packedCookieBuf);
        return ret;    
        }                
    iCookieServer.SetFileName(appUid);
    CleanupStack::PopAndDestroy(packedCookieBuf);
    return ret;    
    }
// End of file
