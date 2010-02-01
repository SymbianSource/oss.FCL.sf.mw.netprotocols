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

#ifndef __THTTPDATAPARSER_H__
#define __THTTPDATAPARSER_H__

#include <e32std.h>

class MHttpBufferSupplier;

class THttpDataParser 
/**
	The THttpDataParser provides the functionality to parse a data buffer for
	lines, as specified in RFC2616. This states in section 2.2 that the end of
	line marker for all protocol elements (except the entity body) as the 
	sequence CR LF.

	Parsed lines are copied into a buffer supplied by an MHttpBufferSupplier
	object. If there is not enough space for the line date, the data parser
	asks the buffer supplier to resize the buffer to at least the required size.

	When parsing a header field line values can be folded onto multiple lines
	if the continuation line begins with one or more SPs or HTs. This is known
	as LWS and can be replaced by a single SP without changing any of the 
	semantics.

	Also, some tolerence has been built-in for robustness. The data parser can
	parse for lines were the end of line marker is just a LF - the leading CR
	has been omitted. This tolerance is suggested in section 19.4 of RFC2616.
	There is also tolerence to spurios CRs in the data.
	@internalComponent
	@see		MHttpBufferSupplier
*/
	{
public:	// enums

	enum TParseResult
/**	
	The TParseResult enumeration defines the possible result of a specified data
	parsing request.
*/
		{
		/** The requested parse for a line was successful.
		*/
		ELineParsed			=0,

		/** The requested parse was not successful due to a lack of data.
		*/
		EPartialData,

		/** The requested parse for an empty line was successful.
		*/
		EEmptyLine,

		/** The requested parse for the specified data chunk was successful.
		*/
		EGotData
		};

public:	// methods

	THttpDataParser(MHttpBufferSupplier& aBufferSupplier);

	TParseResult GetLineL(TPtrC8& aLine);
	TParseResult GetHeaderLineL(TPtrC8& aLine);
	TParseResult GetData(TPtrC8& aData, TInt aMaxSize);

	void SetData(const TDesC8& aData);
	void UnparsedData(TPtrC8& aData);

	void Reset();

private:	// enums

	enum TDataParserState
/**	
	The TDataParserState enumeration defines the state machine for the data 
	parser when performing a requested line parse. This can be either a standard
	line or a header value line.
*/
		{
		/**	The data parser is idle. It ready to parse for another line or for
			a data chunk.
		*/
		EIdle				= 0,

		/**	The data parser has not found the start of a CRLF. More data is
			required.
		*/
		EPendingMoreData,

		/** The data parser has found a CR. The LF is the next expected character.
		*/
		EPendingLF,

		/**	The data parser has found a CR at the start of its data buffer. This
			indicates that an empty line has been found. The LF is the next
			expected character.
		*/
		EPendingEmptyLine,

		/**	The LF of non-empty line has been found. The next expected character
			should not be a SP or a HT which would otherwise indicate LWS 
			when searching for a header line. In the case of a standard line any
			character is expected.
		*/
		EPendingFoundLine
		};


	enum TLineType
/**	
	The TLineType enumeration defines the type of line that the data parser has
	been requested to parse. There are two types - a standard line and a header
	line. The difference is that a header line is delimited by a CRLF that is not
	followed by one or more SPs or HTs.
*/
		{
		/**	This specifies a standard line - delimited by a CRLF.
		*/
		EStandardLine		= 0,

		/**	This specifies a header line - delimited by a CRLF that is not
			followed by one or more SPs or HTs.
		*/
		EHeaderLine
		};

private:	// methods

	void AppendToBufferL(const TDesC8& aData);
	TParseResult GetLineL(TPtrC8& aLine, TLineType aLineType);
	TInt FindEOLMarker(TChar& aEOLMarker);

private:	// attributes

	MHttpBufferSupplier&	iBufferSupplier;
	TDataParserState		iState;
	TPtrC8					iData;
	TPtr8					iLineBuffer;

	};

#endif	// __THTTPDATAPARSER_H__
