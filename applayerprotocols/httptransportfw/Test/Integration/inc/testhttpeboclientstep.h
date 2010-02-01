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
// Contains declaration of CHttpEboClientStep class
// @internalAll
// 
//

#ifndef __TEST_HTTP_EBOCLIENT_STEP_H__
#define __TEST_HTTP_EBOCLIENT_STEP_H__

//System Include
#include "testhttpbasestep.h"
#include <http/mhttpdataoptimiser.h>

/**
This is the eboclient to test the ebo functionality.
@internalTechnology
@test
*/

class CHttpEboClientStep : public CTestHttpBaseStep, public MHttpDataOptimiser
	{
public:
	virtual ~CHttpEboClientStep();
	CHttpEboClientStep();

	//These methods from MHttpDataOptimiser
	void EncodeL (const TDesC8& aHttpData, HBufC8*& aEncodedData);
	void DecodeL (const TDesC8& aData, HBufC8*& aHttpData, TBool& aTransFail);

private:
	void StartClientL();
	void InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod);
	
	// methods from MHTTPTransactionCallback
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:
	TBool iSavingResponseBody;
	RFile iRespBodyFile;
	MHTTPDataSupplier* iRespBody;
	TPtrC iOutputFilename;
	};
	
/*@{*/
// Literal constant defined for identifying step name
_LIT(KTestHttpEboClientStep, "TestHttpEboClientStep");
/*@}*/

#endif		// __TEST_HTTP_EBOCLIENT_STEP_H__