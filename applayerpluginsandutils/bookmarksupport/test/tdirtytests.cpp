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

_LIT(KFilename, "tdirtytests.cpp");

LOCAL_D CTestWrapper* gTestWrapper;

_LIT(KTxtTester1, 		"DirtyTEST Tester 1");

_LIT(KTxtFolder1, 		"DirtyTEST Folder 1");

_LIT(KTxtBookmark1,		"DirtyTEST Bookmark 1");
_LIT(KTxtBookmarkMod,	"DirtyTEST Bookmark Modified");

_LIT(KTxtDes1,			"This is DirtyTEST Bookmark 1");
_LIT(KTxtDesMod,		"This is a modified description");

_LIT8(KIconData1,		"****4644372131**7*&879");

_LIT(KTxtDesc, "This is the home page text");
_LIT8(KTxtUri, "sip:user:pass@location.com:1666;transport=tcp;x=123;df=223?asd=b");
_LIT8(KTxtUser, "user");
_LIT8(KTxtPassword, "pass");
_LIT8(KTxtNewUser, "user2");
_LIT8(KTxtNewPassword, "password");

const TUint32 KProxyId 	= 0x7CFFFFFF;
const TUint32 KNapId 	= 0x6CFFF000;

const TInt KMaxTestLength = 50;
_LIT8(KData8Bit1, "8BitData");
_LIT(KData16Bit1, "16BitData");
const TInt KInt1 = 999;
const TReal KReal1 = 42.999;

const TUid KUid1 = {0x111};
const TUid KUid2 = {0x222};
const TUid KUid3 = {0x333};
const TUid KUid4 = {0x444};
const TUid KUid5 = {0x555};
const TUid KUid6 = {0x666};
const TUid KUid7 = {0x777};
const TUid KUid8 = {0x888};
const TUid KUid9 = {0x999};
const TUid KUid10 = {0xAAA};
const TUid KUid11 = {0xBBB};
const TUid KUid12 = {0xCCC};

class CDirtyTester: public CBase
	{
public:
	static CDirtyTester* NewL(const TDesC& aTitle);
	~CDirtyTester();

	void SetupL();
	void ShutdownL();
	void DeleteBookmarksL();

	void OpenAllL(Bookmark::TVisibility aVisibility = Bookmark::EVisibilityDefault);
	void CloseAll();

	void CheckTitleL();
	void CheckDescriptionL();
	void CheckTimeL();
	void CheckIconL();
	void CheckPublicWritableL();
	void CheckConfigL();
	void CheckCustomPropertiesL();
	void CheckAuthenticationL();

private:
	CDirtyTester();
	void ConstructL(const TDesC& aTitle);

private:
	HBufC*			  	iTitle;
	RBkDatabase	iDatabase;
	RBkFolder		iRoot;
	RBkBookmark			iBookmark;
	RBkFolder		iFolder;
	};

CDirtyTester* CDirtyTester::NewL(const TDesC& aTitle)
	{
	CDirtyTester* self = new (ELeave) CDirtyTester();
	CleanupStack::PushL(self);
	self->ConstructL(aTitle);
	CleanupStack::Pop(self);
	return self;
	}

CDirtyTester::CDirtyTester()
	{
	}

CDirtyTester::~CDirtyTester()
	{
	delete iTitle;
	}


void CDirtyTester::ConstructL(const TDesC& aTitle)
	{
	iTitle = aTitle.AllocL();
	}

void CDirtyTester::OpenAllL(Bookmark::TVisibility aVisibility)
	{
	iDatabase.OpenL(aVisibility);
	iRoot = iDatabase.OpenRootL();

	RBkNode item = iRoot.OpenItemL(0);
	CleanupClosePushL(item);
	gTestWrapper->TEST(item.Type() == Bookmark::ETypeBookmark);
	iBookmark = item.OpenBookmarkL();
	CleanupStack::PopAndDestroy(&item);

	if (iRoot.Count() > 1)
		{
		item = iRoot.OpenItemL(1);
		CleanupClosePushL(item);
		gTestWrapper->TEST(item.Type() == Bookmark::ETypeFolder);
		iFolder = item.OpenFolderL();
		CleanupStack::PopAndDestroy(&item);
		}
	}

void CDirtyTester::CloseAll()
	{
	iBookmark.Close();
	if (iRoot.Count() > 1)
		{
		iFolder.Close();
		}
	iRoot.Close();
	iDatabase.Close();
	}

