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

_LIT(KFilename, "tfoldertests.cpp");

_LIT(KTxtSubRootName, "Test sub-root");
_LIT(KTxtFolder2_1, "Folder 2.1");
_LIT(KTxtFolder2_2, "Folder 2.2");
_LIT(KTxtFolder2_3, "Folder 2.3");
_LIT(KTxtFolder2_2_1, "Folder 2.2.1");

const TUid KUid = {0x111};
_LIT8(KData8Bit, "8BitData");
_LIT(KData16Bit, "16BitData");
const TInt KInt = 999;
const TReal KReal = 42.999;

const TInt KGeneralDelay = 1000 * 1000;

_LIT ( KMyFolder, "MyFolder" );
_LIT ( KMyFolderDesc, "Testing last modified time in bookmark." );
_LIT ( KMyFolderNewDesc, "1: Testing last modified time in bookmark." );

LOCAL_D CTestWrapper* gTestWrapper;
LOCAL_D	RBkDatabase gDatabase;

LOCAL_C void CreateFolderL()
	{
	gTestWrapper->Next(_L("[Folders tests] unique folder names"));
	RBkFolder testSubRoot;
	TRAPD(err, testSubRoot = gDatabase.OpenFolderL(KTxtSubRootName));
	gTestWrapper->TEST(err == KErrNotFound);

	testSubRoot = gDatabase.CreateFolderL(KTxtSubRootName);
	testSubRoot.Close();
	testSubRoot = gDatabase.OpenFolderL(KTxtSubRootName);
	Bookmark::TItemId subRootId = testSubRoot.Id();
	testSubRoot.Close();
	
	gDatabase.CommitL();
	
	RBkFolder duplicate;
	TRAP(err, duplicate = gDatabase.CreateFolderL(KTxtSubRootName));
	gTestWrapper->TESTE(err == Bookmark::KErrTitleAlreadyUsed, err);
	
	gDatabase.DeleteItemL(subRootId, ETrue);

	TRAP(err, testSubRoot = gDatabase.OpenFolderL(KTxtSubRootName));
	gTestWrapper->TEST(err == KErrNotFound);

	RBkFolder root = gDatabase.OpenRootL();
	CleanupClosePushL(root);
	gTestWrapper->TEST(root.Count() == 0);
	CleanupStack::PopAndDestroy(&root);
	
	gDatabase.CommitL();
	}

LOCAL_C void CreateManyFoldersL()
	{
	gTestWrapper->Next(_L("[Folders tests] creating levels of folders"));
	// Create a sub-root folder
	RBkFolder subroot;
	subroot = gDatabase.CreateFolderL(KTxtSubRootName);
	CleanupClosePushL(subroot);

	// Create three folders in the sub-root
	RBkFolder folderL2;
	folderL2 = gDatabase.CreateFolderL(KTxtFolder2_1, &subroot);
	folderL2.Close();

	folderL2 = gDatabase.CreateFolderL(KTxtFolder2_2, &subroot);
	CleanupClosePushL(folderL2);
	// Create a folder in folder 2.2
	RBkFolder folderL3;
	folderL3 = gDatabase.CreateFolderL(KTxtFolder2_2_1, &folderL2);
	folderL3.Close();
	CleanupStack::PopAndDestroy(&folderL2);

	folderL2 = gDatabase.CreateFolderL(KTxtFolder2_3, &subroot);
	folderL2.Close();

	CleanupStack::PopAndDestroy(&subroot);

	// Check that there are three folders in the sub-root
	RBkFolder folder = gDatabase.OpenFolderL(KTxtSubRootName);
	gTestWrapper->TEST(folder.Count() == 3);
	folder.Close();

	// Check that there is a folder in the folder 2.2
	folder = gDatabase.OpenFolderL(KTxtFolder2_2);
	gTestWrapper->TEST(folder.Count() == 1);
	Bookmark::TItemId id = folder.Id();
	folder.Close();

	// Delete folder 2.2. The default action is dump any children to the root folder
	gDatabase.DeleteItemL(id);

	// Check that folder 2.2 can not be found
	TRAPD(error, folder = gDatabase.OpenFolderL(KTxtFolder2_2));
	gTestWrapper->TEST(error == KErrNotFound);

	// There should now be 2 folders in the sub-root
	folder = gDatabase.OpenFolderL(KTxtSubRootName);
	gTestWrapper->TEST(folder.Count() == 2);
	folder.Close();

	// The folder 2.2.1 should now be in the root
	// Check it exists and check that it's parent is the root
	folder = gDatabase.OpenFolderL(KTxtFolder2_2_1);
	CleanupClosePushL(folder);
	subroot = folder.OpenParentL();
	CleanupClosePushL(subroot);
	gTestWrapper->TEST(subroot.Title().Compare(Bookmark::KTxtRootTitle) == 0);
	CleanupStack::PopAndDestroy(2);

	// Get the sub-root and delete it with all it's children
	subroot = gDatabase.OpenFolderL(KTxtSubRootName);
	id = subroot.Id();
	subroot.Close();
	gDatabase.DeleteItemL(id, ETrue);

	// There should now be only one item in the root
	// delete it
	folder = gDatabase.OpenRootL();
	gTestWrapper->TEST(folder.Count() == 1);
	folder.Close();
	folder = gDatabase.OpenFolderL(KTxtFolder2_2_1);
	id = folder.Id();
	folder.Close();
	gDatabase.DeleteItemL(id, ETrue);
	}


