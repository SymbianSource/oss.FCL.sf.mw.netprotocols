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
// Contains implementation of CTestCreateBookmarkItemsStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Include
#include <bookmarkdatabase.h>

// User Include
#include "TestCreateBookmarkItemsStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestCreateBookmarkItemsStep::CTestCreateBookmarkItemsStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestCreateBookmarkItemsStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestCreateBookmarkItemsStep::doTestStepL()
	{
	TPtrC typeOfItem;
	TPtrC title;
	TPtrC parent;
	if(!GetStringFromConfig(ConfigSection(), KIniTypeOfItem, typeOfItem	) ||
	   !GetStringFromConfig(ConfigSection(), KIniTitle,		 title		) ||
	   !GetStringFromConfig(ConfigSection(), KIniParent,	 parent		)
	  )
		{
		ERR_PRINTF4(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n%S\n"
					  ),&KIniTypeOfItem, &KIniTitle, &KIniParent
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		TRAPD(error, DoTestL(typeOfItem, title, parent));
		if(error != KErrNone)
			{
			ERR_PRINTF2(_L("Error occured in CTestCreateBookmarkItemsStep::DoTestL"), error);
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();
	}	// doTestStepL

/**
Performs the actual test. Creates the bookmark item.
@internalTechnology
@test
@param		Descriptor indicating type of item
@param		Descriptor indicating title of item to be created
@param		Title of parent folder under which current item is to be created
@return		None
*/
void CTestCreateBookmarkItemsStep::DoTestL(const TPtrC& aTypeOfItem, const TPtrC& aTitle, const TPtrC& aParent)
	{
	TInt err = KErrNone;
	Bookmark::TItemId id;
	RBkFolder parentFolder;
	if ((err = GetParentFolder(aParent, parentFolder)) != KErrNone)
		{
		ERR_PRINTF3(_L("Error %D occured while opening parent folder %S"), err, &aParent);
		SetTestStepResult(EFail);
		return;
		}
	CleanupClosePushL(parentFolder);
	RBkNode bkNode;

	if(aTypeOfItem == KFolder)
		{// Folder
		TRAP(err, bkNode = iBkDb.CreateFolderL(aTitle, &parentFolder));
		}
	else
		{// Bookmark
		TRAP(err, bkNode = iBkDb.CreateBookmarkL(&parentFolder));
		}
	if(err == KErrNone)
		{// Creation successful, commit.
		INFO_PRINTF2(_L("Creating item : %U"), bkNode.Id());
		CleanupClosePushL(bkNode);
		id = bkNode.Id();
		INFO_PRINTF2(_L("Id of item created = %U"), id);
		AppendArraysL(bkNode.Id(), aTitle);
		CommitDb();
		CleanupStack::PopAndDestroy(&bkNode);
		}
	else
		{// Error occured
		if(err == Bookmark::KErrTitleAlreadyUsed && aTitle == KMainTestFolder)
			{
			INFO_PRINTF1(_L("Main folder already exists"));
			}
		else
			{
			// Creation failed
			ERR_PRINTF2(_L("Error occured while creating item : %D"), err);
			SetTestStepError(err);
			}
		}
	CleanupStack::PopAndDestroy(&parentFolder);
	}	// DoTestL

/**
Update the arrays with the id and title of item created
@internalTechnology
@test
@param		Id of the item to be appended in id array
@param		Title of the item to be appended in titles array
@return		None
*/
void CTestCreateBookmarkItemsStep::AppendArraysL(const Bookmark::TItemId& aId, const TPtrC& aTitle)
	{
	// Append id
	iTestServer.iIds.AppendL(aId);

	// Append title after allocating a new replica on the heap
	// as the pointer returned by GetStingConfig() is destroyed when this
	// test step perishes
	HBufC* hBufC = aTitle.AllocLC();
	iTestServer.iTitles.AppendL(hBufC);
	CleanupStack::Pop(hBufC);
	}	// AppendArraysL

