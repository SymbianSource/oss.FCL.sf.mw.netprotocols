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
// Author: Franco.Bellu@symbian.com
// 
//

#ifndef __TC1_H__
#define __TC1_H__

#include "HttpTestCore.h"
#include <http.h>


/**
 *  Derived test case class : only opens and closes a session
 */
class CHttpTestCase1: public CHttpTestBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	CHttpTestCase1();
	private:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction,
						 const THTTPEvent& aEvent);

	virtual ~CHttpTestCase1();
	private:
	const TDesC& TestName();
	virtual void DoRunL();
	virtual TInt RunError(TInt aErr);	
	virtual void DoCancel();
//	virtual const TDesC& TestName();
protected:
	void CompleteOwnRequest();
	};


#endif
