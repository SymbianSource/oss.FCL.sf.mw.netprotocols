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
#include "CookieArray.h"
#include "cookieipc.h"
#include "CookieCommonConstants.h"
#include "CookieLogger.h"
#include "cookiemanagerclient.h"
#include "CookieManagerServer.h"
#include "CookieManagerStart.h"
#include "CookieServerDef.h"
#include "CookieClientDataArray.h"
#include "CookieClientData.h"
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
: iStringPool( aStringPool ),iCookieMgrData(NULL)
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
    iCookieMgrData = new TCookieMgrInternalStruct(iStringPool);
    if (!(iCookieMgrData && iCookieMgrData->Init() == KErrNone))
        return KErrNoMemory;
            
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
// RCookieManager::DoGetCookies
// ---------------------------------------------------------
//
TInt RCookieManager::DoGetCookies( TDes8& aBuffer ) const
	{
	return SendReceive( EGetCookies, TIpcArgs( &aBuffer ) );
	}

// ---------------------------------------------------------
// RCookieManager::GetCookieSharableFlagFromServer
// ---------------------------------------------------------
//
TInt RCookieManager::GetCookieSharableFlagFromServer(TBool& aCookieSharableFlag )const
    {
    TPckg<TBool> cookieSharableFlag(aCookieSharableFlag);
    return SendReceive( EGetCookieSharableFlag, TIpcArgs( &cookieSharableFlag ) );
    }

// ---------------------------------------------------------
// RCookieManager::DoGetCookieSize
// ---------------------------------------------------------
//
TInt RCookieManager::DoGetCookieSize( const TDesC8& aRequestUri,
									  TPckg<TInt>& aPkgSize,TDesC& aAppuidPtr ) const
	{
    return SendReceive( EGetCookieSize, TIpcArgs( &aAppuidPtr,aRequestUri.Length(), 
                                                  &aRequestUri, &aPkgSize ) );
	}
// ---------------------------------------------------------
// RCookieManager::DestroyCookiesFromMemory
// ---------------------------------------------------------
//
TInt RCookieManager::DestroyCookiesFromMemory( TInt& aDeleteStatus )
    {
    CLOG(( EClient, 0, _L("-> RCookieManager::DestroyCookies") ));

    TPckg<TInt> pkgStatus( aDeleteStatus );
    TInt ret = SendReceive( EDestroyCookies, TIpcArgs( &pkgStatus ) ) ;

    CLOG(( EClient, 0, 
        _L("<- RCookieManager::DestroyCookies cleared %d cookies, errcode%d"),
        aDeleteStatus, ret ) );

    return ret;

    }

