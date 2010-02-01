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
// Contains declaration of CTestVersionStep class
// 
//

#ifndef __TEST_VERSION_STEP_H__
#define __TEST_VERSION_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that tests the DB Version
@internalTechnology
@test
*/
class CTestVersionStep : public CTestBookmarksBaseStep
	{
public:
	CTestVersionStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTest(const TInt& aMajor, const TInt& aMinor, const TInt& aBuild);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestVersionStep, "TestVersionStep");
/*@}*/

#endif		// __TEST_VERSION_STEP_H__
