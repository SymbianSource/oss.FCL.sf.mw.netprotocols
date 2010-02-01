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

#ifndef __CMESSAGE_H__
#define __CMESSAGE_H__

#include <e32base.h>
#include <http/rhttpheaders.h>
#include "CTransaction.h"
#include "CHeaders.h"


/** Abstract base class for implementation of HTTP requests and responses
*/
//##ModelId=3C4C188200EF
class CMessage : public CBase
	{
public:
	/** Destructor.
		(NB Although this method has an implementation - this class must be abstract)
	*/
	//##ModelId=3C4C18820153
	~CMessage() = 0;
	
	/** Specify the body data supplier for this HTTP message.
		@see RHTTPMessage::AddBody
	*/
	//##ModelId=3C4C1882014B
	inline void AddBody(MHTTPDataSupplier* aBody);

	/** Determine whether this HTTP message has a body or not.
		@see RHTTPMessage::HasBody
	*/
	//##ModelId=3C4C1882014A
	inline TBool HasBody() const;

	/** Obtain this HTTP message's body data supplier.
		@see RHTTPMessage::GetBody
	*/
	//##ModelId=3C4C18820149
	inline MHTTPDataSupplier* Body() const;

	/** Obtain the message headers.
	*/
	//##ModelId=3C4C18820141
	inline RHTTPHeaders Headers() const;

protected:

	/** Does base class 2 phase construction
	*/
	//##ModelId=3C4C1882013F
	inline void ConstructL(CTransaction* aTransaction);

private:

	/** The header fields
	*/
	//##ModelId=3C4C1882012D
	CHeaders* iHeaders;

	/** The associated transaction
	*/
	//##ModelId=3C4C18820123
	CTransaction* iTransaction;

	/** The body
	*/
	//##ModelId=3C4C18820110
	MHTTPDataSupplier* iDataSupplier;

	friend class RHTTPMessage;
	friend class RHTTPRequest;
	friend class RHTTPResponse;
	};

#include "CMessage.inl"


#endif // __CMESSAGE_H__
