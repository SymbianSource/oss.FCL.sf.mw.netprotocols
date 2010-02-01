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
// CCommandDO.cpp
// This module implements the CDmpfile (dump output file) class.
// rev:	mjdavey, symbian@mjdss.com, July 2002
// for:	Typhoon (7.0s) & JetStream (8.0)
// Include Files  
// 
//

#include "CDumpFile.h"                    // This module

//-----------------------------------------------------------------------------

CDmpfile* CDmpfile::New( )
//Return Values:  Pointer to a new Dmpfile class object, NULL if fails.
{
CDmpfile* self = NULL;
TRAPD(error,self=NewL());
return ( error == KErrNone ? self : NULL );
}

//-----------------------------------------------------------------------------

CDmpfile* CDmpfile::NewL( )
{
CDmpfile* self = NewLC();
CleanupStack::Pop();
return self; 
}

//-----------------------------------------------------------------------------

CDmpfile* CDmpfile::NewLC(  )
{
CDmpfile* self = new (ELeave) CDmpfile();
CleanupStack::PushL(self);
self->ConstructL();
return self;
}

//-----------------------------------------------------------------------------

CDmpfile::~CDmpfile( )
{
Close();
}

//-----------------------------------------------------------------------------

void CDmpfile::ConstructL( )
{
iIsOpen  = EFalse;
iDoFlush = EFalse;
iError   = KErrNone;
iFileName.Zero();
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Open( const TDesC& aName )
//	Open existing file without replacing it. If it does not already exist,
//	create it.
{
Close();
iError = iParse.Set( aName, NULL, NULL );
if ( iError == KErrNone )
	{
	iFileName.Copy( iParse.FullName() );
	iError = DoOpen();
	}

return iError;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Open( const TDesC& aPath, const TDesC& aName )
{
Close();
iError = iParse.Set( aName, NULL, &aPath );
if ( iError == KErrNone )
	{
	iFileName.Copy( iParse.FullName() );
	iError = DoOpen();
	}
return iError;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Open()
{
Close();
return ( iError = DoOpen() );
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Replace( const TDesC& aName )
//	Open existing file replacing its contents. If it does not already exist,
//	create it.
{
Close();
iError = iParse.Set( aName, NULL, NULL );
if (iError == KErrNone )
	{
	iFileName.Copy( iParse.FullName() );
	iError = DoReplace();
	}
return iError;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Replace( const TDesC& aPath, const TDesC& aName )
{
Close();
iError = iParse.Set( aName, NULL, &aPath );
if ( iError == KErrNone )
	{
	iFileName.Copy( iParse.FullName() );
	iError = DoReplace();
	}
return iError;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Replace()
{
Close();
return ( iError = DoReplace() );
}

//-----------------------------------------------------------------------------

void CDmpfile::Close( )
//	Close file iff is open.
{
if ( iIsOpen )
	{
	iFile.Close();
	iFs.Close();
	iIsOpen = EFalse;
	}
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Write( const TDesC8& aDes )
{
iError = iFile.Write( aDes );
if ( iError == KErrNone && iDoFlush ) 
	iError = iFile.Flush();
return iError;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Write( const TDesC8& aDes, TInt aLength )
{
iError = iFile.Write( aDes, aLength );
if ( iError == KErrNone && iDoFlush ) 
	iError = iFile.Flush();
return iError;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Flush( )
{
return ( iError = iFile.Flush() );
}

//-----------------------------------------------------------------------------

void CDmpfile::SetFlush( TBool doFlush )
{
iDoFlush = doFlush;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Size( )
//	Get file size. Negative KErr* if fails.
{
TInt size;
iError = iFile.Size( size );
return ( iError == KErrNone ? size : iError );
}

//-----------------------------------------------------------------------------

TBool CDmpfile::IsOpen( )
{
return iIsOpen;
}

//-----------------------------------------------------------------------------

TPtrC CDmpfile::FileName( ) const
{
TPtrC filename( iFileName );
return filename;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::Error( )
{
return iError;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::DoOpen( )
{
TInt ret = iFs.Connect();
if ( ret != KErrNone ) 
	return ret;

ret = iFile.Open( iFs, iFileName, EFileWrite );
if ( ret == KErrNotFound ) 
	ret = iFile.Create( iFs, iFileName, EFileWrite );
if ( ret != KErrNone )
	{
	iFs.Close();
	return ret;
	}

TInt filepos;
ret = iFile.Seek( ESeekEnd, filepos );
if ( ret != KErrNone )
	{
	iFs.Close();
	iFile.Close();
	return ret;
	}

iIsOpen = ETrue;
return ret;
}

//-----------------------------------------------------------------------------

TInt CDmpfile::DoReplace( )
{
TInt ret = iFs.Connect();
if ( ret != KErrNone ) 
	return ret;

ret = iFile.Replace( iFs, iFileName, EFileWrite );
if ( ret != KErrNone )
	{
	iFs.Close();
	return ret;
	}

TInt filepos;
ret = iFile.Seek( ESeekEnd, filepos );
if ( ret != KErrNone )
	{
	iFs.Close();
	iFile.Close();
	return ret;
	}

iIsOpen = ETrue;
return ret;
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------

