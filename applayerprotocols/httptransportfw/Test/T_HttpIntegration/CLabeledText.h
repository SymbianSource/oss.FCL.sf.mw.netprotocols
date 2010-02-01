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
// CCatalogue.h
// 
//

#ifndef __CLABELEDTEXT_H__
#define __CLABELEDTEXT_H__

//-----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>

//-----------------------------------------------------------------------------
//	CLabeledText class carrying a labeled text value.

class CLabeledText : public CBase
{

public:     // Constructors and destructors

	// Note: the Label and the Value are copied.
	static CLabeledText* NewL ( const TDesC& aLabel, const TDesC& aValue );
	static CLabeledText* NewLC( const TDesC& aLabel, const TDesC& aValue );
	~CLabeledText();

private:    // Constructors and destructors

	void ConstructL( const TDesC& aLabel, const TDesC& aValue );
	CLabeledText() {};

public:     // New functions

	// Get Label.
	TPtrC Label() const;

	// Get Value.
	TPtrC Value() const;

	// Set Value.
	void SetL(const TDesC &);

private:    // Data

	HBufC *iLabel;
	HBufC *iValue;

};

//-----------------------------------------------------------------------------

#endif  // __CLABELEDTEXT_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------

