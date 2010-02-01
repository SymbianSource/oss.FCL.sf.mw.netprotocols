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
// Contains declaration of CTestUpdateVisitedStep class
// 
//

#ifndef __TEST_UPDATE_VISITED_STEP_H__
#define __TEST_UPDATE_VISITED_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that tests UpdateVisted()
@internalTechnology
@test
*/
class CTestUpdateVisitedStep : public CTestBookmarksBaseStep
	{
public:
	CTestUpdateVisitedStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTest(RBkBookmark& aBookmark);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestUpdateVisitedStep, "TestUpdateVisitedStep");
/*@}*/

#endif		// __TEST_UPDATE_VISITED_STEP_H__
