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
// ineturilistserverunit.cpp
// 
//

#include <ineturilistdef.h>

#include <s32mem.h>
#include <s32buf.h>
#include <sqldb.h>
#include "testutils.h"
#include "urilistinterface.h"
#include "urilistinitializer.h"
#include "ineturiimpl.h"
#include "ineturilistserver.h"
#include "uriqueryfilter.h"

// All URIs are tested against the browser servicetype

_LIT(KTitle, "InetUriList Server Unit Tests");

LOCAL_D RTest gTest( KTitle() );

LOCAL_D CTestWrapper* gTestWrapper;

typedef void ( *TTestFunctionL ) ();

#define __LEAVEIFOOM__ if ( err == KErrNoMemory ) \
				User::Leave ( err );

_LIT8 ( KTestUri, "http://www.symbian.com/developer/index.html" );
_LIT8 ( KFavouriteName, "Symbian Mobile Operating System" );
_LIT8 ( KFavouriteName2, "Symbian OS Phones" );
_LIT8 ( KFavouriteName3, "Symbian developer home" );
_LIT8 ( KFavouriteName4, "Symbian developer network" );
_LIT8 ( KFavouriteName5, "Symbian developer network - Tools" );
_LIT8 ( KFavouriteName6, "Symbian developer network - Library home" );
_LIT8 ( KFavouriteName7, "Symbian developer network - Academy" );
_LIT8 ( KFavouriteName8, "Symbian developer network - Library" );
_LIT8 ( KFavouriteName9, "Symbian wiki" );
_LIT8 ( KFavouriteName10, "All about Symbian - getting started" );

// For doamain search
_LIT8 ( KTestUri2, "http://www.symbian.com/phones/index.html" );
_LIT8 ( KTestUri3, "http://developer.symbian.com/home.action" );

// For domain & path search
_LIT8 ( KTestUri4, "http://developer.symbian.com/main/getstarted/index.jsp" );
_LIT8 ( KTestUri5, "http://developer.symbian.com/main/tools/index.jsp" );
_LIT8 ( KTestUri6, "http://developer.symbian.com/main/oslibrary/index.jsp" );
_LIT8 ( KTestUri7, "http://developer.symbian.com/main/academy/symbian_academy/curriculum/index.jsp" );
_LIT8 ( KTestUri8, "http://developer.symbian.com/main/oslibrary/" );
_LIT8 ( KTestUri9, "http://wiki.symbian.com/mypage.html" );
_LIT8 ( KTestUri10, "http://www.allaboutsymbian.com/main/getstarted/index.jsp" );


// Unknown URI. should return KErrUriNotFound
_LIT8 ( KUnknownUri, "http://www.symbian.com/symbianos/index.html" );

//Test Uris For QueryTld functionality
_LIT8 ( KTldTestUri1, "http://www.yahoo.test/index.html" );	//Whitelisted in TLD 'test'
_LIT8 ( KTldTestUri2, "http://www.yaqQzz.test/index.html" );//Blacklisted in TLD 'test'
_LIT8 ( KTldTestUri3, "http://www.yahoo.twt/index.html" );	//Whitelisted in TLD 'twt'
_LIT8 ( KTldTestUri4, "http://www.yaqQzz.twt/index.html" );	//No Blacklist data for TLD 'twt'
_LIT8 ( KTldTestUri5, "http://www.yahoo.tbl/index.html" );	//Whitelisted in TLD 'tbl', No Whitelist data for TLD 'twt' 
_LIT8 ( KTldTestUri6, "http://www.yaqQzz.tbl/index.html" );	//Blacklisted in TLD 'tbl', No Whitelist data for TLD 'twt' 		
_LIT8 ( KTldTestUri7, "http://www.yahoo.tza/index.html" );	//TLD does not exist
_LIT8 ( KTldTestUri8, "http://www.yahoo.tzz/index.html" );	//TLD does not exist

