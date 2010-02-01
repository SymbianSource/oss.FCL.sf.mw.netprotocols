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

#include "thttpdataparser.h"

#include "mhttpbuffersupplier.h"
#include "thttpmessagepanic.h"

const TUint KCarriageReturn	= '\r';
const TUint KLineFeed		= '\n';
const TUint KSpace			= ' ';
const TUint KTab			= '\t';

THttpDataParser::THttpDataParser(MHttpBufferSupplier& aBufferSupplier)
: iBufferSupplier(aBufferSupplier), iLineBuffer(TPtr8(NULL, 0))
/**
	Constructor.
	@param		aBufferSupplier	The buffer supplier.
*/
	{
	}

void THttpDataParser::Reset()
/**
	Resets the data parser. The line buffer and current data packet buffer are
	reset to a zero length buffer and the parser moves to the Idle state. The 
	observer is notified that the buffer supplying the line buffer should be 
	deleted.
*/
	{
	// Move to the Idle state and reset the line buffer and data chunk.
	iState = EIdle;
	iLineBuffer.Set(NULL, 0, 0);
	iData.Set(NULL, 0);
	iBufferSupplier.DeleteBuffer();
	}

THttpDataParser::TParseResult THttpDataParser::GetHeaderLineL(TPtrC8& aLine)
/**
	Extracts a header field line. The data parser attempts to parse a line of
	data where the end of line marker is defined by the CRLF sequence. In 
	HTTP/1.1 header field values can be folded onto multiple lines if the 
	continuation line begins with a SP or HT. The CRLF sequence in this case is
	part of LWS and is ignored - the CRLF will not be preset in the returned 
	line buffer. The delimiting CRLF is not given in the return data. 
	
	If a line cannot be extracted (due to lack of data) then the value 
	EPartialData is returned. In this case the output argument is not valid. If 
	an empty line is found then the value EEmptyLine is returned. If the line is
	extracted then the value ELineParsed is returned.
	@param		aLine	The output argument set to the buffer containing the 
						extracted header field line. This will only be valid if
						the return value is either ELineParsed or EEmptyLine.
	@return		A value of EPartialData if the requested line type cannot be 
				found in the current data packet. If an empty line is found then
				EEmptyLine is returned otherwise if the line is found then
				ELineParsed is returned.
*/
	{
	return GetLineL(aLine, EHeaderLine);
	}

THttpDataParser::TParseResult THttpDataParser::GetLineL(TPtrC8& aLine)
/**
	Extracts a line. The data parser attempts to parse a line of data where the
	end of line marker is defined by the CRLF sequence. The delimiting CRLF is
	not given in the return data. 
	
	If a line cannot be extracted (due to lack of data) then the value 
	EPartialData is returned. In this case the output argument is not valid. If 
	an empty line is found then the value EEmptyLine is returned. If the line is
	extracted then the value ELineParsed is returned.
	@param		aLine	The output argument set to the buffer containing the 
						extracted header field line. This will only be valid if
						the return value is either ELineParsed or EEmptyLine.
	@return		A value of EPartialData if the requested line type cannot be 
				found in the current data packet. If an empty line is found then
				EEmptyLine is returned otherwise if the line is found then
				ELineParsed is returned.
*/
	{
	return GetLineL(aLine, EStandardLine);
	}

