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

#include "tineturilisttests.h"
#include <cauthentication.h>
#include "testutils.h"

#define __LEAVEIFOOM__ if ( err == KErrNoMemory ) \
				User::Leave ( err );
#define __PRINTIFNOTOOM__ if ( iNormalRun )

CInetUriListTestsBase::CInetUriListTestsBase ( CTestWrapper& aTestWrapper, TBool aNormalRun )
: iTestWrapper ( aTestWrapper ),
iNormalRun ( aNormalRun )
	{
		
	}

CInetUriListTestsBase::~CInetUriListTestsBase ()
	{
	iInetUriList.Close ();		
	}

void CInetUriListTestsBase::ConstructL ()
	{
	iInetUriList.OpenL ();
	}

RInetUri CInetUriListTestsBase::CreateNewUriL ( const TDesC8& aUri, TServiceType aServiceType, TListType aListType )
	{
	RInetUri myInetUri;
	CleanupClosePushL ( myInetUri );
	myInetUri.CreateL ( aUri, aServiceType, aListType );
	CleanupStack::Pop (); // myInetUri 
	return myInetUri;			
	}

void CInetUriListTestsBase::RunTestsL ()
	{
	if ( iNormalRun )
		{
		DoCleanupL ();
		DoOperationL ();		
		}		
	else
		{
		TInt ret = KErrNoMemory;
		TInt failAt = 0;
		while( ret != KErrNone )
			{
			DoCleanupL ();
			failAt++;
   			iTestWrapper.Tester().Printf( _L( "Test step: %d\n" ), failAt );					
			__UHEAP_RESET;
			__UHEAP_SETFAIL( RHeap::EDeterministic, failAt );
			TInt countBefore = User::CountAllocCells();			
			
			TRAP ( ret, DoOperationL () );

			TInt countAfter = User::CountAllocCells();					
			__UHEAP_RESET;	
			if ( countBefore != countAfter )
				{
				iTestWrapper.Tester().Printf ( _L ("OOM test failed at iteration %d"), failAt );
				break;
				}
			}	
		iTestWrapper.Tester().Printf ( _L ("OOM test has been successfully completed. Total iterations: %d"), failAt );							
		}
	}
// -----------------------

CInetUriListStorageTests* CInetUriListStorageTests::NewL ( CTestWrapper& aTestWrapper, TBool aNormalRun )
	{
	CInetUriListStorageTests* self  = new ( ELeave )CInetUriListStorageTests ( aTestWrapper, aNormalRun );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ();
	return self;
	}
	
CInetUriListStorageTests::~CInetUriListStorageTests ()
	{
		
	}

CInetUriListStorageTests::CInetUriListStorageTests ( CTestWrapper& aTestWrapper, TBool aNormalRun )
: CInetUriListTestsBase ( aTestWrapper, aNormalRun )
	{
		
	}

void CInetUriListStorageTests::DoOperationL ()
	{
	iTestWrapper.SetFile ( _L ("tineturilisttests.cpp") );
	AddTestsL ();
	GetListTypeL ();
	UpdateTestsL ();		
	RemoveTestsL ();
	SameUriDiffServiceTypeTestsL ();
	AddFewMoreUrisL ();
	}

void CInetUriListStorageTests::DoCleanupL ()
	{
	CInetUriListCleanup* listCleanup = CInetUriListCleanup::NewL (iTestWrapper);
	CleanupStack::PushL ( listCleanup );
	listCleanup->DoOperationL ();
	CleanupStack::PopAndDestroy (); // listCleanup		
	}