// Added For DEF141504 
_LIT8 ( KTldTestUri9, "www.yahoo.test/index.html" ); //Whitelisted in TLD 'test'
_LIT8 ( KTldTestUri10, "www.yaqQzz.test/index.html" );//Blacklisted in TLD 'test'
_LIT8 ( KTldTestUri11, "www.yahoo.tzz/index.html" );  //TLD does not exist
CUriListInterface *gListInterface;
TBool gNormalRun = ETrue;

using namespace InetUriList;

LOCAL_C void Print ( const TDesC& aStmt )
	{
	if ( gNormalRun )
		gTestWrapper->Next ( aStmt );
	}

LOCAL_C void DeleteDatabase ()
	{
	_LIT( KDatabaseName, "[A00026FF]URIList.dat" );
	RSqlDatabase::Delete ( KDatabaseName() );		
	}

/*LOCAL_C void TestUriListInitializerL ()
	{
	gListInterface = CUriListInterface::NewL ();
//	delete listInterface;
	}*/

LOCAL_C CInetUriImpl* CreateUriL ( const TDesC8& aUri, TListType aListType, const TDesC8& aFavouriteName )
	{
	CInetUriImpl* uri = CInetUriImpl::NewL ( aUri, EBrowser, aListType );
	CleanupStack::PushL ( uri );
	uri->Properties().SetFavouriteNameL ( aFavouriteName );	
	CleanupStack::Pop ();
	return uri;	
	}

LOCAL_C void AddTestL ( CInetUriImpl* aUri, TInt aExpectedError )
	{
	TRAPD ( err, gListInterface->InsertL ( *aUri ) );		
	__LEAVEIFOOM__
	
	gTestWrapper->TEST ( err == aExpectedError );
	}

LOCAL_C void UpdateTestL ( CInetUriImpl* aUri, TInt aExpectedError )
	{
	TRAPD ( err, gListInterface->UpdateUriL ( aUri->Properties().PropId(), aUri->Properties().ListType(), aUri->Properties().FavouriteName() ) );
	__LEAVEIFOOM__
	
	gTestWrapper->TEST ( err == aExpectedError );	
	}

LOCAL_C void RemoveTestL ( CInetUriImpl* aUri, TInt aExpectedError )
	{
	TRAPD ( err, gListInterface->RemoveUriL ( aUri->UriId(), aUri->Properties().PropId() ) );
	__LEAVEIFOOM__
	
	gTestWrapper->TEST ( err == aExpectedError );	
		
	}

LOCAL_C void GetListTypeTestL ( const TDesC8& aUri, TInt aExpectedError )
	{
	TListType lt = EBlackList;
	TRAPD ( err, lt = gListInterface->GetListTypeL ( aUri, EBrowser ) );
	__LEAVEIFOOM__
	gTestWrapper->TEST ( err == aExpectedError );
	if ( aExpectedError == KErrNone )
		gTestWrapper->TEST ( lt == EWhiteList );			
	}

LOCAL_C CInetUriImpl* OpenUriL ( const TDesC8& aUri )
	{
	TQueryArgs args ( EBrowser, EExact );
	CUriQueryFilter* queryFilter = NULL;
	TRAPD ( err, queryFilter = gListInterface->QueryWithUriL ( aUri, args ) );
	__LEAVEIFOOM__
	
	gTestWrapper->TEST ( err == KErrNone );
	CleanupStack::PushL ( queryFilter );	
	CUriListStream* listStream = CUriListStream::NewL ( queryFilter );
	CleanupStack::Pop (); // queryFilter;
	CleanupStack::PushL ( listStream );
	CBufFlat* buffer = CBufFlat::NewL ( KIpcDataSize );
	CleanupStack::PushL ( buffer );
	buffer->ExpandL ( 0, KIpcDataSize );
	TPtrC8 ptr;
	TInt count = listStream->WriteUriListL ( ptr );
	if ( count <= 0 )
		{
		User::Leave ( KErrUriNotFound );
		}
	buffer->Ptr(0).Copy ( ptr );
	
	RBufReadStream readStream( *buffer );
	CleanupClosePushL ( readStream );
	CInetUriImpl* uri = CInetUriImpl::NewL ();
	CleanupStack::PushL ( uri );
	uri->UnpackL ( readStream );
	CleanupStack::Pop (); // uri
	CleanupStack::PopAndDestroy (3); // buffer, listStream
	return uri;	
	}

