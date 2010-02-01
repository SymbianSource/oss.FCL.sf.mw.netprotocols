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
// SingleTrans..h
// 
//


#ifndef __SINGLETRANS__
#define __SINGLETRANS__

#include "HttpTestCore.h"
#include "trans.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include "TestScripts.h"

/*TODO */

class CHttpTestSingleTrans: public CHttpTestTransactions
	{
public:
	static CHttpTestSingleTrans* NewL(CScriptFile& aIniFile, 
		CScriptFile* aIniSettingsFile, const TInt aSectionNumber);
protected:	
	void OpenTestSessionL();
	void CloseTestSession();
	
	void CreateSingleTransactionL();
	void LogCertL();
	void LogCertificate();
	
private:
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	void ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, 
		const TInt aSectionNumber);
	
private:
	TBool iLogCipherSuite;
	};


#endif
