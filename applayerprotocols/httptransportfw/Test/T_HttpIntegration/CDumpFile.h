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
// This header file introduces the CDmpfile (dump file) class.
// 
//

#ifndef __CDMPFILE_H__
#define __CDMPFILE_H__

//-----------------------------------------------------------------------------

#include <e32base.h>
#include <f32file.h>
#include <e32std.h>                     //
#include <s32file.h>                    //
#include <e32hal.h>                     //
#include <fbs.h>                        //

//-----------------------------------------------------------------------------

class CDmpfile : public CBase
{

public:     // Constructors and destructors

	static CDmpfile* New  ();
	static CDmpfile* NewL ();
	static CDmpfile* NewLC();
	~CDmpfile();

private:    // Constructors and destructors

	void ConstructL();
	CDmpfile( ) {};

public:     // New functions

	// Open. Open existing file without replacing it. If
	// it does not already exist, create it.
	TInt Open( const TDesC& aFile );
	TInt Open( const TDesC& aPath, const TDesC& aFile );

	// Replace. Open existing file replacing its contents.
	// If it does not already exist, create it.
	TInt Replace( const TDesC& aFile );
	TInt Replace( const TDesC& aPath, const TDesC& aFile );

	// Open or Replace again. These functions first close
	// the file if it happens to have been left open, and
	// then Open or Replace. You can call these ones also
	// after closing the file yourself.
	TInt Open();
	TInt Replace();

	// Close file.
	void Close();

	// Write.
	TInt Write( const TDesC8& aDes );
	TInt Write( const TDesC8& aDes, TInt aLength );

	// Flush.
	TInt Flush();

	// Set flush write On/Off.
	void SetFlush( TBool doFlush=ETrue );

	// Get size. Negative KErr* if fails.
	TInt Size();

	// Is Open?
	TBool IsOpen();

	// Get (full) file name.
	TPtrC FileName() const;

	// Get error, error code returned by the last called
	// function.
	TInt Error();

private:

	// Open/Replace.
	TInt DoOpen();
	TInt DoReplace();

private:    // Data

	// Is open?
	TBool iIsOpen;

	// Flush write?
	TBool iDoFlush;

	// File (full) name.
	TFileName iFileName;

	// Parse buffer.
	TParse iParse;

	// File server session.
	RFs iFs;

	// File handle.
	RFile iFile;

	// Last error.
	TInt iError;

};

//-----------------------------------------------------------------------------

#endif  // __CDMPFILE_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
