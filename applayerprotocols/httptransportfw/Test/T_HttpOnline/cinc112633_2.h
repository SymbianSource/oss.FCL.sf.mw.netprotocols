// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CINC112633_2_H_
#define CINC112633_2_H_

#include <http.h>
#include <http/mhttpfiltercreationcallback.h>
#include <http/mhttpauthenticationcallback.h>
#include "HttpTestCore.h"

// User Include
#include "TestScripts.h"

/**
	Derived test case class : test cookies
*/
class CINC112633_2: public CHttpTestTransBase, 
							public MHTTPTransactionCallback, 
							public MHTTPAuthenticationCallback
						
	{
public:
	static CINC112633_2* NewL(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	virtual ~CINC112633_2();
	
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								 RStringF aAuthenticationType,
								 RString& aUsername, 
								 RString& aPassword);

protected:
	CINC112633_2(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void ConfigureSessionFiltersL(TFilterConfigurationIterator* aFilterConfigIter);

private:
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iFailureError;
	TInt iTestNumber;
	};
	
#endif /*CINC112633_2_2_H_*/
