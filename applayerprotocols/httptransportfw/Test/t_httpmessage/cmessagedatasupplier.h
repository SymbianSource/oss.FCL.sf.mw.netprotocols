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

#ifndef __CMESSAGEDATASUPPLIER_H__
#define __CMESSAGEDATASUPPLIER_H__

#include <e32base.h>

class MDataSupplierObserver;

class CMessageDataSupplier : public CActive
	{
public:	// methods

	static CMessageDataSupplier* NewL(MDataSupplierObserver& aObserver, const TDesC8& aData, TInt aBufferSize);

	virtual ~CMessageDataSupplier();

	TBool GetData(TPtrC8& aData);

	void ReleaseData();

private:	// methods from CActive

	virtual void RunL();

	virtual void DoCancel();

private:	// methods

	CMessageDataSupplier(MDataSupplierObserver& aObserver, const TDesC8& aData, TInt aBufferSize);

	void ConstructL();

	void CompleteSelf();

	void SetData();

private:	// attributes
	
	MDataSupplierObserver&	iObserver;	

	HBufC8*					iBuffer;

	TPtrC8					iRemainingData;

	TInt					iBufferSize;

	TBool					iLast;

	};

#endif	// __CMESSAGEDATASUPPLIER_H__
