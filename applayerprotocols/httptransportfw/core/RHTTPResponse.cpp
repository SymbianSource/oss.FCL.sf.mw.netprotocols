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
#include <http/rhttpresponse.h>// Class signature


// Local includes
#include "CResponse.h"
#include "CTransaction.h"


EXPORT_C void RHTTPResponse::SetStatusCode(TInt aStatus)
	{
	static_cast<CResponse*>(iImplementation)->Status() = aStatus;
	}
EXPORT_C TInt RHTTPResponse::StatusCode() const
	{
	return static_cast<CResponse*>(iImplementation)->Status();
	}
EXPORT_C void RHTTPResponse::SetStatusText(RStringF aStatusString)
	{
	RStringF& statusString = 
		static_cast<CResponse*>(iImplementation)->StatusString();
	statusString.Close();
	statusString = aStatusString.Copy();
	}
EXPORT_C RStringF RHTTPResponse::StatusText() const
	{
	return iImplementation->iTransaction->Session().StringPool().
		StringF(static_cast<CResponse*>(iImplementation)->StatusString());
	}
EXPORT_C void RHTTPResponse::SetVersion(TVersion aVersion)
	{
	static_cast<CResponse*>(iImplementation)->Version() = aVersion;
	}
EXPORT_C TVersion RHTTPResponse::Version() const
	{
	return static_cast<CResponse*>(iImplementation)->Version();
	}
