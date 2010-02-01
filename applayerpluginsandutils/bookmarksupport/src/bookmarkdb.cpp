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

#include <bookmarkdatabase.h>
#include "bkmrkdb.h"
#include "bkmrk.h"
#include "bkmrkfolder.h"
#include "propertyreg.h"

/**
RBkDatabase constructor
@publishedPartner
@released
*/
EXPORT_C RBkDatabase::RBkDatabase() : iDatabase(NULL)
	{
	}

/**
Creates the bookmark database object and tries to connect to the repository. The client can
register an observer if it wants to be notitied of external changes to the database.

@param aVisibility The visibility condition under which the database should be opened
@param aObserver Pointer to an observer that will be notified of changes to the database
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::OpenL(Bookmark::TVisibility aVisibility, MBookmarkObserver* aObserver)
	{
	if (iDatabase)
		{
		// the database is already open so do nothing
		return;
		}
	iDatabase = CBookmarkDb::NewL(aVisibility, aObserver);
	}

/**
Closes the connection to the repository and frees all resources. Does not commit changes
so you must call CommitL() if you want to save before calling Close().

@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::Close()
	{
	delete iDatabase;
	iDatabase = NULL;
	}

/**
Refreshes the bookmark tree. Ususally called in response to a notification that 
the database has been changed from an external source.

@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::RefreshL()
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->RefreshL();
	}

/**
Runs through the folder tree and commits any changes. Also commits any changes to icons,
registered custom properties and database-wide data.

The central repository, which the bookmark database uses, provides no locking mechanism. 
Concurrent accesses are simply queued. Although this should cause no problems it is worth 
remembering if you find any timing and commiting issues.

@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::CommitL()
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->CommitL();
	}

/**
Retrieves the root folder and constructs the internal folder and bookmark tree if it doesn't
yet exist. Only includes folders and bookmarks that are visible to this application.

@return An open handle to the root folder.
@publishedPartner
@released
*/
EXPORT_C RBkFolder RBkDatabase::OpenRootL() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->RootL().OpenFolder();
	}

/**
Creates a new bookmark. If a parent folder is supplied then it is added to this folder. 
If no parent is supplied then it is placed in the root. The newly created bookmark item is 
writable. The new bookmark will be permanently added to the database once there is a call 
to CommitL(). 

@param aParent The new bookmark will be placed in this folder. The bookmark will be placed in the root is this is NULL.
@return An open handle to the created bookmark. This is pushed to cleanup stack.
@publishedPartner
@released
*/
EXPORT_C RBkBookmark RBkDatabase::CreateBookmarkL(RBkFolder* aParent)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	CBookmarkFolder* parent = NULL;
	if (aParent)
		{
		parent = aParent->Folder();
		}
	else
		{
		parent = &(iDatabase->RootL());
		}
	return iDatabase->CreateBookmarkL(*parent).OpenBookmark();
	}
	
/**
Creates a new folder. If a parent folder is supplied then it is added to this folder. 
If no parent is supplied then it is placed in the root. The newly created bookmark item is 
writable. The new folder will be permanently added to the database once there is a call 
to CommitL();

@param aTitle Title text for the folder. Each folder's title text must be unique
@param aParent The new folder will be placed in this folder. The folder will be placed in the root is this is NULL.
@return An open handle to the created folder. This is pushed to cleanup stack.
@publishedPartner
@released
*/
EXPORT_C RBkFolder RBkDatabase::CreateFolderL(const TDesC& aTitle, RBkFolder* aParent)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	CBookmarkFolder* parent = NULL;
	if (aParent)
		{
		parent = aParent->Folder();
		}
	else
		{
		parent = &(iDatabase->RootL());
		}
	return iDatabase->CreateFolderL(aTitle, *parent).OpenFolder();
	}


/**
Deletes a bookmark item. If the item is a folder and aRecursive is false,
all its children are set to the root folder. If aRecursive is true all child 
bookmarks and folders are also deleted.

@param aBookmarkId Id of the bookmark item
@param aRecursive Set to ETrue to delete all child bookmarks and subfolders
@return An error if the item to be deleted is read only
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::DeleteItemL(Bookmark::TItemId aBookmarkId, TBool aRecursive)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->ConstructTreeL();
	iDatabase->DeleteItemL(aBookmarkId, aRecursive);
	}

/**
Finds a bookmark given a bookmark Id. Leaves if the item can not be found

@param aBookmarkID Id of the bookmark item
@return An open handle to the found bookmark.
@publishedPartner
@released
*/
EXPORT_C RBkBookmark RBkDatabase::OpenBookmarkL(Bookmark::TItemId aBookmarkID) const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->ConstructTreeL();
	CBookmarkBase* item = iDatabase->FindItem(aBookmarkID);
	if (!item || item->Type() != Bookmark::ETypeBookmark) 
		{
		User::Leave(KErrNotFound);
		}
		
	RBkNode handle = item->OpenItemL();
	CleanupClosePushL(handle);
	RBkBookmark bookmark;
	bookmark = handle.OpenBookmarkL();
	CleanupStack::PopAndDestroy(&handle);
	
	return bookmark;
	}
	
