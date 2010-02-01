// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// FTP server answer parser
// Author:	Philippe Gabriel
// RFC 959 defines the syntax for answers sent back by FTP servers
// TFTPServerAnswerParser parses these answers 
// RFC says answers are:
// 1) single line answer
// xxx<space><alphadecimal chars><0x0d><0x0a>
// 2) multiline answer
// xxx-<alphadecimal chars><0x0d><0x0a>
// <several lines>
// <0x0d><0x0a>xxx<space><alphadecimal chars><0x0d><0x0a>
// We parse these regular expressions with the class TFTPServerAnswerParser
// implementing an FSM to parse a regular expression
// The parser is called with the method:
// Parse(const TDesC& aBuffer)
// parameter: aBuffer contains an answer to be parsed
// returns:
// 1) True
// a server answer is available 
// Get the answer with: ServerAnswer(TDes& aServerAnswer)
// Get the number of chars from the buffer corresponding to the
// parsed answer with NChars()
// the NChars first chars can be flushed from the buffer
// note: in this case, call the parser again before
// we do any more socket receive operation, to parse the rest of the buffer
// 2)False
// no answer is available yet
// buffer can be completely flushed
// 
//

/**
 @file ANSPARSE.CPP
 @internalComponent
*/

#include "DEBUG.H"
#include "ANSPARSE.H"

//
// Definitions
//
TFTPServerAnswerParser::TFTPServerAnswerParser()
	{
	iState = EIdle;
	}

TBool	TFTPServerAnswerParser::Parse(const TDesC8& aBuffer)
	{
	const TUint8*	c;
	TInt			bufLen;
	iParsingSuccess = FALSE;
	FTPPROTDEBUG(_DBGAnsparse,_L("TFTPServerAnswerParser::Parse -States: "));
	
	for (iNChars=0,c= aBuffer.Ptr(),bufLen = aBuffer.Length();(!iParsingSuccess && bufLen>0);bufLen--,c++)
		{
		//Log::Printf(_L("<%d>"),iState);
		iNChars++;
		switch (iState)
			{
			case EIdle:
				iDigit.Zero();
				// fall through
			case EState1:
			case EState2:
				// We should have a digit here
				// if not, ignore char and try to be forgiving
				if((*c <'0') || (*c > '9'))
					{
					FTPPROTDEBUG(_DBGAnsparse,_L("TFTPServerAnswerParser::Parser non digit parsed"));
					continue;
					}
				//Ok we have a digit, store it
				iDigit.Append(*c);
				iState++;
				break;
			case EState3:
				switch (*c)
					{
					case '-':
						// We can have a '-' here for a multiline answer
						iState = EState6;
						break;
					case CR: 
						iState = EState5;
						break;
					default:
						// No '-', just wait for Telnet EOL
							iState = EState4;
						break;
					}
				break;
			// Wait for Telnet EOL
			case EState4:
				// note: this state cannot be collapsed with state3
				// we need this state for the transition from state11
				// Wait for CR
				if(*c ==CR)
					iState = EState5;
				break;
			case EState5:
				// Wait for LF
				if(*c ==LF)
					{
					iState = EIdle;
					iParsingSuccess = TRUE;
					// Update iBufferPos
					
					}
				else
					iState = EState4;
				break;
			// Parsing multiline answer
			case EState6:
				// Wait for CR
				if(*c ==CR)
					iState = EState7;
				break;
			case EState7:
				// Wait for LF
				if(*c ==LF)
					iState = EState8;
				else
					iState = EState6;
				break;
			// Parsing final line of a multiline answer
			case EState8:
			case EState9:
			case EState10:
				// Wait for a digit
				if(*c ==iDigit[iState-EState8])
					iState++;
				else
					if (*c == CR)
						iState = EState7;
					else
						iState = EState6;
				break;
			case EState11:
				// Finally we want a space
				if(*c ==' ')
					iState = EState4;
				else
					iState = EState6;
				break;
			default:
				__ASSERT_DEBUG(FALSE, User::Panic(_L("TFTPServerAnswerParser"),EPIPanicOutOfState));
			}
		}
	FTPPROTDEBUG(_DBGAnsparse,_L("\n"));
	return iParsingSuccess;
	}
