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

#ifndef __CHTTPMESSAGECOMPOSER_H__
#define __CHTTPMESSAGECOMPOSER_H__

#include <e32base.h>
#include <http/mhttpdatasupplier.h>

#include "timerlogger.h"
#include "mhttpbuffersupplier.h"
#include "thttpdatacomposer.h"


class MHttpMessageComposerObserver;

class CHttpMessageComposer : public CActive,
							 public MHttpBufferSupplier
/**	
	The CHttpMessageComposer class provides functionality for creating HTTP/1.1 
	messages as defined in RFC2616. The HTTP/1.1 protocol specifies that the CR
	LF sequence is the end of line (eol) marker for all protocol elements except
	the entity-body.

	The composer does not process any header fields or start-line tokens. 
	Therefore it needs to be told if headers and/or an entity body is expected. 
	If the message does contain an entity body and the size of the body data is
	not known then the composer will apply the chunked transfer encoding to the
	entity body. 

	The composer needs an observer (MHttpMessageComposerObserver). The observer 
	supplies the message info such as the start-line tokens, header field tokens
	and other message info. An MHTTPDataSupplier object is used to supply the 
	entity body data if there is one. The observer must ensure that the 
	descriptors containing the tokens it supplies remain valid until the composer
	notifies it that the message is complete.

	The composer is initially in the Idle state waiting to be notified of 
	available message info. When it is notified, the composer moves into the 
	CreatingStartLine state.

	In the CreatingStartLine state the composer obtains the start-line tokens 
	from the observer. These are added to the current data buffer. The composer
	moves to the CreatingHeaders state.

	In the CreatingHeaders state the composer adds header fields to the current
	data buffer. The observer provides the header field tokens for the field 
	name and field value. The composer will not fold header field values onto 
	multiple lines unless the provided field token already contains folded field
	values. The observer also informs the composer if there is no more header 
	field data. The composer remains in the CreatingHeaders state until there 
	are no more header field values. 
	
	In this case the composer adds an empty line to the current data buffer to 
	mark the end of the header fields section. The current data buffer is ready
	to be sent and the composer informs its observer of this. The composer moves
	into the PendingEntityBody and waits for the observer to notify it that it 
	has finished with the current data buffer.
	
	In the PendingEntityBody state the composer releases the current data buffer.
	It then asks its observer if the message has an entity body. If there is no
	entity body or if it is zero length then the composer moves to PendingIdle 
	state. If there is an entity body and the size is known the composer moves 
	to the SendEntityData state. If the entity body size is unknown the composer
	moves to the CreatingChunkSize state.

	In the SendEntityBody state the composer gets the next entity body data part
	from the data supplier for the entity body. It sets this data as the current
	send buffer and notifies the observer that message data is ready. A check is
	made to see that if the data supplier claims that this is the last data part 
	that all the specified amount of entity body data has been sent. If this is 
	not true the observer will receive an error code of KErrCorrupt. The composer
	moves to PendingReleaseData state and waits for the observer to notify it 
	that it has finished with the current data buffer.

	In the PendingReleaseData state the composer notifies the entity body data
	supplier that it has finished with the current data part. If all the 
	specified entity body data has been sent the composer moves to the 
	PendingIdle state. If there is more entity body data to send the composer
	moves to the SendEntityData and waits for the observer to notify it that 
	there is more message info available.

	In the CreatingChunkSize state the composer adds the size of the following
	chunk-data component to the current send buffer as defined in RFC2616 
	section 3.6.1. The send buffer wil contain the empty line delimiting the 
	previous chunk-data component if this is not the first chunk-size component
	to be sent. The composer notifies its observer that message data is ready. 
	The composer moves into the SendChunkData state and waits for the observer 
	to notify it that it has finished with the current data buffer.

	If the chunk-data is zero-length then the chunk-size component is not added
	to the send buffer.	

	In the SendChunkData state the composer releases the current data buffer. It
	then gets the chunk-data from the data supplier for the entity body. It sets
	this data as the current buffer. The observer is only notified that message 
	data is ready if the chunk-data is not zer-length. The composer moves into 
	the PendingReleaseChunk state and waits for the observer to notify it that 
	it has finished with the current data buffer.

	In the PendingReleaseChunk state the composer notifies the entity body data
	supplier that it has finished with the current data part. If that was the 
	last part of the entity body data the composer adds the last-chunk component
	to the current data buffer. It then asks the observer if the message has any 
	trailer headers. If there are trailer headers then the composer moves to the 
	CreatingTrailers state. Otherwise the composer moves to the 
	CreatingEndOfTrailers state. 
	
	If there is more entity body data to follow the composer moves to the 
	CreatingChunkSize state and waits for the observer to notify it that there is 
	more message info available.

	In the CreatingTrailers state the composer adds trailer header fields to the 
	current data buffer. The observer provides the trailer header field tokens 
	for the field name and field value. The composer will not fold header field 
	values onto multiple lines unless the provided field token already contains 
	folded field values. The observer also informs the composer if the provided 
	field info is for the last trailer header field. The composer remains in the
	CreatingTrailers state until there are no more trailer header field values. 
	
	In this case the composer adds an empty line to the current data buffer to 
	mark the end of the trailer header fields section. The current data buffer 
	is ready to be sent and the composer informs its observer of this. The
	composer moves into the PendingEndOfChunkedBody and waits for the observer 
	to notify it that it has finished with the current data buffer.

	In the PendingEndOfChunkedBody the composer releases the current data buffer
	and moves into the PendingIdle state. 

	In the PendingIdle state the composer informs its observer that the message
	is complete. The composer moves to the Idle state.
	@internalComponent
	@see		MHttpMessageComposerObserver
*/
	{
public:	// methods

	static CHttpMessageComposer* NewL(MHttpMessageComposerObserver& aObserver);
	virtual ~CHttpMessageComposer();
	
	void MessageInfoAvailable();
	void GetMessageData(TPtrC8& aData);
	void ReleaseMessageData();
	void Reset();
	TBool CheckMessagePendingComplete();

private:	// enums

	enum TComposerState
/**	
	The TComposerState enumeration defines the state machine for the http message
	state machine.
*/
		{
		/** The composer is idle.
		*/
		EIdle						= 0,

		/** The start-line is being created. The next state depends on whether 
			there are headers or not.
		*/
		ECreatingStartLine,

		/** The message has header fields. The next header field is added to the
			http message. The composer remains in this state until all the header
			fields have been added. If there is no more header field info an 
			empty line is added. The composer has prepared the first part of the
			http message and can notify its observer that message data is ready.
		*/
		ECreatingHeaders,

		/** The composer needs to decide whether there is entity body and if so
			does it need to be chunk encoded or not.
		*/
		EPendingEntityBody,

		/** The message has a non-encoded entity body. It notifies its observer
			that message data is ready.
		*/
		ESendEntityData,

		/** The composer releases the current entity body data. If all the body
			data has been sent then the message is complete. Otherwise the 
			composer waits to be notified that more entity body data is available.
		*/
		EPendingReleaseData,

		/** There is entity body to sent as in chunk-encoded format. The chunk-
			size component specifying the size of the subsequent chunk-data is
			added to the send buffer. The composer notifies its observer that it
			has message ready. Note the send buffer will contain the empty line
			marking the end of the previous chunk-data if this is not the first
			chunk-size.
		*/
		ECreatingChunkSize,

		/** The chunk-data is ready to be sent. The observer is notified that 
			there is message data ready.
		*/
		ESendChunkData,

		/** The composer releases the current chunk-data. If this was the last
			chunk-data then the composer checks for trailer headers. If there 
			are no trailer then the message is complete, otherwise the trailers
			are added.
		*/
		EPendingReleaseChunk,

		/** The message has trailer header fields. The next trailer header field
			is added to the http message. The composer remains in this state 
			until all the trailer header fields have been added. If there is no 
			more trailer header field info an empty line is added. The composer 
			has prepared the final part of the http message and can notify its 
			observer that message data is ready.
		*/
		ECreatingTrailers,

		/** The composer releases the current data buffer. The observer needs
			to be informed that message is complete.
		*/
		EPendingEndOfChunkedBody,

		/** The message is complete and the composer is waiting to go idle. The
			observer is notified.
		*/
		EPendingIdle
		};

	enum TDataState
/**	
	The TDataState enumeration defines the state of the message parser as 
	regards to the current data packet.
*/
		{
		/**	The composer is waiting for more message info to be available to be
			able to continue composing.
		*/
		EWaitingForInfo			= 0,

		/**	The composer is creating the message from the available info.
		*/
		EGotInfo,

		/** The composer is waiting for the observer to release the current data
			buffer before continuing to compose the message.
		*/
		EWaitingForRelease,

		/**	The composer has been reset.
		*/
		EReset
		};

	enum TComposingStatus
/**	
	The TComposingStatus enumeration defines the status of composing for the 
	current state of the composer.
*/
		{
		/**	The current state has been completed.
		*/
		ESectionDone		= 0,

		/** The current state has not been completed.
		*/
		ESectionNotDone,

		/** The current data packet should be sent.
		*/
		ESendData,

		/** The composer should not continue. Either the message is complete or
			more message info is required to continue.
		*/
		EStop
		};

private:	// methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpBufferSupplier

	virtual void ReAllocBufferL(TInt aRequiredSize, TPtr8& aBuffer);
	virtual void DeleteBuffer();

private:	// methods

	CHttpMessageComposer(MHttpMessageComposerObserver& aObserver);

	TComposingStatus ComposeStartLineL();
	TComposingStatus ComposeHeadersL();
	TComposingStatus ComposeSingleHeaderL();
	TComposingStatus ComposeChunkSizeL();
	TComposingStatus ComposeLastChunkL();
	TComposingStatus ComposeTrailerL();

	void CompleteSelf();
	void DoReset();

private:	// attributes

	MHttpMessageComposerObserver&	iObserver;
	THttpDataComposer				iDataComposer;
	TComposerState					iState;
	TDataState						iDataState;
	HBufC8*							iDataBuffer;
	TPtrC8							iSendBuffer;
	MHTTPDataSupplier*				iBodyData;
	TInt							iDataSizeLeft;
	TBool							iLastChunk;
	__DECLARE_PERFORMANCE_LOG
	};

#endif	// __CHTTPMESSAGECOMPOSER_H__
