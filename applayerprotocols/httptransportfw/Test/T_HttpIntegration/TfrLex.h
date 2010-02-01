// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// $Header$
// TfrLex.cpp
// This module implements TfrLex collection of static lexical functions.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// 
//

#ifndef __TFRLEX_H__
#define __TFRLEX_H__

//-----------------------------------------------------------------------------

#include <e32base.h>

//-----------------------------------------------------------------------------

class TfrLex
{
public:     // New functions

	// Extract a term. The TLex variants return KErr* (KErrNone if
	// found) and advance next position. The TDesC& variants check
	// if text begins with term and return ETrue/EFalse.
	static TInt  Val ( TLex& aLex        , const TDesC& aTerm );
	static TBool Val ( const TDesC& aText, const TDesC& aTerm );
	static TInt  ValF( TLex& aLex        , const TDesC& aTerm );
	static TBool ValF( const TDesC& aText, const TDesC& aTerm );

	// Skip spaces and get token up to end-of-string, a space char
	// or up to the very character/characters you define. The GetF
	// supports tokens like "*". If a token begins with apostrophe
	// but does not end to one, the functions L E A V E with error
	// KErrArgument.
	static TPtrC GetL( TLex& aLex, const TChar  aChar = 0 );
	static TPtrC GetL( TLex& aLex, const TDesC& aList     );

	// Skip spaces and then eat the very character or the very term
	// you define. Return ETrue (was found and swallowed) or EFalse.
	static TBool Eat ( TLex& aLex, const TChar aChar  );
	static TBool Eat ( TLex& aLex, const TDesC& aTerm );
	static TBool EatF( TLex& aLex, const TDesC& aTerm );

	// Get count of tokens. REMOVED
	// static TInt TokenCount( const TDesC& aText );
	// static TInt TokenCount( TLex aLex );

	// Get from first non white space onwards.
	static TPtrC TrimLeft( const TDesC& aText );

	// Get up to the last non white space.
	static TPtrC TrimRight( const TDesC& aText );

	// Get from first non white space onwards up to the last
	// non white space i.e. TrimLeft and TrimRight.
	static TPtrC Trim( const TDesC& aText );

	// Get text like "*" without the two apostrophes. If not
	// like "*", then returns the text as such as the whole.
	static TPtrC Peel( const TDesC& aText );

	// Trim and then Peel.
	static TPtrC TrimAndPeel( const TDesC& aText );

	// Peel and them Trim.
	static TPtrC PeelAndTrim( const TDesC& aText );

};

//-----------------------------------------------------------------------------

#endif  // __TFRLEX_H__

//-----------------------------------------------------------------------------
//	Get count of tokens.

/*
TInt TfrLex::TokenCount( const TDesC& aText )
{
TLex lex( aText );
return TokenCount( lex );
}
TInt TfrLex::TokenCount( TLex aLex )
{
TPtrC token;
TInt count = 0;
do
{
token.Set( aLex.NextToken() );
if ( token.Length() > 0 ) count++;
} while ( token.Length() > 0 );

return count;
}        
*/

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
