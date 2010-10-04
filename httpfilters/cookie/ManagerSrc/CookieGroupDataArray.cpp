/*
 * CCookieGroupDataArray.cpp
 *
 *  Created on: Nov 24, 2009
 *      Author: mohanti
 */
//System Includes
#include <e32cmn.h>
#include <e32base.h>

//User Includes
#include "CookieGroupDataArray.h"
#include "CookieGroupData.h"
#include "GroupIdInfoArray.h"
#include "CookieArray.h"
#include "CookieLogger.h"

//Constatnts
const TInt KCookieArrayGranularity = 10;



// ---------------------------------------------------------
// CCookieGroupDataArray::NewL
// ---------------------------------------------------------
//

CCookieGroupDataArray* CCookieGroupDataArray::NewL()
    {
    CCookieGroupDataArray* self = CCookieGroupDataArray::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::NewLC
// ---------------------------------------------------------
//

CCookieGroupDataArray* CCookieGroupDataArray::NewLC()
    {
    CCookieGroupDataArray* self = new (ELeave) CCookieGroupDataArray();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::ConstructL
// ---------------------------------------------------------
//

void CCookieGroupDataArray::ConstructL()
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::ConstructL" ) ) );
    iCookieGroupData = new (ELeave) RPointerArray<CCookieGroupData>(KCookieArrayGranularity);
    CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::ConstructL" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::CCookieGroupDataArray
// ---------------------------------------------------------
//

CCookieGroupDataArray::CCookieGroupDataArray()
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::CCookieGroupDataArray" ) ) );
    CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::CCookieGroupDataArray" ) ) );
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::~CCookieGroupDataArray
// ---------------------------------------------------------
//

CCookieGroupDataArray::~CCookieGroupDataArray()
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::~CCookieGroupDataArray" ) ) );
    if(iCookieGroupData)
        {
         iCookieGroupData->ResetAndDestroy();
         iCookieGroupData->Close();
         delete iCookieGroupData;
         iCookieGroupData = NULL;
        }
    CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::~CCookieGroupDataArray" ) ) );
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::Count
// ---------------------------------------------------------
//
TInt CCookieGroupDataArray::Count()const
    {
    return iCookieGroupData->Count(); 
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::FindIndex
// ---------------------------------------------------------
//
TInt CCookieGroupDataArray::FindIndex(TUint32 aGroupId,TInt& aIndex )
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::FindIndex aGroupId = %x" ), aGroupId ) );
    TInt count = Count();
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::count = %d" ), count ) );
    for(TInt i=0;i<count;i++)
        {
        CCookieGroupData* p = At(i);
        if( p->GetGroupId() == aGroupId)
            {
            aIndex=i;
            CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::FindIndex found for GroupId %x with Index = %d" )
                    , aGroupId,aIndex ) );
            return KErrNone;
            }
        }
     CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::FindIndex NotFound for GroupId %x with Index = %d" )
             , aGroupId ,aIndex) );    
     return KErrNotFound;
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::FindIndex
// ---------------------------------------------------------
//
TInt CCookieGroupDataArray::FindIndex(TUint32 aGroupId,TUint32 aAppUid,TInt& aIndex )
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::FindIndex aGroupId = %x,aAppUid = %x" ), aGroupId, aAppUid ) );
    TInt count = Count();
    for(TInt i=0;i<count;i++)
        {
        CCookieGroupData* p = At(i);
        TInt appCount = p->TotalAppUid();
        TInt appUidFound(EFalse);
        if( p->GetGroupId() == aGroupId)
            {
             for(TInt j=0; j<appCount; j++)
                 {
                  if (p->GetSharedUid(j)== aAppUid)
                      appUidFound = ETrue;
                  break;
                 }
             if (appUidFound)
                 {
                 aIndex=i;
                 return KErrNone;
                 }
            }
        }
     return KErrNotFound;
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::Compress
// ---------------------------------------------------------
//
void CCookieGroupDataArray::AddGroupDataL(CCookieGroupData* aCcookieGroupData)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::AddGroupDataL" ) ) );
    iCookieGroupData->AppendL(aCcookieGroupData);
    CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::AddGroupDataL" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::Compress
// ---------------------------------------------------------
//
void CCookieGroupDataArray::Compress()
    {
    iCookieGroupData->Compress();
    }


// ---------------------------------------------------------
// CCookieGroupDataArray::operator[]
// ---------------------------------------------------------
//
CCookieGroupData* CCookieGroupDataArray::operator[](TInt aIndex) const
    {
    return (*iCookieGroupData)[aIndex];
    }

  // ---------------------------------------------------------
  // CCookieGroupDataArray::GetGroupData
  // ---------------------------------------------------------
  //
