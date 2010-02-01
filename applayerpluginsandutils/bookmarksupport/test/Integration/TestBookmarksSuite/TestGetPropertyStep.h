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
// Contains declaration of CTestGetPropertyStep class
// 
//

#ifndef __TEST_GET_PROPERTY_STEP_H__
#define __TEST_GET_PROPERTY_STEP_H__

// User Include
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that gets the property of an item and verifies
with the expected value mentioned in the ini file
@internalTechnology
@test
*/
class CTestGetPropertyStep : public CTestBookmarksBaseStep
	{
public:
	CTestGetPropertyStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTestL(const TInt& aProperty, const TPtrC& aExpectedValue);
	void					GetAndCompareItemPropertyValueL(const RBkNode& aBkNode, const TInt& aProperty, const TPtrC& aExpectedValue);
	void					GetAndCompareDbPropertyValueL(const TInt& aProperty, const TPtrC& aExpectedValue);
	TInt 					GetItemParent(const RBkNode& aBkNode, const TPtrC& aExpectedValue, TVerdict& aVerdict);
	TInt 					GetAndCompareHomePageBookmark(const TPtrC& aExpectedValue, TVerdict& aVerdict);
	TVerdict 				DoGetAuthenticationL(const RBkBookmark& bkBkMrk);
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestGetPropertyStep, "TestGetPropertyStep");
/*@}*/

#endif		// __TEST_GET_PROPERTY_STEP_H__
