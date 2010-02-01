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
// Class for opening and manipulating the bookmark database.
// 
//

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __BOOKMARKDATABASE_H__
#define __BOOKMARKDATABASE_H__

#include <bookmarkdef.h>
#include <bookmark.h>

class CBookmarkDb;

/**
Applications can derive from this class in order to receive notifications if the
the database changes. The system shares a single bookmark database so it is possible 
for another application or service to change the database while you are using it.
By registering as an observer, and deriving from MBookmarkObserver, you can be 
notified if such an event occurs. 

Your view of the database will not be automatically updated. You will need to call
RBkDatabase::RefreshL() in order for changes to be updated.

@publishedPartner
@released
*/
class MBookmarkObserver
	{
public:
	/**
	  Bookmark database change notification events.
	 */
	enum TEvent
		{
		/**
			One or more folders or bookmarks have had their properties modified
		 */
		EEventUpdatedBookmarkTree,	
		/**
			One or more database-wide properties has been modified
		 */
		EEventUpdatedConfig,		
		/**
			Icon data has been added or modified
		 */
		EEventUpdatedIcons
		};
public:
	/**
	The call-back function that will be called when the database has changed.
	
	@param aEvent The type of bookmark database change.
	@publishedPartner
	@released
	*/
	virtual void MBONotify(TEvent aEvent) = 0;
	};



/**
RBkDatabase represents a handle to the database as a whole. Opening the handle 
will initiate a connection to the system-wide bookmark store. RBkDatabase is 
responsible for all operations such as creating new bookmarks and folders or changing 
database-wide data such as setting the home page or registering custom properties.

@publishedPartner
@released
*/
class RBkDatabase
	{
public:
	IMPORT_C RBkDatabase();

	IMPORT_C void OpenL(Bookmark::TVisibility aVisibility = Bookmark::EVisibilityDefault, MBookmarkObserver* aObserver = NULL);
	IMPORT_C void Close();

	IMPORT_C void RefreshL();

	IMPORT_C void CommitL();

	IMPORT_C RBkFolder OpenRootL() const;
	
	IMPORT_C RBkBookmark CreateBookmarkL(RBkFolder* aParent = NULL);
	IMPORT_C RBkFolder CreateFolderL(const TDesC& aTitle, RBkFolder* aParent = NULL);

	IMPORT_C void DeleteItemL(Bookmark::TItemId aBookmarkId, TBool aRecursive = EFalse);

	IMPORT_C RBkBookmark OpenBookmarkL(Bookmark::TItemId aBookmarkID) const;
	IMPORT_C RBkFolder OpenFolderL(Bookmark::TItemId aFolderID) const;
	IMPORT_C RBkFolder OpenFolderL(const TDesC& aFolderTitle) const;

	IMPORT_C Bookmark::TAttachmentId CreateIconL(const TDesC8& aIconData);
	IMPORT_C const TDesC8& GetIconL(Bookmark::TAttachmentId aIconId) const;
	IMPORT_C TInt DeleteIconL(Bookmark::TAttachmentId aIconId);

	IMPORT_C TVersion Version() const;

	IMPORT_C RBkBookmark OpenHomeL() const;

	/**
	Method for setting the home page bookmark. 
 
	@param aHome Reference to the new home page
	@publishedPartner
	@deprecated Left in for source compatibility, Use SetHomeL function instead
	*/
	inline void SetHome(const RBkBookmark& aHome)	
		{SetHomeL(aHome);}

	IMPORT_C const TDesC& HomePageText() const;
	IMPORT_C void SetHomePageTextL(const TDesC& aHomePageText);

	IMPORT_C const TDesC8& SearchUri() const;
	IMPORT_C void SetSearchUriL(const TDesC8& aUri);

	IMPORT_C TUint32 DefaultProxy() const;
	IMPORT_C void SetDefaultProxy(TUint32 aServiceId);

	IMPORT_C TUint32 DefaultNap() const;
	IMPORT_C void SetDefaultNap(TUint32 aNetworkId);

	IMPORT_C void RegisterDatabasePropertyL(TUid aCustomId, Bookmark::TPropertyType aDataType);
	IMPORT_C void RegisterFolderPropertyL(TUid aCustomId, Bookmark::TPropertyType aDataType);
	IMPORT_C void RegisterBookmarkPropertyL(TUid aCustomId, Bookmark::TPropertyType aDataType);

	IMPORT_C TInt DeregisterDatabasePropertyL(TUid aCustomId);
	IMPORT_C TInt DeregisterFolderPropertyL(TUid aCustomId);
	IMPORT_C TInt DeregisterBookmarkPropertyL(TUid aCustomId);

	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TInt& aValue) const;
	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TReal& aValue) const;
	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TDes& aValue) const;
	IMPORT_C void GetCustomPropertyL(TUid aPropertyId, TDes8& aValue) const;
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, TInt aValue);
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, TReal aValue);
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, const TDesC& aValue);
	IMPORT_C void SetCustomPropertyL(TUid aPropertyId, const TDesC8& aValue);

	IMPORT_C void SetHomeL(const RBkBookmark& aHome);
private:
	CBookmarkDb* iDatabase; // The internal database object that this handle is connected to
	};

#endif //__BOOKMARKDATABASE_H__