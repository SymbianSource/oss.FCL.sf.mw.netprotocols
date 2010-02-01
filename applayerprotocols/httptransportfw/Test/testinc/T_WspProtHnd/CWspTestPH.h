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
//

#ifndef	__CWSPTESTPH_H__
#define	__CWSPTESTPH_H__

#include <http/framework/cprotocolhandler.h>

class CWspTestPH : public CProtocolHandler
	{
public:	// Methods

	static CWspTestPH* NewL(RHTTPSession aSession);

	~CWspTestPH();

private:	// Methods inherited from CProtocolHandler

	virtual void CreateCodecL();

	virtual TBool ServiceL(RHTTPTransaction aTrans);

	virtual void CleanupClosedTransaction(RHTTPTransaction aTrans);

	virtual void CancelTransactionL(RHTTPTransaction aTransaction);

	virtual void NotifyNewRequestBodyPart(RHTTPTransaction aTransaction);

private:	// Methods

	CWspTestPH(RHTTPSession aSession);

	void ConstructL();

private:	// Attributes



	};


#endif	// __CWSPTESTPH_H__ 