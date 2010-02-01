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

#ifndef __CINC052596_H__
#define __CINC052596_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Test case for defect fix CINC052596. 
	Not really a pipelining test case.
	This test checks that if connecting to a http/1.0 server we do not switch to non-persistent
	connection (and thereby cloe it) IF the server response contains the folllowing header :
	"connection: keep-alive"
*/
class CINC052596 : public CPipeliningTestCase
	{
public:
	static CINC052596* NewL(CHTTPTestUtils& aTestUtils);
	~CINC052596();
	
private:
	CINC052596(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;
	void ProcessHeadersL(RHTTPTransaction aTrans);
	
private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CINC052596_H__
