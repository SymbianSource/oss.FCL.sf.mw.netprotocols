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
#include "CookieGroupDataArray.h"
#include "CookieGroupData.h"
#include "GroupIdInfoArray.h"

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
				_L( "<- CCookieManagerSession::ClearAllCookies" ) ) );

    return err;
    }
// ---------------------------------------------------------
// CCookieManagerSession::ClearAllCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::ClearAllAppUidCookies( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
                _L( "-> CCookieManagerSession::ClearAllAppUidCookies" ) ) );

    TInt err = DoClearAllAppUidCookies( aMessage );

    CLOG( ( EServerSession, 0,
                _L( "<- CCookieManagerSession::ClearAllAppUidCookies" ) ) );

    return err;
    }

// ---------------------------------------------------------
// CCookieManagerSession::DestroyCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DestroyCookies( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
                _L( "-> CCookieManagerSession::DestroyCookies" ) ) );

    TInt err = DoDestroyCookies( aMessage );

    CLOG( ( EServerSession, 0,
                _L( "<- CCookieManagerSession::DestroyCookies" ) ) );

    return err;
    }
// ---------------------------------------------------------
// CCookieManagerSession::GetCookieSharableFlag
// ---------------------------------------------------------
//
TInt CCookieManagerSession::GetCookieSharableFlag( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
                _L( "-> CCookieManagerSession::GetCookieSharableFlag" ) ) );

    TInt err = DoGetCookieSharableFlag( aMessage );

    CLOG( ( EServerSession, 0,
                _L( "<- CCookieManagerSession::GetCookieSharableFlag" ) ) );

    return err;
    }

// ---------------------------------------------------------
// CCookieManagerSession::DoGetCookieSharableFlag
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoGetCookieSharableFlag( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
                _L( "-> CCookieManagerSession::DoGetCookieSharableFlag" ) ) );
    TUint32 secureId = aMessage.SecureId().iId;
    TBool cookiesharableflag(EFalse);
    TInt count = iCookieServer.CookieGroupDataArray()->Count();
    CCookieGroupDataArray* cookiegroupdataarray = iCookieServer.CookieGroupDataArray();
    for (TInt i=0; i<count; i++)
        {
         if (cookiegroupdataarray->At(i)->GetGroupId()== secureId )
             {
             cookiesharableflag = cookiegroupdataarray->At(i)->GetCookieSharableFlag();
             break;         
             }
         else 
             {
             TInt shared = cookiegroupdataarray->At(i)->TotalAppUid();
             for (TInt j=0;j<shared;j++)
                 {
                  if (cookiegroupdataarray->At(i)->GetSharedUid(j)== secureId)
                      {
                      cookiesharableflag = cookiegroupdataarray->At(i)->GetCookieSharableFlag();
                      break;
                      }
                 }
             }
         
        }
    TPckg<TBool> iFlag(cookiesharableflag);
    CLOG( ( EServerSession, 0,_L( "<- CCookieManagerSession::DoGetCookieSharableFlag cookiesharableflag = %d" )
            , cookiesharableflag ) );

    return aMessage.Write( 0, iFlag );
 
    
    }
//----------------------------------------------------------
// CCookieManagerSession::DoClearAllCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoClearAllCookies( const RMessage2& aMessage )
	{
    CLOG( ( EServerSession, 0,
                _L( "-> CCookieManagerSession::DoClearAllCookies" ) ) );

    TUint32 secureId = aMessage.SecureId().iId;
    TInt groupDataIndex =0;
   
    if(iCookieServer.CookieGroupDataArray()->Count())
        {
        TInt err = iCookieServer.CookieGroupDataArray()->FindIndex(secureId, groupDataIndex);
        if ( err == KErrNone )
            {
            CCookieGroupData* cookiegroupdata = iCookieServer.CookieGroupDataArray()->At(groupDataIndex);
            TPckg<TInt>count(cookiegroupdata->ClearAllCookies());
            iCookieServer.CookieGroupDataArray()->Remove(groupDataIndex);
            return aMessage.Write( 0, count );
            }
        }
    TPckg<TInt>count2(0);
    CLOG( ( EServerSession, 0,
                _L( "<- CCookieManagerSession::DoClearAllCookies" ) ) );
    return aMessage.Write( 0, count2 );
	}