void CDirtyTester::SetupL()
	{
	iDatabase.OpenL(Bookmark::EVisibilityDefault);
	iRoot = iDatabase.OpenRootL();
	iBookmark = iDatabase.CreateBookmarkL();
	iFolder = iDatabase.CreateFolderL(KTxtFolder1);
	}

void CDirtyTester::ShutdownL()
	{
	iBookmark.Close();
	iFolder.Close();
	DeleteBookmarksL();
	iRoot.Close();
	iDatabase.Close();
	}

void CDirtyTester::CheckTitleL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Check Title"));

	SetupL();
	iBookmark.SetTitleL(KTxtBookmark1);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.Title().Compare(KTxtBookmark1) == 0);
	iBookmark.SetTitleL(KTxtBookmarkMod);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.Title().Compare(KTxtBookmarkMod) == 0);
	ShutdownL();
	}

void CDirtyTester::CheckDescriptionL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Check Description"));

	SetupL();
	iBookmark.SetDescriptionL(KTxtDes1);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.DescriptionL().Compare(KTxtDes1) == 0);
	iBookmark.SetDescriptionL(KTxtDesMod);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.DescriptionL().Compare(KTxtDesMod) == 0);
	ShutdownL();
	}

void CDirtyTester::CheckTimeL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Check Time"));

	SetupL();
	iBookmark.UpdateVisited();
	TTime firstUpdate = iBookmark.LastVisited();
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.LastVisited() == firstUpdate);
	iBookmark.UpdateVisited();
	TTime nextUpdate = iBookmark.LastVisited();
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.LastVisited() == nextUpdate);
	TRAPD(err, iBookmark.SetLastVisitedL(firstUpdate));
	gTestWrapper->TESTE(err == KErrPermissionDenied, err);
	iDatabase.CommitL();
	ShutdownL();
	}

void CDirtyTester::CheckIconL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Check Icon"));

	SetupL();
	Bookmark::TAttachmentId iconId = iDatabase.CreateIconL(KIconData1);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	TRAPD(err, iDatabase.GetIconL(iconId));
	gTestWrapper->TESTE(err == KErrNone, err);
	iBookmark.SetIconIdL(iconId);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.IconIdL() == iconId);
	User::LeaveIfError(iDatabase.DeleteIconL(iconId));
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	TRAP(err, iDatabase.GetIconL(iconId));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	ShutdownL();
	}

void CDirtyTester::CheckPublicWritableL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Check Public & Writable"));

	SetupL();
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	iBookmark.SetPublicL(EFalse);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(!iBookmark.IsPublic());
	RThread thread;
	TSecurityInfo secInfo(thread);
	TRAPD(err, iBookmark.OwnerL());
	gTestWrapper->TESTE(err == KErrPermissionDenied, err);
	TRAP(err, iBookmark.SetOwnerL(0x00112233));
	gTestWrapper->TESTE(err == KErrPermissionDenied, err);
	TRAP(err, iBookmark.SetWritableL(EFalse));
	gTestWrapper->TESTE(err == KErrNone, err);
	ShutdownL();
	}

void CDirtyTester::CheckConfigL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Check Database Config Properties"));

	SetupL();
	RBkBookmark homepage;
	TRAPD(err, homepage = iDatabase.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNotFound, err);
	gTestWrapper->TEST(iDatabase.HomePageText().Compare(KTxtDesc) != 0);
	gTestWrapper->TEST(iDatabase.SearchUri().Compare(KTxtUri) != 0);
	gTestWrapper->TEST(iDatabase.DefaultProxy() != KProxyId);
	gTestWrapper->TEST(iDatabase.DefaultNap() != KNapId);
	iDatabase.SetHomePageTextL(KTxtDesc);
	iDatabase.SetHome(iBookmark);
	iDatabase.SetSearchUriL(KTxtUri);
	iDatabase.SetDefaultProxy(KProxyId);
	iDatabase.SetDefaultNap(KNapId);
	gTestWrapper->TEST(iDatabase.HomePageText().Compare(KTxtDesc) == 0);
	TRAP(err, homepage = iDatabase.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNone, err);
	homepage.Close();
	gTestWrapper->TEST(iBookmark.IsHomePage());
	gTestWrapper->TEST(iDatabase.SearchUri().Compare(KTxtUri) == 0);
	gTestWrapper->TEST(iDatabase.DefaultProxy() == KProxyId);
	gTestWrapper->TEST(iDatabase.DefaultNap() == KNapId);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iDatabase.HomePageText().Compare(KTxtDesc) == 0);
	TRAP(err, homepage = iDatabase.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNone, err);
	homepage.Close();
	gTestWrapper->TEST(iBookmark.IsHomePage());
	gTestWrapper->TEST(iDatabase.SearchUri().Compare(KTxtUri) == 0);
	gTestWrapper->TEST(iDatabase.DefaultProxy() == KProxyId);
	gTestWrapper->TEST(iDatabase.DefaultNap() == KNapId);
	CloseAll();
	OpenAllL(Bookmark::EVisibilityPrivate);
	iDatabase.SetHomePageTextL(KNullDesC);
	iDatabase.SetSearchUriL(KNullDesC8);
	iDatabase.SetDefaultProxy(0);
	iDatabase.SetDefaultNap(0);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iDatabase.HomePageText().Compare(KTxtDesc) != 0);
	gTestWrapper->TEST(iDatabase.SearchUri().Compare(KTxtUri) != 0);
	gTestWrapper->TEST(iDatabase.DefaultProxy() != KProxyId);
	gTestWrapper->TEST(iDatabase.DefaultNap() != KNapId);
	ShutdownL();
	}

