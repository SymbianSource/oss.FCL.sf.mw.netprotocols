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

#include "multitrans.h"
#include <http.h>

_LIT(KRunTestItemName, "RunTest");
_LIT(KSectionSingleTransaction, "MULTI_TRANS");


void CHttpTestMultipleTransactions::DoRunL()
	{
	OpenTestSessionL(); // set up session and transaction here

	for (TInt ii=0; ii< iTransactions.Count();++ii)
		iTransactions[ii].SubmitL();
	CActiveScheduler::Start();
	CloseTestSession(); // close any strings, transaction and session
	}




void CHttpTestMultipleTransactions::OpenTestSessionL()
	{
	iSession.OpenL();
	MHTTPAuthenticationCallback::InstallAuthenticationL(iSession);
	iMyStrP = iSession.StringPool();

	TBuf<24> sectionName;
	sectionName.Format(KSectionSingleTransaction);
	if (iIniFile->ItemValue(sectionName, KRunTestItemName, ETrue))
		{
		TInt count =FindEndOfTransaction(0) + 2; // search for first transaction, start from the item after that, skipping
		// the item with the transaction= on it.
		TInt endSection = iIniFile->Section(iSectionNumber).Items().Count();
		while (count < endSection)
			{
			RHTTPTransaction transaction = CreateTransactionL(count);
			User::LeaveIfError(iTransactions.Append(transaction));
			count = FindEndOfTransaction(count);
			}
		}		
	}

void CHttpTestMultipleTransactions::CloseTestSession()
	{
	// Close strings used in this session before closing the session
	//close Transaction and session
	iTransaction.Close();
	iEngine->Utils().LogIt(_L("Transaction terminated\n"));
	iSession.Close();
	iEngine->Utils().LogIt(_L("Session terminated"));
	}

CHttpTestMultipleTransactions* CHttpTestMultipleTransactions::NewL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber)
	{
	CHttpTestMultipleTransactions* self= new(ELeave) CHttpTestMultipleTransactions;
	CleanupStack::PushL(self);
	self->ConstructL(aIniFile, aIniSettingsFile, aSectionNumber);
	CleanupStack::Pop();
	return self;
	}

void CHttpTestMultipleTransactions::ConstructL(CScriptFile& aIniFile, CScriptFile* aIniSettingsFile, const TInt aSectionNumber)
	{
	CHttpTestTransactions::ConstructL(aIniFile, aIniSettingsFile, aSectionNumber);	
	}

