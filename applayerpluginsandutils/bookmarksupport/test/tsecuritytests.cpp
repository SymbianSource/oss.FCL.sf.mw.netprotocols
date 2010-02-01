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

#include "testutils.h"
#include <bookmarkdatabase.h>

_LIT(KFilename, "tsecuritytests.cpp");
_LIT(KTitle, "Bookmark Security Unit Tests");

_LIT(KTxtFolder1, "securityTEST Folder 1");

_LIT ( KMyFolder, "MyFolder" );
_LIT ( KMyNewFolder, "MyNewFolder" );
_LIT ( KMyFolderDesc, "Testing last modified time in bookmark." );
_LIT ( KMyFolderNewDesc, "1: Testing last modified time in bookmark." );

const TInt KGeneralDelay = 2*1000*1000;

LOCAL_D RTest gTest(KTitle());

LOCAL_D CTrapCleanup* gTheTrapCleanup;
LOCAL_D CTestWrapper* gTestWrapper;

const TInt KTestCleanupStack=0x500;
_LIT_SECURE_ID(KAppId,0x00001234);

// Old bookmark database files path. These files will be copied into the 
// c drive bookmarks private folder. This files will be used for testing 
// missing last-modified time.
_LIT ( KSourcePath1, "z:\\bookmarks\\testdata\\10205af8.cre" );
_LIT ( KDestinationPath1, "c:\\private\\10202BE9\\persists\\10205af8.cre" );
_LIT ( KSourcePath2, "z:\\bookmarks\\testdata\\10205af9.cre" );
_LIT ( KDestinationPath2, "c:\\private\\10202BE9\\persists\\10205af9.cre" );
_LIT ( KSourcePath3, "z:\\bookmarks\\testdata\\10205afa.cre" );
_LIT ( KDestinationPath3, "c:\\private\\10202BE9\\persists\\10205afa.cre" );
_LIT ( KSourcePath4, "z:\\bookmarks\\testdata\\10205afb.cre" );
_LIT ( KDestinationPath4, "c:\\private\\10202BE9\\persists\\10205afb.cre" );
_LIT ( KUninstallDbFiles, "c:\\private\\10202BE9\\persists\\*.cre" );


LOCAL_C void DoOldDbCopyL ()
	{
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** Copying old database files ***"));
	RFs fs;
	User::LeaveIfError ( fs.Connect () );		
	CFileMan* fileMan = CFileMan::NewL ( fs );
	CleanupStack::PushL ( fileMan );
	fs.MkDirAll ( KDestinationPath1 );
	
	User::LeaveIfError ( fileMan->Copy ( KSourcePath1, KDestinationPath1 ) );	
	User::LeaveIfError ( fileMan->Copy ( KSourcePath2, KDestinationPath2 ) );
	User::LeaveIfError ( fileMan->Copy ( KSourcePath3, KDestinationPath3 ) );	
	User::LeaveIfError ( fileMan->Copy ( KSourcePath4, KDestinationPath4 ) );
			
	CleanupStack::PopAndDestroy ( fileMan );
	fs.Close ();		
	}

 
LOCAL_C void DoOldDbRemoveL ()
	{
	gTestWrapper->Next(_L("*** Removing old database files ***"));
	RFs fs;
	User::LeaveIfError ( fs.Connect () );		
	
	CFileMan* fileMan = CFileMan::NewL ( fs );
	CleanupStack::PushL ( fileMan );
	
	// Ensure all files are deletable.
	fileMan->Attribs ( KUninstallDbFiles, KEntryAttArchive, KEntryAttReadOnly, TTime(0), CFileMan::ERecurse );
	
	// Delete the files.
	fileMan->Delete ( KUninstallDbFiles );
	
	// Time for uninstall.	
	User::After ( KGeneralDelay );
	
	CleanupStack::PopAndDestroy ( fileMan );
	fs.Close ();		
	}

