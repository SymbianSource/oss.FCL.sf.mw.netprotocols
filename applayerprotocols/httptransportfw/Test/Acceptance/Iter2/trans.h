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


#ifndef __TRANS_H__
#define __TRANS_H__

#include <http/mhttpauthenticationcallback.h>
#include <ssl.h>	//For TCertInfo

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include "TestScripts.h"
#include "csrvaddrval.h"
enum TConfigureProxyAction
	{
	ECreate,
	EDelete	
	};
	

class CX509Certificate;


class CHttpTestTransactions: public CHttpAcceptTestBase, public MHTTPAuthenticationCallback
	{
public:
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword);
protected:	
	TInt FindEndOfTransaction(TInt aStartItem);
	HBufC8* FindItemLC(const TDesC& aItem, TInt aStart, TInt aEnd, const TDesC8 aDefault);
	RHTTPTransaction CreateTransactionL(TInt aStartItem);
	~CHttpTestTransactions();

	const TDesC& TestName();
	TBool GetNextDataPart(TPtrC8& aDataChunk);
	void ReleaseData(); 
	TInt OverallDataSize();
	TPtr8 WideToNarrowLC(TPtrC aPtr16);
	void ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber);
	void CreateAcceptStringL(RHTTPHeaders aRequestHeaders);
	void CreateAcceptCharsetStringL(RHTTPHeaders aRequestHeaders);
	void CreateAcceptEncodingStringL(RHTTPHeaders aRequestHeaders);
	void DoHeaderL(RHTTPHeaders aRequestHeaders, const TDesC8& aHeader);
	void CloseTestSession();
	TReal StrToReal(TPtr8);
	void SetProxyL(TInt aStartItem);
	void SetMaxRedirectsL(TInt aStartItem);
	void LogCertificateL(TCertInfo aCertInfo);
	void LogCertificateL(const CX509Certificate* aCertInfo);
	void LoadBodyFromFileL(const TDesC& aFilePath);
	void ConfigureDefaultProxyL(TConfigureProxyAction aAction);

protected:
	HBufC8* iBody;
	TBool iLogCert;

private:
	TBuf<80> iTestName;
	RString iUsername;
	RString iPassword;
	TBool iUsernameUsed;
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};

#endif
