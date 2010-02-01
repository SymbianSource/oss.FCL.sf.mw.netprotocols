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
#include "testutils.h"

_LIT(KFilename, "tbookmarktests.cpp");

_LIT(KTxtFolder1, 		"Folder 1");
_LIT(KTxtFolder1_1, 	"Folder 1.1");
_LIT(KTxtFolder1_2, 	"Folder 1.2");
_LIT(KTxtFolder1_2_1, 	"Folder 1.2.1");
_LIT(KTxtFolder1_2_1_1,	"Folder 1.2.1.1");

_LIT(KTxtBookmark1,		"Bookmark 1");
_LIT(KTxtBookmark2,		"Bookmark 2");
_LIT(KTxtBookmark3, 	"Bookmark 3");
_LIT(KTxtBookmark4, 	"Bookmark 4");
_LIT(KTxtBookmark5, 	"Bookmark 5");
_LIT(KTxtBookmark6, 	"Bookmark 6");
_LIT(KTxtBookmark7, 	"Bookmark 7");
_LIT(KTxtBookmark8, 	"Bookmark 8");
_LIT(KTxtBookmark9, 	"Bookmark 9");
_LIT(KTxtBookmark10, 	"Bookmark 10");

_LIT8(KIconData,		"****4644372131**7*&879");

LOCAL_D CTestWrapper* gTestWrapper;
LOCAL_D	RBkDatabase gDatabase;

LOCAL_D void CreateBookmarkTreeL()
	{
	Bookmark::TAttachmentId iconId = gDatabase.CreateIconL(KIconData);
	
	// Create the first two bookmarks in the root
	RBkBookmark bmrk = gDatabase.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark1);
	bmrk.SetIconIdL(iconId);
	CleanupStack::PopAndDestroy(&bmrk);
	
	bmrk = gDatabase.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark2);
	
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create a folder and put 2 bookmarks into it
	RBkFolder folder = gDatabase.CreateFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
	bmrk = gDatabase.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark3);
	CleanupStack::PopAndDestroy(&bmrk);
	bmrk = gDatabase.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark4);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create two more folders in this folder and place a bookmark in each one
	RBkFolder folderL2 = gDatabase.CreateFolderL(KTxtFolder1_1, &folder);
	CleanupClosePushL(folderL2);
	bmrk = gDatabase.CreateBookmarkL(&folderL2);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark5);
	CleanupStack::PopAndDestroy(&bmrk);
	CleanupStack::PopAndDestroy(&folderL2);
	
	folderL2 = gDatabase.CreateFolderL(KTxtFolder1_2, &folder);
	CleanupClosePushL(folderL2);
	bmrk = gDatabase.CreateBookmarkL(&folderL2);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark6);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create a folder under 1_2 and add a bookmark
	RBkFolder folderL3 = gDatabase.CreateFolderL(KTxtFolder1_2_1, &folderL2);
	CleanupClosePushL(folderL3);
	bmrk = gDatabase.CreateBookmarkL(&folderL3);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark7);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create a folder under 1_2_1 and add 3 bookmarks
	RBkFolder folderL4 = gDatabase.CreateFolderL(KTxtFolder1_2_1_1, &folderL3);
	CleanupClosePushL(folderL4);
	bmrk = gDatabase.CreateBookmarkL(&folderL4);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark8);
	CleanupStack::PopAndDestroy(&bmrk);
	bmrk = gDatabase.CreateBookmarkL(&folderL4);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark9);
	CleanupStack::PopAndDestroy(&bmrk);
	bmrk = gDatabase.CreateBookmarkL(&folderL4);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark10);
	CleanupStack::PopAndDestroy(&bmrk);
	
	
	CleanupStack::PopAndDestroy(&folderL4);
	CleanupStack::PopAndDestroy(&folderL3);
	CleanupStack::PopAndDestroy(&folderL2);
	CleanupStack::PopAndDestroy(&folder);
	}

void CreateSomeBookmarksL()
	{
	RBkDatabase database;
	database.OpenL();
	CleanupClosePushL(gDatabase);
	
	// Create the first two bookmarks in the root
	RBkBookmark bmrk = database.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark1);
	CleanupStack::PopAndDestroy(&bmrk);
	
	bmrk = database.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark2);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create a folder
	RBkFolder folder = database.CreateFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
	
	// Put 2 bookmarks into the new folder
	bmrk = database.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark3);
	CleanupStack::PopAndDestroy(&bmrk);
	bmrk = database.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark4);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Close the folder
	CleanupStack::PopAndDestroy(&folder);
	
	// Commit the changes to the database and close it
	database.CommitL();
	CleanupStack::PopAndDestroy(&database);
	}

_LIT8(KFolderIconData,		"****4644372131**7*&879");
_LIT8(KBookmarkIconData,		"****46=-=-=4372131**7*&879");
_LIT(KTxtFolderDesc, "This is the folder description");
_LIT(KTxtBookmarkDesc, "This is the bookmark description");
_LIT8(KBookmarkUri, "www.ft.com");
_LIT8(KSearchUri, "www.google.com");
_LIT(KHomePageDesc, "This is the home page description");