void CDirtyTester::CheckCustomPropertiesL()
	{
	TInt retInt = 0;
	TReal retReal = 0.0;
	HBufC* retDesC16 = HBufC::NewLC(KMaxTestLength);
	TPtr retPtr16 = retDesC16->Des();
	HBufC8* retDesC8 = HBufC8::NewLC(KMaxTestLength * 2);
	TPtr8 retPtr8 = retDesC8->Des();

	gTestWrapper->Next(_L("[Dirty tests] Custom Properties"));

	SetupL();

	TRAPD(err, iDatabase.RegisterDatabasePropertyL(KUid1, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid2, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid3, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid4, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid5, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid6, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid7, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid8, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid9, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid10, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid11, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid12, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == KErrNone, err);

	iDatabase.CommitL();
	CloseAll();
	OpenAllL();

	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid1, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid2, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid3, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid4, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);

	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid5, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid6, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid7, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid8, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);

	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid9, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid10, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid11, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid12, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == Bookmark::KErrUidAlreadyUsed, err);

	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid2));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid3));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid4));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid5));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid6));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid7));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid8));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid9));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid10));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid11));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid12));
	gTestWrapper->TESTE(err == KErrNone, err);

	iDatabase.CommitL();
	CloseAll();
	OpenAllL();

	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid1, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid2, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid3, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterDatabasePropertyL(KUid4, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid5, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid6, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid7, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterBookmarkPropertyL(KUid8, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid9, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid10, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid11, Bookmark::EDataTypeDes8));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.RegisterFolderPropertyL(KUid12, Bookmark::EDataTypeDes16));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.SetCustomPropertyL(KUid1, KInt1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.SetCustomPropertyL(KUid2, KReal1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.SetCustomPropertyL(KUid3, KData8Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.SetCustomPropertyL(KUid4, KData16Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iBookmark.SetCustomPropertyL(KUid5, KInt1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iBookmark.SetCustomPropertyL(KUid6, KReal1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iBookmark.SetCustomPropertyL(KUid7, KData8Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iBookmark.SetCustomPropertyL(KUid8, KData16Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iFolder.SetCustomPropertyL(KUid9, KInt1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iFolder.SetCustomPropertyL(KUid10, KReal1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iFolder.SetCustomPropertyL(KUid11, KData8Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iFolder.SetCustomPropertyL(KUid12, KData16Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);

	iDatabase.CommitL();
	CloseAll();
	OpenAllL();

	TRAP(err, iDatabase.GetCustomPropertyL(KUid1, retInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retInt == KInt1);
	TRAP(err, iDatabase.GetCustomPropertyL(KUid2, retReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retReal == KReal1);
	TRAP(err, iDatabase.GetCustomPropertyL(KUid3, retPtr8));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retPtr8.Compare(KData8Bit1) == 0);
	TRAP(err, iDatabase.GetCustomPropertyL(KUid4, retPtr16));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retPtr16.Compare(KData16Bit1) == 0);

	TRAP(err, iBookmark.GetCustomPropertyL(KUid5, retInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retInt == KInt1);
	TRAP(err, iBookmark.GetCustomPropertyL(KUid6, retReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retReal == KReal1);
	TRAP(err, iBookmark.GetCustomPropertyL(KUid7, retPtr8));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retPtr8.Compare(KData8Bit1) == 0);
	TRAP(err, iBookmark.GetCustomPropertyL(KUid8, retPtr16));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retPtr16.Compare(KData16Bit1) == 0);

	TRAP(err, iFolder.GetCustomPropertyL(KUid9, retInt));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retInt == KInt1);
	TRAP(err, iFolder.GetCustomPropertyL(KUid10, retReal));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retReal == KReal1);
	TRAP(err, iFolder.GetCustomPropertyL(KUid11, retPtr8));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retPtr8.Compare(KData8Bit1) == 0);
	TRAP(err, iFolder.GetCustomPropertyL(KUid12, retPtr16));
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retPtr16.Compare(KData16Bit1) == 0);

	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid2));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid3));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterDatabasePropertyL(KUid4));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid5));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid6));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid7));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterBookmarkPropertyL(KUid8));
	gTestWrapper->TESTE(err == KErrNone, err);

	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid9));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid10));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid11));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, iDatabase.DeregisterFolderPropertyL(KUid12));
	gTestWrapper->TESTE(err == KErrNone, err);

	iDatabase.CommitL();
	CloseAll();
	OpenAllL();

	TRAP(err, iDatabase.GetCustomPropertyL(KUid1, retInt));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iDatabase.GetCustomPropertyL(KUid2, retReal));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iDatabase.GetCustomPropertyL(KUid3, retPtr8));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iDatabase.GetCustomPropertyL(KUid4, retPtr16));
	gTestWrapper->TESTE(err == KErrNotFound, err);

	TRAP(err, iBookmark.GetCustomPropertyL(KUid5, retInt));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iBookmark.GetCustomPropertyL(KUid6, retReal));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iBookmark.GetCustomPropertyL(KUid7, retPtr8));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iBookmark.GetCustomPropertyL(KUid8, retPtr16));
	gTestWrapper->TESTE(err == KErrNotFound, err);

	TRAP(err, iFolder.GetCustomPropertyL(KUid9, retInt));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iFolder.GetCustomPropertyL(KUid10, retReal));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iFolder.GetCustomPropertyL(KUid11, retPtr8));
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, iFolder.GetCustomPropertyL(KUid12, retPtr16));
	gTestWrapper->TESTE(err == KErrNotFound, err);

	ShutdownL();

	CleanupStack::PopAndDestroy(retDesC8);
	CleanupStack::PopAndDestroy(retDesC16);
	}

