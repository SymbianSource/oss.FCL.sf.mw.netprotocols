// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contains declaration of CTestWListBListUriBaseStep class
// @internalAll
// 
//

#ifndef __TEST_WLISTBLISTURI_BASE_STEP_H__
#define __TEST_WLISTBLISTURI_BASE_STEP_H__
// User Include
#include "testwlistblisturiserver.h"

/**
This is the base test step from which all other test steps are derived.
@internalTechnology
@test
*/
class CTestWListBListUriBaseStep : public CTestStep, public MQueryResultsCallback, public MUriCustomiser
	{
public:
	
	CTestWListBListUriBaseStep();

	~CTestWListBListUriBaseStep();

	// TEF virtuals
	virtual TVerdict doTestStepPreambleL();

	// TEF pure virtual
	virtual TVerdict doTestStepL();
	
	void CreateL(const TDesC& aUri, const TInt aListType, const TInt aServiceType, const TDesC& aFavName);
	void FetchL(const TDesC& aUri, const TInt aServiceType, const TDesC& aFavName);
	
	static CUri8* SchemeOrProNormaliseUriL(const TUriC8& aUri);
	void ClearUriArray();
	TInt Count ();
	
	//from MQueryResultsCallback
	TBool OnQueryResultsL(RInetUri aUri);
	
	//from MUriCustomiser
	CUri8* OnUriCustomisationL ( const TUriC8& aUri );
	
protected:

    /** Database handle */
	RInetUriList     iUriDb;
	
	/* URI properties */
	RInetUri   iUriProps;
	
	/* Active scheduler */
	CActiveScheduler* iSched;
	
	/* Result array */
	RArray < RInetUri > iQueryResults;	
	};

	
/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestWListBListUriBaseStep, "TestWListBListUriBaseStep");
/*@}*/

#endif		// __TEST_WLISTBLISTURI_BASE_STEP_H__

