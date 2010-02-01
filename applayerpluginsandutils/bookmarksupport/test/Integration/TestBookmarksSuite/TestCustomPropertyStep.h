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
// Contains declaration of CTestCustomPropertyStep class
// 
//

#ifndef __TEST_CUSTOM_PROPERTY_STEP_H__
#define __TEST_CUSTOM_PROPERTY_STEP_H__

// User Includes
#include "TestBookmarksServer.h"
#include "TestBookmarksBaseStep.h"

/**
The test step that sets the custom property of the DB or a Folder or a
Bookmark based on the value mentioned in the ini file
@internalTechnology
@test
*/
class CTestCustomPropertyStep : public CTestBookmarksBaseStep
	{
public:
	CTestCustomPropertyStep(CTestBookmarksServer& aTestServer);

	// TEF pure virtual
	virtual enum TVerdict 	doTestStepL();

	void 					DoTestL(const TInt& aProperty, const TPtrC& aValue);
	void					DoRegDeregCustomProperty(const TPtrC& aOperation, const TPtrC& aCategory, const TPtrC& aUid);
	void					DoSetGetCustomPropertyL(const TPtrC& aOperation, const TPtrC& aCategory, const TPtrC& aUid);
	TInt 					RegisterCustomProperty(const TPtrC& aUid, const TPtrC& aCategory, const TPtrC& aDataType);
	TInt					DeRegisterCustomProperty(const TPtrC& aUid, const TPtrC& aCategory);
	Bookmark::TPropertyType GetPropertyType(const TPtrC& aDataType);
	
	template <class RType>
	void 					DoSetPropertyL(RType &aBkWho, const TPtrC& aDataType, const TPtrC& aUid);

	template <class RType>
	void 					DoGetPropertyL(RType &aBkWho, const TPtrC& aDataType, const TPtrC& aUid);


	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestCustomPropertyStep, "TestCustomPropertyStep");
/*@}*/

#endif		// __TEST_CUSTOM_PROPERTY_STEP_H__
