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

#ifndef	__T_COOKIES_H__
#define	__T_COOKIES_H__

#include <http.h>
#include <http/mhttpfiltercreationcallback.h>

#include "HttpTestCore.h"

// User Include
#include "TestScripts.h"

class CExampleCookieManager;

/**
	Derived test case class : test cookies
*/
class CTextModeTestCookies: public CHttpTestTransBase, 
							public MHTTPTransactionCallback, 
							public MHTTPFilterCreationCallback
	{
public:
	static CTextModeTestCookies* NewL(TInt aTestNumber, 
		CScriptFile* aIniSettingsFile);
	virtual ~CTextModeTestCookies();
	// to implement in this class' member function definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CTextModeTestCookies(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void CreateTransArrayL(const TDesC8& aUri);
	void ConstructL();
	void ConfigureSessionFiltersL(TFilterConfigurationIterator* aFilterConfigIter);
	void ValidateBodyL(RHTTPTransaction aTransaction);

	void DoGenericCookieTestL(const TDesC8* aCookie1, const TDesC8* aCookie2, const TDesC8* aCookie3);
	void DoCookieTest1L();
	void DoCookieTest2L();
	void DoCookieTest3L();
	void DoCookieTest4L();
	void DoCookieTest5L();
	void DoCookieTest6L();
	void DoCookieTest7L();
	void DoCookieTest8L();
	void DoCookieTest9L();
	void DoCookieTest10L();
	void DoCookieTest12L();

	void ValidateRequestL();
	void ValidateResponseL();
	void CheckForCookiesL(RHTTPHeaders aHeaders, TInt aTestNumber, TInt aTransNumber);
	void CheckForSetCookiesL(RHTTPHeaders aHeaders, TInt aTestNumber, TInt aTransNumber);
	RStringF StringToMatchL(TInt aTestNumber, TInt aTransNumber, TInt aCookieCount);

private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	
protected:
	RArray<RHTTPTransaction> iTransArray;
	TInt iOpenTransactionCount;
	TInt iFailureError;
	CExampleCookieManager* iCookieManager;
	TInt iTestNumber;
	};

#endif // __T_COOKIES_H__
