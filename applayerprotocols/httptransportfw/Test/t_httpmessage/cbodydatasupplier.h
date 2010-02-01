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

#ifndef __CBODYDATASUPPLIER_H__
#define __CBODYDATASUPPLIER_H__

#include <e32base.h>
#include <badesca.h>
#include <http/mhttpdatasupplier.h>

class MBodyDataSupplierObserver;

class CBodyDataSupplier : public CActive,
						  public MHTTPDataSupplier
	{
public:	// methods

	static CBodyDataSupplier* NewL(MBodyDataSupplierObserver& aObserver);

	virtual ~CBodyDataSupplier();

	void AddBodyPartL(const TDesC8& aData);

	void SetBodySize(TInt aSize);

	TBool IsChunked() const;

	void ResetSupplier();

private:	// methods from CActive

	virtual void RunL();

	virtual void DoCancel();

private:	// methods from MHTTPDataSupplier

	virtual TBool GetNextDataPart(TPtrC8& aDataPart);

	virtual void ReleaseData();
	
	virtual TInt OverallDataSize();

	virtual TInt Reset();

private:	// enums

	enum TSupplierState
		{
		EIdle				=0,
		EPendingRelease,
		EPendingNotify,
		EDone
		};

private:	// methods

	CBodyDataSupplier(MBodyDataSupplierObserver& aObserver);

	void ConstructL();

	void CompleteSelf();

private:	// attributes

	MBodyDataSupplierObserver&	iObserver;

	CDesC8Array*				iBodyParts;

	TSupplierState				iState;

	TInt						iOverallDataSize;

	TBool						iIsChunked;

	TInt						iIndex;	

	};

#endif	// __CBODYDATASUPPLIER_H__
