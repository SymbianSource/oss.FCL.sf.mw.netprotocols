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

#ifndef __CEVENTSENDER_H__
#define	__CEVENTSENDER_H__

#include <e32base.h>
#include <stringpool.h>
#include <uri8.h>

#include <http/rhttptransaction.h>

class CHTTPSession;
class MHTTPTransactionCallback;
class CProtocolHandler;

/** The CEventSender API is an interface object that sends events to the Protocol
	Handler of choice.

  */
class CEventSender : public CBase
	{
public:	// Methods

	IMPORT_C static CEventSender* NewL();

	IMPORT_C ~CEventSender();

	IMPORT_C void SessionConnectL();

	IMPORT_C void SessionDisconnectL();

	IMPORT_C RHTTPTransaction InvokeMethodL(MHTTPTransactionCallback& aCallback, const TUriC8& aUri, RStringF aMethod);

	IMPORT_C void AbortMethodL(RHTTPTransaction& aTransaction);

	IMPORT_C RHTTPSession GetHTTPSession() const;

	IMPORT_C void SetProtocolHandler(CProtocolHandler* aProtocolHandler);

private:	// Methods

	CEventSender();

	void ConstructL();

private:	// Attributes

	CHTTPSession*		iSession;

	CProtocolHandler*	iProtocolHandler;

	};

#endif	// __CEVENTSENDER_H__


