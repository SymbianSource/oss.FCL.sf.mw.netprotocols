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

#include "testutils.h"
#include "ineturilisttestbase.h"

#define __LEAVE_IF_OOM_ERROR 	if ( iError == KErrNoMemory ) \
							User::Leave ( iError );


CInetUriListTest* CInetUriListTest::NewL ( CTestWrapper& aTest )
	{
	CInetUriListTest* self = new ( ELeave ) CInetUriListTest( aTest );
	CleanupStack::PushL ( self );
	self->ConstructL ( );
	CleanupStack::Pop (); // self
	return self;
	}
	
void CInetUriListTest::ConstructL ()
	{
	TRAP ( iError, iUriList.OpenL () ); // Open the list.	
	__LEAVE_IF_OOM_ERROR
	}

CInetUriListTest::CInetUriListTest ( CTestWrapper& aTest )
: iError ( KErrNone ),
iTestWrapper ( aTest )
	{
		
	}

CInetUriListTest::~CInetUriListTest ()
	{
	iUriList.Close ();			
	}

void CInetUriListTest::TestSuccess ( TInt aError )
	{
	iTestWrapper.TESTE ( iError == aError, aError );
	}
	
// ----------------------

CAddUriTest* CAddUriTest::NewLC ( CTestWrapper& aTest, const TDesC8& aUri, ServiceType aServiceType, ListType aListType, const TDesC8& aFavouriteName )
	{
	CAddUriTest* self = new ( ELeave )	CAddUriTest ( aTest );
	CleanupStack::PushL ( self );
	self->ConstructL ( aUri, aServiceType, aListType, aFavouriteName );
	return self;
	}

CAddUriTest::CAddUriTest ( CTestWrapper& aTest )
: CInetUriListTest ( aTest )
	{
		
	}

CAddUriTest::~CAddUriTest ()
	{
	iInetUri.Close ();		
	}

void CAddUriTest::ConstructL ( const TDesC8& aUri, ServiceType aServiceType, ListType aListType, const TDesC8& aFavouriteName )
	{
	CInetUriListTest::ConstructL ();
	iInetUri.CreateL ( aUri, aServiceType, aListType );
	iInetUri.SetFavouriteNameL ( aFavouriteName );
	}

void CAddUriTest::DoOperationL ()
	{
	TRAP ( iError, iUriList.AddL ( iInetUri ) );
	__LEAVE_IF_OOM_ERROR		
	}

// --------------------

CCountUriTest* CCountUriTest::NewLC ( CTestWrapper& aTest, ServiceType aServiceType, ListType aListType )
	{
	CCountUriTest* self = new ( ELeave ) CCountUriTest ( aTest, aServiceType, aListType );
	CleanupStack::PushL ( self );
	self->ConstructL ( );
	return self;
		
	}

CCountUriTest::~CCountUriTest ()
	{		
	}

void CCountUriTest::DoOperationL ()
	{
	TRAP ( iError, iExpectedResult = iUriList.Count ( iServiceType, iListType ) );
	__LEAVE_IF_OOM_ERROR
	}

void CCountUriTest::ExpectedResult ( TInt aExpectedResult )
	{
	iTestWrapper.TEST ( iExpectedResult == aExpectedResult );		
	}

CCountUriTest::CCountUriTest ( CTestWrapper& aTest, ServiceType aServiceType, ListType aListType )
: CInetUriListTest ( aTest ),
iServiceType ( aServiceType ),
iListType ( aListType ),
iExpectedResult (0)
	{
		
	}

void CCountUriTest::ConstructL ()
	{
	CInetUriListTest::ConstructL ();
	}
