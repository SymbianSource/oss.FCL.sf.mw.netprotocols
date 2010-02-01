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

#ifndef __T_INC035621_H__
#define __T_INC035621_H__


#include <e32test.h>
#include <http.h>
#include <http/mhttpauthenticationcallback.h>

#include "HttpTestCore.h"
#include "csrvaddrval.h"

class CTestFixINC035621 : public CHttpTestBase, 
						  public MHTTPAuthenticationCallback,
   					  	  public MHTTPTransactionCallback

	{
public:
	static CTestFixINC035621* NewLC(CScriptFile* aIniSettingsFile);
	~CTestFixINC035621();
	virtual const TDesC& TestName();
	void DoRunL();
	void DoCancel();

	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType,
								RString& aUsername,RString& aPassword);

protected:
	// From MHTTPTransactionCallback
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);

private:
	CTestFixINC035621(CScriptFile* aIniSettingsFile);

private:
	CScriptFile* iIniSettingsFile;
 
protected:
	TInt iCount;
	TInt iError;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;
	TBool complete;
	};

#endif // __T_INC035621_H__
