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

#ifndef	__CCOOKIEFILTERINTERFACE_H__
#define	__CCOOKIEFILTERINTERFACE_H__

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

// Forward declarations
class MHttpCookieManager;

/**
	@class			CCookieFilterInterface
	@since			7.0
	The ECOM interface definition for an HTTP cookie filter.  Implementations
	of a cookie filter must inherit this class.
 */

class CCookieFilterInterface : public CBase
	{
public: // Methods
	/**
	@fn				InstallFilterL(RHTTPSession& aSession, MHttpCookieManager* aCallback)
	Intended Usage:	This method is used to install the cookie filter to the filter queue.
	@since			7.0
	@param			aSession A handle to the transport session
	@param			aCallback A pointer to the object implementing the callback function
	@pre 			The session had already been setup
	@post			The filter(s) have been installed
	*/
	inline static void InstallFilterL(RHTTPSession& aSession, MHttpCookieManager* aCallback);

	/**
	@fn				~CCookieFilterInterface()
	Intended Usage:	Virtual destructor
	@since			7.0
	@pre 			The object identified by the destructor key in iEcomDtorID exists
	@post			The object is destroyed
	*/
	inline ~CCookieFilterInterface();

public:
	struct TCookieFilterParams
		{
		RHTTPSession* iSession;
		MHttpCookieManager* iCallback;
		};

private: // Attributes
	// The ECom destructor key identifier
	TUid iEcomDtorID;
	};

//----------------------------------------------------------------------------------------

inline void CCookieFilterInterface::InstallFilterL(RHTTPSession& aSession, MHttpCookieManager* aCallback)
	{
	const TUid KUidCookieFilter = {0x101F55E6};

	TCookieFilterParams filterParams;
	filterParams.iSession = &aSession;
	filterParams.iCallback = aCallback;
	REComSession::CreateImplementationL(KUidCookieFilter, 
										_FOFF(CCookieFilterInterface,iEcomDtorID), 
										&filterParams);
	}

inline CCookieFilterInterface::~CCookieFilterInterface()
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	}

#endif // __CCOOKIEFILTERINTERFACE_H__