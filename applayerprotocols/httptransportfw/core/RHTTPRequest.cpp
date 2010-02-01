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
#include <http/rhttprequest.h>// Class signature


// Local includes
#include "CRequest.h"
#include "CTransaction.h"


EXPORT_C RStringF RHTTPRequest::Method() const
	{
	return iImplementation->iTransaction->Session().StringPool().StringF(
		static_cast<CRequest*>(iImplementation)->Method());
	}
EXPORT_C const TUriC8& RHTTPRequest::URI() const
	{
	return (static_cast<CRequest*>(iImplementation)->URI()->Uri());
	}

EXPORT_C void  RHTTPRequest::SetMethod(RStringF aMethod)
	{
	RStringF& method = 
		static_cast<CRequest*>(iImplementation)->Method();
	method.Close();
	method = aMethod;
	method.Copy();
	}

EXPORT_C void RHTTPRequest::SetURIL(const TUriC8& aURI)
	{
	CUri8*& uri = static_cast<CRequest*>(iImplementation)->URI();
	CUri8* oldUri = uri;
	uri = CUri8::NewL(aURI);
	// If the NewL leaves, the delete won't get executed, and so the request
	// will remain unmodified.
	delete oldUri;
	}

