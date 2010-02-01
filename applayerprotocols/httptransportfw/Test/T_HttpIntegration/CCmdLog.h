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
// CCmdLog..h
// The header file of the CCmdLog (log mode) command class.
// 
//

#ifndef __CCMDLOG_H__
#define __CCMDLOG_H__

//-----------------------------------------------------------------------------

#include <e32base.h>

//-----------------------------------------------------------------------------

#include "GT0149.h"
#include "ECommandIDs.h"
#include "TfrLex.h"
#include "CCmdBase.h"
#include "CCmdFamily.h"

//-----------------------------------------------------------------------------

class CCmdLog : public CCmdBase
{
public:

	static CCmdLog *NewL ( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);
	static CCmdLog *NewLC( TInt, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);

protected:  // from CCmdBase

	void ConstructL( TInt aCommandId, const TDesC& aKeyphrase, const TDesC& aHelpPhrase);

public:     // from CCmdBase

	TInt ProcessL ( const TDesC& aCommand );

};

//-----------------------------------------------------------------------------

#endif  // __CCMDLOG_H__

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------
