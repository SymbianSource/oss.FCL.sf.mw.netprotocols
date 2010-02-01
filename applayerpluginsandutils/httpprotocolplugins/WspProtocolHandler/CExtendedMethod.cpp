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

#include "CExtendedMethod.h"

CExtendedMethod* CExtendedMethod::NewL(TUint8 aPDUType, const TDesC8& aMethodName)
	{
	CExtendedMethod* self = new (ELeave) CExtendedMethod(aPDUType);
	CleanupStack::PushL(self);
	self->ConstructL(aMethodName);
	CleanupStack::Pop(self);
	return self;
	}

CExtendedMethod::~CExtendedMethod()
	{
	delete iName;
	}

CExtendedMethod::CExtendedMethod(TUint8 aPDUType)
: CBase(), iType(aPDUType)
	{
	}

void CExtendedMethod::ConstructL(const TDesC8& aMethodName)
	{
	iName = aMethodName.AllocL();
	}
