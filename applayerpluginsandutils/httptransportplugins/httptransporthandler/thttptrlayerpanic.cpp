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

#include "thttptrlayerpanic.h"

void THttpTrLayerPanic::Panic(THttpTrLayerPanicCode aPanic)
/**	
	The panic function for the http transport layer.
	@internalComponent
	@param		aPanic	The panic code.
*/
	{
	_LIT(KHttpTfHndPanic, "HTTPTH");
	User::Panic(KHttpTfHndPanic(), aPanic);
	}
