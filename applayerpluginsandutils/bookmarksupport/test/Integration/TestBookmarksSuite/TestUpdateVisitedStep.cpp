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
// Contains implementation of CTestUpdateVisitedStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Include
#include <bookmarkdatabase.h>

// User Include
#include "TestUpdateVisitedStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestUpdateVisitedStep::CTestUpdateVisitedStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestUpdateVisitedStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestUpdateVisitedStep::doTestStepL()
	{
	TInt error = KErrNone;
	RBkFolder parentFolder;
	if ((error = GetParentFolder(KMainTestFolder(), parentFolder)) != KErrNone)
		{
		ERR_PRINTF3(_L("Error %D occured while opening parent folder %S"), error, &(KMainTestFolder()));
		SetTestStepResult(EFail);
		}
	else
		{
		CleanupClosePushL(parentFolder);
		RBkBookmark bookmark = iBkDb.CreateBookmarkL(&parentFolder);
		CleanupClosePushL(bookmark);
		DoTest(bookmark);
		DoTest(bookmark);
		iBkDb.DeleteItemL(bookmark.Id());
		CleanupStack::PopAndDestroy(2, &parentFolder);
		}
	return TestStepResult();
	}	// doTestStepL

/**
Tests UpdateVisited() API
@internalTechnology
@test
@param		Reference to handle to the bookmark that is under test
@return		None
*/
void CTestUpdateVisitedStep::DoTest(RBkBookmark& aBookmark)
	{
	const TInt KOneSecond = 1000000;
	TTime initialTime;
	// Set the time to universal time
	initialTime.UniversalTime();
	INFO_PRINTF2(_L("Current time before updating = %Ld"), initialTime.Int64());
	INFO_PRINTF1(_L("Updating LastVisited..."));

	// Call updatevisited after one second
	User::After(KOneSecond);
	aBookmark.UpdateVisited();
	TTime lastVisitedTime = aBookmark.LastVisited();
	INFO_PRINTF2(_L("LastVisitedTime after updating = %Ld"), lastVisitedTime.Int64());

	// Check whether last visited is greater than the initial time
	if(lastVisitedTime <= initialTime)
		{
		INFO_PRINTF1(_L("LastVisited was not updated properly"));
		SetTestStepResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("LastVisited was updated properly"));
		}
	}	// DoTest


