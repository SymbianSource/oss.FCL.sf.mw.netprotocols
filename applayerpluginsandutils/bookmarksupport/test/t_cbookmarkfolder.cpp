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
#include "testutils.h"

LOCAL_D CTestWrapper* gTestWrapper;

_LIT(KFilename, "t_cbookmarkfolder.cpp");
_LIT(KTxtFolder1, "Folder 1");
_LIT(KTxtFolder2, "Folder 2");
_LIT(KTxtFolder3, "Folder 3");
_LIT(KTxtDesc, "This is an extended property description");

LOCAL_D void MultiFolderTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CBookmarkFolder& folder = db->CreateFolderL(KTxtFolder1, db->RootL());
	Bookmark::TItemId folderId = folder.Id();
	
	gTestWrapper->Next(_L("[CBookmarkFolder tests] unique folder names"));
	TRAPD(err, db->CreateFolderL(KTxtFolder1, db->RootL()));
	gTestWrapper->TESTE(err == Bookmark::KErrTitleAlreadyUsed, err);
	TRAP(err, db->CreateFolderL(KTxtFolder1, folder));
	gTestWrapper->TESTE(err == Bookmark::KErrTitleAlreadyUsed, err);
	
	CBookmarkFolder& root = db->RootL();
	gTestWrapper->TEST(root.Count() == 1);
	
	db->CommitL();
	db->RefreshL();
	
	TRAP(err, db->CreateFolderL(KTxtFolder1, db->RootL()));
	gTestWrapper->TESTE(err == Bookmark::KErrTitleAlreadyUsed, err);
	TRAP(err, db->CreateFolderL(KTxtFolder1, folder));
	gTestWrapper->TESTE(err == Bookmark::KErrTitleAlreadyUsed, err);
	
	CBookmarkFolder& root2 = db->RootL();
	gTestWrapper->TEST(root2.Count() == 1);
	
	db->DeleteItemL(folderId, ETrue);
	db->CommitL();
	
	CleanupStack::PopAndDestroy(db);
	}
	
LOCAL_D void GeneralTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CBookmarkFolder& folder = db->CreateFolderL(KTxtFolder1, db->RootL());
	
	gTestWrapper->Next(_L("[CBookmarkFolder tests] send children to folder"));
	CBookmarkFolder& root = db->RootL();
	gTestWrapper->TEST(root.Count() == 1);
	gTestWrapper->TEST(folder.Count() == 0);
	
	CBookmark& bookmark1 = db->CreateBookmarkL(folder);
	CBookmark& bookmark2 = db->CreateBookmarkL(folder);
	CBookmark& bookmark3 = db->CreateBookmarkL(folder);
	CBookmark& bookmark4 = db->CreateBookmarkL(folder);
	gTestWrapper->TEST(root.Count() == 1);
	gTestWrapper->TEST(folder.Count() == 4);
	folder.SendChildrenToFolderL(root);
	gTestWrapper->TEST(root.Count() == 5);
	gTestWrapper->TEST(folder.Count() == 0);
	
	gTestWrapper->Next(_L("[CBookmarkFolder tests] append"));
	folder.AppendL(bookmark1);
	folder.AppendL(bookmark2);
	folder.AppendL(bookmark3);
	folder.AppendL(bookmark4);
	gTestWrapper->TEST(root.Count() == 1);
	gTestWrapper->TEST(folder.Count() == 4);
	
	gTestWrapper->Next(_L("[CBookmarkFolder tests] InsertL() & At()"));
	root.InsertL(bookmark3,0);
	root.InsertL(bookmark2,1);
	root.InsertL(bookmark1,1);
	root.InsertL(bookmark4,2);
	gTestWrapper->TEST(root.Count() == 5);
	gTestWrapper->TEST(folder.Count() == 0);
	gTestWrapper->TEST(root.At(0).Id() == bookmark3.Id());
	gTestWrapper->TEST(root.At(1).Id() == bookmark1.Id());
	gTestWrapper->TEST(root.At(2).Id() == bookmark4.Id());
	gTestWrapper->TEST(root.At(3).Id() == bookmark2.Id());
		
	gTestWrapper->Next(_L("[CBookmarkFolder tests] Move"));
	root.Move(0,2);
	gTestWrapper->TEST(root.At(0).Id() == bookmark1.Id());
	gTestWrapper->TEST(root.At(1).Id() == bookmark4.Id());
	gTestWrapper->TEST(root.At(2).Id() == bookmark3.Id());
	gTestWrapper->TEST(root.At(3).Id() == bookmark2.Id());
	
	gTestWrapper->Next(_L("[CBookmarkFolder tests] Index"));
	gTestWrapper->TEST(root.Index(bookmark1) == 0);
	gTestWrapper->TEST(root.Index(bookmark4) == 1);
	gTestWrapper->TEST(root.Index(bookmark3) == 2);
	gTestWrapper->TEST(root.Index(bookmark2) == 3);
	
	gTestWrapper->Next(_L("[CBookmarkFolder tests] Cyclic append"));
	CBookmarkFolder& folder2 = db->CreateFolderL(KTxtFolder2, folder);
	CBookmarkFolder& folder3 = db->CreateFolderL(KTxtFolder3, folder2);
	gTestWrapper->TEST(root.Count() == 5);
	gTestWrapper->TEST(folder.Count() == 1);
	gTestWrapper->TEST(folder2.Count() == 1);
	gTestWrapper->TEST(folder3.Count() == 0);
	
	TRAPD(err, folder3.AppendL(root));
	gTestWrapper->TESTE(err == Bookmark::KErrCyclicLoop, err);
	TRAP(err, folder3.AppendL(folder));
	gTestWrapper->TESTE(err == Bookmark::KErrCyclicLoop, err);
	TRAP(err, folder3.AppendL(folder2));
	gTestWrapper->TESTE(err == Bookmark::KErrCyclicLoop, err);
	
	gTestWrapper->Next(_L("[CBookmarkFolder tests] handle"));
	RBkFolder handle = folder.OpenFolder();
	CleanupClosePushL(handle);
	gTestWrapper->TEST(handle.Type() == folder.Type());
	gTestWrapper->TEST(handle.Title() == folder.Title());
	CleanupStack::PopAndDestroy(&handle);
	
	CleanupStack::PopAndDestroy(db);
	}
	
LOCAL_D void ExtendedPropertyTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CBookmarkFolder& folder = db->CreateFolderL(KTxtFolder1, db->RootL());
	CBkmrkProperties& properties = folder.BkmrkPropertiesL();
	
	gTestWrapper->TEST(properties.Description().Compare(KTxtDesc) != 0);
	properties.SetDescriptionL(KTxtDesc);
	gTestWrapper->TEST(properties.Description().Compare(KTxtDesc) == 0);
		
	CleanupStack::PopAndDestroy(db);
	}

void DoCBookmarkFolderUnitL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** CBookmarkFolder tests ***"));

	MultiFolderTestsL();
	GeneralTestsL();
	ExtendedPropertyTestsL();
	}