void SettingStandardPropertiesL()
	{
	// Open the database
	RBkDatabase database;
	database.OpenL();
	CleanupClosePushL(database);

	// Create some icons	
	Bookmark::TAttachmentId folderIconId = database.CreateIconL(KFolderIconData);
	Bookmark::TAttachmentId bookmarkIconId = database.CreateIconL(KBookmarkIconData);

	// Create a folder
	RBkFolder folder = database.CreateFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
	// Set some folder properties
	folder.SetDescriptionL(KTxtFolderDesc);
	folder.SetIconIdL(folderIconId);
	
	// Create a bookmark in the new folder
	RBkBookmark bmrk = database.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
	// Set some bookmark properties
	bmrk.SetTitleL(KTxtBookmark1);
	bmrk.SetDescriptionL(KTxtBookmarkDesc);
	bmrk.SetUriL(KBookmarkUri);
	bmrk.SetIconIdL(bookmarkIconId);
	
	// Set some database properties
	database.SetHomePageTextL(KHomePageDesc);
	database.SetHome(bmrk);
	database.SetSearchUriL(KSearchUri);
	
	// Close the bookmark
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Close the folder
	CleanupStack::PopAndDestroy(&folder);
	
	// Commit the changes to the database and close it
	database.CommitL();
	CleanupStack::PopAndDestroy(&database);
	}
	
void CreatingAnIconL()
	{
	// Open the database
	RBkDatabase database;
	database.OpenL();
	CleanupClosePushL(database);

	// Create an icon	
	Bookmark::TAttachmentId bookmarkIconId = database.CreateIconL(KBookmarkIconData);

	// Create a bookmark in the root
	RBkBookmark bmrk = database.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	// Set some bookmark properties
	bmrk.SetIconIdL(bookmarkIconId);
	// Close the bookmark
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Commit the changes to the database and close it
	database.CommitL();
	CleanupStack::PopAndDestroy(&database);
	}
	
void CreateBookmarkWithCustomPropertiesL()
	{
	TUid des16PropertyUid = {0x00001234};
	TUid des8PropertyUid = {0x00001235};
	TUid intPropertyUid = {0x00001236};
	TUid realPropertyUid = {0x00001237};
	
	_LIT(KTxtFirst16EditorName, "Joe Bloggs");
	_LIT(KTxtSecond16EditorName, "Joe Bloggs II");
	
	_LIT8(KTxtFirst8ReferrerName, "Krishna");
	_LIT8(KTxtSecond8RefererName, "Ganesh");
		
	// Register a custom propery
	gDatabase.RegisterBookmarkPropertyL(des16PropertyUid, Bookmark::EDataTypeDes16);
	
	// Register a custom propery
	gDatabase.RegisterBookmarkPropertyL(des8PropertyUid, Bookmark::EDataTypeDes8);
	
	// Register a custom propery
	gDatabase.RegisterBookmarkPropertyL(intPropertyUid, Bookmark::EDataTypeInt);
	
	// Register a custom propery
	gDatabase.RegisterBookmarkPropertyL(realPropertyUid, Bookmark::EDataTypeReal);
	
	// Create a bookmark in the root
	RBkBookmark bmrk = gDatabase.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark1);
	
	//Added twice to check for memory leak described in INC067424
	
	// Set the custom property value
	bmrk.SetCustomPropertyL(des16PropertyUid, KTxtFirst16EditorName);
	bmrk.SetCustomPropertyL(des16PropertyUid, KTxtSecond16EditorName);
	
	// Set the custom property value
	bmrk.SetCustomPropertyL(des8PropertyUid, KTxtFirst8ReferrerName);
	bmrk.SetCustomPropertyL(des8PropertyUid, KTxtSecond8RefererName);
	
	
	// Set the custom property value
	bmrk.SetCustomPropertyL(intPropertyUid, 1);
	bmrk.SetCustomPropertyL(intPropertyUid, 2);
	
	// Set the custom property value
	bmrk.SetCustomPropertyL(realPropertyUid, 3.14159);
	bmrk.SetCustomPropertyL(realPropertyUid, 3.14159);
	
	
	CleanupStack::PopAndDestroy(&bmrk);
	
	}

Bookmark::TItemId FindBookmarkIdL(const TDesC& aName, RBkFolder aFolder)	
	{
	RBkNode item;
	Bookmark::TItemId id = Bookmark::KNullItemID;
	for (TInt index = aFolder.Count() - 1; index >= 0; --index)
		{
		// open the child item at this index value
		item = aFolder.OpenItemL(index);
		CleanupClosePushL(item);
		// check the title
		const TDesC& title = item.Title();
		if (item.Type() == Bookmark::ETypeBookmark && title.Compare(aName) == 0)
			{
			id = item.Id();
			}
		else if (item.Type() == Bookmark::ETypeFolder)
			{
			// if the item is a folder, open as a folder and call FindBookmarkL to
			// check its children
			RBkFolder folder = item.OpenFolderL();
			CleanupClosePushL(folder);
			id = FindBookmarkIdL(aName, folder);
			// close the folder
			CleanupStack::PopAndDestroy(&folder);
			}
		// close the child item
		CleanupStack::PopAndDestroy(&item);
		if (id != Bookmark::KNullItemID)
			{
			// the bookmark has been found so no need to look at any more items
			break;
			}
		}
	return id;
	}	

	
