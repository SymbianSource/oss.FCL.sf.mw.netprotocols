/**
* Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* FTP server answer parser
* Author:	Philippe Gabriel
* RFC 959 defines the syntax for answers sent back by FTP servers
* TFTPServerAnswerParser parses these answers 
* RFC says answers are:
* 1) single line answer
* xxx<space><alphadecimal chars><0x0d><0x0a>
* 2) multiline answer
* xxx-<alphadecimal chars><0x0d><0x0a>
* <several lines>
* <0x0d><0x0a>xxx<space><alphadecimal chars><0x0d><0x0a>
* We parse these regular expressions with the class TFTPServerAnswerParser
* implementing an FSM to parse a regular expression
* The parser is called with the method:
* Parse(const TDesC& aBuffer)
* parameter: aBuffer contains an answer to be parsed
* returns:
* 1) True
* a server answer is available 
* Get the answer with: ServerAnswer(TDes& aServerAnswer)
* Get the number of chars from the buffer corresponding to the
* parsed answer with NChars()
* the NChars first chars can be flushed from the buffer
* note: in this case, call the parser again before
* we do any more socket receive operation, to parse the rest of the buffer
* 2)False
* no answer is available yet
* buffer can be completely flushed
* 
*
*/



/**
 @file ANSPARSE.H
 @internalComponent
*/

#if !defined(__ANSPARSE_H__)
#define __ANSPARSE_H__
#include "FTPDEF.H"
#include "DEBUG.H"
#include <e32base.h>

//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////

class TFTPServerAnswerParser
/**
@internalComponent
*/
{

public:
	enum TState
		{
		/** Do some clever encoding here
		*/
		EIdle=0,	
		/** to simplify parsing code
		*/
		EState1=1,	
		EState2=2,
		EState3=3,
		EState4,EState5,EState6,EState7,
		EState8,EState9,EState10,EState11
		};
	enum TPanic 
		{
		EPIPanicOutOfState = -1
		};

	TFTPServerAnswerParser(void);
	TBool	Parse(const TDesC8&);
	TInt	NChars(void) {return iNChars;}
	void	ServerAnswer(TDes8& aServerAnswer)	{aServerAnswer.Copy(iDigit);}
private:
	/** The current state of the parser */
	TUint	iState;	
	/** The answer parsed */
	TInt	iServerReply;	
	/** The number of chars that can be fetched from the buffer */
	TInt	iNChars; 
	/** The parsed answer digits */
	TBuf<3>	iDigit; 
	/** We've completed parsing answer */
	TBool	iParsingSuccess; 
};
#endif // __ANSPARSE_H__
