// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tbmtests.h"
#include <cauthentication.h>
#include "testutils.h"

_LIT ( KTxtFolder1, "Folder 1" );
_LIT ( KTxtFolder1_1, "Folder 1.1" );
_LIT ( KTxtFolder1_2, "Folder 1.2" );
_LIT ( KTxtFolder1_2_1, "Folder 1.2.1" );
_LIT ( KTxtFolder1_2_2, "Folder 1.2.2" );
_LIT ( KTxtFolder1_3, "Folder 1.3" );

_LIT ( KTxtFolder2, "Folder 2" );
_LIT ( KTxtFolder2_1, "Folder 2.1" );
_LIT ( KTxtFolder2_2, "Folder 2.2" );
_LIT ( KTxtFolder2_2_1, "Folder 2.2.1" );
_LIT ( KTxtFolder2_2_2, "Folder 2.2.2" );
_LIT ( KTxtFolder2_2_3, "Folder 2.2.3" );
_LIT ( KTxtFolder2_2_4, "Folder 2.2.4" );

_LIT ( KTxtFolder3, "Folder 3" );
_LIT ( KTxtFolder3_1, "Folder 3.1" );
_LIT ( KTxtFolder3_1_1, "Folder 3.1.1" );
_LIT ( KTxtFolder3_1_2, "Folder 3.1.2" );
_LIT ( KTxtFolder3_1_3, "Folder 3.1.3" );
_LIT ( KTxtFolder3_1_4, "Folder 3.1.4" );
_LIT ( KTxtFolder3_2, "Folder 3.2" );

_LIT8 ( KAuthUri, "sip:user:pass@location.com:1666;transport=tcp;x=123;df=223?asd=b" );
_LIT8 ( KAuthName2, "joe" );
_LIT8 ( KAuthPassword2, "pswd1234" );


_LIT8 (KIconData,		"****4644372131**7*&879");
_LIT8 (KIconData2,		"****abcdefghij**7*&xyz");

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
const TUid KUid13 = {0x1111};
const TUid KUid14 = {0x2222};
const TUid KUid15 = {0x3333};
const TUid KUid16 = {0x4444};
const TUid KUid17 = {0x5555};
const TUid KUid18 = {0x6666};
const TUid KUid19 = {0x7777};
const TUid KUid20 = {0x8888};
const TUid KUid21 = {0x9999};
const TUid KUid22 = {0xAAAA};
const TUid KUid23 = {0xBBBB};
const TUid KUid24 = {0xCCCC};

const TInt KMaxTestLength = 50;

TInt intValue1 = 1200;
TReal realValue1 = 67.456;
TInt intValue2 = 2000;
TReal realValue2 = 255.123;
TInt intValue3 = 3500;
TReal realValue3 = 289.567;

TInt intValue4 = 1200;
TReal realValue4 = 67.456;
TInt intValue5 = 2000;
TReal realValue5 = 255.123;
TInt intValue6 = 3500;
TReal realValue6 = 289.567;

_LIT8 ( KDbValue8, "Database 8-bit value1" );
_LIT ( KDbValue, "Database 16-bit value1" );
_LIT8 ( KFldrValue8, "Folder 8-bit value1" );
_LIT ( KFldrValue, "Folder 16-bit value1" );
_LIT8 ( KBkmrkValue8, "Bookmark 8-bit value1" );
_LIT ( KBkmrkValue, "Bookmark 16-bit value1" );

_LIT8 ( KDb2Value8, "Database 8-bit value2" );
_LIT ( KDb2Value, "Database 16-bit value2" );
_LIT8 ( KFldr2Value8, "Folder 8-bit value2" );
_LIT ( KFldr2Value, "Folder 16-bit value2" );
_LIT8 ( KBkmrk2Value8, "Bookmark 8-bit value2" );
_LIT ( KBkmrk2Value, "Bookmark 16-bit value2" );


