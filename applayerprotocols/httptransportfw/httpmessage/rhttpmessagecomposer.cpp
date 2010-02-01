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

#include "rhttpmessagecomposer.h"

#include "chttpmessagecomposer.h"

EXPORT_C void RHttpMessageComposer::OpenL(MHttpMessageComposerObserver& aObserver)
/**
	Opens the object.
	@param		aObserver	The observer for the composer.
	@panic		EInvariantFalse	This RHttpMessageComposer has already been opened.
*/
	{
	__ASSERT_ALWAYS( iImplementation == NULL, User::Invariant() );

	iImplementation = CHttpMessageComposer::NewL(aObserver);
	}

EXPORT_C void RHttpMessageComposer::Close()
/**
	Closes the object. Any resources are freed.
*/
	{
	delete iImplementation;
	iImplementation = NULL;
	}

EXPORT_C void RHttpMessageComposer::MessageInfoAvailable()
/**
	Notifies the composer that more message info is available. The composer can
	continue composing the message.
	@pre		The composer is waiting for more message info.
	@post		The composer continues composing the message.
	@panic		EHttpMessagePanicBadDataState	The composer was not waiting for
												more info.
*/
	{
	iImplementation->MessageInfoAvailable();
	}

EXPORT_C void RHttpMessageComposer::GetMessageData(TPtrC8& aData)
/**
	Get the current message data.
	@panic		EHttpMessagePanicBadDataState	There is no current data buffer 
												ready.
	@pre		The observer has been notified of available message data.
*/
	{
	iImplementation->GetMessageData(aData);
	}

EXPORT_C void RHttpMessageComposer::ReleaseMessageData()
/**
	Release the message data. The observer has finished with the current message 
	data buffer. The composer can continue composing the message.
	@panic		EHttpMessagePanicBadDataState	There is no current data buffer 
												ready.
	@pre		The observer has been notified of available message data.
	@post		The composer continues composing the message.
*/
	{
	iImplementation->ReleaseMessageData();
	}

EXPORT_C void RHttpMessageComposer::Reset()
/**
	Composer reset request. As the observer can reset the composer during one of 
	the callback functions, the composer must check for re-entrancy to avoid 
	releasing resources that are still required. If the composer is either 
	waiting for more message info or is waiting to process its state machine, 
	the composer can safely reset immediately. Otherwise the composer is being
	reset from within its RunL() and so it must defer resetting itself until a
	safer point - this is the point in the RunL() where the next step is decided.
	@panic		EHttpMessagePanicDoubleReset	The composer has been reset twice
												in one of the callback functions.
*/
	{
	iImplementation->Reset();
	}

EXPORT_C TBool RHttpMessageComposer::CheckMessagePendingComplete()
/**
	Message may be fully composed and sent. Have not yet received confirmation from socket.
*/
	{
	return iImplementation->CheckMessagePendingComplete();
	}

EXPORT_C void RHttpMessageComposer::Reserved_RHttpMessageComposer()
/**
	Reserved function.
	@panic	EInvariantFalse	This function should not be called.
*/
	{
	User::Invariant();
	}
