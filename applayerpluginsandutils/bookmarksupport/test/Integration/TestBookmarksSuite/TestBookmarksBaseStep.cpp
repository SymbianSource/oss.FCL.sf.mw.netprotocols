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
// Contains implementation of CTestBookmarksBaseStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// User Include
#include "TestBookmarksBaseStep.h"

/**
Constructor. Sets the test step name and initialises the TestServer
reference.
@internalTechnology
@test
*/
CTestBookmarksBaseStep::CTestBookmarksBaseStep(CTestBookmarksServer& aTestServer) : iTestServer(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestBookmarksBaseStep);
	}

/**
Destructor. Closes the database handle
@internalTechnology
@test
*/
CTestBookmarksBaseStep::~CTestBookmarksBaseStep()
	{
	iBkDb.Close();
	}

/**
Base class virtual. Opens connection with the bookmark database.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of opening the DB.
*/
TVerdict CTestBookmarksBaseStep::doTestStepPreambleL()
	{
	TSecurityInfo info;
	info.Set(RProcess());
	INFO_PRINTF2(_L("\n\nSecure id = %U\n\n"), info.iSecureId);

	TInt error = KErrNone;
	TPtrC dbMode;
	// Get DB mode from ini
	if(!GetStringFromConfig(ConfigSection(), KIniDbMode, dbMode))
		{
		TRAP(error, iBkDb.OpenL());
		}
	else
		{
		INFO_PRINTF2(_L("Opening DB in %S mode"), &dbMode);
		TRAP(error, iBkDb.OpenL(GetDbOpenMode(dbMode)));
		}
	if(error != KErrNone)
		{
		ERR_PRINTF2(_L("Error occured while opening DB : %D"), error);
		SetTestStepError(error);
		}
	else
		{
		INFO_PRINTF1(_L("DB opened successfully"));
		}
	return TestStepResult();
	}	// doTestPreambleL

/**
Base class pure virtual. Just returns the test step result.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestBookmarksBaseStep::doTestStepL()
	{
	return TestStepResult();
	}	// doTestStepL

/**
Commits the Bookmarks Database
@internalTechnology
@test
@param		None
@return		None
*/
void CTestBookmarksBaseStep::CommitDb()
	{
	TRAPD(err, iBkDb.CommitL());
	if(err == KErrNone)
		{
		INFO_PRINTF1(_L("Commit succeeded"));
		}
	else
		{
		ERR_PRINTF2(_L("Error occured during commit: %D"), err);
		SetTestStepError(err);
		}
	}

/**
Gets the array index of the id for the item with the particular title
@internalTechnology
@test
@param		Title of the Bookmark item
@return		The index of the item in the array of ids
*/
TInt CTestBookmarksBaseStep::GetArrayIndex(const TPtrC& aTitle)
	{
	for(TInt index = 0; index < iTestServer.iTitles.Count(); ++index)
		{
		if (iTestServer.iTitles[index]->Des().Compare(aTitle) == KErrNone)
			{
			return index;
			}
		}
	return KErrGeneral;
	}

/**
Opens a folder or root based on title passed
@internalTechnology
@test
@param		Title of the parent folder
@param		Reference to a non-open folder handle.
@return		Error code that indicates the result of the operation.
*/
TInt CTestBookmarksBaseStep::GetParentFolder(const TPtrC& aParent, RBkFolder& aParentFolder)
	{
	TInt error = KErrNone;
	if(aParent == KRoot)
		{
		TRAP(error, aParentFolder = iBkDb.OpenRootL());
		}
	else
		{
		TRAP(error, aParentFolder = iBkDb.OpenFolderL(aParent));
		}
	return error;
	}