//----------------------------------------------------------
// CCookieManagerSession::DoClearAllAppUidCookies
// For deleting cookies belongs to a specific appuid during 
// uninstallation process of a widget
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoClearAllAppUidCookies( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
                _L( "-> CCookieManagerSession::DoClearAllAppUidCookies" ) ) );

    TUint32 secureId = aMessage.SecureId().iId;
    TInt groupDataIndex =-1;
    
    //READ FROM MESSAGE
    HBufC* packedAppUidBuf = HBufC::NewLC( aMessage.Int0() );
    TPtr packedAppUidPtr( packedAppUidBuf->Des() );
    aMessage.ReadL( 1, packedAppUidPtr );   
    TLex lex(packedAppUidPtr);
    TUint32 appUid(0);
    TInt ret = lex.Val(appUid,EHex);

    if(iCookieServer.CookieGroupDataArray()->Count())
        {
        TInt err = iCookieServer.CookieGroupDataArray()->FindIndex(secureId,appUid,groupDataIndex);
        if ( err == KErrNone )
            {
            CCookieGroupData* cookiegroupdata = iCookieServer.CookieGroupDataArray()->At(groupDataIndex);
            TPckg<TInt>count(cookiegroupdata->ClearAllCookies());
            iCookieServer.CookieGroupDataArray()->Remove(groupDataIndex);
            return aMessage.Write( 0, count );
            }
        }
    TPckg<TInt>count2(0);
    CLOG( ( EServerSession, 0,
                 _L( "<- CCookieManagerSession::DoClearAllAppUidCookies" ) ) );
    return aMessage.Write( 0, count2 );
    }

// ---------------------------------------------------------
// CCookieManagerSession::DoDestroyCookies
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoDestroyCookies( const RMessage2& aMessage )
    {
    CLOG( ( EServerSession, 0,
                 _L( "-> CCookieManagerSession::DoDestroyCookies" ) ) );

    TUint32 secureId = aMessage.SecureId().iId;
    TInt err = iCookieServer.CookieGroupDataArray()->DestroyGroupData(secureId);
    TPckg<TInt>status(err);
    CLOG( ( EServerSession, 0,
                _L( "<- CCookieManagerSession::DoDestroyCookies err = %d" ),err ) );
    return aMessage.Write( 0, status );
    }

