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
// Classes for manipulating bookmarks and bookmark folders.
// 
//

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __BOOKMARK_H__
#define __BOOKMARK_H__

#include <bookmarkdef.h>
#include <cauthentication.h>

class CBookmarkBase;
class CBookmark;
class CBookmarkFolder;
class CBkmrkProperties;
class RBkBookmark;
class RBkFolder;

/**
RBkNode is the base class for RBkBookmark and RBkFolder handle classes.
Bookmarks and folders have many attributes in common. This class encapulates these
elements and provides the implementation for accessing and setting them. It is also
used when iterating through a folders child list where you can't know in advance
whether the item is a bookmark or folder. A RBkNode can be passed as a parameter
when opening RBkBookmark and RBkFolder handles once you know the RBkNode's type
@publishedPartner
@released
*/
class RBkNode
	{
public:
	IMPORT_C RBkNode();
	IMPORT_C void Close();

	IMPORT_C Bookmark::TType Type() const;
	IMPORT_C Bookmark::TItemId Id() const;
	IMPORT_C RBkFolder OpenParentL() const;
	IMPORT_C RBkBookmark OpenBookmarkL() const;
	IMPORT_C RBkFolder OpenFolderL() const;
	IMPORT_C void SetParentL(RBkFolder& aNewParent);

	IMPORT_C const TDesC& Title() const;
	IMPORT_C void SetTitleL(const TDesC& aTitle);

	IMPORT_C const TDesC& DescriptionL() const;
	IMPORT_C void SetDescriptionL(const TDesC& aDescription);

	IMPORT_C TBool IsPublic() const;
	IMPORT_C void SetPublicL(TBool aPublic);

	IMPORT_C TBool IsWritable() const;
	IMPORT_C void SetWritableL(TBool aWritable);

	IMPORT_C TSecureId OwnerL() const;
	IMPORT_C void SetOwnerL(TSecureId aOwner = Bookmark::KNoOwnerId);

	IMPORT_C Bookmark::TAttachmentId IconIdL() const;
	IMPORT_C void SetIconIdL(Bookmark::TAttachmentId aIconId = Bookmark::KNullAttachmentID);

	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TInt& aValue) const;
	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TReal& aValue) const;
	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TDes& aValue) const;
	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TDes8& aValue) const;
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, TInt aValue);
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, TReal aValue) __SOFTFP;
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, const TDesC& aValue);
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, const TDesC8& aValue);
	
	IMPORT_C void SetLastModifiedL ( const TTime& aTime );
	IMPORT_C TTime LastModifiedL () const;
	
	// not exported
	CBookmarkBase* Item() const;
	void SetItem(CBookmarkBase& aItem);
	CBkmrkProperties* ExtendedPropertiesL() const;

private:
	void LeaveIfRootL() const;
	
	static void DeleteBkmrkItem ( TAny* aPtr );
protected:
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
	/**
	@internalComponent
	*/
#endif//SYMBIAN_ENABLE_SPLIT_HEADERS
	CBookmarkBase* iItem; // The internal bookmark item that this handle is connected to
	};


/**
The bookmark tree maintained by the database is made up to folders containing bookmarks.
The RBkBookmark class is a handle to a bookmark in the database.
The RBkDatabase handle will return an RBkBookmark handle to you when you
create a new bookmark or on a call to OpenBookmarkL(). As with all items in the database,
the client application never owns the bookmark item itself. All actions are performed
through this handle.
@publishedPartner
@released
*/
class RBkBookmark: public RBkNode
	{
public:
	IMPORT_C RBkBookmark();

	IMPORT_C TBool IsHomePage();
	IMPORT_C TTime LastVisited() const;
	IMPORT_C void SetLastVisitedL(TTime aTime);
	IMPORT_C void UpdateVisited();

	IMPORT_C const CAuthentication& AuthenticationL();
	IMPORT_C void SetAuthenticationL(const CAuthentication& aAuthentication);

	IMPORT_C const TDesC8& Uri() const;
	IMPORT_C void SetUriL(const TDesC8& aUri);

	IMPORT_C TUint32 ProxyL() const;
	IMPORT_C void SetProxyL(TUint32 aProxy);

	IMPORT_C TUint32 GetNapL() const;
	IMPORT_C void SetNapL(TUint32 aNap);

	// not exported
	CBookmark* Bookmark() const;
	};


/**
The bookmark tree maintained by the database is made up to folders containing bookmarks.
The RBkFolder class is a handle to a bookmark folder. Bookmark folders contain a list
of children which can be bookmarks or other folders. As with all items in the database,
the client application never owns the folder item itself. All actions are performed
through this handle.
@publishedPartner
@released
*/
class RBkFolder: public RBkNode
	{
public:
	IMPORT_C RBkFolder();

	IMPORT_C void AppendL(RBkNode& aItem);
	IMPORT_C void InsertL(RBkNode& aItem, TInt aIndexPosition);
	IMPORT_C TInt Move(TInt aOldPosition, TInt aNewPosition);
	IMPORT_C TInt Count() const;
	IMPORT_C TInt Index(const RBkNode& aItem) const;
	IMPORT_C RBkNode OpenItemL(TInt aIndex);

	// not exported
	CBookmarkFolder* Folder() const;
	};

#endif //__BOOKMARK_H__