void CInetUriListStorageTests::AddTestsL ()
	{
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "URI add tests" ) );
	// Create a new uri
	RInetUri myInetUri = CreateNewUriL ( KTestUri(), InetUriList::EWapPush, InetUriList::EWhiteList );
	CleanupClosePushL ( myInetUri );
	myInetUri.SetFavouriteNameL ( KUriFavouriteName() );
	
	// Add the uri
	TRAPD ( err, iInetUriList.AddL ( myInetUri ) );
	__LEAVEIFOOM__	
	iTestWrapper.TESTE( err==KErrNone, err );
	
	// Create another uri
	RInetUri myInetUri2 = CreateNewUriL ( KTestUri2(), InetUriList::EWapPush, InetUriList::EWhiteList );
	CleanupClosePushL ( myInetUri2 );

	// Add the uri
	TRAP ( err, iInetUriList.AddL ( myInetUri2 ) );
	__LEAVEIFOOM__
	iTestWrapper.TESTE( err==KErrNone, err );
	
	TRAP ( err, iInetUriList.AddL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TESTE( err==InetUriList::KErrUriAlreadyExists, err );

	// Create another uri
	RInetUri myInetUri3 = CreateNewUriL ( KTestUri3(), InetUriList::EWapPush, InetUriList::EBlackList );
	CleanupClosePushL ( myInetUri3 );

	// Add the uri
	TRAP ( err, iInetUriList.AddL ( myInetUri3 ) );
	__LEAVEIFOOM__
	iTestWrapper.TESTE( err==KErrNone, err );
	
	TInt count = iInetUriList.Count ( InetUriList::EWapPush, InetUriList::EWhiteList );
	// 2 URI has been added
	iTestWrapper.TEST( count == 2 );
	
	CleanupStack::PopAndDestroy (3); // inetUri, inetUri2, inetUri3

	
	count = iInetUriList.Count ( InetUriList::EWapPush, InetUriList::EWhiteList );
	// 2 URI has been added
	iTestWrapper.TEST( count == 2 );
	count = iInetUriList.Count ( InetUriList::EWapPush, InetUriList::EBlackList );
	// 2 URI has been added
	iTestWrapper.TEST( count == 1 );
		
	}

void CInetUriListStorageTests::RemoveTestsL ()
	{
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "URI remove test" ) );			
	RInetUri myInetUri = iInetUriList.OpenInetUriL ( KTestUri2(), InetUriList::EWapPush );
	CleanupClosePushL ( myInetUri );
	TRAPD ( err, iInetUriList.RemoveL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	
	// Try to update. Must return KErrNotFound
	TRAP ( err, iInetUriList.UpdateL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == InetUriList::KErrUriNotFound );

	CleanupStack::PopAndDestroy ( &myInetUri );	
	
	// Open the same URI
	TRAP ( err, myInetUri = iInetUriList.OpenInetUriL ( KTestUri2(), InetUriList::EWapPush ) );		
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == InetUriList::KErrUriNotFound );
	
	// Now we are having only 1 URI in whitelist and blacklist
	TInt count = iInetUriList.Count ( InetUriList::EWapPush, InetUriList::EWhiteList );
	// 1 URI has been added
	iTestWrapper.TEST( count == 1 );
	count = iInetUriList.Count ( InetUriList::EWapPush, InetUriList::EBlackList );
	// 1 URI has been added
	iTestWrapper.TEST( count == 1 );		
	}

