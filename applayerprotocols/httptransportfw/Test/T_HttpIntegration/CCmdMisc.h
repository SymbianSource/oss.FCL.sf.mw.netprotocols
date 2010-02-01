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
// This header file defines miscellaneous CCmd* command classes,
// e.g. CCmdCase and CCmdSelect.
// 
//

#ifndef __CMDMISC_H__
#define __CMDMISC_H__

//-----------------------------------------------------------------------------

#include <e32base.h>
#include <e32cons.h>

//-----------------------------------------------------------------------------

#include "GT0149.h"                 //
//#include "TextCommands.h"
#include "TfrLex.h"
#include "ECommandIDs.h"
#include "TEngine.h"
#include "CCmdBase.h"                // Base class for commands
#include "CCmdFamily.h"              // Command set

//-----------------------------------------------------------------------------

class CCmdCase : public CCmdBase
{
public:     // Construction and destruction

	static CCmdCase* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdCase* NewLC( TInt aCommandId, const TDesC& aKeyphrase );
	virtual ~CCmdCase();

protected:  // From CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase);
	CCmdCase() { };

public:     // From CCmdBase        
	TBool Recognize( const TDesC& aCommand );
	TInt  ProcessL ( const TDesC& aCommand );
};

//-----------------------------------------------------------------------------

class CCmdEndCase : public CCmdBase
{
public:     // Construction and destruction

	static CCmdEndCase* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdEndCase* NewLC( TInt aCommandId, const TDesC& aKeyphrase );
	virtual ~CCmdEndCase();

protected:  // From CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase);
	CCmdEndCase() { };

public:     // From CCmdBase        

	TBool Recognize( const TDesC& aCommand );
	TInt  ProcessL ( const TDesC& aCommand );

};

//-----------------------------------------------------------------------------

class CCmdSelect : public CCmdBase
{
public:     // Construction and destruction

	static CCmdSelect* NewL ( TInt aCommandId, const TDesC& aKeyphrase );
	static CCmdSelect* NewLC( TInt aCommandId, const TDesC& aKeyphrase );
	virtual ~CCmdSelect();

protected:  // From CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase);
	CCmdSelect() { };

public:     // From CCmdBase        

	TInt ProcessL( const TDesC& aCommand );

public:     // New functions

	TPtrC Selector() const;
	TBool Selected( const TDesC& aCandidate );

private:    // Data

	HBufC*  iSelector;

};

//-----------------------------------------------------------------------------

#endif  // __CMDMISC_H__  

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------


