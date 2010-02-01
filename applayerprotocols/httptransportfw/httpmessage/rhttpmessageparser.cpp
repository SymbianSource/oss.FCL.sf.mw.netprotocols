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

#include "rhttpmessageparser.h"

#include "chttpmessageparser.h"

EXPORT_C void RHttpMessageParser::OpenL(MHttpMessageParserObserver& aObserver)
/**
	Opens the object.
	@param		aObserver	The observer for the parser.
	@panic		EInvariantFalse	This RHttpMessageParser has already been opened.
*/
	{
	__ASSERT_ALWAYS( iImplementation == NULL, User::Invariant() );

	iImplementation = CHttpMessageParser::NewL(aObserver);
	}

EXPORT_C void RHttpMessageParser::Close()
/**
	Closes the object. Any resources are freed.
*/
	{
	delete iImplementation;
	iImplementation = NULL;
	}

EXPORT_C void RHttpMessageParser::ReceivedMessageData()
/**
	Notifies the parser of more message data. The parser gets the data packet
	from the observer and continues processing its state machine.
	@panic		EHttpMessagePanicBadDataState	The current data packet has not 
												been completely parsed.
*/
	{
	iImplementation->ReceivedMessageData();
	}

EXPORT_C void RHttpMessageParser::CompletedBodyDataL()
/**
	Notifies the parser of that there is no more body data and should complete
	its processing. This is used for HTTP 1.0 style responses where the content
	length is unknown.
*/
	{
	iImplementation->CompletedBodyDataL();
	}

EXPORT_C void RHttpMessageParser::Reset()
/**
	Parser reset request. As the observer can reset the parser during one the 
	callback functions, the parser must check for re-entrancy to avoid releasing
	resources that are still required. If the parser is either waiting for more
	message data or is waiting to process its state machine, the parser can 
	safely reset immediately. Otherwise the parser is being reset from within 
	its RunL() and so it must defer resetting itself to a safer point. This is
	the point in the RunL() where the next step is decided.
	@panic		EHttpMessagePanicDoubleReset	The parser has been reset twice 
												in one of the observer callback 
												functions.
*/
	{
	iImplementation->Reset();
	}

EXPORT_C void RHttpMessageParser::Reserved_RHttpMessageParser()
/**
	Reserved function.
	@panic		EInvariantFalse	This function should not be called.
*/
	{
	User::Invariant();
	}
	
/**
 Tell the parser to finish up with the message if required. 
 */
EXPORT_C void RHttpMessageParser::Flush ()
	{
	iImplementation->Flush ();
	}

EXPORT_C TBool RHttpMessageParser::CompleteMessage ( const TDesC8& aData )
	{
	return iImplementation->CompleteMessage ( aData );
	}