void CInetUriListStorageTests::UpdateTestsL ()
	{
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "URI update test" ) );		
	RInetUri myInetUri = iInetUriList.OpenInetUriL ( KTestUri2(), InetUriList::EWapPush );
	CleanupClosePushL ( myInetUri );
	// Update list type
	myInetUri.SetListType ( InetUriList::EBlackList );
	TRAPD ( err, iInetUriList.UpdateL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TESTE( err==KErrNone, err );
	CleanupStack::PopAndDestroy (&myInetUri); // myInetUri

	// Check list type is updated proeprly
	myInetUri = iInetUriList.OpenInetUriL ( KTestUri2(), InetUriList::EWapPush );
	CleanupClosePushL ( myInetUri );	
	iTestWrapper.TEST ( myInetUri.ListType() == InetUriList::EBlackList );
	// Update favourite name
	myInetUri.SetFavouriteNameL ( KUriFavouriteName2() );
	TRAP ( err, iInetUriList.UpdateL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TESTE( err==KErrNone, err );
	CleanupStack::PopAndDestroy (); // myInetUri
	
	// Open again and see favourite name is updated properly
	myInetUri = iInetUriList.OpenInetUriL ( KTestUri2(), InetUriList::EWapPush );
	CleanupClosePushL ( myInetUri );	
	iTestWrapper.TEST ( myInetUri.FavouriteName().Compare ( KUriFavouriteName2() ) == 0 );
	CleanupStack::PopAndDestroy (); // myInetUri

	// Now we are having 1 URI in whitelist and 2 URI in blacklist
	TInt count = iInetUriList.Count ( InetUriList::EWapPush, InetUriList::EWhiteList );
	// 2 URI has been added
	iTestWrapper.TEST( count == 1 );
	count = iInetUriList.Count ( InetUriList::EWapPush, InetUriList::EBlackList );
	// 2 URI has been added
	iTestWrapper.TEST( count == 2 );
		
	}

void CInetUriListStorageTests::GetListTypeL ()
	{
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Retrieve URI list type" ) );	
	InetUriList::TListType listType;
	TInt err = iInetUriList.GetListType ( KTestUri(), InetUriList::EWapPush, listType );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	iTestWrapper.TEST ( listType == InetUriList::EWhiteList );
	
	err = iInetUriList.GetListType ( KTestUri3(), InetUriList::EWapPush, listType );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	iTestWrapper.TEST ( listType == InetUriList::EBlackList );

	err = iInetUriList.GetListType ( KUnknownUri(), InetUriList::EWapPush, listType );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == InetUriList::KErrUriNotFound );
	
	// Open an invalid URI
	RInetUri myInetUri;
	TRAP ( err, myInetUri = iInetUriList.OpenInetUriL ( KUnknownUri(), EWapPush ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == InetUriList::KErrUriNotFound );
	}

void CInetUriListStorageTests::SameUriDiffServiceTypeTestsL ()
	{
	RInetUri myInetUri = CreateNewUriL ( KTestUri4(), InetUriList::EWapPush, InetUriList::EWhiteList );
	CleanupClosePushL ( myInetUri );
	RInetUri myInetUri2 = CreateNewUriL ( KTestUri4(), InetUriList::EBrowser, InetUriList::EWhiteList );
	CleanupClosePushL ( myInetUri2 );
	
	// Add the 2 URIs
	TRAPD ( err, iInetUriList.AddL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	TRAP ( err, iInetUriList.AddL ( myInetUri2 ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	
	CleanupStack::PopAndDestroy (2); // myInetUri, myInetUri2
	
	// Now open the URI
	TRAP ( err, myInetUri = iInetUriList.OpenInetUriL ( KTestUri4(), InetUriList::EWapPush ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	CleanupClosePushL ( myInetUri );
	// Remove the URI
	TRAP ( err, iInetUriList.RemoveL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	CleanupStack::PopAndDestroy ( &myInetUri );
	
	TRAP ( err, myInetUri = iInetUriList.OpenInetUriL ( KTestUri4(), InetUriList::EWapPush ) );	
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == InetUriList::KErrUriNotFound );
	
	TRAP ( err, myInetUri = iInetUriList.OpenInetUriL ( KTestUri4(), InetUriList::EBrowser ) );	
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	myInetUri.Close ();

	// Now we are having 1 URI in Browser Service Type in whitelist
	TInt count = iInetUriList.Count ( InetUriList::EBrowser, InetUriList::EWhiteList );
	// 1 URI has been added
	iTestWrapper.TEST( count == 1 );
	
	// Add few more URIs in Browser service type 1 in BL and 1 in WL
	myInetUri = CreateNewUriL ( KTestUri5(), InetUriList::EBrowser, InetUriList::EBlackList );
	CleanupClosePushL ( myInetUri );
	myInetUri2 = CreateNewUriL ( KTestUri6(), InetUriList::EBrowser, InetUriList::EWhiteList );
	CleanupClosePushL ( myInetUri2 );
	myInetUri2.SetFavouriteNameL ( KUriFavouriteName3() );
	
	TRAP ( err, iInetUriList.AddL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	TRAP ( err, iInetUriList.AddL ( myInetUri2 ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	CleanupStack::PopAndDestroy ( 2 ); // myInetUri, myInetUri2			
	}

void CInetUriListStorageTests::AddFewMoreUrisL ()
	{
	RInetUri myInetUri = CreateNewUriL ( KTestUri7(), EBrowser, EWhiteList );
	CleanupClosePushL ( myInetUri );
	RInetUri myInetUri2 = CreateNewUriL ( KTestUri8(), EBrowser, EWhiteList );
	CleanupClosePushL ( myInetUri2 );
	RInetUri myInetUri3 = CreateNewUriL ( KTestUri9(), EBrowser, EBlackList );
	CleanupClosePushL ( myInetUri3 );
	RInetUri myInetUri4 = CreateNewUriL ( KTestUri10(), EBrowser, EBlackList );
	CleanupClosePushL ( myInetUri4 );
	RInetUri myInetUri5 = CreateNewUriL ( KTestUri11(), EBrowser, EBlackList );
	CleanupClosePushL ( myInetUri5 );
	
	TRAPD ( err, iInetUriList.AddL ( myInetUri ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	TRAP ( err, iInetUriList.AddL ( myInetUri2 ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	TRAP ( err, iInetUriList.AddL ( myInetUri3 ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	TRAP ( err, iInetUriList.AddL ( myInetUri4 ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	TRAP ( err, iInetUriList.AddL ( myInetUri5 ) );
	__LEAVEIFOOM__
	iTestWrapper.TEST ( err == KErrNone );
	
	CleanupStack::PopAndDestroy (5); // myInetUri ... myInetUri4	
	}



// -------------------------

CInetUriListQuery* CInetUriListQuery::NewLC ( CTestWrapper& aTest, TBool aNormalRun )
	{
	CInetUriListQuery* self = new (ELeave)CInetUriListQuery ( aTest, aNormalRun );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	return self;
	}


CInetUriListQuery::CInetUriListQuery ( CTestWrapper& aTest, TBool aNormalRun )
: CInetUriListTestsBase ( aTest, aNormalRun )
	{
		
	}


CInetUriListQuery::~CInetUriListQuery ()
	{
	Cleanup ();
	}

void CInetUriListQuery::DoOperationL ()
	{
	TRAPD ( err, QueryListL() );
	Cleanup ();
	User::LeaveIfError ( err );
	TRAPD ( tlderr, QueryTldListL());
	User::LeaveIfError ( tlderr );
	}

void CInetUriListQuery::QueryListL ()
	{	
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with ServiceType & ListType - Browser & Whitelist") );
	// Generate the expected URIs for this query
	AppendExpectedUriL ( KTestUri4(), EBrowser );
	AppendExpectedUriL ( KTestUri6(), EBrowser );
	AppendExpectedUriL ( KTestUri7(), EBrowser );
	AppendExpectedUriL ( KTestUri8(), EBrowser );	
	TQueryArgs args ( InetUriList::EBrowser, InetUriList::EWhiteList );	
	DoQueryL ( args );
	
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with ServiceType & ListType - Browser & Blacklist") );	
	AppendExpectedUriL ( KTestUri5(), EBrowser );
	AppendExpectedUriL ( KTestUri9(), EBrowser );
	AppendExpectedUriL ( KTestUri10(), EBrowser );
	AppendExpectedUriL ( KTestUri11(), EBrowser );			
	TQueryArgs args2 ( InetUriList::EBrowser, InetUriList::EBlackList );	
	DoQueryL ( args2 );

	// Do query operation with exact path match and a list type
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with exact path match & ListType") );		
	AppendExpectedUriL ( KTestUri6(), EBrowser );
	const TPtrC8 uri ( KTestUri6() );
	TQueryArgs args3 ( &uri, EBrowser, EWhiteList, EExactPath );
	DoQueryL ( args3 );
	
	// Do query operation with partial suffix path match and a list type
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with partial suffix path match & ListType") );		
	const TPtrC8 uri2 ( KPartialPathSuffixUri() );
	AppendExpectedUriL ( KTestUri4(), EBrowser );
	AppendExpectedUriL ( KTestUri6(), EBrowser );
	TQueryArgs args4 ( &uri2, EBrowser, EWhiteList, EPartialSuffixPath );
	DoQueryL ( args4 );
	
	// Do query operation with partial prefix path match and a list type
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with partial prefix path match & ListType") );	
	AppendExpectedUriL ( KTestUri5(), EBrowser );
	const TPtrC8 uri3 ( KPartialPathPrefixUri() );
	TQueryArgs args5 ( &uri3, EBrowser, EBlackList, EPartialPrefixPath );
	DoQueryL ( args5 );
	
	// Do query operation with partial suffix path match and without a list type
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with partial suffix path match & without ListType") );	
	AppendExpectedUriL ( KTestUri4(), EBrowser );
	AppendExpectedUriL ( KTestUri5(), EBrowser );	
	AppendExpectedUriL ( KTestUri6(), EBrowser );	
	const TPtrC8 uri4 ( KPartialPathSuffixUri() );
	TQueryArgs args6 ( &uri4, EBrowser, EPartialSuffixPath );
	DoQueryL ( args6 );
	
	// Do query operation with partial prefix path match and without a list type
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with partial prefix path match & without ListType") );		
	AppendExpectedUriL ( KTestUri5(), EBrowser );
	const TPtrC8 uri5 ( KPartialPathPrefixUri() );
	TQueryArgs args7 ( &uri5, EBrowser, EPartialPrefixPath );
	DoQueryL ( args7 );
	
	// Do a domain search with List type - 2 in whitelist & 3 in blacklist for mydomain.com
	// Domain search with servicetype & listtype ( Browser & Whitelist )
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with domain match with ListType - WhiteList") );
	const TPtrC8 uri6 ( KDomainSearchStr() );
	AppendExpectedUriL ( KTestUri7(), EBrowser );
	AppendExpectedUriL ( KTestUri8(), EBrowser );	
	TQueryArgs args8 ( &uri6, EBrowser, EWhiteList, EDomain );
	DoQueryL ( args8 );	

	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with domain match with ListType - BlackList") );	
	const TPtrC8 uri7 ( KDomainSearchStr() );	
	AppendExpectedUriL ( KTestUri9(), EBrowser );
	AppendExpectedUriL ( KTestUri10(), EBrowser );	
	AppendExpectedUriL ( KTestUri11(), EBrowser );		
	TQueryArgs args9 ( &uri7, EBrowser, EBlackList, EDomain );
	DoQueryL ( args9 );	
	
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query URIs with domain match with no ListType") );		
	const TPtrC8 uri8 ( KDomainSearchStr() );		
	AppendExpectedUriL ( KTestUri7(), EBrowser );
	AppendExpectedUriL ( KTestUri8(), EBrowser );	
	AppendExpectedUriL ( KTestUri9(), EBrowser );
	AppendExpectedUriL ( KTestUri10(), EBrowser );	
	AppendExpectedUriL ( KTestUri11(), EBrowser );		
	TQueryArgs args10 ( &uri8, EBrowser, EDomain );
	DoQueryL ( args10 );	
	}

void CInetUriListQuery::DoQueryL ( TQueryArgs aArgs )
	{
	iInetUriList.QueryUriL ( aArgs, this );
	MatchQueryResults	();
	Cleanup	();
	}

void CInetUriListQuery::QueryTldListL()
	{
	DoQueryPolicyL();
	DoQueryListL();
	}

void CInetUriListQuery::DoQueryPolicyL()
	{
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri1 ( KTldTestUri1() );
	TPolicyQueryArgs queryArg1( &tlduri1, EWhiteList, EPolicyCharSet);
	TQueryResults PolicyDataArg1;
	DoQueryPolcydataL(queryArg1, PolicyDataArg1);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri2 ( KTldTestUri2() );
	TPolicyQueryArgs queryArg2( &tlduri2, EBlackList, EPolicyCharSet);
	TQueryResults PolicyDataArg2;
	DoQueryPolcydataL(queryArg2, PolicyDataArg2);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri3 ( KTldTestUri3() );
	TPolicyQueryArgs queryArg3( &tlduri3, EWhiteList, EPolicyCharSet);
	TQueryResults PolicyDataArg3;
	DoQueryPolcydataL(queryArg3, PolicyDataArg3);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri4 ( KTldTestUri4() );
	TPolicyQueryArgs queryArg4( &tlduri4, EBlackList, EPolicyCharSet);
	TQueryResults PolicyDataArg4;
	DoQueryPolcydataL(queryArg4, PolicyDataArg4);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri5 ( KTldTestUri5() );
	TPolicyQueryArgs queryArg5( &tlduri5, EWhiteList, EPolicyCharSet);
	TQueryResults PolicyDataArg5;
	DoQueryPolcydataL(queryArg5, PolicyDataArg5);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri6 ( KTldTestUri6() );
	TPolicyQueryArgs queryArg6( &tlduri6, EBlackList, EPolicyCharSet);
	TQueryResults PolicyDataArg6;
	DoQueryPolcydataL(queryArg6, PolicyDataArg6);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri7 ( KTldTestUri7() );
	TPolicyQueryArgs queryArg7( &tlduri7, EWhiteList, EPolicyCharSet);
	TQueryResults PolicyDataArg7;
	DoQueryPolcydataL(queryArg7, PolicyDataArg7);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri8 ( KTldTestUri7() );
	TPolicyQueryArgs queryArg8( &tlduri8, EBlackList, EPolicyCharSet);
	TQueryResults PolicyDataArg8;
	DoQueryPolcydataL(queryArg8, PolicyDataArg8);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri9 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg9( &tlduri9, EWhiteList, EPolicyCharSet);
	TQueryResults PolicyDataArg9;
	DoQueryPolcydataL(queryArg9, PolicyDataArg9);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri10 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg10( &tlduri10, EBlackList, EPolicyCharSet);
	TQueryResults PolicyDataArg10;
	DoQueryPolcydataL(queryArg10, PolicyDataArg10);

	//Negative tests
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs without Uri -- Negative test") );
	TPolicyQueryArgs queryArg11( EBlackList, EPolicyCharSet);
	TQueryResults PolicyDataArg11;
	DoQueryPolcydataL(queryArg11, PolicyDataArg11);
	
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs without List Type and TldQueryType -- Negative test") );
	const TPtrC8 tlduri12 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg12( &tlduri12 );
	TQueryResults PolicyDataArg12;
	DoQueryPolcydataL(queryArg12, PolicyDataArg12);
	
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs without URI & List Type  -- Negative test") );
	TPolicyQueryArgs queryArg13( EPolicyCharSet );
	TQueryResults PolicyDataArg13;
	DoQueryPolcydataL(queryArg13, PolicyDataArg13);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs without URI & TldQueryType -- Negative test") );
	TPolicyQueryArgs queryArg14( EBlackList );
	TQueryResults PolicyDataArg14;
	DoQueryPolcydataL(queryArg14, PolicyDataArg14);
	
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs without TldQueryType -- Negative test") );
	const TPtrC8 tlduri15 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg15( &tlduri15, EBlackList );
	TQueryResults PolicyDataArg15;
	DoQueryPolcydataL(queryArg15, PolicyDataArg15);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs without ListType -- Negative test") );
	const TPtrC8 tlduri16 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg16( &tlduri16, EPolicyCharSet );
	TQueryResults PolicyDataArg16;
	DoQueryPolcydataL(queryArg16, PolicyDataArg16);
	}

void CInetUriListQuery::DoQueryListL()
	{
	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi1 ( KTldTestUri1() );
	TPolicyQueryArgs query1( &tldlisturi1, EPolicyListType);
	TQueryResults resultList1;
	DoQueryTldListInfoL(query1, resultList1);

	__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi2 ( KTldTestUri2() );
	TPolicyQueryArgs query2( &tldlisturi2, EPolicyListType);
	TQueryResults resultList2;
	DoQueryTldListInfoL(query2, resultList2);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi3 ( KTldTestUri3() );
	TPolicyQueryArgs query3( &tldlisturi3, EPolicyListType);
	TQueryResults resultList3;
	DoQueryTldListInfoL(query3, resultList3);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi4 ( KTldTestUri4() );
	TPolicyQueryArgs query4( &tldlisturi4, EPolicyListType);
	TQueryResults resultList4;
	DoQueryTldListInfoL(query4, resultList4);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi5 ( KTldTestUri5() );
	TPolicyQueryArgs query5( &tldlisturi5, EPolicyListType);
	TQueryResults resultList5;
	DoQueryTldListInfoL(query5, resultList5);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi6 ( KTldTestUri6() );
	TPolicyQueryArgs query6( &tldlisturi6, EPolicyListType);
	TQueryResults resultList6;
	DoQueryTldListInfoL(query6, resultList6);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi7 ( KTldTestUri7() );
	TPolicyQueryArgs query7( &tldlisturi7, EPolicyListType);
	TQueryResults resultList7;
	DoQueryTldListInfoL(query7, resultList7);
	
		__PRINTIFNOTOOM__
	iTestWrapper.Next ( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	const TPtrC8 tldlisturi8 ( KTldTestUri8() );
	TPolicyQueryArgs query8( &tldlisturi8, EPolicyListType);
	TQueryResults resultList8;
	DoQueryTldListInfoL(query8, resultList8);
	}

/**
Tests Fetching Policy data for given Uri
*/
void CInetUriListQuery::DoQueryPolcydataL ( TPolicyQueryArgs aArgs, TQueryResults aResults )
	{
	TInt error(KErrNone);
	TRAP( error, iInetUriList.QueryTldInfoL ( aArgs, aResults ) );
	if(error == KErrNone )
		{
		HBufC8* policyData = aResults.CharsetL();
		delete policyData;	
		}
	iTestWrapper.TEST (ETrue);
	}

/**
Tests Fetching List type of given Uri
*/
void CInetUriListQuery::DoQueryTldListInfoL ( TPolicyQueryArgs aArgs, TQueryResults aResults )
	{
	TInt error(KErrNone);
	TRAP( error, iInetUriList.QueryTldInfoL ( aArgs, aResults ) );
		if(error == KErrNone )
		{
		InetUriList::TListType listType = aResults.ListType();
		}
	iTestWrapper.TEST (ETrue);
	}

TBool CInetUriListQuery::OnQueryResultsL ( RInetUri aUri )
	{
	TInt count = iResults.Count ();
	CleanupClosePushL ( aUri );
	iResults.AppendL ( aUri );
	CleanupStack::Pop (); // aUri
	return ETrue;
	}

void CInetUriListQuery::MatchQueryResults ()
	{
	iTestWrapper.TEST ( iExpectedResults.Count() == iResults.Count() );
	}

void CInetUriListQuery::Cleanup ()
	{
	ClearArray ( iResults );
	ClearArray ( iExpectedResults );		
	}

void CInetUriListQuery::ClearArray ( QueryResultsArray& aUriArray )
	{
	TInt count = aUriArray.Count ();
	while ( count > 0 )
		{
		aUriArray[--count].Close ();			
		aUriArray.Remove (count);
		}
	aUriArray.Reset ();	
	TInt count2 = aUriArray.Count ();
	}

void CInetUriListQuery::AppendExpectedUriL ( const TDesC8& aUri, TServiceType aServiceType )
	{
	RInetUri myInetUri = iInetUriList.OpenInetUriL ( aUri, aServiceType );
	CleanupClosePushL ( myInetUri );
	iExpectedResults.AppendL ( myInetUri );
	CleanupStack::Pop ();
	}

void CInetUriListQuery::DoCleanupL ()
	{
	Cleanup ();		
	}

// -----------------------------------------------------

CInetUriListCleanup* CInetUriListCleanup::NewL ( CTestWrapper& aTest )
	{
	CInetUriListCleanup* self = new ( ELeave )CInetUriListCleanup ( aTest );
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ();
	return self;
	}

CInetUriListCleanup::~CInetUriListCleanup ()
	{
		
	}

CInetUriListCleanup::CInetUriListCleanup ( CTestWrapper& aTest )
: CInetUriListTestsBase ( aTest, ETrue )
	{
		
	}

TBool CInetUriListCleanup::OnQueryResultsL ( RInetUri aUri )
	{
	CleanupClosePushL ( aUri );
	iInetUriList.RemoveL ( aUri );
	CleanupStack::PopAndDestroy (); // aUri
	return ETrue;
	}

void CInetUriListCleanup::DoOperationL ()
	{
	TQueryArgs args ( EBrowser );
	iInetUriList.QueryUriL ( args, this );
	TQueryArgs args2 ( EWapPush );	
	iInetUriList.QueryUriL ( args2, this );	
	}

void CInetUriListCleanup::DoCleanupL ()
	{
	// Do nothing
	}

// -----------------------------------------------------
