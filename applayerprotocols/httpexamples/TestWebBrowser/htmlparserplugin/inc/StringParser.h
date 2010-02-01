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

#ifndef _STRING_PARSER_H
#define _STRING_PARSER_H

// String parser class to parse a string incremently
class CStringParser : public CBase
{
public:

	static CStringParser* NewLC ( const TDesC8& aBuffer );
	static CStringParser* NewL  ( const TDesC8& aBuffer );

	virtual ~CStringParser ();	

	TBool GetNextCharacter ( TChar& aChar );
	TBool GetCurrentCharacter ( TChar& aChar );
	TBool ParseTill ( TPtrC8& aWord, TChar aEndChar );
	void ParseTill ( TPtrC8& aWord, const TDesC8& aCharSet );
	
	TBool SkipLength ( TInt aLen );

	void GetRemainder ( TPtrC8& aBuffer );
protected:
	CStringParser ( const TDesC8& aBuffer );
	void ConstructL ( const TDesC8& aBuffer );
	
private:
	void IncrementCurrentPos ();
	
private:
	const TDesC8& iBuffer;
	TInt iCurrentPos;
};

#endif // _STRING_PARSER_H