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
// Contains declaration of CTestHttpBufferSizeStep class
// @internalAll
// 
//

#ifndef __TEST_HTTP_BUFFER_SIZE_STEP_H__
#define __TEST_HTTP_BUFFER_SIZE_STEP_H__

// User Include
#include "testhttpbasestep.h"

// Literal constant defined for identifying step name
_LIT(KTestHttpBufferSizeStep, "TestHttpBufferSizeStep");

/**
This is the base test step from which all other test steps are derived
to test the HTTP Library.
@internalTechnology
@test
*/
class CTestHttpBufferSizeStep : public CTestHttpBaseStep
	{
public:
    CTestHttpBufferSizeStep();
	virtual ~CTestHttpBufferSizeStep();
	
private:
    void StartClientL();
    void InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod);
	void ValidateTest();
	
	// methods from MHTTPTransactionCallback
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	
public:
	MHTTPDataSupplier* iRespBody;
	TInt iChunkSize;
	};

#endif		// __TEST_HTTP_BUFFER_SIZE_STEP_H__
