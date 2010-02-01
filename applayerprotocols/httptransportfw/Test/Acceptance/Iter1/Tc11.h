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


#ifndef __TC11_H__
#define __TC11_H__

#include "HttpTestCore.h"
#include <http.h>
#include "csrvaddrval.h"

/** 
 * BEHAVIOR TO TEST: The VF to generate an error Event(with negative iStatus).
 * This can happen as a consequence of either 
 * 1)response headers present in the request or
 * 2)entity headers present in the request when no body is present.
 * The DoRunL() function simulates this 2 situation in sequence(each one 
 * opening  and closing a Session).
 * ADDITIONAL BEHAVIOR TO TEST : Fix the problem removing the erroneous 
 * headers and send then a ESuceeded
 * message (the problem has been fixed and the transaction can complete 
 * normally )
 * 1) TRANSACTION: POST with erroneous response header 'Server: 11.111.1.1'.
 * The MHFRunL() should log the KErrHttpWrongRespHeaderInReqServer error 
 * status message issued by the VF
 * just after the submission, and log the fact that the Server header has been
 * removed after
 * the removal of the Server header.Then ESuceeded is logged.
 * 2) TRANSACTION: GET  with no body(as it should be) but with an entity 
 * header:'Content-Length:6'.
 * The MHFRunL() should log the KErrHttpEntityHeaderWithoutBodyContentLength 
 * error status message issued by the VF
 * just after the submission, and log the fact that the Content_Length header 
 * has been removed.
 * Then ESuceeded is logged.
 * NB: For the purpose of this test case the content of the headers and the 
 * body won't be showed.
 *
 * Also, Tests Multiple HTTP connections over default bearer 
 */

class CHttpTestCase11: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase11(CScriptFile* aIniSettingsFile);
	virtual ~CHttpTestCase11();

	virtual void MHFRunL(RHTTPTransaction aTransaction,
									const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction,
									const THTTPEvent& aEvent);
private:	
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	virtual void DoCancel();	
	void OpenTestSession1L();
	void CloseTestSession1();
	void OpenTestSession2L();
	void CloseTestSession2();
//	virtual const TDesC& TestName();
protected:
	void CompleteOwnRequest();
private:
	RHTTPSession iSession1;
	RHTTPSession iSession2;
	RHTTPTransaction iTransaction1;
	RHTTPTransaction iTransaction2;
	RStringF iIpAddr;
	RStringF iServer;
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};


#endif
