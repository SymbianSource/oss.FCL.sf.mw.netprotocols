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
#include "bkmrk.h"
#include "bkmrkfolder.h"

/**
RBkNode constructor
@publishedPartner
@released
*/
EXPORT_C RBkNode::RBkNode() : iItem(NULL)
	{
	}

/**
Closes the RBkNode handle and decrements the reference count
@publishedPartner
@released
*/
EXPORT_C void RBkNode::Close()
	{
	if (iItem)
		{
		iItem->DecRefCount();
		}
	iItem = NULL;
	}

/**
Returns the type of the object that the RBkNode handle is connected to

@return The type of the object connected to
@publishedPartner
@released
*/
EXPORT_C Bookmark::TType RBkNode::Type() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iItem->Type();
	}

/**
Method for querying the bookmark item's database id.

@return The item's ID.
@publishedPartner
@released
*/
EXPORT_C Bookmark::TItemId RBkNode::Id() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iItem->Id();
	}

/**
Method for querying a bookmark item's parent folder. Only the root will not have
a parent. The method will leave if an attempt is made to call this on the root folder

@return The opened RBkFolder object.
@leave KErrNotFound This will leave if called on the root folder (the root folder has no parent).
@publishedPartner
@released
*/
EXPORT_C RBkFolder RBkNode::OpenParentL() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	CBookmarkFolder* parent = iItem->Parent();
	if (!parent)
		{
		User::Leave(KErrNotFound);
		}
	return parent->OpenFolder();
	}
	
/**
Opens RBkBookmark handle from this RBkNode handle. This is usually done when iterating through a folder. 
You must ensure that this RBkNode handle is of the type: Bookmark::ETypeBookmark before calling this method. 

This method does NOT close the RBkNode handle

@return The opened RBkBookmark object
@leave	Bookmark::KErrWrongType The method will leave if an attempt is made to call this with a RBkNode handle which is not of
the Bookmark::ETypeBookmark type.
@publishedPartner
@released
*/
EXPORT_C RBkBookmark RBkNode::OpenBookmarkL() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	if (Type() != Bookmark::ETypeBookmark)
		{
		// can't create a bookmark from a folder item
		User::Leave(Bookmark::KErrWrongType);
		}
		
	RBkBookmark handle;
	handle.SetItem(*iItem);
	return handle;
	}

/**
Opens RBkFolder handle from this RBkNode handle. This is usually done when iterating through a folder. 
You must ensure that this RBkNode handle is of the type: Bookmark::ETypeFolder before calling this method. 

This method does NOT close the RBkNode handle

@return The opened RBkFolder object.
@leave	Bookmark::KErrWrongType The method will leave if an attempt is made to call this with a RBkNode handle which is not of
the Bookmark::ETypeFolder type.
@publishedPartner
@released
*/
EXPORT_C RBkFolder RBkNode::OpenFolderL() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	if (Type() != Bookmark::ETypeFolder)
		{
		// can't create a bookmark from a folder item
		User::Leave(Bookmark::KErrWrongType);
		}
		
	RBkFolder handle;
	handle.SetItem(*iItem);
	return handle;
	}

/**
Moves the item to another folder

This function will leave if called on the root folder or an out of memory condition
occurs. 

@param aNewParent The new parent folder
@leave Bookmark::KErrReadOnly This function will leave if the item is read only.
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetParentL(RBkFolder& aNewParent)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	LeaveIfRootL();
	// Push the bookmark internal item into the cleanupstack using TCleanupItem.
	// else the iItem will not get deleted in case of a leave from AppendL function.
	CleanupStack::PushL ( TCleanupItem ( RBkNode::DeleteBkmrkItem, iItem ) );
	aNewParent.Folder()->AppendL(*iItem);
	CleanupStack::Pop ();
	}

/**
Gets the items title text.

@return A reference to the bookmark's title text
@publishedPartner
@released
*/
EXPORT_C const TDesC& RBkNode::Title() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	return iItem->Title();
	}
	
/**
Sets the item's title text. Folder titles must be unique.

@param aTitle The item's new title.
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetTitleL(const TDesC& aTitle)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));

	LeaveIfRootL();
	iItem->SetTitleL(aTitle);
	}

/**
Gets the item description text.

@return Reference to the bookmark's description text
@publishedPartner
@released
*/
EXPORT_C const TDesC& RBkNode::DescriptionL() const
	{
	CBkmrkProperties* properties = ExtendedPropertiesL();
	return properties->Description();
	}
	
