// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TC3_H__
#define __TC3_H__

#include "HttpTestCore.h"
#include <http.h>

/** 
 * like TC2 but the request is submitted and the response' headers are received
 */
class CHttpTestCase3: public CHttpTestTransBase, public MHTTPTransactionCallback
	{
	//to implement in this class' member functs definitions
public:
	virtual void MHFRunL(RHTTPTransaction aTransaction,
						 const THTTPEvent& aEvent) ;
	virtual TInt MHFRunError(TInt aError,RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual ~CHttpTestCase3();
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