LOCAL_D void MoveBookmarksTest()
	{
	RBkFolder root;
	root = gDatabase.OpenRootL();
	CleanupClosePushL(root);
	gTestWrapper->TEST(root.Count() == 0);

	RBkFolder folder1 = gDatabase.CreateFolderL(KTxtFolder2_1);
	CleanupClosePushL(folder1);
	RBkFolder folder2 = gDatabase.CreateFolderL(KTxtFolder2_2);
	CleanupClosePushL(folder2);

	RBkBookmark bookmark = gDatabase.CreateBookmarkL(&folder1);
	CleanupClosePushL(bookmark);
	Bookmark::TItemId id1 = bookmark.Id();
	CleanupStack::PopAndDestroy(&bookmark);

	bookmark = gDatabase.CreateBookmarkL(&folder1);
	CleanupClosePushL(bookmark);
	Bookmark::TItemId id2 = bookmark.Id();
	CleanupStack::PopAndDestroy(&bookmark);

	bookmark = gDatabase.CreateBookmarkL(&folder2);
	CleanupClosePushL(bookmark);
	Bookmark::TItemId id3 = bookmark.Id();
	CleanupStack::PopAndDestroy(&bookmark);

	bookmark = gDatabase.CreateBookmarkL(&folder2);
	CleanupClosePushL(bookmark);
	Bookmark::TItemId id4 = bookmark.Id();
	CleanupStack::PopAndDestroy(&bookmark);

	gTestWrapper->TEST(folder1.Count() == 2);
	gTestWrapper->TEST(folder2.Count() == 2);

	RBkNode item = folder1.OpenItemL(0);
	CleanupClosePushL(item);
	folder2.AppendL(item);
	CleanupStack::PopAndDestroy(&item);

	gTestWrapper->TEST(folder1.Count() == 1);
	gTestWrapper->TEST(folder2.Count() == 3);

	folder2.Move(0,2);

	item = folder2.OpenItemL(0);
	CleanupClosePushL(item);
	folder1.InsertL(item, 0);
	CleanupStack::PopAndDestroy(&item);

	gTestWrapper->TEST(folder1.Count() == 2);
	gTestWrapper->TEST(folder2.Count() == 2);

	item = folder1.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Id() == id4);
	CleanupStack::PopAndDestroy(&item);

	item = folder1.OpenItemL(1);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Id() == id2);
	CleanupStack::PopAndDestroy(&item);

	item = folder2.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Id() == id1);
	CleanupStack::PopAndDestroy(&item);

	item = folder2.OpenItemL(1);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Id() == id3);
	CleanupStack::PopAndDestroy(&item);

	CleanupStack::PopAndDestroy(2); //folders
	CleanupStack::PopAndDestroy(&root);
	}