/**
Sets the item's decription text.

@param aDescription The item's new description
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetDescriptionL(const TDesC& aDescription)
	{
	LeaveIfRootL();
	CBkmrkProperties* properties = ExtendedPropertiesL();
	properties->SetDescriptionL(aDescription);
	}

/**
Returns whether the item is public or private

@return ETrue if the bookmark or folder is public, otherwise EFalse
@publishedPartner
@released
*/
EXPORT_C TBool RBkNode::IsPublic() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));
	return iItem->IsPublic();
	}
	
/**
Sets the public or private status.

@param aPublic Whether the item is to be public or private
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetPublicL(TBool aPublic)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));
	LeaveIfRootL();
	iItem->SetPublicL(aPublic);
	}

/**
Returns whether the item is read only or not

@return ETrue if the bookmark or folder can be written to, otherwise EFalse
@publishedPartner
@released
*/
EXPORT_C TBool RBkNode::IsWritable() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));
	return iItem->IsWritable();;
	}
	
/**
Sets item to Read Only. An item can always be made Read Only, but the bookmark database must be opened as
Bookmark::EVisibilityManager in order to set an item back to Read/Write.
This function will leave if called on the root folder.

@see Bookmark::TVisibility
@param aWritable Whether the item is to be read only or not
@leave KErrPermissionDenied This function will leave, when the bookmark database is not opened in 
Bookmark::EVisibilityManager.
@leave Bookmark::KErrOperationDenied This function will leave, if called on the root folder.
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetWritableL(TBool aWritable)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));
	
	LeaveIfRootL();
	iItem->SetWritableL(aWritable);
	}

/**
Returns the ID if an icon is assigned to this bookmark item.

@return The ID of the icon or KNullBookmarkID if no item has been assigned.
@publishedPartner
@released
*/
EXPORT_C Bookmark::TAttachmentId RBkNode::IconIdL() const
	{
	CBkmrkProperties* properties = ExtendedPropertiesL();
	return properties->IconId();
	}
	
/**
Sets the icon. Call this with KNullIconID to remove the icon. Icons are stored centrally in
the database and are shared by bookmark items. If the icon oes not exist in the database this
method will leave

@param aIconId The ID of the icon.
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetIconIdL(Bookmark::TAttachmentId aIconId)
	{
	LeaveIfRootL();
	CBkmrkProperties* properties = ExtendedPropertiesL();
	properties->SetIconIdL(aIconId);
	}

/**
Gets a custom property for the given property ID. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to get
@param aValue An integer the data value
@publishedPartner
@released
*/
EXPORT_C void RBkNode::GetCustomPropertyL(TUid aPropertyId, TInt& aValue) const
	{
	User::LeaveIfError(ExtendedPropertiesL()->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}
	
/**
Gets a custom property for the given property ID. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to get
@param aValue A floating point data value
@publishedPartner
@released
*/
EXPORT_C void RBkNode::GetCustomPropertyL(TUid aPropertyId, TReal& aValue) const
	{
	User::LeaveIfError(ExtendedPropertiesL()->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}
	
/**
Gets a custom property for the given property ID. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to get
@param aValue A 16 bit descriptor
@publishedPartner
@released
*/
EXPORT_C void RBkNode::GetCustomPropertyL(TUid aPropertyId, TDes& aValue) const
	{
	User::LeaveIfError(ExtendedPropertiesL()->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}
	
/**
Gets a custom property for the given property ID. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to get
@param aValue An 8 bit descriptor
@publishedPartner
@released
*/
EXPORT_C void RBkNode::GetCustomPropertyL(TUid aPropertyId, TDes8& aValue) const
	{
	User::LeaveIfError(ExtendedPropertiesL()->CustomProperties().GetCustomProperty(aPropertyId, aValue));
	}
	
/**
Assigns a new custom property value to the bookmark item. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to set
@param aValue An integer data value
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetCustomPropertyL(TUid aPropertyId, TInt aValue)
	{
	LeaveIfRootL();
	ExtendedPropertiesL()->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iItem->SetDirty();
	}

/**
Assigns a new custom property value to the bookmark item. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to set
@param aValue A floating point data value
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetCustomPropertyL(TUid aPropertyId, TReal aValue) __SOFTFP
	{
	LeaveIfRootL();
	ExtendedPropertiesL()->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iItem->SetDirty();
	}

/**
Assigns a new custom property value to the bookmark item. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to set
@param aValue A 16 bit descriptor
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetCustomPropertyL(TUid aPropertyId, const TDesC& aValue)
	{
	LeaveIfRootL();
	ExtendedPropertiesL()->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iItem->SetDirty();
	}

/**
Assigns a new custom property value to the bookmark item. This method will leave
if no such property has been registered on the database

@param aPropertyId The ID of the property you wish to set
@param aValue An 8 bit descriptor
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetCustomPropertyL(TUid aPropertyId, const TDesC8& aValue)
	{
	LeaveIfRootL();
	ExtendedPropertiesL()->CustomProperties().SetCustomPropertyL(aPropertyId, aValue);
	iItem->SetDirty();
	}

/**
Returns the item's owner if it is a private bookmark.  

@see Bookmark::TVisibility
@return The item's owner security ID
@leave KErrPermissionDenied This function will leave when the bookmark database 
is not opened as Bookmark::EVisibilityManager.
@publishedPartner
@released
*/
EXPORT_C TSecureId RBkNode::OwnerL() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));
	iItem->LeaveIfNotInManagerModeL ();
	TSecureId id;
	iItem->GetOwnerL(id);
	return id;
	}
	
