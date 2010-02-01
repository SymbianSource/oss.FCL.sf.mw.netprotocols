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
// Contains implementation of CTestDeleteItemsInFolderStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Include
#include <bookmarkdatabase.h>

// User Include
#include "TestDeleteItemsInFolderStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestDeleteItemsInFolderStep::CTestDeleteItemsInFolderStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestDeleteItemsInFolderStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestDeleteItemsInFolderStep::doTestStepL()
	{
	TPtrC title;
	if(!GetStringFromConfig(ConfigSection(), KIniTitle, title))
		{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n"
					  ), &KIniTitle
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		TRAPD(error, DoTestL(title));
		if(error != KErrNone)
			{
			ERR_PRINTF2(_L("Error occured in CTestDeleteItemsInFolderStep::DoTestL: %D"), error);
			SetTestStepError(error);
			}
		}
	return TestStepResult();
	}	// doTestStepL

/**
Deletes the children of a folder
@internalTechnology
@test
@param		Title of the folder whose children are to be deleted
@return		None
*/
void CTestDeleteItemsInFolderStep::DoTestL(const TPtrC& aTitle)
	{
	_LIT(KPrintType, "The type of item is %S");
	RBkFolder bkFolder;
	if(aTitle == KRoot)
		{// Root folder
		bkFolder = iBkDb.OpenRootL();
		}
	else
		{
		bkFolder = iBkDb.OpenFolderL(aTitle);
		}
	CleanupClosePushL(bkFolder);
	RBkNode bkItem;
	for(TInt index = 0; index < bkFolder.Count(); ++index)
		{
		// Open the item to get the id
		bkItem = bkFolder.OpenItemL(index);
		CleanupClosePushL(bkItem);
		
		// Print
		INFO_PRINTF4(_L("The title of child %D is %S and Id is %U"), index + 1 , &(bkItem.Title()), bkItem.Id());
		if(bkItem.Type() == Bookmark::ETypeFolder)
			{
			INFO_PRINTF2(KPrintType, &(KFolder()));
			}
		else
			{
			INFO_PRINTF2(KPrintType, &(KBookmark()));
			}

		// Delete
		iBkDb.DeleteItemL(bkItem.Id(), ETrue);
		CleanupStack::PopAndDestroy(&bkItem);
		}
	// Commit
	CommitDb();
	CleanupStack::PopAndDestroy(&bkFolder);
	}	// DoTestL
