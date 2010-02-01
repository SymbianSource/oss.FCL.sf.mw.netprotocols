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

#include "Iter3Post7.h"
#include <http.h>

const TDesC& CHttpTestCaseIter3Post7::TestName()
	{
	_LIT(KHeaderTestName,"Iteration 3 Post 7 ");
	return KHeaderTestName;
	}


TBool CHttpTestCaseIter3Post7::GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword)
	{
	iEngine->Utils().LogIt(_L("Supplying credentials"));
	// Can't log 8 bit data, so dump it.
	iEngine->Utils().DumpData(aURI.UriDes());
	iEngine->Utils().DumpData(aRealm.DesC());	
	iEngine->Utils().DumpData(aAuthenticationType.DesC());	
	_LIT8(KScooby, "scooby");
	_LIT8(KDoo, "doo");
	RStringPool p = aRealm.Pool();
	aUsername = p.OpenStringL(KScooby);
	aPassword = p.OpenStringL(KDoo);
	return ETrue;
	}

CHttpTestCaseIter3Post7::CHttpTestCaseIter3Post7(CScriptFile* aIniSettingsFile)
	{
	iIniSettingsFile = aIniSettingsFile;
	iExpectedStatusCode=200;
	}

TInt CHttpTestCaseIter3Post7::OverallDataSize()
	{
	return 9;
	}
