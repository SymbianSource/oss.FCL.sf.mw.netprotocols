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

#ifndef __CUAPROFFILTERINTERFACE_H__
#define __CUAPROFFILTERINTERFACE_H__

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

// Forward declarations
class MHTTPCpiManager;


/**	The ECOM interface definition for an UAProf filter.  Implementations
	of a UAProf filter must inherit this class
 */
class CUAProfFilterInterface : public CBase
	{
public:
	
	/**			This method is used to install the UAProf filter to the filter queue
	@since		8.0
	@param		aSession A handle to the transport session
	@param		aCallback A pointer to the object implementing the callback function
	*/
	inline static void InstallFilterL(RHTTPSession& aSession, MHTTPCpiManager* aCallback);
	
	/**			Virtual destructor
	@since		8.0
	@pre 		The object identified by the destructor key in iEcomDtorID exists
	@post		The object is destroyed
	*/
	inline ~CUAProfFilterInterface();

public:
	struct TUAProfFilterParams
		{
		RHTTPSession* iSession;
		MHTTPCpiManager* iCallback;
		};

private:

	/**			The ECom destructor key identifier
	*/
	TUid iEcomDtorID;
	};
	
	
//-----------------------------------------------------------------------------


inline void CUAProfFilterInterface::InstallFilterL(RHTTPSession& aSession, MHTTPCpiManager* aCallback)
	{
	const TUid KUidUAProfFilter = {0x101F55EA};
	TUAProfFilterParams filterParams;
	filterParams.iSession = &aSession;
	filterParams.iCallback = aCallback;
	REComSession::CreateImplementationL(KUidUAProfFilter, 
										_FOFF(CUAProfFilterInterface, iEcomDtorID),
										&filterParams);
	}
	
inline CUAProfFilterInterface::~CUAProfFilterInterface()
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	} 	
	
#endif // __CUAPROFFILTERINTERFACE_H__