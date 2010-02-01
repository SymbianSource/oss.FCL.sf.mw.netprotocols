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

#include "bkmrkdb.h"
#include "bkmrk.h"
#include "bkmrkfolder.h"
#include "testutils.h"

LOCAL_D CTestWrapper* gTestWrapper;
LOCAL_D Bookmark::TAttachmentId gIconId1;
LOCAL_D Bookmark::TAttachmentId gIconId2;

_LIT(KFilename, "t_cbookmarkdb.cpp");

_LIT(KTxtFolder1, "BkmrkTEST Folder 1");
_LIT(KTxtFolder2, "BkmrkTEST Folder 2");
_LIT(KTxtFolder3, "BkmrkTEST Folder 3");

_LIT(KTxtNewFolder, "New Folder 1");
_LIT(KTxtNewFolder2, "New Folder 2");
_LIT(KTxtNewFolder3, "New Folder 3");

_LIT(KTxtBookmark1, "BkmrkTEST Bookmark 1");
_LIT(KTxtBookmark2, "BkmrkTEST Bookmark 2");
_LIT(KTxtBookmark3, "BkmrkTEST Bookmark 3");
_LIT(KTxtBookmark4, "BkmrkTEST Bookmark 4");

_LIT8(KIconData, 	"123456789");
_LIT8(KIconData2,	"999999999");

_LIT(KTxtDesc, "This is the home page text");
_LIT8(KTxtUri, "sip:user:pass@location.com:1666;transport=tcp;x=123;df=223?asd=b");

const TUint32 KProxyId 	= 0x7CFFFFFF;
const TUint32 KNapId 	= 0x6CFFF000;

