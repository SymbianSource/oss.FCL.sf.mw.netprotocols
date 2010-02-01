// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TESTEXPLICITSESSIONSTEP_H_
#define TESTEXPLICITSESSIONSTEP_H_

#include <http/rexplicithttpsession.h>
#include "testhttpbasestep.h"

_LIT(KTestExplicitSessionStep, "TestExplicitSessionStep");

class CTestExplicitSessionStep : public CTestHttpBaseStep
	{
public:
	CTestExplicitSessionStep();
	virtual ~CTestExplicitSessionStep();
	
private:
    void StartClientL();
    void InvokeHttpMethodL(const TDesC8& aUri, RStringF aMethod);
	
	void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent) ;
	
public:
	RExplicitHTTPSession	iExplicitSession;
	RHTTPSession			iSession;
	};

#endif /*TESTEXPLICITSESSIONSTEP_H_*/
