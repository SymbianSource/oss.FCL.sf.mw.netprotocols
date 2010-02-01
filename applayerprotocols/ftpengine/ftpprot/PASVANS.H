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
* PASV command answer parser
* Author:	Philippe Gabriel
* RFC 959 defines the syntax for answer to the PASV command
* We must parse an IP+Port number (decimal, big endian)
* (ip1,ip2,ip3,ip4,ip5,port1,port2)
* We parse these regular expressions using an FSM
* Use:
* TFtpPASVAnswerParser::Reset 
* to Reset the parser before we begin to parse a new answer
* TFtpPASVAnswerParser::Parse 
* to parse an answer
* returns:
* TRUE: we parsed an answer (answer might be correct or inccorect)
* FALSE: An answer has not been completely parsed yet
* TFtpPASVAnswerParser::Fetch 
* Fetch an answer
* returns:
* TRUE: We got a valid answer
* FALSE: The answer we got had an invalid syntax
* 
*
*/



/**
 @file PASVANS.H
 @internalComponent
*/

#if !defined(__PASVANS_H__)
#define __PASVANS_H__
#include <e32base.h>
#include <in_sock.h>

//////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////

class TFtpPASVAnswerParser 
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
		EBeginParse=1,	
		EParsing=2,
		ESuccess,EFailed
		};

			TFtpPASVAnswerParser(void){iState=EIdle;}
	TBool	Parse(const TDesC8&, const TInetAddr& aAddr);
	TBool	Fetch(TInetAddr& aFTPServerAddress);
	void	Reset(void);
private:
	/** Helper to Parse() */
	TBool ParsePASV(const TDesC8&); 
	/** Helper to Parse() */
	TBool ParseEPSV(const TDesC8&); 
	/** The current state of the parser */
	TUint		iState;	
	/** The parsed answer digits */
	TBuf<12>	iDigit; 
	TInetAddr   iFTPServerAddress;
	TInt		iNumberCounter;
};

#endif // __PASVANS_H__
