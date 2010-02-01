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

#include "CWspPrimitiveSender.h"

#include <wsp/WspTypes.h>

#include <http/framework/cprotocolhandler.h>

#include "MWspPrimitiveSenderCallback.h"

const TInt KWspPrimitiveSenderActivePriority = KTransactionActivePriority;

CWspPrimitiveSender* CWspPrimitiveSender::NewL(MWspPrimitiveSenderCallback& aCallback)
	{
	return new (ELeave) CWspPrimitiveSender(aCallback);
	}

CWspPrimitiveSender::~CWspPrimitiveSender()
	{
	Cancel();
	}

CWspPrimitiveSender::CWspPrimitiveSender(MWspPrimitiveSenderCallback& aCallback)
: CActive(KWspPrimitiveSenderActivePriority), iCallback(aCallback)
	{
	CActiveScheduler::Add(this);
	}

void CWspPrimitiveSender::InitiateSend(TWspPrimitive aPrimitive)
	{
	// Cancel ourselves as this could be superceeding a previous request
	Cancel();

	// Need to set this object active and complete the request
	if (!IsActive())
		{
		iPrimitive = aPrimitive;
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrNone);
		SetActive();
		}
	}

void CWspPrimitiveSender::RunL()
	{
	// Get the callback object to send the WSP primitive
	iCallback.SendPrimitiveL(iPrimitive);
	}

TInt CWspPrimitiveSender::RunError(TInt aError)
	{
	// RunL() left - get callback object to handle this
	return iCallback.WspPrimitiveSenderCallbackError(aError);
	}

void CWspPrimitiveSender::DoCancel()
	{
	}
