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

#ifndef __CHEADERDECODE_H__
#define __CHEADERDECODE_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	
	Test case for the following defects:
	DEF041101 - HTTP Codec cannot handle Server sending back two Content-Length header fields
	INC040893 - Nokia 6600 sends incorrect parameter in Accept-Language HTTP header when Finnish
*/
class CHeaderDecode : public CPipeliningTestCase
	{
public:
	static CHeaderDecode* NewL(CHTTPTestUtils& aTestUtils);
	~CHeaderDecode();
	
private:
	CHeaderDecode(CHTTPTestUtils& aTestUtils);
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
	
#endif // __CHEADERDECODE_H__