LOCAL_D TBool IsItemInFolderL(const TDesC& aName, RBkFolder aFolder)	
	{
	RBkNode item;
	TBool found = EFalse;
	TInt index = aFolder.Count() - 1;
	for (;index >= 0; --index)
		{
		item = aFolder.OpenItemL(index);
		CleanupClosePushL(item);
		const TDesC& title = item.Title();
		if (title.Compare(aName) == 0)
			{
			found = ETrue;
			}
		CleanupStack::PopAndDestroy(&item);
		if (found)
			{
			break;
			}
		}
	return found;
	}
	
LOCAL_D void CreateBookmarksL()	
	{
	gTestWrapper->Next(_L("[Bookmark tests] create bookmarks"));
	
	CreateBookmarkTreeL();
	
	// Check there are 3 items in the root (2 bookmarks and 1 folder)
	RBkFolder folder = gDatabase.OpenRootL();
	CleanupClosePushL(folder);
	gTestWrapper->TEST(folder.Count() == 3);
	// Check the titles are correct
	gTestWrapper->TEST(IsItemInFolderL(KTxtFolder1, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark1, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark2, folder));
	CleanupStack::PopAndDestroy(&folder);
	
	// Open folder 1 and check there are 4 items (2 bookmarks and 2 folders)
	folder = gDatabase.OpenFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
	gTestWrapper->TEST(folder.Count() == 4);
	// Check the titles are correct
	gTestWrapper->TEST(IsItemInFolderL(KTxtFolder1_1, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtFolder1_2, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark3, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark4, folder));
	CleanupStack::PopAndDestroy(&folder);

	// Open folder 1.1 and check there is 1 item
	folder = gDatabase.OpenFolderL(KTxtFolder1_1);
	CleanupClosePushL(folder);
	gTestWrapper->TEST(folder.Count() == 1);
	// Check the title is correct
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark5, folder));
	CleanupStack::PopAndDestroy(&folder);

	// Open folder 1.2 and check there are 2 items (1 bookmark and 1 folder)
	folder = gDatabase.OpenFolderL(KTxtFolder1_2);
	CleanupClosePushL(folder);
	gTestWrapper->TEST(folder.Count() == 2);
	// Check the titles are correct
	gTestWrapper->TEST(IsItemInFolderL(KTxtFolder1_2_1, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark6, folder));
	CleanupStack::PopAndDestroy(&folder);
	
	// Open folder 1.2.1 and check there are 2 items (1 bookmark and 1 folder)
	folder = gDatabase.OpenFolderL(KTxtFolder1_2_1);
	CleanupClosePushL(folder);
	gTestWrapper->TEST(folder.Count() == 2);
	// Check the titles are correct
	gTestWrapper->TEST(IsItemInFolderL(KTxtFolder1_2_1_1, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark7, folder));
	CleanupStack::PopAndDestroy(&folder);
	
	// Open folder 1.2.1.1 and check there are 3 items (3 bookmarks)
	folder = gDatabase.OpenFolderL(KTxtFolder1_2_1_1);
	CleanupClosePushL(folder);
	gTestWrapper->TEST(folder.Count() == 3);
	// Check the titles are correct
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark8, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark9, folder));
	gTestWrapper->TEST(IsItemInFolderL(KTxtBookmark10, folder));
	CleanupStack::PopAndDestroy(&folder);
	}

LOCAL_D void CreateDatabaseL()
	{
	gDatabase.OpenL();
	CleanupClosePushL(gDatabase);
	CreateBookmarkTreeL();
	CleanupStack::PopAndDestroy(&gDatabase);
	}

LOCAL_D void OOMTestL()
	{
	gTestWrapper->Next(_L("[Bookmark tests] OOM testing"));
	TInt ret = KErrNoMemory;
	TInt failAt = 0;
	while(ret != KErrNone)
		{
		failAt++;
		gTestWrapper->Tester().Printf(_L("OOM test step: %d\n"),failAt);

		__UHEAP_SETFAIL(RHeap::EDeterministic, failAt);
		__UHEAP_MARK;
		TRAP(ret, CreateDatabaseL());
		__UHEAP_MARKEND;
		__UHEAP_RESET;
		
		gTestWrapper->TESTE(ret == KErrNoMemory || ret == KErrNone, ret);
		}
	}
	
void DoBookmarkTestsL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** Bookmark tests ***"));
	
	gDatabase.OpenL();
	CleanupClosePushL(gDatabase);

	CreateBookmarksL();
    CreateBookmarkWithCustomPropertiesL();
    
	CleanupStack::PopAndDestroy(&gDatabase);
	
	OOMTestL();
	
	
	}
