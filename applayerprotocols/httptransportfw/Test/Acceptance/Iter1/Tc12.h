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


#ifndef __TC12_H__
#define __TC12_H__

#include "HttpTestCore.h"
#include <http.h>
#include "csrvaddrval.h"


/** 
 * BEHAVIOR TO TEST: The VF to generate an EFailed Event AND to terminate 
 * the transaction with RHTTPTransaction::Cancel().
 * This situation can happen as a result of a badly formulated Request (At 
 * this stage only POST and GET are tested):
 * 1)GET request with body data 
 * 2)POST request without body data
 * 3)POST request wit body data but without the minimum entity header 
 * The DoRunL() function simulate this 3 situation making 3 transaction in 
 * sequence using the same session:
 * 1) GET request with body data
 * The MHFRunL() should log the KErrHttpGetReqWithBody error status message 
 * issued by the VF and the consequent EFailed one .In addition the 
 * Transaction is cancelled as shown in the httptextprt.txt log file.
 * 2) POST request without body datais sent with no body
 * The MHFRunL() should log the KErrHttpPostReqWithoutBody error status 
 * message issued by the VF and the consequent EFailed one. In addition the 
 * Transaction is cancelled as shown in the httptextprt.txt log file.
 * 3) POST request with body data(as it should be)but without an entity header:
 * 'Content-Length:6'.
 * The MHFRunL() should log the KErrHttpReqWithoutEntityHeaders error status 
 * message issued by the VF and the consequent EFailed one. In addition the 
 * Transaction is cancelled as shown in the httptextprt.txt log file.
 * NB: For the purpose of this test case the content of the headers and the 
 * body won't be showed.
 */

class CHttpTestCase12: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase12(CScriptFile* aIniSettingsFile);
	virtual ~CHttpTestCase12();

	virtual void MHFRunL(RHTTPTransaction aTransaction,
								const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction,
								const THTTPEvent& aEvent);
private:	
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	virtual void DoCancel();	
	void OpenTestTransaction1L();
	void CloseTestTransaction1();
	void OpenTestTransaction2L();
	void CloseTestTransaction2();
	void OpenTestTransaction3L();
	void CloseTestTransaction3();
protected:
	void CompleteOwnRequest();
private:
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	RStringF iIpAddr;
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};

#endif
