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

#include "CWspUnknownCapabilities.h"

#include "CUnknownCapability.h"

CWspUnknownCapabilities* CWspUnknownCapabilities::NewL()
	{
	return new (ELeave) CWspUnknownCapabilities();
	}

CWspUnknownCapabilities::~CWspUnknownCapabilities()
	{
	iUnknownCapabilities.ResetAndDestroy();
	}

CWspUnknownCapabilities::CWspUnknownCapabilities()
: CBase(), iIndex(KErrNotFound)
	{
	}

TInt CWspUnknownCapabilities::Start() const
	{
	// Have any unknown capabilities been added?
	if( iUnknownCapabilities.Count() != 0 )
		{
		// There are unknown capabilities in array - set index to start
		iIndex = 0;
		}
	// iIndex now is the error (return) value
	return iIndex;
	}

TInt CWspUnknownCapabilities::GetNext(TPtrC8& aIdentifier, TPtrC8& aValue) const
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
		// Have still got an unknown capability to access - get values
		const CUnknownCapability& capability = *iUnknownCapabilities[iIndex];
		aIdentifier.Set(capability.GetIdentifier());
		aValue.Set(capability.GetValue());

		// Increment index and check to see reached the end of the array
		++iIndex;
		if( iIndex == iUnknownCapabilities.Count() )
			{
			iIndex = KErrNotFound;
			}
		}
	return error;
	}

void CWspUnknownCapabilities::Reset()
	{
	// Reset the array and index
	iUnknownCapabilities.ResetAndDestroy();
	iIndex = KErrNotFound;
	}

void CWspUnknownCapabilities::AddUnknownCapabilityL(const TDesC8& aIdentifier, const TDesC8& aValue)
	{
	// Create unknown capability object
	CUnknownCapability* capability = CUnknownCapability::NewL(aIdentifier, aValue);
	CleanupStack::PushL(capability);

	// Append to array
	User::LeaveIfError(iUnknownCapabilities.Append(capability));
	CleanupStack::Pop(capability);
	}

