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
// Internal class for manipulating the common bookmark properties.
// @internalComponent
// 
//

#include "bkmrkproperties.h"
#include "bkmrkbase.h"

CBkmrkProperties* CBkmrkProperties::NewL(CBookmarkBase& aParent, CBookmarkDb& aDb, CRepository& aRepository)
	{
	CBkmrkProperties* self = new (ELeave) CBkmrkProperties(aParent, aDb, aRepository);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CBkmrkProperties::CBkmrkProperties(CBookmarkBase& aParent, CBookmarkDb& aDb, CRepository& aRepository) 
	: CRepositoryAccessor(), iParent(aParent), iDatabase(aDb), iIconId(Bookmark::KNullAttachmentID), iUpdateModifiedOnCommit ( ETrue )
	{
	SetRepository(aRepository);
	}

CBkmrkProperties::~CBkmrkProperties()
	{
	delete iDescription;
	delete iCustomProperties;
	}

void CBkmrkProperties::ConstructL()
	{
	CPropertyReg::TPropertyGroup type = CPropertyReg::EGroupBookmark;
	if (iParent.Type() == Bookmark::ETypeFolder)
		{
		type = CPropertyReg::EGroupFolder;
		}
	iCustomProperties = CPropertyList::NewL(*iRepository, iDatabase.PropertyRegister(), type, IndexBase() + KBkmrkCusPropertyStart);
	}

const TDesC& CBkmrkProperties::Description() const
	{
	if (!iDescription)
		{
		return KNullDesC;
		}
	return *iDescription;
	}
	
void CBkmrkProperties::SetDescriptionL(const TDesC& aDescription)
	{
	// The aDescription must be smaller that the maximim descriptor storage size
	__ASSERT_ALWAYS(aDescription.Length() <= Bookmark::KMaxDescriptorLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));

	iParent.LeaveIfNotWritableL();
	
	if (iDescription)
		{
		delete iDescription;
		iDescription = NULL;
		}
	iDescription = aDescription.AllocL();
	SetDirty();
	}

Bookmark::TAttachmentId CBkmrkProperties::IconId() const
	{
	return iIconId;
	}
	
void CBkmrkProperties::SetIconIdL(Bookmark::TAttachmentId aIconId)
	{
	iParent.LeaveIfNotWritableL();
	// ensure the icon exists
	iDatabase.IconL(aIconId);
	iIconId = aIconId;
	SetDirty();
	}

TUint32 CBkmrkProperties::IndexBase()
	{
	return iParent.IndexBase();
	}
	
void CBkmrkProperties::SetIdFromIndexBase(TUint32 aIndexBase)
	{
	return iParent.SetIdFromIndexBase(aIndexBase);
	}
	
void CBkmrkProperties::CommitL()
	{
	CRepositoryAccessor::CommitL();
	iCustomProperties->CommitL();
	}
	
void CBkmrkProperties::SetDirty()
	{
	CRepositoryAccessor::SetDirty();
	iParent.SetDirty();
	}
	
void CBkmrkProperties::TransNewL()
	{
	SetModified ();
	
	TUint32 indexBase = IndexBase();
	// create entries in the repository
	TUint32 timeLo = I64LOW(iLastModified.Int64());
	TUint32 timeHi = I64HIGH(iLastModified.Int64());
	iRepository->Create(indexBase + KBkmrkLastModifiedLoIndex, static_cast<TInt>(timeLo));
	iRepository->Create(indexBase + KBkmrkLastModifiedHiIndex, static_cast<TInt>(timeHi));
	iRepository->Create(indexBase + KCmnDescriptionIndex, Description());
	iRepository->Create(indexBase + KCmnIconIndex, static_cast<TInt>(iIconId));
	iCustomProperties->TransNewL();
	}

void CBkmrkProperties::TransSaveL()
	{
	SetModified ();
	
	TUint32 indexBase = IndexBase();
	TUint32 timeLo = I64LOW(iLastModified.Int64());
	TUint32 timeHi = I64HIGH(iLastModified.Int64());
	iRepository->Set(indexBase + KBkmrkLastModifiedLoIndex, static_cast<TInt>(timeLo));
	iRepository->Set(indexBase + KBkmrkLastModifiedHiIndex, static_cast<TInt>(timeHi));
	iRepository->Set(indexBase + KCmnDescriptionIndex, Description());
	iRepository->Set(indexBase + KCmnIconIndex, static_cast<TInt>(iIconId));
	iCustomProperties->TransSaveL();
	}

void CBkmrkProperties::TransLoadL()
	{
	TUint32 indexBase = IndexBase();
	TInt low  = 0;
	TInt high = 0;
	TInt err = iRepository->Get( indexBase + KBkmrkLastModifiedLoIndex, low );
	if ( err == KErrNone )
		{
		User::LeaveIfError(iRepository->Get(indexBase + KBkmrkLastModifiedHiIndex, high));					
		TUint32 timeLo = static_cast<TUint32>(low);
		TUint32 timeHi = static_cast<TUint32>(high);
		TInt64 time = MAKE_TINT64(timeHi, timeLo);
		iLastModified = time;
		}
	else
		{
		// Set the time as 1st January 1970 midnight.
		TDateTime dt ( 1970, EJanuary, 1, 0, 0, 0, 0 );
		iLastModified = TTime ( dt );
		}
		
	HBufC* descBuffer = HBufC::NewLC(Bookmark::KMaxDescriptorLength);
	TPtr descPtr = descBuffer->Des();
	User::LeaveIfError(iRepository->Get(indexBase + KCmnDescriptionIndex, descPtr));
	delete iDescription;
	iDescription = NULL;
	iDescription = descPtr.AllocL();
	CleanupStack::PopAndDestroy(descBuffer);
	
	TInt retVal;
	User::LeaveIfError(iRepository->Get(indexBase + KCmnIconIndex, retVal));
	iIconId = static_cast<Bookmark::TAttachmentId>(retVal);
	
	iCustomProperties->TransLoadL();
	SetClean();
	}

void CBkmrkProperties::TransRemoveL()
	{
	TUint32 indexBase = IndexBase();
	iRepository->Delete(indexBase + KCmnDescriptionIndex);
	iRepository->Delete(indexBase + KCmnIconIndex);
	TInt low  = 0;
	TInt err = iRepository->Get( indexBase + KBkmrkLastModifiedLoIndex, low );
	if ( err == KErrNone )
		{
		iRepository->Delete(indexBase + KBkmrkLastModifiedLoIndex);
		iRepository->Delete(indexBase + KBkmrkLastModifiedHiIndex);			
		}
	iCustomProperties->TransRemoveL();
	}

CPropertyList& CBkmrkProperties::CustomProperties()
	{
	return *iCustomProperties;
	}

TTime CBkmrkProperties::LastModified () const
	{
	return iLastModified;		
	}
	
void CBkmrkProperties::SetLastModified ( const TTime& aTime )
	{
	iLastModified = aTime.Int64();	
	iUpdateModifiedOnCommit  = EFalse;
	}

void CBkmrkProperties::SetModified ()
	{
	if ( iUpdateModifiedOnCommit  )
		{
		TTime currentTime;
		currentTime.HomeTime ();
		
		SetLastModified ( currentTime );						
		}
	// Reset the value.		
	iUpdateModifiedOnCommit = ETrue; 
	}
