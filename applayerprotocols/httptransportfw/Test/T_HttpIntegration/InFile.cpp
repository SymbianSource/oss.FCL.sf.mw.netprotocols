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
// 
//

#include "InFile.h"                 // This module

//-----------------------------------------------------------------------------
//	Gets a file. Leaves in case of any failure.
//-----------------------------------------------------------------------------

HBufC8* InFile::GetL( const TDesC& aFile )
{

// Connect to file server, Leave if fails.
RFs fs;
TInt r;
if ( r = fs.Connect(), r != KErrNone )
	User::Leave(r);

// Open the file for reading, Leave if fails.
RFile file;
if ( r = file.Open( fs, aFile, EFileRead ), r != KErrNone )
	{
	fs.Close();
	User::Leave(r);
	}

// Get file size, Leave if fails.
TInt size;
if ( r = file.Size( size ), r != KErrNone )
	{
	file.Close();
	fs.Close();
	User::Leave(r);
	}

// Allocate heap buffer long enough. Leave if fails.
HBufC8* hbuf = HBufC8::NewMax(size);
if ( hbuf == NULL )
	{
	file.Close();
	fs.Close();
	User::Leave(KErrNoMemory);
	}

// Read whole file if there's data.
if ( size > 0 )
	{
	TPtr8 temp(NULL,0);
	temp.Set(hbuf->Des());
	r = file.Read(temp);
	}

// Close file and disconnect from file server.
file.Close();
fs.Close();

// Delete the buffer and leave if read failed. 
if ( r != KErrNone )
	{
	delete hbuf;
	User::Leave(r);
	}

// Successful, return pointer to heap buffer.
return hbuf;
}

//-----------------------------------------------------------------------------

HBufC8* InFile::GetL( const TDesC& aPath, const TDesC& aFile )
{
TParse parse;
TInt r = parse.Set( aFile, NULL, &aPath );
if ( r != KErrNone ) 
	User::Leave(r);
return GetL( parse.FullName() );
}

//-----------------------------------------------------------------------------
//  End of File  
//-----------------------------------------------------------------------------
