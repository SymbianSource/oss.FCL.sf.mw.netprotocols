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
// Place holder for common constants, type definitions and enums.
// 
//


#ifndef __BOOKMARKDEF_H__
#define __BOOKMARKDEF_H__

#include <e32base.h>
#include <centralrepository.h>

/**
The Bookmark namespace contains all the bookmark database specific constants and enums

@publishedPartner
@released
*/
namespace Bookmark
{
	/**
	Id for bookmarks and folders
	*/
	typedef TUint32 TItemId;							
	/**
	Id for bookmark attachments like icons
	*/
	typedef TUint32 TAttachmentId;
	
	/**
	A NULL value for a bookmark item ID
	*/
	const TItemId KNullItemID = 0x00000000;
	/**
	A NULL value for an attachment (icon) ID
	*/
	const TAttachmentId KNullAttachmentID = 0x00000000;
	/**
	The title of the root folder
	*/
	_LIT(KTxtRootTitle, "Root");
	/**
	The root folder's ID
	*/
	const TItemId KRootItemID = 0xffffffff;

	/**
	The maximum length of a 16 bit desciptor for a bookmark property
	*/
	const TInt KMaxDescriptorLength = NCentralRepositoryConstants::KMaxUnicodeStringLength;	
	/**
	The maximum length of a 8 bit desciptor for a bookmark property
	*/
	const TInt KMaxDataLength = NCentralRepositoryConstants::KMaxBinaryLength;
	/**
	The maximum length of a bookmark URI
	*/
	const TInt KMaxUriLength = NCentralRepositoryConstants::KMaxBinaryLength;
	/**
	The maximum number of bookmark entries available in the database
	*/
	const TInt KMaxBookmarkEntries = 0xFFFFFF;
	/**
	The maximum depth that folders can be placed under folders
	*/
	const TInt KMaxFolderDepth = 1024;
	/**
	The maximum number of custom properties that can be registered
	*/
	const TInt KMaxCustomProperties = 255;
	/**
	A bookmark's owner ID will be set to this if it has no owner
	*/
	_LIT_SECURE_ID(KNoOwnerId,0x00000000);		

	/**
	A bookmark item will always be either a bookmark or a folder. The TType enum is used
	to identify which.
	*/
	enum TType
		{
		/** 
		The item is a folder
		*/
		ETypeFolder,
		/** 
		The item is a bookmark			
		*/
		ETypeBookmark			
		};

	/**
	The TVisibility enum is used to specify which bookmarks will be seen by the client opening the
	bookmark database
	*/
	enum TVisibility
		{
		/**
		Open as All
		@deprecated Deprecated in 9.1
		*/				
		EVisibilityDefault,
		/**
		View only public items
		*/
		EVisibilityPublic,
		/**
		View only items owned by the application
		*/
		EVisibilityPrivate,
		/**
		View public items and private items owned by the application
		*/
		EVisibilityAll,
		/**
		View all items regardless of owner - requires WriteUserData
		*/
		EVisibilityManager
		};

	/**
	The TPropertyType is used when registering custom properties. Custom properties must be of one of
	these types
	bookmark database
	*/
	enum TPropertyType
		{
		/**
		The property is an int
		*/
		EDataTypeInt,
		/**
		The property is an floating point number
		*/
		EDataTypeReal,
		/**
		The property is a 16 bit descriptor
		*/
		EDataTypeDes16,
		/**
		The property is a 8 bit descriptor
		*/
		EDataTypeDes8
		};

	/**
	Error codes error numbers -7150 to -7199 have been reserved for the bookmark database
	*/
	static const TInt KErrorBase 			= -7150;
	/**
	The version number in the repository does not match the bookmark database version number
	*/
	static const TInt KErrBadVersion		= KErrorBase;
	/**
	The item is of the wrong type. A folder item is given where a bookmark item is required or visa versa
	*/
	static const TInt KErrWrongType			= KErrorBase - 1;
	/** 
	Not all handles have been closed. A database object has been opened and has not been closed.
	*/
	static const TInt KErrOpenHandle		= KErrorBase - 2;
	/**
	The handle has not been opened. The handle is not attached to any bookmark object
	*/
	static const TInt KErrNotOpen			= KErrorBase - 3;
	/**
	The bookmark item is read only and can not be modified.
	*/
	static const TInt KErrReadOnly			= KErrorBase - 4;
	/**
	An attempt has been made to set the parent of an item to an item it owns lower in the tree. This invalidate the tree structure.
	*/
	static const TInt KErrCyclicLoop		= KErrorBase - 5;
	/**
	The central database store is either locked or unaccessable at this time. You may want to try again later.
	*/
	static const TInt KErrDatabaseLocked	= KErrorBase - 6;
	/**
	The database has no more space for new bookmark entries
	*/
	static const TInt KErrNoMoreSpace		= KErrorBase - 7;
	/**
	The database is in a corrupt state
	*/
	static const TInt KErrCorrupt			= KErrorBase - 8;
	/**
	A custom property with the given UID has not been reistered
	*/
	static const TInt KErrNotRegistered		= KErrorBase - 9;
	/**
	The an attempt was made to register a property using a UID that has already been used
	*/
	static const TInt KErrUidAlreadyUsed	= KErrorBase - 10;
	/**
	An orphaned item is one that is currently in the root folder due to its normal parent not being loaded
	This usually occurs when folders are private but some of the bookmarks it contains are public
	Some operations (such as changing the rank or order of an item) can not be performed on an orphaned object
	*/
	static const TInt KErrOrphanedItem		= KErrorBase - 11;
	/**
	An attempt has been made to create a folder with a title that already exists
	*/
	static const TInt KErrTitleAlreadyUsed	= KErrorBase - 12;
	/**
	The operation is not allowed. (Like trying to delete the root folder)
	*/
	static const TInt KErrOperationDenied	= KErrorBase - 13;
	/**
	The operation can not be completed until the item has been committed to the database (e.g. setting the owner ID)
	*/
	static const TInt KErrNotCommitted		= KErrorBase - 14;

	// Panic text
	_LIT(KBookmarkErrNullPointer, 		"Pointer not null");
	_LIT(KBookmarkErrNegativeRefCount, 	"Ref count zero");
	_LIT(KBookmarkErrOpenHandles, 		"Handle(s) open");
	_LIT(KBookmarkErrWrongType, 		"Wrong item type");
	_LIT(KBookmarkErrHandleNotOpen,		"Handle not open");
	_LIT(KBookmarkErrNoParent,			"No parent");
	_LIT(KBookmarkErrParent,			"Invalid parent");
	_LIT(KBookmarkErrCorrupt,			"Corrupt database");
	_LIT(KBookmarkErrTooLong,			"Length too long");
}

#endif // __BOOKMARKDEF_H__