LOCAL_D void GeneralTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] Icons"));
	Bookmark::TAttachmentId id = db->CreateIconL(KIconData);
	gTestWrapper->TEST(db->IconL(id).Compare(KIconData) == 0);
	gTestWrapper->TEST(db->IconL(id).Compare(KIconData2) != 0);
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] Home"));
	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());
	gTestWrapper->TEST(!db->Home());
	db->SetHome(&bookmark);
	gTestWrapper->TEST(db->Home()->Id() == bookmark.Id());
	db->SetHome(NULL);
	gTestWrapper->TEST(!db->Home());
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] home page text"));
	db->SetHomePageTextL(KTxtDesc);
	gTestWrapper->TEST(db->HomePageText().Compare(KTxtDesc) == 0);
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] search uri"));
	db->SetSearchUriL(KTxtUri);
	gTestWrapper->TEST(db->SearchUri().Compare(KTxtUri) == 0);

	gTestWrapper->Next(_L("[CBookmarkDb tests] default proxy and nap"));
	gTestWrapper->TEST(db->DefaultProxy() != KProxyId);
	db->SetDefaultProxy(KProxyId);
	gTestWrapper->TEST(db->DefaultProxy() == KProxyId);
	
	gTestWrapper->TEST(db->DefaultNap() != KNapId);
	db->SetDefaultNap(KNapId);
	gTestWrapper->TEST(db->DefaultNap() == KNapId);
	
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void TreeTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] Construct tree and root"));
	db->ConstructTreeL();
	CBookmarkBase* rootPtr = db->FindItem(Bookmark::KRootItemID);
	gTestWrapper->TEST(rootPtr != NULL);
	gTestWrapper->TEST(db->RootL().Id() == rootPtr->Id());
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] Create items"));
	CBookmarkFolder& folder1 = db->CreateFolderL(KTxtFolder1, db->RootL());
	CBookmark& bookmark1 = db->CreateBookmarkL(folder1);
	CBookmarkFolder& folder2 = db->CreateFolderL(KTxtFolder2, folder1);
	CBookmark& bookmark2 = db->CreateBookmarkL(folder2);
	CBookmark& bookmark3 = db->CreateBookmarkL(folder2);
	CBookmarkFolder& folder3 = db->CreateFolderL(KTxtFolder3, folder2);
	CBookmark& bookmark4 = db->CreateBookmarkL(folder3);
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] Find items"));
	CBookmarkBase* item = db->FindItem(folder1.Id());
	CBookmarkFolder* folder = db->FindFolder(KTxtFolder1);
	gTestWrapper->TEST(item != NULL);
	gTestWrapper->TEST(item == folder);
	gTestWrapper->TEST(item->Parent()->Id() == Bookmark::KRootItemID);
	item = db->FindItem(bookmark1.Id());
	gTestWrapper->TEST(item != NULL);
	gTestWrapper->TEST(item->Parent()->Id() == folder1.Id());
	item = db->FindItem(folder2.Id());
	folder = db->FindFolder(KTxtFolder2);
	gTestWrapper->TEST(item != NULL);
	gTestWrapper->TEST(item == folder);
	gTestWrapper->TEST(item->Parent()->Id() == folder1.Id());
	item = db->FindItem(bookmark2.Id());
	gTestWrapper->TEST(item != NULL);
	gTestWrapper->TEST(item->Parent()->Id() == folder2.Id());
	item = db->FindItem(bookmark3.Id());
	gTestWrapper->TEST(item != NULL);
	gTestWrapper->TEST(item->Parent()->Id() == folder2.Id());
	item = db->FindItem(folder3.Id());
	folder = db->FindFolder(KTxtFolder3);
	gTestWrapper->TEST(item != NULL);
	gTestWrapper->TEST(item == folder);
	gTestWrapper->TEST(item->Parent()->Id() == folder2.Id());
	item = db->FindItem(bookmark4.Id());
	gTestWrapper->TEST(item != NULL);
	gTestWrapper->TEST(item->Parent()->Id() == folder3.Id());
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] Delete items"));
	CBookmarkFolder& root = db->RootL();
	gTestWrapper->TEST(root.Count() == 1);
	gTestWrapper->TEST(folder1.Count() == 2);
	gTestWrapper->TEST(folder2.Count() == 3);
	gTestWrapper->TEST(folder3.Count() == 1);
	db->DeleteItemL(folder2.Id(), EFalse);
	folder = db->FindFolder(KTxtFolder2);
	gTestWrapper->TEST(!folder);
	gTestWrapper->TEST(root.Count() == 4);
	gTestWrapper->TEST(folder1.Count() == 1);
	gTestWrapper->TEST(folder3.Count() == 1);
	db->DeleteItemL(folder1.Id(), ETrue);
	folder = db->FindFolder(KTxtFolder1);
	gTestWrapper->TEST(!folder);
	gTestWrapper->TEST(root.Count() == 3);
	db->DeleteItemL(bookmark2.Id(), EFalse);
	folder = db->FindFolder(KTxtBookmark2);
	gTestWrapper->TEST(folder == NULL);
	db->DeleteItemL(bookmark4.Id(), ETrue);
	folder = db->FindFolder(KTxtBookmark4);
	gTestWrapper->TEST(!folder);
	gTestWrapper->TEST(root.Count() == 2);
	gTestWrapper->TEST(folder3.Count() == 0);
	
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void CreateTreeL(CBookmarkDb* aDb)
	{
	gTestWrapper->Next(_L("[CBookmarkDb tests] Create tree"));
	CBookmarkFolder& folder1 = aDb->CreateFolderL(KTxtFolder1, aDb->RootL());
	CBookmark& bookmark1 = aDb->CreateBookmarkL(folder1);
	bookmark1.SetTitleL(KTxtBookmark1);
	CBookmarkFolder& folder2 = aDb->CreateFolderL(KTxtFolder2, folder1);
	CBookmark& bookmark2 = aDb->CreateBookmarkL(folder2);
	bookmark2.SetTitleL(KTxtBookmark2);
	CBookmark& bookmark3 = aDb->CreateBookmarkL(folder2);
	bookmark3.SetTitleL(KTxtBookmark3);
	CBookmarkFolder& folder3 = aDb->CreateFolderL(KTxtFolder3, folder2);
	CBookmark& bookmark4 = aDb->CreateBookmarkL(folder3);
	bookmark4.SetTitleL(KTxtBookmark4);
	
	// create a couple of icons
	gIconId1 = aDb->CreateIconL(KIconData);
	gIconId2 = aDb->CreateIconL(KIconData2);
	CBkmrkExtendedProperties& properties1 = bookmark1.BkmrkExtendedPropertiesL();
	properties1.SetIconIdL(gIconId1);
	CBkmrkExtendedProperties& properties2 = bookmark2.BkmrkExtendedPropertiesL();
	properties2.SetIconIdL(gIconId2);
	
	CBookmarkFolder& root = aDb->RootL();
	gTestWrapper->TEST(root.Count() == 1);
	gTestWrapper->TEST(folder1.Count() == 2);
	gTestWrapper->TEST(folder2.Count() == 3);
	gTestWrapper->TEST(folder3.Count() == 1);
	}
	
