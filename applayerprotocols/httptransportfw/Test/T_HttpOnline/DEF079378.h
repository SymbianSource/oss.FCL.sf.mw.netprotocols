// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef	__T_DEF079378_H__
#define	__T_DEF079378_H__

#include <http.h>
#include <http/mhttpfiltercreationcallback.h>
#include <http/mhttpauthenticationcallback.h>
#include "HttpTestCore.h"

// User Include
#include "TestScripts.h"

class CExampleCookieManager;

/**
	Derived test case class : test cookies
*/
class CDEF079378: public CHttpTestTransBase, 
							public MHTTPTransactionCallback, 
							public MHTTPFilterCreationCallback,
							public MHTTPAuthenticationCallback
	{
public:
	static CDEF079378* NewL(TInt aTestNumber, 
		CScriptFile* aIniSettingsFile);
	virtual ~CDEF079378();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;
	// method inherited from MHTTPAuthenticationCallback
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								 RStringF aAuthenticationType,
								 RString& aUsername, 
								 RString& aPassword);

protected:
	CDEF079378(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void ConfigureSessionFiltersL(TFilterConfigurationIterator* aFilterConfigIter);



private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iOpenTransactionCount;
	TInt iFailureError;
	CExampleCookieManager* iCookieManager;
	TInt iTestNumber;
	};

#endif // __T_DEF079378_H__
