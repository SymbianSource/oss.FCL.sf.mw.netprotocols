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
#include "propertyreg.h"
#include "bkmrkdb.h"
#include "bkmrk.h"
#include "bkmrkfolder.h"

LOCAL_D CTestWrapper* gTestWrapper;

_LIT(KFilename, "t_cpropertyreg.cpp");

_LIT(KTxtFolder, "propertyTEST folder 1");

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

LOCAL_D void GeneralTestsL()
	{
	CPropertyReg* propRegister = CPropertyReg::NewL();
	CleanupStack::PushL(propRegister);

	propRegister->RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeInt);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid2, Bookmark::EDataTypeReal);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid3, Bookmark::EDataTypeDes8);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid4, Bookmark::EDataTypeDes16);
	
	propRegister->RegisterPropertyL(CPropertyReg::EGroupFolder, KUid5, Bookmark::EDataTypeInt);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupFolder, KUid6, Bookmark::EDataTypeReal);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupFolder, KUid7, Bookmark::EDataTypeDes8);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupFolder, KUid8, Bookmark::EDataTypeDes16);
	
	propRegister->RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid9, Bookmark::EDataTypeInt);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid10, Bookmark::EDataTypeReal);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid11, Bookmark::EDataTypeDes8);
	propRegister->RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid12, Bookmark::EDataTypeDes16);
	
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeInt) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid2, Bookmark::EDataTypeReal) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid3, Bookmark::EDataTypeDes8) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid4, Bookmark::EDataTypeDes16) != KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid5, Bookmark::EDataTypeInt) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid6, Bookmark::EDataTypeReal) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid7, Bookmark::EDataTypeDes8) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid8, Bookmark::EDataTypeDes16) != KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid9, Bookmark::EDataTypeInt) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid10, Bookmark::EDataTypeReal) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid11, Bookmark::EDataTypeDes8) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid12, Bookmark::EDataTypeDes16) != KErrNotFound);
	
	// some negative cases
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeDes16) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid1, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid1, Bookmark::EDataTypeInt) == KErrNotFound);
	
	propRegister->CommitL();
	CleanupStack::PopAndDestroy(propRegister);
	propRegister = CPropertyReg::NewL();
	CleanupStack::PushL(propRegister);
	
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeInt) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid2, Bookmark::EDataTypeReal) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid3, Bookmark::EDataTypeDes8) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid4, Bookmark::EDataTypeDes16) != KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid5, Bookmark::EDataTypeInt) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid6, Bookmark::EDataTypeReal) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid7, Bookmark::EDataTypeDes8) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid8, Bookmark::EDataTypeDes16) != KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid9, Bookmark::EDataTypeInt) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid10, Bookmark::EDataTypeReal) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid11, Bookmark::EDataTypeDes8) != KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid12, Bookmark::EDataTypeDes16) != KErrNotFound);
	
	// some negative cases
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeDes16) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid1, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid1, Bookmark::EDataTypeInt) == KErrNotFound);
	
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid1));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid2));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid3));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid4));
	
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid5));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid6));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid7));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid8));
	
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid9));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid10));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid11));
	User::LeaveIfError(propRegister->DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid12));
	
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid2, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid3, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid4, Bookmark::EDataTypeDes16) == KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid5, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid6, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid7, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid8, Bookmark::EDataTypeDes16) == KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid9, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid10, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid11, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid12, Bookmark::EDataTypeDes16) == KErrNotFound);
	
	propRegister->CommitL();
	CleanupStack::PopAndDestroy(propRegister);
	propRegister = CPropertyReg::NewL();
	CleanupStack::PushL(propRegister);
	
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid2, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid3, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupDatabase, KUid4, Bookmark::EDataTypeDes16) == KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid5, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid6, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid7, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupFolder, KUid8, Bookmark::EDataTypeDes16) == KErrNotFound);

	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid9, Bookmark::EDataTypeInt) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid10, Bookmark::EDataTypeReal) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid11, Bookmark::EDataTypeDes8) == KErrNotFound);
	gTestWrapper->TEST(propRegister->PropertyIndex(CPropertyReg::EGroupBookmark, KUid12, Bookmark::EDataTypeDes16) == KErrNotFound);
	
	CleanupStack::PopAndDestroy(propRegister);
	}

