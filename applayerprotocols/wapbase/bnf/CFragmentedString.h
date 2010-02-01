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
// A string class implementation which allows quick addition of partial strings
// (no copying) by using an internal array of fragmented strings.
// The class allows comprehensive character based matching functionality 
// along with infinite depth marking.
// 
//

#ifndef __CFRAGMENTEDSTRING_H__
#define __CFRAGMENTEDSTRING_H__

// includes
#include <e32base.h>
#include <cstack.h>


//
// CFragmentedString

//##ModelId=3B666BC6034A



class CFragmentedString : protected CArrayPtrFlat<HBufC>
/**  
Utility that allows a single string to be built from an array of consecutive sub-strings.

The sub-strings can be inserted by reference or copied.

The object maintains information that points to a current position within the string. A typical 
use is to test the contents of the string using one of the Match...() functions, and then use 
ConsumeMatched() to advance past the matched area.

The class also supports inserting an unlimited number of marks in the string, and performing 
operations relative to the head (i.e. last inserted) mark. 
    @publishedAll
    @deprecated
*/
	{
protected:
	class TStringMark
	/** A mark at a string position. */
		{
	public:
		/** Constructor.

		@param aIndex Array index of the marked sub-string
		@param aCharacter Character position within the sub-string for the mark
		*/
		TStringMark(TInt aIndex, TInt aCharacter)
		: iMarkIndex(aIndex), iMarkCharacter(aCharacter)
		{
		}

	public:
		/** Array index of the marked sub-string. */
		TInt iMarkIndex;
		/** Character position within the sub-string for the mark. */
		TInt iMarkCharacter;
		};
	/** A stack of string position marks. */
	typedef CStack<TStringMark, ETrue> CMarkStack;

public:
	/** Defines possible results of a string matching operation for this class. */
	enum TStringMatch 
		{ 
		/** There was no match. */
		ENoMatch, 
		/** There was a complete match. */
		EMatch,
		/** String contained insufficient data to perform the match operation.

		This can mean that the start of the target string was matched, but the string 
		being searched ended before a complete match was found. */
		EInsufficientData
		};

public:
	IMPORT_C CFragmentedString();
	//##ModelId=3B666BC700AD
	IMPORT_C ~CFragmentedString();

	//##ModelId=3B666BC70099
	IMPORT_C void AddStringL(HBufC* aString); // this version is more efficient
	//##ModelId=3B666BC700A3
	IMPORT_C void AddStringL(const TDesC& aString);
	
	//##ModelId=3B666BC70090
	IMPORT_C TInt Length() const;
	//##ModelId=3B666BC70071
	IMPORT_C HBufC* StringL() const;
	//##ModelId=3B666BC70068
	IMPORT_C HBufC* ContentL() const;
	//##ModelId=3B666BC70067
	IMPORT_C void Reset();

	//##ModelId=3B666BC7005D
	IMPORT_C TStringMatch Match(const TDesC& aString);
	//##ModelId=3B666BC70049
	IMPORT_C TStringMatch MatchRange(const TUint aLower, const TUint aUpper);
	//##ModelId=3B666BC7003F
	IMPORT_C TStringMatch MatchSelect(const TDesC& aSelection);
	//##ModelId=3B666BC70037
	IMPORT_C TStringMatch MatchNotSelect(const TDesC& aSelection);
	//##ModelId=3B666BC70036
	IMPORT_C void ConsumeMatched();

	//##ModelId=3B666BC70035
	IMPORT_C HBufC* MarkedL();
	//##ModelId=3B666BC7002B
	IMPORT_C HBufC* MarkedWithInitialTextL(const TDesC& aInitialText);
	//##ModelId=3B666BC70022
	IMPORT_C void Mark(); // Mark can leave
	//##ModelId=3B666BC70021
	IMPORT_C void DeleteMark();
	//##ModelId=3B666BC70018
	IMPORT_C void ResetToMark();

	//##ModelId=3B666BC7000E
	IMPORT_C void ReplaceMarkedL(HBufC* aString);
	//##ModelId=3B666BC70005
	IMPORT_C void ReplaceMarkedAndSkipL(HBufC* aString);
	//##ModelId=3B666BC70003
	IMPORT_C void InsertStringL(HBufC* aString);

protected:
	//##ModelId=3B666BC603E1
	IMPORT_C void DeleteToMark(const TStringMark& aStringMark);
	//##ModelId=3B666BC603C4
	IMPORT_C void InsertStringToL(HBufC* aString, TInt aStringIndex, TInt aLengthIntoString);
	//##ModelId=3B666BC70072
	HBufC* StringL(TInt aStartIndex, TInt aStartCharacter, TInt aEndIndex, TInt aEndCharacter, const TDesC* aInitialText=NULL) const;
	//##ModelId=3B666BC603C3
	void StartMatch();
	//##ModelId=3B666BC603B8
	CFragmentedString::TStringMatch DoMatchSelect(const TDesC& aSelection, TBool aInSelection);
	//##ModelId=3B666BC603AE
	TBool FindNextMatchChar(TUint& aChar);

protected:
	//##ModelId=3B666BC603A4
	/** Result of the last match operation. */
	TStringMatch iMatched;

	/** Array index of the sub-string found in the last match operation. */
	//##ModelId=3B666BC6039A
	TInt iMatchedToIndex;
	/** Current character position within the iMatchedToIndex sub-string found in the last match operation. */
	//##ModelId=3B666BC60390
	TInt iMatchedToCharacter;
	/** Array index of the current sub-string. */
	//##ModelId=3B666BC60386
	TInt iCurrentIndex;
	/** Current character position within the current sub-string. */
	//##ModelId=3B666BC6037C
	TInt iCurrentCharacter;
	/** Stack of marks in the string.

	Mark() pushes a mark on the stack; DeleteMark() pops one off.
	*/
	//##ModelId=3B666BC60372
	CMarkStack iMarkStack;
	};

#endif // __CFRAGMENTEDSTRING_H__
