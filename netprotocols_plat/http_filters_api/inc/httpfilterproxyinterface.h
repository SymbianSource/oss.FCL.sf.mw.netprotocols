/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Proxy Filter 
*
*/


#ifndef CHTTPFILTERPROXYINTERFACE_H
#define CHTTPFILTERPROXYINTERFACE_H

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

/**
	@public
	@class			CHttpFilterProxyInterface
	@since			2.0
	@dependencies	CBase, REComSession, RHTTPSession
	@lib			euser.lib ecom.lib
	The ECOM interface definition for an HTTP proxy filter.  Implementations
	of a proxy filter must inherit this class.
 */
const TInt KProxyFilterUidValue = 0x101FD667;

const TUid KUidProxyFilter = { KProxyFilterUidValue };

class CHttpFilterProxyInterface : public CBase
{
public: // Methods
    /**
    @fn				InstallFilterL(RHTTPSession& aSession)
    Intended Usage:	This method is used to install the proxy filter to the filter queue.
    @since			2.0
    @param			aSession A handle to the transport session
    @pre 			The session had already been setup
    @post			The filter(s) have been installed
    */
    inline static void InstallFilterL( RHTTPSession& aSession );

    /**
    @fn				~CHttpProxyFilterInterface()
    Intended Usage:	Virtual destructor
    @since			2.0
    @pre 			The object identified by the destructor key in iEcomDtorID exists
    @post			The object is destroyed
    */
    inline ~CHttpFilterProxyInterface();

public:
    // The ECom destructor key identifier
    TUid iEcomDtorID;
private: // Attributes

};

//----------------------------------------------------------------------------------------

inline void CHttpFilterProxyInterface::InstallFilterL( RHTTPSession& aSession )
{
    REComSession::CreateImplementationL(KUidProxyFilter, _FOFF(CHttpFilterProxyInterface,iEcomDtorID), &aSession);
}

inline CHttpFilterProxyInterface::~CHttpFilterProxyInterface()
{
    REComSession::DestroyedImplementation(iEcomDtorID);
}

#endif // CHTTPFILTERPROXYINTERFACE_H
