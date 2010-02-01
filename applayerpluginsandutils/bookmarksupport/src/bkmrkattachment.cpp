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
// Internal class for manipulating bookmark extended properties.
// @internalComponent
// 
//

#include "bkmrkattachment.h"

CBkmrkAttachment* CBkmrkAttachment::NewL(CRepository& aRepository, const TDesC8& aData)
	{
	CBkmrkAttachment* self = new (ELeave) CBkmrkAttachment();
	CleanupStack::PushL(self);
	self->ConstructL(aRepository, aData);
	CleanupStack::Pop(self);
	return self;
	}

CBkmrkAttachment::CBkmrkAttachment() 
	: CRepositoryAccessor(), iId(Bookmark::KNullAttachmentID)
	{
	}

CBkmrkAttachment::~CBkmrkAttachment()
	{
	delete iData;
	}

void CBkmrkAttachment::ConstructL(CRepository& aRepository, const TDesC8& aData)
	{
	SetRepository(aRepository);
	SetDataL(aData);
	}

Bookmark::TAttachmentId CBkmrkAttachment::Id() const
	{
	return iId;
	}
	
void CBkmrkAttachment::SetIdL()
	{
	iId = NextIndexL();
	}

TInt CBkmrkAttachment::Size() const
	{
	return iData->Size();
	}
	
const TDesC8& CBkmrkAttachment::Data() const
	{
	return *iData;
	}

void CBkmrkAttachment::SetDataL(const TDesC8& aData)
	{
	// The aData must be smaller that the maximim data storage size
	__ASSERT_ALWAYS(aData.Length() <= Bookmark::KMaxDataLength, User::Panic(Bookmark::KBookmarkErrTooLong, KErrArgument));
	
	delete iData;
	iData = NULL;
	iData = aData.AllocL();
	SetDirty();
	}

TUint32 CBkmrkAttachment::IndexBase()
	{
	return (iId << KIconIndexShift);
	}
	
void CBkmrkAttachment::SetIdFromIndexBase(TUint32 aIndexBase)
	{
	iId = aIndexBase >> KIconIndexShift;
	}

void CBkmrkAttachment::TransNewL()
	{
	// create entries in the repository
	iRepository->Create(IndexBase() + KIconDataIndex, Data());
	}
	
void CBkmrkAttachment::TransSaveL()
	{
	iRepository->Set(IndexBase() + KIconDataIndex, Data());
	}

void CBkmrkAttachment::TransLoadL()
	{
	HBufC8* dataBuffer = HBufC8::NewLC(Bookmark::KMaxUriLength);
	TPtr8 dataPtr = dataBuffer->Des();
	User::LeaveIfError(iRepository->Get(IndexBase() + KIconDataIndex, dataPtr));
	SetDataL(dataPtr);
	CleanupStack::PopAndDestroy(dataBuffer);
	SetClean();
	}

void CBkmrkAttachment::TransRemoveL()
	{
	iRepository->Delete(IndexBase() + KIconDataIndex);
	}

