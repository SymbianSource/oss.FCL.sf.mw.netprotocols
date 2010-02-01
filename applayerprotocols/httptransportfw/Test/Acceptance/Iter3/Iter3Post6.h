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


#ifndef __ITER3POST6_H__
#define __ITER3POST6_H__

#include "HttpTestCore.h"
#include "CHttpAccepTtestBase.h"
#include "CHttpTestChunkedBase.h"

#include <http.h>

/*	post content which requires authorisation
	and provide valid username/password (scooby/doo) */


class CHttpTestCaseIter3Post6: public CHttpTestChunkedBase 
	{
public:
	CHttpTestCaseIter3Post6(CScriptFile* aIniSettingsFile);
	TInt OverallDataSize();
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername,RString& aPassword);
protected:	
	const TDesC& TestName();
private:
	TInt iDataChunkCount;
	};


#endif
