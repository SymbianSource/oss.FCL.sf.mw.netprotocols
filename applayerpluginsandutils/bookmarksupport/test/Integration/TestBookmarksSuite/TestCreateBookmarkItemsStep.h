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
// Contains declaration of CTestCreateBookmarkItemsStep class
// 
//

#ifndef __TEST_CREATE_BOOKMARK_ITEMS_STEP_H__
#define __TEST_CREATE_BOOKMARK_ITEMS_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that creates a bookmark item as per the configuration
specified in the ini file
@internalTechnology
@test
*/
class CTestCreateBookmarkItemsStep : public CTestBookmarksBaseStep
	{
public:
	CTestCreateBookmarkItemsStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTestL(const TPtrC& aTypeOfItem, const TPtrC& aTitle, const TPtrC& aParent);
	void 					AppendArraysL(const Bookmark::TItemId& aId, const TPtrC& aTitle);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestCreateBookmarkItemsStep, "TestCreateBookmarkItemsStep");
/*@}*/

#endif		// __TEST_CREATE_BOOKMARK_ITEMS_STEP_H__
