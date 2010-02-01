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

#include "bkmrk.h"
#include "bkmrkfolder.h"
#include "bkmrkdb.h"
#include "testutils.h"

LOCAL_D CTestWrapper* gTestWrapper;

_LIT(KFilename, "t_cbookmarkbase.cpp");

_LIT(KTxtNewFolder, "New Folder 1");

_LIT(KTxtItem1, "Bookmark Item 1");

const Bookmark::TItemId KItemId = 0x00000123;

LOCAL_D void GeneralTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityManager, NULL);
	CleanupStack::PushL(db);
	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());

	gTestWrapper->Next(_L("[CBookmarkBase tests] id"));
	gTestWrapper->TEST(bookmark.Id() != KItemId);
	bookmark.SetId(KItemId);
	gTestWrapper->TEST(bookmark.Id() == KItemId);

	gTestWrapper->Next(_L("[CBookmarkBase tests] reference counting"));
	gTestWrapper->TEST(bookmark.RefCount() == 0);
	bookmark.IncRefCount();
	gTestWrapper->TEST(bookmark.RefCount() == 1);
	bookmark.DecRefCount();
	gTestWrapper->TEST(bookmark.RefCount() == 0);
	bookmark.IncRefCount();
	bookmark.IncRefCount();
	bookmark.IncRefCount();
	gTestWrapper->TEST(bookmark.RefCount() == 3);
	bookmark.DecRefCount();
	bookmark.DecRefCount();
	bookmark.DecRefCount();
	gTestWrapper->TEST(bookmark.RefCount() == 0);
	
	gTestWrapper->Next(_L("[CBookmarkBase tests] parent"));
	CBookmarkFolder& folder = db->CreateFolderL(KTxtNewFolder, db->RootL());
	gTestWrapper->TEST(bookmark.Parent()->Id() == db->RootL().Id());
	bookmark.SetParentL(folder);
	gTestWrapper->TEST(bookmark.Parent()->Id() == folder.Id());
	bookmark.SetParentL(db->RootL());
	gTestWrapper->TEST(bookmark.Parent()->Id() == db->RootL().Id());

	gTestWrapper->Next(_L("[CBookmarkBase tests] title"));
	bookmark.SetTitleL(KTxtItem1);
	gTestWrapper->TEST(bookmark.Title().Compare(KTxtItem1) == 0);

	gTestWrapper->Next(_L("[CBookmarkBase tests] public and writable"));
	gTestWrapper->TEST(bookmark.IsPublic());
	
	TInt err = KErrNone;
	gTestWrapper->TEST(bookmark.IsPublic());
	
	gTestWrapper->TEST(bookmark.IsWritable());
	bookmark.SetWritableL(EFalse);
	gTestWrapper->TEST(!bookmark.IsWritable());
	// test that write operations work as application is WriteDeviceData
	TRAP(err, bookmark.SetTitleL(KTxtItem1));
	gTestWrapper->TEST(err == KErrNone);
	TRAP(err, bookmark.SetPublicL(ETrue));
	gTestWrapper->TEST(err == KErrNone);
	bookmark.SetWritableL(ETrue);
	
	gTestWrapper->Next(_L("[CBookmarkBase tests] handle"));
	gTestWrapper->TEST(bookmark.RefCount() == 0);
	RBkFolder root = db->RootL().OpenFolder();
	CleanupClosePushL(root);
	RBkNode handle = root.OpenItemL(0);
	CleanupClosePushL(handle);
	
	gTestWrapper->TEST(bookmark.RefCount() == 1);
	gTestWrapper->TEST(handle.Type() == bookmark.Type());
	gTestWrapper->TEST(handle.Title() == bookmark.Title());
	
	CleanupStack::PopAndDestroy(&handle);
	CleanupStack::PopAndDestroy(&root);

	CleanupStack::PopAndDestroy(db);
	}

void DoCBookmarkBaseUnitL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** CBookmarkBase tests ***"));

	GeneralTestsL();
	}
