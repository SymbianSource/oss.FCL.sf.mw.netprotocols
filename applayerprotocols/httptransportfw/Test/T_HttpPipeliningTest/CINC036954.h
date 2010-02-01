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

#ifndef __CINC036954_H__
#define __CINC036954_H__

#include <e32base.h>
#include <http.h>

#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

/*	Test case for defect fix INC025140. 
	HTTP stack reports OK when transaction has been disconnected because of long GPR 
*/
class CINC036954 : public CPipeliningTestCase
	{
public:
	static CINC036954* NewL(CHTTPTestUtils& aTestUtils);
	~CINC036954();
	
private:
	CINC036954(CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;
	TInt ExpectedError(RHTTPTransaction aTrans);
	
private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CINC036954_H__
