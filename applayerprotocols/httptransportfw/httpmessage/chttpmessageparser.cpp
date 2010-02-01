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

#include "chttpmessageparser.h"

#include <inetprottextutils.h>
#include <uriutilscommon.h>

#include "mhttpmessageparserobserver.h"
#include "thttpmessagepanic.h"

const TInt KDefaultBufferSize	= 128;
const TUint KColon				= ':';
_LIT8 ( KLineFeed, "\n" );

// 'this' used in base member initializer list, The 'this' pointer being used is a base class pointer.
#pragma warning( disable : 4355 )

CHttpMessageParser* CHttpMessageParser::NewL(MHttpMessageParserObserver& aObserver)
/**
	Factory constructor. 
	@param		aObserver	The observer for the parser.
	@return		A pointer to a fully constructed and initialised object.
*/
	{
	return new (ELeave) CHttpMessageParser(aObserver);
	}

CHttpMessageParser::~CHttpMessageParser()
/**
	Destructor
*/
	{
	Cancel();

	// Cleanup
	delete iLineBuffer;
	}

CHttpMessageParser::CHttpMessageParser(MHttpMessageParserObserver& aObserver)
: CActive(CActive::EPriorityStandard + 1), iObserver(aObserver), iDataParser(*this)
/**
	Constructor.
	See note in CHttpMessageComposer
*/
	{
	CActiveScheduler::Add(this);
	}

void CHttpMessageParser::ReceivedMessageData()
/**
	Notifies the parser of more message data. The parser gets the data packet
	from the observer and continues processing its state machine.
	@panic	EHttpMessagePanicBadDataState	The current data packet has not been
											completely parsed.
*/
	{
	__ASSERT_DEBUG( iDataState == EWaitingForData, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataState) );

	// Have now got some data - update the data state
	iDataState = EGotData;

	// Get the received data packet
	TPtrC8 data;
	iObserver.GetDataPacket(data);

	// Pass it to the data parser
	iDataParser.SetData(data);

	// Continue parsing...
	CompleteSelf();
	}

void CHttpMessageParser::CompletedBodyDataL()
/**
	Tells the parser that there is no more body data. When using HTTP 1.0 style
	responses the body data maybe of unknown length so this method allows clients
	to inform the parser that the body data is completed.
*/
	{
	iDataSizeLeft = 0;
	
	// Notify the observer that the body has been received
	iObserver.BodyCompleteL();

	// Notify the observer that the message is complete.
	iObserver.MessageCompleteL(KNullDesC8());

	// Move to the Idle state.
	iParserState = EIdle;
	iDataState = EWaitingForData;
	}

void CHttpMessageParser::Reset()
/**
	Parser reset request. As the observer can reset the parser during one the 
	callback functions, the parser must check for re-entrancy to avoid releasing
	resources that are still required. If the parser is either waiting for more
	message data or is waiting to process its state machine, the parser can 
	safely reset immediately. Otherwise the parser is being reset from within 
	its RunL() and so it must defer resetting itself to a safer point. This is
	the point in the RunL() where the next step is decided.
	@panic EHttpMessagePanicDoubleReset	The parser has been reset twice in one 
										of the observer callback functions.
*/
	{
	// Check the data state of the parser - the parser cannot be reset if the 
	// Reset() was called in one of the observer callbacks. It is safe to reset
	// now if - 
	// 1) the data state is WaitingForData
	// 2) the parser is active - waiting to for its RunL() to be called.
	if( iDataState == EWaitingForData || IsActive() )
		{
		// Cancel and do the reset.
		Cancel();
		DoReset();
		}
	else
		{
		// Debug check for a double Reset() call...
		__ASSERT_DEBUG( iDataState != EReset, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicDoubleReset) );

		// The Reset() was called inside a callback - defer resetting the parser
		// until call stack back in the parser RunL().
		iDataState = EReset;
		}
	}

void CHttpMessageParser::CompleteSelf()
/**
	Self-complete function. Ensures that the state machine is processed.
*/
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CHttpMessageParser::DoReset()
/**
	Resets the parser. The parser moves into the Idle state. Allocated resources 
	are also reset. 
*/
	{
	// Reset the parser - parser state should be Idle and the data state should
	// be WaitingForData.
	iParserState = EIdle;
	iDataState = EWaitingForData;

	// Reset the data parser
	iDataParser.Reset();
	}

