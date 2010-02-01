/*
* Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/


#ifndef	__COOKIEFILTERINTERFACE_H__
#define	__COOKIEFILTERINTERFACE_H__

// INCLUDE FILES
	// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

// CLASS DECLARATION

/**
	@public
	@class			CHttpCookieFilter
	@since			7.0
	@dependencies	CBase, REComSession, RHTTPSession
	@lib			euser.lib ecom.lib
	The ECOM interface definition for an HTTP cookie filter.  Implementations
	of a cookie filter must inherit this class.
 */

class CHttpCookieFilter : public CBase
	{
public: // Methods
	/**
	@fn				InstallFilterL( RHTTPSession& aSession )
	Intended Usage:	This method is used to install the cookie filter to the
	filter queue.
	@since			7.0
	@param			aSession A handle to the transport session
	@param			aCallback A pointer to the object implementing the
	callback function
	@pre 			The session had already been setup
	@post			The filter(s) have been installed
	*/
	inline static void InstallFilterL( RHTTPSession& aSession );

	/**
	@fn				~CHttpCookieFilter()
	Intended Usage:	Virtual destructor
	@since			7.0
	@pre 			The object identified by the destructor key in
	iEcomDtorID exists
	@post			The object is destroyed
	*/
	inline ~CHttpCookieFilter();

private: // Attributes
	/// The ECom destructor key identifier
	TUid iEcomDtorID;
	};

//------------------------------------------------------------------------

const TUid KCookieFilterUid = { 0x101F852D };

/**
	Factory function
*/
inline void CHttpCookieFilter::InstallFilterL( RHTTPSession& aSession )
	{
	REComSession::CreateImplementationL( KCookieFilterUid,
									_FOFF( CHttpCookieFilter, iEcomDtorID ),
									&aSession );
	}

/**
	Destructor
*/
inline CHttpCookieFilter::~CHttpCookieFilter()
	{
	REComSession::DestroyedImplementation(iEcomDtorID);
	}

#endif	// __COOKIEFILTERINTERFACE_H__