/*LOCAL_C void CountUriL ()
	{
	gTestWrapper->TEST ( gListInterface->CountUriL ( InetUriList::EBrowser, InetUriList::EWhiteList ) == 1 );
	}*/
	
LOCAL_C void MatchUris ( const CInetUriImpl& aUri, const CInetUriImpl& aUri2 )
	{
	gTestWrapper->TEST ( aUri.UriDes().Compare( aUri2.UriDes() ) == 0 );
	CInetUriProperties& properties = aUri.Properties();
	CInetUriProperties& properties2 = aUri2.Properties();
	gTestWrapper->TEST ( properties.Permission() == properties2.Permission() );
	gTestWrapper->TEST ( properties.ListType() == properties2.ListType() );
	gTestWrapper->TEST ( properties.ServiceType() == properties2.ServiceType() );
	gTestWrapper->TEST ( properties.FavouriteName().Compare( properties2.FavouriteName() ) == 0 );
	}

LOCAL_C void DoStorageTestsL ()
	{	
	DeleteDatabase ();
	gListInterface = CUriListInterface::NewL ();
	CleanupStack::PushL ( gListInterface );
	
	Print ( _L( "Doing URI add tests" ) );
	CInetUriImpl* uri = CreateUriL ( KTestUri(), EWhiteList, KFavouriteName() );
	CleanupStack::PushL ( uri );
	AddTestL ( uri, KErrNone );
	AddTestL ( uri, KErrUriAlreadyExists );	
	
	CInetUriImpl* uri2 = NULL;
	TRAPD ( err, uri2 = OpenUriL ( KTestUri() ) );
	__LEAVEIFOOM__
	gTestWrapper->TEST ( err == KErrNone );
	CleanupStack::PushL ( uri2 );
	MatchUris ( *uri, *uri2 );
	
	Print ( _L( "Doing URI update tests" ) );	
	// Update tests. Update the favouritename
	uri2->Properties().SetFavouriteNameL ( KFavouriteName2() );
		
	UpdateTestL	( uri2, KErrNone );
	CInetUriImpl* uri3 = NULL;

	TRAP ( err, uri3 = OpenUriL ( KTestUri() ) );
	__LEAVEIFOOM__	
	gTestWrapper->TEST ( err == KErrNone );
	
	CleanupStack::PushL ( uri3 );
	MatchUris ( *uri3, *uri2 );
	
	Print ( _L( "Doing retrieval of URI list type tests" ) );	
	GetListTypeTestL ( KTestUri(), KErrNone );
	GetListTypeTestL ( KUnknownUri(), KErrUriNotFound );
	
	Print ( _L( "Doing URI remove tests" ) );	
	RemoveTestL ( uri3, KErrNone );
	CInetUriImpl* uri4 = NULL;
	TRAP ( err, uri4 = OpenUriL ( KTestUri() ) );
	CleanupStack::PushL ( uri4 );
	__LEAVEIFOOM__
	gTestWrapper->TEST ( err == KErrUriNotFound );
	
	CleanupStack::PopAndDestroy (5); //uri, uri2, uri3, uri4, gListInterface		
	}

