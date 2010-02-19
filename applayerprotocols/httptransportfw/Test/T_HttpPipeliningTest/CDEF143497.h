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

#ifndef __CDEF143497_H__
#define __CDEF143497_H__

#include <e32base.h>
#include <http.h>
#include <http/mhttpauthenticationcallback.h>


#include "CPipeliningTestCase.h"

class CHTTPTestUtils;

//Test case for defect fix DEF143497
class CDEF143497  : public CPipeliningTestCase, MHTTPAuthenticationCallback
	{
public:
	static CDEF143497 * NewL(CHTTPTestUtils& aTestUtils);
	~CDEF143497 ();
public :
		virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								 RStringF aAuthenticationType,
								 RString& aUsername, 
								 RString& aPassword);

	
private:
	CDEF143497 (CHTTPTestUtils& aTestUtils);
	void ConstructL();

	// From MPipeliningTestCase
	const TDesC& TestCaseName() const;
	TInt TotalTransactionCount() const;
	RHTTPTransaction GetTransactionL(TInt aIndex, RHTTPSession aSession, MHTTPTransactionCallback& aClient);
	const TDesC8& GetRawRequest(TInt aConnectionIndex, TInt aTransIndex);
	const TDesC8& GetRawResponse(TInt aConnectionIndex, TInt aTransIndex);
	TInt ConnectionCount() const;
	virtual TInt ExpectedError(RHTTPTransaction aTrans);
	
private:
	CHTTPTestUtils&			iTestUtils;
	};
	
#endif // __CDEF143497_H__