LOCAL_D void DatabasePropertyTestsL()
	{
	TInt retIntVal;
	TReal retRealVal;
	HBufC* buf16 = HBufC::NewLC(Bookmark::KMaxDescriptorLength);
	TPtr buf16Ptr(buf16->Des());
	HBufC8* buf8 = HBufC8::NewLC(Bookmark::KMaxDataLength);
	TPtr8 buf8Ptr(buf8->Des());

	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CPropertyReg& propRegister = db->PropertyRegister();
		
	propRegister.RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid1, Bookmark::EDataTypeInt);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid2, Bookmark::EDataTypeReal);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid3, Bookmark::EDataTypeDes8);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupDatabase, KUid4, Bookmark::EDataTypeDes16);
	
	TInt err = db->CustomProperties().GetCustomProperty(KUid1, retIntVal);
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, db->CustomProperties().SetCustomPropertyL(KUid5, KInt1));
	gTestWrapper->TESTE(err == Bookmark::KErrNotRegistered, err);
	
	TRAP(err, db->CustomProperties().SetCustomPropertyL(KUid1, KInt1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, db->CustomProperties().SetCustomPropertyL(KUid2, KReal1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, db->CustomProperties().SetCustomPropertyL(KUid3, KData8Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, db->CustomProperties().SetCustomPropertyL(KUid4, KData16Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	
	err = db->CustomProperties().GetCustomProperty(KUid1, retIntVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retIntVal == KInt1);
	err = db->CustomProperties().GetCustomProperty(KUid2, retRealVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retRealVal == KReal1);
	err = db->CustomProperties().GetCustomProperty(KUid3, buf8Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf8Ptr.Compare(KData8Bit1) == 0);
	err = db->CustomProperties().GetCustomProperty(KUid4, buf16Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf16Ptr.Compare(KData16Bit1) == 0);
	
	db->CommitL();
	
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	err = db->CustomProperties().GetCustomProperty(KUid1, retIntVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retIntVal == KInt1);
	err = db->CustomProperties().GetCustomProperty(KUid2, retRealVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retRealVal == KReal1);
	err = db->CustomProperties().GetCustomProperty(KUid3, buf8Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf8Ptr.Compare(KData8Bit1) == 0);
	err = db->CustomProperties().GetCustomProperty(KUid4, buf16Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf16Ptr.Compare(KData16Bit1) == 0);
	
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid1));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid2));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid3));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupDatabase, KUid4));
	
	db->CommitL();
	
	CleanupStack::PopAndDestroy(3);
	}

