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

#ifndef PANIC_H
#define PANIC_H

#include <e32std.h>
#include "clientrequest.h"

enum TPanic
	{
	ESessionNotInitialised,
	ESessionAlreadyInitialised,
	ERepositoryNotFound,
	EStartAlreadyInTransaction,
	ECommitNotInTransaction
	};

GLREF_C void Panic(TPanic aReason);
GLREF_C void PanicClient(TPanic aReason, const TClientRequest& aMessage);

#endif
