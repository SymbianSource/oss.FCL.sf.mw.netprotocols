// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "thttpconnectcallback.h"

#include <http/mhttpdatasupplier.h>

#include "mhttpconnectobserver.h"


THttpConnectCallback::THttpConnectCallback(MHttpConnectObserver& aObserver)
: iObserver(aObserver)
	{
	}

/*
 *	Methods from MHTTPTransactionCallback
 */

void THttpConnectCallback::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch( aEvent.iStatus )
		{
	case THTTPEvent::EGotResponseHeaders:
		{
		// @todo could have a callback that notifies someone of CONNECT response headers...
		} break;
	case THTTPEvent::EResponseComplete:
		{
		// Inform the observer that this CONNECT transaction has succeeded.
		iObserver.ConnectCompleteL(aTransaction.Id());
		} break;
	case THTTPEvent::EGotResponseBodyData:
		{
		// Just release the body data, as it is of no use!
		MHTTPDataSupplier* data = aTransaction.Response().Body();
		data->ReleaseData();
		} break;
	case THTTPEvent::ERedirectRequiresConfirmation:
	case THTTPEvent::ENeedTunnel:
	case THTTPEvent::EGotResponseTrailerHeaders:
		{
		// Should not be receiving these events
		User::Invariant();
		} break;
	case THTTPEvent::EFailed:
		{
		// Inform the observer that the CONNECT transaction has failed.
		iObserver.ConnectErrorL(aTransaction.Id());	
		} break;

	case THTTPEvent::ESucceeded:
	case THTTPEvent::EUnrecoverableError:
	default:
		// Do nothing - Succeeded handled by response complete, failed handled by Failed States
		break;
		};
	}

TInt THttpConnectCallback::MHFRunError(TInt /*aError*/, RHTTPTransaction aTransaction, const THTTPEvent& /*aEvent*/)
	{
	iObserver.FailPendingTransactions(aTransaction.Id());
	return KErrNone;
	}
