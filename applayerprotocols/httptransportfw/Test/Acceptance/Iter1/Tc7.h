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


#ifndef __TC7_H__
#define __TC7_H__

#include "HttpTestCore.h"
#include <http.h>
#include "csrvaddrval.h"

/** 
 * BEHAVIOR TO TEST:The VF to send  a RHTTPEvent::ESucceded event to the 
 * client.
 * This  happen when the Request is well formulated and the response from the 
 * Origin Server has a 2xx or 3xx status code.
 * The DoRunL() function  simulate such situation: the specified URI  point
 * to a valid resource.The server's response with a 200 ( ok) status message.
 * This will cause the VF to send  an ESucceded status message to the client
 * The MHFRunL() logs the ESucceded status message issued by the Validation 
 * Filter as soon as it gets the server's response. 
 * NB: For the purpose of this test case the content of the headers and the 
 * body won't be showed.
 */
class CHttpTestCase7: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase7(CScriptFile* aIniSettingsFile);
	virtual ~CHttpTestCase7();

	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:	
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	virtual void DoCancel();	
	void OpenTestSessionL();
	void CloseTestSession();
//	virtual const TDesC& TestName();
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