LOCAL_C void AddMoreUrisTestL ()
	{	
	gListInterface = CUriListInterface::NewL ();
	CleanupStack::PushL ( gListInterface );
	
	CInetUriImpl* uri = CreateUriL ( KTestUri(), EWhiteList, KFavouriteName() );
	CleanupStack::PushL ( uri );
	CInetUriImpl* uri2 = CreateUriL ( KTestUri2(), EWhiteList, KFavouriteName2() );
	CleanupStack::PushL ( uri2 );
	CInetUriImpl* uri3 = CreateUriL ( KTestUri3(), EWhiteList, KFavouriteName3() );
	CleanupStack::PushL ( uri3 );
	CInetUriImpl* uri4 = CreateUriL ( KTestUri4(), EWhiteList, KFavouriteName4() );
	CleanupStack::PushL ( uri4 );
	CInetUriImpl* uri5 = CreateUriL ( KTestUri5(), EWhiteList, KFavouriteName5() );
	CleanupStack::PushL ( uri5 );
	CInetUriImpl* uri6 = CreateUriL ( KTestUri6(), EWhiteList, KFavouriteName6() );
	CleanupStack::PushL ( uri6 );
	CInetUriImpl* uri7 = CreateUriL ( KTestUri7(), EWhiteList, KFavouriteName7() );
	CleanupStack::PushL ( uri7 );
	CInetUriImpl* uri8 = CreateUriL ( KTestUri8(), EWhiteList, KFavouriteName8() );
	CleanupStack::PushL ( uri8 );
	CInetUriImpl* uri9 = CreateUriL ( KTestUri9(), EWhiteList, KFavouriteName9() );
	CleanupStack::PushL ( uri9 );
	CInetUriImpl* uri10 = CreateUriL ( KTestUri10(), EWhiteList, KFavouriteName10() );
	CleanupStack::PushL ( uri10 );

	
	AddTestL ( uri, KErrNone );
	AddTestL ( uri2, KErrNone );
	AddTestL ( uri3, KErrNone );
	AddTestL ( uri4, KErrNone );
	AddTestL ( uri5, KErrNone );
	AddTestL ( uri6, KErrNone );
	AddTestL ( uri7, KErrNone );
	AddTestL ( uri8, KErrNone );
	AddTestL ( uri10, KErrNone );
	AddTestL ( uri9, KErrNone );
			
	CleanupStack::PopAndDestroy ( 11 ); // gListInterface, uri1 - uri11	
 	}

typedef CArrayFixFlat <CInetUriImpl*> TResultList;
const TInt KMaxArraySize = 20;
static void ResetAndDestroyResultList ( TAny* aPtr )
	{
	TResultList* resultList = ( TResultList* )aPtr;
	// Delete all items added. Otherwise these items will not get deleted and there will
	// be a memory leak.
	for ( TInt i = 0; i < resultList->Count(); ++i )
		delete resultList->At(i);

	delete resultList;
	}


LOCAL_C TResultList* RetrieveQueryResultsLC ( CUriListStream& aListStream )
	{
	TResultList* resultsArray = new ( ELeave ) TResultList ( KMaxArraySize );
	CleanupStack::PushL ( TCleanupItem ( ResetAndDestroyResultList, resultsArray ) );
	CBufFlat* buffer = CBufFlat::NewL ( KIpcDataSize );
	CleanupStack::PushL ( buffer );
	buffer->ExpandL ( 0, KIpcDataSize );
	RBufReadStream readStream( *buffer );
	CleanupClosePushL ( readStream );
	TBool bContinue = ETrue;
	do
	{
		TPtrC8 ptr;
		TInt totalRecords = aListStream.WriteUriListL ( ptr );
		if ( totalRecords > 0 )
			{
			buffer->Ptr(0).Copy ( ptr );				
			for ( TInt i = 0; i < totalRecords; ++i )
				{
				CInetUriImpl* uri = CInetUriImpl::NewL ();
				CleanupStack::PushL ( uri );
				uri->UnpackL ( readStream );
				// Append to the resutls array				
				resultsArray->AppendL ( uri );
				CleanupStack::Pop (); // uri
				MStreamBuf* srcStream = readStream.Source ();
				srcStream->SeekL ( MStreamBuf::ERead, EStreamBeginning, 0 );
				}
			}
			else
				{
				bContinue = EFalse;					
				}
	}while ( bContinue );
	CleanupStack::PopAndDestroy ( 2 ); //buffer, readStream
	return resultsArray;
	}

