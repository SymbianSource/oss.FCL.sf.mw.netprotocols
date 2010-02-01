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

#include "cmessagedatasupplier.h"

#include "mdatasupplierobserver.h"

CMessageDataSupplier* CMessageDataSupplier::NewL(MDataSupplierObserver& aObserver, const TDesC8& aData, TInt aBufferSize)
	{
	CMessageDataSupplier* self = new(ELeave) CMessageDataSupplier(aObserver, aData, aBufferSize);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMessageDataSupplier::~CMessageDataSupplier()
	{
	Cancel();

	delete iBuffer;
	}

CMessageDataSupplier::CMessageDataSupplier(MDataSupplierObserver& aObserver, const TDesC8& aData, TInt aBufferSize)
: CActive(CActive::EPriorityStandard), iObserver(aObserver), iRemainingData(aData), iBufferSize(aBufferSize)
	{
	CActiveScheduler::Add(this);
	}

void CMessageDataSupplier::ConstructL()
	{
	iBuffer = HBufC8::NewL(iBufferSize);

	SetData();
	}


TBool CMessageDataSupplier::GetData(TPtrC8& aData)
	{
	aData.Set(*iBuffer);

	return iLast;
	}

void CMessageDataSupplier::ReleaseData()
	{
	iBuffer->Des().Zero();

	if( !iLast )
		CompleteSelf();
	}

void CMessageDataSupplier::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CMessageDataSupplier::SetData()
	{
	// Need to set the new data...
	TInt dataLength = iBufferSize;

	if( dataLength > iRemainingData.Length() )
		{
		dataLength = iRemainingData.Length();
		}
	// Copy the data...
	iBuffer->Des().Copy(iRemainingData.Left(dataLength));

	// Update the remaining data
	iRemainingData.Set(iRemainingData.Mid(dataLength));

	// Is this the last part?
	iLast = iRemainingData.Length() == 0;
	}

/*
 *	Methods from CActive
 */

void CMessageDataSupplier::RunL()
	{
	// Set the data...
	SetData();

	// Notify the observer that the data is ready
	iObserver.DataReady();
	}

void CMessageDataSupplier::DoCancel()
	{
	// Do nothing...
	}