CBmTests* CBmTests::NewL ( CTestWrapper& aTestWrapper )
	{
	CBmTests* self = new ( ELeave )CBmTests ( aTestWrapper );
	return self;
	}

CBmTests::CBmTests ( CTestWrapper& aTestWrapper )
: iTestWrapper ( aTestWrapper )
	{
	
	}
	
CBmTests::~CBmTests ()
	{
	CloseDb ();
	}

void CBmTests::OpenDbL ()
	{
	iBkDatabase.OpenL ();
	}
	
void CBmTests::CloseDb ()
	{
	iBkDatabase.Close ();
	}

RBkFolder CBmTests::CreateFolderLC ( const TDesC& aFolderName, RBkFolder* aParent )
	{
	RBkFolder folder;
	TRAPD ( err, folder = iBkDatabase.CreateFolderL ( aFolderName, aParent ) );
	User::LeaveIfError ( err );
	CleanupClosePushL ( folder );
	return folder;
	}

RBkFolder CBmTests::OpenFolderLC ( const TDesC& aFolderName )
	{
	RBkFolder folder = iBkDatabase.OpenFolderL ( aFolderName );
	CleanupClosePushL ( folder );
	return folder;
	}

void CBmTests::CommitDbL ()
	{
	iBkDatabase.CommitL ();
	}

void CBmTests::SetBkmrkDbPropertiesL ()
	{
	RBkBookmark home = iBkDatabase.CreateBookmarkL ();
	CleanupClosePushL ( home );
	
	home.SetTitleL ( _L ( "Home bookmark" ) );
	
	iBkDatabase.SetHome ( home );	
	iBkDatabase.SetHomePageTextL ( _L ( "My home page text" ) );
	iBkDatabase.SetSearchUriL ( _L8 ( "www.searchuri.com" ) );
	
	CleanupStack::PopAndDestroy ( &home ); // home
	}

void CBmTests::UpdateBkmrkDbPropertiesL ()
	{
	RBkBookmark home = iBkDatabase.CreateBookmarkL ();
	CleanupClosePushL ( home );
	
	home.SetTitleL ( _L ( "New home bookmark" ) );
	
	iBkDatabase.SetHome ( home );	
	iBkDatabase.SetHomePageTextL ( _L ( "My new home page text" ) );
	iBkDatabase.SetSearchUriL ( _L8 ( "www.newsearchuri.com" ) );
	
	CleanupStack::PopAndDestroy ( &home ); // home	
	}

