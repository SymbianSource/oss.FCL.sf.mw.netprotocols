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
#include <httperr.h>
#include <http/framework/ctxdata.h>// Class signature


// Local includes


// D'tor
EXPORT_C
CTxData::~CTxData()
	{
	}

// Obtain the transaction to which this class corresponds 
EXPORT_C
CProtTransaction& CTxData::ProtTrans()
	{
	__ASSERT_ALWAYS(iProtTrans, HTTPPanic::Panic(HTTPPanic::EInvalidTxData));
	return *iProtTrans;
	}


// Normal constructor
EXPORT_C CTxData::CTxData(CProtTransaction& aProtTransaction) :
	iProtTrans(&aProtTransaction)
	{}


// Default constructor, no transaction used,
EXPORT_C CTxData::CTxData()
	{
	}

// Default 2nd phase construction, used when a transaction has been specified in NewL()
EXPORT_C
void CTxData::ConstructL()
	{
	}
