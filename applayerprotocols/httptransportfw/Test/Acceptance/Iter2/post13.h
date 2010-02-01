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


#ifndef __POST13_H__
#define __POST13_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include <http/mhttpauthenticationcallback.h>

/*	post content which requires authorisation
	and provide invalid username/password (invalid/forgot) */

class CHttpTestCasePost13: public CHttpAcceptTestBase, MHTTPAuthenticationCallback
	{
public:
	CHttpTestCasePost13(CScriptFile* aIniSettingsFile);
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword);
	void MHFRunL(RHTTPTransaction aTransaction, THTTPEvent aEvent);
protected:	
	const TDesC& TestName();
	void OpenTestSessionL();
	void CloseTestSession();
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	TInt iAttempt;
	};

#endif