LOCAL_C void DoSecurityTestL()
	{
	gTestWrapper->Next(_L("*** Security tests ***"));
	
	RBkDatabase database1;
	database1.OpenL(Bookmark::EVisibilityManager);
	CleanupClosePushL(database1);
	gTestWrapper->Next(_L("[Security tests] Check Public & Writable"));
	
	RBkFolder folder = database1.CreateFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
	Bookmark::TItemId folderId = folder.Id();
	CleanupStack::PopAndDestroy(&folder);
	database1.CommitL();
	RBkFolder folder1 = database1.OpenFolderL(KTxtFolder1);
	CleanupClosePushL(folder1);
	folder1.SetOwnerL(0x00112233);
	RBkBookmark bookmark1 = database1.CreateBookmarkL(&folder1);
	CleanupClosePushL(bookmark1);
	Bookmark::TItemId bookmarkId = bookmark1.Id();
	CleanupStack::PopAndDestroy(&bookmark1);
	CleanupStack::PopAndDestroy(&folder1);
	database1.CommitL();
	CleanupStack::PopAndDestroy(&database1);
	
	RBkDatabase database2;
	database2.OpenL(Bookmark::EVisibilityManager);
	CleanupClosePushL(database2);
	RBkFolder folder2 = database2.OpenFolderL(KTxtFolder1);
	gTestWrapper->TEST(!folder2.IsPublic());
	CleanupClosePushL(folder2);
	RBkBookmark bookmark2 = database2.OpenBookmarkL(bookmarkId);
	CleanupClosePushL(bookmark2);
	gTestWrapper->TEST(bookmark2.IsPublic());
	gTestWrapper->TEST(bookmark2.OwnerL() == Bookmark::KNoOwnerId);
	bookmark2.SetOwnerL(0x00112233);
	gTestWrapper->TEST(!bookmark2.IsPublic());
	bookmark2.SetWritableL(EFalse);
	CleanupStack::PopAndDestroy(&bookmark2);
	CleanupStack::PopAndDestroy(&folder2);
	database2.CommitL();
	CleanupStack::PopAndDestroy(&database2);
	
	RBkDatabase database3;
	database3.OpenL(Bookmark::EVisibilityManager);
	CleanupClosePushL(database3);
	RBkBookmark bookmark3 = database3.OpenBookmarkL(bookmarkId);
	CleanupClosePushL(bookmark3);
	gTestWrapper->TEST(!bookmark3.IsPublic());
	RThread thread;
	TSecurityInfo secInfo(thread);
	gTestWrapper->TEST(bookmark3.OwnerL() == 0x00112233);
	gTestWrapper->TEST(!bookmark3.IsWritable());
	CleanupStack::PopAndDestroy(&bookmark3);
	database3.CommitL();
	CleanupStack::PopAndDestroy(&database3);
	
	RBkDatabase database4;
	database4.OpenL(Bookmark::EVisibilityManager);
	CleanupClosePushL(database4);
	RBkBookmark bookmark4 = database4.OpenBookmarkL(bookmarkId);
	CleanupClosePushL(bookmark4);
	gTestWrapper->TEST(!bookmark4.IsPublic());
	bookmark4.SetWritableL(ETrue);
	bookmark4.SetPublicL(ETrue);
	gTestWrapper->TEST(bookmark4.IsPublic());
	CleanupStack::PopAndDestroy(&bookmark4);
	database4.DeleteItemL(folderId, ETrue);
	database4.CommitL();
	CleanupStack::PopAndDestroy(&database4);
	}
	
LOCAL_C void DoFolderTitleTestL()
	{
	gTestWrapper->Next(_L("[Security tests] Folder title test"));
	RBkDatabase database;
	database.OpenL(Bookmark::EVisibilityManager);
	CleanupClosePushL(database);
	RBkFolder folder = database.CreateFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
	Bookmark::TItemId folderId = folder.Id();
	CleanupStack::PopAndDestroy(&folder);
	database.CommitL();
	folder = database.OpenFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
 	folder.SetOwnerL(KAppId);
	CleanupStack::PopAndDestroy(&folder);
	database.CommitL();
	CleanupStack::PopAndDestroy(&database);
	
	database.OpenL(Bookmark::EVisibilityPrivate);
	CleanupClosePushL(database);
	RBkFolder root = database.OpenRootL();
	CleanupClosePushL(root);
	gTestWrapper->TEST(root.Count() == 0);
	CleanupStack::PopAndDestroy(&root);
	TRAPD(err, folder = database.CreateFolderL(KTxtFolder1));
	gTestWrapper->TESTE(err == Bookmark::KErrTitleAlreadyUsed, err);
	CleanupStack::PopAndDestroy(&database);
	
	database.OpenL(Bookmark::EVisibilityManager);
	CleanupClosePushL(database);
	database.DeleteItemL(folderId, ETrue);
	database.CommitL();
	CleanupStack::PopAndDestroy(&database);
	}

