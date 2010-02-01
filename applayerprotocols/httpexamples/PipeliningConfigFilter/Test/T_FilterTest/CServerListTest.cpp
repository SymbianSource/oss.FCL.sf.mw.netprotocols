// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CServerListTest.h"
#include "CDomainList.h"
#include <domainstrings.h>

const TInt KMaxListSize = 25;

CServerListTest* CServerListTest::NewL()
	{
	CServerListTest* self = new(ELeave) CServerListTest();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CServerListTest::~CServerListTest()
	{
	delete iDomainList;
	iStringPool.Close();
	}
	
CServerListTest::CServerListTest()
	:iStringTable(DOMAINSTRINGS::Table)
	{
	}
	
void CServerListTest::ConstructL()
	{
	iStringPool.OpenL(iStringTable);
	}

const TDesC& CServerListTest::TestName()
	{
	_LIT(KTestName, "Domain List Test");
	return KTestName();
	}

void CServerListTest::DoRunL()
	{
	RunTestCase1L();
	RunTestCase2L();
	RunTestCase3L();
	RunTestCase4L();
	RunTestCase5L();
	RunTestCase6L();
	}

void CServerListTest::DoCancel()
	{
	}

void CServerListTest::ResetListL()
	{
	delete iDomainList;
	iDomainList = CDomainList::NewL(KMaxListSize);
	}

void CServerListTest::RunTestCase1L()
	{
	// Test Case 1 - Add unique five domains to empty list
	iEngine->Utils().LogIt(_L("***** Test Case 1 - Add to empty list *****\n"));
	ResetListL();
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain1, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain2, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain3, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain4, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain5, iStringTable));
	iEngine->Utils().LogIt(_L("Added 5 unique domains\n"));
	
	// Test validation
	TInt domainCount = iDomainList->Count();
	iEngine->Utils().LogIt(_L("Expected 5 domains, Actual: %d\n"), domainCount);
	if( domainCount != 5 )
		{
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	}

void CServerListTest::RunTestCase2L()
	{
	// Test Case 2 - Ensure duplicates are not added
	iEngine->Utils().LogIt(_L("***** Test Case 2 - No duplicates *****\n"));
	ResetListL();
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain1, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain1, iStringTable));
	iEngine->Utils().LogIt(_L("Added 2 identical domains\n"));
	
	// Test validation
	TInt domainCount = iDomainList->Count();
	iEngine->Utils().LogIt(_L("Expected 1 domains, Actual: %d\n"), domainCount);
	if( domainCount != 1 )
		{
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	}

void CServerListTest::RunTestCase3L()
	{
	// Test Case 3 - Adding domains to a full list
	iEngine->Utils().LogIt(_L("***** Test Case 3 - Adding to a full list *****\n"));
	ResetListL();
	
	// Loop through and domains to fill the list
	for(TInt ii=0; ii<KMaxListSize; ++ii)
		{
		iDomainList->AddDomainToListL(iStringPool.StringF(ii, iStringTable));	
		}
	TInt domainCount = iDomainList->Count();
	iEngine->Utils().LogIt(_L("List size (Before): %d\n"), domainCount);
	
	// Add 2 more domain to ensure above the max size of list
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain26, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain27, iStringTable));

	// Test validation
	TInt afterDomainCount = iDomainList->Count();
	iEngine->Utils().LogIt(_L("List size (After): %d\n"), afterDomainCount);
	if( domainCount != afterDomainCount )
		{
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
		
	iEngine->Utils().LogIt(_L("Checking domains are valid...\n"));
	if( iDomainList->IsDomainInList(iStringPool.StringF(DOMAINSTRINGS::EDomain1, iStringTable)) != KErrNotFound )
		{
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	if( iDomainList->IsDomainInList(iStringPool.StringF(DOMAINSTRINGS::EDomain2, iStringTable)) != KErrNotFound )
		{
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	if( iDomainList->IsDomainInList(iStringPool.StringF(DOMAINSTRINGS::EDomain26, iStringTable)) == KErrNotFound )
		{
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	if( iDomainList->IsDomainInList(iStringPool.StringF(DOMAINSTRINGS::EDomain27, iStringTable)) == KErrNotFound )
		{
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	}

void CServerListTest::RunTestCase4L()
	{
	// Test Case 4 - Search for existing domain
	iEngine->Utils().LogIt(_L("***** Test Case 4 - Search for existing domain *****\n"));
	ResetListL();
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain1, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain2, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain3, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain4, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain5, iStringTable));
	iEngine->Utils().LogIt(_L("Added 5 unique domains and searching for 3.\n"));
	
	// Test validation
	if( iDomainList->IsDomainInList(iStringPool.StringF(DOMAINSTRINGS::EDomain3, iStringTable)) == KErrNotFound )
		{
		iEngine->Utils().LogIt(_L("Domain NOT found."));
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	}

void CServerListTest::RunTestCase5L()
	{
	// Test Case 5 - Search for non-existing domain
	iEngine->Utils().LogIt(_L("***** Test Case 5 - Search for non-existing domain *****\n"));
	ResetListL();
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain1, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain2, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain3, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain4, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain5, iStringTable));
	iEngine->Utils().LogIt(_L("Added 5 unique domains and searching for 6.\n"));
	
	// Test validation
	if( iDomainList->IsDomainInList(iStringPool.StringF(DOMAINSTRINGS::EDomain6, iStringTable)) != KErrNotFound )
		{
		iEngine->Utils().LogIt(_L("Domain IS found."));
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	}

void CServerListTest::RunTestCase6L()
	{
	// Test Case 6 - Duplicates are moved to last postition
	iEngine->Utils().LogIt(_L("***** Test Case 6 - Duplicates are moved to last postition *****\n"));
	ResetListL();
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain1, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain2, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain3, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain4, iStringTable));
	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain5, iStringTable));
	iEngine->Utils().LogIt(_L("Added 5 unique domains and re-adding 3 again.\n"));

	iDomainList->AddDomainToListL(iStringPool.StringF(DOMAINSTRINGS::EDomain3, iStringTable));
	
	// Test validation
	TInt domainPos = iDomainList->IsDomainInList(iStringPool.StringF(DOMAINSTRINGS::EDomain3, iStringTable));
	if( domainPos == KErrNotFound )
		{
		iEngine->Utils().LogIt(_L("Domain NOT found."));
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	iEngine->Utils().LogIt(_L("Domain position: Expected: 4, Actual: %d."), domainPos);
	if( domainPos != 4 )
		{
		iEngine->Utils().LogIt(_L("Domain in wrong position."));
		iEngine->Utils().LogIt(_L("Test Case Failed."));
		User::Leave(-1);
		}
	}