/**
Opens the node which has a particular title
@internalTechnology
@test
@param		Title of the item to be opened
@param		Type of item : bookmark/folder
@param		Type of item
@param		Reference to a non-open node handle.
@return		Error code that indicates the result of the operation.
*/
TInt CTestBookmarksBaseStep::GetBkNode(const TPtrC& aTitle, \
									const TPtrC& aTypeOfItem,\
								    RBkNode& aBkNode)
	{
	TInt error = KErrNone;
	if(aTitle == KRoot)
		{
		TRAP(error, aBkNode = iBkDb.OpenRootL());
		}
	else if(aTypeOfItem == KFolder)
		{
		TRAP(error, aBkNode = iBkDb.OpenFolderL(aTitle));
		}
	else if(aTypeOfItem == KBookmark)
		{
		// If type is bookmark, try to find it in the array
		TInt index = GetArrayIndex(aTitle);
		if(index < KErrNone)
			{
			// Not found in array, do a search in the tree
			INFO_PRINTF1(_L("Item not found in Titles array"));
			Bookmark::TItemId id = Bookmark::KNullItemID;
			RBkFolder root;
			TRAP(error, root = iBkDb.OpenRootL());
			if(error != KErrNone)
				{
				ERR_PRINTF2(_L("Error occured while opening root: %D"), error);
				}
			else
				{
				TRAP(error, id = GetItemFromTreeL(aTitle, root));
				root.Close();
				if(error != KErrNone)
					{
					ERR_PRINTF2(_L("Error occured in IsItemInFolderL: %D"), error);
					}
				else if(id == Bookmark::KNullItemID)
					{
					INFO_PRINTF1(_L("Item not found in folder tree"));
					error = KErrNotFound;
					}
				else
					{
					INFO_PRINTF2(_L("Trying to open item with id %U"), id);
					TRAP(error, aBkNode = iBkDb.OpenBookmarkL(id));
					}
				}
			}
		else
			{
			TRAP(error, aBkNode = iBkDb.OpenBookmarkL(iTestServer.iIds[index]));
			}
		}
	if(error != KErrNone)
		{
		ERR_PRINTF2(_L("Error occured in GetBkNode : %D"), error);
		}
	return error;
	}

/**
Resolves DB open mode from the string passed
@internalTechnology
@test
@param		Descriptor indicating mode to open the DB
@return		Bookmark::TVisibility enumeration based on the open-mode
*/
Bookmark::TVisibility CTestBookmarksBaseStep::GetDbOpenMode(const TPtrC& aMode)
	{
	if(aMode.Compare(KDbModePublic) == KErrNone)
		{
		return Bookmark::EVisibilityPublic;
		}
	else if(aMode.Compare(KDbModePrivate) == KErrNone)
		{
		return Bookmark::EVisibilityPrivate;
		}
	else if(aMode.Compare(KDbModeAll) == KErrNone)
		{
		return Bookmark::EVisibilityAll;
		}
	else if(aMode.Compare(KDbModeManager) == KErrNone)
		{
		return Bookmark::EVisibilityManager;
		}
	else
		{
		return Bookmark::EVisibilityDefault;
		}
	}

/**
A recursive routine to find an item with a particular title from the tree
@internalTechnology
@test
@param		Title of the item to be found
@param		Handle to the main folder from which search is to begin.
@return		Id of the bookmark item if found. Null Id if not.
*/
Bookmark::TItemId CTestBookmarksBaseStep::GetItemFromTreeL(const TDesC& aName, RBkFolder aFolder)
	{
	RBkNode item;
	Bookmark::TItemId id = Bookmark::KNullItemID;
	// Loop through all children
	TInt index = aFolder.Count() - 1;
	for (;index >= 0; --index)
		{
		item = aFolder.OpenItemL(index);
		CleanupClosePushL(item);
		const TDesC& title = item.Title();

		INFO_PRINTF3(_L("Parent-title = %S, id = %U"), &(aFolder.Title()), aFolder.Id());
		INFO_PRINTF4(_L("index = %D Title = %S, id = %U"), index,  &title, item.Id());

		if (title.Compare(aName) == 0)
			{// Title matched
			id = item.Id();
			}
		else if (item.Type() == Bookmark::ETypeFolder)
			{// Title did not match, but item is a folder, so recurse into it
			RBkFolder folder = item.OpenFolderL();
			CleanupClosePushL(folder);
			// recurse
			id = GetItemFromTreeL(aName, folder);
			CleanupStack::PopAndDestroy(&folder);
			}
		CleanupStack::PopAndDestroy(&item);
		if (id != Bookmark::KNullItemID)
			{// If valid id, then we are done
			break;
			}
		}
	return id;
	}

/**
Compare two TUints and return verdict
@internalTechnology
@test
@param		TUint32 Value 1 to be compared
@param		TUint32 Value 2 to be compared
@return		EPass or EFail based on comparison
*/
TVerdict CTestBookmarksBaseStep::CompareTUints(const TUint32& aTUint32Val1, const TUint32& aTUint32Val2)
	{
	INFO_PRINTF2(_L("Value 1 = %U"), aTUint32Val1);
	INFO_PRINTF2(_L("Value 2 = %U"), aTUint32Val2);
	return (aTUint32Val1 == aTUint32Val2) ? EPass : EFail;
	}

