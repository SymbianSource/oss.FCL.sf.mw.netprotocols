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

// System includes
#include <wsp/mwspcomethodinvoker.h>
#include <wsp/mwspcomethodcallback.h>
#include <wsp/mwspcosessioncallback.h>
#include <uri8.h>

// Local includes 
#include "cnwssconnectguard.h"

CNwssConnectGuard* CNwssConnectGuard::NewL(MWspCOMethodInvoker& aMethodInvoker, MWspCOSessionCallback& aSessionCallback)
	{
	CNwssConnectGuard* self = new (ELeave) CNwssConnectGuard(aMethodInvoker, aSessionCallback);
	return self;
	}

CNwssConnectGuard::~CNwssConnectGuard()
	{
	ResetMethodInfo();
	}

CNwssConnectGuard::CNwssConnectGuard(MWspCOMethodInvoker& aMethodInvoker, MWspCOSessionCallback& aSessionCallback)
: CActive(CActive::EPriorityStandard), iMethodInvoker(aMethodInvoker), iSessionCallback(aSessionCallback)
	{
	CActiveScheduler::Add(this);
	}

void CNwssConnectGuard::SendMethodInvokeReq()
	{
	// Check to see if object is active - this implies that T-MethodAbort.ind
	// is waiting to sent.
	if( !IsActive() && iMethodCallback )
		{
		// Send the T-MethodInvoke.req primitive.
		iMethodInvoker.MethodInvokeReq(
									  *iMethodCallback,
									  iMethod,
									  iRequestUri->Uri(),
									  *iRequestHeaders,
									  *iRequestBody,
									  EFalse
									  );
		// Release resources - no longer neeeded
		ResetMethodInfo();
		}
	}

void CNwssConnectGuard::SendDisconnectInd(Wap::TWspReason aReason)
	{
	// Specify that need to send S-Disconnect.ind
	iSendDisconnect = ETrue;

	// Store the reason code
	iDisconnectReason = aReason;

	CompleteSelf();
	}

void CNwssConnectGuard::SendMethodAbortInd()
	{
	CompleteSelf();
	}

void CNwssConnectGuard::ReceivedMethodInvokeReqL(
												MWspCOMethodCallback&	aMethodCallback,
												RStringF				aMethod,
												const TUriC8&			aRequestUri,
												const TDesC8&			aRequestHeaders,
												const TDesC8&			aRequestBody
												)
	{
	if( iMethodCallback )
		{
		// Have already got method info - leave as this is not allowed.
		User::Leave(KErrAlreadyExists);
		}
	iMethodCallback	= &aMethodCallback;
	iMethod			= aMethod.Copy();
	iRequestUri		= CUri8::NewL(aRequestUri);
	iRequestHeaders	= aRequestHeaders.AllocL();
	iRequestBody	= aRequestBody.AllocL();
	}

void CNwssConnectGuard::RunL()
	{
	// Check to see if there is any method info.
	if( iMethodCallback )
		{
		Wap::TWspReason reason = Wap::EUserReq;

		if( iSendDisconnect && iDisconnectReason != Wap::EUserReq )
			{
			// The session was disconnected due to a connect failure - could be
			// a WTLS failure, etc.
			reason = Wap::EConnectErr;
			}
		// Yep - send the T-MethodAbort.ind
		iMethodCallback->MethodAbortInd(reason);

		// Release resources - no longer neeeded
		ResetMethodInfo();
		}
	// Send S-Disconnect?
	if( iSendDisconnect )
		{
		TWspRedirectedAddress emptyAddr;
		iSessionCallback.DisconnectInd(iDisconnectReason, EFalse, emptyAddr, KNullDesC8(), KNullDesC8());
		}
	}

void CNwssConnectGuard::DoCancel()
	{
	}

void CNwssConnectGuard::CompleteSelf()
	{
	if (!IsActive())
		{
		TRequestStatus* pStat = &iStatus;
		User::RequestComplete(pStat, KErrNone);
		SetActive();
		}
	}

void CNwssConnectGuard::ResetMethodInfo()
	{
	iMethod.Close();
	iMethod = RStringF();
	delete iRequestUri;
	iRequestUri = NULL;
	delete iRequestHeaders;
	iRequestHeaders = NULL;
	delete iRequestBody;
	iRequestBody = NULL;
	}
