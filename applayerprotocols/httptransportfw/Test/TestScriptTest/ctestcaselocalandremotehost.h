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

#ifndef __CTESTCASELOCALANDREMOTEHOST_H__
#define __CTESTCASELOCALANDREMOTEHOST_H__

#include <e32base.h>
#include <http.h>
#include "CPipeliningTestClient.h"


class CHTTPTestUtils;
class MPipeliningTestObserver;

/*	First send a request to local host
		Once the response is received open a RSocketServ & RConnection and set to the HTTP stack.
		Send a request to a remote host
		once the response is received send another request to local host		
*/
class CTestCaseLocalAndRemoteHost : public CPipeliningTestClient
	{
public:
	static CTestCaseLocalAndRemoteHost* NewL(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);
	~CTestCaseLocalAndRemoteHost ();
	
private:
    void ConstructL();
	CTestCaseLocalAndRemoteHost(CHTTPTestUtils& aTestUtils, MPipeliningTestObserver& aObserver);

    // From MHTTPTransactionCallback
    void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
    RHTTPTransaction GetLocalTransactionL();
    RHTTPTransaction GetRemoteTransactionL();
private:
	TBool                   iSentRemoteHostTrans;
	};

#endif /*__CTESTCASE22_H__*/


