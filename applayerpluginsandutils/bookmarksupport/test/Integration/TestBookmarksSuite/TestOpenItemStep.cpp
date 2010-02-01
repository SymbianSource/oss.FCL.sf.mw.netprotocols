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
// Contains implementation of CTestOpenItemStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Include
#include <bookmarkdatabase.h>

// User Include
#include "TestOpenItemStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestOpenItemStep::CTestOpenItemStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestOpenItemStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestOpenItemStep::doTestStepL()
	{
	TPtrC typeOfItem;
	TPtrC title;
	if(!GetStringFromConfig(ConfigSection(), KIniTypeOfItem, typeOfItem	) ||
	   !GetStringFromConfig(ConfigSection(), KIniTitle,		 title		)
	  )
		{
		ERR_PRINTF3(_L("Problem in reading values from ini.			\
						\nExpected fields are: \n%S\n%S\n"
					  ),&KIniTypeOfItem, &KIniTitle
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		DoTest(typeOfItem, title);
		}
	return TestStepResult();
	}	// doTestStepL

/**
Opens an item with a particular title
@internalTechnology
@test
@param		Type of item.
@param		Title of item to be opened
@return		None
*/
void CTestOpenItemStep::DoTest(const TPtrC& aTypeOfItem, const TPtrC& aTitle)
	{
	TInt error = KErrNone;
	RBkNode bkNode;
	if((error = GetBkNode(aTitle, aTypeOfItem, bkNode)) != KErrNone)
		{
		ERR_PRINTF3(_L("Error occured while opening item %S : %D"), &aTitle, error);
		SetTestStepError(error);
		}
	else
		{
		INFO_PRINTF2(_L("Attempt to open item %S succeeded"), &aTitle);
		}
	bkNode.Close();
	}	// DoTest
