// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Author: Franco.Bellu@symbian.com
// 
//


#include "Tc1.h"
#include <http.h>

CHttpTestCase1::CHttpTestCase1()
	{
	iExpectedStatusCode=0;
	}

void CHttpTestCase1::DoRunL()
	{
	RHTTPSession mySession;
	iEngine->Utils().LogIt(_L("A RHTTPSession object created"));
	
	mySession.OpenL();
	CleanupClosePushL(mySession);
	iEngine->Utils().LogIt(_L("Session Created (TC1)(Default) params:RHTTPProxy aProxy = RHTTPProxy(),MHTTPSessionCallback* aCallback = NULL"));
	
	mySession.Close();
	iEngine->Utils().LogIt(_L("RHTTPSession.Close called"));
	if (iTestFail==1)
		{
		User::Leave(-1);
		}
	CleanupStack::Pop();//mySession
	}

TInt CHttpTestCase1::RunError(TInt aErr)
	{
	iEngine->Utils().LogIt(_L("\nTest failed with error code %d\n"), aErr);
	return KErrNone;
	}

void CHttpTestCase1::DoCancel()
	{
	}
CHttpTestCase1::~CHttpTestCase1()
	{
	}	

/** set this object active */
void CHttpTestCase1::CompleteOwnRequest()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);	
	if (!IsActive())
		SetActive();
	}


/** sets base test name.
	@param void
	@return string - the test name 
*/
const TDesC& CHttpTestCase1::TestName()
	{
	_LIT(KHeaderTestName,"Test Case 1 ");
	return KHeaderTestName;
	}

void CHttpTestCase1::MHFRunL(RHTTPTransaction /*aTransaction*/,
						 const THTTPEvent& /*aEvent*/)
	{
	}

TInt CHttpTestCase1::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/,
						 const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}
