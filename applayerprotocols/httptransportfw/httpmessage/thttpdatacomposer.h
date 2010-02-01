// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __THTTPDATACOMPOSER_H__
#define __THTTPDATACOMPOSER_H__

#include <e32std.h>

class MHttpBufferSupplier;


class THttpDataComposer
/**	
	The THttpDataComposer class allows tokens to be added to a data buffer.
	A token can be followed by a specified delimiter. The data buffer can be
	released at any time, allowing a fresh set of data to composed. Resetting
	the composer results in the data buffer being deleted.

	An MHttpBufferSupplier object owns the data buffer into which the tokens
	are placed. If there is not enough space for the tokens, the data composer
	asks the buffer supplier to resize the buffer to at least the required size.
	@internalComponent
	@see		MHttpBufferSupplier
*/
	{
public:	// enums

	enum TDelimiterType
/**	
	The TDelimiterType enumeration defines the types of delimiter that can be 
	specified to follow a token.
	
*/
		{
		/**	Specifies that no delimiter should follow.
		*/
		ENone		=0,

		/**	The token should be followed by a Space character.
		*/
		ESpace,

		/** The token should be followed by a Colon and then a Space character.
		*/
		EColonSpace,

		/** The token should be followed by a Carriage Return and then a Line
			Feed character.
		*/
		ECRLF
		};

public:	// methods

	THttpDataComposer(MHttpBufferSupplier& aBufferSupplier);

	void AddTokenL(const TDesC8& aToken, TDelimiterType aDelimiter);
	void Release();
	void Reset();

private:	// attributes

	MHttpBufferSupplier&	iBufferSupplier;
	TPtr8					iBuffer;
	TInt					iRequiredLength;

	};

#endif	// __THTTPDATACOMPOSER_H__
