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

#ifndef __MTIMEOUTCALLBACK_H__
#define __MTIMEOUTCALLBACK_H__

//-----------------------------------------------------------------------------

#include <e32std.h>

//-----------------------------------------------------------------------------

class MTimeoutCallback
{
public:	// Methods

	virtual void Timeout() = 0;

};

//-----------------------------------------------------------------------------

#endif	// __MTIMEOUTCALLBACK_H__

//-----------------------------------------------------------------------------
//	End of File
//-----------------------------------------------------------------------------