THttpDataParser::TParseResult THttpDataParser::GetLineL(TPtrC8& aLine, TLineType aLineType)
/**
	Parses data for the specified line type. In the HTTP/1.1 protocol there are
	two types of line. The standard line is delimited by the CRLF sequence. The
	header field line is delimited by a CRLF sequence that is not followed by a 
	SP or HT. When parsing for a header field line any CRLF sequences followed 
	by a SP or HT are classed as part of LWS and ignored - header field values 
	can be folded onto multiple line using this LWS in this way. The parser is
	robust to spurious CRs in the data and eol markers that are missing the
	leading CR.

	Initially the data parser is in the Idle state when starting to parse for a
	line. In the Idle state it clears the line buffer and moves to the 
	PendingMoreData state.

	In the PendingMoreData state the data parser looks for an end of line marker.
	The eol marker should can be either a CRLF sequence or just a LF in cases 
	where there has been a deviation from the protocol - this deviation is 
	tolerated for robustness.
	 
	If an eol marker is not found one in the current data packet, the parser 
	appends the data packet to the line buffer and returns EPartialData. The 
	current data packet is then discarded. 
	
	If an eol maker is found at the start of the data packet this indicates a
	possible empty line. The data parser moves to the PendingEmptyLine state if
	the eol marker was a CR. If the eol marker was a LF then an empty line has
	been found - the parser moves to the Idle state and returns the EEmptyLine 
	value. The current data packet is updated to skip past the eol marker. 
	
	If an eol marker is found but not at the start of the data packet this 
	indicates a possible line. The data parser moves to the PendingLF state if 
	the eol marker was a CR. If the eol marker was a LF then the parser moves to
	the PendingFoundLine state. The data upto, but not including the eol marker,
	is appended to the line buffer and the current data packet is updated to 
	skip past the eol marker.
	
	In the PendingLF state the data parser expects a LF to be present at the 
	start of the data packet. If the LF is found then the parser moves to the 
	PendingFoundLine state and the data packet is updated to skip past the LF. 
	If a CR is found at the start of the data packet then it is ignored and the
	parser remains in the PendingLF state. If the any other character is found
	then the initial CR is deemed to be spurious and the parser moves back to 
	the PendingMoreData state.

	In the PendingEmptyLine state the data parser expects a LF to be present at
	the start of the data packet. If the LF is found then the parser moves to 
	the Idle state and the data packet is updated to skip past the LF. The value
	EEmptyLine is returned.	If a CR is found at the start of the data packet 
	then it is ignored and the parser remains in the PendingLF state. If the any
	other character is found then the initial CR is deemed to be spurious and 
	the parser moves back to the PendingMoreData state.

	In the PendingFoundLine state the data parser checks the first character in 
	the data packet. If it is a SP or HT and the a header field line was 
	requested then the parser moves to the PendingMoreData state. Otherwise a 
	line has been found. The line buffer will contain the line less any CRLF 
	sequences either as the end of line marker or as part of LWS. The output 
	argument aLine is set to the line buffer. The value ELineParsed is returned
	and the parser moves to the Idle state.

	The current data packet is checked before processing each state. If there is
	no data then the parser does not continue as it requires more data. The 
	value EPartialData is returned.
	@param		aLine		The output argument that is set to the parsed line 
							when the value ELineParsed is returned.
	@param		aLineType	The type of line requested to be parsed.
	@return		A value of EPartial data if a the requested line type cannot be 
				found in the current data packet. If an empty line is found then
				EEmptyLine is returned otherwise if a line is found then
				ELineParsed is returned.
	@panic		EHttpMessagePanicBadDataParserState	The parser is in an invalid
													state.
*/
	{
	// Parse the current data for a 'line'
	TBool done	= EFalse;
	TParseResult result = EPartialData;
	while( !done && iData.Length() > 0 )
		{
		// The data buffer always has data - no need to check...
		switch( iState )
			{
		case EIdle:
			{	
			// Not waiting for more data - clear the buffer.
			iLineBuffer.Zero();

			// Drop through to PendingMoreData case...
			}
		case EPendingMoreData:
			{
			// Locate end of line marker - need to be tolerant of protocol 
			// deviations. Therefore support eol termination by just a LF.
			TChar eol = 0;
			TInt posEOL = FindEOLMarker(eol);

			if( posEOL != KErrNotFound )
				{
				if( posEOL == 0 && iState == EIdle )
					{
					// Data was at start of a new line and the eol marker was 
					// found at the start - possible empty line. Check what the
					// eol marker was
					if( eol == KCarriageReturn )
						{
						// Move to the PendingEmptyLine state.
						iState = EPendingEmptyLine;
						}
					else
						{
						// An empty line has been found - can stop parsing and
						// move to the Idle state.
						done = ETrue;
						result = EEmptyLine;
						iState = EIdle;
						}
					}
				else
					{
					// Append the data found upto the eol marker.
					AppendToBufferL(iData.Left(posEOL));

					// Check what the eol marker was.
					if( eol == KCarriageReturn )
						{
						// Expecting a LF - move to the PendingLF state.
						iState = EPendingLF;
						}
					else
						{
						// The eol marker was a LF - move to the PendingFoundLine state.
						iState = EPendingFoundLine;
						}
					}
				// Skip past the eol marker...
				iData.Set(iData.Mid(posEOL + 1));
				}
			else
				{
				// Not enough data - append the data to the buffer.
				AppendToBufferL(iData);

				// Move to the PendingMoreData state and wait for next data 
				// packet				
				done = ETrue;
				iState = EPendingMoreData;
				}
			} break;
		case EPendingLF:
			{
			// Expecting a LF at the start of the data
			if( iData[0] == KLineFeed )
				{
				// Skip past the LF and move to PendingFoundLine
				iData.Set(iData.Mid(1));
				iState = EPendingFoundLine;
				}
			else if( iData[0] == KCarriageReturn )
				{
				// Need to be robust to spurious amounts of CRs - skip past the 
				// CR and remain in PendingLF state.
				iData.Set(iData.Mid(1));
				}
			else
				{
				// Data is wrong - had a spurious CR in the data. Need to be so
				// return to the PendingMoreData state.
				iState = EPendingMoreData;
				}
			} break;
		case EPendingEmptyLine:
			{
			// Expecting a LF at the start of the data
			if( iData[0] == KLineFeed )
				{
				// This CRLF delimits an empty line - no need to check that it 
				// is part of LWS. 
				result = EEmptyLine;
				done = ETrue;

				// Move to the Idle state and skip past the LF.
				iData.Set(iData.Mid(1));
				iState = EIdle;
				}
			else if( iData[0] == KCarriageReturn )
				{
				// Need to be robust to spurious amounts of CRs - skip past the 
				// CR and remain in PendingEmptyLine state.
				iData.Set(iData.Mid(1));
				}
			else
				{
				// Data is wrong - had a spurious CR in the data. Need to be so
				// return to the PendingMoreData state.
				iState = EPendingMoreData;
				}
			} break;
		case EPendingFoundLine:
			{
			// Expecting a leading char tha is not a SP or a HT
			TChar ch = iData[0];
			if( aLineType == EHeaderLine && (ch == KSpace || ch == KTab) )
				{
				// This CRLF is part of LWS between tokens in a header field 
				// line - move to PendingMoreData state and search for the next CR.
				iState = EPendingMoreData;
				}
			else if( aLineType == EHeaderLine && (ch == KCarriageReturn) )
				{
				// Need to be robust to spurious amounts of CRs - skip past the 
				// CR and remain in EPendingFoundLine state.
				iData.Set(iData.Mid(1));
				}
			else
				{
				// This CRLF delimits the end of a line - move to Idle state
				done = ETrue;
				result = ELineParsed;
				iState = EIdle;
				}
			} break;
		default:
			THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataParserState);
			break;
			}
		}
	// Set the output argument aLine
	if( result == ELineParsed )
		{
		// Line was found - set buffer to the line buffer
		aLine.Set(iLineBuffer);
		}
	else
		{
		// Either an empty line was found or waiting for more data - set buffer
		// to empty buffer.
		aLine.Set(KNullDesC8());
		}
	return result;
	}

