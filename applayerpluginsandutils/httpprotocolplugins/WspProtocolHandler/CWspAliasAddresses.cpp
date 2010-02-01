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

#include "CWspAliasAddresses.h"

#include "CAliasAddress.h"

CWspAliasAddresses* CWspAliasAddresses::NewL()
	{
	return new (ELeave) CWspAliasAddresses();
	}

CWspAliasAddresses::~CWspAliasAddresses()
	{
	iAddresses.ResetAndDestroy();
	}

CWspAliasAddresses::CWspAliasAddresses()
: CBase(), iIndex(KErrNotFound)
	{
	}

TInt CWspAliasAddresses::Start() const
	{
	// Have any addresses been added?
	if( iAddresses.Count() != 0 )
		{
		// There are addresses in array - set index to start
		iIndex = 0;
		}
	// iIndex now is the error (return) value
	return iIndex;
	}

TInt CWspAliasAddresses::GetNext(TWspBearer& aBearer, TUint16& aPort, TPtrC8& aAddress) const
	{
	// Check to see if array can be accessed
	TInt error = KErrNone;
	if( iIndex == KErrNotFound )
		{
		// Array is empty or have indexed to end of array
		error = KErrNotFound;
		}
	else
		{
		// Have still got an address to access - get values
		const CAliasAddress& address = *iAddresses[iIndex];
		aBearer	= address.GetBearer();
		aPort	= address.GetPort();
		aAddress.Set(address.GetAddress());

		// Increment index and check to see reached the end of the array
		++iIndex;
		if( iIndex == iAddresses.Count() )
			{
			iIndex = KErrNotFound;
			}
		}
	return error;
	}

void CWspAliasAddresses::Reset()
	{
	// Reset the array and index
	iAddresses.ResetAndDestroy();
	iIndex = KErrNotFound;
	}

void CWspAliasAddresses::AddAddressL(TWspBearer aBearer, TUint16 aPort, const TDesC8& aAddress)
	{
	// Create address object
	CAliasAddress* address = CAliasAddress::NewL(aBearer, aPort, aAddress);
	CleanupStack::PushL(address);

	// Append to array
	User::LeaveIfError(iAddresses.Append(address));
	CleanupStack::Pop(address);
	}