void CBmTests::CreateTreeL ()
	{
	// Create bookmarks under root
	RBkBookmark bookmark1 = iBkDatabase.CreateBookmarkL ();
	CleanupClosePushL ( bookmark1 );
	RBkBookmark bookmark2 = iBkDatabase.CreateBookmarkL ();
	CleanupClosePushL ( bookmark2 );
	
	bookmark1.SetTitleL ( _L("Google") );
	bookmark2.SetTitleL ( _L("Hotmail") );
	
	bookmark1.SetUriL ( KAuthUri );
	CAuthentication* auth = CAuthentication::NewL(KAuthName2, KAuthPassword2, CAuthentication::EBasic);
	CleanupStack::PushL(auth);
	bookmark2.SetAuthenticationL(*auth);
	CleanupStack::PopAndDestroy ( auth );
	
	CleanupStack::PopAndDestroy ( 2 ); // bookmark1, bookmark2
	
	// Create folder1
	RBkFolder folder1 = iBkDatabase.CreateFolderL ( KTxtFolder1() );
	CleanupClosePushL ( folder1 );
	// Create sub-folders under folder1
	RBkFolder folder1_1 = iBkDatabase.CreateFolderL ( KTxtFolder1_1(), &folder1 );
	CleanupClosePushL ( folder1_1 );
	RBkFolder folder1_2 = iBkDatabase.CreateFolderL ( KTxtFolder1_2(), &folder1 );
	CleanupClosePushL ( folder1_2 );
	RBkFolder folder1_3 = iBkDatabase.CreateFolderL ( KTxtFolder1_3(), &folder1 );
	CleanupClosePushL ( folder1_3 );
	RBkFolder folder1_2_1 = iBkDatabase.CreateFolderL ( KTxtFolder1_2_1(), &folder1_2 );
	CleanupClosePushL ( folder1_2_1 );
	RBkFolder folder1_2_2 = iBkDatabase.CreateFolderL ( KTxtFolder1_2_2(), &folder1_2 );
	CleanupClosePushL ( folder1_2_2 );
	
	// Create bookmarks under folder1
	bookmark1 = iBkDatabase.CreateBookmarkL ( &folder1 );
	CleanupClosePushL ( bookmark1 );
	bookmark2 = iBkDatabase.CreateBookmarkL ( &folder1 );
	CleanupClosePushL ( bookmark2 );
	bookmark1.SetTitleL ( _L ( "ESPN Sports" ) );
	bookmark2.SetTitleL ( _L ( "Star Sports" ) );
	
	Bookmark::TAttachmentId iconId = iBkDatabase.CreateIconL ( KIconData );
	bookmark1.SetIconIdL ( iconId );
		
	CleanupStack::PopAndDestroy ( 2 ); // bookmark1, bookmark2
	
	CleanupStack::PopAndDestroy ( 6 ); 	// folder1, folder1_1, folder1_2, folder1_3,
										// folder1_2_1, folder1_2_2
											
	RBkFolder folder2 = iBkDatabase.CreateFolderL ( KTxtFolder2() );
	CleanupClosePushL ( folder2 );
	RBkFolder folder2_1 = iBkDatabase.CreateFolderL ( KTxtFolder2_1(), &folder2 );
	CleanupClosePushL ( folder2_1 );
	RBkFolder folder2_2 = iBkDatabase.CreateFolderL ( KTxtFolder2_2(), &folder2 );	
	CleanupClosePushL ( folder2_2 );		
	RBkFolder folder2_2_1 = iBkDatabase.CreateFolderL ( KTxtFolder2_2_1(), &folder2_2 );
	CleanupClosePushL ( folder2_2_1 );
	RBkFolder folder2_2_2 = iBkDatabase.CreateFolderL ( KTxtFolder2_2_2(), &folder2_2 );
	CleanupClosePushL ( folder2_2_2 );
	
	CleanupStack::PopAndDestroy ( 5 ); 	// folder2, folder2_1, folder2_2, 
										// folder2_2_1, folder2_2_2	

	RBkFolder folder3 = iBkDatabase.CreateFolderL ( KTxtFolder3() );
	CleanupClosePushL ( folder3 );
	Bookmark::TAttachmentId iconId2 = iBkDatabase.CreateIconL ( KIconData2 );
	folder3.SetIconIdL ( iconId2 );
	RBkFolder folder3_1 = iBkDatabase.CreateFolderL ( KTxtFolder3_1(), &folder3 );
	CleanupClosePushL ( folder3_1 );
	RBkFolder folder3_2 = iBkDatabase.CreateFolderL ( KTxtFolder3_2(), &folder3 );
	CleanupClosePushL ( folder3_2 );	
	RBkFolder folder3_1_1 = iBkDatabase.CreateFolderL ( KTxtFolder3_1_1(), &folder3_1 );
	CleanupClosePushL ( folder3_1_1 );
	RBkFolder folder3_1_4 = iBkDatabase.CreateFolderL ( KTxtFolder3_1_4(), &folder3_1 );
	CleanupClosePushL ( folder3_1_4 );
	
	CleanupStack::PopAndDestroy ( 5 ); // folder3, folder3_1, folder3_2, folder3_1_1, folder3_1_4
	}

