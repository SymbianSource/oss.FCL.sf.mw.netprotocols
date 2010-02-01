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

_LIT(KFilename, "tnotifytests.cpp");

LOCAL_D CTestWrapper* gTestWrapper;
LOCAL_D Bookmark::TAttachmentId gIconId;

_LIT(KTxtTester1, 		"NotifyTEST Tester 1");
_LIT(KTxtTester2, 		"NotifyTEST Tester 2");

_LIT(KTxtFolder1, 		"NotifyTEST Folder 1");
_LIT(KTxtFolder1_1, 	"NotifyTEST Folder 1.1");
_LIT(KTxtFolder1_2, 	"NotifyTEST Folder 1.2");
_LIT(KTxtFolder1_2_1, 	"NotifyTEST Folder 1.2.1");
_LIT(KTxtFolder1_2_1_1,	"NotifyTEST Folder 1.2.1.1");

_LIT(KTxtBookmark1,		"NotifyTEST Bookmark 1");
_LIT(KTxtBookmark2,		"NotifyTEST Bookmark 2");
_LIT(KTxtBookmark3, 	"NotifyTEST Bookmark 3");
_LIT(KTxtBookmark4, 	"NotifyTEST Bookmark 4");
_LIT(KTxtBookmark5, 	"NotifyTEST Bookmark 5");
_LIT(KTxtBookmark6, 	"NotifyTEST Bookmark 6");
_LIT(KTxtBookmark7, 	"NotifyTEST Bookmark 7");
_LIT(KTxtBookmark8, 	"NotifyTEST Bookmark 8");
_LIT(KTxtBookmark9, 	"NotifyTEST Bookmark 9");
_LIT(KTxtBookmark10, 	"NotifyTEST Bookmark 10");

_LIT8(KIconData,		"****4644372131**7*&879");
const TUint32 KProxyId 	= 0x7CFFFFFF;

/*
TInt gIndentLevel = 0;
TBufC<256> gBuffer;

LOCAL_D const TDesC& Indented(const TDesC& aText)
	{
	TPtr ptr = gBuffer.Des();
	ptr.SetLength(0);
	TInt dots = gIndentLevel - 1;
	for (;dots >= 0; --dots)
		{
		ptr.Append(_L(" . "));
		}
	ptr.Append(aText);
	
	return gBuffer;
	}
	
LOCAL_D void DumpTreeL(RBkFolder& aFolder)
	{
	const TDesC& title = aFolder.Title();
	gTestWrapper->Tester().Printf(Indented(title));

	++gIndentLevel;
	RBkNode item;
	TInt count = aFolder.Count();
	TInt index = 0;
	for (;index < count; ++index)
		{
		item = aFolder.OpenItemL(index);
		CleanupClosePushL(item);
		if (item.Type() == Bookmark::ETypeFolder)
			{
			RBkFolder folder;
			folder.OpenL(item);
			CleanupClosePushL(folder);
			DumpTreeL(folder);
			CleanupStack::PopAndDestroy(&folder);
			}
		else
			{
			gTestWrapper->Tester().Printf(Indented(item.Title()));
			}
		CleanupStack::PopAndDestroy(&item);
		}
	--gIndentLevel;
	}
*/
class CNotifyTester: public CActive, public MBookmarkObserver
	{
public:
	static CNotifyTester* NewL(const TDesC& aTitle);
	~CNotifyTester();
	
	void CreateInitialTreeL();
	void CheckInitialTreeL();
	
	void InitiateTest1L();
	void InitiateTest2L();
	void InitiateTest3L();
	void StartTestL();
	
	void DeleteBookmarksL();
	
protected:
	// From CActive
	void RunL();
	void DoCancel();
	
	// From MBookmarkObserver
	void MBONotify(TEvent aEvent);
	
private:
	CNotifyTester();
	void ConstructL(const TDesC& aTitle);
	
private:
	RBkDatabase iDatabase;
	HBufC*		iTitle;
	TInt		iCurrentTest;
	};

CNotifyTester* CNotifyTester::NewL(const TDesC& aTitle)
	{
	CNotifyTester* self = new (ELeave) CNotifyTester();
	CleanupStack::PushL(self);
	self->ConstructL(aTitle);
	CleanupStack::Pop(self);
	return self;
	}

CNotifyTester::CNotifyTester()
	: CActive(EPriorityStandard), iCurrentTest(1)
	{
	}
	
CNotifyTester::~CNotifyTester()
	{
	delete iTitle;
	Cancel();
	iDatabase.Close();
	}
	
void CNotifyTester::ConstructL(const TDesC& aTitle)
	{
	iTitle = aTitle.AllocL();
	CActiveScheduler::Add(this);
	iDatabase.OpenL(Bookmark::EVisibilityDefault, this);
	}
	