LOCAL_D void RootFolderTestL()
	{
	gTestWrapper->Next(_L("[Folders tests] root folder tests"));
	
	RBkFolder root;
	root = gDatabase.OpenRootL();
	CleanupClosePushL(root);
	
	TRAPD(err, root.SetTitleL(KTxtSubRootName));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetDescriptionL(KTxtSubRootName));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetPublicL(EFalse));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetWritableL(EFalse));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetOwnerL(0x0001234));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetIconIdL(EFalse));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetCustomPropertyL(KUid, KInt));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetCustomPropertyL(KUid, KReal));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetCustomPropertyL(KUid, KData8Bit));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	TRAP(err, root.SetCustomPropertyL(KUid, KData16Bit));
	gTestWrapper->TESTE(err == Bookmark::KErrOperationDenied, err);
	
	CleanupStack::PopAndDestroy(&root);
	}

LOCAL_D void TestBKLastModifiedTimeL ()
	{
	TTime currentTime;		

	RBkDatabase db;
	db.OpenL ();
	CleanupClosePushL ( db );	
	// Create a new folder.
	RBkFolder folder = db.CreateFolderL ( KMyFolder );
	CleanupClosePushL ( folder );
	folder.SetDescriptionL ( KMyFolderDesc );

	db.CommitL(); // Commit the transaction.
	User::After ( KGeneralDelay );
	currentTime.HomeTime ();
	gTestWrapper->TEST( folder.LastModifiedL() < currentTime ); // Creation time.
	TTime newTime;
	newTime.HomeTime ();
	TRAPD( err, folder.SetLastModifiedL( newTime ) );
	// Should leave with KErrPermissionDenied error
	gTestWrapper->TESTE( err == KErrPermissionDenied, err );	

	CleanupStack::PopAndDestroy ( &folder ); // destroy/close
	// Open the above created folder.
	folder = db.OpenFolderL ( KMyFolder );
	CleanupClosePushL ( folder );
	folder.SetDescriptionL ( KMyFolderNewDesc );
 	
	db.CommitL (); 
	User::After ( KGeneralDelay );
	currentTime.HomeTime ();
	gTestWrapper->TEST( folder.LastModifiedL() < currentTime );  // Save time.
		
	Bookmark::TItemId id = folder.Id();
	CleanupStack::PopAndDestroy ( &folder );
	db.DeleteItemL ( id ); // mark for deleting	
	db.CommitL ();

	CleanupStack::PopAndDestroy ( &db );		
	}

LOCAL_D void TestBKLastModifiedTime1L ()
	{
	RBkDatabase db;
	db.OpenL ();
	CleanupClosePushL ( db );	
	
	// Create a new folder.
	RBkFolder folder = db.CreateFolderL ( KMyFolder );
	CleanupClosePushL ( folder );
	folder.SetDescriptionL ( KMyFolderDesc );

	db.CommitL(); // Commit the transaction.	
	
	TTime modifiedTime = folder.LastModifiedL ();
	
	CleanupStack::PopAndDestroy ( &folder );
	CleanupStack::PopAndDestroy ( &db ); // close the db
	
	// re-open it
	db.OpenL ();
	CleanupClosePushL ( db );
	folder  = db.OpenFolderL ( KMyFolder );
	CleanupClosePushL ( folder );
	gTestWrapper->TEST ( modifiedTime == folder.LastModifiedL() );
	
	Bookmark::TItemId id = folder.Id();
	CleanupStack::PopAndDestroy ( &folder );
	db.DeleteItemL ( id ); // mark for deleting	
	db.CommitL ();

	CleanupStack::PopAndDestroy ( &db );		
	}	
void DoFolderTestsL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** Folder tests ***"));

	gDatabase.OpenL();
	CleanupClosePushL(gDatabase);

	CreateFolderL();
	CreateManyFoldersL();
	RootFolderTestL();
	
	MoveBookmarksTest();
	
	TestBKLastModifiedTimeL ();
	TestBKLastModifiedTime1L ();
	
	CleanupStack::PopAndDestroy(&gDatabase);
	}
