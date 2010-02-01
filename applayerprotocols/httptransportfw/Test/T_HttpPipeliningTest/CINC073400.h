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
//

#ifndef __CINC073400_H__
#define __CINC073400_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Pipelining 5 simple request to pipelining test server. The 3rd response has a
	Connection: Close header. This expects the first 3 requests are pipelined and
	the last 2 are not pipelined. 
*/
class CINC073400 : public CPipeliningTestCase
	{
public:
	static CINC073400* NewL(CHTTPTestUtils& aTestUtils);
	~CINC073400();
	
private:
	CINC073400(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TransactionCount(TInt aConnectionIndex) const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;

private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CINC073400_H__
