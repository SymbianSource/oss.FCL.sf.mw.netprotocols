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

#include "CAliasAddress.h"

CAliasAddress* CAliasAddress::NewL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress)
	{
	CAliasAddress* self = new (ELeave) CAliasAddress(aBearer, aPort);
	CleanupStack::PushL(self);
	self->ConstructL(aAddress);
	CleanupStack::Pop(self);
	return self;
	}

CAliasAddress::~CAliasAddress()
	{
	delete iAddress;
	}

CAliasAddress::CAliasAddress(TWspBearer aBearer, TUint16 aPort)
: CBase(), iBearer(aBearer), iPort(aPort)
	{
	}

void CAliasAddress::ConstructL(const TDesC8& aAddress)
	{
	iAddress = aAddress.AllocL();
	}
