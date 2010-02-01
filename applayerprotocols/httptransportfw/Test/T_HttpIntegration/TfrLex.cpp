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
// This module implements TfrLex collection of static lexical functions.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// 
//

#include <e32std.h>                     //
#include <e32cons.h>

//-----------------------------------------------------------------------------
//  Include Files  

#include "TfrLex.h"                     // this module

//-----------------------------------------------------------------------------
//	Extract (skip over) a term.
//-----------------------------------------------------------------------------

TInt  TfrLex::Val( TLex& aLex, const TDesC& aTerm )
{
TLex term( aTerm );
TLexMark mark;

aLex.Mark( mark );
while ( !term.Eos() )
	{
	if ( aLex.Eos() || term.Get() != aLex.Get() )
		{
		aLex.UnGetToMark( mark );
		return KErrNotFound;
		}
	}
return KErrNone;
}

//-----------------------------------------------------------------------------
//	Extract (check if text begins with) a term.

TBool TfrLex::Val( const TDesC& aText, const TDesC& aTerm )
{
TLex lex( aText );
return ( Val( lex, aTerm ) == KErrNone );
}

//-----------------------------------------------------------------------------
//	Extract (skip over) a term. Folded comparison.

TInt  TfrLex::ValF( TLex& aLex, const TDesC& aTerm )
{
TLex term( aTerm );
TLexMark mark;

aLex.Mark( mark );
while ( !term.Eos() )
	{
	if ( aLex.Eos() || User::Fold(term.Get()) != User::Fold(aLex.Get()) )
		{
		aLex.UnGetToMark( mark );
		return KErrNotFound;
		}
	}
return KErrNone;
}

//-----------------------------------------------------------------------------
//	Extract (check if text begins with) a term.

TBool TfrLex::ValF( const TDesC& aText, const TDesC& aTerm )
{
TLex lex( aText );
return ( ValF( lex, aTerm ) == KErrNone );
}

//-----------------------------------------------------------------------------
//	Extract (check if text begins with) a term.
//
//	Skip spaces and get token up to end-of-string, a space char or up to the
//	very character you define. The GetF supports tokens like "*". If a token
//	begins with apostrophe but does not end to one, the function L E A V E S
//	with error KErrArgument.

TPtrC TfrLex::GetL( TLex& aLex, const TChar  aChar )
{
// Skip spaces and mark the token's start point.
aLex.SkipSpace();
TLexMark mark;

aLex.Mark(mark);    

if (aLex.Peek() == '"' )
	{
	// Skip the " and do find next " followed by eos, space or aChar.
	aLex.Inc();
	TChar x;
	while ( x = aLex.Get(), !x.Eos() )
		{
		if ( x == '"' )
			{
			// Found a " character - but is it the end of the token?
			x = aLex.Peek(); // peek the next character
			if ( x.Eos() || x.IsSpace() || x == aChar )
				// End of token: return token.
				return aLex.MarkedToken(mark);
			}
		}
	// Unget and L E A V E because did not find the end " of token.
	aLex.UnGetToMark(mark);
	User::Leave(KErrArgument);
	return aLex.MarkedToken(mark); // never reached (l e a v e).
	}
else
	{
	// Is not a "*" token: find eos or the next space or the aChar
	// and return the token.
	TChar x;
	while ( x = aLex.Peek(), !x.Eos() && !x.IsSpace() && x != aChar )
		aLex.Inc();

	return aLex.MarkedToken(mark);
	}
}

//-----------------------------------------------------------------------------
//	Skip spaces and get token up to end-of-string, a space char or up to the
//	very characters you define. The GetF supports tokens like "*".  If token
//	begins with apostrophe but does not end to one, the function L E A V E S
//	with error KErrArgument.