void CBmTests::ModifyTreeL ()
	{
	RBkFolder folder2_2 = iBkDatabase.OpenFolderL ( KTxtFolder2_2() );
	CleanupClosePushL ( folder2_2 );
	RBkFolder folder2_2_3 = iBkDatabase.CreateFolderL ( KTxtFolder2_2_3() );
	CleanupClosePushL ( folder2_2_3 );
	RBkFolder folder2_2_4 = iBkDatabase.CreateFolderL ( KTxtFolder2_2_4() );
	CleanupClosePushL ( folder2_2_4 );
	
	folder2_2.AppendL ( folder2_2_3 );
	folder2_2.AppendL ( folder2_2_4 );
	
	CleanupStack::PopAndDestroy ( 3 ); // folder2_2, folder2_2_3, folder2_2_4
	
	RBkFolder folder3_1 = iBkDatabase.OpenFolderL ( KTxtFolder3_1() );
	CleanupClosePushL ( folder3_1 );
	RBkFolder folder3_1_2 = iBkDatabase.CreateFolderL ( KTxtFolder3_1_2() );
	CleanupClosePushL ( folder3_1_2 );
	RBkFolder folder3_1_3 = iBkDatabase.CreateFolderL ( KTxtFolder3_1_3() );
	CleanupClosePushL ( folder3_1_3 );
	folder3_1.InsertL ( folder3_1_2, 1 );
	folder3_1.InsertL ( folder3_1_3, 2 );
	
	CleanupStack::PopAndDestroy ( 3 ); // folder3_1, folder3_1_2, folder3_3

	}

void CBmTests::CreateCustomPropertiesL ()
	{
	iBkDatabase.RegisterDatabasePropertyL ( KUid1, Bookmark::EDataTypeInt );
	iBkDatabase.RegisterDatabasePropertyL ( KUid2, Bookmark::EDataTypeReal );
	iBkDatabase.RegisterDatabasePropertyL ( KUid3, Bookmark::EDataTypeDes8 );
	iBkDatabase.RegisterDatabasePropertyL ( KUid4, Bookmark::EDataTypeDes16 );

	iBkDatabase.RegisterBookmarkPropertyL ( KUid5, Bookmark::EDataTypeInt );
	iBkDatabase.RegisterBookmarkPropertyL ( KUid6, Bookmark::EDataTypeReal );
	iBkDatabase.RegisterBookmarkPropertyL ( KUid7, Bookmark::EDataTypeDes8 );
	iBkDatabase.RegisterBookmarkPropertyL ( KUid8, Bookmark::EDataTypeDes16 );

	iBkDatabase.RegisterFolderPropertyL ( KUid9, Bookmark::EDataTypeInt );
	iBkDatabase.RegisterFolderPropertyL ( KUid10, Bookmark::EDataTypeReal );
	iBkDatabase.RegisterFolderPropertyL ( KUid11, Bookmark::EDataTypeDes8 );
	iBkDatabase.RegisterFolderPropertyL ( KUid12, Bookmark::EDataTypeDes16 );

	iBkDatabase.RegisterDatabasePropertyL ( KUid13, Bookmark::EDataTypeInt );
	iBkDatabase.RegisterDatabasePropertyL ( KUid14, Bookmark::EDataTypeReal );
	iBkDatabase.RegisterDatabasePropertyL ( KUid15, Bookmark::EDataTypeDes8 );
	iBkDatabase.RegisterDatabasePropertyL ( KUid16, Bookmark::EDataTypeDes16 );

	iBkDatabase.RegisterBookmarkPropertyL ( KUid17, Bookmark::EDataTypeInt );
	iBkDatabase.RegisterBookmarkPropertyL ( KUid18, Bookmark::EDataTypeReal );
	iBkDatabase.RegisterBookmarkPropertyL ( KUid19, Bookmark::EDataTypeDes8 );
	iBkDatabase.RegisterBookmarkPropertyL ( KUid20, Bookmark::EDataTypeDes16 );

	iBkDatabase.RegisterFolderPropertyL ( KUid21, Bookmark::EDataTypeInt );
	iBkDatabase.RegisterFolderPropertyL ( KUid22, Bookmark::EDataTypeReal );
	iBkDatabase.RegisterFolderPropertyL ( KUid23, Bookmark::EDataTypeDes8 );
	iBkDatabase.RegisterFolderPropertyL ( KUid24, Bookmark::EDataTypeDes16 );

	}
	
