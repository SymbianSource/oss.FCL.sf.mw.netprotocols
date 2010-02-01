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

#ifndef __WSPTESTTRANSACTIONS__
#define __WSPTESTTRANSACTIONS__


// User includes
#include "trans.h"
#include <http/mhttpsessioneventcallback.h>


class CWspTestTransactions : public CHttpTestTransactions, public MHTTPSessionEventCallback
	{
public: //methods
	static CWspTestTransactions* NewL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber);
	~CWspTestTransactions();

protected: // methods
	virtual void DoRunL();
	void ConnectToGatewayL();
	void DisconnectFromGatewayL();
	virtual void OpenTestSessionL();
	virtual void CloseTestSession();
	void DoCloseTestSession();
	void CreateSingleTransactionL();
	void SetWspSessionPropertiesL();
	void SetWspProxyPropertiesL(TInt aStartItem);
	void SetWspCapabilityPropertiesL(TInt aStartItem);
	void SetWspSecurityPolicyPropertiesL(TInt aStartItem);
	void LogCertificate();
	void LogCertL();

private: // methods
	CWspTestTransactions(CScriptFile* aIniSettingsFile);
	void ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber);
	virtual void MHFSessionRunL(const THTTPSessionEvent& aEvent);
	virtual TInt MHFSessionRunError(TInt aError, const THTTPSessionEvent& aEvent);

private: // attributes
	THTTPSessionEvent iSessionEvent;
	TBool iAuthenticationFailure;
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	};

#endif // __WSPTESTTRANSACTIONS__
