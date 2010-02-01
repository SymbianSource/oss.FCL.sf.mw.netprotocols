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

// System includes
#include <e32std.h>
#include <http/framework/mrxdataobserver.h>
#include <http/framework/crxdata.h>// Class signature


// Local includes


// D'tor
EXPORT_C
CRxData::~CRxData()
	{
	}

// Obtain the transaction to which this class corresponds 
EXPORT_C
CProtTransaction& CRxData::ProtTrans()
	{
	return *iProtTrans;
	}


EXPORT_C CRxData::CRxData(CProtTransaction& aProtTrans, MRxDataObserver& aObserver) :
	iProtTrans(&aProtTrans), iObserver(&aObserver)
	{}


// Default c'tor
EXPORT_C
CRxData::CRxData()
	{
	//Reset();
	}

// Second phase construction
EXPORT_C
void CRxData::ConstructL()
	{
	}