void CBmTests::SetCustomPropertiesValueL ()
	{	
	iBkDatabase.SetCustomPropertyL ( KUid1, intValue1 );
	iBkDatabase.SetCustomPropertyL ( KUid2, realValue1 );	
	iBkDatabase.SetCustomPropertyL ( KUid3, KDbValue8 );
	iBkDatabase.SetCustomPropertyL ( KUid4, KDbValue );	
	
	RBkBookmark bookmark1 = iBkDatabase.OpenBookmarkL ( 1 );
	CleanupClosePushL ( bookmark1 );
	bookmark1.SetCustomPropertyL ( KUid5, intValue2 );
	bookmark1.SetCustomPropertyL ( KUid6, realValue2 );	
	bookmark1.SetCustomPropertyL ( KUid7, KBkmrkValue8 );
	bookmark1.SetCustomPropertyL ( KUid8, KBkmrkValue );	
	CleanupStack::PopAndDestroy ( 1 ); // bookmark1

	RBkFolder folder1 = iBkDatabase.OpenFolderL ( KTxtFolder1 );
	CleanupClosePushL ( folder1 );
	folder1.SetCustomPropertyL ( KUid9, intValue3 );
	folder1.SetCustomPropertyL ( KUid10, realValue3 );	
	folder1.SetCustomPropertyL ( KUid11, KFldrValue8 );
	folder1.SetCustomPropertyL ( KUid12, KFldrValue );	
	CleanupStack::PopAndDestroy ( &folder1 );

	iBkDatabase.SetCustomPropertyL ( KUid13, intValue4 );
	iBkDatabase.SetCustomPropertyL ( KUid14, realValue4 );	
	iBkDatabase.SetCustomPropertyL ( KUid15, KDb2Value8 );
	iBkDatabase.SetCustomPropertyL ( KUid16, KDb2Value );	

	RBkBookmark bookmark2 = iBkDatabase.OpenBookmarkL ( 2 );
	CleanupClosePushL ( bookmark2 );
	bookmark2.SetCustomPropertyL ( KUid17, intValue5 );
	bookmark2.SetCustomPropertyL ( KUid18, realValue5 );	
	bookmark2.SetCustomPropertyL ( KUid19, KBkmrk2Value8 );
	bookmark2.SetCustomPropertyL ( KUid20, KBkmrk2Value );	
	CleanupStack::PopAndDestroy ( &bookmark2 );

	// Reset the custom property value for bookmark1
	bookmark1 = iBkDatabase.OpenBookmarkL ( 1 );
	CleanupClosePushL ( bookmark1 );
	bookmark1.SetCustomPropertyL ( KUid17, intValue5 );
	bookmark1.SetCustomPropertyL ( KUid18, realValue5 );	
	bookmark1.SetCustomPropertyL ( KUid19, KBkmrk2Value8 );
	bookmark1.SetCustomPropertyL ( KUid20, KBkmrk2Value );	
	CleanupStack::PopAndDestroy ( 1 ); // bookmark1


	RBkFolder folder2 = iBkDatabase.OpenFolderL ( KTxtFolder2 );
	CleanupClosePushL ( folder2 );
	folder2.SetCustomPropertyL ( KUid21, intValue6 );
	folder2.SetCustomPropertyL ( KUid22, realValue6 );	
	folder2.SetCustomPropertyL ( KUid23, KFldr2Value8 );
	folder2.SetCustomPropertyL ( KUid24, KFldr2Value );	
	CleanupStack::PopAndDestroy ( &folder2 );
	folder1 = iBkDatabase.OpenFolderL ( KTxtFolder1 );
	CleanupClosePushL ( folder1 );
	folder1.SetCustomPropertyL ( KUid21, intValue6 );
	folder1.SetCustomPropertyL ( KUid22, realValue6 );	
	folder1.SetCustomPropertyL ( KUid23, KFldr2Value8 );
	folder1.SetCustomPropertyL ( KUid24, KFldr2Value );	
	CleanupStack::PopAndDestroy ( &folder1 );
	}