void CNotifyTester::RunL()
	{
	iDatabase.RefreshL();
	TInt err = KErrNone;
	switch (iCurrentTest)
		{
	case 1:
		gTestWrapper->Next(_L("Tree After Refresh"));
		TRAP(err, iDatabase.GetIconL(gIconId));
		gTestWrapper->TESTE(err == KErrNotFound, err);
		gTestWrapper->TEST(iDatabase.DefaultProxy() == 0);
		CheckInitialTreeL();
		break;
	case 2:
		gTestWrapper->Next(_L("Check Icon"));
		gTestWrapper->TEST(iDatabase.GetIconL(gIconId).Compare(KIconData) == 0);
		gTestWrapper->TEST(iDatabase.DefaultProxy() == 0);
		break;
	case 3:
		gTestWrapper->Next(_L("Check Config"));
		gTestWrapper->TEST(iDatabase.DefaultProxy() == KProxyId);
		break;
	default:
		// should not get here
		User::Leave(KErrUnknown);
		break;
		}
		
	++iCurrentTest;
    CActiveScheduler::Stop();
	}

void CNotifyTester::DoCancel()
	{
	}
	
void CNotifyTester::MBONotify(TEvent aEvent)
	{
	if (IsActive())
		{
		if ((aEvent == EEventUpdatedBookmarkTree && iCurrentTest == 1) ||
			(aEvent == EEventUpdatedIcons && iCurrentTest == 2 ) ||
			(aEvent == EEventUpdatedConfig && iCurrentTest == 3 ) )
			{
			TRequestStatus* reqStatus = &iStatus;
			User::RequestComplete(reqStatus, KErrNone);
			}
		}
	}
	
void CNotifyTester::InitiateTest1L()
	{
	iDatabase.SetDefaultProxy(0);
	CreateInitialTreeL();
	CheckInitialTreeL();
	iDatabase.CommitL();
	}

void CNotifyTester::InitiateTest2L()
	{
	gIconId = iDatabase.CreateIconL(KIconData);
	iDatabase.CommitL();
	}

void CNotifyTester::InitiateTest3L()
	{
	iDatabase.SetDefaultProxy(KProxyId);
	iDatabase.CommitL();
	}

void CNotifyTester::StartTestL()
	{
	if (IsActive())
		{
		return;
		}
	iStatus = KRequestPending;
	SetActive();
	}

void CNotifyTester::DeleteBookmarksL()
	{
	RBkFolder root = iDatabase.OpenRootL();
	CleanupClosePushL(root);
	RBkNode item;
	TInt index = root.Count() -1;
	for (; index >= 0; --index)
		{
		item = root.OpenItemL(index);
		CleanupClosePushL(item);
		iDatabase.DeleteItemL(item.Id(), ETrue);
		CleanupStack::PopAndDestroy(&item);
		}
	CleanupStack::PopAndDestroy(&root);
	
	User::LeaveIfError(iDatabase.DeleteIconL(gIconId));
	iDatabase.SetDefaultProxy(0);
	
	iDatabase.CommitL();
	
	}

/* 	
After a call to the the tree structure looks like:
			Root
			 |
[Bookmark1	Bookmark2	Folder1]
						  |
[Bookmark3	Bookmark4	Folder1_1	Folder1_2]
						  |		  	 |
					[Bookmark5]		[Bookmark6	Folder1_2_1]
									  			  |
								  			[Bookmark7	Folder1_2_1_1]
								  						  |
								  		[Bookmark8	Bookmark9	Bookmark10]

*/							  				 
void CNotifyTester::CreateInitialTreeL()
	{
	// Create the first two bookmarks in the root
	RBkBookmark bmrk = iDatabase.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark1);
	CleanupStack::PopAndDestroy(&bmrk);
	
	bmrk = iDatabase.CreateBookmarkL();
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark2);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create a folder and put 2 bookmarks into it
	RBkFolder folder = iDatabase.CreateFolderL(KTxtFolder1);
	CleanupClosePushL(folder);
	bmrk = iDatabase.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark3);
	CleanupStack::PopAndDestroy(&bmrk);
	bmrk = iDatabase.CreateBookmarkL(&folder);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark4);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create two more folders in this folder and place a bookmark in each one
	RBkFolder folderL2 = iDatabase.CreateFolderL(KTxtFolder1_1, &folder);
	CleanupClosePushL(folderL2);
	bmrk = iDatabase.CreateBookmarkL(&folderL2);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark5);
	CleanupStack::PopAndDestroy(&bmrk);
	CleanupStack::PopAndDestroy(&folderL2);
	
	folderL2 = iDatabase.CreateFolderL(KTxtFolder1_2, &folder);
	CleanupClosePushL(folderL2);
	bmrk = iDatabase.CreateBookmarkL(&folderL2);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark6);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create a folder under 1_2 and add a bookmark
	RBkFolder folderL3 = iDatabase.CreateFolderL(KTxtFolder1_2_1, &folderL2);
	CleanupClosePushL(folderL3);
	bmrk = iDatabase.CreateBookmarkL(&folderL3);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark7);
	CleanupStack::PopAndDestroy(&bmrk);
	
	// Create a folder under 1_2_1 and add 3 bookmarks
	RBkFolder folderL4 = iDatabase.CreateFolderL(KTxtFolder1_2_1_1, &folderL3);
	CleanupClosePushL(folderL4);
	bmrk = iDatabase.CreateBookmarkL(&folderL4);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark8);
	CleanupStack::PopAndDestroy(&bmrk);
	bmrk = iDatabase.CreateBookmarkL(&folderL4);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark9);
	CleanupStack::PopAndDestroy(&bmrk);
	bmrk = iDatabase.CreateBookmarkL(&folderL4);
	CleanupClosePushL(bmrk);
	bmrk.SetTitleL(KTxtBookmark10);
	CleanupStack::PopAndDestroy(&bmrk);
	
	
	CleanupStack::PopAndDestroy(&folderL4);
	CleanupStack::PopAndDestroy(&folderL3);
	CleanupStack::PopAndDestroy(&folderL2);
	CleanupStack::PopAndDestroy(&folder);
	}

