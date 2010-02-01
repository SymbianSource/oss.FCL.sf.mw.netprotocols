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

// System includes
#include <e32std.h>

// Local includes




// Class signature
#include "CRequest.h"


CRequest* CRequest::NewL(RStringF aMethod, const TUriC8& aURI,
						  CTransaction* aTransaction)
	{
	CRequest* req = new (ELeave) CRequest;
	CleanupStack::PushL(req);
	req->ConstructL(aMethod, aURI, aTransaction);
	CleanupStack::Pop(req);
	return req;
	}

CRequest::~CRequest()
	{
	delete iURI;
	iMethod.Close();
	}

void CRequest::ConstructL(RStringF aMethod, const TUriC8& aURI,
						  CTransaction* aTransaction)
	{
	CMessage::ConstructL(aTransaction);
	iURI = CUri8::NewL(aURI);
	iMethod = aMethod;
	}


