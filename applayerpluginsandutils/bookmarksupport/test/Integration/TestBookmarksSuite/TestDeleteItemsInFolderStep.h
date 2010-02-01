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
// Contains declaration of CTestDeleteItemsInFolderStep class
// 
//

#ifndef __TEST_DELETE_ITEMS_IN_FOLDER_STEP_H__
#define __TEST_DELETE_ITEMS_IN_FOLDER_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that deletes the children of a folder mentioned in
the ini file
@internalTechnology
@test
*/
class CTestDeleteItemsInFolderStep : public CTestBookmarksBaseStep
	{
public:
	CTestDeleteItemsInFolderStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTestL(const TPtrC& aTitle);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestDeleteItemsInFolderStep, "TestDeleteItemsInFolderStep");
/*@}*/

#endif		// __TEST_DELETE_ITEMS_IN_FOLDER_STEP_H__
