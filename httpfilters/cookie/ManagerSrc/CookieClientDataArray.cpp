/*
 * CookieClientDataArray.cpp
 *
 *  Created on: Dec 18, 2009
 *      Author: mohanti
 */

//System Includes
#include <e32cmn.h>
#include <e32base.h>
#include <e32std.h>

//User Includes
#include "CookieClientDataArray.h"
#include "cookiemanagerclient.h"
#include "CookieArray.h"
#include "CookieLogger.h"
#include "CookieClientData.h"

//Constatnts
const TInt KCookieArrayGranularity = 10;


// ---------------------------------------------------------
// CCookieClientDataArray::New
// ---------------------------------------------------------
//

CCookieClientDataArray* CCookieClientDataArray::New()
    {
    CCookieClientDataArray* arrayInstance = new CCookieClientDataArray();
	if (arrayInstance)
		{
		arrayInstance->Init();
		}
    return arrayInstance ;
    }

CCookieClientDataArray::CCookieClientDataArray()
    {
    }

void CCookieClientDataArray::Init()
    {
    iCookieClientData = new RPointerArray<CCookieClientData>(KCookieArrayGranularity);
    }    

// ---------------------------------------------------------
// CCookieClientDataArray::~CCookieClientDataArray
// ---------------------------------------------------------
//
CCookieClientDataArray::~CCookieClientDataArray()
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::~CCookieClientDataArray" ) ) );
    if(iCookieClientData)
        {
         iCookieClientData->ResetAndDestroy();
         iCookieClientData->Close();
         delete iCookieClientData;
         iCookieClientData = NULL;
        }
    CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::~CCookieClientDataArray" ) ) );
    }

// ---------------------------------------------------------
// CCookieClientDataArray::Count
// ---------------------------------------------------------
//
TInt CCookieClientDataArray::Count()const
    {
     if (iCookieClientData)
       return iCookieClientData->Count();
     else 
       return 0;
    }

// ---------------------------------------------------------
// CCookieClientDataArray::Compress
// ---------------------------------------------------------
//
void CCookieClientDataArray::AddClientGroupDataL(CCookieClientData* aCookieClientData)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::AddGroupDataL" ) ) );
    iCookieClientData->AppendL(aCookieClientData);
    CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::AddGroupDataL" ) ) );
    }

// ---------------------------------------------------------
// CCookieClientDataArray::Compress
// ---------------------------------------------------------
//
void CCookieClientDataArray::Compress()
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::Compress" ) ) );
    iCookieClientData->Compress();
    CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::Compress" ) ) );
    }

// ---------------------------------------------------------
// CCookieClientDataArray::operator[]
// ---------------------------------------------------------
//
CCookieClientData* CCookieClientDataArray::operator[](TInt aIndex) const
    {
    return (*iCookieClientData)[aIndex];
    }

// ---------------------------------------------------------
// CCookieClientDataArray::At
// ---------------------------------------------------------
//
CCookieClientData* CCookieClientDataArray::At(TInt aIndex) const
    {
    return (*iCookieClientData)[ aIndex ];
    }
// ---------------------------------------------------------
// CCookieClientDataArray::Find
// ---------------------------------------------------------
//
CCookieClientData*  CCookieClientDataArray::Find(const TUint32& aGroupId, const TUint32& aWidgetUid )
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::Find GroupId = %x, WidgetUid = %x" ),aGroupId,aWidgetUid ) );
    TInt total = Count();
    TBool WidgetUidFound = EFalse;
    TInt index = 0;
    for ( TInt i = 0;i<total;i++ )
        {
         if (At(i)->GetGroupId()== aGroupId && (At(i)->GetCookieSharableFlag()))
           {
            WidgetUidFound = ETrue;
            index = i;
            break;
           }
         else if (At(i)->GetGroupId()== aGroupId && !(At(i)->GetCookieSharableFlag()) 
            && (At(i)->GetWidgetUid()== aWidgetUid))
           {
            WidgetUidFound = ETrue;
            index = i;
            break;
           }
        }
  if ( WidgetUidFound )
      {
      CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::Find found with index = %d" ),index ) );
      return At(index);
      }
  else
      {
      CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::Find not found " ) ) );
      return NULL;
      }
    }
// ---------------------------------------------------------
// CCookieClientDataArray::FindIndex
// ---------------------------------------------------------
//
TInt CCookieClientDataArray::FindIndex(const TUint32& aGroupId, const TUint32& aWidgetUid,TInt& aIndex )
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::FindIndex GroupId = %x,WidgetUid = %x" )
            ,aGroupId,aWidgetUid ) );
    TInt total = Count();
    TBool WidgetUidFound = EFalse;
    TInt index = 0;
    for ( TInt i = 0;i<total;i++ )
        {
         if (At(i)->GetGroupId()== aGroupId && (At(i)->GetCookieSharableFlag()))
           {
            WidgetUidFound = ETrue;
            index = i;
            break;
           }
         else if (At(i)->GetGroupId()== aGroupId && !(At(i)->GetCookieSharableFlag()) 
            && (At(i)->GetWidgetUid()== aWidgetUid))
           {
            WidgetUidFound = ETrue;
            index = i;
            break;
           }
        }
  if ( WidgetUidFound )
      {
      aIndex = index;
      CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::FindIndex found with index = %d" ),index ) );
      return KErrNone;
      }
  else
      {
      CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::FindIndex not found " ) ) );
      return KErrNotFound;
      }
    }

// ---------------------------------------------------------
// CCookieClientDataArray::CookieArray
// ---------------------------------------------------------
//
CCookieArray* CCookieClientDataArray::CookieArray(TInt aIndex)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::CookieArray Index =%d" ),aIndex ) );
    CCookieClientData* cookieclientdata = At(aIndex);
    return cookieclientdata->CookieArray();
    }


// ---------------------------------------------------------
// CCookieClientDataArray::Remove
// ---------------------------------------------------------
//
void CCookieClientDataArray::Remove(TInt aIndex)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::Remove %d" ), aIndex ) );
    delete (*iCookieClientData)[ aIndex ];
    iCookieClientData->Remove(aIndex);
    iCookieClientData->Compress();
    CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::Remove " )) );
    }


// ---------------------------------------------------------
// CCookieClientDataArray::DestroyGroupData
// ---------------------------------------------------------
//
TInt CCookieClientDataArray::DestroyClientData(const TUint32 aAppUid)
    {
    CLOG( ( EClient, 0, _L( "-> CCookieClientDataArray::DestroyClientData" ) ) );
    TUint32 groupid = RProcess().SecureId().iId;
    TInt index = -1;
    TInt err = FindIndex(groupid,aAppUid,index);
    if ( err == KErrNone )
      {
       Remove(index);
      }
    CLOG( ( EClient, 0, _L( "<- CCookieClientDataArray::DestroyClientData removed index = %d" ),index ) );
    return err;
    }
//EOF
