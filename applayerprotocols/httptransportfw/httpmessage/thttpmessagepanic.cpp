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

#include "thttpmessagepanic.h"

void THttpMessagePanic::Panic(THttpMessagePanicCode aPanic)
/**	
	The panic function for the http message component.
	@internalComponent
	@param		aPanic	The panic code.
*/
	{
	_LIT(KHttpMessagePanic, "HTTPMSG");
	User::Panic(KHttpMessagePanic(), aPanic);
	}
