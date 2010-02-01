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
#include "testutils.h"

LOCAL_D CTestWrapper* gTestWrapper;

_LIT(KFilename, "t_cbookmark.cpp");

_LIT(KTxtBookmark1, "Bookmark 1");

_LIT8(KTxtUri, "http://lon-xcom.intra/duff/index.html");

const TInt KOneSecond = 1000000;
const TUint32 proxyId =	0x7C123456;

LOCAL_D void GeneralTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());
	
	gTestWrapper->Next(_L("[CBookmark tests] type"));
	gTestWrapper->TEST(bookmark.Type() == Bookmark::ETypeBookmark);
	
	gTestWrapper->Next(_L("[CBookmark tests] title"));
	bookmark.SetTitleL(KTxtBookmark1);
	gTestWrapper->TEST(bookmark.Title() == KTxtBookmark1);
                                       
	gTestWrapper->Next(_L("[CBookmark tests] home page flag"));
	gTestWrapper->TEST(bookmark.IsHomePage() == EFalse);
	db->SetHome(&bookmark);	
	gTestWrapper->TEST(bookmark.IsHomePage());
	                                                   
	gTestWrapper->Next(_L("[CBookmark tests] handle"));
	RBkBookmark handle = bookmark.OpenBookmark();
	CleanupClosePushL(handle);
	gTestWrapper->TEST(handle.Type() == bookmark.Type());
	gTestWrapper->TEST(handle.Title() == bookmark.Title());
	CleanupStack::PopAndDestroy(&handle);
	
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void TimeTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());
	
	gTestWrapper->Next(_L("[CBookmark tests] time - update visited"));
	TTime initalTime;
	initalTime.UniversalTime();
	User::After(KOneSecond);
	bookmark.UpdateVisited();
	TTime time1Sec = bookmark.LastVisited();
	gTestWrapper->TEST(time1Sec > initalTime);
	
	gTestWrapper->Next(_L("[CBookmark tests] time - set last visited"));
	bookmark.SetLastVisited(initalTime);
	TTime newTime = bookmark.LastVisited();
	gTestWrapper->TEST(newTime < time1Sec);
	
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void UriTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());
	
	gTestWrapper->Next(_L("[CBookmark tests] URI"));
	bookmark.SetUriL(KTxtUri);
	gTestWrapper->TEST(bookmark.Uri().Compare(KTxtUri) == 0);
	
	CleanupStack::PopAndDestroy(db);
	}

LOCAL_D void ExtendedPropertyTestsL()
	{
	CBookmarkDb* db = CBookmarkDb::NewL(Bookmark::EVisibilityDefault, NULL);
	CleanupStack::PushL(db);
	
	CBookmark& bookmark = db->CreateBookmarkL(db->RootL());
	CBkmrkExtendedProperties& properties = bookmark.BkmrkExtendedPropertiesL();
	gTestWrapper->TEST(properties.Proxy() != proxyId);
	properties.SetProxyL(proxyId);
	gTestWrapper->TEST(properties.Proxy() == proxyId);
		
	CleanupStack::PopAndDestroy(db);
	}

void DoCBookmarkUnitL(CTestWrapper* aTest)
	{
	gTestWrapper = aTest;
	gTestWrapper->SetFile(KFilename);
	gTestWrapper->Next(_L("*** CBookmark tests ***"));
	
	GeneralTestsL();
	TimeTestsL();
	UriTestsL();
	ExtendedPropertyTestsL();
	}
