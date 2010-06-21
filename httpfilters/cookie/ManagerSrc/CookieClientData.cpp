/*
 * CookieClientData.cpp
 *
 *  Created on: Dec 28, 2009
 *      Author: mohanti
 */
//User Includes
#include "CookieClientData.h"
#include "cookie.h"
#include "CookieArray.h"
#include "CookieLogger.h"
#include "CookieCommonConstants.h"
#include <uri8.h>

// ---------------------------------------------------------
// CCookieGroupData::NewL
// ---------------------------------------------------------
//
CCookieClientData* CCookieClientData::NewL(TUint32 aGroupId)
    {
    CCookieClientData* self = CCookieClientData::NewLC(aGroupId);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CCookieClientData::NewLC
// ---------------------------------------------------------
//
CCookieClientData* CCookieClientData::NewLC(TUint32 aGroupId)
    {
    CCookieClientData* self = new (ELeave) CCookieClientData(aGroupId);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
// ---------------------------------------------------------
// CCookieGroupData::NewL
// ---------------------------------------------------------
//
CCookieClientData* CCookieClientData::NewL(TUint32 aGroupId,TUint32 aWidgetUid,TBool aCookieSharableFlag, TBool aInitFlag)
    {
    CCookieClientData* self = CCookieClientData::NewLC(aGroupId, aWidgetUid, aCookieSharableFlag, aInitFlag);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CCookieClientData::NewLC
// ---------------------------------------------------------
//
CCookieClientData* CCookieClientData::NewLC(TUint32 aGroupId,TUint32 aWidgetUid,TBool aCookieSharableFlag, TBool aInitFlag)
    {
    CCookieClientData* self = new (ELeave) CCookieClientData(aGroupId, aWidgetUid, aCookieSharableFlag, aInitFlag);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// CCookieClientData::CCookieClientData
// ---------------------------------------------------------
//
CCookieClientData::CCookieClientData(TUint32 aGroupId,TUint32 aWidgetUid ,TBool aCookieSharableFlag , TBool aInitFlag)
:iInitFlag(aInitFlag),iCookieSharableFlag(aCookieSharableFlag),iGroupId(aGroupId),iWidgetUid(aWidgetUid)
    {
    
    }
// ---------------------------------------------------------
// CCookieClientData::CCookieClientData
// ---------------------------------------------------------
//
CCookieClientData::CCookieClientData(TUint32 aGroupId):iInitFlag(EFalse),iCookieSharableFlag(EFalse),iGroupId(aGroupId),iWidgetUid(0)
    {
    
    }
// ---------------------------------------------------------
// CCookieGroupData::ConstructL
// ---------------------------------------------------------
//
void CCookieClientData::ConstructL()
    {
    iCookieArray = new (ELeave) CCookieArray;
    }

// ---------------------------------------------------------
// CCookieClientData::~CCookieClientData
// ---------------------------------------------------------
//
CCookieClientData::~CCookieClientData()
    {
    CLOG(( EClient, 0, _L("-> CCookieClientData::~CCookieClientData iCookieArray: 0x%08x"), iCookieArray ));
    delete iCookieArray;
    iCookieArray = NULL;
    CLOG(( EClient, 0, _L("<- CCookieClientData::~CCookieClientData") ));
    }

// ---------------------------------------------------------
// CCookieClientData::StorePersistentCookieL
// ---------------------------------------------------------
//
void CCookieClientData::StorePersistentCookieL( const CCookie* aCookie,
                                                 const TDesC8& aRequestUri,
                                                 const TInt aIndex )
    {
    CLOG(( EClient, 0, _L("-> RCookieManager::StorePersistentCookieL") ));
    if (aIndex == -1)
        {
        iCookieArray->AddL( aCookie, aRequestUri);
        }
    else
        {
        iCookieArray->InsertL( aCookie, aIndex);
        }
    CLOG(( EClient, 0, _L("<- RCookieManager::StorePersistentCookieL") ));
    }

// ---------------------------------------------------------
// RCookieManager::StoreCookieAtClientSideL
// ---------------------------------------------------------
//
void CCookieClientData::StoreCookieAtClientSideL(const CCookie* aCookie,const TDesC8& aUri )
    {
    CLOG(( EClient, 0, _L("-> RCookieManager::StoreCookieAtClientSideL: aUri:%S"), &aUri ));
    //Creates a clone of the passed cookie objects as the ownership of this object is held by Clint of Cookie Manager Dll
    TUriParser8 uriParser;
    
    TInt err = uriParser.Parse( aUri );
    CCookie* clone(NULL);
    if (err !=KErrNone)
        return;
    else
        {
        // first get the details of the current requestUri,
        // that is, Domain, Path and port
        TPtrC8 requestPath( uriParser.IsPresent( EUriPath ) ?
                            uriParser.Extract( EUriPath ) : KNullDesC8() );
        TPtrC8 requestDomain( uriParser.IsPresent( EUriHost ) ?
                            uriParser.Extract( EUriHost ) : KNullDesC8() );
        TPtrC8 requestPort( uriParser.IsPresent( EUriPort ) ?
                uriParser.Extract( EUriPort ) : KCookieDefaultRequestPort() );

        clone = CCookie::CloneL( *aCookie,requestDomain,requestPath,requestPort);
        CleanupStack::PushL( clone );
        }

    //CCookie* clone = CCookie::CloneL( *aCookie,requestDomain,requestPath);
    //CleanupStack::PushL( clone );
  
    TInt index(0);
    CCookieArray* perscookiearray = CookieArray();
        if ( perscookiearray->DoesAlreadyExists( clone, index ) )
        { // must overwrite !!!
        // but first add the new one if needed
        // just not to give a chance of beeing lost...
        // persistence of the cookie will be handled on 
        // saving all cookies to disk
        perscookiearray->MakeRoomForInsertIfNeededL(clone, aUri, index);            
        if(index >=0)
            {
            // insert cookie at valid index 
            //iCookieClientData->StorePersistentCookieL(aCookie,aUri,index);
            StorePersistentCookieL(clone,aUri,index);
            //iCookieClientData->StorePersistentCookieL( aCookie, aUri, index );
            // remove the old cookie
            perscookiearray->Remove( index + 1 );
            }
        else
            {   // invalid index means old cookie has been deleted in the process of making room
                // append the new cookie to the end of array
            //iCookieClientData->StorePersistentCookieL( aCookie, aUri );
            StorePersistentCookieL( clone, aUri );
            }
        }
    else
        { // it is not in the array yet, add it now
        //iCookieClientData->StorePersistentCookieL( aCookie, aUri );
        StorePersistentCookieL( clone, aUri );
        }
    // Cookie supervisor API takes immediate ownership of both cookie and array
    CleanupStack::Pop(clone);
    CLOG(( EClient, 0, _L("<- RCookieManager::StoreCookieAtClientSideL") )); 
    }

// ---------------------------------------------------------
// CCookieGroupData::Count
// ---------------------------------------------------------
//
TInt CCookieClientData::Count()const
    {
    return iCookieArray->Count(); 
    }

// ---------------------------------------------------------

// ---------------------------------------------------------
// CCookieGroupData::GetClientSideCookies
// ---------------------------------------------------------
//
TInt CCookieClientData::GetClientSideCookies(const TDesC8& aRequestUri,RPointerArray<CCookie>& aCookies,TBool& aCookieFound )const
    {
    return iCookieArray->GetCookies(aRequestUri,aCookies,aCookieFound);
    }

// ---------------------------------------------------------
// CCookieGroupData::CookieArray
// ---------------------------------------------------------
//
CCookieArray* CCookieClientData::CookieArray()
    {
    return iCookieArray;
    }
//EOF
