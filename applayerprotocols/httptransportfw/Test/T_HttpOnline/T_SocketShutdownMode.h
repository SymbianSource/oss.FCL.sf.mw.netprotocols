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

#ifndef __T_SOCKETSHUTDOWNMODE_H__
#define __T_SOCKETSHUTDOWNMODE_H__

#include <http.h>
#if !defined(__HTTPTESTCORE_H__)
#include "HttpTestCore.h"
#endif

#include "TestScripts.h"

/**
 *  Derived test case class : Socket shut down mode test
 */
class CSocketShutdownModeTest: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
public:
	static CSocketShutdownModeTest* NewL(TBool aImmediateShutdown, TBool aInterrupt, CScriptFile* aIniSettingsFile);
	virtual ~CSocketShutdownModeTest();
	
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;

protected:
	CSocketShutdownModeTest(TBool aImmediateShutdown, TBool aInterrupt, CScriptFile* aIniSettingsFile);
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
	void CreateTransArrayL();
	
private:
	/** This has the instance of the settings.ini file.  This instance is passed
	 to ReplaceHostNameL() function which uses this instance to retrieve the
	 data from settings.ini file.
	 */
	CScriptFile* iIniSettingsFile;
	
protected:
	TBool iImmediateShutdown;
	TBool iInterruptClose;
	TInt iOpenTransactionCount;
	TInt iCompleteTransCount;
	RArray<RHTTPTransaction> iTransArray;
	TInt iFailureError;
	};

#endif // __T_SOCKETSHUTDOWNMODE_H__
