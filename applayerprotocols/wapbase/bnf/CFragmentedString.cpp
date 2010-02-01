// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CFreagmentedString.cpp
// 
//

// header
#include <cfragmentedstring.h>

// constants
const TInt KFragmentedStringGranularity=128;

/** Constructor */
EXPORT_C CFragmentedString::CFragmentedString()
: CArrayPtrFlat<HBufC>(KFragmentedStringGranularity)
	{
	}

/** Destructor.

This resets and destroys the HBufC array.
*/
EXPORT_C CFragmentedString::~CFragmentedString()
	{
	ResetAndDestroy();
	}

/** Copies a sub-string and then appends the copy to the string.

@param aString Sub-string to append
*/
EXPORT_C void CFragmentedString::AddStringL(const TDesC& aString)
// Adds the string to our internal array of strings
	{
	HBufC* string=aString.AllocL();
	AddStringL(string);
	}

/** Appends a sub-string to the string.

@param aString Sub-string to append
*/
EXPORT_C void CFragmentedString::AddStringL(HBufC* aString)
// Adds the string to our internal array of strings
	{
	// take ownership of aString
	CleanupStack::PushL(aString);
	AppendL(aString);
	CleanupStack::Pop(); // aString
	}

/** Gets the length of the string.

@return String length
*/
EXPORT_C TInt CFragmentedString::Length() const
// Calculates the entire length of our 'complete' string.
	{
	if (Count()==0)
		CONST_CAST(CFragmentedString*, this)->iCurrentCharacter=0;

	TInt len = -iCurrentCharacter;
	for (TInt index = iCurrentIndex; index < Count(); index++)
		len+=At(index)->Length();
	return len;
	}

/** Gets a string containing all sub-strings after the current position in a newly-allocated buffer.

It is the caller's responsibility to clean up the returned string.

@return String containing all sub-strings after the current position in a newly-allocated buffer
*/
EXPORT_C HBufC* CFragmentedString::StringL() const
// Returns a newly allocated non-fragmented version of our string.
	{
	if (Count()>0)
		return StringL(iCurrentIndex, iCurrentCharacter, Count(), At(Count()-1)->Length());
	else
		return HBufC::NewL(1);
	}

/** Gets the entire string in a newly-allocated buffer.

It is the caller's responsibility to clean up the returned string.

@return Entire string in a newly-allocated buffer
*/
EXPORT_C HBufC* CFragmentedString::ContentL() const
// Returns a newly allocated non-fragmented version of our string.
	{
	if (Count()>0)
		return StringL(0, 0, Count(), At(Count()-1)->Length());
	else
		return HBufC::NewL(1);
	}

void CFragmentedString::StartMatch()
	{
	iMatched = ENoMatch;
	iMatchedToIndex = iCurrentIndex;
	iMatchedToCharacter = iCurrentCharacter;
	}


// could be made more efficient by matching blocks of text
/** Tests if a specified target string occurs at the current position.

The function does a byte-for-byte comparison of the string contents from the current position against aString.

@return Match result
@param aString String to attempt to match
*/
EXPORT_C CFragmentedString::TStringMatch CFragmentedString::Match(const TDesC& aString)
	{
	TInt stringIndex = 0;
	StartMatch();
	const TText* stringPtr=aString.Ptr();

	while (iMatchedToIndex<Count() && stringIndex<aString.Length())
		{
		HBufC const* current=At(iMatchedToIndex);
		const TText* currentPtr=current->Ptr()+iMatchedToCharacter;
		while (stringIndex<aString.Length() && iMatchedToCharacter<current->Length())
			{
			if (*stringPtr!=*currentPtr)
				return ENoMatch;
			++iMatchedToCharacter;
			++stringIndex;
			++stringPtr;
			++currentPtr;
			}

		if (iMatchedToCharacter==current->Length())
			{
			iMatchedToCharacter=0;
			iMatchedToIndex++;
			}
		}

	if (stringIndex<aString.Length())
		iMatched=EInsufficientData;
	else
		iMatched=EMatch;

	return iMatched;
	}


TBool CFragmentedString::FindNextMatchChar(TUint& aChar)
	{
	const TInt numberOfFragments=Count();

	TBool finished=EFalse;
	while (iMatchedToIndex<numberOfFragments && !finished)
		{
		HBufC* current = At(iMatchedToIndex);
		finished=iMatchedToCharacter<current->Length();
		if (!finished)
			{
			iMatchedToCharacter = 0;
			iMatchedToIndex++;
			}
		}
	
	/*if (iMatchedToIndex<numberOfFragments && iMatchedToCharacter>=At(iMatchedToIndex)->Length())
		{
		iMatchedToCharacter=0;
		while (iMatchedToIndex<numberOfFragments && At(iMatchedToIndex)->Length()==0)
			iMatchedToIndex++;
		}
	*/
		
	const TBool foundChar=iMatchedToIndex<numberOfFragments && iMatchedToCharacter<At(iMatchedToIndex)->Length();

	if (foundChar)
		{
		HBufC const* current = At(iMatchedToIndex);
		aChar=(*current)[iMatchedToCharacter];
		}

	return foundChar;
	}


