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
// 3BasicAuthTransactions.h
// 
//


#ifndef __CEOFTRANSACTIONRESUBMISSION_H__
#define __CEOFTRANSACTIONRESUBMISSION_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include <http/mhttpauthenticationcallback.h>

// Testing fix ..
// INC036950 - HTTP testing with emulator(100-4659 Some HTTP transactions are
// not fetched)

class CEofTransactionResubmission: public CHttpAcceptTestBase
	{
public:
	CEofTransactionResubmission(CScriptFile* aIniSettingsFile);
protected:	

	const TDesC& TestName();
	void OpenTestSessionL();
	void CloseTestSession();
	void DoRunL();

	void OpenTestTrans1L();
	void OpenTestTrans2L();

	TInt MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/);
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	void DisablePipeliningL(RHTTPTransaction trans);
	
private:
	RHTTPTransaction iTransaction1;
	RHTTPTransaction iTransaction2;

	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;

	TInt transCount;
	TInt testHdrReceived;
	TInt iTransNum;
	TInt iTransSucceeded;
	TBool iSetCredentials;
	TBool i2ndTrans;
	};


#endif // __CEOFTRANSACTIONRESUBMISSION_H__

