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
// CINC110203_H_
// 
//


#ifndef CINC110203_H_
#define CINC110203_H_
#include <http.h>
#include <http/mhttpfiltercreationcallback.h>
#include <http/mhttpauthenticationcallback.h>
#include "HttpTestCore.h"

// User Include
#include "TestScripts.h"

/**
	Derived test case class : test cookies
*/
class CINC110203: public CHttpTestTransBase, 
							public MHTTPTransactionCallback
						
	{
public:
	static CINC110203* NewL(TInt aTestNumber, 
	CScriptFile* aIniSettingsFile);
	virtual ~CINC110203();
	
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
							 const THTTPEvent& aEvent) ;

protected:
	CINC110203(TInt aTestNumber, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue);

private:
	CScriptFile* iIniSettingsFile;
	
protected:
	TInt iOpenTransactionCount;
	TInt iFailureError;
	TInt iTestNumber;
	};
#endif /*CINC110203_H_*/
