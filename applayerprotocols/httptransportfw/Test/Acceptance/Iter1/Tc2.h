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

#ifndef __TC2_H__
#define __TC2_H__

#include "HttpTestCore.h"
#include <http.h>
#include "csrvaddrval.h"

/**
 * Opens a session, a transaction, creates headers and submit the request 
 * without actually starting the active scheduler
 *
 */
class CHttpTestCase2: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	CHttpTestCase2(CScriptFile* aIniSettingsFile);
	virtual void MHFRunL(RHTTPTransaction aTransaction,
							const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction,
							const THTTPEvent& aEvent);
	virtual ~CHttpTestCase2();

protected:
	void CompleteOwnRequest();

private:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);
	virtual void DoCancel();

	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};


#endif
