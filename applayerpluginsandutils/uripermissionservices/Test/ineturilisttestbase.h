// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __INEURILISTTESTBASE_H__
#define __INEURILISTTESTMAIN_H__

#include <ineturilist.h>
#include <ineturilistdef.h>

class CTestWrapper;

typedef InetUriList::TServiceType ServiceType;
typedef InetUriList::TListType ListType;

class CInetUriListTest : public CBase
	{
	public:
	static CInetUriListTest* NewL ( CTestWrapper& aTest );
	virtual ~CInetUriListTest ();
	
	void TestSuccess ( TInt aError );
	
	protected:
	CInetUriListTest (CTestWrapper& aTest);
	void ConstructL ();
	
	protected:
	RInetUriList 	iUriList;
	TInt 			iError;
	CTestWrapper& 	iTestWrapper;
	};

class CAddUriTest : public CInetUriListTest 
	{
	public:	
	static CAddUriTest* NewLC ( CTestWrapper& aTest, const TDesC8& aUri, ServiceType aServiceType, ListType aListType, const TDesC8& aFavouriteName );
	~CAddUriTest  ();
	void DoOperationL ();
	private:
	CAddUriTest ( CTestWrapper& aTest );
	void ConstructL ( const TDesC8& aUri, ServiceType aServiceType, ListType aListType, const TDesC8& aFavouriteName );
		
	private:
	RInetUri iInetUri;		
	};

class CCountUriTest : public CInetUriListTest
	{
	public:
	static CCountUriTest* NewLC ( CTestWrapper& aTest, ServiceType aServiceType, ListType aListType );
	~CCountUriTest ();
	void DoOperationL ();
	void ExpectedResult ( TInt aExpectedResult );
	
	private:
	CCountUriTest ( CTestWrapper& aTest, ServiceType aServiceType, ListType aListType );
	void ConstructL ();
	
	private:
	ServiceType iServiceType;
	ListType 	iListType;
	TInt iExpectedResult;
	};
#endif // __INEURILISTTESTMAIN_H__
