// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <http/rhttpmessage.h>// Class signature


// Local includes
#include "CMessage.h"


EXPORT_C RHTTPHeaders RHTTPMessage::GetHeaderCollection()
	{
	return iImplementation->iHeaders->Handle();
	}

EXPORT_C void RHTTPMessage::SetBody(MHTTPDataSupplier& aBody)
	{
	iImplementation->AddBody(&aBody);
	}

EXPORT_C void RHTTPMessage::RemoveBody()
	{
	iImplementation->AddBody(NULL);
	}

EXPORT_C TBool RHTTPMessage::HasBody() const
	{
	return iImplementation->HasBody();
	}

EXPORT_C MHTTPDataSupplier* RHTTPMessage::Body() const
	{
	return iImplementation->Body();
	}