CHttpMessageParser::TParsingStatus CHttpMessageParser::ParseStartLineL()
/**
	Parses for the start-line. The start-line is delimited by the first eol 
	marker. No further parsing of the start-line is done by the parser - it is
	left to the observer to parse for either a request-line or a status-line.
	The observer is informed of the start-line.
	@return		The parsing status for the start-line. The value ESectionDone 
				indicates the start-line has been parsed. The value EBufferEmpty
				indicates that more message data is required to parse the 
				start-line.
	@panic		EHttpMessagePanicBadDataParserResult	An unexpected status was 
														returned by the data 
														parser.
*/
	{
	// Get line from the parser
	TPtrC8 startLine;
	THttpDataParser::TParseResult parseResult = iDataParser.GetLineL(startLine);

	// Check the parse status...
	TParsingStatus  startlineStatus = ESectionNotDone;
	switch( parseResult )
		{
	case THttpDataParser::ELineParsed:
		{
		// The start-line has been parsed - inform the observer.
		iObserver.StartLineL(startLine);
		startlineStatus = ESectionDone;
		} break;
	case THttpDataParser::EPartialData:
		{
		// Not all of start-line received - need more data.
		startlineStatus = EBufferEmpty;
		} break;
	case THttpDataParser::EEmptyLine:
		{
		// Although blank-lines are not allowed, be tolerent. Need to parse for 
		// the start-line again. The start-line is not complete - use default 
		// return value.
		} break;
	default:
		// This covers EGotData case.
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataParserResult);
		break;
		}
	return startlineStatus;
	}

CHttpMessageParser::TParsingStatus CHttpMessageParser::ParseSingleHeaderL()
/**
	Parses for a header field line. A header field line is delimited by an eol 
	marker. If an empty line is parser then this indicates the end of the header
	fields section. If a header field is found then the field name and field
	value are extracted. The observer is passed the field name and value.
	
	If a malformed header field is found (ie the colon ':' is missing) then that
	line is ignored. The function will leave if an error occurs in removing the 
	leading and trailing whitespace from the parsed header field name. 
	
	Note that the header field value may be empty.
	@return		The parsing status for the header fields section. The value 
				ESectionDone indicates that an empty line has been parsed which
				means that all the header fields have been parsed. The value 
				ESectionNotDone indicates that a header field has been found. 
				The	value EBufferEmpty indicates that more message data is 
				required to parse the header fields section.
	@panic		EHttpMessagePanicBadDataParserResult	An unexpected status was 
														returned by the data 
														parser.
*/
	{
	// Get line from the parser
	TPtrC8 line;
	THttpDataParser::TParseResult parseResult = iDataParser.GetHeaderLineL(line);

	// Check the parse status...
	TParsingStatus  headerStatus = ESectionNotDone;
	switch( parseResult )
		{
	case THttpDataParser::ELineParsed:
		{
		// Got a header field - find the field name and value.
		TInt colonPos = line.Locate(KColon);

		if( colonPos == KErrNotFound )
			{
			// No colon - syntax error. Be robust and ignore the this line. 
			// There are still more headers to find - use default return value.
			break;
			}

		// Found the field name and value
		TPtrC8 name = line.Left(colonPos);
		TPtrC8 value = line.Mid(colonPos + 1);	// move past the colon

		// Remove any leading/trailing whitespace but if it is empty, try to be robust and continue with the next one.
		TInt rwsErr = InetProtTextUtils::RemoveWhiteSpace(name, InetProtTextUtils::ERemoveBoth);

		// Field value may be empty - e.g. "host:". Ignore any returned error.
		InetProtTextUtils::RemoveWhiteSpace(value, InetProtTextUtils::ERemoveBoth);

		// Pass the header field name and value to the observer if it seemed valid.
		if( rwsErr == KErrNone )
			{
			iObserver.HeaderL(name, value);
			}
 
		// There are still more headers to find - use default return value.
		} break;
	case THttpDataParser::EPartialData:
		{
		// Not all of header field received - need more data.
		headerStatus = EBufferEmpty;
		} break;
	case THttpDataParser::EEmptyLine:
		{
		// Reached the end of the headers section - move on.
		headerStatus = ESectionDone;
		} break;
	default:
		// This covers EGotData case.
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataParserResult);
		break;
		}
	return headerStatus;
	}

