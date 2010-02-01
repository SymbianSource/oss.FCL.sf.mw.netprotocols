// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>
#include "StringParser.h"


CStringParser::CStringParser ( const TDesC8& aBuffer )
: iBuffer ( aBuffer ),
iCurrentPos ( 0 )
	{
	}

CStringParser::~CStringParser ()
	{
	}

CStringParser* CStringParser::NewLC ( const TDesC8& aBuffer )
	{
	CStringParser* self = new ( ELeave ) CStringParser ( aBuffer );
	CleanupStack::PushL ( self );
	return self;
	}

CStringParser* CStringParser::NewL ( const TDesC8& aBuffer )
	{
	CStringParser* self = CStringParser::NewLC ( aBuffer );
	CleanupStack::Pop ( self );

	return self;
	}

/**
	Get the next character from the current position, and increments the 
	current position by 1. 
	
	@param aChar [out] Next character to be read
	
	@return TBool ETrue if the current position is less than the string length 
	else returns EFalse
 */
TBool CStringParser::GetNextCharacter ( TChar& aChar )
	{
	IncrementCurrentPos ();
	return ( GetCurrentCharacter ( aChar ) );	
	}

/**
	Get the character from the current position. 	
	
	@param aChar [out] Current character to be read
	
	@return TBool ETrue if the current position is less than the string length 
	else returns EFalse
 */
TBool CStringParser::GetCurrentCharacter ( TChar& aChar )
	{
	if ( iCurrentPos >= iBuffer.Length () )
		{
		return EFalse;		
		}
	
	aChar = iBuffer[ iCurrentPos ];
	return ETrue;
	}

/**
	Parses the string till it finds the aEndChar.
	
	@param TChar [in] Character to search for.
	@see GetNextWorrd
 */	
TBool CStringParser::ParseTill ( TPtrC8& aWord, TChar aEndChar )
	{	
	TPtrC8 cur( iBuffer.Mid( iCurrentPos ) );
	TBool found = ETrue;
	TInt pos = cur.Locate( aEndChar );
	if ( pos < 0 )
		{
		// Unable to locate the aEndChar. Sets the end position to
		// the length of the string.
		pos = iBuffer.Length (); 
		found = EFalse;
		}

	aWord.Set ( cur.Left (pos) ); 
	SkipLength ( pos );
	return found;	
	}	

/**
	Increments the current parsing position by aLen.
	
	@param TInt [in] Number of character to skip.
	
	@return TBool ETrue if the current parsing position is less than the buffer length
    else returns EFalse.
 */
TBool CStringParser::SkipLength ( TInt aLen )
	{
	iCurrentPos += aLen;

	if ( iCurrentPos < iBuffer.Length () )
		{
		return ETrue;		
		}
	return EFalse;
	}

/**
	Finds the remainder string from the current parsing position to the buffer length.
		
	@param TPtrC8& [out] Points to the current starting position till end
	
	@return TBool ETrue if the current parsing position is less than the buffer length
    else returns EFalse.
 */
void CStringParser::GetRemainder ( TPtrC8& aBuffer )
	{
	aBuffer.Set ( iBuffer.Mid ( iCurrentPos ) ); 
	SkipLength ( aBuffer.Length () );
	}

/**
 	Increments the current parsing position by 1.
  */
void CStringParser::IncrementCurrentPos ( )
	{
	++iCurrentPos;
	}

/**
	Parses till the first occurrence of any one of a set of characters in aCharSet,
	from the current parsing position.		
	
	@param TPtrC8& [out] Points to the current starting position.
	@param const TDesC8& [in] Descriptor containing characters for matching.
 */
void CStringParser::ParseTill ( TPtrC8& aWord, const TDesC8& aCharSet )
	{
	TPtrC8 cur( iBuffer.Mid( iCurrentPos ) );
	
	TInt lowpos = cur.Length ();
	TInt pos = KErrNotFound;
	
	for ( TInt i = 0; i < aCharSet.Length(); ++i )
		{
		pos = cur.Locate ( aCharSet [ i ] );
		
		if ( (pos != KErrNotFound) && (pos < lowpos ) )
			{
			lowpos = pos;				
			}
		}
		
	aWord.Set ( cur.Left ( lowpos ) );
	SkipLength ( lowpos );	
	}