/**
Finds a folder given a folder Id. Leaves if the item can not be found

@param aFolderID Id of the folder item
@return An open handle to the found folder.
@publishedPartner
@released
*/
EXPORT_C RBkFolder RBkDatabase::OpenFolderL(Bookmark::TItemId aFolderID) const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->ConstructTreeL();
	CBookmarkBase* item = iDatabase->FindItem(aFolderID);
	if (!item || item->Type() != Bookmark::ETypeFolder)
		{
		User::Leave(KErrNotFound);
		}
		
	RBkNode handle = item->OpenItemL();
	CleanupClosePushL(handle);
	RBkFolder folder;
	folder = handle.OpenFolderL();
	CleanupStack::PopAndDestroy(&handle);
	
	return folder;
	}
	
/**
Finds a folder with the given title text. Leaves if the item can not be found

@param aFolderTitle Title text to find
@return An open handle to the found folder.
@publishedPartner
@released
*/
EXPORT_C RBkFolder RBkDatabase::OpenFolderL(const TDesC& aFolderTitle) const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->ConstructTreeL();
	CBookmarkFolder* folder = iDatabase->FindFolder(aFolderTitle);
	if (!folder)
		{
		User::Leave(KErrNotFound);
		}
		
	return folder->OpenFolder();
	}
	
/**
Creates a new icon and adds it to the database's icon list. The database retains ownership of
the icon and the new item's ID is returned. 

@param aIconData Descriptor containing the icon's raw data
@return The new icon's Id.
@publishedPartner
@released
*/
EXPORT_C Bookmark::TAttachmentId RBkDatabase::CreateIconL(const TDesC8& aIconData)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->CreateIconL(aIconData);
	}
	
/**
Gets the icon with the given icon Id.

@param aIconId Id of the icon to find.
@return A reference to a descriptor containing the icon's raw data
@publishedPartner
@released
*/
EXPORT_C const TDesC8& RBkDatabase::GetIconL(Bookmark::TAttachmentId aIconId) const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->IconL(aIconId);
	}
	
/**
Deletes an icon with the given icon Id.

@param aIconId Id of the icon to delete.
@return An error if the icon id can't be found.
@publishedPartner
@released
*/
EXPORT_C TInt RBkDatabase::DeleteIconL(Bookmark::TAttachmentId aIconId)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->DeleteIconL(aIconId);
	}

/**
Gets the database version number

@return The version number.
@publishedPartner
@released
*/
EXPORT_C TVersion RBkDatabase::Version() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->Version();	
	}

/**
Method for getting the home page bookmark. Leaves if no home page has been set.

@return An open handle to the home page bookmark
@publishedPartner
@released
*/
EXPORT_C RBkBookmark RBkDatabase::OpenHomeL() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->ConstructTreeL();
	CBookmark* home = iDatabase->Home();
	if (!home)
		{
		User::Leave(KErrNotFound);
		}
	
	return home->OpenBookmark();
	}
	
/**
Method for setting the home page bookmark. 

@param aHome Reference to the new home page
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetHomeL(const RBkBookmark& aHome)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->ConstructTreeL();
	CBookmark* home =  aHome.Bookmark();
	iDatabase->SetHome(home);
	}

/**
Method for getting the home page text.

@return Descriptor containing the text
@publishedPartner
@released
*/
EXPORT_C const TDesC& RBkDatabase::HomePageText() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->HomePageText();
	}
	
/**
Method for setting the home page text.

@param aHomePageText Descriptor containing the new text
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetHomePageTextL(const TDesC& aHomePageText)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->SetHomePageTextL(aHomePageText);
	}

/**
Method for getting the search page Uri.

@return Descriptor containing the Uri.
@publishedPartner
@released
*/
EXPORT_C const TDesC8& RBkDatabase::SearchUri() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->SearchUri();
	}
/**
Method for setting the search page Uri.

@param aUri Descriptor containing the new Uri.
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetSearchUriL(const TDesC8& aUri)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->SetSearchUriL(aUri);
	}

/**
Method for getting the default proxy.

@return Id for identifying the proxy entry in the commdb
@publishedPartner
@released
*/
EXPORT_C TUint32 RBkDatabase::DefaultProxy() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->DefaultProxy();
	}
	
/**
Method for setting the default proxy.

@param aServiceId Id for identifying the proxy entry in the commdb
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetDefaultProxy(TUint32 aServiceId)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->SetDefaultProxy(aServiceId);
	}

/**
Method for getting the default nap.

@return Id for identifying the nap entry in the commdb
@publishedPartner
@released
*/
EXPORT_C TUint32 RBkDatabase::DefaultNap() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->DefaultNap();
	}
	
