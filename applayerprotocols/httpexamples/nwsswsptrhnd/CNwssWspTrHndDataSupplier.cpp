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
#include <wapcli.h>

// Local includes
#include "tnwsswsptrhndpanic.h"
#include "testoom.h"

// Class signature
#include "cnwsswsptrhnddatasupplier.h"

// Constants used in this file
#define LOGDIR "http"
#define LOGFILE "nwsswsptrhnd.txt"

CNwssWspTrHndDataSupplier* CNwssWspTrHndDataSupplier::NewL(RWSPCOTrans& aStackTrans, MNwssOomHandler& aOomHandler)
	{
	CNwssWspTrHndDataSupplier* me = new(ELeave)CNwssWspTrHndDataSupplier(aStackTrans, aOomHandler);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

CNwssWspTrHndDataSupplier::~CNwssWspTrHndDataSupplier()
	{
	__CLOSELOG
	}

TBool CNwssWspTrHndDataSupplier::GetNextDataPart(TPtrC8& aDataPart)
	{
	// Set the data buffer
	aDataPart.Set(iBuffer);

	return (iRemainingBytes == 0);
	}

void CNwssWspTrHndDataSupplier::ReleaseData()
	{
	// If more data is expected, then extract the next chunk from the WAP Stack
	if (iRemainingBytes > 0)
		{
		// Extract the next chunk of data from the transaction, handling OOM appropriately
		// should it occur
		if (ExtractBodyData() == KErrNoMemory)
			{
			// Ensure the buffer is empty (should the client call again to GetNextDataPart after
			// releasing), then mimic a method abort.  No stack abort to occur since this can
			// only execute following a MethodResult-ind
			iBuffer.Zero();
			iRemainingBytes = 0;
			iOomHandler.SendOomMethodAbort(iStackTransId, EFalse);
			}
		}
	}

TInt CNwssWspTrHndDataSupplier::OverallDataSize()
	{
	return iOverallDataSize;
	}

TInt CNwssWspTrHndDataSupplier::Reset()
	{
	return KErrNotSupported;
	}

CNwssWspTrHndDataSupplier::CNwssWspTrHndDataSupplier(RWSPCOTrans& aStackTrans, MNwssOomHandler& aOomHandler)
	: iStackTrans(aStackTrans), iOomHandler(aOomHandler)
	{
	__OPENLOG(LOGDIR, LOGFILE)
	}

void CNwssWspTrHndDataSupplier::ConstructL()
	{
	// Store the transaction ID for later
	__TESTOOMD(stkErr, iStackTrans.Id(iStackTransId));
	__LOG1("--RWSPCOTrans::Id() returned %d", stkErr)
	User::LeaveIfError(stkErr);

	// Extract the first chunk of data from the transaction.
	User::LeaveIfError( ExtractBodyData() );

	// Calculate the overall data size
	iOverallDataSize = iBuffer.Length() + iRemainingBytes;

	__LOG1("--Overall data size is %d", iOverallDataSize)
	}

TInt CNwssWspTrHndDataSupplier::ExtractBodyData()
	{
	__TESTOOMD(stkErr, iStackTrans.GetData(iBuffer, RWSPCOTrans::EResultBody, &iRemainingBytes));
	__LOG1("--RWSPCOTrans::GetData(EResultBody) returned %d", stkErr)
	if ((stkErr != KErrNone) && (stkErr != RWAPConn::EMoreData))
		return stkErr;

	__LOG("--Result Body is:")
	__DUMP("--", "--", iBuffer)
	__LOG1("--Remaining bytes to extract is %d", iRemainingBytes)
	return KErrNone;
	}