// ---------------------------------------------------------
// CCookieManagerSession::DoGetCookieSize
// ---------------------------------------------------------
//
TInt CCookieManagerSession::DoGetCookieSize( const RMessage2& aMessage )
	{
	CLOG( ( EServerSession, 0,
	                 _L( "-> CCookieManagerSession::DoGetCookieSize" ) ) );

    TInt err = KErrNone;
    HBufC* CookieBuf = HBufC::NewL(128);
    TPtr CookiePtr( CookieBuf->Des() );
    aMessage.ReadL( 0, CookiePtr );   
    TLex lex(CookiePtr);
    TUint32 appUid(0);
    TInt ret = lex.Val(appUid,EHex);
    delete CookieBuf;
    TUint32 secureId = aMessage.SecureId().iId;
    TUint32 groupId=0;
    TBool cookieSharable(EFalse);
    TInt retstatus = iCookieServer.GroupIdArray()->GetGroupId(secureId,groupId, cookieSharable);
    if( groupId && retstatus != KErrNotFound )
        {
        secureId = groupId;
        }
    TInt groupDataIndex =0;
    iCookieServer.CookieGroupDataArray()->GetGroupDataIndexL(secureId,appUid,cookieSharable,groupDataIndex);        
	if ( iGetCookieList.Count() || iGetCookieListSize )
		{
		iGetCookieList.Reset();
		iGetCookieListSize = 0;
		}
	else
		{
		// read in the size of the URI in bytes
		TInt uriSize = aMessage.Int1();
		HBufC8* uriBuf = HBufC8::New( uriSize );
        if ( uriBuf ) 
            {
		    // read in the URI
		    TPtr8 uriDes( uriBuf->Des() );
		    err = aMessage.Read( 2, uriDes );
            if ( err == KErrNone )
                {
		        // fill the cookie array with the appropriate cookies :
		        // both from the server (persistent cookies) and from our local 
		        // cookie list (transient cookies)
		        err = iCookieServer.GetCookies( uriDes, iGetCookieList,groupDataIndex );
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
                    err = aMessage.Write( 3, pkgSize );
                    }
                }

            delete uriBuf;
            }
        else
            {
            err = KErrNoMemory;
            }
		}
    CLOG( ( EServerSession, 0,
                     _L( "<- CCookieManagerSession::DoGetCookieSize err = %d" ),err ) );
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
    HBufC* cookiebuf = HBufC::NewL(128);
    TPtr cookiePtr( cookiebuf->Des() );
    aMessage.ReadL( 3, cookiePtr );   
    TLex lex(cookiePtr);
    TUint32 appUid(0);
    TInt ret = lex.Val(appUid,EHex);
    delete cookiebuf;
    //Get THe Secure ID
    TUint32 secureId = aMessage.SecureId().iId;
    TInt groupDataIndex =0;
    TUint32 groupId=0;
    TBool cookieSharableFlag(EFalse);
    TInt retstatus = iCookieServer.GroupIdArray()->GetGroupId(secureId,groupId, cookieSharableFlag);
    if( groupId && retstatus != KErrNotFound )
        {
        secureId = groupId;
        }
    iCookieServer.CookieGroupDataArray()->GetGroupDataIndexL(secureId,appUid, cookieSharableFlag,groupDataIndex);        
    CCookieGroupData* cookiegroupdata = iCookieServer.CookieGroupDataArray()->At(groupDataIndex);
    //iGrDataArray->GetGroupDataIndexL(secureId, groupDataIndex);
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
        //Hrdcoded value has taken as 4 slots has already been occupied and 
        //there is no way to pass the uri length from client to Server
        //this length is sufficient enough to accomodate a uri.
        
	    HBufC8* uriBuf = HBufC8::NewLC(500);
	    TPtr8 uriPtr( uriBuf->Des() );

    	aMessage.ReadL( 2, uriPtr );

        // first need to check if it is present in the array as it must 
        // overwrite already existing cookies...
        TInt index(0);
        //CCookieArray* perscookiearray = iCookieServer.CookieArray();
        CCookieArray* perscookiearray = iCookieServer.CookieArray(groupDataIndex);
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
     			cookiegroupdata->StorePersistentCookieL( cookie, *uriBuf, index );
            	// remove the old cookie
            	perscookiearray->Remove( index + 1 );
            	}
            else
                {   // invalid index means old cookie has been deleted in the process of making room
                    // append the new cookie to the end of array
                cookiegroupdata->StorePersistentCookieL( cookie, *uriBuf );
                }
            }
        else
            { // it is not in the array yet, add it now
            cookiegroupdata->StorePersistentCookieL( cookie, *uriBuf );
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
        case EDestroyCookies :
            {
            if ( result == KErrNone )
                {
                result = DestroyCookies( aMessage );
                }

            aMessage.Complete( result );

            break;
            }
        case EGetCookieSharableFlag :
            {
            if ( result == KErrNone )
                {
                result = GetCookieSharableFlag( aMessage );
                }

            aMessage.Complete( result );

            break;
            }
        case EClearAppUidCookies :
            {
            if ( result == KErrNone )
                {
                result = ClearAllAppUidCookies( aMessage );
                }

            aMessage.Complete( result );

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
    CLOG( ( EServerSession, 0,
            _L( "-> CCookieManagerSession::SetAppUidL" ) ) );
    TInt ret(KErrNone);
    //READ FROM MESSAGE
    HBufC* packedCookieBuf = HBufC::NewLC( aMessage.Int0() );
    TPtr packedCookiePtr( packedCookieBuf->Des() );
    aMessage.ReadL( 1, packedCookiePtr );   
    TLex lex(packedCookiePtr);
    TUint32 appUid(0);
    ret = lex.Val(appUid,EHex);    
    TUint32 secureId = aMessage.SecureId().iId;
    TInt groupDataIndex =0;
    //iCookieServer.CookieGroupDataArray()->GetGroupDataIndexL(secureId,appUid,groupDataIndex);
    CCookieGroupData* cookiegroupdata = iCookieServer.CookieGroupDataArray()->At(groupDataIndex);
    cookiegroupdata->SetFileName(appUid,secureId);
    CleanupStack::PopAndDestroy(packedCookieBuf);
    CLOG( ( EServerSession, 0,
                _L( "<- CCookieManagerSession::SetAppUidL" ) ) );

    return ret;    
    }
// End of file
