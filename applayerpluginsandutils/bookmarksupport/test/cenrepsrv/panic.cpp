// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "panic.h"
#include <e32std.h>

GLDEF_C void Panic(TPanic aReason)
	{
	_LIT(KCategory, "CENREPSRV");
	User::Panic(KCategory, aReason);
	}

GLDEF_C void PanicClient(TPanic aReason, const TClientRequest& aMessage)
	{
	_LIT(KCategory, "CENREPCLI");
	aMessage.Panic(KCategory, aReason);
	}