TPtrC TfrLex::GetL( TLex& aLex, const TDesC& aList )
{
// Skip spaces and mark the token's start point.
aLex.SkipSpace();
TLexMark mark;
aLex.Mark(mark);    

if ( aLex.Peek() == '"' )
	{
	// Skip the " and do find next " followed by eos, space or any char
	// in the aList.
	aLex.Inc();
	TChar x;
	while ( x = aLex.Get(), !x.Eos() )
		{
		if ( x == '"' )
			{
			// Found a " character - but is it the end of the token?
			x = aLex.Peek();
			if ( x.Eos() || x.IsSpace() || aList.Locate(x) < 0 )
				// End of token: return token.
				return aLex.MarkedToken(mark);
			}
		}
	// Unget and L E A V E because did not find the end " of token.
	aLex.UnGetToMark(mark);
	User::Leave(KErrArgument);
	return aLex.MarkedToken(mark); // never reached (l e a v e).
	}
else
	{
	// Is not a "*" token: find eos or the next space or any char
	// in the aList and return the token.
	TChar x;
	while ( x = aLex.Peek(), !x.Eos() && !x.IsSpace() && aList.Locate(x) < 0 )
		aLex.Inc();

	return aLex.MarkedToken(mark);
	}
}

//-----------------------------------------------------------------------------
//	Skip spaces and then eat the very character or the very term you define.
//	Return ETrue (was found and swallowed) or EFalse.
// Skip spaces and eat a particular character.
//

TBool TfrLex::Eat( TLex& aLex, const TChar aChar )
{
TLexMark unget;
if ( aChar.IsSpace() )
	{
	// A space character requires its own logic = go over
	// the other space characters - x gets the next char.
	TChar x;
	aLex.Mark(unget);
	while ( x = aLex.Get(), x.IsSpace() && x != aChar )
		{};
	if ( x == aChar ) 
		return ETrue;
	}
else
	{
	// For other, non-space, characters: skip spaces and
	// get the next character x.
	aLex.SkipSpace();
	aLex.Mark(unget);
	if ( aLex.Get() == aChar ) 
	return ETrue;
	}

// The character wasn't there, unget to the start point.
aLex.UnGetToMark(unget);
return EFalse;
}

//-----------------------------------------------------------------------------
// Skip spaces and eat a term, exact comparison.

TBool TfrLex::Eat( TLex& aLex, const TDesC& aTerm )
{
aLex.SkipSpace();
TLexMark unget;
aLex.Mark(unget);
if ( Val(aLex,aTerm) == KErrNone ) 
	return ETrue;
aLex.UnGetToMark(unget);
return EFalse;
}

//-----------------------------------------------------------------------------
// Skip spaces and eat a term, folded comparison.
//

TBool TfrLex::EatF( TLex& aLex, const TDesC& aTerm )
{
aLex.SkipSpace();
TLexMark unget;
aLex.Mark(unget);
if ( ValF(aLex,aTerm) == KErrNone ) return ETrue;
aLex.UnGetToMark(unget);
return EFalse;
} 

//-----------------------------------------------------------------------------
//	Get from first non white space onwards.

TPtrC TfrLex::TrimLeft( const TDesC& aText )
{
TLex lex( aText );
lex.SkipSpaceAndMark();
return lex.Remainder();
}

//-----------------------------------------------------------------------------
//	Get up to the last non white space.

TPtrC TfrLex::TrimRight( const TDesC& aText )
{
TLex lex( aText );
TPtrC remainder;
do
	{
	lex.SkipCharacters();
	remainder.Set( lex.MarkedToken() );
	lex.SkipSpace();
	} while ( !lex.Eos() );

return remainder;
}

//-----------------------------------------------------------------------------
//	Get from first non white space onwards and up to last non white space.
//	Is TrimLeft and TrimRight.

TPtrC TfrLex::Trim( const TDesC& aText )
{
return TrimRight( TrimLeft( aText ) );
}

//-----------------------------------------------------------------------------
//	Peel = Get text like "*" without the two apostrophes. If not like "*",
//	then returns the text as such as the whole.

TPtrC TfrLex::Peel( const TDesC& aText )
{
TPtrC result;
TInt  length = aText.Length();
if ( length >= 2 && (aText[0] == '"' && aText[length-1] == '"') )
	result.Set( aText.Mid(1,length-2) );
else
	result.Set( aText );

return result;
}

//-----------------------------------------------------------------------------
//	Trim and then Peel.

TPtrC TfrLex::TrimAndPeel( const TDesC& aText )
{
return Peel(Trim(aText));
}

//-----------------------------------------------------------------------------
//	Peel and them Trim.

TPtrC TfrLex::PeelAndTrim( const TDesC& aText )
{
return Trim(Peel(aText));
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------