CHttpMessageParser::TParsingStatus CHttpMessageParser::ParseHeadersL()
    {
    // Do parsing of 5 headers without AO scheduling
    TParsingStatus  headerStatus = ParseSingleHeaderL();
    if(headerStatus == ESectionNotDone)
        {
        headerStatus = ParseSingleHeaderL();
        if(headerStatus == ESectionNotDone)
            {
            headerStatus = ParseSingleHeaderL();
            if(headerStatus == ESectionNotDone)
                {
               headerStatus = ParseSingleHeaderL();
                if(headerStatus == ESectionNotDone)
                    {
                    headerStatus = ParseSingleHeaderL();
                    }
                }
            }
        }
    return headerStatus;
    }

CHttpMessageParser::TParsingStatus CHttpMessageParser::ReadBodyData(TPtrC8& aData)
/**
	Reads the entity body data from the current data packet. As the entity body
	data can be segmented by the tranport layer this function may need to be 
	called more than once. The remaining amount of entity body data is updated.
	@param		aData	An output argument set the entity body data extracted 
						from the current data packet.
	@return		The parsing status for the entity body. The value ESectionDone 
				indicates the entity body has been extracted. The value 
				EBufferEmpty indicates that more message data is required to 
				extract the entity body.
	@panic		EHttpMessagePanicBadDataParserResult	An unexpected status was 
														returned by the data 
														parser.
	@panic		EInvariantFalse	The data parser returned a status that conflicts
								with the amount of entity body data to extract.
*/
	{
	// If the body length is unknown then set the data size to a maximum value
	if(iDataSizeLeft==-1)
		iDataSizeLeft = KMaxTInt;

	// Get the remaining body data from the data parser
	THttpDataParser::TParseResult parseResult = iDataParser.GetData(aData, iDataSizeLeft);

	// Update the size of the data left to get
	iDataSizeLeft -= aData.Length();

	// Check the parse status...
	TParsingStatus  dataStatus = EBufferEmpty;
	switch( parseResult )
		{
	case THttpDataParser::EGotData:
		{
		__ASSERT_DEBUG( iDataSizeLeft == 0, User::Invariant() );

		// Got all the data - section is done.
		dataStatus = ESectionDone;
		} break;
	case THttpDataParser::EPartialData:
		{
		__ASSERT_DEBUG( iDataSizeLeft > 0, User::Invariant() );

		// Use the default value...
		} break;
	default:
		// This covers ELineParsed and EEmptyLine cases.
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataParserResult);
		break;
		}
	return dataStatus;
	}

CHttpMessageParser::TParsingStatus CHttpMessageParser::ParseChunkSizeL()
/**
	Parses for a chunk-size component. The chunk-size component specifies the 
	size of following chunk-data component. As the chunk-size can be segmented 
	at the transport layer this function may need to called several times for 
	the same chunk-size component.

	Section 3.6.1 in RFC2616 defines the following - 
		chunk			=	chunk-size [ chunk-extension ] CRLF
							chunk-data CRLF
		chunk-size		=	1*HEX
		chunk-extension	=	*( ";" chunk-ext-name [ "=" chunk-ext-val ] )

	The chunk-size component is delimited by the eol marker. There can be an 
	optional chunk-extension component between the chunk-size and the eol marker.
	This is parsed but ignored - a leading ';' is not checked for. Also, if an
	empty line is parsed this is also ignored. The function will also leave if
	there is an error in converting the hex number to its numeric value.
	@return		The parsing status for this chunk-size component. The value 
				ESectionDone indicates that all the chunk-data has been parsed
				and the value EBufferEmpty indicates that more message data is
				required to parse this chunk-size component.
	@leave		THttpDataParser::GetLineL
	@panic		EHttpMessagePanicBadDataParserResult	An unexpected status was 
														returned by the data 
														parser.
*/
	{
	// Get line from the parser
	TPtrC8 line;
	THttpDataParser::TParseResult parseResult = iDataParser.GetLineL(line);

	// Check the parse status...
	TParsingStatus  sizeStatus = ESectionDone;
	switch( parseResult )
		{
	case THttpDataParser::ELineParsed:
		{
		// Ok got the line - find the hex number defining the chunk-size.
		TInt val = InetProtTextUtils::ConvertDescriptorToHex(line, iDataSizeLeft);
		if( val < 0 )
			{
			// Error: invalid data.. ignore it.
			sizeStatus = ESectionDone;
			}
		// Do not care about the chunk-extension - do not bother to check that 
		// it is well formed.

		// The chunk-size has been parsed - use default return value.
		} break;
	case THttpDataParser::EPartialData:
		{
		// Not all of start-line received - need more data.
		sizeStatus = EBufferEmpty;
		} break;
	case THttpDataParser::EEmptyLine:
		{
		// The chunk-size was expected - got an empty line. Ignore.
		sizeStatus = ESectionNotDone;
		} break;
	default:
		// This covers EGotData case.
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataParserResult);
		break;
		}
	return sizeStatus;
	}

