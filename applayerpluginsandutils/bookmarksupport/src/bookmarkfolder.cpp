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
//

#include <bookmark.h>
#include "bkmrkfolder.h"

/**
RBkFolder constructor
@publishedPartner
@released
*/
EXPORT_C RBkFolder::RBkFolder() : RBkNode()
	{
	}

/**
Moves the given bookmark item into this folder. Append simply adds the bookmark to the end of
the folder's child list.

@param aBookmarkItem The bookmark item to add
@publishedPartner
@released
*/
EXPORT_C void RBkFolder::AppendL(RBkNode& aBookmarkItem)
	{
	Folder()->AppendL(*(aBookmarkItem.Item()));
	}
	
/**
Moves the given bookmark item into this folder. Insert adds the bookmark into a specific position
in the folder's child list. The item is just added to the end if the specified index is greater
than the number of current children.

@param aBookmarkItem The bookmark item to add
@param aIndexPosition The index position in which to place the bookmark item
@publishedPartner
@released
*/
EXPORT_C void RBkFolder::InsertL(RBkNode& aBookmarkItem, TInt aIndexPosition)
	{
	Folder()->InsertL(*(aBookmarkItem.Item()), aIndexPosition);
	}

/**
Shifts the position of a bookmark item in the child list.

@param aOldPosition The index position of the bookmark item to be moved
@param aNewPosition The new index position where the bookmark item is to be placed
@return A error if either index values are out of range
@publishedPartner
@released
*/
EXPORT_C TInt RBkFolder::Move(TInt aOldPosition, TInt aNewPosition)
	{
	return Folder()->Move(aOldPosition, aNewPosition);
	}
	
/**
Returns the number of items in the folder's child list

@return Number of children in the folder's child list.
@publishedPartner
@released
*/
EXPORT_C TInt RBkFolder::Count() const
	{
	return Folder()->Count();
	}

/**
Gets the index of the child with the given ID.

@param aItem Id of the bookmark item to be found
@return The index value or KErrNotFound
@publishedPartner
@released
*/
EXPORT_C TInt RBkFolder::Index(const RBkNode& aItem) const
	{
	return Folder()->Index(*(aItem.Item()));
	}

/**
Method for opening a handle to a child bookmark item by index value

@param aIndex Index position to retrieve
@return An open handle to the bookmark item
@publishedPartner
@released
*/
EXPORT_C RBkNode RBkFolder::OpenItemL(TInt aIndex)
	{
	return Folder()->At(aIndex).OpenItemL();
	}

/**
Not intended for external use

@internalComponent
*/
CBookmarkFolder* RBkFolder::Folder() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return static_cast<CBookmarkFolder*>(iItem);
	}


