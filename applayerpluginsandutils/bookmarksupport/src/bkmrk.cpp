// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Internal class for manipulating the bookmarks.
// @internalComponent
// 
//

#include "bkmrk.h"
#include "bkmrkfolder.h"
#include "bkmrkdb.h"
#include "repository.h"

CBookmark* CBookmark::NewL(CBookmarkFolder& aParent, CBookmarkDb& aDb)
	{
	CBookmark* self = new (ELeave) CBookmark(aDb);
	CleanupStack::PushL(self);
	self->ConstructL(aParent);	
	CleanupStack::Pop(self);
	return self;
	}
	
CBookmark::CBookmark(CBookmarkDb& aDb)
	: CBookmarkBase(aDb, aDb.BookmarkRepository())
	{
	}
	
CBookmark::~CBookmark()
	{
	delete iTitle;
	delete iUri;
	delete iProperties;
	}
	

void CBookmark::DeleteL()
	{
	iDatabase.RemoveBookmarkL(this);
	iParent->Remove(*this);
	iParent = NULL;
	iStatus = EStatusDeleted;
	}

void CBookmark::ConstructL(CBookmarkFolder& aParent)
	{
	aParent.AppendL(*this);
	iUri = CUri8::NewL();
	}

Bookmark::TType CBookmark::Type() const
	{
	return Bookmark::ETypeBookmark;
	}
	
RBkBookmark CBookmark::OpenBookmark()
	{
	RBkBookmark bookmark;
	bookmark.SetItem(*this);
	return bookmark;
	}

TBool CBookmark::IsHomePage()
	{
	TBool isHome = EFalse;
	CBookmark* home = iDatabase.Home();
	if (home)
		{
		isHome = (home->Id() == Id());
		}
	return isHome;
	}
	
const TTime& CBookmark::LastVisited() const
	{
	return iLastVisited;
	}
	
void CBookmark::SetLastVisited(const TTime& aTime)
	{
	iLastVisited = aTime;
	SetDirty();
	}
	
void CBookmark::UpdateVisited()
	{
	iLastVisited.HomeTime();
	SetDirty();
	}

const TDesC8& CBookmark::Uri() const
	{
	return iUri->Uri().UriDes();
	}

void CBookmark::SetUriL(const TDesC8& aUri)
	{
	// The aUri must be smaller that the maximim uri storage size
	__ASSERT_ALWAYS(aUri.Length() <= Bookmark::KMaxUriLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));
	LeaveIfNotWritableL();
	
	delete iUri;
	iUri = NULL;
	
	TUriParser8 parser;
	User::LeaveIfError(parser.Parse(aUri));
	if (parser.Extract(EUriUserinfo).Length() != 0)
		{
		CAuthentication* auth = CAuthentication::NewL(parser);
		CleanupStack::PushL(auth);
		CBkmrkExtendedProperties& properties = BkmrkExtendedPropertiesL();
		properties.SetAuthenticationL(*auth);
		CleanupStack::PopAndDestroy(auth);
			
		// construct the uri without the userinfo
		iUri = CUri8::NewL();
		iUri->SetComponentL(parser.Extract(EUriScheme), EUriScheme);
		iUri->SetComponentL(parser.Extract(EUriHost), EUriHost);
		iUri->SetComponentL(parser.Extract(EUriPort), EUriPort);
		iUri->SetComponentL(parser.Extract(EUriPath), EUriPath);
		iUri->SetComponentL(parser.Extract(EUriQuery), EUriQuery);
		iUri->SetComponentL(parser.Extract(EUriFragment), EUriFragment);
		}
	else
		{
		iUri = CUri8::NewL(parser);
		}
		
	SetDirty();
	}

CBkmrkExtendedProperties& CBookmark::BkmrkExtendedPropertiesL()
	{
	if (!iProperties)
		{
		iProperties = CBkmrkExtendedProperties::NewL(*this, iDatabase, *iRepository);
		if (iStatus != EStatusCreating)
			{
			iProperties->TransactionL(ETransLoad);
			}
		}
		
	return *iProperties;
	}


// From CRepositoryAccessor
TUint32 CBookmark::IndexBase()
	{
	return (iId << KBookmarkIndexShift);
	}
	
void CBookmark::SetIdFromIndexBase(TUint32 aIndexBase)
	{
	iId = aIndexBase >> KBookmarkIndexShift;
	}

void CBookmark::TransNewL()
	{
	CBookmarkBase::TransNewL();
	TUint32 indexBase = IndexBase();
	TUint32 timeLo = I64LOW(iLastVisited.Int64());
	TUint32 timeHi = I64HIGH(iLastVisited.Int64());
	iRepository->Create(indexBase + KBkmrkLastVisitedLoIndex, static_cast<TInt>(timeLo));
	iRepository->Create(indexBase + KBkmrkLastVisitedHiIndex, static_cast<TInt>(timeHi));
	iRepository->Create(indexBase + KBkmrkURIIndex, Uri());
	BkmrkExtendedPropertiesL().TransNewL();
	}
	
void CBookmark::TransSaveL()
	{
	TUint32 indexBase = IndexBase();
	CBookmarkBase::TransSaveL();
	TUint32 timeLo = I64LOW(iLastVisited.Int64());
	TUint32 timeHi = I64HIGH(iLastVisited.Int64());
	iRepository->Set(indexBase + KBkmrkLastVisitedLoIndex, static_cast<TInt>(timeLo));
	iRepository->Set(indexBase + KBkmrkLastVisitedHiIndex, static_cast<TInt>(timeHi));
	iRepository->Set(indexBase + KBkmrkURIIndex, Uri());
	if (iProperties)
		{
		BkmrkExtendedPropertiesL().TransSaveL();
		}
	}

void CBookmark::TransLoadL()
	{
	CBookmarkBase::TransLoadL();
	
	TUint32 indexBase = IndexBase();
	TInt low  = 0;
	TInt high = 0;
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkLastVisitedLoIndex, low));
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkLastVisitedHiIndex, high));
	TUint32 timeLo = static_cast<TUint32>(low);
	TUint32 timeHi = static_cast<TUint32>(high);
	TInt64 time = MAKE_TINT64(timeHi, timeLo);
	iLastVisited = time;
	HBufC8* uriBuffer = HBufC8::NewLC(Bookmark::KMaxUriLength);
	TPtr8 uriPtr = uriBuffer->Des();
	User::LeaveIfError(iRepository->Get(indexBase + KBkmrkURIIndex, uriPtr));
	SetUriL(uriPtr);
	CleanupStack::PopAndDestroy(uriBuffer);
	
	if (iProperties)
		{
		BkmrkExtendedPropertiesL().TransLoadL();
		}
		
	SetClean();
	}

void CBookmark::TransRemoveL()
	{
	CBookmarkBase::TransRemoveL();
	
	TUint32 indexBase = IndexBase();
	iRepository->Delete(indexBase + KBkmrkLastVisitedLoIndex);
	iRepository->Delete(indexBase + KBkmrkLastVisitedHiIndex);
	iRepository->Delete(indexBase + KBkmrkURIIndex);
	
	// ensure extended properties are loaded
	BkmrkExtendedPropertiesL();
	iProperties->TransRemoveL();
	}