/**
Method for setting the default nap.

@param aNetworkId Id for identifying the nap entry in the commdb
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetDefaultNap(TUint32 aNetworkId)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->SetDefaultNap(aNetworkId);
	}

/**
Method for registering a custom database property

@param aCustomId Unique identifier to assign for this property.
@param aDataType The type of data this property will contain.
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::RegisterDatabasePropertyL(TUid aCustomId, Bookmark::TPropertyType aDataType)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->CreateDefaultRepositoriesL ();
	iDatabase->PropertyRegister().RegisterPropertyL(CPropertyReg::EGroupDatabase, aCustomId, aDataType);
	}
	
/**
Method for registering a custom folder property

@param aCustomId Unique identifier to assign for this property.
@param aDataType The type of data this property will contain.
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::RegisterFolderPropertyL(TUid aCustomId, Bookmark::TPropertyType aDataType)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->CreateDefaultRepositoriesL ();
	iDatabase->PropertyRegister().RegisterPropertyL(CPropertyReg::EGroupFolder, aCustomId, aDataType);
	}
	
/**
Method for registering a custom bookmark property

@param aCustomId Unique identifier to assign for this property.
@param aDataType The type of data this property will contain.
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::RegisterBookmarkPropertyL(TUid aCustomId, Bookmark::TPropertyType aDataType)
	{
	iDatabase->CreateDefaultRepositoriesL ();
	iDatabase->PropertyRegister().RegisterPropertyL(CPropertyReg::EGroupBookmark, aCustomId, aDataType);
	}

/**
Method for deregistering a custom database property

@param aCustomId The identifier of the property.
@return An error if a property with this aCustomId does not exist
@publishedPartner
@released
*/
EXPORT_C TInt RBkDatabase::DeregisterDatabasePropertyL(TUid aCustomId)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->PropertyRegister().DeregisterPropertyL(CPropertyReg::EGroupDatabase, aCustomId);
	}
	
/**
Method for deregistering a custom folder property

@param aCustomId The identifier of the property.
@return An error if a property with this aCustomId does not exist
@publishedPartner
@released
*/
EXPORT_C TInt RBkDatabase::DeregisterFolderPropertyL(TUid aCustomId)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->PropertyRegister().DeregisterPropertyL(CPropertyReg::EGroupFolder, aCustomId);
	}
	
/**
Method for deregistering a custom bookmark property

@param aCustomId The identifier of the property.
@return An error if a property with this aCustomId does not exist
@publishedPartner
@released
*/
EXPORT_C TInt RBkDatabase::DeregisterBookmarkPropertyL(TUid aCustomId)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iDatabase->PropertyRegister().DeregisterPropertyL(CPropertyReg::EGroupBookmark, aCustomId);
	}

/**
Gets a custom database property for the given property ID.

@param aPropertyId The ID of the property you wish to get
@param aValue An integer that will hold the data value
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::GetCustomPropertyL(TUid aPropertyId, TInt& aValue) const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	User::LeaveIfError(iDatabase->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}

/**
Gets a custom database property for the given property ID.

@param aPropertyId The ID of the property you wish to get
@param aValue A floating point that will hold the data value
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::GetCustomPropertyL(TUid aPropertyId, TReal& aValue) const
	{
	// The handle must be open and at != NULL, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	User::LeaveIfError(iDatabase->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}

/**
Gets a custom database property for the given property ID.

@param aPropertyId The ID of the property you wish to get
@param aValue A 16 bit descriptor that will hold the data value
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::GetCustomPropertyL(TUid aPropertyId, TDes& aValue) const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	User::LeaveIfError(iDatabase->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}

/**
Gets a custom database property for the given property ID.

@param aPropertyId The ID of the property you wish to get
@param aValue An 8 bit descriptor that will hold the data value
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::GetCustomPropertyL(TUid aPropertyId, TDes8& aValue) const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	User::LeaveIfError(iDatabase->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}

/**
Assigns a new custom database property value to the bookmark item.

@param aPropertyId The ID of the property you wish to set
@param aValue An integer data value
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetCustomPropertyL(TUid aPropertyId, TInt aValue)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iDatabase->SetDirty();
	}

/**
Assigns a new custom database property value to the bookmark item.

@param aPropertyId The ID of the property you wish to set
@param aValue A floating point data value
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetCustomPropertyL(TUid aPropertyId, TReal aValue) __SOFTFP
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iDatabase->SetDirty();
	}

/**
Assigns a new custom database property value to the bookmark item.

@param aPropertyId The ID of the property you wish to set
@param aValue A 16 bit descriptor
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetCustomPropertyL(TUid aPropertyId, const TDesC& aValue)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iDatabase->SetDirty();
	}

/**
Assigns a new custom database property value to the bookmark item.

@param aPropertyId The ID of the property you wish to set
@param aValue An 8 bit descriptor
@publishedPartner
@released
*/
EXPORT_C void RBkDatabase::SetCustomPropertyL(TUid aPropertyId, const TDesC8& aValue)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iDatabase, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	iDatabase->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iDatabase->SetDirty();
	}
