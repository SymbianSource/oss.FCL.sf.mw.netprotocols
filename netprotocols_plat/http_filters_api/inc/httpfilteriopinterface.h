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
* Description:  HTTP Connection Handler filter
*
*/


#ifndef CHTTPFILTERIOPINTERFACE_H
#define CHTTPFILTERIOPINTERFACE_H

// System includes
#include <e32base.h>
#include <ecom/ecom.h>
#include <http/rhttpsession.h>

/**
	@public
	@class			CHttpFilterIopInterface
	@since			2.0
	@dependencies	CBase, REComSession, RHTTPSession
	@lib			euser.lib ecom.lib
	The ECOM interface definition for an HTTP Iop filter.  Implementations
	of a Iop filter must inherit this class.
 */

const TInt KIopFilterUidValue = 0x101F8591;
const TUid KUidIopFilter = { KIopFilterUidValue };

class MIopRedirectCallback;

/**
 * Iop params
 */
typedef enum TIopOptions{
    iopOptionHostHeader = 0x1, 
	iopOptionRedirectAction = 0x2
}TIopOptions;

typedef struct
{
    RHTTPSession* iSession;
	MIopRedirectCallback* iIopRedirectCallback;
	TUint iIopCapabilities;
}
TIopParams;

class CHttpFilterIopInterface : public CBase
{
public: // Methods
    /**
    @fn				InstallFilterL(RHTTPSession& aSession)
    Intended Usage:	This method is used to install the Iop filter to the filter queue.
    @since			2.0
    @param			aSession A handle to the transport session
	@param			aIopOption Iop Option to use, will be defaulted to iopOptionHostHeader
    @pre 			The session had already been setup
    @post			The filter(s) have been installed
    */
    inline static void InstallFilterL( RHTTPSession& aSession, TIopOptions aIopOption );

    /**
    @fn				InstallFilterL(RHTTPSession& aSession)
    Intended Usage:	This method is used to install the Iop filter to the filter queue.
    @since			3.0
    @param			aSession A handle to the transport session
	@param			aIopRedirectCallback Redirect Callback
	@param			aIopCapabilities Capabilities requested from IOP filter.
    @pre 			The session had already been setup
    @post			The filter(s) have been installed
    */
    inline static void InstallFilterL( RHTTPSession& aSession,  MIopRedirectCallback* aIopRedirectCallback, TUint aIopCapabilities );

    /**
    @fn				~CHttpIopFilterInterface()
    Intended Usage:	Virtual destructor
    @since			2.0
    @pre 			The object identified by the destructor key in iEcomDtorID exists
    @post			The object is destroyed
    */
    inline ~CHttpFilterIopInterface();

public:
    // The ECom destructor key identifier
    TUid iEcomDtorID;
private: // Attributes

};

//----------------------------------------------------------------------------------------
inline void CHttpFilterIopInterface::InstallFilterL( RHTTPSession& aSession, TIopOptions aIopOption)
{
    TIopParams iopParams;
    iopParams.iSession = &aSession;
	iopParams.iIopRedirectCallback = (MIopRedirectCallback*) NULL;
    iopParams.iIopCapabilities = (TUint) aIopOption; //for backward compatibility, will be set to iopOptionHostHeader
    REComSession::CreateImplementationL(KUidIopFilter, _FOFF(CHttpFilterIopInterface,iEcomDtorID), &iopParams);
}

inline void CHttpFilterIopInterface::InstallFilterL( RHTTPSession& aSession, MIopRedirectCallback* aIopRedirectCallback, TUint aIopCapabilities)
{
    TIopParams iopParams;
    iopParams.iSession = &aSession;
	iopParams.iIopRedirectCallback = aIopRedirectCallback;
    iopParams.iIopCapabilities = aIopCapabilities;
    REComSession::CreateImplementationL(KUidIopFilter, _FOFF(CHttpFilterIopInterface,iEcomDtorID), &iopParams);
}

inline CHttpFilterIopInterface::~CHttpFilterIopInterface()
{
    REComSession::DestroyedImplementation(iEcomDtorID);
}

#endif // CHTTPFILTERIOPINTERFACE_H
