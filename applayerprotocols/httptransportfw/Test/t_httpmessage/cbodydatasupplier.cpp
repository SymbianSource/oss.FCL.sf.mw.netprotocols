// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cbodydatasupplier.h"

#include "mbodydatasupplierobserver.h"

const TInt KGranularity	= 5;

CBodyDataSupplier* CBodyDataSupplier::NewL(MBodyDataSupplierObserver& aObserver)
	{
	CBodyDataSupplier* self = new (ELeave) CBodyDataSupplier(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CBodyDataSupplier::~CBodyDataSupplier()
	{
	if( iBodyParts )
		iBodyParts->Reset();
	delete iBodyParts;
	}

CBodyDataSupplier::CBodyDataSupplier(MBodyDataSupplierObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

void CBodyDataSupplier::ConstructL()
	{
	iBodyParts = new (ELeave) CDesC8ArrayFlat(KGranularity);
	}

void CBodyDataSupplier::AddBodyPartL(const TDesC8& aData)
	{
	iBodyParts->AppendL(aData);

	if( iState == EIdle )
		{
		iState = EPendingRelease;
		}
	}

void CBodyDataSupplier::SetBodySize(TInt aSize)
	{
	iOverallDataSize = aSize;
	}

TBool CBodyDataSupplier::IsChunked() const
	{
	return iOverallDataSize == KErrNotFound;
	}

void CBodyDataSupplier::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CBodyDataSupplier::ResetSupplier()
	{
	Cancel();
	iState = EPendingRelease;
	iIndex = 0;
	}

/*
 *	Methods from CActive
 */

void CBodyDataSupplier::RunL()
	{
	// Move to the EPendingRelease state.
	iState = EPendingRelease;

	// Notify the observer
	iObserver.NotifyMoreData();
	}

void CBodyDataSupplier::DoCancel()
	{
	// Do nothing...
	}

/*
 *	Methods from MHTTPDataSupplier
 */

TBool CBodyDataSupplier::GetNextDataPart(TPtrC8& aDataPart)
	{
	__ASSERT_DEBUG( iState == EPendingRelease, User::Invariant() );

	aDataPart.Set(iBodyParts->MdcaPoint(iIndex));

	return ( iIndex == iBodyParts->MdcaCount()-1 );
	}

void CBodyDataSupplier::ReleaseData()
	{
	// Move onto the next part
	++iIndex;

	// Have all the parts been released?
	if( iIndex < iBodyParts->MdcaCount() )
		{
		// More to go - self-complete to notify observer and move to the 
		// EPendingNotify state.
		iState = EPendingNotify;
		CompleteSelf();
		}
	else
		{
		// All done - move to EDone state
		iState = EDone;
		}	
	}
	
TInt CBodyDataSupplier::OverallDataSize()
	{
	if( iIsChunked )
		return KErrNotFound;
	else
		return iOverallDataSize;
	}

TInt CBodyDataSupplier::Reset()
	{
	return KErrNotFound;
	}