/** Tests if the character at the current position is within a specified character code range.

@return Match result
@param aLower Lower range (inclusive) for a match
@param aUpper Upper range (inclusive) for a match
*/
EXPORT_C CFragmentedString::TStringMatch CFragmentedString::MatchRange(TUint aLower, TUint aUpper)
	{
	StartMatch();
	TUint character;
	if (FindNextMatchChar(character))
		{
		if (character>=aLower && character<=aUpper)
			{
			iMatched=EMatch;
			++iMatchedToCharacter;		
			}
		}
	else
		iMatched = EInsufficientData;
	
	return iMatched;
	}

CFragmentedString::TStringMatch CFragmentedString::DoMatchSelect(const TDesC& aSelection, TBool aInSelection)
	{
	StartMatch();
	TUint character;
	if (FindNextMatchChar(character))
		{
		if ((aSelection.Locate(character)==KErrNotFound)!=aInSelection)
			{
			iMatched=EMatch;
			++iMatchedToCharacter;
			}
		}
	else
		iMatched = EInsufficientData;	

	return iMatched;
	}

/** Tests if the character at the current position matches any character in a specified string.

@return Match result
@param aSelection String specifying one or more characters, any of which will result in a successful match
*/
EXPORT_C CFragmentedString::TStringMatch CFragmentedString::MatchSelect(const TDesC& aSelection)
	{
	return DoMatchSelect(aSelection, ETrue);
	}

/** Tests if the character at the current position does not match any character in a specified string.

@return Match result
@param aSelection String specifying one or more characters, any of which will result in a failed match
*/
EXPORT_C CFragmentedString::TStringMatch  CFragmentedString::MatchNotSelect(const TDesC& aSelection)
	{
	return DoMatchSelect(aSelection, EFalse);
	}

/** Removes and deletes all sub-strings occurring before the position of the last successful match.

Any sub-strings at or after a mark are not deleted, regardless of the last successful match position. */
EXPORT_C void CFragmentedString::ConsumeMatched()
	{
	// update currents
	iCurrentIndex = iMatchedToIndex;
	iCurrentCharacter = iMatchedToCharacter;

	// find out if we can delete any parts
	TInt freeIndex;
	if (iMarkStack.IsEmpty())
		freeIndex=0;
	else
		{
		TStringMark* stringMark=iMarkStack.Last();
		freeIndex=stringMark->iMarkIndex-1;
		}

	// delete any parts which are no longer needed
	if (freeIndex<iCurrentIndex && freeIndex>=0)
		{
		for (TInt index=iMarkStack.Count(); --index>=0;)
			iMarkStack[index]->iMarkIndex-=freeIndex+1;

		for (TInt stringIndex=freeIndex; stringIndex>=0; stringIndex--)
			delete(At(stringIndex));
		Delete(0,freeIndex+1);
		iCurrentIndex-=freeIndex+1;
		iMatchedToIndex-=freeIndex+1;
		}
	}

/** Resets the string.

This resets and destroys the HBufC array, and clears all indexes and marks. */
EXPORT_C void CFragmentedString::Reset()
	{
	ResetAndDestroy();
	iMarkStack.Clear();
	iCurrentIndex = 0;
	iCurrentCharacter = 0;
	}

HBufC* CFragmentedString::StringL(TInt aStartIndex, TInt aStartCharacter, TInt aEndIndex, TInt aEndCharacter, const TDesC* aInitialText/*=NULL*/) const
	{
	// Work out how long our string is going to be
	TInt markedLength=-aStartCharacter;
	TInt index;
	for (index=aStartIndex; index<aEndIndex; index++)
		markedLength+=At(index)->Length();
	if (aEndIndex<Count())
		markedLength+=aEndCharacter;

	const TInt extraLen=aInitialText?aInitialText->Length():0;
	HBufC* markedString = NULL;
 	if(markedLength || extraLen)
 		{
 		// Now allocate the string
 		markedString = HBufC::NewL(markedLength+extraLen);
 		TPtr markedStringPtr = markedString->Des();

    if (aInitialText)
 		markedStringPtr.Append(*aInitialText);
	 		// Now build the string up
 	if (aStartIndex == aEndIndex)
 		markedStringPtr.Append(At(aStartIndex)->Mid(aStartCharacter, markedLength));
 	else
 		{
 		if (aStartIndex < aEndIndex)
 			markedStringPtr.Append(At(aStartIndex)->Mid(aStartCharacter));
 		for (index = aStartIndex+1; index < aEndIndex; index++)
 			markedStringPtr.Append(*At(index));
 		if (aEndIndex < Count())
 			markedStringPtr.Append(At(aEndIndex)->Left(aEndCharacter));
 		}
	
	}
	return markedString;
	}

/** Gets a new string containing the string contents from the head mark to the current index position, prepended with a specified string.

It is the caller's responsibility to clean up the returned string.

@return New string containing aInitialText and then the marked contents
@param aInitialText String to prepend to the result
*/
EXPORT_C HBufC* CFragmentedString::MarkedWithInitialTextL(const TDesC& aInitialText)
	{
	TStringMark* stringMark = iMarkStack.Head();
	return StringL(stringMark->iMarkIndex, stringMark->iMarkCharacter, iCurrentIndex, iCurrentCharacter, &aInitialText);
	}