void CDirtyTester::CheckAuthenticationL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Check Authenication"));

	SetupL();
	iBookmark.SetUriL(KTxtUri);
	gTestWrapper->TEST(iBookmark.AuthenticationL().Name().Compare(KTxtUser) == 0);
	gTestWrapper->TEST(iBookmark.AuthenticationL().Password().Compare(KTxtPassword) == 0);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.AuthenticationL().Name().Compare(KTxtUser) == 0);
	gTestWrapper->TEST(iBookmark.AuthenticationL().Password().Compare(KTxtPassword) == 0);
	CAuthentication* newAuth = CAuthentication::NewL(KTxtNewUser(), KTxtNewPassword());
	CleanupStack::PushL(newAuth);
	iBookmark.SetAuthenticationL(*newAuth);
	CleanupStack::PopAndDestroy(newAuth);
	gTestWrapper->TEST(iBookmark.AuthenticationL().Name().Compare(KTxtNewUser) == 0);
	gTestWrapper->TEST(iBookmark.AuthenticationL().Password().Compare(KTxtNewPassword) == 0);
	iDatabase.CommitL();
	CloseAll();
	OpenAllL();
	gTestWrapper->TEST(iBookmark.AuthenticationL().Name().Compare(KTxtNewUser) == 0);
	gTestWrapper->TEST(iBookmark.AuthenticationL().Password().Compare(KTxtNewPassword) == 0);
	ShutdownL();
	}