void CNotifyTester::CheckInitialTreeL()
	{
	RBkFolder root = iDatabase.OpenRootL();
	CleanupClosePushL(root);
	gTestWrapper->TEST(root.Count() == 3);
	
	if (root.Count() != 3)
		{
		User::Leave(KErrNotFound);
		}
		
	RBkNode item = root.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Title().Compare(KTxtBookmark1) == 0);
	CleanupStack::PopAndDestroy(&item);
	
	item = root.OpenItemL(2);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Type() == Bookmark::ETypeFolder);
	RBkFolder folder1;
	folder1 = item.OpenFolderL();
	CleanupStack::PopAndDestroy(&item);
	CleanupClosePushL(folder1);
	
	item = folder1.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Title().Compare(KTxtBookmark3) == 0);
	CleanupStack::PopAndDestroy(&item);
	
	item = folder1.OpenItemL(2);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Type() == Bookmark::ETypeFolder);
	RBkFolder folder1_1;
	folder1_1 = item.OpenFolderL();
	CleanupStack::PopAndDestroy(&item);
	CleanupClosePushL(folder1_1);
	
	item = folder1_1.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Title().Compare(KTxtBookmark5) == 0);
	CleanupStack::PopAndDestroy(&item);
	
	CleanupStack::PopAndDestroy(&folder1_1);
	
	item = folder1.OpenItemL(3);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Type() == Bookmark::ETypeFolder);
	RBkFolder folder1_2;
	folder1_2 = item.OpenFolderL();
	CleanupStack::PopAndDestroy(&item);
	CleanupClosePushL(folder1_2);

	item = folder1_2.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Title().Compare(KTxtBookmark6) == 0);
	CleanupStack::PopAndDestroy(&item);
	
	item = folder1_2.OpenItemL(1);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Type() == Bookmark::ETypeFolder);
	RBkFolder folder1_2_1;
	folder1_2_1 = item.OpenFolderL();
	CleanupStack::PopAndDestroy(&item);
	CleanupClosePushL(folder1_2_1);
	
	item = folder1_2_1.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Title().Compare(KTxtBookmark7) == 0);
	CleanupStack::PopAndDestroy(&item);
	
	item = folder1_2_1.OpenItemL(1);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Type() == Bookmark::ETypeFolder);
	RBkFolder folder1_2_1_1;
	folder1_2_1_1 = item.OpenFolderL();
	CleanupStack::PopAndDestroy(&item);
	CleanupClosePushL(folder1_2_1_1);
	
	item = folder1_2_1_1.OpenItemL(2);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Title().Compare(KTxtBookmark10) == 0);
	CleanupStack::PopAndDestroy(&item);
	
	CleanupStack::PopAndDestroy(&folder1_2_1_1);
	CleanupStack::PopAndDestroy(&folder1_2_1);
	CleanupStack::PopAndDestroy(&folder1_2);
	CleanupStack::PopAndDestroy(&folder1);
	CleanupStack::PopAndDestroy(&root);
	}

void DoNotifyTestsL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** Notify tests ***"));
	
    // Create and install the active scheduler
    CActiveScheduler* scheduler=new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

	CNotifyTester* tester1 = CNotifyTester::NewL(KTxtTester1);
	gTestWrapper->TEST(tester1 != NULL);
	CleanupStack::PushL(tester1);
	
	CNotifyTester* tester2 = CNotifyTester::NewL(KTxtTester2);
	gTestWrapper->TEST(tester2 != NULL);
	CleanupStack::PushL(tester2);
	
	tester1->InitiateTest1L();
	tester2->StartTestL();

    // start the active scheduler
    CActiveScheduler::Start();

	tester1->InitiateTest2L();
	tester2->StartTestL();

    // start the active scheduler
    CActiveScheduler::Start();

	tester1->InitiateTest3L();
	tester2->StartTestL();

    // start the active scheduler
    CActiveScheduler::Start();

	tester1->DeleteBookmarksL();
	
	CleanupStack::PopAndDestroy(tester2);
	CleanupStack::PopAndDestroy(tester1);
	CleanupStack::PopAndDestroy(scheduler);
	}