LOCAL_C void DoTestSecLastModifiedTimeL ()
	{
	gTestWrapper->Next(_L("[Security tests] Test last modified time"));
	TTime currentTime;
	RBkDatabase db;
	db.OpenL (Bookmark::EVisibilityManager);
	CleanupClosePushL ( db );	
	
	// Create a new folder.
	RBkFolder folder = db.CreateFolderL ( KMyNewFolder );
	CleanupClosePushL ( folder );
	folder.SetDescriptionL ( KMyFolderDesc );
	currentTime.HomeTime ();
	folder.SetLastModifiedL ( currentTime );
	db.CommitL ();
	gTestWrapper->TEST( folder.LastModifiedL() == currentTime ); // Creation time.
	CleanupStack::PopAndDestroy ( &folder );
	
	// Open the above created folder.
	folder = db.OpenFolderL ( KMyNewFolder );
	CleanupClosePushL ( folder );
	folder.SetDescriptionL ( KMyFolderNewDesc );
	currentTime.HomeTime ();
	folder.SetLastModifiedL ( currentTime );
	db.CommitL ();
	gTestWrapper->TEST( folder.LastModifiedL() == currentTime ); // Creation time.
	Bookmark::TItemId id = folder.Id();
	db.DeleteItemL ( id ); // mark for deleting	
	db.CommitL ();	
	
	CleanupStack::PopAndDestroy ( &folder );				
	CleanupStack::PopAndDestroy ( &db );		
	}

LOCAL_C void DoTestSecLastModifiedTime1L ()
	{	
	gTestWrapper->Next(_L("[Security tests] Test last modified time1"));
	TTime currentTime;
	
	RBkDatabase db;
	db.OpenL (Bookmark::EVisibilityManager);
	CleanupClosePushL ( db );	
	
	// Create a new folder.
	RBkFolder folder = db.CreateFolderL ( KMyNewFolder );
	CleanupClosePushL ( folder );
	folder.SetDescriptionL ( KMyFolderDesc );
	currentTime.HomeTime ();
	folder.SetLastModifiedL ( currentTime );
	db.CommitL ();
	
	CleanupStack::PopAndDestroy ( &folder );
	CleanupStack::PopAndDestroy ( &db );
	
	db.OpenL (Bookmark::EVisibilityManager);
	CleanupClosePushL ( db );	
	folder = db.OpenFolderL ( KMyNewFolder );
	CleanupClosePushL ( folder );
	gTestWrapper->TEST ( currentTime == folder.LastModifiedL() );
	
	Bookmark::TItemId id = folder.Id();
	db.DeleteItemL ( id ); // mark for deleting	
	db.CommitL ();	
	
	CleanupStack::PopAndDestroy ( &folder );				
	CleanupStack::PopAndDestroy ( &db );		
	}

LOCAL_D void DoTestOldDB ()
	{
	gTestWrapper->Next(_L("*** Testing old database ***"));
	RBkDatabase db;
	db.OpenL (Bookmark::EVisibilityManager);
	CleanupClosePushL ( db );		
		
	// Check for older database data.
	RBkFolder folder = 	db.OpenFolderL ( KMyFolder );	
	CleanupClosePushL ( folder );
	Bookmark::TItemId id = folder.Id ();
	TDateTime dt ( 1970, EJanuary, 1, 0, 0, 0, 0 );
	TTime currentTime ( dt );
	
	gTestWrapper->TEST( folder.LastModifiedL() == currentTime );	

	CleanupStack::PopAndDestroy ( &folder );		
	
	CleanupStack::PopAndDestroy ( &db );			
	}

LOCAL_C void DoTestsL()
	{
	DoOldDbCopyL ();	
	DoTestOldDB ();
	DoOldDbRemoveL ();
	DoSecurityTestL();
	DoFolderTitleTestL();
	DoTestSecLastModifiedTimeL ();
	DoTestSecLastModifiedTime1L ();
	DoOldDbRemoveL ();
	}
	
LOCAL_C void TestL()
	{
	gTestWrapper = CTestWrapper::NewLC(gTest);
	gTestWrapper->Start(_L("@SYMTestCaseID IWS-APPPROTOCOLS-BOOKMARKS-TSECURITYTESTS-0001 test"));

	TRAPD(ret,DoTestsL());
	gTestWrapper->TESTE(ret==KErrNone, ret);

	gTestWrapper->End();
	CleanupStack::PopAndDestroy(gTestWrapper);
	}

LOCAL_C void setupCleanup()
//
// Initialise the cleanup stack.
//
    {
	gTheTrapCleanup=CTrapCleanup::New();
	gTest(gTheTrapCleanup!=NULL);
	TRAPD(r,\
		{\
		for (TInt i=KTestCleanupStack;i>0;i--)\
			CleanupStack::PushL((TAny*)0);\
		CleanupStack::Pop(KTestCleanupStack);\
		});
	gTest(r==KErrNone);
	}


GLDEF_C TInt E32Main()
    {
	setupCleanup();
	__UHEAP_MARK;
	//

	TRAPD(ret,TestL());
	gTest(ret==KErrNone);
	//

	__UHEAP_MARKEND;
	delete gTheTrapCleanup;
	return KErrNone;
    }

