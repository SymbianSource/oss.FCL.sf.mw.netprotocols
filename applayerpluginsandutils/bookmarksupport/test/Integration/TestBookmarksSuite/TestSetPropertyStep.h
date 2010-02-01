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
// Contains declaration of CTestSetPropertyStep class
// 
//

#ifndef __TEST_SET_PROPERTY_STEP_H__
#define __TEST_SET_PROPERTY_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that sets the property of an item based on the
value mentioned in the ini file
@internalTechnology
@test
*/
class CTestSetPropertyStep : public CTestBookmarksBaseStep
	{
public:
	CTestSetPropertyStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTestL(const TInt& aProperty, const TPtrC& aValue);
	void					SetItemPropertyValueL(RBkNode& aBkNode, const TInt& aProperty, const TPtrC& aValue);
	void					SetDbPropertyValueL(const TInt& aProperty, const TPtrC& aExpectedValue);
	void 					DoSetAuthenticationL(RBkBookmark& bkBkMrk);
	TInt					SetItemParent(RBkNode& aBkNode, const TPtrC& aValue);
	TInt 					SetHomePageBookmark(const TPtrC& aValue);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestSetPropertyStep, "TestSetPropertyStep");
/*@}*/

#endif		// __TEST_SET_PROPERTY_STEP_H__