/** Gets a new string containing the string contents from the head mark to the current index position.

It is the caller's responsibility to clean up the returned string.

@return New string containing marked contents
*/
EXPORT_C HBufC* CFragmentedString::MarkedL()
	{
	TStringMark* stringMark = iMarkStack.Head();
	return StringL(stringMark->iMarkIndex, stringMark->iMarkCharacter, iCurrentIndex, iCurrentCharacter);
	}

/** Adds a mark at the current index position.

This mark becomes the head mark.

Note this function can leave with an out of memory error.
 */
EXPORT_C void CFragmentedString::Mark() // Mark can leave
	{
	TStringMark* stringMark = new (ELeave) TStringMark(iCurrentIndex, iCurrentCharacter);
	iMarkStack.PushL(stringMark);
	}

/** Deletes the head mark. */
EXPORT_C void CFragmentedString::DeleteMark()
	{
	TStringMark* stringMark = iMarkStack.Pop();
	delete stringMark;
	}

// move current position to top (head) mark
/** Moves the current index position to the head mark. */
EXPORT_C void CFragmentedString::ResetToMark()
	{
	TStringMark* stringMark = iMarkStack.Head();
	iCurrentIndex = stringMark->iMarkIndex;
	iCurrentCharacter = stringMark->iMarkCharacter;
	}

/** Replaces the string contents to the head mark with a specified string.

@param aString Replacment string
*/
EXPORT_C void CFragmentedString::ReplaceMarkedL(HBufC* aString)
	{
	TStringMark* stringMark = iMarkStack.Head();
	DeleteToMark(*stringMark);
	InsertStringL(aString);
	}

/** Replaces the string contents to the head mark with a specified string, and then 
advances the current index position to the next sub-string.

@param aString Replacment string
*/
EXPORT_C void CFragmentedString::ReplaceMarkedAndSkipL(HBufC* aString)
	{
	ReplaceMarkedL(aString);
	iCurrentIndex++;
	}

// could be speeded up
/** Deletes from the current index position to the specified mark.

@param aStringMark Mark to delete to
*/
EXPORT_C void CFragmentedString::DeleteToMark(const TStringMark& aStringMark)
	{
	if (iCurrentIndex>=Count())
		{
		iCurrentIndex=Count()-1;
		if (iCurrentIndex==-1)
			{
			iCurrentIndex=0;
			iCurrentCharacter=0;
			}
		else
			iCurrentCharacter=At(iCurrentIndex)->Length();
		}

	while (iCurrentIndex > aStringMark.iMarkIndex  
			|| (iCurrentIndex==aStringMark.iMarkIndex 
				&& iCurrentCharacter>aStringMark.iMarkCharacter))
		{
		if(--iCurrentCharacter<0)
			{
			if(At(iCurrentIndex)->Length()==0)
				Delete(iCurrentIndex);
				
			iCurrentIndex--;
			iCurrentCharacter = At(iCurrentIndex)->Length();
			}
		At(iCurrentIndex)->Des().Delete(iCurrentCharacter,1);
		}
	}

/** Inserts a specified string at the current index position.

@param aString String to insert
*/
EXPORT_C void CFragmentedString::InsertStringL(HBufC* aString)
	{
	InsertStringToL(aString, iCurrentIndex, iCurrentCharacter);
	}

/** Inserts a string at a specified position.

1. if aLengthIntoString is 0, the function inserts a new sub-string at array position aStringIndex

2. if aLengthIntoString is equal to the length of the sub-string at aStringIndex, then it inserts a new sub-string at array position aStringIndex+1. 

3. If aLengthIntoString is in the middle of theaStringIndex sub-string, then it:

	a) inserts a new sub-string at aStringIndex+1 holdingaString

	b) inserts a new sub-string at aStringIndex+2 holding the data from aStringIndex after aLengthIntoString 

	c) truncates the original aStringIndex to hold only the data before aLengthIntoString

@param aString String to insert
@param aStringIndex Array index of the sub-string at which to insert
@param aLengthIntoString Character position within the sub-string at which to insert
*/
EXPORT_C void CFragmentedString::InsertStringToL(HBufC* aString, TInt aStringIndex, TInt aLengthIntoString)
	{
	if (aString==NULL || aString->Length()==0)
		return;
	
	// In the beginning of a string
	if (aLengthIntoString==0)
		{
		InsertL(aStringIndex, aString);
		return;
		}

	// At the end of a string
	if (aLengthIntoString==At(aStringIndex)->Length())
		{
		InsertL(aStringIndex+1, aString);
		return;
		}

	// In the middle
	TPtrC remainder = At(iCurrentIndex)->Mid(iCurrentCharacter);
	InsertL(aStringIndex+1, aString);
	InsertL(aStringIndex+2, remainder.AllocL() );
	At(iCurrentIndex)->Des().SetLength(iCurrentCharacter);
	iCurrentIndex++;
	iCurrentCharacter = 0;
	}
