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
// The CTEngine is the class that manages the test infrastructure
// It knows how to process scripts and where to find command knowledge
// The plan is:
// Process console command line or command line supplied script
// read in each line (or command) and process that (if appropriate)
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Module Definition & Files  
// 
//

#include "CCmdFile.h" 

//-----------------------------------------------------------------------------

CCmdFile *CCmdFile::NewL( )
{
CCmdFile* self = NewLC( );
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CCmdFile *CCmdFile::NewLC()
{
CCmdFile* self = new (ELeave) CCmdFile();
CleanupStack::PushL(self);
self->ConstructL( );
return self;
}

//-----------------------------------------------------------------------------

CCmdFile::~CCmdFile()
{
Close();
iArgs->ResetAndDestroy();
delete iArgs;
iArgs = NULL;
}

//-----------------------------------------------------------------------------

void CCmdFile::ConstructL()
{
iFileName.Zero();
iBuf.Set( NULL, 0 );
iLine = 0;
iArgs = new (ELeave) CArrayPtrSeg<HBufC>(32);
}

//-----------------------------------------------------------------------------
//	Reads the whole file into memory
//	Not necessarily ideal - really want to read in line by line as required...

TInt CCmdFile::Open(const TDesC& aPath, const TDesC& aFile)
{
// Construct full file name and open the file.
TInt error = iParse.Set( aFile, NULL, &aPath );
if (error == KErrNone) 
	error = Open(iParse.FullName());
return error;
}

//-----------------------------------------------------------------------------

TInt CCmdFile::Open(const TDesC& aFile)
{
// Close if file is open. Note: the Close() resets the member data.
Close();

// Connect to file server.
RFs fs;
TInt error;
error = fs.Connect();
if ( error != KErrNone )
  return error;

// Open the file.
RFile file;
error = file.Open( fs, aFile, EFileStreamText|EFileRead );
if ( error != KErrNone )
  {
	fs.Close();
  return error;
  }

// Get file size.
TInt size;
error = file.Size( size );
if ( error != KErrNone )
  {
  file.Close();
  fs.Close();
  return error;
  }

// Tackle an empty file => nothing in the iBuf buffer.
if ( size == 0 )
  {
  file.Close();
  fs.Close();
  iFileName.Copy( aFile );
  return error; // = KErrNone
  }

// Allocate buffer, read in whole file and close it.
TUint8* buf = (TUint8*)User::Alloc( size );
if ( buf == NULL )
  // No memory.
  {
  file.Close();
  fs.Close();
  return ( error = KErrNoMemory );
  }

TPtr8 ptr( buf, size );
error = file.Read( ptr );
file.Close();
fs.Close();
if ( error != KErrNone )
  // Read error.
  {
  delete buf;
  file.Close();
  fs.Close();
  return error;
  }

file.Close();   // Close file
fs.Close();     // Close session

#ifndef _UNICODE
// Set iBuf from which to give lines in Read.
iBuf.Set( (TText*)buf, size/sizeof(TText) );
#else
//	Tackle UNICODE and NON-UNICODE Text files.
//	the cast is necessary  in order to remove a warning.
//	It's safe because a size variable it's always positive
if ( (TUint)size >= sizeof(TText) && buf[0] == 0xFF && buf[1] == 0xFE ) 
  {
  // UNICODE: loose 0xFF,0xFE from beginning of the file.
  Mem::Move( buf, buf+sizeof(TText), size-sizeof(TText) );
  iBuf.Set( (TText*)buf, size/sizeof(TText)-1 );
  }
else
//  Remark: the size is known to be greater than zero here.
  {
  // NON-UNICODE: convert and replace the original buffer.
  // a) make new buffer
  TText* newbuf = (TText*)User::Alloc( size*sizeof(TText) );
  if ( newbuf == NULL )
      // No memory.
      {
      delete buf;
      return ( error = KErrNoMemory );
      }
  // b) convert from old to new buffer
  TInt i;
  for ( i = 0; i < size; i++ ) newbuf[i] = buf[i];
  // c) replace and delete the old one
  iBuf.Set( newbuf, size );
  delete buf;
  }
#endif
// Set file name, rewind and return Ok (error = KErrNone).
iFileName.Copy( aFile );
Rewind();
return error; // = KErrNone
}

//-----------------------------------------------------------------------------
//	reads a line in 

TInt CCmdFile::Read(TDes& aDes)
{

// No data.
aDes.Zero();

// Return End-of-file if there's nothing; also if no file open.
if ( iBuf.Ptr() == NULL || iBuf.Length() == 0 ) 
	return KErrEof;

// Return End-of-file also after giving everything from buffer.
iLex.Mark();
if (iLex.Eos()) 
	return KErrEof;

//	Otherwise eat characters up to the end of the line/file and hand over the characters into the descriptor. 
//	The following skips CRs and handles every LF as end-of-line and hopefully
//	works right with LF, CRLF and LFCR terminated lines. The CR alone will not do as the end-of-line.
TChar ch;
while ( !iLex.Eos() && ( ch = iLex.Get(), ch != '\n' ) )
  {
  if ( (ch != '\r') && (aDes.Length() < aDes.MaxLength()) )
		aDes.Append(ch);
  }

// Increment line number (read counter) and return KErrNone.
++iLine;
return KErrNone;
}

//-----------------------------------------------------------------------------

TInt CCmdFile::Rewind()
{
iLex  = iBuf;
iLine = 0;
return KErrNone;
}

//-----------------------------------------------------------------------------

void CCmdFile::Close()
{
//
// Reset member data.
if (iBuf.Ptr() != NULL) 
	delete (TText*)iBuf.Ptr();

iBuf.Set( NULL, 0 );
iLine = 0;
iFileName.Zero();
}

//-----------------------------------------------------------------------------
//	Add more arguments (makes copies). L E A V E S  on failure.

void CCmdFile::AddArgsL( const TDesC& aArgs )
{
TLex  parse(aArgs);
TPtrC argv;
while (argv.Set(TfrLex::GetL(parse)), argv.Length() > 0)
  {
  argv.Set(TfrLex::Peel(argv));
  HBufC* buf = argv.AllocL();
  iArgs->AppendL( buf );
  }
}

//-----------------------------------------------------------------------------
//	Get argument count.

TInt CCmdFile::Argc() const
{
return iArgs->Count();
}

//-----------------------------------------------------------------------------
//	get the i'th argument, legal i assumed.

TPtrC CCmdFile::Argv(TInt aIndex) const
{
TPtrC argv ((iArgs->At(aIndex))->Des());
return argv;
}

//-----------------------------------------------------------------------------
//	Get file name. Having no file does no harm (returns empty name).

TPtrC CCmdFile::FileName() const
{
TPtrC filename(iFileName);
return filename;
}

//-----------------------------------------------------------------------------
//	Get line number (= count of lines read so fat). Having no file does no
//	harm (returns line number 0).

TInt CCmdFile::Line()
{
return iLine;
}

//-----------------------------------------------------------------------------
