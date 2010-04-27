/*
 * CGroupIdInfoArray.cpp
 *
 *  Created on: Nov 24, 2009
 *      Author: mohanti
 */
//System Includes

//User Includes
#include "GroupIdInfoArray.h"
#include "CookieLogger.h"

//Constatnts

const TInt KGroupArrayGranularity = 10;

// ---------------------------------------------------------
// CCookieGroupDataArray::NewL
// ---------------------------------------------------------
//

CGroupIdInfo* CGroupIdInfo::NewL( TUint32 aGroupId, const RArray<TUint32>& aAppuid, TBool aCookieSharableFlag )
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::NewL aGroupId = %x, CookieSharableFlag = %x" )
            ,aGroupId,aCookieSharableFlag ) );
    CGroupIdInfo* self = CGroupIdInfo::NewLC(aGroupId, aAppuid, aCookieSharableFlag);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::NewLC
// ---------------------------------------------------------
//

CGroupIdInfo* CGroupIdInfo::NewLC(TUint32 aGroupId, const RArray<TUint32>& aAppuid, TBool aCookieSharableFlag)
    {
    CGroupIdInfo* self = new (ELeave) CGroupIdInfo( aGroupId, aCookieSharableFlag );
    CleanupStack::PushL(self);
    self->ConstructL(aAppuid);
    return self;
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::NewLC
// ---------------------------------------------------------
//

CGroupIdInfo::~CGroupIdInfo()
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::~CGroupIdInfo" ) ) );
    iAppUid.Close();
    CLOG( ( EClient, 0, _L( "<- CGroupIdArray::~CGroupIdInfo" ) ) );
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::NewLC
// ---------------------------------------------------------
//

void CGroupIdInfo::ConstructL(const RArray<TUint32>& aAppuid)
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::ConstructL" ) ) );
    TInt count = aAppuid.Count();
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::count = %d" ), count ) );
    for(TInt i=0; i<count; i++)
        {
        CLOG( ( EClient, 0, _L( "-> CGroupIdArray::aAppuid[%d] = %x " ), aAppuid[i] ) );
        iAppUid.AppendL(aAppuid[i]);
        }
    CLOG( ( EClient, 0, _L( "<- CGroupIdArray::ConstructL" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::CGroupIdInfo
// ---------------------------------------------------------
//

CGroupIdInfo::CGroupIdInfo( TUint32 aGroupId,TBool aCookieSharableFlag ):iCookieSharableFlag( aCookieSharableFlag ), iGroupId(aGroupId)
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::CGroupIdInfo" ) ) );
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::CGroupIdInfo" ) ) );
    }
TBool CGroupIdInfo::GetCookieSharableFlag()
    {
    return iCookieSharableFlag;
    }
// ---------------------------------------------------------
// CGroupIdInfo::GetGroupName
// ---------------------------------------------------------
//

TInt CGroupIdInfo::GetGroupName(TUint32 aSecureId,TUint32& aGroupId)
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::GetGroupName aSecureId = %x,aGroupId = %x" )
            , aSecureId, aGroupId ) );
    if(aSecureId == iGroupId)
        {
        aGroupId = iGroupId; 
        return KErrNone;
        }
    else
        {
        return KErrNotFound;
        }
    }
TBool CGroupIdInfo::IsThisPartOfSharedGroup( TUint32& aClientId )
    {
    TBool found(EFalse);
    if(!GetCookieSharableFlag())
        return EFalse;
    TInt count = iAppUid.Count();
    for (TInt i=0;i<count;i++)
        {
         if ( aClientId == At(i) )
             {
             found = ETrue;
             CLOG( ( EClient, 0, _L( "-> CGroupIdInfo::IsThisPartOfSharedGroup aClientId %x is Part of Group " ), aClientId ) );
             break;
             }
        }
    return found;
    }
// ---------------------------------------------------------
// CGroupIdInfo::AddClientL
// ---------------------------------------------------------
//

void CGroupIdInfo::AddClientL(TUint32 aClientId)
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::AddClientL aClientId %x" ), aClientId  ) );
    iAppUid.AppendL(aClientId);
    CLOG( ( EClient, 0, _L( "<- CGroupIdArray::AddClientL aClientId %x" ) ) );
    }
// ---------------------------------------------------------
// CGroupIdInfo::operator[]
// ---------------------------------------------------------
//

TUint32 CGroupIdInfo::operator[](TInt aIndex) const
    {
    return (iAppUid[aIndex]);
    }

// ---------------------------------------------------------
// CGroupIdInfo::At
// ---------------------------------------------------------
//

