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

#include "CWspHeaderCodePages.h"

#include "CHeaderCodePage.h"

CWspHeaderCodePages* CWspHeaderCodePages::NewL()
	{
	return new (ELeave) CWspHeaderCodePages();
	}

CWspHeaderCodePages::~CWspHeaderCodePages()
	{
	iCodePages.ResetAndDestroy();
	}

CWspHeaderCodePages::CWspHeaderCodePages()
: CBase(), iIndex(KErrNotFound)
	{
	}

TInt CWspHeaderCodePages::Start() const
	{
	// Have any code pages been added?
	if( iCodePages.Count() != 0 )
		{
		// There are code pages in array - set index to start
		iIndex = 0;
		}
	// iIndex now is the error (return) value
	return iIndex;
	}

TInt CWspHeaderCodePages::GetNext(TUint8& aPageCode, TPtrC8& aPageName) const
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
		// Have still got a code page to access - get values
		const CHeaderCodePage& codePage = *iCodePages[iIndex];
		aPageCode = codePage.GetPageCode();
		aPageName.Set(codePage.GetPageName());

		// Increment index and check to see reached the end of the array
		++iIndex;
		if( iIndex == iCodePages.Count() )
			{
			iIndex = KErrNotFound;
			}
		}
	return error;
	}

void CWspHeaderCodePages::Reset()
	{
	// Reset the array and index
	iCodePages.ResetAndDestroy();
	iIndex = KErrNotFound;
	}

void CWspHeaderCodePages::AddHeaderCodePageL(TUint8 aPageCode, const TDesC8& aPageName)
	{
	// Create code page object
	CHeaderCodePage* codePage = CHeaderCodePage::NewL(aPageCode, aPageName);
	CleanupStack::PushL(codePage);

	// Append to array
	User::LeaveIfError(iCodePages.Append(codePage));
	CleanupStack::Pop(codePage);
	}

