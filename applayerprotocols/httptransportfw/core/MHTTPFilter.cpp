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
#include <http/thttpevent.h>
#include <http/rhttptransaction.h>
#include <http/rhttpsession.h>
#include <http/mhttpfilter.h>// Class signature


// Local includes


/** Most Filters will be either a transaction filter or a session event filter. Default 
	implementations of all MHTTPFilterBase functions are provided so filter implementors only have
	to override the functions they need
*/

EXPORT_C void MHTTPFilterBase::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{}


EXPORT_C void MHTTPFilterBase::MHFSessionRunL(const THTTPSessionEvent& /*aEvent*/)
	{}


EXPORT_C TInt MHTTPFilterBase::MHFRunError(TInt aError, 
										   RHTTPTransaction /*aTransaction*/,
										   const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}


EXPORT_C TInt MHTTPFilterBase::MHFSessionRunError(TInt aError, const THTTPSessionEvent& /*aEvent*/)
	{
	return aError;
	}


EXPORT_C void MHTTPFilter::MHFUnload(RHTTPSession, THTTPFilterHandle)
	{
	}


EXPORT_C void MHTTPFilter::MHFLoad(RHTTPSession, THTTPFilterHandle)
	{
	}



