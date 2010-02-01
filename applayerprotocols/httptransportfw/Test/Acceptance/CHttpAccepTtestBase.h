// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//


#ifndef __CHTTPACCEPTTESTBASE_H__
#define __CHTTPACCEPTTESTBASE_H__

#include "HttpTestCore.h"
#include <http.h>
#include "testservers.h"
#include "TestScripts.h"

/** CHttpAcceptTestBase includes common code for all acceptance tests which usdes 
	a .ini file to set the test parameters
	Its abstract as TestName, OpenTestSessionL,CloseTestSession must be overloaded in 
	the derived test class
*/
class CHttpAcceptTestBase: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpAcceptTestBase();
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	virtual ~CHttpAcceptTestBase();
	static void Cleanup ( TAny* aPtr );
protected:	
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	virtual void DoCancel();
	virtual const TDesC& TestName() =0;
	virtual void OpenTestSessionL()=0;
	virtual void CloseTestSession() =0;
	void CompleteOwnRequest();
	
private:
	void Validate(RHTTPTransaction aTransaction);

protected:
	TBuf<80> iTestName;
	CScriptFile* iIniFile;
	TInt iSectionNumber;
	TInt iOpenTransactionCount;
	TInt iExpectedError;
	TInt iExpectedNumberRedirects;
	TBool iExpectError;
	};

#endif
 
