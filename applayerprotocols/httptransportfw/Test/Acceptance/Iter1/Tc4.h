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

#ifndef __TC4_H__
#define __TC4_H__

#include "HttpTestCore.h"
#include <http.h>
#include "csrvaddrval.h"

/** 
 * like TC3 but also  the response' body is received and logged
 * a completed transaction is simulated content type :
 * EApplicationXWwwFormUrlEncoded
 */
class CHttpTestCase4: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase4(CScriptFile* aIniSettingsFile);

	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

	virtual ~CHttpTestCase4();
private:	
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	virtual void DoCancel();	
//	virtual const TDesC& TestName();
protected:
	void CompleteOwnRequest();
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};

#endif
