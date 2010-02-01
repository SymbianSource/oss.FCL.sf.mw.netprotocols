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

#ifndef __MHTTPMESSAGECOMPOSEROBSERVER_H__
#define __MHTTPMESSAGECOMPOSEROBSERVER_H__

#include <e32std.h>

class MHTTPDataSupplier;

class MHttpMessageComposerObserver
/**	
The MHttpMessageComposerObserver class is the observer API for the http 
message composer. It allows the composer to notify its observer when it has
http message data ready to send and when the message has been completed.

Also, the composer uses this API to obtain the http message info, e.g the 
required start-line. The observer is responsible for ensuring that the 
supplied tokens remain valid until the composer notifies it that it has 
completed the message.

The observer is notified of any errors that occur in the composer.	
@see		RHttpMessageComposer
*/
	{
public:	// methods

/**	
	Provides the start-line tokens.
	@param		aToken1		An output argument set to the first token.
	@param		aToken2		An output argument set to the second token.
	@param		aToken3		An output argument set to the third token.
*/
	virtual void StartLineL(TPtrC8& aToken1, TPtrC8& aToken2, TPtrC8& aToken3) =0;

/**	
	Provides the next header field tokens. The return value indicates whether
	there is a header or not. If there is no more headers, then the output 
	arguments are not valid and therefore should not be used.
	@param		aHeaderName		An output argument set to the header field name.
	@param		aHeaderValue	An output argument set to the header field value.
	@return		A value of KErrNotFound if there are no more headers. A value 
				of KErrNone if a header field is specified in the output 
				arguments.
*/
	virtual TInt NextHeaderL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue) =0;

/**
	Request to see if there is an entity body.
	@return		A value of NULL if there is no entity body. If there is a entity
				body a pointer to the data supplier object that will supply the
				entity body data is returned.
*/
	virtual MHTTPDataSupplier* HasBodyL() =0;

/**
	Provides the next trailer header field tokens. The return value indicates 
	whether there is a trailer header or not. If there is no more headers, then
	the output arguments are not valid and therefore should not be used.
	@param		aHeaderName		An output argument set to the header field name.
	@param		aHeaderValue	An output argument set to the header field value.
	@return		A value of KErrNotFound if there are no more trailers. A value 
				of KErrNone if a trailer header field is specified in the output
				arguments.
*/
	virtual TInt NextTrailerL(TPtrC8& aHeaderName, TPtrC8& aHeaderValue) =0;

/**	
	Notifies the observer that the message has been completed.
*/
	virtual void MessageComplete() =0;

/**
	Notifies the observer that there is message data ready to be sent.
*/
	virtual void MessageDataReadyL() =0;

/**
	Notifies the observer that an error has ocurred in the composer. The 
	composer has reset itself.
	@param		aError	The error code that has occurred.
	@return		A value of KErrNone if the error has been handled. Any other 
				value if the error has not been handled.
*/
	virtual TInt HandleComposeError(TInt aError) =0;

private:	// methods

/**
	Reserved function.
*/
	virtual void Reserved_MHttpMessageComposerObserver() =0;

	};

#endif	// __MHTTPMESSAGECOMPOSEROBSERVER_H__
