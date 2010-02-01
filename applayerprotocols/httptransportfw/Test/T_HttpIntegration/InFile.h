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
// GT0149 Applications Protocol Integration Test Harness
// blank application
// 
//

#ifndef __INFILE_H__
#define __INFILE_H__

//----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>
#include <e32std.h>
#include <s32file.h>
#include <f32file.h>
#include <e32hal.h>
#include <fbs.h>

//-----------------------------------------------------------------------------
//	InFile, static file input functions.
//-----------------------------------------------------------------------------

class InFile
{

public:     // Data types
protected:  // Data types
private:    // Data types
public:     // Constructors and destructors
private:    // Constructors and destructors
protected:  // Constructors and destructors
public:     // New functions

	// Get a file. Leaves in case of a fail.
	static HBufC8* GetL( const TDesC& aFile );
	static HBufC8* GetL( const TDesC& aPath, const TDesC& aFile );

public:     // From ?class_name
protected:  // Data types
protected:  // New functions
protected:  // From ?class_name
private:    // Data types
private:    // New functions
private:    // From ?class_name
public:     // Data
protected:  // Data
private:    // Data

// Friend classes

};

//-----------------------------------------------------------------------------

#endif  // __INFILE_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