CHttpMessageParser::TParsingStatus CHttpMessageParser::ParseChunkDataL(TPtrC8& aData)
/**
	Parses for a chunk-data component. The preceeding chunk-size component 
	specified the length of this chunk-data component. As the chunk-data can be
	segmented at the transport layer this function may need to called several
	times for the same chunk-data component.

	Section 3.6.1 in RFC2616 defines the following - 
		chunk			=	chunk-size [ chunk-extension ] CRLF
   							chunk-data CRLF
   
	The chunk-data component is delimited by the eol marker. If the amount of
	chunk-data does not match the amount defined by the chunk-size then the
	function will leave with KErrCorrupt.
	@param		aData	An output argument set to the chunk-data parsed.
	@return		The parsing status for this chunk-data component. The value 
				ESectionDone indicates that all the chunk-data has been parsed
				and the value EBufferEmpty indicates that more message data is
				required to parse this chunk-data component.
	@leave		KErrCorrupt	An incorrect amount of chunk-data was parsed.
	@panic		EInvariantFalse	The body data extraction indicated that it had
								extracted all the body data but the parser still
								expected more.
	@panic		EHttpMessagePanicBadDataParserResult	An unexpected status was 
														returned by the data 
														parser.
*/
   	{
   	// Has the data-chunk been read? Check iDataSizeLeft.
   	TParsingStatus dataStatus = ESectionDone;
   	if( iDataSizeLeft > 0 )
   		{
   		// There is chunk-data to extract
   		dataStatus = ReadBodyData(aData);
   		}

	// Does the end CRLF need to be parsed? The chunk-data may have been found.
	if( dataStatus == ESectionDone )
		{
		__ASSERT_DEBUG( iDataSizeLeft == 0, User::Invariant() );

		// Need to extract an empty-line
		TPtrC8 line;
		THttpDataParser::TParseResult parseResult = iDataParser.GetLineL(line);

		// Check the parse status...
		switch( parseResult )
			{
		case THttpDataParser::EEmptyLine:
			{
			// Got the CRLF - section is done. Use default value.
			} break;
		case THttpDataParser::EPartialData:
			{
			dataStatus = EBufferEmpty;
			} break;
		case THttpDataParser::ELineParsed:
			{
			// There was more data than the chunk-size specified.
			
			// Error ignored to add robustness against problematic real-world servers.
			//User::Leave(KErrCorrupt);
			} break;
		default:
			// This covers EGotData case.
			THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataParserResult);
			break;
			}
		}
	return dataStatus;
	}

/*
 *	Methods from MHttpDataParserObserver
 */

void CHttpMessageParser::ReAllocBufferL(TInt aRequiredSize, TPtr8& aBuffer)
/**	
	Reallocates the line buffer. The parser supplies the line buffer to the data
	parser. The data parser needs more space to store the current line. If the 
	line buffer has not been created then it is created, otherwise it is 
	reallocated to at least the required size.
	@param		aRequiredSize	The minimum size of buffer required.
	@param		aBuffer			An output argument set to the reallocated buffer.
	@panic		EInvariantFalse	The required size was less then the current max
								size of the buffer.
*/
	{
	if( iLineBuffer == NULL )
		{
		// Create the buffer..
		iLineBuffer = HBufC8::NewL(aRequiredSize + KDefaultBufferSize);
		}
	else
		{
		__ASSERT_DEBUG( aRequiredSize > iLineBuffer->Des().MaxLength(), User::Invariant() );

		iLineBuffer = iLineBuffer->ReAllocL(aRequiredSize + KDefaultBufferSize);
		}
	aBuffer.Set(iLineBuffer->Des());
	}

void CHttpMessageParser::DeleteBuffer()
/**
	Deletes the line buffer.
	@internalComponent
*/
	{
	delete iLineBuffer;
	iLineBuffer = NULL;
	}

/*
 *	Methods from CActive
 */
