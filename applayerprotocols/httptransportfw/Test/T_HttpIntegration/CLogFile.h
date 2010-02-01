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
// This header file introduces the CLogFile amnd the CLogFilePile classes.
// 
//

#ifndef __CLOGFILE_H__
#define __CLOGFILE_H__

//-----------------------------------------------------------------------------
//  INCLUDES

#include <e32base.h>
#include <f32file.h>

//-----------------------------------------------------------------------------

class   TLogFileOverflow : public TDes16Overflow
{
public:
	
	virtual void Overflow( TDes16& aDes );
};

//-----------------------------------------------------------------------------

class CLogFile : public CBase
{
public:     // Constructors and destructors

	// aMaxLength sets the maximum length of formatted output.
	static CLogFile* NewL ( TInt aMaxLength = 512 );
	static CLogFile* NewLC( TInt aMaxLength = 512 );
	~CLogFile( );

	private:    // Constructors and destructors

	void ConstructL( TInt aMaxLength = 512 );
	CLogFile( ) {};

public:     // New functions

	// Open. Closes the present file.
	TInt Open( const TDesC& aFileName );

	// Reopen after closed.
	TInt Open( );

	// Close iff is open. Preserves the file name.
	void Close( );

	// Is open?
	TBool IsOpen( );

	// Get file name.
	TPtrC FileName( ) const;

	// Get last error code; is set by Open and Prints.
	TInt Error( );

	// Print standard format heading iff file is open.
	void PrintHeading( );

	// Print unformatted text iff file is open.
	// Will be removed - please use the Writes.
	void Print  ( const TDesC& aText );
	void Println( const TDesC& aText );
	void Println( ); // end-line = \r\n

	// Write unformatted text iff file is open.
	void Write  ( const TDesC& aText );
	void Writeln( const TDesC& aText );
	void Writeln();

	// Printf formatted text iff file is open.
	void Printf( TRefByValue<const TDesC> aFmt, ... );
	void Printf( TRefByValue<const TDesC> aFmt, VA_LIST& aList );

private:

	// Open file.
	TInt DoOpen ( );

	// Close file.
	void DoClose( );

	// Flush file.
	TInt DoFlush( );

	// Write heading.
	TInt DoHeading( );

	// Write formatted.
	TInt DoWrite( TRefByValue<const TDesC> aFmt, VA_LIST& aList );

	// Write unformatted.
	TInt DoWrite( const TDesC&  aText );

	#ifdef _UNICODE
	// Write 8-bit string.
	TInt DoWrite( const TDesC8& aText );
	#endif

private:    // Data

	// Is open?
	TBool iIsOpen;

	// File name.
	TFileName iFileName;

	// File server session.
	RFs iFs;

	// File handle.
	RFile iFile;

	// Last error.
	TInt iError;

	// Buffer length.
	TInt iBufLength;

	// Buffer itself.
	HBufC*  iBuffer;

	// Overflow handler.
	TLogFileOverflow iOverflow;

};

//-----------------------------------------------------------------------------

#endif //__CLOGFILE_H__            

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