LOCAL_C void DoMatchResultArray ( const TResultList& aResult, const TResultList& aResult2 )
	{
	gTestWrapper->TEST ( aResult.Count() == aResult2.Count() );

	for ( TInt i = 0; i < aResult.Count(); ++i )	
		{
		CInetUriImpl* uri = aResult.At(i);
		for ( TInt j = 0; j < aResult2.Count(); ++j )
			{
			CInetUriImpl* uri2 = aResult2.At(j);
			if ( uri->Properties().PropId() == uri2->Properties().PropId() )
				{
				MatchUris ( *uri, *uri2 );
				break;
				}
			}
		}
	}

LOCAL_C void DoSearchL ( const TDesC8& aUri, TURIMatch aMatch, const TResultList& aExpectedResults )
	{
	TQueryArgs args ( EBrowser, aMatch );
	CUriQueryFilter* queryFilter = gListInterface->QueryWithUriL ( aUri, args );
	CleanupStack::PushL ( queryFilter );
	CUriListStream* listStream = CUriListStream::NewL ( queryFilter );
	CleanupStack::Pop (); // queryFilter
	CleanupStack::PushL ( listStream );
	TResultList* resultArray = RetrieveQueryResultsLC (*listStream);
	DoMatchResultArray ( aExpectedResults, *resultArray );
	CleanupStack::PopAndDestroy ( 2 ); // listStream, resultArray
	}

/**
Tests Fetching Policy data for given Uri
*/
LOCAL_C void DoQueryPolcydataL (const TDesC8& aUri, TPolicyQueryArgs aArgs )
	{
	__UHEAP_MARK;
	gListInterface = CUriListInterface::NewL ();
	CleanupStack::PushL ( gListInterface );
	TInt error(KErrNone);
	HBufC8* policyData(NULL);
	TRAP( error, policyData = gListInterface->QueryTldInfoL (aUri, aArgs ) );
	delete policyData;
	gTestWrapper->TEST ( ETrue );
	CleanupStack::PopAndDestroy ( gListInterface );
	__UHEAP_MARKEND;
	}

/**
Tests Fetching List type of given Uri
*/
LOCAL_C void DoQueryTldListInfoL ( const TDesC8& aUri )
	{
	gListInterface = CUriListInterface::NewL ();
	CleanupStack::PushL ( gListInterface );
	TInt error(KErrNone);
	InetUriList::TListType listType(InetUriList::EBlackList);
	TRAP( error, listType = gListInterface->GetHostTypeL ( aUri ) );
	gTestWrapper->TEST ( ETrue );
	CleanupStack::PopAndDestroy ( gListInterface );
	}
	
/**
Tests Fetching List type of given Uri
*/
LOCAL_C void DoPolicyLengthL ( const TDesC8& aUri, TPolicyQueryArgs aArgs )
	{
	TInt error(KErrNone);
	TInt length(0);
	gListInterface = CUriListInterface::NewL ();
	CleanupStack::PushL ( gListInterface );
	TRAP( error, length = gListInterface->PolicyDataLengthL( aUri, aArgs ) );
	gTestWrapper->TEST ( ETrue );
	CleanupStack::PopAndDestroy ( gListInterface );
	}

