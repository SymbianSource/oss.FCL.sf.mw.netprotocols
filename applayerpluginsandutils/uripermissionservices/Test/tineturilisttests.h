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
// tbmtests.h
// 
//

#include <ineturilistdef.h>
#include <ineturilist.h>
class CTestWrapper;

using namespace InetUriList;

// ------------Test URIs
_LIT8 (KTestUri, "http://www.google.com/index.html" );
_LIT8 (KTestUri2, "http://www.yahoo.com/ab/cd/bc/index.html" );
_LIT8 (KTestUri3, "http://www.google.com/cd/tf/mypage.html" );

_LIT8 (KTestUri4, "http://www.mynewdomain.com/cd/tf/mypage.html" );
_LIT8 (KTestUri5, "http://www.mynewdomain.com/ab/tf/mypage.html" );
_LIT8 (KTestUri6, "http://www.mynewdomain.com/ef/tf/mypage.html" );

_LIT8 ( KTestUri7, "http://www.mydomain.com/index.html" );
_LIT8 ( KTestUri8, "http://pws.mydomain.com/index.html" );
_LIT8 ( KTestUri9, "http://mail.mydomain.com/index.html" );
_LIT8 ( KTestUri10, "http://messenger.mydomain.com/index.html" );
_LIT8 ( KTestUri11, "http://webdav.mydomain.com/index.html" );

_LIT8 ( KUriFavouriteName, "Google website" );
_LIT8 ( KUriFavouriteName2, "Yahoo website" );
_LIT8 ( KUriFavouriteName3, "My new domain" );

_LIT8 ( KUnknownUri, "http://myinvaliddomain.com/mypage.html" );
_LIT8 ( KPartialPathSuffixUri, "http://www.mynewdomain.com/mypage.html" );
_LIT8 ( KPartialPathPrefixUri, "http://www.mynewdomain.com/ab" );
_LIT8 ( KDomainSearchStr, "http://www.mydomain.com" );

//Test Uris For QueryTld functionality
_LIT8 ( KTldTestUri1, "http://www.yahoo.test/index.html" );	//Whitelisted in TLD 'test'
_LIT8 ( KTldTestUri2, "http://www.yaqQzz.test/index.html" );//Blacklisted in TLD 'test'
_LIT8 ( KTldTestUri3, "http://www.yahoo.twt/index.html" );	//Whitelisted in TLD 'twt'
_LIT8 ( KTldTestUri4, "http://www.yaqQzz.twt/index.html" );	//No Blacklist data for TLD 'twt'
_LIT8 ( KTldTestUri5, "http://www.yahoo.tbl/index.html" );	//Whitelisted in TLD 'tbl', No Whitelist data for TLD 'tbl' 
_LIT8 ( KTldTestUri6, "http://www.yaqQzz.tbl/index.html" );	//Blacklisted in TLD 'tbl', No Whitelist data for TLD 'tbl' 		
_LIT8 ( KTldTestUri7, "http://www.yahoo.tza/index.html" );	//TLD does not exist
_LIT8 ( KTldTestUri8, "http://www.yahoo.tzz/index.html" );	//TLD does not exist


_LIT ( KUriListInitFileSrcPath, "z:\\ineturilist\\config\\ineturilist.xml" );
_LIT ( KUriListInitFileDestPath, "c:\\private\\20009d70\\ineturilist.xml" );
// -------------------------

class CInetUriListTestsBase : public CBase
	{
	public:	
	virtual ~CInetUriListTestsBase ();
	
	void RunTestsL ();
//	
	protected:	
	CInetUriListTestsBase ( CTestWrapper& aTestWrapper, TBool aNormalRun );
	void ConstructL ();
	
	RInetUri CreateNewUriL ( const TDesC8& aUri, TServiceType aServiceType, TListType aListType );
	
	private:
	virtual void DoOperationL () = 0;
	virtual void DoCleanupL () = 0;	
	protected:	
	RInetUriList 	iInetUriList;
	CTestWrapper& 	iTestWrapper;
	TBool		  	iNormalRun;
	};


class CInetUriListStorageTests : public CInetUriListTestsBase 
	{
	public:
	static CInetUriListStorageTests* NewL ( CTestWrapper& aTestWrapper, TBool aNormalRun = ETrue );
	~CInetUriListStorageTests ();
		
	private:
	void DoOperationL ();
	void DoCleanupL ();
	
	CInetUriListStorageTests ( CTestWrapper& aTestWrapper, TBool aNormalRun );

	void AddTestsL ();
	void RemoveTestsL ();
	void UpdateTestsL ();
	void GetListTypeL ();
	void SameUriDiffServiceTypeTestsL ();
	void AddFewMoreUrisL ();
	};

typedef RArray <RInetUri> QueryResultsArray;

class CInetUriListQuery : public CInetUriListTestsBase,
						 public MQueryResultsCallback
	{
	public:
	static CInetUriListQuery* NewLC ( CTestWrapper& aTest, TBool aNormalRun = ETrue );
	virtual ~CInetUriListQuery ();

	TBool OnQueryResultsL ( RInetUri aUri );
		
	protected:
	CInetUriListQuery ( CTestWrapper& aTest, TBool aNormalRun );	
	CInetUriListQuery ( CTestWrapper& aTest, QueryResultsArray aResults );		
	
	private:
	void DoQueryL ( TQueryArgs aArgs );
	void DoQueryPolicyL();
	void DoQueryListL();		
	void DoQueryPolcydataL ( TPolicyQueryArgs aArgs, TQueryResults aResults );
	void DoQueryTldListInfoL ( TPolicyQueryArgs aArgs, TQueryResults aResults );
	
	void Cleanup ();
	void ClearArray ( QueryResultsArray& aUriArray );

	void AppendExpectedUriL ( const TDesC8& aUri, TServiceType aServiceType );
	void MatchQueryResults ();
	
	void DoOperationL ();	
	void QueryListL ();	
	void DoCleanupL ();	
	void QueryTldListL();
		
	private:
	QueryResultsArray iResults;
	QueryResultsArray iExpectedResults;
	};


class CInetUriListCleanup : public CInetUriListTestsBase,
						 	public MQueryResultsCallback
	{
	public:
	static CInetUriListCleanup* NewL ( CTestWrapper& aTest );
	~CInetUriListCleanup ();
	
	void DoOperationL ();
	void DoCleanupL ();	
	TBool OnQueryResultsL ( RInetUri aUri );
	
	private:
	CInetUriListCleanup ( CTestWrapper& aTest );
	};

