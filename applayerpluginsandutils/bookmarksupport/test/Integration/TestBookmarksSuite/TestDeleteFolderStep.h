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
// Contains declaration of CTestDeleteFolderStep class
// 
//

#ifndef __TEST_DELETE_FOLDER_STEP_H__
#define __TEST_DELETE_FOLDER_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that deletes a folder
@internalTechnology
@test
*/
class CTestDeleteFolderStep : public CTestBookmarksBaseStep
	{
public:
	CTestDeleteFolderStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTest(const TPtrC& aTitle, const TBool& aRecursive);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestDeleteFolderStep, "TestDeleteFolderStep");
/*@}*/

#endif		// __TEST_DELETE_FOLDER_STEP_H__