LOCAL_C void DoQueryPolicyL()
	{
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri1 ( KTldTestUri1() );
	TPolicyQueryArgs queryArg1( &tlduri1, EWhiteList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri1, queryArg1 );
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri2 ( KTldTestUri2() );
	TPolicyQueryArgs queryArg2( &tlduri2, EBlackList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri2, queryArg2);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri3 ( KTldTestUri3() );
	TPolicyQueryArgs queryArg3( &tlduri3, EWhiteList, EPolicyCharSet);
	DoQueryPolcydataL( tlduri3, queryArg3);
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri4 ( KTldTestUri4() );
	TPolicyQueryArgs queryArg4( &tlduri4, EBlackList, EPolicyCharSet);
	DoQueryPolcydataL( tlduri4, queryArg4);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri5 ( KTldTestUri5() );
	TPolicyQueryArgs queryArg5( &tlduri5, EWhiteList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri5, queryArg5);
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri6 ( KTldTestUri6() );
	TPolicyQueryArgs queryArg6( &tlduri6, EBlackList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri6, queryArg6);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri7 ( KTldTestUri7() );
	TPolicyQueryArgs queryArg7( &tlduri7, EWhiteList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri7, queryArg7);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri8 ( KTldTestUri7() );
	TPolicyQueryArgs queryArg8( &tlduri8, EBlackList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri8, queryArg8);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri9 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg9( &tlduri9, EWhiteList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri9, queryArg9);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	const TPtrC8 tlduri10 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg10( &tlduri10, EBlackList, EPolicyCharSet);
	DoQueryPolcydataL(tlduri10, queryArg10);
	
	// Addede to Test DEF141504
	
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	    const TPtrC8 tlduri11 ( KTldTestUri9() );
	    TPolicyQueryArgs queryArg11( &tlduri11, EBlackList, EPolicyCharSet);
	    DoQueryPolcydataL(tlduri11, queryArg11);
	    
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	     const TPtrC8 tlduri12 ( KTldTestUri10() );
	     TPolicyQueryArgs queryArg12( &tlduri12, EBlackList, EPolicyCharSet);
	     DoQueryPolcydataL(tlduri12, queryArg12);
	
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch BlackList data") );
	     const TPtrC8 tlduri14 ( KTldTestUri11() );
	     TPolicyQueryArgs queryArg14( &tlduri14, EBlackList, EPolicyCharSet);
	     DoQueryPolcydataL(tlduri14, queryArg14);
	}

LOCAL_C void DoQueryListL ()
	{
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL(KTldTestUri1());

	
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL( KTldTestUri2() );
	
		
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL( KTldTestUri3() );
	
		
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL( KTldTestUri4() );
	
		
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL( KTldTestUri5() );
	
		
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL( KTldTestUri6() );
	
		
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL( KTldTestUri7() );
	
		
	Print( _L ( "Query TLD URIs with Uri and TldQueryType -- Gets White/Black list Host") );
	DoQueryTldListInfoL( KTldTestUri8() );
	}

LOCAL_C void DoPolicyDataLengthL ()
	{
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri1 ( KTldTestUri1() );
	TPolicyQueryArgs queryArg1( &tlduri1, EWhiteList, EPolicyCharSet);
	DoPolicyLengthL( tlduri1, queryArg1 );
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri2 ( KTldTestUri2() );
	TPolicyQueryArgs queryArg2( &tlduri2, EBlackList, EPolicyCharSet);
	DoPolicyLengthL(tlduri2, queryArg2);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri3 ( KTldTestUri3() );
	TPolicyQueryArgs queryArg3( &tlduri3, EWhiteList, EPolicyCharSet);
	DoPolicyLengthL(tlduri3, queryArg3);
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri4 ( KTldTestUri4() );
	TPolicyQueryArgs queryArg4( &tlduri4, EBlackList, EPolicyCharSet);
	DoPolicyLengthL(tlduri4, queryArg4);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri5 ( KTldTestUri5() );
	TPolicyQueryArgs queryArg5( &tlduri5, EWhiteList, EPolicyCharSet);
	DoPolicyLengthL(tlduri5, queryArg5);
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri6 ( KTldTestUri6() );
	TPolicyQueryArgs queryArg6( &tlduri6, EBlackList, EPolicyCharSet);
	DoPolicyLengthL(tlduri6, queryArg6);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri7 ( KTldTestUri7() );
	TPolicyQueryArgs queryArg7( &tlduri7, EWhiteList, EPolicyCharSet);
	DoPolicyLengthL(tlduri7, queryArg7);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri8 ( KTldTestUri7() );
	TPolicyQueryArgs queryArg8( &tlduri8, EBlackList, EPolicyCharSet);
	DoPolicyLengthL(tlduri8, queryArg8);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri9 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg9( &tlduri9, EWhiteList, EPolicyCharSet);
	DoPolicyLengthL(tlduri9, queryArg9);
	
		
	Print(  _L ( "Query TLD URIs with Uri, ListType and TldQueryType -- Fetch WhiteList data") );
	const TPtrC8 tlduri10 ( KTldTestUri8() );
	TPolicyQueryArgs queryArg10( &tlduri10, EBlackList, EPolicyCharSet);
	DoPolicyLengthL(tlduri10, queryArg10);
	}
	
