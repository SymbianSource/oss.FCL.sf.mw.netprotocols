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


#ifndef __MULTITRANS_H__
#define __MULTITRANS_H__

#include "HttpTestCore.h"
#include "trans.h"
#include "CHttpAccepTtestBase.h"
#include <http.h>
#include "TestScripts.h"

/*TODO */

class CHttpTestMultipleTransactions: public CHttpTestTransactions
	{
public:
	static CHttpTestMultipleTransactions* NewL(CScriptFile& aIniFile, 
		CScriptFile* aIniSettingsFile, const TInt aSectionNumber);
protected:	
	void DoRunL();
	void OpenTestSessionL();
	void CloseTestSession();
	
	void CreateSingleTransactionL();

	
private:
	void ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, 
		const TInt aSectionNumber);

private:
	RArray<RHTTPTransaction> iTransactions;
	};

#endif

