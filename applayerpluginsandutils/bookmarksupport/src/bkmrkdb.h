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
// @internalComponent
// 
//

#ifndef __BKMRKDB_H__
#define __BKMRKDB_H__

#include <centralrepository.h>
#include <bautils.h>
#include <bookmarkdatabase.h>
#include "bkmrkattachment.h"
#include "bkmrkwatcher.h"
#include "propertyreg.h"
#include "propertylist.h"

typedef RArray<CBookmarkBase*>			TBkmrkList;

/**
Internal representation for the bookmark database. The database object creates the connection 
to the repositories and is the entry point for browsing the boomark tree. It is also responsible 
for all database-wide properties.
*/
class CBookmarkDb: public CRepositoryAccessor, public MBkmrkWatcherObserver
	{
public:
	class TProperty
		{
	public:
		TUid iId;
		Bookmark::TPropertyType iDataType;
		};
public:
	static CBookmarkDb* NewL(Bookmark::TVisibility aVisibility, MBookmarkObserver* aObserver);
	~CBookmarkDb();

	TBool IsTreeConstructed();
	void ConstructTreeL();
	
	void RefreshL();

	CBookmarkFolder& RootL();

	CBookmarkBase* FindItem(Bookmark::TItemId aItem);
	CBookmarkFolder* FindFolder(const TDesC& aFolderName);

	CBookmark& CreateBookmarkL(CBookmarkFolder& aParent);
	CBookmarkFolder& CreateFolderL(const TDesC& aFolderName, CBookmarkFolder& aParent);

	void DeleteItemL(Bookmark::TItemId aItem, TBool aRecursive);

	void RemoveBookmarkL(CBookmark* aBookmark);
	void RemoveFolderL(CBookmarkFolder* aFolder);

	const TDesC8& IconL(Bookmark::TAttachmentId aIconId);
	Bookmark::TAttachmentId CreateIconL(const TDesC8& aData);
	TInt DeleteIconL(Bookmark::TAttachmentId aIconId);

	TVersion Version();
	Bookmark::TVisibility Visibility();
	
	void SetHome(CBookmark* aHome);
	CBookmark* Home();

	const TDesC& HomePageText() const;
	void SetHomePageTextL(const TDesC& aHomePageText);

	const TDesC8& SearchUri() const;
	void SetSearchUriL(const TDesC8& aUri);

	TUint32 DefaultProxy() const;
	void SetDefaultProxy(TUint32 aServiceId);

	TUint32 DefaultNap() const;
	void SetDefaultNap(TUint32 aNetworkId);

	CRepository& FolderRepository();
	CRepository& BookmarkRepository();

	TBool IsLoading();
	TBool FindUnloadedFolder(const TDesC& aFolderName);

	// custom properties
	CPropertyReg& PropertyRegister();
	CPropertyList& CustomProperties();

	// From MRepositoryAccessor
	void CommitL();
	TUint32 IndexBase();
	void SetIdFromIndexBase(TUint32 aIndexBase);
	void TransSaveL();
	void TransNewL();
	void TransLoadL();
	void TransRemoveL();

	// From MBkmrkWatcherObserver
	void BWONotify(TBkmrkWatcherType aType);
	
	
	void CreateDefaultRepositoriesL ();	
private:
	CBookmarkDb(MBookmarkObserver* aObserver);
	void ConstructL(Bookmark::TVisibility aVisibility);
	void InitialiseRepL(CRepository& aRepository);
	
	void Reset();
	void LoadBookmarkListL(CRepository& aRepository, TBkmrkList& aList, Bookmark::TType aType, TBool aLoadAll);
	void AddItemsToFolderTreeL(TBkmrkList& aList);
	TBool RemoveFolderIfPublicL(CBookmarkFolder& aFolder);
	void LoadTreeL();
	void ResetTree();
	void ResetIcons();

	void LoadIconsL();

	TBool HasOpenHandles();

	TInt FindIcon(Bookmark::TAttachmentId aIconId);

	TBool CheckAccessL(CBookmarkBase& aItem);
	
	static void ResetAndDestroyBkmrkList ( TAny* aPtr );
	TBool HasDataInRepositoryL ();
private:
	Bookmark::TVisibility	iVisibility;	// the visibility level for the database
	MBookmarkObserver*		iObserver;		// if set, points to an observer that is notified when changes occur to the database

	CPropertyReg* 			iPropertyRegister;		// Register for custom properties
	CPropertyList*			iCustomProperties;

	CRepository*			iDatabaseRepository;	// the database config settings
	CBkmrkWatcher*			iDatabaseWatcher;
	CRepository*			iFolderRepository;		// the folder repository
	CBkmrkWatcher*			iFolderWatcher;
	CRepository*			iBookmarkRepository;	// the bookmark repository
	CBkmrkWatcher*			iBookmarkWatcher;
	CRepository*			iIconRepository;		// the icon repository
	CBkmrkWatcher*			iIconWatcher;

	CBookmarkFolder*		iRoot;			// the root folder
	CBookmark*				iHome;			// the currently set home page
	TInt					iHomeId;
	HBufC*					iHomeText;
	HBufC8*					iSearchUri;
	TInt					iDefaultProxy;
	TInt					iDefaultNap;

	TBool					iLoading;
	TBkmrkList				iFolderList;			// a list of all the folders in ID order
	TBkmrkList				iBookmarkList;			// a list of all the bookmarks in ID order
	TBkmrkAccessorList		iDeletedList;			// a list of all the bookmarks items that have been deleted
	CDesCArrayFlat* 		iUnloadedFolderList;	// a list of folder names not loaded but that need to be checked for uniqueness


	TBool						iTreeSync;
	TBool						iConfigSync;
	TBool						iIconSync;
	RArray<CBkmrkAttachment*> 	iIconList;
	
	TBool 						iBkmrkDbInitialized;
	TBool						iTreeConstructed;
	};

#endif //__BKMRKDB_H__
