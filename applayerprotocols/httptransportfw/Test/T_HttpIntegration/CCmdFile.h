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
// CCmdFile class for reading lines from command files. Supports argument
// strings.
// 
//

#ifndef __CCMDFILE_H__
#define __CCMDFILE_H__

//-----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>
#include <e32std.h>
#include <e32hal.h>
#include <s32file.h>
#include <f32file.h>
#include <fbs.h>

//-----------------------------------------------------------------------------

#include "TfrLex.h"

//-----------------------------------------------------------------------------

class CCmdFile : public CBase
{

public:     // Constructors and destructors

	static CCmdFile* NewL ();
	static CCmdFile* NewLC();
	~CCmdFile();

private:    // Constructors and destructors

	void ConstructL();
	CCmdFile() {};

public:     // New functions

	// Open file. Closes the old one.
	TInt Open(const TDesC& aFile);
	TInt Open(const TDesC& aPath, const TDesC& aFile);

	// Add more arguments, 0 or more from aArgs string.
	void AddArgsL(const TDesC& aArgs);

	// Get count and the i'th argument, legal i assumed.
	TInt  Argc() const;
	TPtrC Argv(TInt aIndex) const;

	// Read line. KErrEof if no file.
	TInt Read(TDes& aDes);

	// Rewind file.
	TInt Rewind();

	// Close file.
	void Close();

	// Get file name.
	TPtrC FileName() const;

	// Get line number, nbr of lines read so far.
	TInt Line();

private:    // Data

	// File name.
	TFileName iFileName;

	// Arguments.
	CArrayPtrSeg<HBufC>* iArgs;

	// For parse.
	TParse iParse;

	// Line number, number of lines read so far.
	TInt iLine;

	// Buffers.
	TPtrC iBuf;
	TLex  iLex;

};

//-----------------------------------------------------------------------------

#endif  // __CCMDFILE_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