void CHttpMessageParser::RunL()
/**
	Asynchronous request service handler. The parser state machine is processed
	in this function. Behaviour depends on the state. The parser will self-
	complete if the current data packet has unparsed data. If all the data in 
	the current data packet has been parsed then the parser suspends its state
	machine and waits for the observer to notify it when there is more data
	available. If the observer has reset the parser in one of the callback then
	the parser will defer resetting itself until it is back in the RunL().
	@panic		EHttpMessagePanicBadParserState	The parser state machine was in 
												a illegal state.
	@panic		EHttpMessagePanicBadDataState	The parser was in the incorrect
												data state.
	@panic		EHttpMessagePanicBadBodySize	The observer specified an unknown
												non-positive body size.
*/
	{
	__ASSERT_DEBUG( iDataState == EGotData, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataState) ); 
	TParsingStatus status = ESectionNotDone;
	switch( iParserState )
		{
	case EIdle:
		{
		// Move to the ParsingStartLine and set the parsing status to move on.
		iParserState = EParsingStartLine;
		status = ESectionDone;
		} 
		//coverity [MISSING_BREAK]
		// Fallthrough is required here as we no longer parse line by line. We parse startline & headers (5) at one go.
	case EParsingStartLine:
		{
		status = ParseStartLineL();

		// Has the start-line been parsed?
		if( status == ESectionDone )
			{
			// Start-line has been parsed - parse for headers next.
			iParserState = EParsingHeaders;
			}
		else
			{
			break; //Start line is not parsed yet as we haven't received the complete data. Break and wait for more data.
			}
		}
	case EParsingHeaders:
		{
		status = ParseHeadersL();

		if( status == ESectionDone )
			{
			// No more headers - obtain entity body size from the observer.
			iDataSizeLeft = iObserver.BodySizeL();
			switch( iDataSizeLeft )
				{
			case MHttpMessageParserObserver::EChunked:
				{
				// Chunk-encoded body.
				iParserState = EParsingChunkSize;
				} break;
			case MHttpMessageParserObserver::EUnknown:
				{
				// for HTTP/1.0-style responses
				iParserState = EReadingBodyData;
				} break;
			case MHttpMessageParserObserver::ENoBody:
				{
				// No body expected - message is complete.
				iParserState = EMessageComplete;
				} break;
			default:
				__ASSERT_DEBUG( iDataSizeLeft > 0, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadBodySize) );

				// Non-encoded body.
				iParserState = EReadingBodyData;
				break;
				}
			}
		} break;
	case EParsingChunkSize:
		{
		status = ParseChunkSizeL();

		if( status == ESectionDone )
			{
			// Need to check the chunk size for last-chunk
			if( iDataSizeLeft == 0 )
				{
				// Recieved a chunk of size 0, inform the observer.
				TPtrC8 data;
				iObserver.BodyChunkL(data);

				// Notify the observer that the body has been received
				iObserver.BodyCompleteL();

				// Received the last-chunk token - now parse for trailers.
				iParserState = EParsingTrailerHeaders;
				}
			else
				{
				// Still expecting chunk-data...
				iParserState = EReadingChunkData;
				}
			}
		} break;
	case EParsingTrailerHeaders:
		{
		status = ParseHeadersL();

		if( status == ESectionDone )
			{
			// All the trailers have been found.
			iParserState = EMessageComplete;
			}
		} break;
	case EReadingBodyData:
		{
		// Read the body data...
		TPtrC8 data;
		status = ReadBodyData(data);

		// Only inform the observer if there is any data.
		if( data.Length() > 0 )
			{
			iObserver.BodyChunkL(data);
			}
		// Has all the body data been received - need to check that the parser
		// has not been reset.
		if( status == ESectionDone && iDataState != EReset )
			{
			// Notify the observer that the body has been received
			iObserver.BodyCompleteL();

			// Have received all the body data - message is complete.
			iParserState = EMessageComplete;
			}
		} break;
	case EReadingChunkData:
		{
		// Read the chunk-data...
		TPtrC8 data;
		status = ParseChunkDataL(data);

		// Only inform the observer if there is any data.
		if( data.Length() )
			{
			iObserver.BodyChunkL(data);
			}

		if( status == ESectionDone )
			{
			// Have received all the body data - get the size of the next chunk.
			iParserState = EParsingChunkSize;
			}
		} break;
	case EMessageComplete:
		{
		// Message complete - is there any excess data?
		TPtrC8 data;
		iDataParser.UnparsedData(data);

		// Notify the observer that the message is complete.
		iObserver.MessageCompleteL(data);

		// Move to the Idle state and set the parsing status to Stop.
		iParserState = EIdle;
		status = EStop;
		} break;
	default:
		THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadParserState);
		break;
		}
	// Determine the next action...
	if( iDataState == EReset )
		{
		// The observer has reset the parser during one of the callbacks - it is
		// now safe to do the reset.
		DoReset();
		}
	else if( status == EBufferEmpty || status == EStop )
		{
		// Expect more data before being able to continue parsing.
		iDataState = EWaitingForData;

		// The current message component cannot be parsed due to lack of data,
		// or the parsing has stopped. Need to release the current data packet
		// (and wait for the next one in the case of EBufferEmpty).
		iObserver.ReleaseDataPacket();
		}
	else 
		{
		// Here status is ESectionNotDone or ESectionDone - either the current
		// message component has been completed and the parser needs to move 
		// onto the next component, or the current component still needs to 
		// complete. In both cases, self-complete to continue parsing.
		CompleteSelf();
		}
	}

