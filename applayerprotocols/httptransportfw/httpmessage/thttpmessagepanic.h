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

#ifndef __THTTPMESSAGEPANIC_H__
#define __THTTPMESSAGEPANIC_H__

#include <e32std.h>

class THttpMessagePanic
/**	
The THttpMessagePanic class provides the panic function and codes for the 
http message component.
*/
	{
public:	// enums

	enum THttpMessagePanicCode
/**	
	The THttpMessagePanicCode enumeration defines the panic codes used by the
	http message component.
*/
		{
		/** The data state in either the message composer or parser was not 
			correct. An API was used incorrectly.
		*/
		EHttpMessagePanicBadDataState			= 0,

		/**	The message composer or parser was reset twice in the same observer
			callback function.
		*/
		EHttpMessagePanicDoubleReset			= 1,

		/** The message parser received an unexpected data parsing result from 
			the data parser.
		*/
		EHttpMessagePanicBadDataParserResult	= 2,

		/** The message parser entered an illegal state.
		*/
		EHttpMessagePanicBadParserState			= 3,

		/** The data parser was in an illegal state.
		*/
		EHttpMessagePanicBadDataParserState		= 4,

		/** The message composer entered an illegal state.
		*/
		EHttpMessagePanicBadComposerState		= 5,

		/** An illegal delimiter has been specified to the data composer.
		*/
		EHttpMessagePanicBadDelimiterType		= 6,

		/** The message parser obserser has specified an unknown body size type.
		*/
		EHttpMessagePanicBadBodySize			= 7
		};

public:	// methods

	static void Panic(THttpMessagePanicCode aPanic);

	};

#endif	// __THTTPMESSAGEPANIC_H__
