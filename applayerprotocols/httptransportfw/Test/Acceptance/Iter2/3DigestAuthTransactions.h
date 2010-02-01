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


#ifndef __C3DIGESTAUTHTRANSACTIONS_H__
#define __C3DIGESTAUTHTRANSACTIONS_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include <http/mhttpauthenticationcallback.h>

// Testing fix ..
// INC036806 - Unauthorised requests are not resent when pipelined with digest 
// authentication.

class C3DigestAuthTransactions: public CHttpAcceptTestBase, MHTTPAuthenticationCallback
	{
public:
	C3DigestAuthTransactions(CScriptFile* aIniSettingsFile);
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
	TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword);
	
private:
	RHTTPTransaction iTransaction1;
	RHTTPTransaction iTransaction2;
	RHTTPTransaction iTransaction3;

	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;

	TInt transCount;
	TInt testHdrReceived;
	TInt iTransNum;
	TInt iTransSucceeded;
	TBool iSetCredentials;
	};


#endif // __C3DIGESTAUTHTRANSACTIONS_H__
