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

// System Includes
#include <httpstringconstants.h>
#include <http/rhttpheaders.h>

// User Includes
#include "uaproffilter.h"
#include <http/mhttpcpimanager.h>
#include "uaprofpanic.h"


CUAProfFilter* CUAProfFilter::InstallFilterL(TAny* aFilterParams)
/**	Installs the UAProf filter, supplying it with this callback interface
	@since		8.0
	@param		aSession the session to install the filter into.
	@leave	KErrNoMemory There was not enough memory.
*/
	{
	TUAProfFilterParams* filterParams = REINTERPRET_CAST(TUAProfFilterParams*, aFilterParams);
	CUAProfFilter* filter = new (ELeave) CUAProfFilter(*(filterParams->iSession), *(filterParams->iCallback));
	CleanupStack::PushL(filter);
	filter->ConstructL();
	CleanupStack::Pop(filter);
	return filter;
	}
	
CUAProfFilter::CUAProfFilter(RHTTPSession aSession, MHTTPCpiManager& aUAProfFilterCallback) 
	:	iStringPool(aSession.StringPool()),
		iSession(aSession),
		iCpiManager(aUAProfFilterCallback),
		iStringTable(RHTTPSession::GetTable())
/**	Standard constructor
	@since		8.0
*/
	{
	}
	
void CUAProfFilter::ConstructL()
/**	Standard second phase constructor
	@since		8.0
*/
	{
	iSession.FilterCollection().AddFilterL(*this, 
										   THTTPSessionEvent::EConnect,
										   RStringF(),
										   KAnyStatusCode,
										   MHTTPFilter::EUAProf,
										   iStringPool.StringF(HTTP::EUAProfFilter, iStringTable));
	}
	
CUAProfFilter::~CUAProfFilter()
/**	Standard destructor
	@since		8.0
*/
	{
	// If we've been destroyed from the cleanup stack during creation
	// of the object, it might still be loaded. So check. (Normally the
	// delete is initiated by the 'delete this' in MHFUnload)
	if (iLoadCount > 0)
		{
		// As we're already in a destructor, MHFUnload must not delete us again
		iLoadCount = -1;
		iSession.FilterCollection().RemoveFilter(iStringPool.StringF(HTTP::EUAProfFilter, iStringTable));
		}
	}
	
void CUAProfFilter::MHFLoad(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	++iLoadCount;
	}
	
void CUAProfFilter::MHFUnload(RHTTPSession /*aSession*/, THTTPFilterHandle /*aHandle*/)
	{
	if (--iLoadCount)
		return;
	
	delete this;
	}
	
void CUAProfFilter::MHFSessionRunL(const THTTPSessionEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPSessionEvent::EConnect:
			GetCpiAndAppendL();
			break;
		default:
			{
			if (aEvent.iStatus < 0)
				{
				UAProfPanic::Panic(UAProfPanic::EInvalidEventReceived);
				}
			break;
			}
		}
	}
	
TInt CUAProfFilter::MHFSessionRunError(TInt /*aError*/, const THTTPSessionEvent& /*aEvent*/)
	{
	iSession.FailSessionEvent();
	return KErrNone;
	}
	
void CUAProfFilter::MHFRunL(RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	}
	
TInt CUAProfFilter::MHFRunError(TInt aError,
								RHTTPTransaction /*aTransaction*/,
								const THTTPEvent& /*aEvent*/)
	{
	return aError;
	}
	
void CUAProfFilter::GetCpiAndAppendL()
/**	Obtains any Profile or Profile-Diff information discovered by the CPI Manager
	and then appends this data to the session header during a session connection
	request.
	@since		8.0
*/
	{
	RHTTPHeaders sessionHeaders = iSession.RequestSessionHeadersL();;
	RStringF profileField = iStringPool.StringF(HTTP::EProfile, iStringTable);

	// store the content of any Profile files discovered by the CPI Manager in an
	// RPointerArray of HBufC8s
	iProfiles = iCpiManager.GetProfiles();

	const TInt numProfiles = iProfiles.Count();
	for (TInt ii = 0; ii < numProfiles; ++ii)
		{
		RStringF profileValStr = iSession.StringPool().OpenFStringL((*iProfiles[ii]).Des());
		CleanupClosePushL(profileValStr);
		THTTPHdrVal profileVal(profileValStr);
		// Add the Profile name and value pair to the session header
		sessionHeaders.SetFieldL(profileField, profileVal);
		CleanupStack::PopAndDestroy(&profileValStr);
		}
	profileField.Close();
	
	// Profile-Diffs are only added to the session header if a Profile also exists
	if (numProfiles)
		{
		RStringF profileDiffField = iStringPool.StringF(HTTP::EProfileDiff, iStringTable);

		// store the content of any Profile-Diff files discovered by the CPI Manager in an 
		// RPointerArray of HBufC8s
		iProfileDiffs = iCpiManager.GetProfileDiffs();

		const TInt numProfileDiffs = iProfileDiffs.Count();
		for (TInt jj = 0; jj < numProfileDiffs; ++jj)
			{
			RStringF profileDiffValStr = iSession.StringPool().OpenFStringL((*iProfileDiffs[jj]).Des());
			CleanupClosePushL(profileDiffValStr);
			THTTPHdrVal profileDiffVal(profileDiffValStr);
			// Add the Profile-Diff name and value pair to the session header
			sessionHeaders.SetFieldL(profileDiffField, profileDiffVal);
			CleanupStack::PopAndDestroy(&profileDiffValStr);
			}
		profileDiffField.Close();
		}
	}

