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

/**
 @file
 @warning : This file contains Rose Model ID comments - please do not delete
*/

#ifndef __RHTTPMESSAGE_H__
#define __RHTTPMESSAGE_H__

// Forward declarations
class RHTTPHeaders;
class MHTTPDataSupplier;
class CMessage;


//##ModelId=3A375D1F0377
class RHTTPMessage 
/** 
An abstract HTTP message. In RFC2616, an HTTP message is defined as having a
header and an optional body.  This class is specialised for HTTP requests and
responses.
@publishedAll
@released
*/
	{
 public:
	// Default (uninitialised) constructor.
	//##ModelId=3A375DD60140
	inline RHTTPMessage();

	/** Returns the header collection for the message */
	//##ModelId=3A375DDE0368
	IMPORT_C RHTTPHeaders GetHeaderCollection();

	/** Sets the messages body, replacing any existing body. The body
        is supplied as a MHTTPDataSupplier, which will then return the
        data in 1 or more chunks on request.  
		@param aBody The data supplier that will supply the body.  */
	//##ModelId=3A375DE0019E
	IMPORT_C void SetBody(MHTTPDataSupplier& aBody);

	/** Removes the body */
	//##ModelId=3B1E66FF004E
	IMPORT_C void RemoveBody();

	/** Determine whether this message has any associated body data.
		@return ETrue if there is some body data, EFalse if not.
	*/
	//##ModelId=3A914DFF01E5
	IMPORT_C TBool HasBody() const;

	/** Gets the body. The body is supplied as a MHTTPDataSupplier,
        which can be used to return the current data chunk, and to
        acknowledge when that chunk has been consumed by the client.

		Note that the client can assume the body object will remain
		unchanged from when it receives the first data until the end
		of the transaction.

		@return A data supplier that provides access to a body
		data chunk. NULL if the body has not been set, or has been removed */
	//##ModelId=3A375DE40190
	IMPORT_C MHTTPDataSupplier* Body() const;

 protected:
	//##ModelId=3A914DFF01C7
	CMessage* iImplementation;
};


inline RHTTPMessage::RHTTPMessage()
		: iImplementation(NULL)
	{
	};


#endif // __RHTTPMESSAGE_H__ 
