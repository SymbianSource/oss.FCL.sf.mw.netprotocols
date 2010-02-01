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

#include "CWspExtendedMethods.h"

#include "CExtendedMethod.h"

CWspExtendedMethods* CWspExtendedMethods::NewL()
	{
	return new (ELeave) CWspExtendedMethods();
	}

CWspExtendedMethods::~CWspExtendedMethods()
	{
	iMethods.ResetAndDestroy();
	}

CWspExtendedMethods::CWspExtendedMethods()
: CBase(), iIndex(KErrNotFound)
	{
	}

TInt CWspExtendedMethods::Start() const
	{
	// Have any methods been added?
	if( iMethods.Count() != 0 )
		{
		// There are methods in array - set index to start
		iIndex = 0;
		}
	// iIndex now is the error (return) value
	return iIndex;
	}

TInt CWspExtendedMethods::GetNext(TUint8& aPDUType, TPtrC8& aMethodName) const
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
		// Have still got a method to access - get values
		const CExtendedMethod& method = *iMethods[iIndex];
		aPDUType = method.GetPDUType();
		aMethodName.Set(method.GetMethodName());

		// Increment index and check to see reached the end of the array
		++iIndex;
		if( iIndex == iMethods.Count() )
			{
			iIndex = KErrNotFound;
			}
		}
	return error;
	}

void CWspExtendedMethods::Reset()
	{
	// Reset the array and index
	iMethods.ResetAndDestroy();
	iIndex = KErrNotFound;
	}

void CWspExtendedMethods::AddExtendedMethodL(TUint8 aPDUType, const TDesC8& aMethodName)
	{
	// Create method object
	CExtendedMethod* method = CExtendedMethod::NewL(aPDUType, aMethodName);
	CleanupStack::PushL(method);

	// Append to array
	User::LeaveIfError(iMethods.Append(method));
	CleanupStack::Pop(method);
	}

