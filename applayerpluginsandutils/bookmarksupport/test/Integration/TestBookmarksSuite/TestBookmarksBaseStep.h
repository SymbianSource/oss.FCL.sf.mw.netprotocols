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
// Contains declaration of CTestBookmarksBaseStep class
// 
//

#ifndef __TEST_BOOKMARKS_BASE_STEP_H__
#define __TEST_BOOKMARKS_BASE_STEP_H__

// System Include
#include <bookmarkdatabase.h>

// User Include
#include "TestBookmarksServer.h"

/**
The base test step from which all other test steps are derived
@internalTechnology
@test
*/
class CTestBookmarksBaseStep : public CTestStep
	{
public:
	// Construction
	CTestBookmarksBaseStep(CTestBookmarksServer& aTestServer);

	//Destruction
	~CTestBookmarksBaseStep();

	// TEF virtuals
	virtual enum TVerdict 	doTestStepPreambleL();

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

protected:
	// Utility functions used by the test steps
	void 					CommitDb();
	TInt 					GetArrayIndex(const TPtrC& aTitle);
	TInt					GetParentFolder(const TPtrC& aParent, RBkFolder& aParentFolder);
	TInt					GetBkNode(const TPtrC& aTitle, const TPtrC& aTypeOfItem, RBkNode& aBkNode);
	Bookmark::TItemId		GetItemFromTreeL(const TDesC& aName, RBkFolder aFolder);
	TVerdict 				CompareTUints(const TUint32& aTUint32Val1, const TUint32& aTUint32Val2);
	TVerdict				CompareBools(const TBool& aBoolVal1, const TBool& aBoolVal2);
	TVerdict				CompareStrings(const TPtrC& aStrVal1, const TPtrC& aStrVal2);
	TVerdict 				CompareTInts(const TInt& aTIntVal1, const TInt& aTIntVal2);
	TVerdict 				CompareTReals(const TReal& aTRealVal1, const TReal& aTRealVal2);
	TVerdict 				CompareTTimes(const TTime& aTTimeVal1, const TTime& aTTimeVal2);
	void 					CheckErrorAndCommit(const TInt& aError);
	void 					CheckErrorAndVerdict(const TInt& aError, const TVerdict& aVerdict);

private:
	Bookmark::TVisibility 	GetDbOpenMode(const TPtrC& aMode);

protected:

	/** Database handle */
	RBkDatabase 			iBkDb;

	/** Test server reference for sharing data between test steps */
	CTestBookmarksServer& 	iTestServer;
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestBookmarksBaseStep, "TestBookmarksBaseStep");
/*@}*/

#endif		// __TEST_BOOKMARKS_BASE_STEP_H__
