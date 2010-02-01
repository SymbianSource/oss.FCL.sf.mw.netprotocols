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
// All the index values used to reference items in the central repository
// @internalComponent
// 
//

#ifndef __REPOSITORY_H__
#define __REPOSITORY_H__

#include <centralrepository.h>
#include <bookmarkdef.h>

// Repository IDs
const TUid KUidBookmarkDatabaseRepository	= { 0x10205AF8 };
const TUid KUidFolderRepository 			= { 0x10205AF9 };
const TUid KUidBookmarkRepository 			= { 0x10205AFA };
const TUid KUidIconRepository 				= { 0x10205AFB };

const TUint32 KIndexBaseZero	= 0;
const TUint32 KSemaphoreIndex	= 0;

// Common index list
const TUint32 KVersionIndex		= 1;
const TUint32 KRepNextIndex		= 2;

// Database repository index list
const TUint32 KDbHomeIdIndex 			= 3;
const TUint32 KDbHomeTextIndex 			= 4;
const TUint32 KDbSearchUriIndex 		= 5;
const TUint32 KDbDefaultProxyIndex 		= 6;
const TUint32 KDbDefaultNapIndex 		= 7;

// Common bookmark properties index list
const TUint32 KCmnTitleIndex 			= 0;
const TUint32 KCmnParentIndex 			= 1;
const TUint32 KCmnFlagsIndex 			= 2;
const TUint32 KCmnOwnerIndex 			= 3;
const TUint32 KCmnDescriptionIndex 		= 4;
const TUint32 KCmnIconIndex				= 5;
const TUint32 KCmnRankIndex				= 6;
const TUint32 KBkmrkLastModifiedLoIndex = 7;
const TUint32 KBkmrkLastModifiedHiIndex = 8;

// Bookmark property index list
const TUint32 KBkmrkLastVisitedLoIndex	= 20;
const TUint32 KBkmrkLastVisitedHiIndex	= 21;
const TUint32 KBkmrkURIIndex			= 22;
const TUint32 KBkmrkAuthNameIndex		= 23;
const TUint32 KBkmrkAuthPasswordIndex	= 24;
const TUint32 KBkmrkAuthMethodIndex		= 25;
const TUint32 KBkmrkProxyIndex			= 26;
const TUint32 KBkmrkNapIndex			= 27;

// Custom property start index
const TUint32 KDbCusPropertyStart		= 0x00000480;
const TUint32 KBkmrkCusPropertyStart	= 0x00000080;
const TUint32 KCusPropertyMask			= 0xFFFFFF80;

// Icon properties index list
const TUint32 KIconDataIndex 			= 0;

// Shifts required to isolate the index value
const TInt KBookmarkIndexShift			= 8;
const TInt KIconIndexShift				= 2;

const TUint32 KRepExactMask				= 0xFFFFFFFF;
const TUint32 KRepIndexMask				= 0x000000FF;
const TUint32 KRepIndexFind				= 0x00000000;
const TUint32 KRepIconIndexMask			= 0x00000003;

const TUint32 KRepPropertyMask			= 0xFFFFFF00;
const TUint32 KRepBkmrkPropertyStart	= 0x00000100;
const TUint32 KRepFldrPropertyStart		= 0x00000200;
const TUint32 KRepDbPropertyStart		= 0x00000300;

const TUint32 KRepFirstBookmarkIndex	= 0x100;

/**
This class provides a standard way of saving and loading properties to the central repository. 
It provides methods for committing data and for maintaining a 'modified' status. This status is used to
determine what actions, if any, are required when the item is committed. 
Items that save and load themselves on the central repository derive from this class
*/
class CRepositoryAccessor: public CBase
	{
public:
	enum TTransType
		{
		ETransNew,		// create a new repository entry
		ETransSave,		// save a respository entry
		ETransLoad,		// load a respository entry
		ETransRemove	// remove an entry from the repository
		};

protected:
	enum TBkmrkStatus
		{
		EStatusClean,	// is in the database and has not been modified
		EStatusDirty,	// has changes that have not been committed
		EStatusCreating,// is being created and is not yet committed
		EStatusDeleted	// is being deleted from the repository
		};

public:
	CRepositoryAccessor();

	void SetRepository(CRepository& aRepository);

	virtual void CommitL();
	virtual void DeleteL();
	virtual void SetDirty();
	void SetClean();
	Bookmark::TItemId NextIndexL();
	
	void TransactionL(TTransType aTransType);
	
	virtual TUint32 IndexBase() = 0;
	virtual void SetIdFromIndexBase(TUint32 aIndexBase) = 0;
	
	virtual void TransNewL() = 0;
	virtual void TransSaveL() = 0;
	virtual void TransLoadL() = 0;
	virtual void TransRemoveL() = 0;

protected:
	TBkmrkStatus		iStatus;
	CRepository*		iRepository;
	};

typedef RArray<CRepositoryAccessor*>	TBkmrkAccessorList;

#endif // __REPOSITORY_H__
