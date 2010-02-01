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


#ifndef __GET21_H__
#define __GET21_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include <es_sock.h>


/* Tests Multiple HTTP connections over different bearers 
 */

class CHttpTestCaseGet21: public CHttpAcceptTestBase
	{
public:
	CHttpTestCaseGet21();
protected:	

	const TDesC& TestName();
	void OpenTestSessionL();
	void CloseTestSession();
	void DoRunL();

	void OpenTestTrans1L();
	void OpenTestTrans2L();
	void OpenTestTrans3L();
	TInt MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/);
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	
private:
	RHTTPTransaction iTransaction1;
	RHTTPTransaction iTransaction2;
	RHTTPTransaction iTransaction3;
	RHTTPRequest iMyRequest1 ;
	RHTTPHeaders iMyHeaders1 ;
	RHTTPRequest iMyRequest2 ;
	RHTTPHeaders iMyHeaders2 ;
	RHTTPRequest iMyRequest3 ;
	RHTTPHeaders iMyHeaders3 ;
	RHTTPSession iSession1;
	RHTTPSession iSession2;
	RSocketServ iSockServ;
	RConnection iConnection1;
	RConnection iConnection2;
private:
	TInt iTransNum;
	TInt iTransSucceeded;
	TInt transCount;
	};


#endif