LOCAL_D void CheckDatabaseL(CBookmarkDb& aDb)
	{
	CBookmarkFolder& root = aDb.RootL();
	gTestWrapper->TEST(root.Count() == 1);
	CBookmarkFolder* folder1 = aDb.FindFolder(KTxtFolder1);
	gTestWrapper->TEST(folder1 != NULL);
	gTestWrapper->TEST(folder1->Count() == 2);
	gTestWrapper->TEST(folder1->At(0).Type() == Bookmark::ETypeBookmark);
	CBookmark& bookmark1 = static_cast<CBookmark&>(folder1->At(0));
	CBkmrkExtendedProperties& properties1 = bookmark1.BkmrkExtendedPropertiesL();
	gTestWrapper->TEST(properties1.IconId() == gIconId1);
	CBookmarkFolder* folder2 = aDb.FindFolder(KTxtFolder2);
	gTestWrapper->TEST(folder2 != NULL);
	gTestWrapper->TEST(folder2->Count() == 3);
	gTestWrapper->TEST(folder2->At(0).Type() == Bookmark::ETypeBookmark);
	CBookmark& bookmark2 = static_cast<CBookmark&>(folder2->At(0));
	CBkmrkExtendedProperties& properties2 = bookmark2.BkmrkExtendedPropertiesL();
	gTestWrapper->TEST(properties2.IconId() == gIconId2);
	CBookmarkFolder* folder3 = aDb.FindFolder(KTxtFolder3);
	gTestWrapper->TEST(folder3 != NULL);
	gTestWrapper->TEST(folder3->Count() == 1);
	}
	
LOCAL_D void CommitTestsL()
	{
	gTestWrapper->Next(_L("[CBookmarkDb tests] Commit"));
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	CreateTreeL(db);
	db->CommitL();
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CheckDatabaseL(*db);
	
	CBookmarkFolder* folder = db->FindFolder(KTxtFolder1);
	db->DeleteItemL(folder->Id(), ETrue);
	gTestWrapper->TEST(db->RootL().Count() == 0);
	
	CleanupStack::PopAndDestroy(db);
	
	// The delete has not been committed so it should reload 
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CheckDatabaseL(*db);
	
	folder = db->FindFolder(KTxtFolder1);
	db->DeleteItemL(folder->Id(), ETrue);
	gTestWrapper->TEST(db->RootL().Count() == 0);
	
	User::LeaveIfError(db->DeleteIconL(gIconId1));
	User::LeaveIfError(db->DeleteIconL(gIconId2));
	
	db->CommitL();
	
	CleanupStack::PopAndDestroy(db);
	
	// The delete has been committed so it should not reload 
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	CBookmarkFolder& root = db->RootL();
	gTestWrapper->TEST(root.Count() == 0);
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void IconDataTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	gTestWrapper->Next(_L("[CBookmarkDb tests] Icon data tests"));
	Bookmark::TAttachmentId iconId = db->CreateIconL(KIconData);
	gTestWrapper->TEST(db->IconL(iconId).Compare(KIconData) == 0);
	gTestWrapper->TEST(db->IconL(iconId).Compare(KIconData2) != 0);

	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());
	Bookmark::TItemId bookmarkId = bookmark.Id();
	CBkmrkExtendedProperties& properties = bookmark.BkmrkExtendedPropertiesL();
	properties.SetIconIdL(iconId);
	
	db->CommitL();
	
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	db->ConstructTreeL();
	
	CBookmark* bookmarkPtr = static_cast<CBookmark*>(db->FindItem(bookmarkId));
	if (!bookmarkPtr)
		{
		User::Leave(KErrNotFound);
		}
	CBkmrkExtendedProperties& properties2 = bookmarkPtr->BkmrkExtendedPropertiesL();
	iconId = properties2.IconId();
	gTestWrapper->TEST(db->IconL(iconId).Compare(KIconData) == 0);
	gTestWrapper->TEST(db->IconL(iconId).Compare(KIconData2) != 0);
	
	// cleanup
	db->DeleteItemL(bookmarkId, ETrue);
	db->DeleteIconL(iconId);
	db->CommitL();
	
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void CreateSecurityTreeL(CBookmarkDb& aDb)
	{
	CBookmarkFolder& folder1 = aDb.CreateFolderL(KTxtNewFolder, aDb.RootL());
	CBookmarkFolder& folder2 = aDb.CreateFolderL(KTxtNewFolder2, folder1);
	CBookmarkFolder& folder3 = aDb.CreateFolderL(KTxtNewFolder3, folder2);
	aDb.CommitL();
	
	folder3.SetPublicL(EFalse);
	
	CBookmark& bookmark1 = aDb.CreateBookmarkL(folder1);
	CBookmark& bookmark2 = aDb.CreateBookmarkL(folder2);
	CBookmark& bookmark3 = aDb.CreateBookmarkL(folder3);
	aDb.CommitL();
	
	bookmark1.SetPublicL(ETrue); // to avoid compiler warnings
	bookmark2.SetPublicL(EFalse);
	bookmark3.SetPublicL(ETrue); // to avoid compiler warnings
	aDb.CommitL();
	}
	
