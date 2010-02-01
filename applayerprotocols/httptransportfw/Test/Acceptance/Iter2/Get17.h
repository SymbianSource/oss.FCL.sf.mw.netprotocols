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
//


#ifndef __GET17_H__
#define __GET17_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>

/*TODO */

class CHttpTestCaseGet17: public CHttpAcceptTestBase
	{
public:
	CHttpTestCaseGet17(CScriptFile* aIniSettingsFile);
protected:	

	const TDesC& TestName();
	void OpenTestSessionL();
	void CloseTestSession();
	void DoRunL();

	void OpenTestTrans1L();
	void OpenTestTrans2L();
	void OpenTestTrans3L();
	void OpenTestTrans4L();
	void OpenTestTrans5L();
	void OpenTestTrans6L();
	void OpenTestTrans7L();
	void OpenTestTrans8L();
	TInt MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/);
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	
private:
	RHTTPTransaction iTransaction1;
	RHTTPTransaction iTransaction2;
	RHTTPTransaction iTransaction3;
	RHTTPTransaction iTransaction4;
	RHTTPTransaction iTransaction5;
	RHTTPTransaction iTransaction6;
	RHTTPTransaction iTransaction7;
	RHTTPTransaction iTransaction8;
	RHTTPRequest iMyRequest1 ;
	RHTTPRequest iMyRequest2 ;
	RHTTPRequest iMyRequest3 ;
	RHTTPRequest iMyRequest4 ;
	RHTTPRequest iMyRequest5 ;
	RHTTPHeaders iMyRequest6 ;
	RHTTPRequest iMyRequest7 ;
	RHTTPRequest iMyRequest8 ;

	RHTTPHeaders iMyHeaders1 ;
	RHTTPHeaders iMyHeaders2 ;
	RHTTPHeaders iMyHeaders3 ;
	RHTTPHeaders iMyHeaders4 ;
	RHTTPRequest iMyHeaders5 ;
	RHTTPHeaders iMyHeaders6 ;
	RHTTPHeaders iMyHeaders7 ;
	RHTTPHeaders iMyHeaders8 ;

	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;

	TInt transCount;
	TInt testHdrReceived;
	TInt iTransNum;
	TInt iTransSucceeded;
	};


#endif

