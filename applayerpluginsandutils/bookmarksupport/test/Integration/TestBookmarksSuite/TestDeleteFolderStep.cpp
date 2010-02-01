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
// Contains implementation of CTestDeleteFolderStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Include
#include <bookmarkdatabase.h>

// User Include
#include "TestDeleteFolderStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestDeleteFolderStep::CTestDeleteFolderStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestDeleteFolderStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestDeleteFolderStep::doTestStepL()
	{
	TPtrC title;
	TBool recursive;
	if(!GetStringFromConfig(ConfigSection(), KIniTitle, title))
		{
		ERR_PRINTF2(_L("Problem in reading values from ini.			\
						\nExpected field is : \n%S\n"
					  ),&KIniTitle
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		if(!GetBoolFromConfig(ConfigSection(), KIniRecursive, recursive))
			{
			recursive = ETrue;
			INFO_PRINTF1(_L("recursive field not found in ini, defaulting to ETrue"));
			}
		DoTest(title, recursive);
		}
	return TestStepResult();
	}	// doTestStepL

/**
Deletes the folder with a particular title.
@internalTechnology
@test
@param		Title of the folder to be deleted
@param		TBool indicating recursive delete or not
@return		None
*/
void CTestDeleteFolderStep::DoTest(const TPtrC& aTitle, const TBool& aRecursive)
	{
	TInt error = KErrNone;
	RBkFolder rFolder;

	FOREVER
		{// Just in case DB is corrupt and multiple folders with same name exist
		TRAP(error, rFolder = iBkDb.OpenFolderL(aTitle));
		if(error == KErrNotFound)
			{
			// Folder not found, but still result is kept as pass as this
			// test step is used for cleanup even before starting the tests
			INFO_PRINTF2(_L("Folder %S not found"), &aTitle);
			break;
			}
		else if(error == KErrNone)
			{
			Bookmark::TItemId id = rFolder.Id();
			rFolder.Close();
			TRAP(error, iBkDb.DeleteItemL(id, aRecursive));
			}

		if(error == KErrNone)
			{
			// No error, commit
			INFO_PRINTF2(_L("Folder %S deleted"), &aTitle);
			CommitDb();
			}
		else
			{
			ERR_PRINTF2(_L("Error occured while deleting folder : %D"), error);
			SetTestStepError(error);
			break;
			}
		}
	}	// DoTest


