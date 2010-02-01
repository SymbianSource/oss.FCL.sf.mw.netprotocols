/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of Http filter for handling dynamic accept headers
*
*/



#ifndef HTTPFILTERACCEPTHEADERINTERFACE_H
#define HTTPFILTERACCEPTHEADERINTERFACE_H

//  INCLUDES
#include <http/mhttpfilter.h>
#include <ecom/ecom.h>


// CONSTANTS
const TInt KAcceptHeaderUIDValue = 0x101F8565;
const TUid KUidHttpFilterAcceptHeader = { KAcceptHeaderUIDValue };

/**
* Loading types. According to this parameter in the transaction
* properties, the filter adds the MIME types. E.g. ECss causes
* adding only Mime type "text/css".
*/
typedef enum
    {
    EAcceptAll,     // Add all MIME types to accept headers
    EAcceptImages,  // Add image types to accept headers (if any supported)
    EAcceptCss,     // Add text/css to accept header (if supported)
    EAcceptJavaScript, // Add application/x-javascript to accept header (if supported)
    EAcceptNpi,     // Add */* to the request. The plugin should handle it
    EAcceptSounds,  // Add sound types to accept headers (if any supported)
    EAcceptNone     // Add no new types to accept header.
    } ELoadTypes;


class CHttpFilterAcceptHeaderInterface: public CBase
	{
    public: // Methods

	    /**
	    @fn				InstallFilterL( RHTTPSession& aSession )
	    Intended Usage:	This method is used to install the cache filter to the filter queue.
	    @since			7.0
	    @param			aSession A handle to the transport session
	    @pre 			The session had already been setup
	    @post			The filter(s) have been installed
	    */
	    inline static void InstallFilterL( RHTTPSession& aSession );

	    /**
	    @fn				~CHttpCacheFilterInterface()
	    Intended Usage:	Virtual destructor
	    @since			7.0
	    @pre 			The object identified by the destructor key in iEcomDtorID exists
	    @post			The object is destroyed
	    */
	    inline ~CHttpFilterAcceptHeaderInterface();

    public: // Attributes

	    /// The ECom destructor key identifier
	    TUid iEcomDtorID;
	};

inline void CHttpFilterAcceptHeaderInterface::InstallFilterL( RHTTPSession& aSession )
	{
	REComSession::CreateImplementationL( 
        KUidHttpFilterAcceptHeader, 
        _FOFF( CHttpFilterAcceptHeaderInterface,iEcomDtorID ), 
        &aSession );
	}

inline CHttpFilterAcceptHeaderInterface::~CHttpFilterAcceptHeaderInterface()
	{
	REComSession::DestroyedImplementation( iEcomDtorID );
	}

#endif      // CHTTPFILTERACCEPTHEADERINTERFACE_H

// End of File
