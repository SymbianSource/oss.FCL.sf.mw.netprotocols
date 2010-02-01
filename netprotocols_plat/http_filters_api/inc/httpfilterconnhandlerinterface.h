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
* Description:  ConnHandler Filter 
*
*/


#ifndef CHTTPFILTERCONNHANDLERINTERFACE_H
#define CHTTPFILTERCONNHANDLERINTERFACE_H

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

/**
	@public
	@class			CHttpFilterConnHandlerInterface
	@since			2.0
	@dependencies	CBase, REComSession, RHTTPSession
	@lib			euser.lib ecom.lib
	The ECOM interface definition for an HTTP ConnHandler filter.  Implementations
	of a ConnHandler filter must inherit this class.
 */

const TInt KConnHandlerFilterUidValue = 0x101F8584;
const TUid KUidConnHandlerFilter = { KConnHandlerFilterUidValue };

class MConnectionCallback;

/**
 * Cache params
 */
typedef struct
{
    RHTTPSession* iSession;
    MConnectionCallback* iConnCallback;
}
TConnParams;


class CHttpFilterConnHandlerInterface : public CBase
{
public: // Methods
    /**
    @fn				InstallFilterL(RHTTPSession& aSession)
    Intended Usage:	This method is used to install the ConnHandler filter to the filter queue.
    @since			2.0
    @param			aSession A handle to the transport session
    @pre 			The session had already been setup
    @post			The filter(s) have been installed
    */
    inline static void InstallFilterL( RHTTPSession& aSession, MConnectionCallback* aConnCallback );

    /**
    @fn				~CHttpConnHandlerFilterInterface()
    Intended Usage:	Virtual destructor
    @since			2.0
    @pre 			The object identified by the destructor key in iEcomDtorID exists
    @post			The object is destroyed
    */
    inline ~CHttpFilterConnHandlerInterface();

public:
    // The ECom destructor key identifier
    TUid iEcomDtorID;
private: // Attributes

};

//----------------------------------------------------------------------------------------

inline void CHttpFilterConnHandlerInterface::InstallFilterL( RHTTPSession& aSession, MConnectionCallback* aConnCallback)
{
    TConnParams connParams;
    connParams.iSession = &aSession;
    connParams.iConnCallback = aConnCallback;
    REComSession::CreateImplementationL(KUidConnHandlerFilter, _FOFF(CHttpFilterConnHandlerInterface,iEcomDtorID), &connParams);

}

inline CHttpFilterConnHandlerInterface::~CHttpFilterConnHandlerInterface()
{
    REComSession::DestroyedImplementation(iEcomDtorID);
}

#endif // CHTTPFILTERCONNHANDLERINTERFACE_H
