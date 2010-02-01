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
// CCmdLog.cpp
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// This module implements the CLogFile class
// Include Files  
// 
//

#include <e32std.h>                     //
#include <s32file.h>                    //
#include <f32file.h>                    //
#include <e32hal.h>                     //
#include <fbs.h>                        //

//-----------------------------------------------------------------------------

#include "CLogFile.h"                    // This module

//-----------------------------------------------------------------------------
// End-of-line

_LIT8(KEndOfLine8,"\r\n");
_LIT (KEndOfLine ,"\r\n");

//-----------------------------------------------------------------------------

void TLogFileOverflow::Overflow( TDes16& aDes )
{
_LIT(KErrOverflowMsg, "...");
if( aDes.MaxLength() >= KErrOverflowMsg().Length() + aDes.Length() )
	aDes.Append(KErrOverflowMsg);
}

//-----------------------------------------------------------------------------

CLogFile *CLogFile::NewL( TInt aMaxLength )
{
CLogFile* self = NewLC( aMaxLength );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CLogFile* CLogFile::NewLC( TInt aMaxLength )
{
CLogFile* self = new (ELeave) CLogFile();
CleanupStack::PushL(self);
self->ConstructL( aMaxLength );
return self;
}

//-----------------------------------------------------------------------------

CLogFile::~CLogFile( )
{
Close();
delete iBuffer;
iBuffer = NULL;
}

//-----------------------------------------------------------------------------

void CLogFile::ConstructL( TInt aMaxLength )
{
iIsOpen = EFalse;
iError = KErrNone;
iFileName.Zero();
iBufLength = aMaxLength;
iBuffer = NULL;
}

//-----------------------------------------------------------------------------

TInt CLogFile::Open( const TDesC& aFileName )
{
iError = KErrNone;
if ( IsOpen() )
	DoClose( );

iFileName.Copy( aFileName );
iError = DoOpen();

return iError;
}

//-----------------------------------------------------------------------------

TInt CLogFile::Open( )
{
iError = (!IsOpen()) ? DoOpen() : KErrNone;
return iError;
}

//-----------------------------------------------------------------------------

void CLogFile::Close( )
{
if ( IsOpen() ) 
	DoClose();

delete iBuffer;
iBuffer = NULL;
}

//-----------------------------------------------------------------------------

TBool CLogFile::IsOpen( )
{
return iIsOpen;
}

//-----------------------------------------------------------------------------

TPtrC CLogFile::FileName( ) const
{
TPtrC filename( iFileName );
return filename;
}

//-----------------------------------------------------------------------------

TInt CLogFile::Error( )
{
return iError;
}

//-----------------------------------------------------------------------------

void CLogFile::PrintHeading( )
{
iError = KErrNone;
if ( IsOpen() )
	{
	iError = DoHeading();
	(void)DoFlush();
	}
}

//-----------------------------------------------------------------------------

void CLogFile::Write( const TDesC& aText )
{
iError = KErrNone;
if ( IsOpen() )
	{
	iError = DoWrite( aText );
	(void)DoFlush();
	}
}

//-----------------------------------------------------------------------------

void CLogFile::Writeln( const TDesC& aText )
{
iError = KErrNone;
if ( IsOpen() )
	{
	iError = DoWrite( aText );
	(void)DoWrite( KEndOfLine8 );
	(void)DoFlush();
	}
}

//-----------------------------------------------------------------------------

void CLogFile::Writeln( )
{
iError = (IsOpen()) ? DoWrite( KEndOfLine8 ) : KErrNone;
}

//-----------------------------------------------------------------------------

void CLogFile::Printf( TRefByValue<const TDesC> aFmt, ... )
{
VA_LIST list;
VA_START(list, aFmt);
Printf(aFmt, list);
}

//-----------------------------------------------------------------------------

void CLogFile::Printf(TRefByValue<const TDesC> aFmt, VA_LIST& aList)
{
iError = KErrNone;
if (IsOpen())
	{
	iError = DoWrite(aFmt, aList);
	(void)DoFlush();
	}
}

//-----------------------------------------------------------------------------

TInt CLogFile::DoOpen ( )
{
// Allocate the formatting buffer if there is none so far.
if ( iBuffer == NULL ) 
	iBuffer = HBufC::New( iBufLength );

if ( iBuffer == NULL ) 
	return KErrNoMemory;

// Connect file server.
TInt ret = KErrNone;
if ( ret = iFs.Connect(), ret != KErrNone )
	return ret;

// Open/Create the file. Notice, that this does not create the log
// directory i.e. the directory must have been created in advance.
TUint mode = EFileStreamText|EFileWrite|EFileShareAny;
if ( ret = iFile.Open( iFs, iFileName, mode ), ret == KErrNotFound )
	// Create the file since it wasn't there.
	ret = iFile.Create( iFs, iFileName, mode );

if ( ret != KErrNone )
	{
	iFs.Close();
	return ret;
	}

// Seek to the end; error in seek is not recorded.
TInt filepos;
(void)iFile.Seek( ESeekEnd, filepos );

// Mark the file open and return.
iIsOpen = ETrue;
return ret;
}

//-----------------------------------------------------------------------------

void CLogFile::DoClose( )
{
iFile.Close();
iFs.Close();
iIsOpen = EFalse;
}

//-----------------------------------------------------------------------------

TInt CLogFile::DoFlush( )
{
return iFile.Flush();
}

//-----------------------------------------------------------------------------

TInt CLogFile::DoHeading( )
{
// Make a temporary buffer to format the date and time and format.
HBufC* buf = HBufC::New( 80 );
if ( buf == NULL ) 
	return KErrNoMemory;

TPtr timebuf = buf->Des();
TPtr buffer  = iBuffer->Des();
TTime time;
time.HomeTime();
TInt ret = KErrNone;
TRAP(ret, time.FormatL( timebuf, _L("%H:%T:%S on %E, %F%D-%*M-%Y")) );
if ( ret == KErrNone )
	// Then format the standard header (file name and date/time). An
	// overflow in here does not cause panic I hope.
	TRAP(	ret, 
				buffer.AppendFormat( _L("%S%S - %S%S%S"), 
				&iOverflow,
				&KEndOfLine, 
				&iFileName, 
				&timebuf, 
				&KEndOfLine, 
				&KEndOfLine )
				);

// Delete the temporary buffer and if formatting went ok, write.
delete buf;

if ( ret == KErrNone ) 
	ret = DoWrite( buffer );
return ret;
}

//-----------------------------------------------------------------------------

TInt CLogFile::DoWrite(TRefByValue<const TDesC> aFmt, VA_LIST& aList)
{
// Format to the buffer and write. Overflow in formatting is captured
// in order to not cause any panic.
TPtr buffer = iBuffer->Des();
buffer.Zero();
TInt ret = KErrNone;
TRAP(ret, buffer.AppendFormatList(aFmt, aList, &iOverflow));
if (ret == KErrNone) 
	ret = DoWrite(buffer);
return ret;
}

//-----------------------------------------------------------------------------

TInt CLogFile::DoWrite( const TDesC&  aText )
{
#ifndef _UNICODE

// Just write as such.
return iFile.Write ( aText );

#else

// Convert to 8-bit and write. Every write allocates a temporary buffer
// from heap and that buffer is allocated long enough to write the text
// as the whole.
HBufC8* buf8 = HBufC8::New( aText.Length() );
if ( buf8 == NULL ) 
	return KErrNoMemory;
TPtr8 ptr8 = buf8->Des();
ptr8.Copy( aText );
TInt ret = iFile.Write( ptr8 );

delete buf8;
return ret;

#endif // _UNICODE
}

//-----------------------------------------------------------------------------

#ifdef _UNICODE

TInt CLogFile::DoWrite( const TDesC8& aText )
{
return iFile.Write( aText );
}

#endif // _UNICODE

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------