TUint32 CGroupIdInfo::At(TInt aIndex) const
    {
    return (iAppUid[ aIndex ]);
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::GetSecureId
// ---------------------------------------------------------
//
const TUint32& CGroupIdInfo::GetGroupId()
    {
    return iGroupId;
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::NewL
// ---------------------------------------------------------
//

CGroupIdArray* CGroupIdArray::NewL()
    {
    CGroupIdArray* self = CGroupIdArray::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::NewLC
// ---------------------------------------------------------
//

CGroupIdArray* CGroupIdArray::NewLC()
    {
    CGroupIdArray* self = new (ELeave) CGroupIdArray();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::CGroupIdArray
// ---------------------------------------------------------
//
CGroupIdArray::CGroupIdArray()
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::CGroupIdArray" ) ) );
    CLOG( ( EClient, 0, _L( "<- CGroupIdArray::CGroupIdArray" ) ) );
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::~CGroupIdArray
// ---------------------------------------------------------
//

CGroupIdArray::~CGroupIdArray()
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::~CGroupIdArray" ) ) );
    if(iGroupIdInfoArray && Count())
        {
        iGroupIdInfoArray->ResetAndDestroy();
        iGroupIdInfoArray->Close();
        //delete iGroupIdInfoArray;
        iGroupIdInfoArray = NULL;
        }
    CLOG( ( EClient, 0, _L( "<- CGroupIdArray::~CGroupIdArray" ) ) );
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::ConstructL
// ---------------------------------------------------------
//
void CGroupIdArray::ConstructL()
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::ConstructL" ) ) );
    iGroupIdInfoArray = new (ELeave) RPointerArray<CGroupIdInfo>(KGroupArrayGranularity);
    CLOG( ( EClient, 0, _L( "<- CGroupIdArray::ConstructL" ) ) );
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::AddGroupIdL
// ---------------------------------------------------------
//

void CGroupIdArray::AddGroupIdL(CGroupIdInfo* aCGroupIdInfo)
    {
    return iGroupIdInfoArray->AppendL(aCGroupIdInfo);
    }

// ---------------------------------------------------------
// CGroupIdArray::operator[]
// ---------------------------------------------------------
//
CGroupIdInfo& CGroupIdArray::operator[](TInt aIndex) const
    {
    return *(*iGroupIdInfoArray)[aIndex];
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::At
// ---------------------------------------------------------
//
CGroupIdInfo& CGroupIdArray::At(TInt aIndex) const
    {
    return *(*iGroupIdInfoArray)[ aIndex ];
    }

// ---------------------------------------------------------
// CCookieGroupDataArray::GetGroupId
// ---------------------------------------------------------
//
TInt CGroupIdArray::GetGroupId(TUint32 aClientId,TUint32& aGroupId)
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::GetGroupId aClientId = %x" ), aClientId ) );
    TInt count = Count();
    TBool found(EFalse);
    TUint32 groupId =0;
    for(TInt i=0;i<count;i++)
        {
        TInt ret = At(i).GetGroupName(aClientId,groupId);
         if ( ret == KErrNone )
             {
             aGroupId = groupId;
             found = ETrue;
             CLOG( ( EClient, 0, _L( "<- CGroupIdArray::Group Id of aClientId= %x is %x" ), aClientId, aGroupId ) );
             break;
             }
           }
    if(found)
        return KErrNone;
    else
        return KErrNotFound;
    }
// ---------------------------------------------------------
// CCookieGroupDataArray::GetGroupId
// ---------------------------------------------------------
//

TInt CGroupIdArray::GetGroupId(TUint32 aClientId,TUint32& aGroupId,TBool& aCookieSharableFlag)
    {
    CLOG( ( EClient, 0, _L( "-> CGroupIdArray::GetGroupId aClientId = %x,aGroupId =%x" ), aClientId, aGroupId ) );
    TInt count = Count();
    TBool found(EFalse);
    
    for(TInt i=0;i<count;i++)
        {
         if( At(i).GetGroupId() == aClientId || At(i).IsThisPartOfSharedGroup(aClientId) )
           {
           aGroupId = At(i).GetGroupId();
           aCookieSharableFlag = At(i).GetCookieSharableFlag();
           found = ETrue;
           break;
           }
        }
    if(found)
        return KErrNone;
    else
        return KErrNotFound;

    }

// ---------------------------------------------------------
// CCookieGroupDataArray::Count
// ---------------------------------------------------------
//
TInt CGroupIdArray::Count()
    {
    return iGroupIdInfoArray->Count();
    }
