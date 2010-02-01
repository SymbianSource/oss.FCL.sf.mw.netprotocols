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

#ifndef __MHTTPMESSAGEPARSEROBSERVER_H__
#define __MHTTPMESSAGEPARSEROBSERVER_H__

#include <e32std.h>

class MHttpMessageParserObserver
/**	
The MHttpMessageParserObserver class is the observer API for the http message
parser. It allows the parser to notify its observer when it has found http
message components, e.g. the start-line. 

Also, the parser uses this API to obtain the data buffer containg http 
message info. The observer is responsible for ensuring that the supplied 
buffer remains valid until the parser notifies it that it has finished with
the data.
@see		RHttpMessageParser
*/
	{
public:	// enums

	enum TEntityBodyType
/**	
	The TEntityBodyType enumeration specifies the types of entity body. These 
	are used when informing the parser of the entity body size.
*/
		{
		/**	The entity body is chunk-encoded.
		*/
		EChunked		=		-2, 

		/**	The size of the entity body is not known.
		*/
		EUnknown		=		-1,

		/**	There is no entity body or the size is zero.
		*/
		ENoBody			=		0
		};

public:	// methods

/**	
	Obtain current data packet. The parser requires the data packet with the
	current http message data. The observer must ensure that the buffer remains
	valid until the parser releases the data.
	@param		aData	An output argument set to the buffer containing the 
						current http message data.
*/
	virtual void GetDataPacket(TPtrC8& aData) =0;

/**
	Current data is no longer needed. The parser has finished with the current
	data buffer. The observer may release that buffer.
*/
	virtual void ReleaseDataPacket() =0;

/**
	The message start-line. The parser has found the message start-line. The 
	observer is responsible for parsing it further as a request or status line.
	The parser ensures that the buffer containing the start-line remains valid
	for the duration of the call.
*/
	virtual void StartLineL(const TDesC8& aStartLine) =0;

/**	
	A header has been found. The parser has found a header field. Any header
	values that were folded onto multiple lines have been merged into a single
	line - any LWS between header values has been replaced by a SP or HT. Any
	leading or trailing has been removed. Although, multiple SPs or HTs between
	tokens in the field value have not been reduced to a single SP. The parser 
	ensures that the buffers containing the field name and value remain valid
	for the duration of the call. The observer is also notified of trailer 
	header fields using this API.
	@param		aFieldName		The header field name.
	@param		aFieldValue		The header field value.
*/
	virtual void HeaderL(const TDesC8& aFieldName, TDesC8& aFieldValue) =0;

/**	
	Request the size of the entity body. The parser has found the empty line that
	indicates the end of the header fields. The observer must indicate to the
	parser the size of entity body. The are four cases - 
	
		1) EChunked	- the entity is chunk-encoded.
		2) EUnknown	- the enity body length is unknown.
		3) ENoBody	- there is no entity body or the entity body length is zero.

	The fourth case is a positive value (greater than zero) that indicates the 
	expected number of bytes in the entity body.
	@return		An integer value greater than zero indicating the number bytes in
				the entity body. A value of EChunked indicates a chunked-body, 
				EUnknown an unknown size and ENoBody indicates that there is no
				entity body.
*/
	virtual TInt BodySizeL() =0;

/**	
	Entity body data. The parser has parsed entity body data. If the entity body
	was chunk-encoded the parser has removed the encoding. The supplied buffer
	contains just the body data and no encoding info. The parser does not do any
	copying of entity body data. Therefore the supplied buffer remains valid 
	until the observer releases it. The observer should not release the data 
	until the parser has notified it that it has finished with the current data
	packet.
	@param		aData	The buffer containing the entity body data.
*/
	virtual void BodyChunkL(const TDesC8& aData) =0;

/**	
	The entity body has been parsed. The parser has parsed all of the entity 
	body data.
*/
	virtual void BodyCompleteL() =0;

/**
	The message is complete. The parser has parsed the entire message. Any data
	in the current buffer that is part of the parsed message is supplied in the
	buffer argument. This data could be the start of the next message in the 
	case where pipelining of requests has occurred.
*/
	virtual void MessageCompleteL(const TPtrC8& aExcessData) =0;

/**
	Error notifier. The parser has experienced an error. It has reset itself and
	is no longer able to continue parsing the current message.
	@param		aError	The error code.
*/
	virtual TInt HandleParserError(TInt aError) =0;

private:	// methods

/**	
	Reserved function.
*/
	virtual void Reserved_MHttpMessageParserObserver() =0;
	
	};

#endif	// __MHTTPMESSAGEPARSEROBSERVER_H__