void CHttpMessageParser::DoCancel()
/**
	Asynchronous request cancel. This function does nothing as the only asynch
	request that is made is a self-complete request.
*/
	{
	// Do nothing...
	}

TInt CHttpMessageParser::RunError(TInt aError)
/**	
	Asynchronous request service error handler. An error has occured whilst
	processing the state machine. Reset the parser and notify the observer of 
	the error.
	@param		aError	The error code.
	@return		An interger value of KErrNone indicates that the error has been
				handled.
	@post		The parser has been reset.
*/
	{
	// Stop parsing and reset.
	DoReset();

	// Notify the observer of the error
	return iObserver.HandleParserError(aError);
	}

void CHttpMessageParser::Flush ()
	{
	// Say a response like this.
	// HTTP/1.0 302 Moves\nStatus: 302 Moved\nPragma: no-cache\nLocation: http://127.0.0.1\n
	// In this response, there is no empty line after the header. Parser cannot parse the last header if 
	// it didn't find an empty line after the header. Force the parser to parse the last header line.
	if ( iDataState == EWaitingForData && iParserState == EParsingHeaders )
		{
		iDataParser.SetData ( KLineFeed() );		
		TRAP_IGNORE( ParseHeadersL () );
		}
	// Message is completed but observer notification hasn't happened
	// Notify now with uparsed data.
	if ( iParserState == EMessageComplete )
		{
		TPtrC8 unparsedData;
		iDataParser.UnparsedData(unparsedData);
		iObserver.MessageCompleteL(unparsedData);
		iParserState = EIdle;			
		}		
	}

	// Completes parsing of the message. Header parts has been parsed.
	// Complete the body part of the message. This happens in case of 3xx response
	// where HTTP FW parses the headers and notify the client/filters. The client/filter
	// will cancel the transaction and resubmit on the new URL.
TBool CHttpMessageParser::CompleteMessage ( const TDesC8& aData )
	{		
	// Set the parser data	
	if ( aData.Length () > 0 )
		iDataParser.SetData ( aData );
	
	// We are reading body data. A content length value is known.
	if ( iParserState == EReadingBodyData )		
 		{
		TPtrC8 data;
		if ( ReadBodyData(data) == ESectionDone )
			{
			iObserver.BodyCompleteL();
			iParserState = EMessageComplete;
 			}				
 		}
	// Read the chunked response.
	if ( iParserState == EReadingChunkData || iParserState == EParsingChunkSize 
		||  iParserState == EParsingTrailerHeaders )
		{
		TParsingStatus status = ESectionNotDone;
		// This tiny look executes till the buffer is empty or the parser state is EMessageComplete
		while ( status != EBufferEmpty && iParserState != EMessageComplete )
			{
			switch ( iParserState )
				{
				case EReadingChunkData:
					{
					TPtrC8 data;
					status = ParseChunkDataL(data);				
					if( status == ESectionDone )
						{				
						iParserState = EParsingChunkSize;
						}													
					}
				break;				
				case EParsingChunkSize:
				status = ParseChunkSizeL();
				if ( status == ESectionDone )
					iParserState = (iDataSizeLeft == 0) ? EParsingTrailerHeaders : EReadingChunkData;
				break;
					
				case EParsingTrailerHeaders:
				status = ParseHeadersL();
				if ( status == ESectionDone )
					iParserState = EMessageComplete;
				break;
				default:
				// Do nothing.
				break;
				}
			}				
 		}
	return iParserState == EMessageComplete;
 	}