LOCAL_D void CheckAllL(CBookmarkDb& aDb)
	{
	CBookmarkFolder& root = aDb.RootL();
	gTestWrapper->TEST(root.Count() == 1);
	
	CBookmarkFolder* folder = aDb.FindFolder(KTxtNewFolder);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 2);
	
	folder = aDb.FindFolder(KTxtNewFolder2);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 2);
	
	folder = aDb.FindFolder(KTxtNewFolder3);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 1);
	}

LOCAL_D void CheckAllMinusOneL(CBookmarkDb& aDb)
	{
	CBookmarkFolder& root = aDb.RootL();
	gTestWrapper->TEST(root.Count() == 3);
	
	CBookmarkFolder* folder = aDb.FindFolder(KTxtNewFolder);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 1);
	
	folder = aDb.FindFolder(KTxtNewFolder2);
	gTestWrapper->TEST(!folder);
	
	folder = aDb.FindFolder(KTxtNewFolder3);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 1);
	}

LOCAL_D void CheckPrivateL(CBookmarkDb& aDb)
	{
	CBookmarkFolder& root = aDb.RootL();
	gTestWrapper->TEST(root.Count() == 1);
	
	CBookmarkFolder* folder = aDb.FindFolder(KTxtNewFolder);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 1);
	
	folder = aDb.FindFolder(KTxtNewFolder2);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 2);
	
	folder = aDb.FindFolder(KTxtNewFolder3);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 0);
	}

LOCAL_D void CheckPublicL(CBookmarkDb& aDb)
	{
	CBookmarkFolder& root = aDb.RootL();
	gTestWrapper->TEST(root.Count() == 2);
	
	CBookmarkFolder* folder = aDb.FindFolder(KTxtNewFolder);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 2);
	
	folder = aDb.FindFolder(KTxtNewFolder2);
	gTestWrapper->TEST(folder != NULL);
	gTestWrapper->TEST(folder->Count() == 0);
	
	folder = aDb.FindFolder(KTxtNewFolder3);
	gTestWrapper->TEST(!folder);
	}

LOCAL_D void SecurityTestsL()
	{
	gTestWrapper->Next(_L("[CBookmarkDb tests] Security"));
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityManager, NULL);
	CleanupStack::PushL(db);
	CreateSecurityTreeL(*db);
	CheckAllL(*db);
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	db->ConstructTreeL();
	CheckAllL(*db);
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityPublic, NULL);
	CleanupStack::PushL(db);
	db->ConstructTreeL();
	CheckPublicL(*db);
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityPrivate, NULL);
	CleanupStack::PushL(db);
	db->ConstructTreeL();
	CheckPrivateL(*db);
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityManager, NULL);
	CleanupStack::PushL(db);
	db->ConstructTreeL();
	CBookmarkFolder* publicFolder = db->FindFolder(KTxtNewFolder2);
	publicFolder->SetOwnerL(0x00001234); 
	db->CommitL();
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityAll, NULL);
	CleanupStack::PushL(db);
	db->ConstructTreeL();
	CheckAllMinusOneL(*db);
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityManager, NULL);
	CleanupStack::PushL(db);
	db->ConstructTreeL();
	CheckAllL(*db);
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	db->ConstructTreeL();
	CBookmarkFolder* folder = db->FindFolder(KTxtNewFolder);
	gTestWrapper->TEST(folder != NULL);
	db->DeleteItemL(folder->Id(), ETrue);
	db->CommitL();
	CleanupStack::PopAndDestroy(db);
	}
	
void DoCBookmarkDbUnitL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** CBookmarkDb tests ***"));
	
	GeneralTestsL();
	TreeTestsL();
	CommitTestsL();
	IconDataTestsL();
	SecurityTestsL();
	}
