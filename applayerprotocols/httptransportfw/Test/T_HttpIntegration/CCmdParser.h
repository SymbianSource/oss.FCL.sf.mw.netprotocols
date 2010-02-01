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
// This header file defines the abstract base class CCmdPar.
// It is used for deriving different SET commands.
// 
//

#ifndef __CCMDPAR_H__
#define __CCMDPAR_H__

//-----------------------------------------------------------------------------
//  Include Files  

#include <e32base.h>
#include <e32cons.h>

//-----------------------------------------------------------------------------

//#include "TextCommands.h"
#include "TfrLex.h"
#include "TEngine.h"
#include "CCmdBase.h"
#include "CCmdFamily.h"

//----------------------------------------------------------------------------

class CCmdPar : public CCmdBase
{
public:     // Construction and destruction

	// No allocation because used only as a base type
	virtual ~CCmdPar();

protected:  // From CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase);
	CCmdPar() {};

public:     // From CCmdBase        

	TInt ProcessL( const TDesC& aCommand );

public:    // New functions       

	// Check and set the value given in this command
	virtual TInt SetValue(const TDesC& aValue) = 0;

	// Check and add the value given in this command
	// (by default gives an error message)
	virtual TInt AddValue(const TDesC& aValue);

	// Set the value by reading from a given file
	// (by default gives an error message)
	virtual TInt SetFrom(const TDesC& aValue); // +TVi

	// Return ETrue if the value has been set (or added) 
	// or marks that information as false
	TBool HasBeenSet();
	void MarkNotSet();

private:    // Data

	// Has the value been set
	TBool iHasBeenSet;
};

//----------------------------------------------------------------------------

#endif  // __CCMDPAR_H__  
  
//----------------------------------------------------------------------------
// End of File
//----------------------------------------------------------------------------