/**
Sets the owner security ID of this bookmark item. All applications which have opened the bookmark database as 
Bookmark::EVisibilityManager can use this function. In other visibility modes the aOwner must be the current SID, 
otherwise function will leave with KErrPermissionDenied

@param aOwner The new owner ID.
@leave KErrPermissionDenied When the aOwner is not the current SID and the bookmark database is opened in
non-manager mode.
@leave Bookmark::KErrOperationDenied This function will leave, if called on the root folder.
@publishedPartner
@released
*/
EXPORT_C void RBkNode::SetOwnerL(TSecureId aOwner)
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));
	LeaveIfRootL();
	iItem->SetOwnerL(aOwner);
	}

/**
Not intended for external use.

@internalComponent
*/
CBookmarkBase* RBkNode::Item() const
	{
	return iItem;
	}

/**
Not intended for external use.

@internalComponent
*/
void RBkNode::SetItem(CBookmarkBase& aItem)
	{
	// Must not try to assign an already open item
	__ASSERT_ALWAYS(!iItem, User::Panic(Bookmark::KBookmarkErrCorrupt, Bookmark::KErrCorrupt));

	iItem = &aItem;
	iItem->IncRefCount();
	}

/**
Not intended for external use.

@internalComponent
*/
CBkmrkProperties* RBkNode::ExtendedPropertiesL() const
	{
	// The handle must be open and attached to a concrete bookmark object
	__ASSERT_ALWAYS(iItem, User::Panic(Bookmark::KBookmarkErrHandleNotOpen, Bookmark::KErrNotOpen));
	
	CBkmrkProperties* properties = &(iItem->ExtendedPropertiesL());
	return properties;
	}

void RBkNode::LeaveIfRootL() const
	{
	if (Title().Compare(Bookmark::KTxtRootTitle) == 0)
		{
		User::Leave(Bookmark::KErrOperationDenied);
		}
	}

/**
 Sets the last modified time to an arbitrary value. Only applications which have opened the bookmark 
 database as Bookmark::EVisibilityManager can use this function, otherwise it will leave with 
 KErrPermissionDenied error code.

 Note: The last modified time is automatically set to the current time during a commit. This
 function should only be used if the caller needs to set to a different time (for example when 
 synchronising bookmarks).
 
 @param aTime The time to set.
 @leave KErrPermissionDenied This function will leave, when the bookmark database is not opened in 
 Bookmark::EVisibilityManager.
 @publishedPartner
 @released
 */
EXPORT_C void RBkNode::SetLastModifiedL ( const TTime& aTime )
	{	
	iItem->LeaveIfNotInManagerModeL ();
	
	CBkmrkProperties* properties = ExtendedPropertiesL();		
	properties->SetLastModified ( aTime );
	iItem->SetDirty();
	}

/**
 Returns the last modified time. If the last modified time was never set, this will return 
 the value of midnight January 1st 1970 UTC. This can occur when using a version 1.0.0 database.
 
 @return Last modified date value.
 @publishedPartner
 @released
 */
EXPORT_C TTime RBkNode::LastModifiedL () const
	{
	CBkmrkProperties* properties = ExtendedPropertiesL();
	return properties->LastModified();
	}

void RBkNode::DeleteBkmrkItem ( TAny* aPtr )
	{
	CBookmarkBase* bkItem = ( CBookmarkBase* )aPtr;
	if ( bkItem )
		{
		bkItem->DecRefCount ();
		delete bkItem;
		bkItem = NULL;					
		}
	}