void SmallDatabaseTestL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Small custom database property test"));

	RBkDatabase db;
	db.OpenL();
	CleanupClosePushL(db);

	TUid myuid = TUid::Uid(0x1234a);
	TRAPD(error, db.RegisterDatabasePropertyL(myuid, Bookmark::EDataTypeInt));
	gTestWrapper->TESTE(error == KErrNone, error);
	db.CommitL();
	db.Close();

	TRAP(error, db.OpenL());
	gTestWrapper->TESTE(error == KErrNone, error);
	TUid myuid2  = TUid::Uid(0x1234b);
	TRAP(error, db.RegisterDatabasePropertyL(myuid2, Bookmark::EDataTypeReal));
	gTestWrapper->TESTE(error == KErrNone, error);
	db.CommitL();
	db.Close();

	TRAP(error, db.OpenL());
	gTestWrapper->TESTE(error == KErrNone, error);
	TRAP(error, db.SetCustomPropertyL(myuid, 10));
	gTestWrapper->TESTE(error == KErrNone, error);
	db.CommitL();
	db.Close();

	TRAP(error, db.OpenL());
	gTestWrapper->TESTE(error == KErrNone, error);
	TReal real = 234.1234;
	TRAP(error, db.SetCustomPropertyL(myuid2, real));
	gTestWrapper->TESTE(error == KErrNone, error);
	db.CommitL();
	db.Close();

	TRAP(error, db.OpenL());
	gTestWrapper->TESTE(error == KErrNone, error);
	TInt var = 0;
	TRAP(error, db.GetCustomPropertyL(myuid, var));
	gTestWrapper->TESTE(error == KErrNone, error);
	gTestWrapper->TEST(var == 10);
	db.CommitL();
	db.Close();

	TRAP(error, db.OpenL());
	gTestWrapper->TESTE(error == KErrNone, error);
	TReal realvar = 0;
	TRAP(error, db.GetCustomPropertyL(myuid2, realvar));
	gTestWrapper->TESTE(error == KErrNone, error);
	gTestWrapper->TEST(var == 10);
	db.CommitL();
	db.Close();

	TRAP(error, db.OpenL());
	gTestWrapper->TESTE(error == KErrNone, error);
	TRAP(error, db.DeregisterDatabasePropertyL(myuid));
	gTestWrapper->TESTE(error == KErrNone, error);
	db.CommitL();
	db.Close();

	TRAP(error, db.OpenL());
	gTestWrapper->TESTE(error == KErrNone, error);
	TRAP(error, db.DeregisterDatabasePropertyL(myuid2));
	gTestWrapper->TESTE(error == KErrNone, error);
	db.CommitL();

	CleanupStack::PopAndDestroy(&db);
	}

void DeleteHomePageTestL()
	{
	gTestWrapper->Next(_L("[Dirty tests] Fix: DEF057872 Bookmarks home page is not updated when the home page bookmark is deleted"));
	
	RBkDatabase db;
	db.OpenL();
	CleanupClosePushL(db);
	
	RBkBookmark homepage;
	TRAPD(err, homepage = db.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNotFound, err);
	homepage = db.CreateBookmarkL();
	db.SetHome(homepage);
	homepage.Close();
	TRAP(err, homepage = db.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNone, err);
	homepage.Close();
	db.CommitL();
	db.Close();
	
	TRAP(err, db.OpenL());
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, homepage = db.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNone, err);
	Bookmark::TItemId homeId = homepage.Id();
	homepage.Close();
	TRAP(err, db.DeleteItemL(homeId));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, homepage = db.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNotFound, err);
	db.CommitL();
	db.Close();
	
	TRAP(err, db.OpenL());
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, homepage = db.OpenHomeL());
	gTestWrapper->TESTE(err == KErrNotFound, err);
	
	CleanupStack::PopAndDestroy(&db);
	}
	
void CDirtyTester::DeleteBookmarksL()
	{
	RBkNode item;
	TInt index = iRoot.Count() -1;
	for (; index >= 0; --index)
		{
		item = iRoot.OpenItemL(index);
		CleanupClosePushL(item);
		iDatabase.DeleteItemL(item.Id(), ETrue);
		CleanupStack::PopAndDestroy(&item);
		}
	iDatabase.CommitL();
	}

void DoDirtyTestsL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** Dirty tests ***"));

	CDirtyTester* tester = CDirtyTester::NewL(KTxtTester1);
	gTestWrapper->TEST(tester != NULL);
	CleanupStack::PushL(tester);

	tester->CheckTitleL();
	tester->CheckDescriptionL();
	tester->CheckTimeL();
	tester->CheckAuthenticationL();
	tester->CheckIconL();
	tester->CheckPublicWritableL();
	tester->CheckCustomPropertiesL();
	tester->CheckConfigL();

	CleanupStack::PopAndDestroy(tester);

	SmallDatabaseTestL();
	DeleteHomePageTestL();
	}


