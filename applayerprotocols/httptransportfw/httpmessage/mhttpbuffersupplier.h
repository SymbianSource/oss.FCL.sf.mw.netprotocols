// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __MHTTPBUFFERSUPPLIER_H__
#define __MHTTPBUFFERSUPPLIER_H__

#include <e32std.h>

class MHttpBufferSupplier
/**	
	The MHttpBufferSupplier class allows the caller to request that a supplied
	buffer be either reallocated or deleted.
	@internalComponent
*/
	{
public:	// methods

/**	
	Request to reallocate the line buffer.
	@param		aRequiredSize	The minimum size of buffer required.
	@param		aBuffer			An output argument set to the reallocated buffer.
*/
	virtual void ReAllocBufferL(TInt aRequiredSize, TPtr8& aBuffer) =0;

/**
	Request to delete the line buffer.
*/
	virtual void DeleteBuffer() =0;
	
	};

#endif	// __MHTTPBUFFERSUPPLIER_H__