CCookieGroupData* CCookieGroupDataArray::At(TInt aIndex) const
    {
    return (*iCookieGroupData)[ aIndex ];
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::GetGroupData
// ---------------------------------------------------------
//
CCookieGroupData* CCookieGroupDataArray::GetGroupData(TInt aIndex)
    {
    return(At(aIndex));
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::GetGroupDataIndexL
// ---------------------------------------------------------
//
void CCookieGroupDataArray::GetGroupDataIndexL(TUint32 aGroupId,TUint32 aAppUid,TBool& aCookieSharableFlag, TInt& aIndex)
    {
     CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::GetGroupDataIndexL aGroupId = %x, aAppUid = %x" ), aGroupId, aAppUid ) );
     TInt count = Count();
     TBool found(EFalse);
     CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::GetGroupDataIndexL count = %d" ), count ) );
     if ( count == 0 )
         {
         RArray<TUint32> appUidArray(1);
         appUidArray.Append(aAppUid);
         CCookieGroupData* cookiegroupdata = CCookieGroupData::NewL(aGroupId,appUidArray,aCookieSharableFlag);
         CleanupStack::PushL( cookiegroupdata );
         AddGroupDataL(cookiegroupdata);
         aIndex = 0;
         CleanupStack::Pop(cookiegroupdata);
         appUidArray.Close();
         } else {
                
                TInt totalappUid = iCookieGroupData->Count();
                for ( TInt i=0; i<count; i++ )
                   {
                    CCookieGroupData* cookiegroupdata = At(i);
                    if ( cookiegroupdata->GetGroupId() == aGroupId && !(cookiegroupdata->GetCookieSharableFlag()) )
                      {
                      found = ETrue;
                      aIndex = i;
                      break;
                      }else if (cookiegroupdata->GetGroupId() == aGroupId && (cookiegroupdata->GetCookieSharableFlag()))
                          {
                           TBool groupMatch(ETrue);
                           TBool sharedUidMatch(EFalse);
                           TInt total = cookiegroupdata->TotalAppUid();
                           for(TInt j=0;j<total;j++)
                              {
                               if (cookiegroupdata->GetSharedUid(j)== aAppUid)
                                   {
                                       sharedUidMatch = ETrue;
                                       break;
                                   }
                              }
                           if (groupMatch || sharedUidMatch)
                               {
                               found = ETrue;
                               aIndex = i;
                               break;
                               }
                          }
                          
                        
                   CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::GetGroupDataIndexL aGroupId %x and aAppUid %x found at index %d" ), aGroupId, aAppUid, aIndex ) );
                      }
                    }
               if(!found)
                {
                 RArray<TUint32> appUidArray(1);
                 appUidArray.Append(aAppUid);
                 CCookieGroupData* cookiegroupdata = CCookieGroupData::NewL(aGroupId, appUidArray, aCookieSharableFlag);
                 CleanupStack::PushL( cookiegroupdata );
                 AddGroupDataL(cookiegroupdata);
                 TInt total = Count();
                 aIndex = total-1;
                 CleanupStack::Pop(cookiegroupdata);
                 appUidArray.Close();
                }
     CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::GetGroupDataIndexL" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::CookieArray
// ---------------------------------------------------------
//
CCookieArray* CCookieGroupDataArray::CookieArray(TInt aIndex)
    {
    CCookieGroupData* cookiegroupdata = At(aIndex);
    return cookiegroupdata->CookieArray();
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::GetCookies
// ---------------------------------------------------------
//
TInt CCookieGroupDataArray::GetCookies( const TDesC8& aRequestUri,
                                      RPointerArray<CCookie>& aCookies,TInt aIndex ) const
    {
    CCookieArray* cookieArray = At(aIndex)->CookieArray();
    return cookieArray->GetCookies( aRequestUri, aCookies );
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::Remove
// ---------------------------------------------------------
//
void CCookieGroupDataArray::Remove(TInt aIndex)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray:: Remove %d" ), aIndex ) );
    delete (*iCookieGroupData)[ aIndex ];
    iCookieGroupData->Remove(aIndex);
    iCookieGroupData->Compress();
    CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::Remove " ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::DestroyGroupData
// ---------------------------------------------------------
//
TInt CCookieGroupDataArray::DestroyGroupData(TUint32 aGroupId)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::DestroyGroupData aGroupId = %x" ), aGroupId ) );
    TInt count = Count();
    CLOG( ( EClient, 0, _L( "-> CCookieGroupDataArray::DestroyGroupData count = %d" ), count ) );
    if (count == 0)
        {
         return KErrNotFound;
        }
    
    TBool found(EFalse);
    for (TInt i=0; i<count; i++)
        {
        CCookieGroupData* cookiegroupdata = At(i);
        if ( cookiegroupdata->GetGroupId() == aGroupId)
            {
            found = ETrue;
            Remove(i);
            --i;
            --count;
            CLOG( ( EClient, 0, _L( "<- CCookieGroupDataArray::DestroyGroupData aGroupId = %x removed from Array" ), aGroupId ) );
            }
        }
    if ( found )
        return KErrNone;
	else
		return KErrNotFound;
    }
