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
// Contains implementation of CTestVersionStep class
// 
//

/**
 @file
 @internalTechnology 
*/

// System Include
#include <bookmarkdatabase.h>

// User Include
#include "TestVersionStep.h"

/**
Constructor. Sets the test step name
@internalTechnology
@test
*/
CTestVersionStep::CTestVersionStep(CTestBookmarksServer& aTestServer) : CTestBookmarksBaseStep(aTestServer)
	{
	//Call base class method to set human readable name for test step
	SetTestStepName(KTestVersionStep);
	}


/**
Base class pure virtual.
@internalTechnology
@test
@param		None
@return		EPass or EFail indicating the result of the test step.
*/
TVerdict CTestVersionStep::doTestStepL()
	{
	TInt major;
	TInt minor;
	TInt build;

	if(!GetIntFromConfig(ConfigSection(), KIniMajor, major ) ||
	   !GetIntFromConfig(ConfigSection(), KIniMinor, minor ) ||
	   !GetIntFromConfig(ConfigSection(), KIniBuild, build )
	  )
	  	{
		ERR_PRINTF4(_L("Problem in reading values from ini.			\
						\nExpected fields are : \n%S\n%S\n%S\n"
					  ),&KIniMajor, &KIniMinor, &KIniBuild
				   );
		SetTestStepResult(EFail);
		}
	else
		{
		DoTest(major, minor, build);
		}
	return TestStepResult();
	}	// doTestStepL

/**
Gets the version and compares with values specified in ini
@internalTechnology
@test
@param		Major version
@param		Minor version
@param		Build version
@return		None
*/
void CTestVersionStep::DoTest(const TInt& aMajor, const TInt& aMinor, const TInt& aBuild)
	{
	TVersion version = iBkDb.Version();

	INFO_PRINTF4(_L("Version expected = %D.%D.%D"), aMajor, aMinor, aBuild);
	INFO_PRINTF4(_L("Version returned = %D.%D.%D"), version.iMajor, version.iMinor, version.iBuild);

	// Examine major, minor and build versions
	if(version.iMajor != aMajor)
		{
		ERR_PRINTF1(_L("Major version did not match"));
		SetTestStepResult(EFail);
		}
	if(version.iMinor != aMinor)
		{
		ERR_PRINTF1(_L("Minor version did not match"));
		SetTestStepResult(EFail);
		}
	if(version.iBuild != aBuild)
		{
		ERR_PRINTF1(_L("Build version did not match"));
		SetTestStepResult(EFail);
		}
	}	// DoTest


