/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Used to call back to httploader to cancel transaction if redirecting RTSP request
*
*/



#ifndef MIOPREDIRECTACTION_H
#define MIOPREDIRECTACTION_H

//  INCLUDES
#include <e32base.h>
#include <http/rhttptransaction.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*   Client Callback interface to encapsulate redirect in case of NOT http/https schemes(e.g. rtsp, ftp, etc.)
*/
class MIopRedirectCallback 
{
public:  // Constructors and destructor

    /**
    * This function willl callback to HttpLoader to cancel(async) transaction and pass URL to Scheme handler.
    * @param aUrl URL to redirect to
	* @param HTTP transaction
    **/

	IMPORT_C virtual void NonHttpRedirect(TDesC8& aUrl, RHTTPTransaction aTransaction) = 0;
									
};

#endif      // MIOPREDIRECTACTION_H

// End of File
