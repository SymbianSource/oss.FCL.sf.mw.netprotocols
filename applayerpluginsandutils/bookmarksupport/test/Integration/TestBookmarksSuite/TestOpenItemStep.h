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
// Contains declaration of CTestOpenItemStep class
// 
//

#ifndef __TEST_OPEN_ITEM_STEP_H__
#define __TEST_OPEN_ITEM_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that opens an item specified in the ini file
@internalTechnology
@test
*/
class CTestOpenItemStep : public CTestBookmarksBaseStep
	{
public:
	CTestOpenItemStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTest(const TPtrC& aTypeOfItem, const TPtrC& aTitle);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestOpenItemStep, "TestOpenItemStep");
/*@}*/

#endif		// __TEST_OPEN_ITEM_STEP_H__
