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

#ifndef __RHTTPMESSAGECOMPOSER_H__
#define __RHTTPMESSAGECOMPOSER_H__

#include <e32std.h>

class CHttpMessageComposer;
class MHttpMessageComposerObserver;

class RHttpMessageComposer
/**	
The RHttpMessageComposer class provides functionality for creating HTTP/1.1 
messages as defined in RFC2616. The HTTP/1.1 protocol specifies that the CR
LF sequence is the end of line (eol) marker for all protocol elements except
the entity-body.

The composer does not process any header fields or start-line tokens. 
Therefore it needs to be told if headers and/or an entity body is expected. 
If the message does contain an entity body and the body data is streamed the
composer will apply the chunked transfer encoding to the entity body.

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
asks its observer if the message has any headers. If there are headers the 
composer moves to the CreatingHeaders state otherwise it moves to the
CreatingEndOfHeaders state.

In the CreatingHeaders state the composer adds header fields to the current
data buffer. The observer provides the header field tokens for the field name
and field value. The composer will not fold header field values onto multiple
lines unless the provided field token already contains folded field values.
The observer also informs the composer if the provided field info is for the
last header field. If this is the case the composer moves into the 
CreatingEndOfHeaders state, otherwise it remains in the CreatingHeaders state.

In the CreatingEndOfHeaders state the composer adds an empty line to the 
current data buffer to mark the end of the header fields section. The current
data buffer is ready to be sent and the composer informs its observer of this.
The composer moves into the PendingEntityBody and waits for the observer to
notify it that it has finished with the current data buffer.

In the PendingEntityBody state the releases the current data buffer. It then
asks its observer if the message has an entity body. If there is no entity 
body or if it is zero length then the composer moves to PendingIdle state. 
If there is an entity body and the size is known the composer moves to the 
SendEntityData state. If the entity body size is unknown, implying streamed
entity body data, the composer moves to the CreatingChunkSize state.

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
field info is for the last trailer header field. If this is the case the 
composer moves into the CreatingEndOfTrailers state, otherwise it remains in 
the CreatingTrailers state.

In the CreatingEndOfTrailers state the composer adds an empty line to the 
current data buffer to mark the end of the trailer header fields section. 
The current data buffer is ready to be sent and the composer informs its 
bserver of this. The composer moves into the PendingEndOfChunkedBody and 
waits for the observer to notify it that it has finished with the current 
data buffer.

In the PendingEndOfChunkedBody the composer releases the current data buffer
and moves into the PendingIdle state. 

In the PendingIdle state the composer informs its observer that the message
is complete. The composer moves to the Idle state.
@see		MHttpMessageComposerObserver
*/
	{
public:	// methods

	inline RHttpMessageComposer();

	IMPORT_C void OpenL(MHttpMessageComposerObserver& aObserver);
	IMPORT_C void Close();
	IMPORT_C void MessageInfoAvailable();
	IMPORT_C void GetMessageData(TPtrC8& aData);
	IMPORT_C void ReleaseMessageData();
	IMPORT_C void Reset();
	IMPORT_C TBool CheckMessagePendingComplete();

private:	// methods

	IMPORT_C void Reserved_RHttpMessageComposer();

private:	// attributes

	CHttpMessageComposer*	iImplementation;

	};

inline RHttpMessageComposer::RHttpMessageComposer()
: iImplementation(NULL)
	{
	}

#endif	// __RHTTPMESSAGECOMPOSER_H__

