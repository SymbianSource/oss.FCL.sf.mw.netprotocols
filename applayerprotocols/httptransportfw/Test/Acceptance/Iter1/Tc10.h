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


#ifndef __TC10_H__
#define __TC10_H__

#include "HttpTestCore.h"
#include <http.h>
#include "csrvaddrval.h"

/** 
 * BEHAVIOR TO TEST:The VF to ignore a RHTTPEvent coming from the client in 2 
 * possible situation:
 * 1)There is an unknown request header( in this test case 'UnknownHeader: 
 * UnknownHeader:').
 * 2)The Uid of an event sent by the client(RHHTPEvent.iUID) is not KHTTPUid( 
 * in this test case 0x1000A440).
 * The DoRunL() function  simulates these 2  situations in 2 sequenced 
 * transactions:
 * In OpenTestTransaction1() a the unknown header is sent in the request
 * In OpenTestTransaction2()
 * The fact that the VF ignores can be prooved  checking the 
 * HttpValidationFilterLogging.txt log file produced by the VF in the 
 * \c\Logs\Httpth directory. The VF should not log any message sending but 
 * just  messagges received passivelly.
 * The MHFRunL() should not log tany status message from the VF.
 * NB: For the purpose of this test case the content of the headers and the 
 * body won't be showed.
 */

class CHttpTestCase10: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase10(CScriptFile* aIniSettingsFile);
	virtual ~CHttpTestCase10();

	virtual void MHFRunL(RHTTPTransaction aTransaction,
					const THTTPEvent& aEvent);
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
//	virtual const TDesC& TestName();
protected:
	void CompleteOwnRequest();
private:
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	RStringF iIpAddr;
	RStringF iUnknownHeader;
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};

#endif
