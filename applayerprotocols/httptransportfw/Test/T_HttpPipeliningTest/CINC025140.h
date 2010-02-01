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

#ifndef __CINC025140_H__
#define __CINC025140_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Test case for defect fix INC025140. Test server returns response with status line without
	status text, ie 'HTTP/1.1 200 ' (Note space after status code). The test case ensures that
	this case is handled correctly.
*/
class CINC025140 : public CPipeliningTestCase
	{
public:
	static CINC025140* NewL(CHTTPTestUtils& aTestUtils);
	~CINC025140();
	
private:
	CINC025140(CHTTPTestUtils& aTestUtils);
	void ConstructL();

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
	
#endif // __CINC025140_H__
