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


#ifndef __GET14_H__
#define __GET14_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include <http/mhttpauthenticationcallback.h>

class CHttpTestCaseGet14: public CHttpAcceptTestBase, MHTTPAuthenticationCallback
	{
public:
	CHttpTestCaseGet14(CScriptFile* aIniSettingsFile);
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword);
protected:	
	const TDesC& TestName();
	void OpenTestSessionL();
	void CloseTestSession();
	void DoRunL();
	void OpenTestTrans1L();
	void OpenTestTrans2L();
private:
	RHTTPTransaction iTransaction1;
	RHTTPTransaction iTransaction2;
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	TInt iCallsNum;
	};
	

#endif
 
