// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CUnknownCapability.h"

CUnknownCapability* CUnknownCapability::NewL(const TDesC8& aIdentifier, const TDesC8& aValue)
	{
	CUnknownCapability* self = new (ELeave) CUnknownCapability();
	CleanupStack::PushL(self);
	self->ConstructL(aIdentifier, aValue);
	CleanupStack::Pop(self);
	return self;
	}

CUnknownCapability::~CUnknownCapability()
	{
	delete iIdentifier;
	delete iValue;
	}

CUnknownCapability::CUnknownCapability()
: CBase()
	{
	}

void CUnknownCapability::ConstructL(const TDesC8& aIdentifier, const TDesC8& aValue)
	{
	iIdentifier = aIdentifier.AllocL();
	iValue = aValue.AllocL();
	}
