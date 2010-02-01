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
// Contains declaration of CTestHttpServer class
// @internalAll
// 
//

#ifndef __TEST__HTTP_SERVER_H__
#define __TEST__HTTP_SERVER_H__

// System Include
// For accessing TEF classes 
#include <test/testexecuteserverbase.h>

/*@{*/
// Literal constants for INI file field-names
_LIT(KIniSessProxy, 		   "SessProxy");
_LIT(KIniSessProxyUsage, 	   "SessProxyUsage");
_LIT(KIniTranProxy,        "TranProxy");
_LIT(KIniTranProxyUsage,   "TranProxyUsage");
_LIT(KIniUri, 		       "Uri");
_LIT(KIniRespBodyFileName, "RespBodyFileName");
_LIT(KIniFileName,  	   "OutputFileName");
_LIT(KIniSessHttpOptUsage, "SessHttpOptUsage");
_LIT(KIniTranHttpOptUsage, "TranHttpOptUsage");
_LIT(KIniIsEncoded,  "IsEncoded");
_LIT(KIniIsDecoded,  "IsDecoded");
_LIT(KIniFailTransaction,  "FailTransaction");
_LIT(KDisableTransOpt, "DisableTransOpt");
_LIT(KIAP, "IAP");
/*@}*/
_LIT(KTxt, "ERROR");

/**
This is the test server to test the white/black-list uri service API. 
@internalTechnology
*/
class CTestHttpServer : public CTestServer
	{
public:
	// Construction
	static	CTestHttpServer* 	NewL();
	// Base class pure virtual
	virtual CTestStep* 	CreateTestStep(const TDesC& aStepName);
	
protected:
	CTestStep* 	 CreateTestStepL(const TDesC& aStepName);
	const TPtrC  ServerName();
	};

#endif		// __TEST__HTTPEBO_SERVER_H__


