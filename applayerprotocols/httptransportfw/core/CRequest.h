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

#ifndef __CREQUEST_H__
#define __CREQUEST_H__

#include "CMessage.h"

/** An HTTP Request.  Provides the real implementation of a request,
	as proxied in the public API by RHTTPRequest.
	@see RHTTPRequest
*/
//##ModelId=3C4C186E030D
class CRequest : public CMessage
	{
public:

	/** Constructs a request
	*/
	//##ModelId=3C4C186E034B
	static CRequest* NewL(RStringF aMethod, const TUriC8& aURI, 
						  CTransaction* aTransaction);
	/** Destructor
	*/
	//##ModelId=3C4C186E034A
	~CRequest();

	/** Returns the method
	*/
	//##ModelId=3C4C186E0342
	const RStringF& Method() const;

	/** Returns the method (as a reference so you can set it)
	*/
	//##ModelId=3C4C186E0349
	RStringF& Method();

	/** Returns the URI (as a reference so you can set it)
	*/
	//##ModelId=3C4C186E0341
	CUri8*& URI();

	/** Obtain a handle for this request that can be used by the client.
	*/
	//##ModelId=3C4C186E0340
	inline RHTTPRequest Handle();

private:

	/** Constructor
	*/
	//##ModelId=3C4C186E033F
	CRequest();

	/** Does 2 phase construction
	*/
	//##ModelId=3C4C186E0335
	void ConstructL(RStringF aMethod, const TUriC8& aURI,
					CTransaction* aTransaction);

private:

	/** The URI
	*/
	//##ModelId=3C4C186E032D
	CUri8* iURI;

	/** The method
	*/
	//##ModelId=3C4C186E0323
	RStringF iMethod;
	};

#include "CRequest.inl"

#endif // __CREQUEST_H__
