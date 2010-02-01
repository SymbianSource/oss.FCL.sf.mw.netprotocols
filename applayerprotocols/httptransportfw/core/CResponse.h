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

#ifndef __CRESPONSE_H__
#define __CRESPONSE_H__

#include "CMessage.h"

/** An HTTP Response.  Provides the real implementation of a response,
	as proxied in the public API by RHTTPResponse.
	@see RHTTPResponse
*/
//##ModelId=3C4C187F0177
class CResponse : public CMessage
	{
 public:
	/** Constructs a response
	*/
	//##ModelId=3C4C187F01C8
	inline static CResponse* NewL(CTransaction* aTransaction);

	/** Destructor
	*/
	//##ModelId=3C4C187F01C7
	~CResponse();

	/** Returns the status (returned as a reference; use this call to set it)
	*/
	//##ModelId=3C4C187F01BF
	inline TInt& Status();

	/** Returns the status
	*/
	//##ModelId=3C4C187F01C0
	inline const TInt& Status() const;

	/** Returns the status string (returned as a reference, which you can set)
	*/
	//##ModelId=3C4C187F01BD
	inline RStringF& StatusString();

	/** Returns the status string
	*/
	//##ModelId=3C4C187F01BE
	inline const RStringF& StatusString() const;

	/** Returns the HTTP version (returned as a reference, which you can set)
	*/
	//##ModelId=3C4C187F01B5
	inline TVersion& Version();

	/** Returns the HTTP version
	*/
	//##ModelId=3C4C187F01B6
	inline const TVersion& Version() const;

	/** Obtain a handle for this response that can be used by the client.
	*/
	//##ModelId=3C4C187F01B4
	inline RHTTPResponse Handle();

 private:
	/** Constructor
	*/
	//##ModelId=3C4C187F01B3
	CResponse();

 private:

	/** The HTTP status code obtained from the HTTP server's response
	*/
	//##ModelId=3C4C187F01A1
	TInt iStatus;

	/** The HTTP status description string obtained from the HTTP server's response
	*/
	//##ModelId=3C4C187F0197
	RStringF iStatusString;

	/** The HTTP version number (major/minor) obtained from the HTTP server's response
	*/
	//##ModelId=3C4C187F018E
	TVersion iVersion;
	};

#include "CResponse.inl"


#endif // __CRESPONSE_H__