THttpDataParser::TParseResult THttpDataParser::GetData(TPtrC8& aData, TInt aMaxSize)
/**
	Attempts to supply the data requested. The argument aMaxSize specifies the 
	amount of data required. If the amount is available the data returns a value
	of EGotData, otherwise EPartialData is returned.
	@param		aData		The output argument that is set to the buffer with 
							the requested data.
	@param		aMaxSize	The amount of data required.
	@return		A value of EGotData if all the requested data was supplied or 
				EPartialData if was not.
	@panic		EHttpMessagePanicBadDataParserState The parser was not in the 
													Idle state.
*/
	{
	__ASSERT_DEBUG( iState == EIdle, THttpMessagePanic::Panic(THttpMessagePanic::EHttpMessagePanicBadDataParserState) );

	// Set the length of the data to copy - intially set to available amount
	TInt copyLength = iData.Length();
	TParseResult result = EPartialData;

	// Make sure that not too much data is given...
	if( copyLength >= aMaxSize )
		{
		// The available data is same or more than the requested amount - adjust
		// the copy size to copy just the requested amount.
		copyLength = aMaxSize;
		result = EGotData;
		}
	// Copy over the data specified.
	aData.Set(iData.Left(copyLength));

	// Update the data buffer...
	iData.Set(iData.Mid(copyLength));

	return result;
	}

void THttpDataParser::SetData(const TDesC8& aData)
/**
	Sets the current data packet.
	@param		aData	The buffer containing the current data packet.
*/
	{
	iData.Set(aData);
	}

void THttpDataParser::UnparsedData(TPtrC8& aData)
/**
	Accessor to the unparsed data in the current data packet. The data parser 
	can supply the unparsed data that is in the current data packet.
	@param		aData	The output argument that is set to the unparsed data.
*/
	{
	aData.Set(iData);
	}

void THttpDataParser::AppendToBufferL(const TDesC8& aData)
/**
	Appends data to line buffer. If the line buffer is not big enough to hold 
	the extra data the parser gets the observer to reallocate a larger buffer.
	@param		aData	The data to be appended to the line buffer.
*/
	{
	// Check to see if there is enough space
	TInt requiredLength = iLineBuffer.Length() + aData.Length();
	if( requiredLength > iLineBuffer.MaxLength() )
		{
		// Need more space...
		iBufferSupplier.ReAllocBufferL(requiredLength, iLineBuffer);
		}
	// Append the data.
	iLineBuffer.Append(aData);
	}

TInt THttpDataParser::FindEOLMarker(TChar& aEOLMarker)
/**
	Locates end of line marker in current data packet. Need to be tolerant of a 
	LF without a leading CR being used as a end of line marker. This function
	searches for either a CR of a LF as the eol marker and returns its position
	if it is found and the what the marker was. If neither a CR of a LF was 
	found then KErrNotFound is returned.
	@param		aEOLMarker	An output argument that is set to the eol marker
							that was found. Is not valid if the return value is
							KErrNotFound.
	@return		A value of KErrNotFound is returned if an eol marker was not 
				found in the current data packet. If an eol marker was found 
				then its position in the data packet is returned.
*/
	{
	const TUint len = iData.Length();
	TUint pos = 0;
	TInt ret = KErrNotFound;
	for(;pos < len;++pos)
		{
		const TUint8& ch = iData[pos];
		if(ch == KLineFeed || ch == KCarriageReturn)
			{
			aEOLMarker = ch;
			ret = pos;
			break;
			}
		}
	return ret;
	}