// ---------------------------------------------------------
// RCookieManager::DoStoreCookie
// ---------------------------------------------------------
//
TInt RCookieManager::DoStoreCookie( const TDesC8& aPackedCookie,
								   const TDesC8& aUri,TDesC& aAppUidPtr ) const
	{
    return SendReceive( EStoreCookie, TIpcArgs( aPackedCookie.Length(), 
                                      &aPackedCookie, &aUri,&aAppUidPtr ) );
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
    TBool cookiefound(EFalse);
    CCookieClientDataArray* cookieclientdataarray = iCookieMgrData->GetCookieClientDataArray();
    
    if(cookieclientdataarray) 
        {
    TInt clientarraycount = cookieclientdataarray->Count();
    if ( clientarraycount!=0 )
        {
        TInt clerr = GetClientSideCookies(aUri,aCookies,cookiefound,0);
        }
      } 

    if(!cookiefound)
        {
	aCookie2Reqd = EFalse;

	TInt size = 0;
	TPckg<TInt> pkgSize( size );
	//Appuid value only takes 8 chars
    HBufC* appbuf = HBufC::New(8);
    TPtr ptr(appbuf->Des());
    TUint32 appUid(0);
    ptr.AppendNum(appUid,EHex);    
	User::LeaveIfError( DoGetCookieSize( aUri, pkgSize,ptr ) );
    delete appbuf;
	if ( size )
		{
		HBufC8* buf = HBufC8::NewLC( size );

		TPtr8 des( buf->Des() );
		User::LeaveIfError( DoGetCookies( des ) );

		// it seems this is the only place where we cannot avoid leaving
		// ==> we allocate memory for cookies when we fill up the cookie array.
		iCookieMgrData->GetCookiePacker().UnpackCookiesFromBufferL( *buf, aCookies );
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
    StoreCookieAtClientSide(&aCookie,aUri.UriDes()); 
	TInt err;

	TInt cookieSize = aCookie.Size( EFalse );
	HBufC8* buf = HBufC8::New( cookieSize );
	if ( buf )
		{
		CLOG( ( EClient, 0,
			    _L( "RCookieManager::StoreCookie, cookie size:%d" ), 
                cookieSize ) );

		TPtr8 bufDes( buf->Des() );
		err = iCookieMgrData->GetCookiePacker().CliPackCookie( bufDes, aCookie );
		if ( !err )
			{
			//Appuid value only takes 8 chars
            HBufC* appbuf = HBufC::New(8);
            TPtr ptr(appbuf->Des());
            TUint32 appUid(0);
            ptr.AppendNum(appUid,EHex);    
            err = DoStoreCookie( *buf, aUri.UriDes(),ptr );
            delete appbuf;
			//err = DoStoreCookie( *buf, aUri.UriDes() );
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
// RCookieManager::SetAppUidL This method is no longer being 
// used in CookieFilter.As this method is exported so in order
// to provide backward compatibilty this API implementation is changed 
// ---------------------------------------------------------
//
EXPORT_C TInt RCookieManager::SetAppUidL( const TUint32& aAppUid )
    {
    CLOG(( EClient, 0, _L("->RCookieManager::SetAppUid") ));   
    TUint32 groupid = RProcess().SecureId().iId;
    CCookieClientData* cookieclientdata = iCookieMgrData->GetCookieClientDataArray()->Find(groupid, const_cast<TUint32&> (aAppUid));
    if(cookieclientdata)
        {
        cookieclientdata->SetWidgetUid(aAppUid);
        }
    else
        {
        TBool cookieSharableFlag(EFalse);
        TInt err = GetCookieSharableFlagFromServer(cookieSharableFlag);
        cookieclientdata = CCookieClientData::NewL(groupid, aAppUid, cookieSharableFlag,ETrue);
        cookieclientdata->SetInitFlag(ETrue);
        iCookieMgrData->GetCookieClientDataArray()->AddClientGroupDataL(cookieclientdata);
        }
    //Appuid value only takes 8 chars
    HBufC* buf = HBufC::NewLC(8);
    TPtr ptr(buf->Des());
    ptr.AppendNum(aAppUid,EHex);
    TInt error = SendReceive(ESetAppUid,TIpcArgs(ptr.Length(),&ptr)); 
    CleanupStack::PopAndDestroy();
    CLOG(( EClient, 0, _L("<-RCookieManager::SetAppUid") ));
    return error;
    }
// ---------------------------------------------------------
// RCookieManager::Close
// ---------------------------------------------------------
//

EXPORT_C void RCookieManager::Close()
    {
    CLOG(( EClient, 0, _L("-> RCookieManager::Close") ));
    TInt deletestatus =0;
    DestroyCookiesFromMemory(deletestatus);
    delete iCookieMgrData;
    iCookieMgrData = NULL;
    CLOG(( EClient, 0, _L("-> RCookieManager::Close deletestatus = %d"), deletestatus ));
    RSessionBase::Close();
    CLOG(( EClient, 0, _L("<- RCookieManager::Close") ));
    }
// ---------------------------------------------------------
// RCookieManager::StoreCookie Newly Added
// ---------------------------------------------------------
//
EXPORT_C TInt RCookieManager::StoreCookie( const CCookie& aCookie,
                                 const TUriC8& aUri,TUint32& aAppUid )
    {
    CLOG( ( EClient, 0, _L( "-> RCookieManager::StoreCookie" ) ) );
    StoreCookieAtClientSide(&aCookie,aUri.UriDes(),aAppUid);
    TInt err;

    TInt cookieSize = aCookie.Size( EFalse );
    HBufC8* buf = HBufC8::New( cookieSize );
    if ( buf )
        {
        CLOG( ( EClient, 0,
                _L( "RCookieManager::StoreCookie, cookie size:%d" ), 
                cookieSize ) );

        TPtr8 bufDes( buf->Des() );
        err = iCookieMgrData->GetCookiePacker().CliPackCookie( bufDes, aCookie );
        
        if ( !err )
            {
            //Appuid value only takes 8 chars
            HBufC* appbuf = HBufC::New(8);
            TPtr ptr(appbuf->Des());
            ptr.AppendNum(aAppUid,EHex);    
            err = DoStoreCookie( *buf, aUri.UriDes(),ptr );
            delete appbuf;
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
// RCookieManager::GetCookiesL Newly Added
// ---------------------------------------------------------
//
EXPORT_C void RCookieManager::GetCookiesL( const TDesC8& aUri,
                                        RPointerArray<CCookie>& aCookies,
                                        TBool& aCookie2Reqd,TUint32& aAppUid )
    {
    CLOG( ( EClient, 0, _L( "-> RCookieManager::GetCookiesL" ) ) );
    TBool cookiefound(EFalse);
    CCookieClientDataArray* cookieclientdataarray = iCookieMgrData->GetCookieClientDataArray();
    if(cookieclientdataarray)
        {
         TInt clientarraycount = cookieclientdataarray->Count();
         if (clientarraycount == 0)
           {
            TUint32 groupid = RProcess().SecureId().iId;
            TBool cookieSharableFlag(EFalse);
            TInt err = GetCookieSharableFlagFromServer(cookieSharableFlag);
            CCookieClientData* cookieclientdata = CCookieClientData::NewL(groupid, aAppUid, cookieSharableFlag,ETrue);
            cookieclientdata->SetInitFlag(ETrue);
            cookieclientdataarray->AddClientGroupDataL(cookieclientdata);
           }
         else
           {
        
            CLOG( ( EClient, 0, _L( "-> RCookieManager::GetClientSideCookies:" ) ) );
            //Gets the Cookie objects for aUri in case it is present
            TInt clerr = GetClientSideCookies(aUri,aCookies,cookiefound,aAppUid);
            CLOG( ( EClient, 0, _L( "RCookieManager::GetClientSideCookies:cookiefound = %d" ), cookiefound ) );
        
           }
        } 
    if(!cookiefound)
        {
            aCookie2Reqd = EFalse;
            //Appuid value only takes 8 chars
            HBufC* appuidbuf = HBufC::NewL(8);
            TPtr ptr(appuidbuf->Des());
            ptr.AppendNum(aAppUid,EHex);    
            TInt size = 0;
            TPckg<TInt> pkgSize( size );
            User::LeaveIfError( DoGetCookieSize( aUri, pkgSize, ptr ) );
            delete appuidbuf;
        if ( size )
            {
            HBufC8* buf = HBufC8::NewLC( size );
    
            TPtr8 des( buf->Des() );
            User::LeaveIfError( DoGetCookies( des ) );
    
            // it seems this is the only place where we cannot avoid leaving
            // ==> we allocate memory for cookies when we fill up the cookie array.
            iCookieMgrData->GetCookiePacker().UnpackCookiesFromBufferL( *buf, aCookies );
            TInt count = aCookies.Count();
            for(TInt j=0; j<count; j++)
                {
                CLOG( ( EClient, 0, _L( "-> RCookieManager::StoreClientSideCookies: aUri=%S" ), &aUri ) );
                //cookieclientdata->StoreCookieAtClientSide( aCookies[j],aUri );
                StoreCookieAtClientSide( aCookies[j],aUri,aAppUid );
                }
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
    
            CleanupStack::PopAndDestroy();  // buf
            }
      }
    CLOG( ( EClient, 0, _L( "<- RCookieManager::GetCookiesL" ) ) );
    }

// ---------------------------------------------------------
// RCookieManager::Close for Widget related destruction during 
//uninstallation process of a widget
// ---------------------------------------------------------
//
EXPORT_C TInt RCookieManager::ClearAllAppUidCookies(const TUint32& aAppUid)
    {
    CLOG(( EClient, 0, _L("-> RCookieManager::ClearCookies") ));
    //Client side cookie deletion specific to a appuid
    iCookieMgrData->GetCookieClientDataArray()->DestroyClientData(aAppUid);
    //Server side Cookie deletion specific to a appuid
    //Appuid value only takes 8 chars
    HBufC* buf = HBufC::NewLC(8);
    TPtr ptr(buf->Des());
    ptr.AppendNum(aAppUid,EHex);
    TInt error = SendReceive(EClearAppUidCookies,TIpcArgs(ptr.Length(),&ptr)); 
    CleanupStack::PopAndDestroy();
    return error;
    }

// ---------------------------------------------------------
// RCookieManager::StoreCookieAtClientSide
// ---------------------------------------------------------
//
void RCookieManager::StoreCookieAtClientSide( const CCookie* aCookie, const TDesC8& aUri,TUint32 aWidgetUid )
    {
    CLOG( ( EClient, 0, _L( "-> RCookieManager::StoreCookieAtClientSide" ) ) );
    TUint32 groupid = RProcess().SecureId().iId;
    CCookieClientDataArray* cookieclientdataarray = iCookieMgrData->GetCookieClientDataArray();
    CCookieClientData* cookieclientdata(NULL);
    TInt count = cookieclientdataarray->Count();
    if (count == 0)
        {
        TBool cookieSharableFlag(EFalse);
        TInt err = GetCookieSharableFlagFromServer(cookieSharableFlag);
        cookieclientdata = CCookieClientData::NewL(groupid, aWidgetUid, cookieSharableFlag,ETrue);
        cookieclientdata->SetInitFlag(ETrue);
        cookieclientdataarray->AddClientGroupDataL(cookieclientdata);
        cookieclientdata->StoreCookieAtClientSideL(aCookie,aUri);
        return;
        }
    TInt itemIndex =0;
    for(TInt i=0; i<count; i++)
        {
          if (cookieclientdataarray->At(i)->GetGroupId() == groupid 
                  && cookieclientdataarray->At(i)->GetCookieSharableFlag())
              {
              itemIndex = i;
              break;
              }
          else if(cookieclientdataarray->At(i)->GetGroupId() == groupid
                  && !(cookieclientdataarray->At(i)->GetCookieSharableFlag()) 
                  && cookieclientdataarray->At(i)->GetWidgetUid() == aWidgetUid)
              {
              itemIndex = i;
              break;
              }
        }
    cookieclientdata = cookieclientdataarray->At(itemIndex);
    cookieclientdata->StoreCookieAtClientSideL(aCookie,aUri);
    CLOG( ( EClient, 0, _L( "<- RCookieManager::StoreCookieAtClientSide" ) ) );
    }

// ---------------------------------------------------------
// RCookieManager::GetClientSideCookies
// ---------------------------------------------------------
//
TInt RCookieManager::GetClientSideCookies( const TDesC8& aRequestUri,RPointerArray<CCookie>& aCookies
                ,TBool& aCookieFound, TUint32 aWidgetUid )
    {
    CLOG( ( EClient, 0, _L( "-> RCookieManager::GetClientSideCookies" ) ) );
    TUint32 groupid = RProcess().SecureId().iId;
    CCookieClientData* cookieclientdata = NULL;
    CCookieClientDataArray* cookieclientdataarray = iCookieMgrData->GetCookieClientDataArray();
    TInt count = cookieclientdataarray->Count();
    if (count == 0)
        {
        TBool cookieSharableFlag(EFalse);
        TInt err = GetCookieSharableFlagFromServer(cookieSharableFlag);
        cookieclientdata = CCookieClientData::NewL(groupid, aWidgetUid, cookieSharableFlag,ETrue);
        cookieclientdata->SetInitFlag(ETrue);
        cookieclientdataarray->AddClientGroupDataL(cookieclientdata);
        } else 
           {
           cookieclientdata = cookieclientdataarray->Find(groupid, aWidgetUid);
           }
     if(cookieclientdata)
        {
         if(!cookieclientdata->GetInitFlag())
           {
            TBool cookieSharableFlag(EFalse);
            TInt err = GetCookieSharableFlagFromServer(cookieSharableFlag);
            cookieclientdata->SetCookieCookieSharableFlag(cookieSharableFlag);
           }
           return cookieclientdata->GetClientSideCookies( aRequestUri,aCookies, aCookieFound );
         } else 
              {
               TBool cookieSharableFlag(EFalse);
               TInt err = GetCookieSharableFlagFromServer(cookieSharableFlag);
               if(err == KErrNone)
                 {
                   CCookieClientData* cookieclientdata 
                    = CCookieClientData::NewL(groupid, aWidgetUid, cookieSharableFlag,ETrue);
                    cookieclientdataarray->AddClientGroupDataL(cookieclientdata);
                  }
               }
    CLOG( ( EClient, 0, _L( "<- RCookieManager::GetClientSideCookies" ) ) );
    return KErrNone;
    }

RCookieManager::TCookieMgrInternalStruct::~TCookieMgrInternalStruct()
    {
    delete iCookieClientDataArray;
    }

TInt RCookieManager::TCookieMgrInternalStruct::Init()
    {
    iCookieClientDataArray = CCookieClientDataArray::New();
    if (iCookieClientDataArray)
        return KErrNone;
    return KErrNoMemory;
        
    }

// End of file
