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
// PASV command answer parser
// Author:	Philippe Gabriel
// RFC 959 defines the syntax for answer to the PASV command
// We must parse an IP+Port number (decimal, big endian)
// (ip1,ip2,ip3,ip4,ip5,port1,port2)
// We parse these regular expressions using an FSM
// Use:
// TFtpPASVAnswerParser::Reset 
// to Reset the parser before we begin to parse a new answer
// TFtpPASVAnswerParser::Parse 
// to parse an answer
// returns:
// TRUE: we parsed an answer (answer might be correct or inccorect)
// FALSE: An answer has not been completely parsed yet
// TFtpPASVAnswerParser::Fetch 
// Fetch an answer
// returns:
// TRUE: We got a valid answer
// FALSE: The answer we got had an invalid syntax
// 
//

/**
 @file PASVANS.CPP
 @internalComponent
*/

#include "DEBUG.H"
#include "PASVANS.H"
#include <e32base.h>
#include <e32test.h>
#include <in_sock.h>

//
// Definitions
//

TBool TFtpPASVAnswerParser::Fetch(TInetAddr& aFTPServerAddress)
	{
	__ASSERT_DEBUG((iState == ESuccess) || (iState == EFailed), User::Panic(_L("TFtpPASVAnswerParser::Fetch incorrect state"), 0));
	if(iState != ESuccess)
		return FALSE;
	aFTPServerAddress = iFTPServerAddress;
	return TRUE;
	}

void	TFtpPASVAnswerParser::Reset(void)
	{
	iState = EIdle;	
	iFTPServerAddress.SetFamily(KAFUnspec);
	}

TBool	TFtpPASVAnswerParser::Parse(const TDesC8& aBuffer, const TInetAddr& aAddr)
	{
	if (aAddr.Family() == KAfInet)
		return ParsePASV(aBuffer);
	else
		{
		__ASSERT_DEBUG(aAddr.Family() == KAfInet6, User::Panic(_L("TFtpPASVAnswerParser"), 0));
		iFTPServerAddress.SetAddress(aAddr.Ip6Address());
		return ParseEPSV(aBuffer);
		}
	}

TBool TFtpPASVAnswerParser::ParsePASV(const TDesC8& aBuffer)
	{
	const TUint8*	c;
	TInt			bufLen;
	TUint		aIPAndPort[6];
	
	if ((iState == ESuccess) || (iState == EFailed))
		// Already parsed an answer
		return TRUE;
	for (c= aBuffer.Ptr(),bufLen = aBuffer.Length();(bufLen>0);bufLen--,c++)
		{
		switch (iState)
			{
			case EIdle:
				if (*c=='(')
					{
					/* We fetched a (, begin parsing*/
					iNumberCounter=0;
					iState++;
					}
				break;
			case EBeginParse:
				if ((*c>='0') && (*c<='9'))
					{
					iDigit.Zero();
					iDigit.Append(*c);
					iState++;
					}
				break;
			case EParsing:
				if ((*c>='0') && (*c<='9'))
					if(iDigit.Length() == 3)
						{
						// We're stuffed, we got more than 3 digits
						iState = EFailed;
						return TRUE;
						}
					else
						{
						iDigit.Append(*c);
						}
				else // We should have a , or a ) I accept everything
					{
					
					//[1] Translate the last number that we parsed
					TUint	tempValue;
					TLex input(iDigit);
					input.Val(tempValue,EDecimal);
					if(tempValue>255)
						{
						// If Value out of bound, we're stuffed
						// Error bail out
						iState = EFailed;
						return TRUE;
						}
					aIPAndPort[iNumberCounter++] = tempValue;
					// [2] Are we waiting for more numbers?
					if(iNumberCounter == 6)
						{
						// OKay We've finished
						iFTPServerAddress.SetAddress(INET_ADDR(aIPAndPort[0],aIPAndPort[1],aIPAndPort[2],aIPAndPort[3]));
						iFTPServerAddress.SetPort(aIPAndPort[4]<<8 | aIPAndPort[5]);
						iState = ESuccess;
						return TRUE;
						}
					else
						// Fetch next number
						iState = EBeginParse;
					}
				break;
			default:
				break;
			}
		}
	return FALSE;
	}

TBool TFtpPASVAnswerParser::ParseEPSV(const TDesC8& aBuffer)
	{
	TInt			bufLen = aBuffer.Length();
	TUint           port = 0;

	if ((iState == ESuccess) || (iState == EFailed))
		// Already parsed an answer
		return ETrue;

	for (const TUint8* c = aBuffer.Ptr();bufLen>0;bufLen--,c++)
		{
		switch (iState)
			{
			case EIdle:
				if (*c=='(')
					{
					/* We fetched a (, begin parsing*/
					iState = EBeginParse;
					}
				break;
			case EBeginParse:
				if (TChar(*c).IsDigit())
					{
					port = *c - '0';
					iState = EParsing;
					}
				break;
			case EParsing:
				if (TChar(*c).IsDigit())
					{
					port = port*10 + (*c - '0');
					}
				else // We should have a "|", end the parsing
					{
					if(port>(1<<16) || *c != '|')
						{
						// If Value out of bound or  the last character is not a '|' 
						// then the parsing failed
						iState = EFailed;
						return ETrue;
						}
					
					iFTPServerAddress.SetPort(port);
					iState = ESuccess;
					return ETrue;
					}
				break;
			default:
				break;
			}
		}
	return EFalse;
	}
