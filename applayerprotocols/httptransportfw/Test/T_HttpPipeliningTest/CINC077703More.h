// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CINC077703.h
// 
//

#ifndef __CINC077703MORE_H__
#define __CINC077703MORE_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

class CINC077703More : public CPipeliningTestCase
{
public:
	static CINC077703More* NewL(CHTTPTestUtils& aTestUtils);
	~CINC077703More();

private:
	CINC077703More(CHTTPTestUtils& aTestUtils);
	

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;
	
private:
	CHTTPTestUtils&			iTestUtils;
};

#endif // __CINC077703MORE_H__
