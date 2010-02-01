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
#include <http/rhttptransaction.h>// Class signature

// Local includes
#include "CTransaction.h"
#include "CRequest.h"
#include "CResponse.h"


//##ModelId=3A38B5E2015B
EXPORT_C void RHTTPTransaction::SendEventL(THTTPEvent aStatus, 
										   THTTPEvent::TDirection aDirection, 
										   THTTPFilterHandle aStart)
	{
	iImplementation->SendEventL(aStatus, aDirection, aStart);
	}

EXPORT_C TInt RHTTPTransaction::SendEvent(THTTPEvent aStatus, 
                                         THTTPEvent::TDirection aDirection, 
                                         THTTPFilterHandle aStart)
    {
    return iImplementation->SendEvent(aStatus, aDirection, aStart);
    }

//##ModelId=3A38F3AF0377
EXPORT_C RHTTPResponse RHTTPTransaction::Response() const
	{
	return iImplementation->Response().Handle();
	}

//##ModelId=3A38F3AF0377
EXPORT_C RHTTPRequest RHTTPTransaction::Request() const
	{
	return iImplementation->Request().Handle();
	}


//##ModelId=3A3F6695022C
EXPORT_C void RHTTPTransaction::Cancel(THTTPFilterHandle aStart)
	{
	// If we're already in sync mode we're either cancelling, closing
	// or failing. One way or another there's no point in another
	// cancel.
	if (iImplementation->SyncMode())
		return;

	iImplementation->CancelTransaction(aStart);
	}

EXPORT_C void RHTTPTransaction::Close()
	{
	if (iImplementation)
		{
		iImplementation->Close();
		iImplementation = NULL;
		}
	}

EXPORT_C void RHTTPTransaction::Fail(THTTPFilterHandle aStart 
									 /*= THTTPFilterHandle::ECurrentFilter*/)
	{
	// If we're already in sync mode we're either cancelling, closing
	// or failing. It doesn't make sense to fail in response to these
	// events.
	if (iImplementation->SyncMode())
		return;
	iImplementation->Fail(aStart);
	}

EXPORT_C void RHTTPTransaction::SubmitL(THTTPFilterHandle aStart)
	{
	SendEventL(THTTPEvent::ESubmit, THTTPEvent::EOutgoing, aStart);
	}

EXPORT_C TInt RHTTPTransaction::Submit(THTTPFilterHandle aStart)
    {
    return SendEvent(THTTPEvent::ESubmit, THTTPEvent::EOutgoing, aStart);
    }

EXPORT_C void RHTTPTransaction::NotifyNewRequestBodyPartL(THTTPFilterHandle aStart)
	{
	SendEventL(THTTPEvent::ENotifyNewRequestBodyPart, THTTPEvent::EOutgoing, aStart);
	}

EXPORT_C RHTTPSession RHTTPTransaction::Session() const 
	{
	return iImplementation->Session();
	}

EXPORT_C TInt RHTTPTransaction::Id() const
	{
	return iImplementation->Id();
	}

EXPORT_C RHTTPTransactionPropertySet RHTTPTransaction::PropertySet() const
	{
	return iImplementation->PropertySet();
	}

EXPORT_C TInt RHTTPTransaction::ServerCert(TCertInfo& aServerCert)
	{
	return iImplementation->ServerCert(aServerCert);
	}
	
EXPORT_C const CCertificate* RHTTPTransaction::ServerCert()
	{
	return iImplementation->ServerCert();
	}
	
EXPORT_C RString RHTTPTransaction::CipherSuite()
	{
	return iImplementation->CipherSuite();
	}

EXPORT_C void RHTTPTransaction::SetupHttpDataOptimiser(MHttpDataOptimiser& aHttpOptimiser)
 	{
 	iImplementation->SetupHttpDataOptimiser (aHttpOptimiser);
 	}
 
EXPORT_C MHttpDataOptimiser* RHTTPTransaction::HttpDataOptimiser()
 	{
 	return iImplementation->HttpDataOptimiser ();
 	}