LOCAL_D void BookmarkPropertyTestsL()
	{
	TInt retIntVal;
	TReal retRealVal;
	HBufC* buf16 = HBufC::NewLC(Bookmark::KMaxDescriptorLength);
	TPtr buf16Ptr(buf16->Des());
	HBufC8* buf8 = HBufC8::NewLC(Bookmark::KMaxDataLength);
	TPtr8 buf8Ptr(buf8->Des());

	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CPropertyReg& propRegister = db->PropertyRegister();
		
	propRegister.RegisterPropertyL(CPropertyReg::EGroupFolder, KUid5, Bookmark::EDataTypeInt);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupFolder, KUid6, Bookmark::EDataTypeReal);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupFolder, KUid7, Bookmark::EDataTypeDes8);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupFolder, KUid8, Bookmark::EDataTypeDes16);
	
	propRegister.RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid9, Bookmark::EDataTypeInt);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid10, Bookmark::EDataTypeReal);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid11, Bookmark::EDataTypeDes8);
	propRegister.RegisterPropertyL(CPropertyReg::EGroupBookmark, KUid12, Bookmark::EDataTypeDes16);
	
	CBookmarkFolder& folder = db->CreateFolderL(KTxtFolder, db->RootL());
	CBookmark& bookmark = db->CreateBookmarkL(folder);
	
	CBkmrkProperties& fldProperties = folder.BkmrkPropertiesL();
	TInt err = fldProperties.CustomProperties().GetCustomProperty(KUid5, retIntVal);
	gTestWrapper->TESTE(err == KErrNotFound, err);
	TRAP(err, fldProperties.CustomProperties().SetCustomPropertyL(KUid1, KInt1));
	gTestWrapper->TESTE(err == Bookmark::KErrNotRegistered, err);
	
	TRAP(err, fldProperties.CustomProperties().SetCustomPropertyL(KUid5, KInt1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, fldProperties.CustomProperties().SetCustomPropertyL(KUid6, KReal1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, fldProperties.CustomProperties().SetCustomPropertyL(KUid7, KData8Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, fldProperties.CustomProperties().SetCustomPropertyL(KUid8, KData16Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	
	err = fldProperties.CustomProperties().GetCustomProperty(KUid5, retIntVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retIntVal == KInt1);
	err = fldProperties.CustomProperties().GetCustomProperty(KUid6, retRealVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retRealVal == KReal1);
	err = fldProperties.CustomProperties().GetCustomProperty(KUid7, buf8Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf8Ptr.Compare(KData8Bit1) == 0);
	err = fldProperties.CustomProperties().GetCustomProperty(KUid8, buf16Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf16Ptr.Compare(KData16Bit1) == 0);
	
	CBkmrkExtendedProperties& properties = bookmark.BkmrkExtendedPropertiesL();
	err = properties.CustomProperties().GetCustomProperty(KUid9, retIntVal);
	gTestWrapper->TESTE(err == KErrNotFound, err);
	
	TRAP(err, properties.CustomProperties().SetCustomPropertyL(KUid9, KInt1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, properties.CustomProperties().SetCustomPropertyL(KUid10, KReal1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, properties.CustomProperties().SetCustomPropertyL(KUid11, KData8Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	TRAP(err, properties.CustomProperties().SetCustomPropertyL(KUid12, KData16Bit1));
	gTestWrapper->TESTE(err == KErrNone, err);
	
	err = properties.CustomProperties().GetCustomProperty(KUid9, retIntVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retIntVal == KInt1);
	err = properties.CustomProperties().GetCustomProperty(KUid10, retRealVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retRealVal == KReal1);
	err = properties.CustomProperties().GetCustomProperty(KUid11, buf8Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf8Ptr.Compare(KData8Bit1) == 0);
	err = properties.CustomProperties().GetCustomProperty(KUid12, buf16Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf16Ptr.Compare(KData16Bit1) == 0);
	
	db->CommitL();
	
	CleanupStack::PopAndDestroy(db);
	
	db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CBookmarkFolder& folder2 = static_cast<CBookmarkFolder&>(db->RootL().At(1));
	CBkmrkProperties& fldProperties2 = folder2.BkmrkPropertiesL();
	
	err = fldProperties2.CustomProperties().GetCustomProperty(KUid5, retIntVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retIntVal == KInt1);
	err = fldProperties2.CustomProperties().GetCustomProperty(KUid6, retRealVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retRealVal == KReal1);
	err = fldProperties2.CustomProperties().GetCustomProperty(KUid7, buf8Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf8Ptr.Compare(KData8Bit1) == 0);
	err = fldProperties2.CustomProperties().GetCustomProperty(KUid8, buf16Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf16Ptr.Compare(KData16Bit1) == 0);
	
	CBookmark& bookmark2 = static_cast<CBookmark&>(folder2.At(0));
	CBkmrkExtendedProperties& properties2 = bookmark2.BkmrkExtendedPropertiesL();
	
	err = properties2.CustomProperties().GetCustomProperty(KUid9, retIntVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retIntVal == KInt1);
	err = properties2.CustomProperties().GetCustomProperty(KUid10, retRealVal);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(retRealVal == KReal1);
	err = properties2.CustomProperties().GetCustomProperty(KUid11, buf8Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf8Ptr.Compare(KData8Bit1) == 0);
	err = properties2.CustomProperties().GetCustomProperty(KUid12, buf16Ptr);
	gTestWrapper->TESTE(err == KErrNone, err);
	gTestWrapper->TEST(buf16Ptr.Compare(KData16Bit1) == 0);
	
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid5));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid6));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid7));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupFolder, KUid8));
	
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid9));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid10));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid11));
	User::LeaveIfError(propRegister.DeregisterPropertyL(CPropertyReg::EGroupBookmark, KUid12));
	
	db->DeleteItemL(folder2.Id(), ETrue);
	
	db->CommitL();
	
	CleanupStack::PopAndDestroy(3);
	}

void DoCPropertyRegUnitL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** CPropertyReg tests ***"));
	
	GeneralTestsL();
	
	DatabasePropertyTestsL();
	BookmarkPropertyTestsL();
	}
