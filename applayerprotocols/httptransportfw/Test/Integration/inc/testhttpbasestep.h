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
// Contains declaration of CTestHttpBaseStep class
// @internalAll
// 
//

#ifndef __TEST_HTTP_BASE_STEP_H__
#define __TEST_HTTP_BASE_STEP_H__

// System Include
#include <http.h>

// User Include
#include "testhttpserver.h"

/**
This is the base test step from which all other test steps are derived
to test the EBO support provided by the http client framework.
@internalTechnology
@test
*/
class CTestHttpBaseStep : public CTestStep, public MHTTPTransactionCallback
	{
public:
    static void InitCommsL();
	CTestHttpBaseStep();
	virtual ~CTestHttpBaseStep();
	// TEF virtuals
	virtual TVerdict doTestStepPreambleL();
	// TEF pure virtual
	virtual TVerdict doTestStepL();

protected:
	virtual void StartClientL() = 0;
	virtual void InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod) = 0;

	// methods from MHTTPTransactionCallback
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) = 0;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent) = 0;
	
	virtual void SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue);
	
protected:
    RHTTPSession iSess;
	RHTTPTransaction iTrans;
    RFs iFileServ;
	CActiveScheduler* iSched;
	};

/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestHttpBaseStep, "TestHttpBaseStep");
/*@}*/

#endif		// __TEST_HTTPEBO_BASE_STEP_H__