LOCAL_C void DoQueryTLDTestsL ()
	{
	DoQueryPolicyL();
	DoQueryListL();
	DoPolicyDataLengthL();	
	}
	
LOCAL_C void DoQueryTestsL ()
	{
	// Query tests performed 
	// For domain search:
	// A URI with http://www.symbian.com/ should return KTestUri - KTestUri10
	// For path search:
	// Perfix path: http://developer.symbian.com/main should return KTestUri4 - KTestUri8 & KTestUri11 ( 6 URIs )
	// Suffix path: http://developer.symbian.com/index.jsp should return KTestUri4 - KTestUri7 & KTestUri10  ( 5 URIs )
	// Exact path: http://developer.symbian.com/main/getstarted/index.jsp should return KTestUri4 & KTestUri10 ( 2 URIs )

	_LIT8 ( KDomainSearchUri, "http://www.symbian.com/" );
	_LIT8 ( KPrefixPathSearchUri, "http://developer.symbian.com/main" );
	_LIT8 ( KSuffixPathSearchUri, "http://developer.symbian.com/index.jsp" );
	_LIT8 ( KExactPathSearchUri, "http://developer.symbian.com/main/getstarted/index.jsp" );
	
	gListInterface = CUriListInterface::NewL ();
	CleanupStack::PushL ( gListInterface );
	
	TInt count = gListInterface->CountUriL ( EBrowser, EWhiteList );
	
	CInetUriImpl* uri = OpenUriL ( KTestUri() );
	CleanupStack::PushL ( uri );
	CInetUriImpl* uri2 = OpenUriL ( KTestUri2() );
	CleanupStack::PushL ( uri2 );
	CInetUriImpl* uri3 = OpenUriL ( KTestUri3() );
	CleanupStack::PushL ( uri3 );
	CInetUriImpl* uri4 = OpenUriL ( KTestUri4() );
	CleanupStack::PushL ( uri4 );
	CInetUriImpl* uri5 = OpenUriL ( KTestUri5() );
	CleanupStack::PushL ( uri5 );
	CInetUriImpl* uri6 = OpenUriL ( KTestUri6() );
	CleanupStack::PushL ( uri6 );
	CInetUriImpl* uri7 = OpenUriL ( KTestUri7() );
	CleanupStack::PushL ( uri7 );
	CInetUriImpl* uri8 = OpenUriL ( KTestUri8() );
	CleanupStack::PushL ( uri8 );
	CInetUriImpl* uri9 = OpenUriL ( KTestUri9() );
	CleanupStack::PushL ( uri9 );
	CInetUriImpl* uri10 = OpenUriL ( KTestUri10() );
	CleanupStack::PushL ( uri10 );
	
	TResultList* domainSearchResults = new ( ELeave )TResultList ( KMaxArraySize );
	CleanupStack::PushL ( domainSearchResults );
	domainSearchResults->AppendL ( uri );
	domainSearchResults->AppendL ( uri2 );
	domainSearchResults->AppendL ( uri3 );
	domainSearchResults->AppendL ( uri4 );			
	domainSearchResults->AppendL ( uri5 );
	domainSearchResults->AppendL ( uri6 );
	domainSearchResults->AppendL ( uri7 );
	domainSearchResults->AppendL ( uri8 );			
	domainSearchResults->AppendL ( uri9 );

	TResultList* prefixPathSearchResults = new ( ELeave )TResultList ( KMaxArraySize );;
	CleanupStack::PushL ( prefixPathSearchResults );
	prefixPathSearchResults->AppendL ( uri4 );			
	prefixPathSearchResults->AppendL ( uri5 );
	prefixPathSearchResults->AppendL ( uri6 );
	prefixPathSearchResults->AppendL ( uri7 );
	prefixPathSearchResults->AppendL ( uri8 );			
	prefixPathSearchResults->AppendL ( uri10 );

	TResultList* suffixPathSearchResults = new ( ELeave )TResultList ( KMaxArraySize );;
	CleanupStack::PushL ( suffixPathSearchResults );
	suffixPathSearchResults->AppendL ( uri4 );			
	suffixPathSearchResults->AppendL ( uri5 );
	suffixPathSearchResults->AppendL ( uri6 );
	suffixPathSearchResults->AppendL ( uri7 );
	suffixPathSearchResults->AppendL ( uri10 );
	
	TResultList* exactPathSearchResults = new ( ELeave )TResultList ( KMaxArraySize );;
	CleanupStack::PushL ( exactPathSearchResults );	
	exactPathSearchResults->AppendL ( uri4 );
	exactPathSearchResults->AppendL ( uri10 );
 	Print( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0005 Doing domain search test" ) );	
	DoSearchL ( KDomainSearchUri(), EDomain, *domainSearchResults );
  	Print( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0006 Doing partial prefix path search test" ) );		
	DoSearchL ( KPrefixPathSearchUri(), EPartialPrefixPath, *prefixPathSearchResults );
	Print( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0007 Doing partial suffix path search test" ) );			
	DoSearchL ( KSuffixPathSearchUri(), EPartialSuffixPath, *suffixPathSearchResults );
	Print( _L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0008 Doing exact path search test" ) );			
	DoSearchL ( KExactPathSearchUri(), EExactPath, *exactPathSearchResults );
	CleanupStack::PopAndDestroy ( 4 );  // domainSearchResults, prefixPathSearchResults, suffixPathSearchResults, exactPathSearchResults
	CleanupStack::PopAndDestroy ( 11); 	 // uris & gListInterface	
	}


LOCAL_C void DoOomTestsL ( TTestFunctionL aTestFunction )
	{
	TInt err = KErrNoMemory;
	TInt failAt = 0;

	while( err != KErrNone )
		{
   		gTestWrapper->Tester().Printf( _L( "Test step: %d\n" ), failAt );
    	__UHEAP_RESET;
		__UHEAP_MARK;
		__UHEAP_SETFAIL( RHeap::EDeterministic, failAt ); 
		TRAP( err, ( aTestFunction )() );
		__UHEAP_MARKEND;
		__UHEAP_RESET;			
		++failAt;
		}
    gTestWrapper->Tester().Printf( _L( "End of OOM testing Total iterations: %d" ), failAt );		
	}


LOCAL_C void StartTestsL ()
	{	
	// Run normal tests
	gTestWrapper->Next (_L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0001 Doing storage tests" ));
	DoStorageTestsL ();
	gTestWrapper->Next (_L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0002 Adding URIs for query testing" ));	
	AddMoreUrisTestL ();
	gTestWrapper->Tester().Printf (_L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0003 Starting query tests" ));	
	DoQueryTestsL ();
	gTestWrapper->Tester().Printf (_L( "@SYMTestCaseID IWS-APPPROTOCOLS-INETURILIST-SERVERUNIT-0004 Starting query TLD tests" ));
	DoQueryTLDTestsL();

	// Running OOM tests
	gNormalRun = EFalse;
	gTestWrapper->Next (_L( "Starting OOM testing" ));
	DoOomTestsL ( DoStorageTestsL );
	AddMoreUrisTestL ();
	DoOomTestsL ( DoQueryTestsL );
	DoOomTestsL ( DoQueryTLDTestsL );
	}

LOCAL_C void DoTestsL ()
	{	
	gTestWrapper = CTestWrapper::NewLC(gTest);
	gTestWrapper->Start(_L("URI List Server tests"));

	TRAPD ( err, StartTestsL () );
	gTestWrapper->TESTE( err==KErrNone, err );
	
	gTestWrapper->End();
	CleanupStack::PopAndDestroy(gTestWrapper);
	}


GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* tc = CTrapCleanup::New();
	TRAPD( err, DoTestsL() );
	if ( err != KErrNone )
		User::Panic( _L( "Test failed with error code: %i" ), err );
	delete tc;
	__UHEAP_MARKEND;
	return KErrNone;
	}
