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
#include "tbookmarktests.h"

_LIT(KTitle, "Bookmark Dll Tests");

LOCAL_D RTest gTest(KTitle());

LOCAL_D CTrapCleanup* gTheTrapCleanup;
LOCAL_D CTestWrapper* gTestWrapper;

const TInt KTestCleanupStack=0x500;

const TInt KGeneralDelay = 2*1000*1000;
_LIT ( KUninstallDbFiles, "c:\\private\\10202BE9\\persists\\*.cre" );
_LIT ( KRefreshTestFolder, "Refresh Test Folder" );

LOCAL_C void DoDbRemoveL ()
	{
	gTestWrapper->Next(_L("*** Removing bookmark database files ***"));
	RFs fs;
	User::LeaveIfError ( fs.Connect () );		
	CleanupClosePushL ( fs );
	CFileMan* fileMan = CFileMan::NewL ( fs );
	CleanupStack::PushL ( fileMan );
	
	// Ensure all files are deletable.
	fileMan->Attribs ( KUninstallDbFiles, KEntryAttArchive, KEntryAttReadOnly, TTime(0), CFileMan::ERecurse );
	
	// Delete the files.
	fileMan->Delete ( KUninstallDbFiles );
	
	// Time for uninstall.	
	User::After ( KGeneralDelay );
	
	CleanupStack::PopAndDestroy ( 2 ); // fileMan, fs	
	}

LOCAL_C void TestRbkDatabaseFnsWithNoRepFilesL ()
	{		
	DoDbRemoveL ();

	gTestWrapper->Next(_L("Testing bookmarks with no repository file present"));

	RBkDatabase db;
	db.OpenL ( Bookmark::EVisibilityManager );
	CleanupClosePushL ( db );
	TRAPD ( err, db.RefreshL () );
	gTestWrapper->TEST ( err == KErrNone );
	TRAP ( err, db.OpenRootL () );
	gTestWrapper->TEST ( err == KErrNone );
	TInt ret = 0;
	TRAP ( err, ret = db.DeleteIconL ( 1 ) );
	gTestWrapper->TEST ( err == KErrNone );
	gTestWrapper->TEST ( ret == KErrNotFound );
	TRAP ( err, db.OpenHomeL () );
	gTestWrapper->TEST ( err == KErrNotFound );
	const TUid KMyUid1 = {0x111222};
	TRAP ( err, ret = db.DeregisterDatabasePropertyL (KMyUid1) );
	gTestWrapper->TEST ( err == KErrNone );
	gTestWrapper->TEST ( ret == KErrNotFound );
	TRAP ( err, ret = db.DeregisterFolderPropertyL (KMyUid1) );
	gTestWrapper->TEST ( err == KErrNone );
	gTestWrapper->TEST ( ret == KErrNotFound );
	TRAP ( err, ret = db.DeregisterBookmarkPropertyL (KMyUid1) );
	gTestWrapper->TEST ( err == KErrNone );
	gTestWrapper->TEST ( ret == KErrNotFound );
	
	const TInt KMaxTestLength  = 60;	
	TInt intValue;
	TReal retReal = 0.0;
	HBufC* retDesC16 = HBufC::NewLC(KMaxTestLength);
	TPtr retPtr16 = retDesC16->Des();
	HBufC8* retDesC8 = HBufC8::NewLC(KMaxTestLength * 2);
	TPtr8 retPtr8 = retDesC8->Des();

	TRAP ( err, db.GetCustomPropertyL (KMyUid1, intValue) );
	gTestWrapper->TEST ( ret == KErrNotFound );	
	
	TRAP ( err, db.GetCustomPropertyL (KMyUid1, retReal) );
	gTestWrapper->TEST ( ret == KErrNotFound );	

	TRAP ( err, db.GetCustomPropertyL (KMyUid1, retPtr16) );
	gTestWrapper->TEST ( ret == KErrNotFound );	

	TRAP ( err, db.GetCustomPropertyL (KMyUid1, retPtr8) );
	gTestWrapper->TEST ( ret == KErrNotFound );	

	TRAP ( err, db.SetCustomPropertyL (KMyUid1, intValue) );
	gTestWrapper->TEST ( ret == KErrNotFound );	
	
	TRAP ( err, db.SetCustomPropertyL (KMyUid1, retReal) );
	gTestWrapper->TEST ( ret == KErrNotFound );	

	TRAP ( err, db.SetCustomPropertyL (KMyUid1, retPtr16) );
	gTestWrapper->TEST ( ret == KErrNotFound );	

	TRAP ( err, db.SetCustomPropertyL (KMyUid1, retPtr8) );
	gTestWrapper->TEST ( ret == KErrNotFound );	
	
	CleanupStack::PopAndDestroy (3);	 // db, retDesC16, retDesC8
	}

LOCAL_C void DoRefreshTestL ()
	{
	DoDbRemoveL ();	
	gTestWrapper->Next(_L("Testing the refreshing functionality of bookmarks"));	
	
	RBkDatabase db;
	CleanupClosePushL ( db );
	db.OpenL ();
	
	db.RefreshL ();
	
	RBkFolder root = db.OpenRootL ();
	CleanupClosePushL ( root );
	gTestWrapper->TEST ( root.Count() == 0 );
	// Create a folder under root.
	RBkFolder folder1 = db.CreateFolderL ( KRefreshTestFolder );
	CleanupClosePushL ( folder1 );
	db.CommitL ();
	CleanupStack::PopAndDestroy (3); // db, root, folder1
	
	db.OpenL ();
	CleanupClosePushL ( db );
	
	db.RefreshL ();
	
	TRAPD ( err, folder1 = db.OpenFolderL ( KRefreshTestFolder ) );
	gTestWrapper->TEST ( err == KErrNone );

	Bookmark::TItemId id = folder1.Id ();
	folder1.Close ();
	db.DeleteItemL ( id );
	db.CommitL ();
	CleanupStack::PopAndDestroy (); // db	
	}

LOCAL_C void DoTestsL()
	{
	TestRbkDatabaseFnsWithNoRepFilesL ();
	DoRefreshTestL ();
	DoFolderTestsL(gTestWrapper);
	DoBookmarkTestsL(gTestWrapper);
	DoNotifyTestsL(gTestWrapper);
	DoDirtyTestsL(gTestWrapper);
	}


LOCAL_C void TestL()
	{
	gTestWrapper = CTestWrapper::NewLC(gTest);
	gTestWrapper->Start(_L("@SYMTestCaseID IWS-APPPROTOCOLS-BOOKMARKS-TBOOKMARK-0001 test"));

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