void CBmTests::ReadCustomPropertiesValueL ()
	{
	HBufC* retDesC16 = HBufC::NewLC(KMaxTestLength);
	TPtr retPtr16 = retDesC16->Des();
	HBufC8* retDesC8 = HBufC8::NewLC(KMaxTestLength * 2);
	TPtr8 retPtr8 = retDesC8->Des();
	TReal realValue;
	TInt intValue;
	
	iBkDatabase.GetCustomPropertyL ( KUid1, intValue );	
	iTestWrapper.TEST ( intValue == intValue1 );

	iBkDatabase.GetCustomPropertyL ( KUid2, realValue );	
	iTestWrapper.TEST ( realValue == realValue1 );

	iBkDatabase.GetCustomPropertyL ( KUid3, retPtr8 );
	iTestWrapper.TEST(retPtr8.Compare(KDbValue8) == 0);
	
	iBkDatabase.GetCustomPropertyL ( KUid4, retPtr16 );
	iTestWrapper.TEST(retPtr16.Compare(KDbValue) == 0);
	
	RBkBookmark bookmark2 = iBkDatabase.OpenBookmarkL ( 2 );
	CleanupClosePushL ( bookmark2 );

	bookmark2.GetCustomPropertyL ( KUid17, intValue );
	iTestWrapper.TEST ( intValue == intValue5 );	

	bookmark2.GetCustomPropertyL ( KUid18, realValue );
	iTestWrapper.TEST ( realValue == realValue5 );	
	
	bookmark2.GetCustomPropertyL ( KUid19, retPtr8 );
	iTestWrapper.TEST(retPtr8.Compare ( KBkmrk2Value8 ) == 0);

	bookmark2.GetCustomPropertyL ( KUid20, retPtr16 );
	iTestWrapper.TEST(retPtr16.Compare ( KBkmrk2Value ) == 0);
	
	RBkFolder folder2 = iBkDatabase.OpenFolderL ( KTxtFolder2 );
	CleanupClosePushL ( folder2 );
	folder2.GetCustomPropertyL ( KUid21, intValue );
	iTestWrapper.TEST ( intValue == intValue6 );	

	folder2.GetCustomPropertyL ( KUid22, realValue );	
	iTestWrapper.TEST ( realValue == realValue6 );	

	folder2.GetCustomPropertyL ( KUid23, retPtr8 );
	iTestWrapper.TEST(retPtr8.Compare ( KFldr2Value8 ) == 0);
	
	folder2.GetCustomPropertyL ( KUid24, retPtr16 );	
	iTestWrapper.TEST(retPtr16.Compare ( KFldr2Value ) == 0);
				
	CleanupStack::PopAndDestroy ( 4 ); // retDesC16, retDesC8, bookmark2, folder2
	}

void CBmTests::UpdateTreeL ()
	{
	iBkDatabase.RefreshL ();
	}

void CBmTests::DeleteAllL ()
	{
	RBkFolder root = iBkDatabase.OpenRootL ();
	CleanupClosePushL ( root );
	
	TInt count = root.Count ();	
	
	while ( count-- )
		{
		RBkNode node = root.OpenItemL ( 0 );
		CleanupClosePushL ( node );
		TBool recursive = ETrue;
		
		if ( node.Type() == Bookmark::ETypeBookmark )
			{
			recursive = EFalse;
			}
		
		iBkDatabase.DeleteItemL ( node.Id(), recursive );
		CleanupStack::PopAndDestroy ( &node );
		}		
	CleanupStack::PopAndDestroy ( &root ); 
	}

	
