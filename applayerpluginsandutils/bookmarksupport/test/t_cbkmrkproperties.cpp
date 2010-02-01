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
#include "bkmrkdb.h"
#include "bkmrkfolder.h"
#include "testutils.h"

LOCAL_D CTestWrapper* gTestWrapper;

_LIT(KFilename, "t_cbkmrkproperties.cpp");

_LIT(KTxtFolder, "propertiesTEST folder");

_LIT(KTxtDesc, "This is an extended property description");

_LIT8(KDataIcon, "123456789");
_LIT8(KAuthUri, "sip:user:pass@location.com:1666;transport=tcp;x=123;df=223?asd=b");
_LIT8(KAuthName, "user");
_LIT8(KAuthPassword, "pass");
_LIT8(KAuthName2, "joe");
_LIT8(KAuthPassword2, "pswd1234");

const Bookmark::TAttachmentId KIconId = 0xFFFFFFFF;
const TUint32 proxyId =	0x7CFFFFFF;
const TUint32 napId =	0x6CFFF000;

LOCAL_D void PropertiesTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
//	CBookmarkBase* item = static_cast<CBookmarkBase*>(&(db->CreateFolderL(KTxtFolder, db->RootL())));
	CBookmarkFolder& folder = db->CreateFolderL(KTxtFolder, db->RootL());
	
	CBkmrkProperties& properties = folder.BkmrkPropertiesL();
	
	gTestWrapper->Next(_L("[Properties] description"));
	properties.SetDescriptionL(KTxtDesc);
	gTestWrapper->TEST(properties.Description().Compare(KTxtDesc) == 0);
	
	gTestWrapper->Next(_L("[Properties] icon"));
	Bookmark::TAttachmentId iconId = KIconId;
	TRAPD(err, properties.SetIconIdL(iconId));
	gTestWrapper->TEST(err == KErrNotFound);
	
	iconId = db->CreateIconL(KDataIcon);
	properties.SetIconIdL(iconId);
	gTestWrapper->TEST(properties.IconId() == iconId);
	
	db->DeleteItemL(folder.Id(), ETrue);
	
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void ExtendedPropertiesTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());
	
	CBkmrkExtendedProperties& properties = bookmark.BkmrkExtendedPropertiesL();
	
	gTestWrapper->Next(_L("[Properties] authentication"));
	bookmark.SetUriL(KAuthUri);
	gTestWrapper->TEST(properties.AuthenticationL().Name().Compare(KAuthName) == 0);
	gTestWrapper->TEST(properties.AuthenticationL().Password().Compare(KAuthPassword) == 0);
	gTestWrapper->TEST(properties.AuthenticationL().Method() == CAuthentication::EDigest);
	
	CAuthentication* auth = CAuthentication::NewL(KAuthName2, KAuthPassword2, CAuthentication::EBasic);
	CleanupStack::PushL(auth);
	properties.SetAuthenticationL(*auth);
	gTestWrapper->TEST(properties.AuthenticationL().Name().Compare(KAuthName2) == 0);
	gTestWrapper->TEST(properties.AuthenticationL().Password().Compare(KAuthPassword2) == 0);
	gTestWrapper->TEST(properties.AuthenticationL().Method() == CAuthentication::EBasic);
	CleanupStack::PopAndDestroy(auth);
	
	gTestWrapper->Next(_L("[Properties] proxy and nap"));
	
	gTestWrapper->TEST(properties.Proxy() != proxyId);
	properties.SetProxyL(proxyId);
	gTestWrapper->TEST(properties.Proxy() == proxyId);
	
	gTestWrapper->TEST(properties.Nap() != napId);
	properties.SetNapL(napId);
	gTestWrapper->TEST(properties.Nap() == napId);
	
	CleanupStack::PopAndDestroy(db);
	}

void DoPropertiesUnitL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** Extended properties tests ***"));
	
	PropertiesTestsL();
	ExtendedPropertiesTestsL();
	}