/**
Compare two TBools and return verdict
@internalTechnology
@test
@param		TBool Value 1 to be compared
@param		TBool Value 2 to be compared
@return		EPass or EFail based on comparison
*/
TVerdict CTestBookmarksBaseStep::CompareBools(const TBool& aBoolVal1, const TBool& aBoolVal2)
	{
	INFO_PRINTF2(_L("Value 1 = %S"), &(aBoolVal1 ? KTrue() : KFalse()));
	INFO_PRINTF2(_L("Value 2 = %S"), &(aBoolVal2 ? KTrue() : KFalse()));
	return (!aBoolVal1 == !aBoolVal2) ? EPass : EFail;
	}

/**
Compare two strings and return verdict
@internalTechnology
@test
@param		Descriptor Value 1 to be compared
@param		Descriptor Value 2 to be compared
@return		EPass or EFail based on comparison
*/
TVerdict CTestBookmarksBaseStep::CompareStrings(const TPtrC& aStrVal1, const TPtrC& aStrVal2)
	{
	INFO_PRINTF2(_L("Value 1 = %S"), &aStrVal1);
	INFO_PRINTF2(_L("Value 2 = %S"), &aStrVal2);
	return (aStrVal1.Compare(aStrVal2) == KErrNone) ? EPass : EFail;
	}

/**
Compare two TInts and return verdict
@internalTechnology
@test
@param		TInt Value 1 to be compared
@param		TInt Value 2 to be compared
@return		EPass or EFail based on comparison
*/
TVerdict CTestBookmarksBaseStep::CompareTInts(const TInt& aTIntVal1, const TInt& aTIntVal2)
	{
	INFO_PRINTF2(_L("Value 1 = %D"), aTIntVal1);
	INFO_PRINTF2(_L("Value 2 = %D"), aTIntVal2);
	return (aTIntVal1 == aTIntVal2) ? EPass : EFail;
	}

/**
Compare two TReals and return verdict
@internalTechnology
@test
@param		TReal Value 1 to be compared
@param		TReal Value 2 to be compared
@return		EPass or EFail based on comparison
*/
TVerdict CTestBookmarksBaseStep::CompareTReals(const TReal& aTRealVal1, const TReal& aTRealVal2)
	{
	INFO_PRINTF2(_L("Value 1 = %g"), aTRealVal1);
	INFO_PRINTF2(_L("Value 2 = %g"), aTRealVal2);
	return (aTRealVal1 == aTRealVal2) ? EPass : EFail;
	}

/**
Compare two TTimes and return verdict
@internalTechnology
@test
@param		TTime Value 1 to be compared
@param		TTime Value 2 to be compared
@return		EPass or EFail based on comparison
*/
TVerdict CTestBookmarksBaseStep::CompareTTimes(const TTime& aTTimeVal1, const TTime& aTTimeVal2)
	{
	INFO_PRINTF2(_L("Value 1 = %g"), aTTimeVal1.Int64());
	INFO_PRINTF2(_L("Value 2 = %g"), aTTimeVal2.Int64());
	return (aTTimeVal1 == aTTimeVal2) ? EPass : EFail;
	}

/**
Checks error and commits the DB if error is KErrNone
@internalTechnology
@test
@param		Error value to be checked
@return		None
*/
void CTestBookmarksBaseStep::CheckErrorAndCommit(const TInt& aError)
	{
	// Examine the result
	if(TestStepResult() == EPass)
		{
		if(aError != KErrNone)
			{
			ERR_PRINTF2(_L("The operation failed with error: %D"), aError);
			SetTestStepError(aError);
			}
		else
			{
			INFO_PRINTF1(_L("The operation was successful."));
			CommitDb();
			}
		}
	}

/**
Checks error and if error is KErrNone, checks and sets the verdict
@internalTechnology
@test
@param		Error value to be checked
@param		Verdict as a result of some operation
@return		None
*/
void CTestBookmarksBaseStep::CheckErrorAndVerdict(const TInt& aError, const TVerdict& aVerdict)
	{
	if(TestStepResult() == EPass)
		{
		if(aError != KErrNone)
			{
			ERR_PRINTF2(_L("The operation failed with error: %D"), aError);
			SetTestStepError(aError);
			}
		else
			{
			if(aVerdict == EPass)
				{
				INFO_PRINTF1(_L("The returned and the expected values matched."));
				}
			else
				{
				INFO_PRINTF1(_L("The returned and the expected values did not match."));
				SetTestStepResult(aVerdict);
				}
			}
		}
	}


