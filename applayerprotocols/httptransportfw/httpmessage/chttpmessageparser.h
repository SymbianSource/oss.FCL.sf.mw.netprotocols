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

#ifndef __CHTTPMESSAGEPARSER_H__
#define __CHTTPMESSAGEPARSER_H__

#include <e32base.h>

#include "thttpdataparser.h"
#include "mhttpbuffersupplier.h"

class MHttpMessageParserObserver;

class CHttpMessageParser : public CActive,
						   public MHttpBufferSupplier
/**	
	The CHttpMessageParser class provides parsing functionality for HTTP/1.1 
	messages as defined in RFC2616. The HTTP/1.1 protocol specifies that the CR
	LF sequence is the end of line (eol) marker for all protocol elements except
	the entity-body. The parser tolerates some deviation from this, as the RFC
	recommends in section 19.3 - a LF without the leading CR can also be parsed
	as an eol marker.

	The parser does not process any header fields. Therefore it needs to be told
	if an entity body is expected. If one is then the parser needs to know if it
	is chunk-encoded and if not what the expected size is.

	With a chunked entity body the parser de-chunks the body data - the observer
	does not need to parse a chunked body.

	The parser needs an observer (MHttpMessageParserObserver). The observer 
	supplies the buffers containing the unparsed http message data. The observer
	must ensure that the buffers it supplies to the parser remain valid until the
	parser notifies it that it has parsed the entire contents of that buffer.

	The parser is initially in the Idle state waiting to be notified of available
	meesage data. When it is notified, the parser obtains the data and moves
	into the ParsingStartLine state.

	Whichever state it is in, the parser will notify the observer when it has
	parsed the entire contents of the current data buffer. It will then wait to
	be told that there is more data available. When it has been given the next 
	part of the message data it continues parsing.

	In the ParsingStartLine state the parser looks for the first eol marker. 
	This delimits the start-line. Once found the observer is notified and the
	parser moves into the ParsingHeaders state.

	In the ParsingHeaders state the parser searches for header field lines. 
	These lines are delimited by eol markers. In HTTP/1.1 it is possible to fold
	header field values over multiple lines if the continuation line is followed
	by a SP or HT. In this case the eol marker is part of LWS and is ignored. 
	Also any eol markers that are part of LWS are omitted from the header field
	data given to the observer.

	The header field section is completed once an empty line is parsed. The
	observer is informed and it should supply the parser with the necessary
	info about the entity body. If no entity body is expected then the parser 
	moves to the MessageComplete state. If a non-encoded entity body is expected
	then the parser moves to the ReadingBodyData state. If a chunk-encoded 
	entity body is expected then the parser moves to the ParsingChunkSize state.

	In the ReadingBodyData state the parser extracts the specified length of 
	entity body data. The observer is notified of each chunk body data parsed. 
	It is possible for the entity body data to be received in several parts due
	to segmentation at the transport layer, Once all the entity body data has 
	been received the parser notifies the observer that the entity body is 
	complete. The parser moves to MessageComplete state.

	Note that although the parser will have notified the observer that it has 
	finished with the current data packet that held some entity body data, the
	observer should only release that data packet once it itself has no more use
	for the entity body data it has been given. Failure to do this will result
	in the buffer containing the received entity body chunks being invalid. The
	same is true when receiving a chunked entity body.

	In the ParsingChunkSize the parser searches for a chunk-size component as 
	defined in RFC2616 section 3.6.1. The chunk-size component is delimited by
	an eol marker. An optional chunk-extension component can be present between
	the chunk-size and the eol marker - the parser will ignore any 
	chunk-extension components. The chunk-size is a hex number specifying the 
	size of the subsequent chunk-data component. A chunk-size of value zero 
	indicates a last-chunk token - there are no subsequent chunk-data components
	and the parser moves to the ParsingTrailerHeaders state. A chunk-size of any
	other value indicates a subsequent chunk-data component and the parser moves
	to the ParsingChunkData state.

	In the ParsingChunkData state the parser extracts the length of entity body
	data specified in the preceeding chunk-size component. The observer is 
	notified of each chunk of entity body data. Note that a chunk-data component
	can be segmented in the transport layer resulting in the observer being 
	notified more than once for a given chunk-data component. The observer will
	not notice any disparate behaviour between receiving an entity body that has 
	not been chunk-encoded and one that has. Once the entire chunk-data has been 
	received the parser moves to the ParsingChunkSize state.

	The ParsingTrailerHeaders state is very similar to the ParsingHeaders state.
	The observer is notified of each header field and the trailer headers 
	section is delimited by an empty line. The observer is notified of the end
	of the trailer headers section and the parser moves to MessageComplete state.

	In the MessageComplete state the parser notifies the observer that the
	message is complete. The observer is presented with any remaining data in 
	the current data packet. This can be either excess data indicating a mal-
	formed message or the start of a pipelined message. In either case if the 
	observer wishes to use that data it must not release the current data packet
	as that will invalidate the data.
	@internalComponent
	@see		MHttpMessageParserObserver
*/
	{
public:	// methods

	static CHttpMessageParser* NewL(MHttpMessageParserObserver& aObserver);	
	virtual ~CHttpMessageParser();

	void ReceivedMessageData();
	void CompletedBodyDataL();
	void Reset();
	void Flush ();

	TBool CompleteMessage ( const TDesC8& aData );

private:	// methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);

private:	// methods from MHttpBufferSupplier

	virtual void ReAllocBufferL(TInt aRequiredSize, TPtr8& aBuffer);
	virtual void DeleteBuffer();

private:	// enums

	enum TParserState
/**
	The TParserState enumeration defines the state machine for the http message
	parser.
*/
		{
		/**	The parser is idle. A new message can be parsed.
		*/
		EIdle					= 0,

		/**	The parser has started parsing and is looking for the start-line of
			the http message. This is delimited by the first eol marker.
		*/
		EParsingStartLine,

		/**	The parser is looking for header fields. Each header field is 
			delimited by an eol marker. The parser continues to parser for 
			header fields until an empty line is found - this marks the end of
			the header fields section.
		*/
		EParsingHeaders,

		/** The parser is looking for the chunk-size component. A chunk-size of
			value zero indicates that no more chunk-data is expected.			
		*/
		EParsingChunkSize,

		/**	The parser is looking for trailer header fields. Each trailer header
			field is delimited by an eol marker. The parser continues to parser 
			for trailer header fields until an empty line is found - this marks the 
			end of the trailer header fields section.
		*/
		EParsingTrailerHeaders,

		/** The parser is reading body data. The size of the body data was 
			supplied to the parser by its observer.
		*/
		EReadingBodyData,

		/**	The parser is reading chunk-data. The size of the this chunk-data 
			was specified in the previous chunk-size.
		*/
		EReadingChunkData,

		/** The parser has parsed the http message.
		*/
		EMessageComplete
		};


	enum TDataState
/**	
	The TDataState enumeration defines the state of the message parser as 
	regards to the current data packet.
*/
		{
		/**	The parser has parsed the previous data packet and is now expecting
			to be given more data to continue with its parsing.
		*/
		EWaitingForData			= 0,

		/**	The parser is parsing the current data packet.
		*/
		EGotData,

		/**	The parser has been reset. The current data packet is no longer 
			valid.
		*/
		EReset
		};

	enum TParsingStatus
/**	
	The TParsingStatus enumeration defines the status of parsing for the current
	state of the parser.
*/
		{
		/**	The current state has been completed.
		*/
		ESectionDone		= 0,

		/** The current state has not been completed.
		*/
		ESectionNotDone,

		/** The current data packet has been completely parsed. More data is
			required to continue parsing the message.
		*/
		EBufferEmpty,

		/** The parser should not continue parsing - the message has been parsed.
		*/
		EStop
		};

private:	// methods

	CHttpMessageParser(MHttpMessageParserObserver& aObserver);
	
	TParsingStatus ParseStartLineL();
	TParsingStatus ParseSingleHeaderL();
	TParsingStatus ParseHeadersL();
	TParsingStatus ReadBodyData(TPtrC8& aData);
	TParsingStatus ParseChunkSizeL();
	TParsingStatus ParseChunkDataL(TPtrC8& aData);

	void CompleteSelf();
	void DoReset();

private:	// attributes

	MHttpMessageParserObserver&	iObserver;
	THttpDataParser				iDataParser;
	TParserState				iParserState;
	TDataState					iDataState;
	TPtrC8						iData;		
	HBufC8*						iLineBuffer;
	TInt						iDataSizeLeft;

	};
	
#endif // __CHTTPMESSAGEPARSER_H__
